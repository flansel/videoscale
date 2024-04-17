//
// Created by felix on 4/16/24.
//

#include "unit_test.h"
#include "queue.h"

#include <stdbool.h>

TEST(push_pop,
    struct VSQueue queue;
    int out;
    vs_queue_init(&queue, sizeof(int), 10, false);
    for (int i = 0; i < 10; ++i) {
        CHECK(vs_queue_push(&queue, &i) == 0);
    }
    for (int i = 0; i < 10; ++i) {
        CHECK(vs_queue_pop(&queue, &out) == 0);
        CHECK(out == i);
    }
    for (int i = 0; i < 10; ++i)
        vs_queue_push(&queue, &i);
    for (int i = 0; i < 10; ++i) {
        vs_queue_pop(&queue, &out);
        CHECK(out == i);
    }
)

TEST(size,
    struct VSQueue queue;
    vs_queue_init(&queue, sizeof(int), 10, false);

    int out;
    for (int i = 0; i < 10; ++i) {
        CHECK(vs_queue_get_size(&queue) == i);
        vs_queue_push(&queue, &i);
    }
    for (int i = 0; i < 10; ++i) {
        CHECK(vs_queue_get_size(&queue) == 10 - i);
        vs_queue_pop(&queue, &out);
    }
    for (int i = 0; i < 10; ++i) {
        CHECK(vs_queue_get_size(&queue) == i);
        vs_queue_push(&queue, &i);
    }
    for (int i = 0; i < 10; ++i) {
        CHECK(vs_queue_get_size(&queue) == 10 - i);
        vs_queue_pop(&queue, &out);
    }
)

TEST(resize,
     struct VSQueue queue;
     vs_queue_init(&queue, sizeof(int), 10, true);

     int out;
     for (int i = 0; i < 10; ++i) {
         vs_queue_push(&queue, &i);
     }
     for (int i = 0; i < 5; ++i) {
         CHECK(vs_queue_pop(&queue, &out) == 0);
         CHECK(out == i);
     }
     for (int i = 10; i < 60; ++i) {
         vs_queue_push(&queue, &i);
     }
     for (int i = 0; i < 50; ++i) {
         CHECK(vs_queue_pop(&queue, &out) == 0);
         CHECK(out == i + 5);
     }
)

UNIT(queue.c,
     push_pop,
     size,
     resize
)