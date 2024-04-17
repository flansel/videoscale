//
// Created by felix on 4/17/24.
//

#include "benchmark.h"
#include "options.h"
#include "frame.h"
#include "filters/x86/vsf_nn.h"
#include "filtergraph.h"
#include "log.h"

#include <time.h>

#define INPUT_W (1280)
#define INPUT_H (720)
#define OUTPUT_W (1920)
#define OUTPUT_H (1080)

int main(void) {
    struct VSFrame frame, out;
    frame.meta.width = INPUT_W;
    frame.meta.height = INPUT_H;
    frame.meta.format = VSPixelFormatYUV420P;
    vs_frame_alloc_buffer(&frame);

    out.meta.width = OUTPUT_W;
    out.meta.height = OUTPUT_H;
    out.meta.format = VSPixelFormatYUV420P;
    vs_frame_alloc_buffer(&out);

    struct VSFilter *nnf = vsf_nearest_neighbor_alloc();
    vs_set_option_int(nnf, "width", OUTPUT_W);
    vs_set_option_int(nnf, "height", OUTPUT_H);

    int luma_size = INPUT_W * INPUT_H;
    int chroma_size = INPUT_W / 2 * INPUT_H / 2;

    FILE *test_yuv_image = fopen("app/assets/torvalds-1280x720.yuv", "r");
    if (test_yuv_image == NULL)
        return -1;
    fread(frame.data.data, luma_size + (chroma_size * 2), 1, test_yuv_image);

    struct VSStatTracker stats;
    vs_stat_tracker_init(&stats);

    struct timespec start, end;
    int runs = 0;
    while (runs < 1000) {
        clock_gettime(CLOCK_REALTIME, &start);
        int filter_err = nnf->filter(nnf, &out, &frame);
        if (filter_err)
            vs_log("filter failed %i", filter_err);
        clock_gettime(CLOCK_REALTIME, &end);
        double time_elapsed = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1e9);
        vs_stat_tracker_push_data(&stats, time_elapsed);
        runs++;
    }

    vs_stat_tracker_log_result(&stats);
}