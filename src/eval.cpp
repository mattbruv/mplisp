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
        return env->getVariable(expr);
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

    // Recursively evaluate all expressions in this list
    //   auto iter = list.begin();
    //    for (iter++; iter != list.end(); iter++)
    for (auto iter = list->begin(); iter != list->end(); iter++)
    {
        Expr* evaled = eval((*iter), env);
        (*iter) = evaled;
    };

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
                auto res = funcAdd(expr, env);
                return res;
                break;
            }
            default:
            {
                throw std::runtime_error("No global function defined for: " + func);
                break;
            }
            }
        }
    }

    return expr;
}

double numberToDouble(Expr* expr)
{
    if (expr->as.number.isInt)
    {
        return (double)expr->as.number.as.intValue;
    }
    return expr->as.number.as.doubleValue;
}

Expr* funcAdd(Expr* expr, Environment* env)
{
    Expr* result = new Expr();

    auto list = *expr->as.list.exprs;

    double sum = 0;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        auto type = (*iter)->type;
        Expr* value = *iter;

        if (type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + type);

        sum += numberToDouble(value);
    }

    result->type = ExprType::Number;
    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;
    return result;
}