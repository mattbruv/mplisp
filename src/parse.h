#pragma once

#include <string>

#include "expr.h"
#include "read.h"

class Parser
{
public:
    Parser(std::vector<Token> tokens);
    Expr* parse();

private:
    int current = 0;
    std::vector<Token> tokens;

    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    Token consume(TokenType type, std::string message);
    std::runtime_error error(Token token, std::string message);
};

bool parseNumber(Token token, Expr* expr);