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
    default:
    {
        throw std::runtime_error("No logic for evaluating expr type: " + expr->type);
        break;
    }
    }
}