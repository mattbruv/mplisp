#include <iostream>
#include <string>
#include <map>

#include "eval.h"
#include "expr.h"

enum STDFunc
{
    ADD,
    SUB,
    MUL,
    DIV,
};

auto StdMap = std::map<std::string, STDFunc>{
    { "+", STDFunc::ADD }, //
    { "-", STDFunc::SUB }, //
    { "*", STDFunc::MUL }, //
    { "/", STDFunc::DIV }, //
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
    for (auto iter = list->begin(); iter != list->end(); iter++)
    {
        Expr* evaled = eval((*iter), env);
        // TODO: this would cause bugs, figure out how to manage memory
        //delete (*iter); // free memory at the old expression pointer
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
            case STDFunc::SUB:
            {
                auto res = funcSub(expr, env);
                return res;
            }
            case STDFunc::MUL:
            {
                auto res = funcMul(expr, env);
                return res;
            }
            case STDFunc::DIV:
            {
                auto res = funcDiv(expr, env);
                return res;
            }
            default:
            {
                throw std::runtime_error("No global function defined for: " +
                                         *first->as.symbol.name);
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

Expr* funcSub(Expr* expr, Environment* env)
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

        sum -= numberToDouble(value);
    }

    result->type = ExprType::Number;
    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;
    return result;
}

Expr* funcMul(Expr* expr, Environment* env)
{
    Expr* result = new Expr();

    auto list = *expr->as.list.exprs;

    double sum = 1;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        auto type = (*iter)->type;
        Expr* value = *iter;

        if (type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + type);

        sum *= numberToDouble(value);
    }

    result->type = ExprType::Number;
    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;
    return result;
}

Expr* funcDiv(Expr* expr, Environment* env)
{
    Expr* result = new Expr();

    auto list = *expr->as.list.exprs;

    auto sum = 1;
    bool populated = false;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        auto type = (*iter)->type;
        Expr* value = *iter;

        if (type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + type);

        if (!populated)
        {
            populated = true;
            sum = numberToDouble(value);
            continue;
        }

        auto div = numberToDouble(value);
        if (div == 0)
            throw std::runtime_error("/: Divison by zero!");

        sum /= div;
    }

    result->type = ExprType::Number;
    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;
    return result;
}