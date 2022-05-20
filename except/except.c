#include "except.h"
#include "stdio.h"


void throwFunction(Exception *exception, const char *_File, unsigned _Line) {
    Try_module_stack *tryModuleStack = pthread_getspecific(key);
    Error_msg *msg = (Error_msg *) malloc(sizeof(Error_msg));
    msg->exception = exception;
    msg->_File = _File;
    msg->_Line = _Line;
    if (tryModuleStack != NULL) {
        tryModuleStack->errorMsg = msg;
        longjmp(tryModuleStack->except_jmpBuf, EXCEPTION_ERROR);
    } else {
        printfError(msg);
        assert(0);
    }
}



void printfError(Error_msg *msg){
    printf("has error ：[%s], at  ：[%s] in [%d]\n",msg->exception->reason,msg ->_File,msg ->_Line);
}