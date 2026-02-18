#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <memory>
#include <variant>
#include <string>
#include <csetjmp>
#include <cstring>
#include <functional>
#include <stdarg.h>
#include <algorithm>

#include "defs.hpp"


#define UB() __attribute__((noinline, noclone, used, optimize("O0")))

#define MAKE_FUNC(x) \
    do {                \
        { x }        \
        revert_stack(); \
    } while (0);


struct _StackFrame {
    volatile bool destroyed = false;
    std::string function_name;
    std::jmp_buf return_state;

    UB()
    ~_StackFrame() {
        if (destroyed) return;
        destroyed = true;
        function_name.~basic_string();
    }
};

struct _Argument {
    uint64_t value;
};

struct alignas(16) StackItem {
    std::variant<_StackFrame, _Argument> value;
    std::shared_ptr<StackItem> next;
    StackItem* parent;

    _StackFrame* frame() {
        return std::get_if<_StackFrame>(&value);
    }

    _Argument* arg() {
        return std::get_if<_Argument>(&value);
    }
};

std::shared_ptr<StackItem> stack;

void stack_init() {
    stack = std::make_shared<StackItem>();
    stack->value.emplace<_StackFrame>();
    stack->frame()->function_name = "return";
    stack->parent = NULL;
    if (setjmp(stack->frame()->return_state)) {
        printf("Exiting with code 0.\n");
        exit(0);
    }
}

UB()
void revert_stack() {
    StackItem* last = stack.get();
    while (last->next != NULL) {
        last = last->next.get();
    }

    while (last->arg())
        last = last->parent;

    std::jmp_buf buf;
    memcpy(&buf, &last->frame()->return_state, sizeof(std::jmp_buf));
    last->parent->next.reset();
    asm volatile("" ::: "memory");
    longjmp(buf, 1);
}

void stack_add_arg(uint64_t value) {
    StackItem* last = stack.get();
    while (last->next != NULL) {
        last = last->next.get();
    }

    last->next = std::make_shared<StackItem>();

    last->next->value.emplace<_Argument>();
    last->next->arg()->value = value;
    last->next->parent = last;
}

UB()
int function_call(std::string name, va_list &args) {
    StackItem* last = stack.get();
    while (last->next != NULL) {
        last = last->next.get();
    }

    std::vector<uint64_t> argsv;

    if (last->arg()) {
        while (last->arg()) {
            argsv.push_back(last->arg()->value);
            last = last->parent;
        }
    }

    std::reverse(argsv.begin(), argsv.end());

    struct FakeVAList storage;
    uint64_t reg_save[6];
    std::vector<uint64_t> overflow;
    make_va_list_from_vector(argsv, args, storage, reg_save, overflow);

    last->next = std::make_shared<StackItem>();
    last->next->value.emplace<_StackFrame>();
    last->next->frame()->function_name = name;
    last->next->parent = last;
    int r = setjmp(last->next->frame()->return_state) != 1;
    asm volatile("" ::: "memory");
    return r;
}

int main() {
    stack_init();
    va_list args;
    stack_add_arg((uint64_t)(const char*)"Hello, world!");
    if (function_call("test", args))
        MAKE_FUNC(
            printf("%s\n", va_arg(args, const char*));
            va_end(args);
        );
}
