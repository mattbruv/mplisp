#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#include "env.h"

Environment::Environment()
{
    this->parent = nullptr;
    this->variables = std::map<std::string, Expr*>();
}

Expr* Environment::getVariable(std::string name)
{
    auto iter = variables.find(name);
    if (iter != variables.end())
    {
        return iter->second;
    }
    // Check in the parent environment up the chain
    if (this->parent != nullptr)
    {
        return this->parent->getVariable(name);
    }
    else
    {
        throw std::runtime_error("Unbound variable: '" + name + "'.");
    }
}
