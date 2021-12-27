#pragma once

#include <string>

#include "expr.h"
#include "read.h"

class Parser
{
public:
    Parser(std::vector<Token> tokens);
    void parse();
    bool isAtEnd();

private:
    int current = 0;
    std::vector<Token> tokens;

    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, std::string message);
    std::runtime_error error(Token token, std::string message);
};

bool parseNumber(Token token, Expr* expr);