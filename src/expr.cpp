#include "expr.h"

Expr createNumber(std::string value)
{
    Expr expr;
    expr.type = ExprType::Number;

    return expr;
}