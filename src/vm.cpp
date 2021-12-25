#include <iostream>

#include "vm.h"
#include "expr.h"

VM::VM()
{
    this->stack = std::vector<Expr*>();
    this->firstExpr = nullptr;
    this->gcThreshold = 100000;
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

Expr* VM::newExpr(ExprType type)
{
    if ((int)this->size() == this->gcThreshold)
    {
        this->GC();
    }

    Expr* expr = new Expr();
    expr->type = type;
    expr->marked = false;

    // Insert it into the list of allocated objects
    expr->next = this->firstExpr;
    this->firstExpr = expr;

    return expr;
}

void VM::GC()
{
    std::cout << "GC called! threshold: " << this->gcThreshold << std::endl;

    markAll();
    sweep();

    this->gcThreshold = (int)this->size() * 2;
}

void VM::sweep()
{
    auto expr = &this->firstExpr;

    while (*expr)
    {
        if (!(*expr)->marked)
        {
            // this expr wasn't reached, remove it from list and free it
            Expr* unreached = *expr;
            *expr = unreached->next;
            this->free(unreached);
        }
        else
        {
            // Expr was reached, so unmark it for next GC
            // and move to the next
            (*expr)->marked = false;
            expr = &(*expr)->next;
        }
    }
}

void VM::free(Expr* expr)
{
    switch (expr->type)
    {
    case ExprType::List:
    {
        auto contents = *expr->as.list.exprs;

        for (auto x : contents)
        {
            this->free(x);
        }

        delete expr;
        break;
    }
    case ExprType::Number:
    case ExprType::Boolean:
    case ExprType::Symbol:
    {
        delete expr;
        break;
    }
    default:
    {
        throw std::runtime_error("No GC routine for expr of type " + expr->type);
    }
    }
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