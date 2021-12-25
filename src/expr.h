#pragma once

#include <vector>
#include <string>

struct Expr;

enum ExprType
{
    Symbol,
    Number,
    List,
    Boolean,
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

/*
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
}; */

struct ExprList
{
    std::vector<Expr*>* exprs;
};

struct ExprBool
{
    bool value;
};

typedef struct Expr
{
    ExprType type;
    bool marked = false;

    union as
    {
        struct ExprSymbol symbol;
        struct ExprNumber number;
        struct ExprList list;
        struct ExprBool boolean;

        // The compmiler can't create constructor
        // for unions with non-static data members
        as(){};
        ~as(){};
    } as;

    Expr(){};
    Expr(const Expr&){};
    //~Expr(){};
} Expr;

//Expr createSymbol(std::string name);
Expr createNumber(std::string value);
void printExpr(Expr* expr, bool newline);