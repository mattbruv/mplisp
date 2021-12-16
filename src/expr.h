#pragma once

#include <vector>
#include <string>

struct Expr;

enum ExprType
{
    Symbol,
    Number,
    Conditional,
    Definition,
    Procedure
};

struct ExprSymbol
{
    std::string* name;
};

struct ExprNumber
{
    bool isInt;
    union
    {
        int64_t intValue;
        double doubleValue;
    } as;
};

struct ExprConditional
{
    Expr* conditionTest;
    Expr* exprTrue;
    Expr* exprFalse;
};

struct ExprProcedure
{
    ExprSymbol* name;
    std::vector<ExprSymbol*> args;
    Expr* body;
};

struct ExprDefinition
{
    ExprSymbol* name;
    Expr* value;
};

struct Expr
{
    ExprType type;
    union
    {
        struct ExprSymbol symbol;
        struct ExprNumber number;
        struct ExprConditional conditional;
        struct ExprProcedure procedure;
        struct ExprDefinition definition;
    } as;
};