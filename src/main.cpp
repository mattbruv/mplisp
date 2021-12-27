#include <iostream>
#include <string>
#include <list>

#include "eval.h"
#include "globals.h"
#include "env.h"
#include "expr.h"
#include "read.h"
#include "parse.h"
#include "vm.h"

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

void applyFile(std::string path, Environment* env)
{
    std::vector<Token> tokens = tokenize(readFile(path));
    auto parser = new Parser(tokens);

    // parse all expressions in the file
    while (!parser->isAtEnd())
    {
        parser->parse();
        auto result = vm.pop();
        //Expr* result = parser->parse();
        //vm.push(eval(result, env));
    }

    // set all new definitions as reserved
    for (auto x : env->variables)
    {
        //std::cout << x.first << std::endl;
        // for some reason inserting new items into my set
        // crashes my program... not sure why.
        //env->reservedSymbols.insert(x.first);
    }

    delete parser;
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

        try
        {
            // Global default environment
            Environment* globalEnv = new Environment(nullptr);
            //applyFile("std/comparison.scm", globalEnv);
            //applyFile("std/logical.scm", globalEnv);
            //applyFile("std/list.scm", globalEnv);

            std::vector<Token> tokens = tokenize(source);
            auto parser = new Parser(tokens);

            // parse all expressions in the file
            while (!parser->isAtEnd())
            {
                parser->parse();
                auto result = vm.pop();

                Expr* evaled = eval(result, globalEnv);
                printExpr(evaled, true);
            }
        }
        catch (std::runtime_error const& error)
        {
            std::cout << error.what() << std::endl;
        }
    }
}