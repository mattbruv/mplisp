#include <iostream>
#include <string>
#include <map>

#include "globals.h"
#include "eval.h"
#include "expr.h"

enum STDFunc
{
    ADD,
    SUB,
    MUL,
    DIV,
    LAMBDA,
    DEFINE,
    GREATERTHAN,
    QUOTE,
    IF,
};

auto StdMap = std::map<std::string, STDFunc>{
    { "+", STDFunc::ADD }, //
    { "-", STDFunc::SUB }, //
    { "*", STDFunc::MUL }, //
    { "/", STDFunc::DIV }, //
    { ">", STDFunc::GREATERTHAN }, //
    { "lambda", STDFunc::LAMBDA }, //
    { "define", STDFunc::DEFINE }, //
    { "quote", STDFunc::QUOTE }, //
    { "if", STDFunc::IF }, //
};

Expr* eval(Expr* expr, Environment* env)
{
    if (verbose())
    {
        std::cout << "evaluating: ";
        printExpr(expr, true);
    }

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
    case ExprType::Boolean:
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
    // ONLY if it's not a lambda function
    for (auto iter = list->begin(); iter != list->end(); iter++)
    {
        Expr* value = (*iter);

        auto test = std::string("lambda").compare(*value->as.symbol.name) == 0;
        if (test)
        {
            return expr;
        }

        // test to see if the first item in the list is a lambda.
        // if so, evaluate it...
        if (value->type == ExprType::List)
        {
            auto list = *value->as.list.exprs;

            if (list.size() > 0)
            {
                auto first = (*list.begin());
                auto test = std::string("lambda").compare(*first->as.symbol.name) == 0;

                if (first->type == ExprType::Symbol && test)
                {
                    return evalLambda(expr, env);
                }
            }
        }

        Expr* first = list->front();

        // if the first item is a symbol, it's a function call
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
                case STDFunc::LAMBDA:
                {
                    //std::cout << "lambda eval?" << std::endl;
                    return expr;
                }
                case STDFunc::GREATERTHAN:
                {
                    return funcGreaterThan(expr, env);
                }
                case STDFunc::IF:
                {
                    return evalIf(expr, env);
                }
                case STDFunc::DEFINE:
                {
                    return funcDefine(expr, env);
                }
                case STDFunc::QUOTE:
                {
                    auto iter = list->begin();
                    iter++;
                    return (*iter);
                    break;
                }
                default:
                {
                    throw std::runtime_error("No global function defined for: " +
                                             *first->as.symbol.name);
                    break;
                }
                }
            }
            // otherwise
            // it's a custom function call.
            else
            {
                auto func = env->getVariable(first);
                auto anon = new Expr();
                anon->type = ExprType::List;
                anon->as.list.exprs = new std::vector<Expr*>();
                anon->as.list.exprs->push_back(func);
                auto iter = list->begin();
                iter++;
                while (iter != list->end())
                {
                    anon->as.list.exprs->push_back((*iter++));
                }
                return evalLambda(anon, env);
            }
        }

        Expr* evaled = eval(value, env);
        // TODO: this would cause bugs, figure out how to manage memory
        //delete (*iter); // free memory at the old expression pointer
        (*iter) = evaled;
    };

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
        Expr* value = eval((*iter), env);

        if (value->type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + value->type);

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
    bool populated = false;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        Expr* value = eval((*iter), env);

        if (value->type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + value->type);

        if (!populated)
        {
            populated = true;
            sum = numberToDouble(value);
            continue;
        }

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
        Expr* value = eval((*iter), env);

        if (value->type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + value->type);

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
        Expr* value = eval((*iter), env);

        if (value->type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + value->type);

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

Expr* evalLambda(Expr* expr, Environment* env)
{
    // Create new environment for this lambda.
    Environment* closure = new Environment(env);

    auto list = *expr->as.list.exprs;
    auto iter = list.begin();

    auto function = (*iter);
    auto funcList = *function->as.list.exprs;
    auto funcIter = funcList.begin();
    funcIter++;

    auto funcArgs = (*funcIter);

    if (funcArgs->type != ExprType::List)
    {
        throw std::runtime_error("First expression in lambda must be a list of args!");
    }

    iter++;

    auto argsList = *funcArgs->as.list.exprs;
    auto funcArgsIter = argsList.begin();

    // apply all the arguments to this lambda.
    for (; funcArgsIter != argsList.end(); funcArgsIter++)
    {
        // get the next argument
        auto argSym = (*funcArgsIter);

        if (argSym->type != ExprType::Symbol)
        {
            throw std::runtime_error("Lambda argument definition must be a symbol!");
        }

        // make sure user actually gave an argument before processing it
        if (iter == list.end())
        {
            std::cout << "Missing argument '";
            printExpr(argSym, false);
            std::cout << "'" << std::endl;
            throw std::runtime_error("Too few arguments given to lambda!");
        }

        auto argument = eval((*iter++), env);

        // add this argument to the closure environment
        closure->variables[*argSym->as.symbol.name] = argument;
    }

    // Finally, eval body with new closure environment and return result
    funcIter++;
    auto body = (*funcIter);
    //std::cout << "body: ";
    //printExpr(body, true);
    auto result = eval(body, closure);
    //std::cout << "result: ";
    //printExpr(result, true);

    return result;
}

Expr* funcDefine(Expr* expr, Environment* env)
{
    auto list = *expr->as.list.exprs;
    auto iter = list.begin();
    iter++;

    auto var = (*iter);
    if (var->type != ExprType::Symbol)
    {
        throw std::runtime_error("Define argument is not a symbol!");
    }

    auto name = *var->as.symbol.name;

    if (env->isReservedWord(name))
    {
        throw std::runtime_error("Define argument cannot be a reserved word!");
    }

    iter++;
    auto value = eval((*iter), env);
    env->variables[name] = value;

    return value;
}

bool isExprTrue(Expr* expr)
{
    switch (expr->type)
    {
    case ExprType::Boolean:
    {
        return expr->as.boolean.value;
    }
    case ExprType::Number:
    {
        if (expr->as.number.isInt)
        {
            return expr->as.number.as.intValue != 0;
        }
        else
        {
            return expr->as.number.as.doubleValue != 0;
        }
    }
    default:
    {
        return true;
    }
    };
}

Expr* evalIf(Expr* expr, Environment* env)
{
    auto list = expr->as.list.exprs;
    auto iter = list->begin();
    iter++;

    if (iter == list->end())
        throw std::runtime_error("if statement missing condition");

    auto cond = (*iter++);

    if (iter == list->end())
        throw std::runtime_error("if statement missing true branch");

    auto branchTrue = (*iter++);

    if (iter == list->end())
        throw std::runtime_error("if statement missing false branch");

    auto branchFalse = (*iter++);

    if (isExprTrue(cond))
    {
        return eval(branchTrue, env);
    }
    else
    {
        return eval(branchFalse, env);
    }
}

Expr* funcGreaterThan(Expr* expr, Environment* env)
{
    auto list = expr->as.list.exprs;

    if (list->size() != 3)
    {
        throw std::runtime_error(">: Expected 2 arguments, got " + (list->size() - 1));
    }

    auto iter = list->begin();
    iter++;

    auto arg1 = (*iter++);
    auto arg2 = (*iter);

    if (arg1->type != ExprType::Number)
        throw std::runtime_error("> first argument not number!");

    if (arg2->type != ExprType::Number)
        throw std::runtime_error("> first argument not number!");

    Expr* result = new Expr();
    result->type = ExprType::Boolean;
    result->as.boolean.value = numberToDouble(arg1) > numberToDouble(arg2);
    return result;
}