#include <iostream>

#include "eval.h"
#include "expr.h"

Expr* eval(Expr* expr, Environment* env)
{
    switch (expr->type)
    {
    case ExprType::Symbol:
    {
        return env->getVariable(*expr->as.symbol.name);
        break;
    }
    case ExprType::Number:
    {
        return expr;
        break;
    }
    case ExprType::List:
    {
        return evalList(expr, env);
        break;
    }
    default:
    {
        throw std::runtime_error("No logic for evaluating expr type: " + expr->type);
        break;
    }
    }
}

Expr* evalList(Expr* expr, Environment* env)
{
    auto list = expr->as.list.exprs;

    if (list->size() == 0)
        return expr;

    // if the first item is a symbol, it's a function call
    Expr* first = list->front();

    if (first->type == ExprType::Symbol)
    {
        /*
        switch (*first->as.symbol.name)
            printExpr(first, true); */
    }

    return expr;
}