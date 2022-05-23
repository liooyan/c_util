#include "../except/except.h"
#include "stdio.h"

Exception ex1 = {"ex1"};
Exception ex2 = {"ex2"};
Exception ex3 = {"ex3"};
Exception ex4 = {"ex4"};

int run(Exception *exception) {
    Throw(exception);
    return 0;
}


void run2(Exception *exception) {

    TRY
            run(exception);
        CATCH(&ex1)
            printfError(&msg);
            Throw(exception);
        CATCH(&ex2)
            printfError(&msg);
        FINILLY
            printf("执行finally\n");
            ENDTRY
}

void ExceptionTest(Exception *exception) {

    TRY
            run2(exception);
        CATCH(&ex1)
            printfError(&msg);
        CATCH(&ex2)
            printfError(&msg);
        FINILLY
            printf("执行finally\n");
            ENDTRY


}


int main() {


    ExceptionTest(&ex1); // 异常1
//    ExceptionTest(&ex2); // 异常2
//    ExceptionTest(&ex3); // 异常3
//    ExceptionTest(&ex4); // 异常3
    return 0;
}
