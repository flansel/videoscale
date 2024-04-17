//
// Created by felix on 4/15/24.
//

#include "vsf_nn.h"
#include "filtergraph.h"
#include "log.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

struct VSFilterNearestNeighbor {
    struct VSFilter filter;
    struct {
        int width;
        int height;
    } params;
};

VS_OPTION_LIST(vsf_nearest_neighbor_options,
    VS_OPTION("width", "Width of the scaled image.", struct VSFilterNearestNeighbor, params.width, 0),
    VS_OPTION("height", "Height of the scaled image.", struct VSFilterNearestNeighbor, params.height, 0)
)

void vsf_nearest_neighbor_free(struct VSFilter **filter) {
    struct VSFilterNearestNeighbor *nn = (struct VSFilterNearestNeighbor*)*filter;
    free(nn);
}

static void vsf_nearest_neighbor_scale_plane_8(uint8_t *out_plane, int out_stride, int out_height, int out_width,
                                            uint8_t *in_plane, int in_stride, int in_height, int in_width) {
    double sfx = in_width / (double)out_width;
    double sfy = in_height / (double)out_height;
    int x2, y2;

    for (int y = 0; y < out_height; ++y) {
        for (int x = 0; x < out_width; ++x) {
            x2 = x * sfx;
            y2 = y * sfy;
            out_plane[(y * out_stride) + x] = in_plane[(y2 * in_stride) + x2];
        }
    }
}

int vsf_nearest_neighbor_filter(struct VSFilter *filter, struct VSFrame *out, struct VSFrame *in) {
    // for now formats match
    assert(out->meta.format == in->meta.format);
    struct VSFilterNearestNeighbor *nn = (struct VSFilterNearestNeighbor*)filter;
    const struct VSPixelDescription *desc = vs_pixel_description_get(in->meta.format);
    int planes = vs_pixel_description_plane_count(desc);

    for (int i = 0; i < planes; ++i) {
        int out_plane_height = out->meta.height >> (i == 0 ? 0 : desc->log2_vertical_chroma_ss);
        int in_plane_height = in->meta.height >> (i == 0 ? 0 : desc->log2_vertical_chroma_ss);
        vsf_nearest_neighbor_scale_plane_8(out->planes[i], out->stride[i], out_plane_height, out->meta.width,
                                         in->planes[i], in->stride[i], in_plane_height, in->meta.width);
    }

    return 0;
}

struct VSFilter *vsf_nearest_neighbor_alloc() {
    struct VSFilterNearestNeighbor *ret = malloc(sizeof *ret);
    ret->filter.free = vsf_nearest_neighbor_free;
    ret->filter.filter = vsf_nearest_neighbor_filter;
    ret->filter.c_obj.options_list = vsf_nearest_neighbor_options;
    ret->filter.c_obj.object_type_name = "nearest neighbor";
    return (struct VSFilter*)ret;
}