//
// Created by felix on 4/15/24.
//

#ifndef VIDEOSCALE_VIDEOSCALE_H
#define VIDEOSCALE_VIDEOSCALE_H

#include "frame.h"
#include "filtergraph.h"

struct VSScaleContext {
    struct VSFilterChain *filters;
};

struct VSScaleContext *vsscalecontext_alloc();
void vsscalecontext_free(struct VSScaleContext *context);
int vsscalecontext_push_frame(struct VSScaleContext *context, struct VSFrame *frame);
int vsscalecontext_get_frame(struct VSScaleContext *context, struct VSFrame *frame);
#endif //VIDEOSCALE_VIDEOSCALE_H
