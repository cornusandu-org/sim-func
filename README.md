> [!NOTE]
> This repository has been archived as of 18th February 2026, and will remain read-only.
> This repository is licensed under the MIT License (© Copyright cornusandu 2026)

This is just a fun experiment I made because I was bored.

This code simulates the context and environment of a normal function, giving a block of code function-like control flow, as well as a *fully fabricated* `va_list`.

Usage example:
```cpp
stack_init();
va_list args;
stack_add_arg((uint64_t)(const char*)"Hello, world!");
if (function_call("test", args))
    MAKE_FUNC(
        printf("%s\n", va_arg(args, const char*));
        va_end(args);
    );
```
```out
Hello, world!
```

> [!NOTE]
> This is FULLY platform-dependant and will only work on Linux (or platforms with a SysV ABI). Additionally, this will only function with NO optimizations (`-O0`)
