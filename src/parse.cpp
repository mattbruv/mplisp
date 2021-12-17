#include <iostream>
#include <string>

#include "expr.h"
#include "parse.h"
#include "read.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->current = 0;
    this->tokens = tokens;
    printTokens(this->tokens);
}

Expr Parser::parse()
{
    auto token = peek();
}

bool Parser::isAtEnd()
{
    return (size_t)current == tokens.size() - 1;
}

Token Parser::peek()
{
    return this->tokens[current];
}

Token Parser::previous()
{
    return this->tokens[current - 1];
}

Token Parser::advance()
{
    if (!isAtEnd())
    {
        current++;
        return previous();
    }
    return peek();
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;
    return peek().type == type;
}

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
        return advance();

    throw error(peek(), message);
}

std::runtime_error Parser::error(Token token, std::string message)
{
    return std::runtime_error(message + token.content);
}