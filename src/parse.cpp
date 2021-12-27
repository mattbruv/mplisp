#include <iostream>
#include <string>

#include "globals.h"
#include "expr.h"
#include "parse.h"
#include "read.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->current = 0;
    this->tokens = tokens;
}

void Parser::parse()
{
    Expr* expr = vm.newExpr(ExprType::Number); //  new Expr();
    std::cout << "\n" << std::endl;
    std::cout << "parse called" << std::endl;
    vm.push(expr);

    auto token = peek();
    //std::cout << "parse() " << token.content << std::endl;
    if (token.type == TokenType::BOOLEAN)
    {
        advance();
        expr->type = ExprType::Boolean;

        if (std::string("true").compare(token.content) == 0)
        {
            expr->as.boolean.value = true;
        }
        else
        {
            expr->as.boolean.value = false;
        }
        printExpr(expr, true);
        return;
    }

    if (token.type == TokenType::SYMBOL_NUMBER)
    {
        //std::cout << "parse number/symbol expr" << std::endl;
        advance();
        if (parseNumber(token, expr))
        {
            //std::cout << "return number" << std::endl;
            printExpr(expr, true);
            return;
        }
        expr->type = ExprType::Symbol;
        expr->as.symbol.name = new std::string(token.content);
        //std::cout << "return symbol" << std::endl;
        printExpr(expr, true);
        return;
    }

    if (token.type == TokenType::QUOTE)
    {
        advance();

        this->parse();
        Expr* result = vm.pop();

        expr->type = ExprType::List;
        expr->as.list.exprs = new std::vector<Expr*>();

        auto quote = vm.newExpr(ExprType::Symbol);
        quote->as.symbol.name = new std::string("quote");

        expr->as.list.exprs->push_back(quote);
        expr->as.list.exprs->push_back(result);

        return;
    }

    consume(TokenType::PAREN_LEFT, "Expected (, found " + token.content);
    expr->type = ExprType::List;
    // Don't forget to initialize your variables or bad things happen
    expr->as.list.exprs = new std::vector<Expr*>();

    while (check(TokenType::PAREN_RIGHT) == false)
    {
        std::cout << "parsing next part in list..." << std::endl;
        this->parse();
        auto temp = vm.pop();
        expr->as.list.exprs->push_back(temp);
        std::cout << "pushed to back" << std::endl;
    }

    consume(TokenType::PAREN_RIGHT, "Expected ), found " + token.content);
    std::cout << "done with list" << std::endl;

    return;
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

bool Parser::isAtEnd()
{
    return (size_t)current == tokens.size();
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
    return std::runtime_error(message);
}

bool parseNumber(Token token, Expr* expr)
{
    try
    {
        // try to parse as double if content contains .
        if (token.content.find(".") != std::string::npos)
        {
            auto doubleVal = std::stod(token.content);
            expr->type = ExprType::Number;
            expr->as.number.isInt = false;
            expr->as.number.as.doubleValue = doubleVal;
            return true;
        }
        // otherwise, parse it as a normal old integer
        else
        {
            auto intVal = std::stoll(token.content);
            expr->type = ExprType::Number;
            expr->as.number.isInt = true;
            expr->as.number.as.intValue = intVal;
            return true;
        }
    }
    catch (std::invalid_argument& error)
    {
        return false;
    }
    return false;
}