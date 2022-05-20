#include "execute_pool.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/**
 * 执行任务
 * @param args
 * @return
 */
static void *work_run(void *args) {
    execute_pool_t *pool = (execute_pool_t *) args;

    while (1) {
        execute_work_t *work = (execute_work_t *) queue_poll(pool->queue_work);
        //队列中没有任务,关闭当前线程
        if (work == NULL) {
            pthread_mutex_t lock = pool->pool_lock;
            pthread_mutex_lock(&lock);
            pool->run_size = pool->run_size - 1;
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL);
            break;
        }

        work->work_function(work->args);

        free(work);

    }
    return NULL;
}

int execute_add_task(execute_pool_t *pool, void *(*function)(void *), void *args) {


    execute_work_t *work = (execute_work_t *) malloc(sizeof(execute_work_t));
    if (!work) {
        printf("in %s,malloc work error!,errno = %d,explain:%s\n", __func__, errno, strerror(errno));
        return -1;
    }

    work->work_function = function;
    work->args = args;
    queue *queue = pool->queue_work;
    int ref = queue_peek(queue, work);
    if (ref) {
        return ref;
    }

    pthread_mutex_t lock = pool->pool_lock;
    pthread_mutex_lock(&lock);
    size_t run_size = pool->run_size;
    size_t core_num_thread = pool->core_num_thread;


    if (core_num_thread > run_size) {
        pthread_t run = pool->thread_id[run_size];
        if (pthread_create(&run, NULL, work_run, pool)) {
            printf("pthread_create failed!\n");
            exit(-1);
        }
        pool->run_size = pool->run_size + 1;
    }

    pthread_mutex_unlock(&lock);

    return ref;
}

execute_pool_t *execute_create_tpool(size_t core_num_thread, size_t max_num_thread) {
    execute_pool_t *pool = (execute_pool_t *) malloc(sizeof(execute_pool_t));
    if (pool == NULL) {
        printf("in %s,malloc execute_pool_t failed!,errno = %d,explain:%s\n", __func__, errno, strerror(errno));
        exit(-1);
    }
    pool->shutdown = 0;
    pool->run_size = 0;
    pool->core_num_thread = core_num_thread;

    pool->queue_work = queue_create(max_num_thread);


    pool->thread_id = (pthread_t *) malloc(sizeof(pthread_t) * core_num_thread);
    if (pool->thread_id == NULL) {
        exit(-1);
    }



    if (pthread_mutex_init(&pool->pool_lock, NULL) != 0) {
        exit(-1);
    }
    return pool;
}

void execute_destroy_tpool(execute_pool_t *pool) {

    if (pool->shutdown) {
        return;
    }
    pool->shutdown = 1;


    for (int i = 0; i < pool->core_num_thread; i++) {
        pthread_join(pool->thread_id[i], NULL);
    }
    free(pool->thread_id);
    queue *queue_t = pool->queue_work;
    execute_work_t *work = (execute_work_t *) queue_poll(queue_t);
    while (work != NULL) {
        free(work);
        work = (execute_work_t *) queue_poll(queue_t);
    }

    pthread_mutex_destroy(&pool->pool_lock);
    free(pool);
    queue_destroy(queue_t);
}