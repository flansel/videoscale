//
// Created by felix on 4/14/24.
//

#ifndef FILTERCHAIN_FRAME_H
#define FILTERCHAIN_FRAME_H

#include "pixelformat.h"
#include "buffer.h"

#define VSFRAME_MAX_PLANES (4)

struct VSFrameMeta {
    enum VSPixelFormat format;
    int width;
    int height;
};

struct VSFrame {
    struct VSFrameMeta meta;
    int stride[VSFRAME_MAX_PLANES];
    unsigned char *planes[VSFRAME_MAX_PLANES];
    struct VSBuffer data;
};

int vs_frame_alloc_buffer(struct VSFrame *frame);

// utility functions
void vs_frame_utils_set_stride(int out_stride[VSFRAME_MAX_PLANES], enum VSPixelFormat format, int width);
#endif //FILTERCHAIN_FRAME_H
