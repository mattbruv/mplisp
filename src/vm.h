#pragma once

#include <vector>
#include <set>

#include "expr.h"

// #define STACK_MAX 51200000 / sizeof(std::shared_ptr<Expr>)

class VM
{
public:
    VM();
    void push(std::shared_ptr<Expr> expr);
    std::shared_ptr<Expr> pop();
    size_t size();
    std::shared_ptr<Expr> newExpr(ExprType type);
    void markAll();
    void GC();

private:
    std::vector<std::shared_ptr<Expr> > stack;
    std::shared_ptr<Expr> firstExpr;
    void mark(std::shared_ptr<Expr> expr);
    void free(std::shared_ptr<Expr> expr);
    void sweep();

    std::set<std::shared_ptr<Expr> > freedPointers;
    int objectCount;
    int gcThreshold;
};
