//
// Created by felix on 4/15/24.
//

#include "videoscale.h"

struct VSScaleContext *vsscalecontext_alloc() {
    return 0x0;
}

void vsscalecontext_free(struct VSScaleContext *context) {

}
int vsscalecontext_push_frame(struct VSScaleContext *context, struct VSFrame *frame) {
    return -1;
}

int vsscalecontext_get_frame(struct VSScaleContext *context, struct VSFrame *frame) {
    return -1;
}