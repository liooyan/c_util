#ifndef EXECUTE_POOL
#define EXECUTE_POOL

#include <pthread.h>
#include "queue.h"

typedef struct execute_work {
    void *(*work_function)(void *); //线程执行方法
    void *args;                   //参数
} execute_work_t;


typedef struct execute_pool {
    size_t shutdown;       // 线程池运行状态, 0 运行 1停止
    size_t core_num_thread; // 线程池核心运行数
    size_t run_size;     // 当前执行任务数量
    pthread_t *thread_id;     // threadsId 数组
    queue *queue_work;     // execute_work 队列
    pthread_mutex_t pool_lock;     // 线程池锁 lock
} execute_pool_t;



/**
 * 创建线程池.
 * @param core_num_thread 核心线程数
 * @param max_num_thread 队列最大等待数量
 * @return
 */
execute_pool_t *execute_create_tpool(size_t core_num_thread,size_t max_num_thread);

/**
 * 销毁线程池
 *
 * @param pool
 */
void execute_destroy_tpool(execute_pool_t *pool);

/**
 *  向线程池添加 任务
 * @param pool 线程池对象
 * @param function 需要执行的函数
 * @param args 函数参数
 * @return
 */
int execute_add_task(execute_pool_t *pool, void *(*function)(void *), void *args);

#endif