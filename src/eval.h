#pragma once

#include "env.h"
#include "expr.h"

void eval(std::shared_ptr<Expr> expression, Environment* env);
void evalList(std::shared_ptr<Expr> expr, Environment* env);
void evalLambda(std::shared_ptr<Expr> expr, Environment* env);
void funcAdd(std::shared_ptr<Expr> expr, Environment* env);
void funcSub(std::shared_ptr<Expr> expr, Environment* env);
void funcMul(std::shared_ptr<Expr> expr, Environment* env);
void funcDiv(std::shared_ptr<Expr> expr, Environment* env);
void funcGreaterThan(std::shared_ptr<Expr> expr, Environment* env);
void funcDefine(std::shared_ptr<Expr> expr, Environment* env);
bool isExprTrue(std::shared_ptr<Expr> expr);
void evalIf(std::shared_ptr<Expr> expr, Environment* env);
void funcCar(std::shared_ptr<Expr> expr, Environment* env);
void funcCdr(std::shared_ptr<Expr> expr, Environment* env);
void funcCons(std::shared_ptr<Expr> expr, Environment* env);
void funcEmpty(std::shared_ptr<Expr> expr, Environment* env);