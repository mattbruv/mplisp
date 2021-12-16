#include <iostream>
#include <string>
#include <list>

#include "expr.h"
#include "read.h"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string source = readFile(argv[1]);
        std::list<Token> tokens = tokenize(source);
        printTokens(tokens);
    }
}