#include <iostream>

#include "vm.h"
#include "expr.h"

VM::VM()
{
    this->stack = std::vector<Expr*>();
}

void VM::push(Expr* expr)
{
    this->stack.push_back(expr);
}

Expr* VM::pop()
{
    Expr* e = this->stack.back();
    this->stack.pop_back();
    // popped expression needs to be 'delete'd afterwards,
    // or else there will be a memory leak
    return e;
}

size_t VM::size()
{
    return this->stack.size();
}

void VM::markAll()
{
    for (auto expr : this->stack)
    {
        this->mark(expr);
    }
}

void VM::mark(Expr* expr)
{
    if (expr->marked)
    {
        return;
    }

    expr->marked = true;

    // mark sub-expressions based on type :)
    switch (expr->type)
    {
    case ExprType::List:
    {
        auto contents = *expr->as.list.exprs;

        for (auto x : contents)
        {
            this->mark(x);
        }

        break;
    }
    case ExprType::Number:
    case ExprType::Boolean:
    case ExprType::Symbol:
    {
        break;
    }
    default:
    {
        throw std::runtime_error("No GC routine for expr of type " + expr->type);
    }
    }
}