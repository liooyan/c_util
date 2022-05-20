#include "except.h"


void throwFunction(Exception *exception, const char *_File, unsigned _Line) {
    Try_module_stack *tryModuleStack = pthread_getspecific(key);
    if (tryModuleStack != NULL) {
        Error_msg *msg = (Error_msg *) malloc(sizeof(Error_msg));
        msg->exception = exception;
        msg->_File = _File;
        msg->_Line = _Line;
        tryModuleStack->errorMsg = msg;
        longjmp(tryModuleStack->except_jmpBuf, EXCEPTION_ERROR);
    } else {
        assert(0);
    }
}
