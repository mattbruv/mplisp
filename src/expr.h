#pragma once

#include <vector>
#include <string>

struct Expr;

enum ExprType
{
    Symbol,
    Conditional,
    Definition,
    Number,
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
    Expr* exprTest;
    Expr* exprIfTrue;
    Expr* exprIfFalse;
};

struct ExprProcedure
{
    Expr* name;
    std::vector<Expr*> args;
    Expr* body;
};

struct ExprDefinition
{
    Expr* name;
    Expr* value;
};

struct Expr
{
    ExprType type;
    union as
    {
        struct ExprSymbol symbol;
        struct ExprNumber number;
        struct ExprConditional conditional;
        struct ExprProcedure procedure;
        struct ExprDefinition definition;

        // The compmiler can't create constructor
        // for unions with non-static data members
        as(){};
        ~as(){};
    } as;
};
