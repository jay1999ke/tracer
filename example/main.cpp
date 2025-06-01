#include "tracer.h"

void foo() {
    tracer::print_stacktrace();
}

void bar() {
    foo();
}

int main() {
    bar();
    return 0;
}
