#pragma once

#include <vector>

#include "expr.h"

// #define STACK_MAX 51200000 / sizeof(Expr*)

class VM
{
public:
    VM();
    void push(Expr* expr);
    Expr* pop();
    size_t size();
    void markAll();

private:
    std::vector<Expr*> stack;
    void mark(Expr* expr);
};

void foo();