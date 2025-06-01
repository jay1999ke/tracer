# Implementation of a simple backtrace library

Notes:
- How function calling works in x86: https://textbook.cs161.org/memory-safety/x86.html

## usage

```
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
```

`./main`:

```
trace@tracer: ~ $ ./main 
[0x562238a1da88]	_Z3foov (./main)
[0x562238a1d738]	_Z3barv (./main)
[0x562238a1d744]	main (./main)
[0x562238a1d750]	__libc_start_main (/lib/x86_64-linux-gnu/libc.so.6)

```