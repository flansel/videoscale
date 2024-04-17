add_executable(test_frame test_frame.c)
target_link_libraries(test_frame videoscale)

add_executable(test_buffer test_buffer.c)
target_link_libraries(test_buffer videoscale)

add_executable(test_queue test_queue.c)
target_link_libraries(test_queue videoscale)