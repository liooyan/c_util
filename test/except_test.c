#include "../except/except_base.h"
#include "stdio.h"


int run(Exception *exception) {
    Throw(exception);
    return 0;
}


void ExceptionTest(Exception *exception) {

    try {
            run(exception);
        }
        //有异常
        catch(ex1) {
            printf("异常类型：%s\n", ex1.reason);
        }
        catch(ex2) {
            printf("异常类型：%s\n", ex2.reason);
        }
        catch(ex3) {
            printf("异常类型：%s\n", ex3.reason);
        }
        defCatch {
            printf("通用异常类型\n");
        }finally{
            printf("执行finally\n");
        }
            endTry


}


int main() {

    run(NULL);
    pthread_key_create(&key, NULL);
    ExceptionTest(&ex1); // 异常1
    ExceptionTest(&ex2); // 异常2
    ExceptionTest(&ex3); // 异常3
    ExceptionTest(&ex4); // 异常3
    return 0;
}