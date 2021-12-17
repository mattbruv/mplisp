#pragma once

#include <map>
#include <string>

#include "expr.h"

class Environment
{
public:
    Environment();
    Environment* parent;
    std::map<std::string, Expr*> variables;
    Expr* getVariable(std::string name);
};