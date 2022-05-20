#include "../except/except_base.h"
#include "stdio.h"




int run(Exception *exception) {
    Throw(exception);
    return 0;
}


int run2(Exception *exception) {
    Throw(exception);
    return 0;
}

void ExceptionTest(Exception *exception) {

    try {
            run(exception);
        }
        //有异常
        catch(ex1) {
            printfError(msg);
        }
        catch(ex2) {
            printfError(msg);
        }
        catch(ex3) {
            printfError(msg);
        }
        defCatch {
            printfError(msg);
        }finally{
            printf("执行finally\n");
        }
            endTry


}


int main() {
    ExceptionTest(&ex1); // 异常1
    ExceptionTest(&ex2); // 异常2
    ExceptionTest(&ex3); // 异常3
    ExceptionTest(&ex4); // 异常3
    return 0;
}
