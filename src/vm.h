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
    Expr* newExpr(ExprType type);
    void markAll();

private:
    std::vector<Expr*> stack;
    Expr* firstExpr;
    void mark(Expr* expr);
    void free(Expr* expr);
    void sweep();
};

void foo();