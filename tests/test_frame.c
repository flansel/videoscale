//
// Created by felix on 4/15/24.
//

#include "unit_test.h"
#include "frame.h"

TEST(test_yuv420p,
    struct VSFrame frame;
    frame.meta.width = 1280;
    frame.meta.height = 720;
    frame.meta.format = VSPixelFormatYUV420P;

    vs_frame_alloc_buffer(&frame);
    CHECK(frame.stride[0] == 1280);
    CHECK(frame.stride[1] == 640);
    CHECK(frame.stride[2] == 640);
    CHECK(frame.stride[3] == 0);

    CHECK(frame.planes[0] == frame.data.data);
    CHECK(frame.planes[1] == frame.planes[0] + (frame.stride[0] * frame.meta.height));
    CHECK(frame.planes[2] == frame.planes[1] + (frame.stride[1] * frame.meta.height / 2));
    CHECK(frame.planes[3] == 0);
)

TEST(test_yuv422p,
     struct VSFrame frame;
     frame.meta.width = 1280;
     frame.meta.height = 720;
     frame.meta.format = VSPixelFormatYUV422P;

     vs_frame_alloc_buffer(&frame);
     CHECK(frame.stride[0] == 1280);
     CHECK(frame.stride[1] == 640);
     CHECK(frame.stride[2] == 640);
     CHECK(frame.stride[3] == 0);

     CHECK(frame.planes[0] == frame.data.data);
     CHECK(frame.planes[1] == frame.planes[0] + (frame.stride[0] * frame.meta.height));
     CHECK(frame.planes[2] == frame.planes[1] + (frame.stride[1] * frame.meta.height));
     CHECK(frame.planes[3] == 0);
)

TEST(test_yuv444p,
     struct VSFrame frame;
     frame.meta.width = 1280;
     frame.meta.height = 720;
     frame.meta.format = VSPixelFormatYUV444P;

     vs_frame_alloc_buffer(&frame);
     CHECK(frame.stride[0] == 1280);
     CHECK(frame.stride[1] == 1280);
     CHECK(frame.stride[2] == 1280);
     CHECK(frame.stride[3] == 0);

     CHECK(frame.planes[0] == frame.data.data);
     CHECK(frame.planes[1] == frame.planes[0] + (frame.stride[0] * frame.meta.height));
     CHECK(frame.planes[2] == frame.planes[1] + (frame.stride[1] * frame.meta.height));
     CHECK(frame.planes[3] == 0);
)

UNIT(frame.c,
     test_yuv420p,
     test_yuv422p,
     test_yuv444p)