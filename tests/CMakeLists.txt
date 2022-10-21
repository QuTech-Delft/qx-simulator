cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

add_qx_test(test_multiple_execution qxelarator/test_multiple_execution.cc qxelarator)

add_qx_test(gatetest gatetest.cc .)
target_link_libraries(gatetest PRIVATE doctest)