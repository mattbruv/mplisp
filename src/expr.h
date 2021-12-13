#pragma once

#include <string>

enum ExprType
{
    Symbol,
    Number,
    Conditional,
    Definition,
    Procedure
};

struct SymbolExpr
{
    std::string* name;
};

struct NumberExpr
{
    bool isInt;
    union
    {
        int64_t intVal;
        double doubleVal;
    };
};

struct Expr
{
    ExprType type;
    union
    {
        struct SymbolExpr symbol;
        struct NumberExpr number;
    } as;
};