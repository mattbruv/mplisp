#include <iostream>

#include "vm.h"
#include "expr.h"

VM::VM()
{
    this->stack = std::vector<Expr*>();
    this->firstExpr = nullptr;
    this->objectCount = 0;
    this->gcThreshold = 50000;
    std::set<Expr*> freedPointers;
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
    if (this->objectCount == this->gcThreshold)
    {
        this->GC();
    }

    Expr* expr = new Expr();
    expr->type = type;
    expr->marked = false;

    // Insert it into the list of allocated objects
    expr->next = this->firstExpr;
    this->firstExpr = expr;

    this->objectCount++;

    return expr;
}

void VM::GC()
{
    markAll();
    sweep();

    this->gcThreshold = this->objectCount * 2;
}

void VM::sweep()
{
    this->freedPointers.clear();
    Expr** expr = &this->firstExpr;

    //int i = 0;
    while (*expr)
    {
        //std::cout << "sweep: " << i++;
        //std::cout << " this : " << *expr;
        //std::cout << " next : " << (*expr)->next << " ";
        //printExpr(*expr, false);
        //std::cout << std::endl;
        //printExpr(*expr, true);
        if (!(*expr)->marked)
        {
            //std::cout << "not marked!" << std::endl;
            // this expr wasn't reached, remove it from list and free it
            Expr* unreached = *expr;
            *expr = unreached->next;

            if (this->freedPointers.find(*expr) != this->freedPointers.end())
            {
                this->free(unreached);
                freedPointers.insert(*expr);
            }
            //std::cout << "done with free" << std::endl;
        }
        else
        {
            //std::cout << "marked!" << std::endl;
            // Expr was reached, so unmark it for next GC
            // and move to the next
            (*expr)->marked = false;
            expr = &(*expr)->next;
        }
        //std::cout << "Got to end!" << std::endl;
    }
}

void VM::free(Expr* expr)
{
    std::cout << "free address" << expr << std::endl;
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

    this->objectCount--;
}

void VM::markAll()
{
    for (auto expr : this->stack)
    {
        this->mark(expr);
        //printExpr(expr, true);
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
            //printExpr(x, true);
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