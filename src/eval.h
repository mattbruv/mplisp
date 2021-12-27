#pragma once

#include "env.h"
#include "expr.h"

void eval(Expr* expression, Environment* env);
void evalList(Expr* expr, Environment* env);
void evalLambda(Expr* expr, Environment* env);
void funcAdd(Expr* expr, Environment* env);
void funcSub(Expr* expr, Environment* env);
void funcMul(Expr* expr, Environment* env);
void funcDiv(Expr* expr, Environment* env);
void funcGreaterThan(Expr* expr, Environment* env);
void funcDefine(Expr* expr, Environment* env);
bool isExprTrue(Expr* expr);
void evalIf(Expr* expr, Environment* env);
void funcCar(Expr* expr, Environment* env);
void funcCdr(Expr* expr, Environment* env);
void funcCons(Expr* expr, Environment* env);
void funcEmpty(Expr* expr, Environment* env);