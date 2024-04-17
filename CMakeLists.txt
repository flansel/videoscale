cmake_minimum_required(VERSION 3.22)
project(videoscale C)

set(CMAKE_C_STANDARD 11)
set(SOURCES
    buffer.c
    filtergraph.c
    frame.c
    options.c
    pixelformat.c
    queue.c
    threadpool.c
    videoscale.c

    filters/vsf_sink.c
    filters/x86/common.c
    filters/x86/vsf_lowpass.c
    filters/x86/vsf_nn.c)

add_library(videoscale SHARED ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(tests)
add_subdirectory(app)
add_subdirectory(tests/benchmarks)