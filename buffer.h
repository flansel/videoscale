//
// Created by felix on 4/15/24.
//

#ifndef VIDEOSCALE_BUFFER_H
#define VIDEOSCALE_BUFFER_H

struct VSBuffer {
    _Atomic int refcount;
    void *data;
};

int vs_buffer_ref(struct VSBuffer *out, struct VSBuffer *in);
int vs_buffer_move(struct VSBuffer *out, struct VSBuffer *in);
int vs_buffer_unref(struct VSBuffer *buf);

#endif //VIDEOSCALE_BUFFER_H
