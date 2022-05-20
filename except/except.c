#include "except.h"
#include "stdio.h"


void throwFunction(Exception *exception, const char *_File, unsigned _Line) {
    Try_module_stack *tryModuleStack = pthread_getspecific(key);
    Error_msg *msg = (Error_msg *) malloc(sizeof(Error_msg));
    msg->exception = exception;
    msg->_File = _File;
    msg->_Line = _Line;
    if (tryModuleStack != NULL) {

        if (tryModuleStack->errorMsg != NULL && tryModuleStack->tryModuleStack != NULL) {
            tryModuleStack->Exception_type = -2;
            tryModuleStack->tryModuleStack->errorMsg = msg;
            tryModuleStack->tryModuleStack->Exception_type = -1;
        } else if (tryModuleStack->tryModuleStack == NULL) {
            tryModuleStack->Exception_type = -4;
            tryModuleStack->errorMsg = msg;
        } else if (tryModuleStack->errorMsg != NULL) {
            tryModuleStack->Exception_type = -3;
            tryModuleStack->errorMsg = msg;
        } else {
            tryModuleStack->Exception_type = -1;
            tryModuleStack->errorMsg = msg;
        }
        longjmp(tryModuleStack->except_jmpBuf, EXCEPTION_ERROR);

    } else {
        printfError(msg);
        assert(0);
    }
}


void printfError(Error_msg *msg) {
    printf("has error :[%s], at  : [%s] in [%d]\n", msg->exception->reason, msg->_File, msg->_Line);
}