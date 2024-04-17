//
// Created by felix on 4/16/24.
//

#include "queue.h"
#include "mathutils.h"
#include "log.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void vs_queue_init(struct VSQueue *queue, int slot_size, int slots, bool resizable) {
    queue->slot_size = slot_size;
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    queue->capacity = slots;
    queue->resizable = resizable;

    queue->data = malloc(queue->slot_size * queue->capacity);
}

void vs_queue_deinit_deep(struct VSQueue *queue, void(*free)(void*)) {
    free(queue->data);
}

int vs_queue_get_size(struct VSQueue *queue) {
    return queue->size;
}

static int resize_queue(struct VSQueue *queue, int new_size) {
    char *newp = malloc(new_size * queue->slot_size);
    if (!newp)
        return -1;

    if (queue->tail >= queue->head && queue->size) {
        memcpy(newp, queue->data + (queue->tail * queue->slot_size), (queue->capacity - queue->tail) * queue->slot_size);
        memcpy(newp + ((queue->capacity - queue->tail) * queue->slot_size), queue->data, queue->head * queue->slot_size);
    } else if (queue->size) {
        memcpy(newp, queue->data + (queue->tail * queue->slot_size), queue->size * queue->slot_size);
    }

    free(queue->data);
    queue->data = newp;
    queue->capacity = new_size;
    queue->tail = 0;
    queue->head = queue->size;
    return 0;
}

int vs_queue_push_size(struct VSQueue *queue, void *data, int size) {
    assert(size == queue->slot_size);
    if (!queue->resizable && queue->size >= queue->capacity)
        return -1;

    if (queue->size >= queue->capacity) {
        // ensure that we keep the same relative ordering.
        int newcap = queue->capacity ? queue->capacity * 2 : 30;
        int err = resize_queue(queue, newcap);
        if (err != 0)
            return err;
    }
    memcpy(queue->data + (queue->slot_size * queue->head), data, size);
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size++;
    return 0;
}

int vs_queue_pop_size(struct VSQueue *queue, void *out, int size) {
    assert(size == queue->slot_size);
    if (queue->size <= 0)
        return -1;
    memcpy(out, queue->data + (queue->slot_size * queue->tail), size);
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size--;
    return 0;
}

void *vs_queue_peeki(struct VSQueue *queue, int i) {
    if (vs_queue_get_size(queue) <= i)
        return NULL;
    int idx = queue->head - i;
    if (idx < 0)
        idx += queue->capacity;
    return queue->data + (idx * queue->slot_size);
}