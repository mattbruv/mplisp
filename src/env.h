#pragma once

#include <map>
#include <set>
#include <string>

#include "expr.h"

class Environment
{
public:
    Environment(Environment* parent);
    Environment* parent;
    std::set<std::string> reservedSymbols;
    std::map<std::string, Expr*> variables;
    Expr* getVariable(Expr* sym);
};