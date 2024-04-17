//
// Created by felix on 4/15/24.
//

#include "vsf_nn.h"
#include "filtergraph.h"
#include "log.h"
#include "threadpool.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

struct VSFilterNearestNeighbor {
    struct VSFilter filter;
    struct VSThreadPool pool;
    struct {
        int width;
        int height;
    } params;
};

struct PlaneData {
    int in_stride;
    int in_width;
    int in_height;
    unsigned char *in_plane;
    int out_stride;
    int out_width;
    int out_height;
    unsigned char *out_plane;
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
        for (int x = 0; x < out_stride; ++x) {
            x2 = x * sfx;
            y2 = y * sfy;
            out_plane[(y * out_stride) + x] = in_plane[(y2 * in_stride) + x2];
        }
    }
}

static void *vsf_task_thread(void *args) {
    struct PlaneData *data = args;
    vsf_nearest_neighbor_scale_plane_8(data->out_plane, data->out_stride, data->out_height, data->out_width,
                                       data->in_plane, data->in_stride, data->in_height, data->in_width);
    return NULL;
}

static struct PlaneData *plane_data_init(struct PlaneData *data, struct VSFrame *out, struct VSFrame *in, int plane) {
    const struct VSPixelDescription *desc = vs_pixel_description_get(out->meta.format);
    data->in_plane = in->planes[plane];
    data->in_width = in->meta.width;
    data->in_height = in->meta.height >> (plane == 0 ? 0 : desc->log2_vertical_chroma_ss);
    data->in_stride = in->stride[plane];
    data->out_plane = out->planes[plane];
    data->out_width = out->meta.width;
    data->out_height = out->meta.height >> (plane == 0 ? 0 : desc->log2_vertical_chroma_ss);
    data->out_stride = out->stride[plane];
    return data;
}

int vsf_nearest_neighbor_filter(struct VSFilter *filter, struct VSFrame *out, struct VSFrame *in) {
    // for now formats match
    assert(out->meta.format == in->meta.format);
    struct VSFilterNearestNeighbor *nn = (struct VSFilterNearestNeighbor*)filter;

    struct PlaneData luma, chroma_red, chroma_blue;
    struct VSThreadTask luma_task = {
        .user = plane_data_init(&luma, out, in, 0),
        .task = vsf_task_thread,
    };
    vs_thread_pool_schedule_task(&nn->pool, &luma_task);
    struct VSThreadTask chroma_red_task = {
            .user = plane_data_init(&chroma_red, out, in, 1),
            .task = vsf_task_thread,
    };
    vs_thread_pool_schedule_task(&nn->pool, &chroma_red_task);
    struct VSThreadTask chroma_blue_task = {
            .user = plane_data_init(&chroma_blue, out, in, 2),
            .task = vsf_task_thread,
    };
    vs_thread_pool_schedule_task(&nn->pool, &chroma_blue_task);

    while (!luma_task.finished || !chroma_red_task.finished || !chroma_blue_task.finished) {
        usleep(500);
    }

    return 0;
}

struct VSFilter *vsf_nearest_neighbor_alloc() {
    struct VSFilterNearestNeighbor *ret = malloc(sizeof *ret);
    ret->filter.free = vsf_nearest_neighbor_free;
    ret->filter.filter = vsf_nearest_neighbor_filter;
    ret->filter.c_obj.options_list = vsf_nearest_neighbor_options;
    ret->filter.c_obj.object_type_name = "nearest neighbor";
    vs_thread_pool_init(&ret->pool, 3);
    return (struct VSFilter*)ret;
}