//
// Created by felix on 4/15/24.
//

#include "frame.h"
#include "filtergraph.h"
#include "filters/x86/vsf_nn.h"
#include "log.h"
#include "options.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#define INPUT_W (1280)
#define INPUT_H (720)
#define OUTPUT_W (640)
#define OUTPUT_H (360)

int main(int argc, char **argv) {
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

    FILE *test_yuv_image = fopen("/home/felix/VideoScale/app/assets/torvalds-1280x720.yuv", "r");
    if (test_yuv_image == NULL)
        return -1;
    fread(frame.data.data, luma_size + (chroma_size * 2), 1, test_yuv_image);

    int filter_err = nnf->filter(nnf, &out, &frame);
    if (filter_err)
        vs_log("filter failed %i", filter_err);

    int windowWidth = OUTPUT_W;
    int windowHeight = OUTPUT_H;
    SDL_Window *window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, OUTPUT_W, OUTPUT_H);
    SDL_RenderClear(renderer);

    bool running = true;
    while (running) {
        SDL_Event e;
        if (SDL_PollEvent(&e) == 0) {
            continue;
        }

        switch (e.type) {
            case SDL_QUIT:
                running = false;
                continue;
        }

        SDL_UpdateYUVTexture(texture, NULL, out.planes[0], out.stride[0], out.planes[1],
                             out.stride[1], out.planes[2], out.stride[2]);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    nnf->free(&nnf);
    return 0;
}