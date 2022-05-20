#include "queue.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


/**
 * 队列出列
 * @param pool
 * @return
 */
void *queue_poll(queue *queue_t) {
    pthread_mutex_t lock = queue_t->queue_lock;

    pthread_mutex_lock(&lock);

    queue_attr *attr = queue_t->execute_work_head;
    if (attr == NULL) {
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    //队列头出列
    queue_t->execute_work_head = attr->next;
    queue_t->queue_size = queue_t->queue_size - 1;

    if (queue_t->execute_work_head == NULL) {
        queue_t->execute_work_tail = NULL;
    }
    pthread_mutex_unlock(&lock);

    void *data = attr->data;
    //销毁已出来元素
    free(attr);

    return data;
}

/**
 * 队列入队
 * @param pool
 * @param work
 */
int queue_peek(queue *queue_t, void *data) {


    pthread_mutex_t lock = queue_t->queue_lock;
    pthread_mutex_lock(&lock);
    if (queue_t->queue_size >= queue_t->max_num_thread) {
        printf("queue max!\n");
        return -1;
    }

    queue_attr *attr = (queue_attr *) malloc(sizeof(queue_attr));
    attr->data = data;
    attr->next = NULL;
    queue_attr *old_attr = queue_t->execute_work_tail;
    //空队列,初始化
    if (old_attr == NULL) {
        attr->head = NULL;
        queue_t->execute_work_head = attr;
    } else {
        attr->head = old_attr;
        old_attr->next = attr;
    }
    queue_t->execute_work_tail = attr;
    queue_t->queue_size = queue_t->queue_size + 1;
    pthread_mutex_unlock(&lock);
    return 0;
}


queue *queue_create(size_t max_num_thread) {
    queue *queue_t = (queue *) malloc(sizeof(queue));
    queue_t->queue_size = 0;
    queue_t->execute_work_head = NULL;
    queue_t->execute_work_tail = NULL;
    queue_t->max_num_thread = max_num_thread;
    if (pthread_mutex_init(&queue_t->queue_lock, NULL) != 0) {
        exit(-1);
    }
    return queue_t;
}

int queue_destroy(queue *queue_t){
    if(queue_t->queue_size != 0){
        printf("queue has data!\n");
        return -1;
    }
    free(queue_t);
    return 0;
}




