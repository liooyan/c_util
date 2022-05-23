#include "mem.h"
#include "stdlib.h"


Exception Mem_Failed = {"Allocation Failed"};
static void throw_error(Exception *exception, const char *file, int line) {
    if (file == NULL) {
        Throw(&Mem_Failed);
    } else {
        throwFunction(&Mem_Failed, file, line);
    }
}


void *mem_alloc(long nbytes, const char *file, int line) {
    void *ptr;
    assert(nbytes > 0);
    ptr = malloc(nbytes);
    if (ptr == NULL) {
        throw_error(&Mem_Failed, file, line);
    }
    return ptr;
}


void *mem_calloc(long count, long nbytes, const char *file, int line) {

    assert(count >0);
    assert(nbytes >0);
    void *ptr = calloc(count, nbytes);
    if(ptr == NULL){
        throwFunction(&Mem_Failed, file, line);
    }
    return ptr;
}


void mem_free(void *ptr,const char *file,int line){
    if(ptr){
        free(ptr);
    }
}


void mem_resize(void *ptr,long  count,const char *file,int line){
    assert(ptr);
    assert(count >0);
    ptr = realloc(ptr,count);
    if(ptr == NULL){
        throwFunction(&Mem_Failed, file, line);
    }
}

