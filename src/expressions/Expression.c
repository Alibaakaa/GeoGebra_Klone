#include "Expression.h"

#include "expr.h"

#include <math.h>

struct _Expression {
    struct expr* expression;
    struct expr_var_list variables;
};

static float sinWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return sinf(expr_eval(&vec_nth(args, 0)));
}

static float cosWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return cosf(expr_eval(&vec_nth(args, 0)));
}

static float tanWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return tanf(expr_eval(&vec_nth(args, 0)));
}

static float asinWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return asinf(expr_eval(&vec_nth(args, 0)));
}

static float acosWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return acosf(expr_eval(&vec_nth(args, 0)));
}

static float atanWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return atanf(expr_eval(&vec_nth(args, 0)));
}

static float sinhWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return sinhf(expr_eval(&vec_nth(args, 0)));
}

static float coshWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return coshf(expr_eval(&vec_nth(args, 0)));
}

static float tanhWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return tanhf(expr_eval(&vec_nth(args, 0)));
}

static float asinhWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return asinhf(expr_eval(&vec_nth(args, 0)));
}

static float acoshWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return acoshf(expr_eval(&vec_nth(args, 0)));
}

static float atanhWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return atanhf(expr_eval(&vec_nth(args, 0)));
}

static float gammaWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return tgammaf(expr_eval(&vec_nth(args, 0)));
}

static float logWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return logf(expr_eval(&vec_nth(args, 0)));
}

static float expWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return expf(expr_eval(&vec_nth(args, 0)));
}

static float absWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return fabsf(expr_eval(&vec_nth(args, 0)));
}

static float sqrtWrapper(struct expr_func*, vec_expr_t* args, void*) {
    return sqrtf(expr_eval(&vec_nth(args, 0)));
}

static struct expr_func g_custom_funcs[] = {
    { "sin", sinWrapper, NULL, 0 },
    { "cos", cosWrapper, NULL, 0 },
    { "tan", tanWrapper, NULL, 0 },
    { "asin", asinWrapper, NULL, 0 },
    { "acos", acosWrapper, NULL, 0 },
    { "atan", atanWrapper, NULL, 0 },
    { "sinh", sinhWrapper, NULL, 0 },
    { "cosh", coshWrapper, NULL, 0 },
    { "tanh", tanhWrapper, NULL, 0 },
    { "asinh", asinhWrapper, NULL, 0 },
    { "acosh", acoshWrapper, NULL, 0 },
    { "atanh", atanhWrapper, NULL, 0 },
    { "gamma", gammaWrapper, NULL, 0 },
    { "log", logWrapper, NULL, 0 },
    { "exp", expWrapper, NULL, 0 },
    { "abs", absWrapper, NULL, 0 },
    { "sqrt", sqrtWrapper, NULL, 0 } 
};

Expression* Expression_create(const char* expression_string) {
    Expression* res = malloc(sizeof(Expression));
    if (res == NULL) return NULL;
    memset(res, 0, sizeof(Expression));

    res->expression = expr_create(expression_string, strlen(expression_string), 
                                  &res->variables, g_custom_funcs);
    if (res->expression == NULL) {
        free(res);
        return NULL;
    }
    return res;
}

void Expression_destroy(Expression* e) {
    if (!e) return;
    expr_destroy(e->expression, &e->variables);
    free(e);
}

float Expression_evaluate(const void* data, float x, float y) {
    const Expression* e = (const Expression*) data;
    for (struct expr_var* var = e->variables.head; var != NULL; var = var->next) {
        if (strcmp(var->name, "x") == 0) var->value = x;
        if (strcmp(var->name, "y") == 0) var->value = y;
    }
    return expr_eval(e->expression);
}
