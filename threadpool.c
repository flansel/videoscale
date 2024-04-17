//
// Created by felix on 4/16/24.
//

#include "threadpool.h"
#include <stdlib.h>

static void *task(void *arg) {
    struct VSThreadPool *pool = arg;
    struct VSThreadTask current;

    while (pool->running) {
        pthread_mutex_lock(&pool->task_queue_mut);
        int err = vs_queue_pop(&pool->task_queue, &current);
        if (err != 0) {
            // no need to loop, spurious wakes are fine here.
            pthread_cond_wait(&pool->new_task_condition, &pool->task_queue_mut);
        }
        pthread_mutex_unlock(&pool->task_queue_mut);

        if (err == 0) {
            current.ret = current.task(current.user);
        }
    }

    return NULL;
}

void vs_thread_pool_init(struct VSThreadPool *pool, int nb_threads) {
    pthread_mutex_init(&pool->task_queue_mut, NULL);
    pthread_cond_init(&pool->new_task_condition, NULL);
    vs_queue_init(&pool->task_queue, sizeof(struct VSThreadTask*), 30, true);
    pool->nb_threads = nb_threads;
    pool->threads = malloc(sizeof(*pool->threads) * nb_threads);
    pool->running = true;

    for (int i = 0; i < nb_threads; ++i) {
        pthread_create(&pool->threads[i], NULL, task, pool);
    }
}

void vs_thread_pool_deinit(struct VSThreadPool *pool) {
    pool->running = false;
    for (int i = 0; i < pool->nb_threads; ++i) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->task_queue_mut);
    pthread_cond_destroy(&pool->new_task_condition);
    free(pool->threads);
    vs_queue_deinit(&pool->task_queue);
}

int vs_thread_pool_schedule_task(struct VSThreadPool *pool, struct VSThreadTask *task) {
    pthread_mutex_lock(&pool->task_queue_mut);
    int err = vs_queue_push(&pool->task_queue, &task);
    pthread_cond_broadcast(&pool->new_task_condition);
    pthread_mutex_unlock(&pool->task_queue_mut);
    return err;
}