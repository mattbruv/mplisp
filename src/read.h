#pragma once

#include <string>
#include <list>

enum TokenType
{
    WHITESPACE,
    COMMENT,
    PAREN_LEFT,
    PAREN_RIGHT,
    QUOTE,
    SYMBOL_NUMBER,
};

struct tokenRegex
{
    TokenType type;
    std::string pattern;
};

struct Token
{
    TokenType type;
    std::string content;
};

void printToken(Token token);
std::list<Token> tokenize(std::string source);
std::string readFile(std::string path);