#include "../util/queue.h."
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *args[]) {
    queue *queue_t = queue_create(20);

    for (int i = 0; i < 10000; i++) {
        int *i_t = (int *) malloc(sizeof(int ));
        *i_t = i;
        queue_peek(queue_t, i_t);
    }
    int *data = (int *)queue_poll(queue_t);
    while (data != NULL){
        printf("%d\n",*data);
        data = (int *)queue_poll(queue_t);
    }
    return 0;
}