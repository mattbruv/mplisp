#include <iostream>
#include <iomanip>

#include "expr.h"

Expr createNumber(std::string value)
{
    Expr expr;
    expr.type = ExprType::Number;

    return expr;
}

void printExpr(Expr* expr, bool newline)
{
    // std::cout << "Expr type: " << expr->type << " ";
    switch (expr->type)
    {
    case ExprType::Number:
    {
        std::cout << ((expr->as.number.isInt) ? "int: " : "dbl: ");
        if (expr->as.number.isInt)
            std::cout << expr->as.number.as.intValue;
        else
            std::cout << std::setprecision(6) << expr->as.number.as.doubleValue;
        break;
    }
    case ExprType::Symbol:
    {
        std::cout << "sym: " << *expr->as.symbol.name;
        break;
    }
    case ExprType::List:
    {
        std::cout << "(";
        auto list = *expr->as.list.exprs;
        for (auto iter = list.begin(); iter != list.end(); iter++)
        {
            printExpr(*iter, false);
            if (std::next(iter, 1) != list.end())
                std::cout << ", ";
        }
        std::cout << ")";
        break;
    }
    default:
    {
        std::cout << "No print rule for Epxr type: " << expr->type;
        break;
    }
    }
    if (newline)
        std::cout << std::endl;
}