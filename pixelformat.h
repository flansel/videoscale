//
// Created by felix on 4/14/24.
//

#ifndef FILTERCHAIN_PIXELFORMAT_H
#define FILTERCHAIN_PIXELFORMAT_H

enum VSPixelFormat {
    VSPixelFormatNone = 0,
    VSPixelFormatYUV420P,
    VSPixelFormatYUV422P,
    VSPixelFormatYUV444P,
    VSPixelFormatNB,
};

struct component {
    int plane;
    int bitdepth;
    int bitoffset;
};

struct VSPixelDescription {
    int flags;
    int log2_horizontal_chroma_ss;
    int log2_vertical_chroma_ss;
    int stride_align;
    int nb_components;
    struct component components[4];
};

const struct VSPixelDescription *vs_pixel_description_get(enum VSPixelFormat format);
int vs_pixel_description_plane_count(const struct VSPixelDescription *desc);
#endif //FILTERCHAIN_PIXELFORMAT_H
