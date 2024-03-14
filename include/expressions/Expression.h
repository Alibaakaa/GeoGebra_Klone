#pragma once

#include <stdlib.h>
#include "expr.h"

typedef struct _Expression {
    struct expr* expression;
    struct expr_var_list variables;
} Expression;

Expression* Expression_create(const char* expression_string);
void Expression_destroy(Expression* e);

float Expression_evaluate(const Expression* e, float x, float y);
