#include "util/execute_pool.h"
#include <unistd.h>
#include <stdio.h>

void *fun(void *args) {
    int thread = (int) args;
    printf("running the thread of %d\n", thread);
    return NULL;
}

int main(int argc, char *args[]) {
    execute_pool_t *pool = execute_create_tpool(2, 3);

    for (int i = 0; i < 20; i++) {
        execute_add_task(pool, fun, (void *) i);
    }
    sleep(2);
    execute_destroy_tpool(pool);
    return 0;
}