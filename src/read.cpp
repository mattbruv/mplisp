#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <regex>

#include "read.h"

struct tokenRegex tokenRegexes[] = {
    { TokenType::WHITESPACE, "[\\s,]*" }, //
    { TokenType::COMMENT, ";.*" }, //
    { TokenType::PAREN_LEFT, "\\(" }, //
    { TokenType::PAREN_RIGHT, "\\)" }, //
    { TokenType::QUOTE, "\'" }, //
    { TokenType::SYMBOL_NUMBER, "[^\\s\\[\\]{}('\"`,;)]*" }, //
};

int REGEX_COUNT = sizeof(tokenRegexes) / sizeof(tokenRegexes[0]);

std::list<Token> tokenize(std::string source)
{
    std::list<Token> tokens;

    std::cout << "Tokenizing:" << std::endl;
    std::cout << source << std::endl;

    int i = 0;
    while (i++ < 3) //source.length() > 0)
    {
        std::cout << source.length() << std::endl;
        for (int i = 0; i < REGEX_COUNT; i++)
        {
            auto const regex = std::regex(tokenRegexes[i].pattern);
            auto results = std::smatch();
            bool const inText = std::regex_search(source, results, regex);

            if (inText)
            {
                std::cout << "found in text: " << tokenRegexes[i].type << std::endl;
                std::cout << source << std::endl;
                source = results.suffix().str();
                std::cout << source << std::endl;
                std::cout << source.length() << std::endl;
                break;
                //std::cout << results << std::endl;
            }
        }
    }

    return tokens;
}

std::string readFile(std::string path)
{
    std::ifstream stream = std::ifstream(path);
    std::string content =
        std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    return content;
}