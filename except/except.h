#ifndef EXCEPTION
#define EXCEPTION

#include <setjmp.h>
#include <pthread.h>
#include "stdlib.h"
#include "assert.h"


#define EXCEPTION_ERROR 1 //发生异常标记
#define EXCEPTION_OK 0 //未异常标记


pthread_key_t key;

/// 异常定义
typedef struct {
    const char *reason;
} Exception;


typedef struct Try_module_stack {
    struct Try_module_stack *tryModuleStack;
    struct Error_msg *errorMsg;
    jmp_buf except_jmpBuf;

} Try_module_stack;

typedef struct Error_msg {
    Exception *exception;
    const char *_File;   //异常代码文件行数
    unsigned _Line; //异常 行数

} Error_msg;


void throwFunction(Exception *exception, const char *_File, unsigned _Line);

/// 可能抛出异常的代码块
#define try  do{ \
    int type, flag = 1; \
    Try_module_stack *tryModuleStack = pthread_getspecific(key);\
    Try_module_stack *newTryModuleStack = (Try_module_stack *) malloc(sizeof(Try_module_stack)); \
    pthread_setspecific(key, newTryModuleStack);\
    if (tryModuleStack != NULL) {\
        newTryModuleStack->tryModuleStack = tryModuleStack;\
    } else {\
        newTryModuleStack->tryModuleStack = NULL;\
    }\
        if ((type = setjmp(newTryModuleStack->except_jmpBuf)) == 0)\



#define endTry  Try_module_stack *old = newTryModuleStack->tryModuleStack;\
    if (old == NULL) {\
        pthread_key_delete(key);\
    } else {\
        pthread_setspecific(key, old);\
    }\
    if (newTryModuleStack->errorMsg != NULL) {\
        free(newTryModuleStack->errorMsg);\
    }\
    free(newTryModuleStack);\
    }while(0); \

/// 捕获特定异常
#define catch(ex)    if (type && flag && (flag = newTryModuleStack->errorMsg->exception != &ex) == 0)
#define defCatch    if (type && flag )
#define finally

/// 抛出异常
#define Throw(exception) throwFunction(exception, __FILE__, __LINE__)


#endif /* __EXCEPION_H__ */