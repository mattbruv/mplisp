#include <iostream>
#include <string>
#include <map>

#include "env.h"

/*
class Environment
{
public:
    Environment* parent;
    std::map<std::string, Expr> variables;
    Expr getVariable(std::string name);
};
*/
Expr Environment::getVariable(std::string name)
{
    if (variables.contains(name))
    {
        std::cout << "FOUND!";
    }
}
