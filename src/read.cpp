#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <regex>

#include "read.h"

struct tokenRegex tokenRegexes[] = {
    { TokenType::WHITESPACE, "[\\s,]+" }, //
    { TokenType::COMMENT, ";.+" }, //
    { TokenType::PAREN_LEFT, "\\(" }, //
    { TokenType::PAREN_RIGHT, "\\)" }, //
    { TokenType::QUOTE, "\'" }, //
    { TokenType::SYMBOL_NUMBER, "[^\\s\\[\\]{}('\"`,;)]+" }, //
    { TokenType::STRING, "\\\".*\\\"" } //
};

int REGEX_COUNT = sizeof(tokenRegexes) / sizeof(tokenRegexes[0]);

std::list<Token> tokenize(std::string source)
{
    std::list<Token> tokens;

    while (source.length() > 0)
    {
        bool errors = true;
        for (int i = 0; i < REGEX_COUNT; i++)
        {
            auto const regex = std::regex(tokenRegexes[i].pattern);
            auto results = std::smatch();
            bool const inText = std::regex_search(source, results, regex);
            auto position = results.position(0);

            if (inText && position == 0)
            {
                auto type = tokenRegexes[i].type;
                std::cout << "Regex Match: " << type; // << std::endl;
                std::cout << " " << tokenRegexes[i].pattern << std::endl;
                std::cout << "Found: \"" << results[0] << "\"" << std::endl;

                // We only care about actual source code
                // Ignore comments and whitespace
                if (type != TokenType::COMMENT && type != TokenType::WHITESPACE)
                {
                    std::cout << "Create token: '" << results[0] << "'" << std::endl;
                    Token t = { type, results[0] };
                    tokens.push_back(t);
                }

                std::cout << std::endl;

                source = results.suffix().str();
                errors = false;
                break;
            }
        }

        if (errors)
        {
            std::cout << "Error parsing remaining input: ";
            std::cout << "'" << source << "'" << std::endl;
            break;
        }
    }

    return tokens;
}

void printTokens(std::list<Token> tokens)
{
    std::cout << "Found " << tokens.size() << " tokens in total." << std::endl;
    int i = 1;
    for (auto const& token : tokens)
    {
        std::cout << "'" << token.content << "' ";
        if (i++ % 10 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    i = 0;
    for (auto const& token : tokens)
    {
        std::cout << i++ << " ";
        printToken(token);
    }
}

void printToken(Token token)
{
    std::cout << token.type << ": ";
    std::cout << "\"" << token.content << "\"";
    std::cout << std::endl;
}

std::string readFile(std::string path)
{
    std::ifstream stream = std::ifstream(path);
    std::string content =
        std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    return content;
}