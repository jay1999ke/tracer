#include "tracer.h"
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace tracer {

int _backtrace(void *callstack, int16_t max_frame_count) { return 0; }

long get_rbp_content() {
    long content;
    asm volatile("mov %%rbp, %0" : "=r"(content));
    return content;
}

long get_value_at_address(long address) {
    long content;
    asm volatile(
        "mov (%1), %0" // Move the value at the address pointed to by %1 into %0
        : "=r"(content) // Output: content will store the value
        : "r"(address)  // Input: address is the pointer to the memory location
    );
    return content;
}

std::vector<std::string> get_stacktrace(int skip) {
    void *callstack[128];
    int nFrames = backtrace(callstack, 128);
    char **symbols = backtrace_symbols(callstack, nFrames);

    std::vector<std::string> trace;
    for (int i = skip; i < nFrames; ++i) {
        trace.push_back(std::string(symbols[i]));
    }
    free(symbols);
    return trace;
}

using namespace std;

std::optional<std::unique_ptr<Dl_info>> get_caller_info_using_rbp(long rbp) {
    long rip = rbp + 8;
    long caller_addr = get_value_at_address(rip);

    Dl_info info;
    if (dladdr(reinterpret_cast<void *>(caller_addr), &info)) {
        return std::make_unique<Dl_info>(std::move(info));
    } else {
        return {};
    }
}

void print_stacktrace(int skip) {
    long rbp = get_rbp_content();

    auto info = get_caller_info_using_rbp(rbp);
    if (info.has_value()) {
        // don't care about this function! print_stacktrace
    }

    std::vector<std::string> trace;

    long prev_rbp = rbp;
    while (info.has_value()) {
        rbp = prev_rbp;
        prev_rbp = get_value_at_address(rbp);
        info = get_caller_info_using_rbp(prev_rbp);
        if (info.has_value()) {
            trace.push_back(std::string(info.value().get()->dli_sname));
        }
    }

    for (auto t : trace) {
        std::cout << "func: " << t << std::endl;
    }
}
} // namespace tracer
