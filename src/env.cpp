#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#include "env.h"

Environment::Environment(Environment* parent)
{
    this->parent = parent;
    this->variables = std::map<std::string, Expr*>();
    this->reservedSymbols = std::set<std::string>();
    this->reservedSymbols.insert("+");
    this->reservedSymbols.insert("-");
    this->reservedSymbols.insert("*");
    this->reservedSymbols.insert("/");
    this->reservedSymbols.insert("lambda");
    this->reservedSymbols.insert("define");
    this->reservedSymbols.insert("quote");
    this->reservedSymbols.insert("if");
    this->reservedSymbols.insert(">");
    this->reservedSymbols.insert("car");
    this->reservedSymbols.insert("cdr");
    this->reservedSymbols.insert("cons");
}

bool Environment::isReservedWord(std::string key)
{
    //std::cout << key << std::endl;
    return this->reservedSymbols.find(key) != this->reservedSymbols.end();
}

Expr* Environment::getVariable(Expr* sym)
{
    auto name = *sym->as.symbol.name;

    // Do not evaluate a reserved symbol
    if (this->reservedSymbols.find(name) != this->reservedSymbols.end())
    {
        return sym;
    }

    auto iter = variables.find(name);
    if (iter != variables.end())
    {
        return iter->second;
    }
    // Check in the parent environment up the chain
    if (this->parent != nullptr)
    {
        return this->parent->getVariable(sym);
    }
    else
    {
        throw std::runtime_error("Unbound variable: '" + name + "'.");
    }
}
