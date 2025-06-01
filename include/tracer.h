#ifndef TRACER_H
#define TRACER_H

#include <string>
#include <vector>

namespace tracer {
    std::vector<std::string> get_stacktrace(int skip = 1);
    void print_stacktrace(int skip = 1);
}

#endif
