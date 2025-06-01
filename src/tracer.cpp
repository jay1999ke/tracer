#include "tracer.h"
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace tracer {

using Trace = std::tuple<uint64_t, std::string, std::string>;

void pretty_print_trace(Trace &trace) {
    std::cout << "[0x" << std::hex << std::get<0>(trace) << "]\t"
              << std::get<1>(trace) << " (" << std::get<2>(trace) << ")"
              << std::endl;
}

uint64_t get_rbp_content() {
    uint64_t content;
    asm volatile("mov %%rbp, %0" : "=r"(content));
    return content;
}

uint64_t get_value_at_address(uint64_t address) {
    uint64_t content;
    asm volatile(
        "mov (%1), %0" // Move the value at the address pointed to by %1 into %0
        : "=r"(content) // Output: content will store the value
        : "r"(address)  // Input: address is the pointer to the memory location
    );
    return content;
}

uint64_t get_callers_rip(uint64_t rbp) { return rbp + 8; }

std::optional<std::unique_ptr<Dl_info>>
get_caller_info_using_rbp(uint64_t rbp) {
    uint64_t rip = get_callers_rip(rbp);
    uint64_t caller_addr = get_value_at_address(rip);

    Dl_info info;
    if (dladdr(reinterpret_cast<void *>(caller_addr), &info)) {
        return std::make_unique<Dl_info>(std::move(info));
    } else {
        return {};
    }
}

void print_stacktrace(int skip) {
    uint64_t rbp = get_rbp_content();

    auto info = get_caller_info_using_rbp(rbp);
    if (info.has_value()) {
        // don't care about this function! (print_stacktrace)
    }

    std::vector<Trace> traces;

    uint64_t prev_rbp = rbp;
    while (info.has_value()) {
        rbp = prev_rbp;
        prev_rbp = get_value_at_address(rbp);
        info = get_caller_info_using_rbp(prev_rbp);
        if (info.has_value() && skip == 0) {
            traces.push_back({
                get_value_at_address(get_callers_rip(rbp)),
                std::string(info.value().get()->dli_sname),
                std::string(info.value().get()->dli_fname),
            });
        }
        if (skip > 0) {
            skip--;
        }
    }

    for (auto &trace : traces) {
        pretty_print_trace(trace);
    }
}
} // namespace tracer
