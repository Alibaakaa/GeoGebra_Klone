#include "Expression.h"

Expression* Expression_create(const char* expression_string) {
    Expression* res = malloc(sizeof(Expression));
    if (res == NULL) return NULL;
    memset(res, 0, sizeof(Expression));

    res->expression = expr_create(expression_string, strlen(expression_string), 
                                  &res->variables, NULL);
    if (res->expression == NULL) {
        free(res);
        return NULL;
    }
}

void Expression_destroy(Expression* e) {
    if (!e) return;
    expr_destroy(e->expression, &e->variables);
    free(e);
}

float Expression_evaluate(const Expression* e, float x, float y) {
    for (struct expr_var* var = e->variables.head; var != NULL; var = var->next) {
        if (strcmp(var->name, "x") == 0) var->value = x;
        if (strcmp(var->name, "y") == 0) var->value = y;
    }
    return expr_eval(e->expression);
}
