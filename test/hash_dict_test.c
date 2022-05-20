#include "../util/dict.h"
#include "stdio.h"
#include "../util/dict_type.h"




int main() {
    dict *di = dictCreate(&strObjectDictType);
    char *a = 0;
    char *a1 = "123";
    char *a2 = "123";
    char *a3 = "123";
    char *a4 = "123";
    char *a5 = "123";
    char *a6 = "123";
    char *a7 = "123";
    char *b = "123";
    char *c = "123";
    dictAdd(di, &a, &c);
    dictAdd(di, &a1, &b);
    dictAdd(di, &a2, &b);
    dictAdd(di, &a3, &b);
    dictAdd(di, &a4, &b);
    dictAdd(di, &a5, &b);
    //    dictDelete(di, &a5);
    dictEntry *value = dictFind(di, &a5);
    dictAdd(di, &a6, &b);
    dictAdd(di, &a7, &b);

    if(value == NULL){
        printf("null");
    } else{
        printf("%d", *(int *)value->v.val);
    }
    return 0;
}
