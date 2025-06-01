#include "tracer.h"
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <iostream>

namespace tracer {
    std::vector<std::string> get_stacktrace(int skip) {
        void* callstack[128];
        int nFrames = backtrace(callstack, 128);
        char** symbols = backtrace_symbols(callstack, nFrames);

        std::vector<std::string> trace;
        for (int i = skip; i < nFrames; ++i) {
            trace.push_back(symbols[i]);
        }
        free(symbols);
        return trace;
    }

    void print_stacktrace(int skip) {
        auto trace = get_stacktrace(skip);
        for (const auto& frame : trace) {
            std::cout << frame << std::endl;
        }
    }
}
