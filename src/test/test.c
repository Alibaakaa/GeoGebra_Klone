#include "Expression.h"
#include <stdio.h>

int main() {
    Expression* e = Expression_create("x**2 + y**2 - 3");
    if (e == NULL) {
        printf("could not create expression\n");
        return 1;
    }

    printf("%f\n", Expression_evaluate(e, 3, 5));
    Expression_destroy(e);

    return 0;
}
