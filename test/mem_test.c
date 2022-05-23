#include "../mem/mem.h"
#include "stdio.h"


int main() {

    int *p;
    NEW(p);
    *p = 3;
    printf("%d\n", *p);
    FREE(p);
    printf("%d\n", *p);

}