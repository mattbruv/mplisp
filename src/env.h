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
    std::map<std::string, std::shared_ptr<Expr> > variables;
    std::shared_ptr<Expr> getVariable(std::shared_ptr<Expr> sym);
    bool isReservedWord(std::string key);
};