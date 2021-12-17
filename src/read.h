#pragma once

#include <string>
#include <vector>

enum TokenType
{
    WHITESPACE,
    COMMENT,
    PAREN_LEFT,
    PAREN_RIGHT,
    QUOTE,
    SYMBOL_NUMBER,
    STRING,
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

void printTokens(std::vector<Token> tokens);
void printToken(Token token);
std::vector<Token> tokenize(std::string source);
std::string readFile(std::string path);