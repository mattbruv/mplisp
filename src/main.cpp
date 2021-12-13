#include <iostream>
#include "expr.h"

int main()
{
    Expr e;
    e.as.symbol.name = new std::string("Dynamic string on the heap!");
    e.as.number.intVal = 420;

    std::cout << "Hello, world!" << std::endl;
    std::cout << *e.as.symbol.name << std::endl;
    std::cout << e.as.number.intVal << std::endl;
    std::cout << e.as.number.doubleVal << std::endl;
}