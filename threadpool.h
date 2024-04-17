//
// Created by felix on 4/16/24.
//

#ifndef VIDEOSCALE_THREADPOOL_H
#define VIDEOSCALE_THREADPOOL_H

#include <pthread.h>
#include <stdbool.h>
#include "queue.h"

struct VSThreadTask {
    bool finished;
    void *ret;
    void *user;
    void*(*task)(void*);
};

struct VSThreadPool {
    bool running;
    pthread_cond_t new_task_condition;
    pthread_mutex_t task_queue_mut;
    struct VSQueue task_queue;
    int nb_threads;
    pthread_t *threads;
};

void vs_thread_pool_init(struct VSThreadPool *pool, int nb_threads);
void vs_thread_pool_deinit(struct VSThreadPool *pool);
int vs_thread_pool_schedule_task(struct VSThreadPool *pool, struct VSThreadTask *task);

#endif //VIDEOSCALE_THREADPOOL_H
