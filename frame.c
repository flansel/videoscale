//
// Created by felix on 4/14/24.
//

#include "frame.h"
#include "pixelformat.h"
#include "mathutils.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void vs_frame_utils_set_stride(int out_stride[VSFRAME_MAX_PLANES], enum VSPixelFormat format, int width) {
    memset(out_stride, 0, sizeof(*out_stride) * VSFRAME_MAX_PLANES);
    const struct VSPixelDescription *desc = vs_pixel_description_get(format);
    int planes = vs_pixel_description_plane_count(desc);
    for (int i = 0; i < desc->nb_components; ++i) {
        int plane = desc->components[i].plane;
        out_stride[plane] += ((desc->components[i].bitdepth * width) >> ((i == 0 || planes == 1) ? 0 : desc->log2_horizontal_chroma_ss)) / 8;
    }
}

int vs_frame_alloc_buffer(struct VSFrame *frame) {
    assert(frame->meta.format != VSPixelFormatNone);
    assert(frame->meta.width > 0 && frame->meta.height > 0);

    vs_frame_utils_set_stride(frame->stride, frame->meta.format, frame->meta.width);
    const struct VSPixelDescription *desc = vs_pixel_description_get(frame->meta.format);
    int planes = vs_pixel_description_plane_count(desc);
    int buffer_size = 0;
    int offsets[VSFRAME_MAX_PLANES];

    for (int i = 0; i < planes; ++i) {
        offsets[i] = buffer_size;
        buffer_size += frame->stride[i] * (frame->meta.height >> ((i == 0 || planes == 1) ? 0 : desc->log2_vertical_chroma_ss));
    }

    frame->data.data = malloc(buffer_size);
    if (!frame->data.data)
        return -1;

    memset(frame->planes, 0, sizeof(frame->planes));
    for (int i = 0; i < planes; ++i) {
        frame->planes[i] = (unsigned char*)frame->data.data + offsets[i];
    }
    return 0;
}
