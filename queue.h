//
// Created by felix on 4/16/24.
//

#ifndef VIDEOSCALE_PTRQUEUE_H
#define VIDEOSCALE_PTRQUEUE_H

#include <stdbool.h>

struct VSQueue {
    int head;
    int tail;
    int size;
    int capacity;
    int slot_size;
    bool resizable;
    char *data;
};

void vs_queue_init(struct VSQueue *queue, int slot_size, int slots, bool resizable);
void vs_queue_deinit_deep(struct VSQueue *queue, void(*free)(void*));
#define vs_queue_deinit(queue) vs_queue_deinit_deep(queue, NULL)
int vs_queue_get_size(struct VSQueue *queue);
int vs_queue_push_size(struct VSQueue *queue, void *data, int size);
#define vs_queue_push(queue, data) vs_queue_push_size(queue, data, sizeof(*data))
int vs_queue_pop_size(struct VSQueue *queue, void *out, int size);
#define vs_queue_pop(queue, out) vs_queue_pop_size(queue, out, sizeof(*out))
void *vs_queue_peeki(struct VSQueue *queue, int i);
#define vs_queue_peek(queue) vs_queue_peeki(queue, 0)

#endif //VIDEOSCALE_PTRQUEUE_H
