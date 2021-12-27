#pragma once

#include <memory>
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
    std::shared_ptr<std::string> name;
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
    std::shared_ptr<Expr> exprTest;
    std::shared_ptr<Expr> exprIfTrue;
    std::shared_ptr<Expr> exprIfFalse;
};

struct ExprProcedure
{
    std::shared_ptr<Expr> name;
    std::vector<std::shared_ptr<Expr>> args;
    std::shared_ptr<Expr> body;
};

struct ExprDefinition
{
    std::shared_ptr<Expr> name;
    std::shared_ptr<Expr> value;
}; */

struct ExprList
{
    std::shared_ptr<std::vector<std::shared_ptr<Expr> > > exprs;
};

struct ExprBool
{
    bool value;
};

typedef struct Expr
{
    ExprType type;
    bool marked = false;
    std::shared_ptr<Expr> next = nullptr;

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
void printExpr(std::shared_ptr<Expr> expr, bool newline);