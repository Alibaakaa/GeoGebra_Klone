#pragma once

typedef struct _Expression Expression;

Expression* Expression_create(const char* expression_string);
void Expression_destroy(Expression* e);

float Expression_evaluate(const Expression* e, float x, float y);
