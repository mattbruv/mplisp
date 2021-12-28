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
    CAR,
    CDR,
    CONS,
    EMPTYCHECK,
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
    { "car", STDFunc::CAR }, //
    { "cdr", STDFunc::CDR }, //
    { "cons", STDFunc::CONS }, //
    { "empty?", STDFunc::EMPTYCHECK }, //
};

void check(size_t start, size_t end)
{
    if (end != start + 1)
    {
        throw std::runtime_error("ERROR WITH STACK SIZE");
    }
}

void eval(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    auto startSize = vm.size();

    if (verbose())
    {
        std::cout << "evaluating: ";
        printExpr(expr, true);
    }

    switch (expr->type)
    {
    case ExprType::Symbol:
    {
        vm.push(env->getVariable(expr));
        return;
    }
    case ExprType::Number:
    {
        vm.push(expr);
        return;
    }
    case ExprType::Boolean:
    {
        vm.push(expr);
        return;
    }
    case ExprType::List:
    {
        evalList(expr, env);
        auto endSize = vm.size();
        check(startSize, endSize);
        return;
    }
    default:
    {
        throw std::runtime_error("No logic for evaluating expr type: " + expr->type);
        break;
    }
    }
}

void evalList(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    auto list = expr->as.list.exprs;

    if (list->size() == 0)
    {
        // vm.push(expr);
        return;
    }

    // Recursively evaluate all expressions in this list
    // ONLY if it's not a lambda function
    for (auto iter = list->begin(); iter != list->end(); iter++)
    {
        std::shared_ptr<Expr> value = (*iter);

        auto test = std::string("lambda").compare(*value->as.symbol.name) == 0;
        if (test)
        {
            // vm.push(expr);
            return;
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
                    evalLambda(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
            }
        }

        std::shared_ptr<Expr> first = list->front();

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
                    funcAdd(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::SUB:
                {
                    funcSub(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::MUL:
                {
                    funcMul(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::DIV:
                {
                    funcDiv(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::LAMBDA:
                {
                    return;
                }
                case STDFunc::GREATERTHAN:
                {
                    funcGreaterThan(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::IF:
                {
                    evalIf(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::DEFINE:
                {
                    funcDefine(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::CAR:
                {
                    funcCar(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::CDR:
                {
                    funcCdr(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::CONS:
                {
                    funcCons(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
                }
                case STDFunc::QUOTE:
                {
                    auto iter = list->begin();
                    iter++;
                    vm.pop(); // expr
                    vm.push((*iter));
                    return;
                }
                case STDFunc::EMPTYCHECK:
                {
                    funcEmpty(expr, env);
                    auto result = vm.pop();
                    vm.pop(); // expr
                    vm.push(result);
                    return;
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
                // TODO: possibly improve this by mutating
                // and replacing the first symbol with the lambda

                // get the function
                auto func = env->getVariable(first);
                auto anon = vm.newExpr(ExprType::List);
                vm.push(anon);

                anon->as.list.exprs = new std::vector<std::shared_ptr<Expr> >();
                anon->as.list.exprs->push_back(func);

                auto iter = list->begin();
                iter++;

                while (iter != list->end())
                {
                    anon->as.list.exprs->push_back((*iter++));
                }

                evalLambda(anon, env);
                auto evalResult = vm.pop(); // pop eval result
                vm.pop(); // pop anon
                vm.pop(); // pop expr
                vm.push(evalResult); // re-add result
                return;
            }
        }

        eval(value, env);
        auto evaled = vm.pop();
        (*iter) = evaled;
    }

    return;
}

double numberToDouble(std::shared_ptr<Expr> expr)
{
    if (expr->as.number.isInt)
    {
        return (double)expr->as.number.as.intValue;
    }
    return expr->as.number.as.doubleValue;
}

void funcAdd(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::Number);

    auto list = *expr->as.list.exprs;

    double sum = 0;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        eval((*iter), env);
        auto value = vm.pop();

        if (value->type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + value->type);

        sum += numberToDouble(value);
    }

    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;

    vm.pop(); // expr
    vm.push(result);
}

void funcSub(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::Number);

    auto list = *expr->as.list.exprs;

    double sum = 0;
    bool populated = false;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        eval((*iter), env);
        auto value = vm.pop();

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

    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;

    vm.pop(); // expr
    vm.push(result);
}

void funcMul(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);
    std::shared_ptr<Expr> result = vm.newExpr(ExprType::Number); // new Expr();

    auto list = *expr->as.list.exprs;

    double sum = 1;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        eval((*iter), env);
        auto value = vm.pop();

        if (value->type != ExprType::Number)
            throw std::runtime_error("+: Invalid expression given: " + value->type);

        sum *= numberToDouble(value);
    }

    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;
    vm.pop(); // expr
    vm.push(result);
}

void funcDiv(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::Number);

    auto list = *expr->as.list.exprs;

    auto sum = 1;
    bool populated = false;
    auto iter = list.begin();

    for (iter++; iter != list.end(); iter++)
    {
        eval((*iter), env);
        auto value = vm.pop();

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

    result->as.number.isInt = false;
    result->as.number.as.doubleValue = sum;
    vm.pop(); // expr
    vm.push(result);
}

void evalLambda(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    // add this expression to the stack to save it
    vm.push(expr);

    // Create new environment for this lambda.
    std::shared_ptr<Environment> closure = std::make_shared<Environment>(); // new Environment(env);
    closure.get()->parent = env;

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

        eval((*iter++), env);
        auto argument = vm.pop();
        //auto argument = eval((*iter++), env);

        // add this argument to the closure environment
        closure->variables[*argSym->as.symbol.name] = argument;
    }

    // Finally, eval body with new closure environment and return result
    funcIter++;
    auto body = (*funcIter);
    //std::cout << "body: ";
    //printExpr(body, true);
    eval(body, closure);
    auto result = vm.pop();
    vm.pop(); // remove the original expr from stack
    vm.push(result); // add resulting expr to stack
}

void funcDefine(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

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
    eval((*iter), env);
    auto value = vm.pop();
    env->variables[name] = value;
    vm.pop(); // expr
    vm.push(value);
}

bool isExprTrue(std::shared_ptr<Expr> expr)
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

void evalIf(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    auto list = expr->as.list.exprs;
    auto iter = list->begin();
    iter++;

    if (iter == list->end())
        throw std::runtime_error("if statement missing condition");

    eval((*iter++), env);
    auto cond = vm.pop();

    if (iter == list->end())
        throw std::runtime_error("if statement missing true branch");

    auto branchTrue = (*iter++);

    if (iter == list->end())
        throw std::runtime_error("if statement missing false branch");

    auto branchFalse = (*iter++);

    if (isExprTrue(cond))
    {
        eval(branchTrue, env);
    }
    else
    {
        eval(branchFalse, env);
    }
    auto result = vm.pop();
    vm.pop(); // expr
    vm.push(result);
}

void funcGreaterThan(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    auto list = expr->as.list.exprs;

    if (list->size() != 3)
    {
        throw std::runtime_error(">: Expected 2 arguments, got " + (list->size() - 1));
    }

    auto iter = list->begin();
    iter++;

    eval((*iter++), env);
    auto arg1 = vm.pop();
    eval((*iter), env);
    auto arg2 = vm.pop();

    if (arg1->type != ExprType::Number)
        throw std::runtime_error("> first argument not number!");

    if (arg2->type != ExprType::Number)
        throw std::runtime_error("> first argument not number!");

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::Boolean);
    result->as.boolean.value = numberToDouble(arg1) > numberToDouble(arg2);

    vm.pop(); // expr
    vm.push(result);
}

void funcCar(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    if (expr->as.list.exprs->size() != 2)
        throw std::runtime_error("Expected 1 argument for car");

    auto iter = expr->as.list.exprs->begin();
    iter++;

    eval((*iter), env);
    auto arg = vm.pop();

    if (arg->type != ExprType::List)
        throw std::runtime_error("Cannot get car from item that is not a list");

    if (arg->as.list.exprs->size() == 0)
        throw std::runtime_error("Cannot get car of empty list");

    auto arg1 = arg->as.list.exprs->begin();

    vm.pop(); // expr
    vm.push((*arg1));
}

void funcCdr(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    if (expr->as.list.exprs->size() != 2)
        throw std::runtime_error("Expected 1 argument for cdr");

    auto iter = expr->as.list.exprs->begin();
    iter++;

    eval((*iter), env);
    auto arg = vm.pop();

    if (arg->type != ExprType::List)
        throw std::runtime_error("Cannot get cdr of an item that is not a list");

    if (arg->as.list.exprs->size() == 0)
        throw std::runtime_error("Cannot get cdr of empty list");

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::List);
    result->as.list.exprs = new std::vector<std::shared_ptr<Expr> >();

    auto list = *arg->as.list.exprs;
    auto it = list.begin();
    it++;

    for (; it != list.end(); it++)
    {
        result->as.list.exprs->push_back((*it));
    }

    vm.pop(); // expr
    vm.push(result);
}

void funcCons(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    if (expr->as.list.exprs->size() != 3)
        throw std::runtime_error("Expected 2 arguments for cons");

    auto iter = expr->as.list.exprs->begin();
    iter++;

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::List); // new Expr();
    // result->type = ExprType::List;
    result->as.list.exprs = new std::vector<std::shared_ptr<Expr> >();

    eval((*iter++), env);
    auto exprToAdd = vm.pop();

    eval((*iter), env);
    auto exprBase = vm.pop();

    // if list, add all items in list to new list
    if (exprBase->type == ExprType::List)
    {
        for (auto item : *exprBase->as.list.exprs)
        {
            result->as.list.exprs->push_back(item);
        }
    }
    else
    {
        result->as.list.exprs->push_back(exprBase);
    }
    // now add the first argument
    result->as.list.exprs->insert(result->as.list.exprs->begin(), exprToAdd);

    vm.pop(); // expr
    vm.push(result);
}

void funcEmpty(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env)
{
    vm.push(expr);

    auto list = expr->as.list.exprs;

    if (list->size() != 2)
        throw std::runtime_error("Expected 2 arguments for empty?");

    auto iter = list->begin();
    iter++;

    eval((*iter), env);
    auto val = vm.pop();

    if (val->type != ExprType::List)
        throw std::runtime_error("empty? argument is not a list!");

    std::shared_ptr<Expr> result = vm.newExpr(ExprType::Boolean);
    result->as.boolean.value = val->as.list.exprs->size() == 0;

    vm.pop(); // expr
    vm.push(result);
}