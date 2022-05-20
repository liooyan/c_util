#ifndef QUEUE
#define QUEUE

#include <crtdefs.h>
#include <pthread.h>

typedef struct queue_attr_struct {

    void *data; //存储内容
    struct queue_attr_struct *next;
    struct queue_attr_struct *head;
} queue_attr;

typedef struct queue_struct {
    size_t queue_size; // 当前队列长度,
    size_t max_num_thread; // 线程池核心运行数
    pthread_mutex_t queue_lock;     // execute_work 队列 lock
    queue_attr *execute_work_head;     // execute_work 队列 头
    queue_attr *execute_work_tail;     // execute_work 队列 尾

} queue;


void *queue_poll(queue *queue_t);

int queue_peek(queue *queue_t, void *work);

int queue_destroy(queue *queue_t);

queue *queue_create(size_t max_num_thread);
#endif