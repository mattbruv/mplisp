#pragma once

#include "env.h"
#include "expr.h"

Expr* eval(Expr* expression, Environment* env);