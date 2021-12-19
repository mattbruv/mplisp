#include <iostream>

#include "expr.h"

Expr createNumber(std::string value)
{
    Expr expr;
    expr.type = ExprType::Number;

    return expr;
}

void printExpr(Expr* expr)
{
    // std::cout << "Expr type: " << expr->type << " ";
    switch (expr->type)
    {
    case ExprType::Number:
    {
        std::cout << ((expr->as.number.isInt) ? "Integer: " : "Double: ");
        if (expr->as.number.isInt)
            std::cout << expr->as.number.as.intValue;
        else
            std::cout << expr->as.number.as.doubleValue;
        break;
    }
    case ExprType::Symbol:
    {
        std::cout << "Symbol: " << *expr->as.symbol.name;
        break;
    }
    case ExprType::Conditional:
    {
        std::cout << "Conditional: " << &expr;
        break;
    }
    default:
    {
        std::cout << "No print rule for Epxr type: " << expr->type;
        break;
    }
    }
    std::cout << std::endl;
}