//
// Created by felix on 4/14/24.
//

#include "pixelformat.h"

static const struct VSPixelDescription descriptions[VSPixelFormatNB] = {
    [VSPixelFormatNone] = { 0 },
    [VSPixelFormatYUV420P] = {
        .flags = 0,
        .log2_horizontal_chroma_ss = 1,
        .log2_vertical_chroma_ss = 1,
        .nb_components = 3,
        .components = {
            {.bitdepth=8, .plane=0, .bitoffset=0},
            {.bitdepth=8, .plane=1, .bitoffset=0},
            {.bitdepth=8, .plane=2, .bitoffset=0},
        }
    },
    [VSPixelFormatYUV422P] = {
        .flags = 0,
        .log2_horizontal_chroma_ss = 1,
        .log2_vertical_chroma_ss = 0,
        .nb_components = 3,
        .components = {
            {.bitdepth=8, .plane=0, .bitoffset=0},
            {.bitdepth=8, .plane=1, .bitoffset=0},
            {.bitdepth=8, .plane=2, .bitoffset=0},
        }
    },
    [VSPixelFormatYUV444P] = {
        .flags = 0,
        .log2_horizontal_chroma_ss = 0,
        .log2_vertical_chroma_ss = 0,
        .nb_components = 3,
        .components = {
            {.bitdepth=8, .plane=0, .bitoffset=0},
            {.bitdepth=8, .plane=1, .bitoffset=0},
            {.bitdepth=8, .plane=2, .bitoffset=0},
        }
    },
};

const struct VSPixelDescription *vs_pixel_description_get(enum VSPixelFormat format) {
    return &descriptions[format];
}

int vs_pixel_description_plane_count(const struct VSPixelDescription *desc) {
    int ret = 0;
    for (int i = 0; i < desc->nb_components; ++i)
        ret = desc->components[i].plane > ret ? desc->components[i].plane : ret;
    return ret + 1;
}