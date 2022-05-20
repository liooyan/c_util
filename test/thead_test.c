//#include "../util/execute_pool.h"
//#include <unistd.h>
//#include <stdio.h>
//
//void *fun(void *args) {
//    int thread = (int) args;
//    printf("running the thread of %d\n", thread);
//    return NULL;
//}
//
//int main(int argc, char *args[]) {
//    execute_pool_t *pool = execute_create_tpool(2, 3);
//
//    for (int i = 0; i < 10000; i++) {
//        execute_add_task(pool, fun, (void *) i);
//    }
//
//
//
//    sleep(2);
//    execute_destroy_tpool(pool);
//    return 0;
//}
//
//
#include <stdio.h>

#include <pthread.h>
#include <unistd.h>

pthread_key_t key;
struct test_struct {
    int i;
    float k;
};

void  run(int x){
    printf("child%d 中 pthread_getspecific(key)返回的指针为:0x%p \n ",x, (struct test_struct *) pthread_getspecific(key));
    printf("利用 pthread_getspecific(key)打印 child%d 线程中与key关联的结构体中成员值: \n struct_data.i:%d \n struct_data.k: %f \n ",x,
           ((struct test_struct *) pthread_getspecific(key))->i, ((struct test_struct *) pthread_getspecific(key))->k);
    printf("------------------------------------------------------ \n ");
}

void *child1(void *arg) {
    struct test_struct struct_data;
    struct_data.i = 10;
    struct_data.k = 3.1415;
    pthread_setspecific(key, &struct_data);
    printf("结构体struct_data的地址为 0x%p \n ", &(struct_data));
    run(1);
}

void *child2(void *arg) {
    int temp = 20;
    sleep(2);
    printf("child2 中变量 temp 的地址为 0x%p \n ", &temp);
    pthread_setspecific(key, &temp);
    run(2);
}

int main(void) {
    pthread_t tid1, tid2;
    pthread_key_create(&key, NULL);
    pthread_create(&tid1, NULL, (void *) child1, NULL);
    pthread_create(&tid2, NULL, (void *) child2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_key_delete(key);
    return (0);
}

