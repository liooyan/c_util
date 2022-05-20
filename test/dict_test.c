#include "../util/dict.h"
#include "stdio.h"


uint64_t dictSdsCaseHash(const int *key) {
    return (uint64_t) key;
}

dictType shaScriptObjectDictType = {
        (uint64_t (*)(const void *)) dictSdsCaseHash,            /* hash function */
        NULL,                       /* key dup */
        NULL,                       /* val dup */
        NULL,      /* key compare */
        NULL,          /* key destructor */
        NULL        /* val destructor */
};

int main() {
    dict *di = dictCreate(&shaScriptObjectDictType);
    int a = 0;
    int a1 = 1;
    int a2 = 2;
    int a3 = 3;
    int a4 = 4;
    int a5 = 5;
    int a6 = 6;
    int a7 = 7;
    int b = 23;
    int c = 64;
    dictAdd(di, &a, &c);
    dictAdd(di, &a1, &b);
    dictAdd(di, &a2, &b);
    dictAdd(di, &a3, &b);
    dictAdd(di, &a4, &b);
    dictAdd(di, &a5, &b);
    dictDelete(di, &a5);
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