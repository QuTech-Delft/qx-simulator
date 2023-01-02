cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

add_qx_test(test_multiple_execution qxelarator/test_multiple_execution.cc qxelarator)

add_qx_test(gatetest gatetest.cc .)
target_link_libraries(gatetest PRIVATE doctest)

add_qx_test(test_rand qxelarator/test_rand.cc qxelarator)
target_link_libraries(test_rand PRIVATE doctest)

add_qx_test(test_aligned_memory_allocator qxelarator/test_aligned_memory_allocator.cc qxelarator)
target_link_libraries(test_aligned_memory_allocator PRIVATE doctest)