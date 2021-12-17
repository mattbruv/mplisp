#include <iostream>
#include <string>
#include <list>

#include "globals.h"
#include "env.h"
#include "expr.h"
#include "read.h"

void printHelp()
{
    std::cout << "MP LISP Interpreter*" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "mplisp [-h, --help] [-v] file" << std::endl;
    std::cout << "file    an input file to interpret." << std::endl;
    std::cout << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "-h, --help    print this message" << std::endl;
    std::cout << "-v            print verbose output" << std::endl;
    std::cout << std::endl;
    std::cout << "*for educational purposes, don't actually use this..." << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string source;

        for (int i = 0; i < argc; i++)
        {
            auto arg = argv[i];
            if (std::string("-h").compare(arg) == 0 || std::string("--help").compare(arg) == 0)
            {
                printHelp();
                return 0;
            }
            else if (std::string("-v").compare(arg) == 0)
            {
                setVerbose(true);
            }
            else
            {
                source = readFile(arg);
            }
        }

        std::list<Token> tokens = tokenize(source);
        printTokens(tokens);

        Environment env;
        Expr n;
        n.as.number.isInt = true;
        n.as.number.as.intValue = 420;
        env.variables["foo"] = &n;

        std::cout << &n << std::endl;
        try
        {
            auto val = env.getVariable("foo");
            std::cout << &*val << std::endl;
            std::cout << val->type << std::endl;
            std::cout << val->as.number.as.intValue << std::endl;
        }
        catch (std::runtime_error const& error)
        {
            std::cout << error.what() << std::endl;
        }
    }
}