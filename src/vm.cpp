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
    // printExpr(expr, true);
    this->stack.push_back(expr);
    //std::cout << "stack push: " << stack.size() << std::endl;
}

Expr* VM::pop()
{
    Expr* e = this->stack.back();
    this->stack.pop_back();
    // std::cout << "stack pop: " << stack.size() << std::endl;
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
    std::cout << "do gc, threshold: " << this->gcThreshold << std::endl;
    markAll();
    std::cout << "markAll() done" << std::endl;
    sweep();
    std::cout << "sweep() done" << std::endl;

    this->gcThreshold = this->objectCount * 2;
}

void VM::sweep()
{
    this->freedPointers.clear();
    Expr** expr = &this->firstExpr;

    int i = 0;
    while (*expr)
    {
        std::cout << "sweep: " << i++;
        std::cout << " this : " << *expr;
        std::cout << " next : " << (*expr)->next << " ";
        std::cout << "next in set? "
                  << (this->freedPointers.find((*expr)->next) != this->freedPointers.end());
        //printExpr(*expr, false);
        //printExpr(*expr, true);
        if (!(*expr)->marked)
        {
            //std::cout << "not marked!" << std::endl;
            // this expr wasn't reached, remove it from list and free it
            Expr* unreached = *expr;
            *expr = unreached->next;

            this->free(unreached);
        }
        else
        {
            //std::cout << "marked!" << std::endl;
            // Expr was reached, so unmark it for next GC
            // and move to the next
            (*expr)->marked = false;
            expr = &(*expr)->next;
        }
        std::cout << std::endl;
        //std::cout << "Got to end!" << std::endl;
    }
}

void VM::free(Expr* expr)
{
    if (this->freedPointers.find(expr) != this->freedPointers.end())
    {
        return;
    }
    freedPointers.insert(expr);

    // std::cout << "free address" << expr << std::endl;
    switch (expr->type)
    {
    case ExprType::List:
    {
        auto contents = *expr->as.list.exprs;

        //std::cout << "free list stuff" << expr << std::endl;
        for (auto x : contents)
        {
            //std::cout << "list element" << x << std::endl;
            this->free(x);
        }

        delete expr->as.list.exprs;
        delete expr;

        break;
    }
    case ExprType::Number:
    case ExprType::Boolean:
    {
        delete expr;
        break;
    }
    case ExprType::Symbol:
    {
        delete expr->as.symbol.name;
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