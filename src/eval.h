#pragma once

#include "env.h"
#include "expr.h"

Expr* eval(Expr* expression, Environment* env);
Expr* evalList(Expr* expr, Environment* env);
Expr* evalLambda(Expr* expr, Environment* env);
Expr* funcAdd(Expr* expr, Environment* env);
Expr* funcSub(Expr* expr, Environment* env);
Expr* funcMul(Expr* expr, Environment* env);
Expr* funcDiv(Expr* expr, Environment* env);
Expr* funcGreaterThan(Expr* expr, Environment* env);
Expr* funcDefine(Expr* expr, Environment* env);
bool isExprTrue(Expr* expr);
Expr* evalIf(Expr* expr, Environment* env);
Expr* funcCar(Expr* expr, Environment* env);
Expr* funcCdr(Expr* expr, Environment* env);
Expr* funcCons(Expr* expr, Environment* env);
Expr* funcEmpty(Expr* expr, Environment* env);