#pragma once

#include "env.h"
#include "expr.h"

void eval(std::shared_ptr<Expr> expression, std::shared_ptr<Environment> env);
void evalList(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void evalLambda(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcAdd(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcSub(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcMul(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcDiv(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcGreaterThan(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcDefine(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
bool isExprTrue(std::shared_ptr<Expr> expr);
void evalIf(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcCar(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcCdr(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcCons(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);
void funcEmpty(std::shared_ptr<Expr> expr, std::shared_ptr<Environment> env);