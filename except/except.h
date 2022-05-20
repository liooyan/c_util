#ifndef EXCEPTION
#define EXCEPTION

#include <setjmp.h>
#include <pthread.h>
#include "stdlib.h"
#include "assert.h"


#define EXCEPTION_ERROR 1 //发生异常标记
#define EXCEPTION_OK 0 //未异常标记

jmp_buf except_jmpBuf1;
jmp_buf except_jmpBuf2;


pthread_key_t key;
/// 异常定义
typedef struct {
    const char *reason;
} Exception;


typedef struct Try_module_stack {
    struct Try_module_stack *tryModuleStack;
    struct Error_msg *errorMsg;
    int Exception_type;
    int finally_type;
    jmp_buf except_jmpBuf;

} Try_module_stack;

typedef struct Error_msg {
    Exception *exception;
    const char *_File;   //异常代码文件行数
    unsigned _Line; //异常 行数

} Error_msg;


void throwFunction(Exception *exception, const char *_File, unsigned _Line);

void printfError(Error_msg *msg);

/// 可能抛出异常的代码块
#define TRY     \
do {\
    int except_type;\
    Try_module_stack *tryModuleStack = pthread_getspecific(key);\
    Try_module_stack *newTryModuleStack = (Try_module_stack *) malloc(sizeof(Try_module_stack));\
    newTryModuleStack->errorMsg = NULL;\
    Error_msg msg;\
    pthread_setspecific(key, newTryModuleStack);\
    if (tryModuleStack != NULL) {\
        newTryModuleStack->tryModuleStack = tryModuleStack;\
    } else {\
    newTryModuleStack->tryModuleStack = NULL;\
    }\
    except_type = setjmp(newTryModuleStack->except_jmpBuf);\
    if (newTryModuleStack->errorMsg != NULL) {\
        msg = *newTryModuleStack->errorMsg;\
    }\
    if (except_type == 0) {


#define ENDTRY          \
    }\
    int exception_type = newTryModuleStack->Exception_type;\
    Try_module_stack *old = newTryModuleStack->tryModuleStack;\
    if (old == NULL) {\
        pthread_key_delete(key);\
    } else {\
        pthread_setspecific(key, old);\
    }\
    if (newTryModuleStack->errorMsg != NULL) {\
        free(newTryModuleStack->errorMsg);\
    }\
    free(newTryModuleStack);\
    if (exception_type == -2) {\
        longjmp(tryModuleStack->except_jmpBuf, EXCEPTION_ERROR);\
    }else                        \
    if (exception_type == -4) {\
        printfError(&msg); \
        assert(0);\
    }    \
} while (0);

/// 捕获特定异常
#define CATCH(ex)      \
    } else if (newTryModuleStack->Exception_type == -1 && newTryModuleStack->errorMsg->exception == &ex1) {


#define DEFCATCH    \
    } else if (newTryModuleStack->Exception_type == -1 ) {

#define FINILLY \
    }\
    if (newTryModuleStack->finally_type != 1) {\
    newTryModuleStack->finally_type = 1;

/// 抛出异常
#define Throw(exception) throwFunction(exception, __FILE__, __LINE__)


#endif /* __EXCEPION_H__ */