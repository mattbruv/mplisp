#include <iostream>
#include <string>
#include <map>

#include "eval.h"
#include "expr.h"

enum STDFunc
{
    ADD
};

auto StdMap = std::map<std::string, STDFunc>{
    { "+", STDFunc::ADD } //
};

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
        // If in global map
        auto search = StdMap.find(*first->as.symbol.name);
        if (search != StdMap.end())
        {
            auto func = StdMap[*first->as.symbol.name];
            switch (func)
            {
            case STDFunc::ADD:
            {
                funcAdd(expr, env);
                break;
            }
            default:
            {
                throw std::runtime_error("No global function defined for: " + func);
                break;
            }
            }
        }

        return expr;
    }

    return expr;
}

Expr* funcAdd(Expr* expr, Environment* env)
{
    Expr* result = new Expr();
    expr->type = ExprType::Number;

    int i = 0;
    auto list = *expr->as.list.exprs;

    for (auto iter = list.begin(); iter != list.end(); iter++)
    {
        std::cout << i << ", ";
    }
    return expr;
}