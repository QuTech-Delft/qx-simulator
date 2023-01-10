cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

add_qx_test(BitsetTest BitsetTest.cc .)
target_link_libraries(BitsetTest PRIVATE doctest)

add_qx_test(DenseUnitaryMatrixTest DenseUnitaryMatrixTest.cc .)
target_link_libraries(DenseUnitaryMatrixTest PRIVATE doctest)

add_qx_test(SparseArrayTest SparseArrayTest.cc .)
target_link_libraries(SparseArrayTest PRIVATE doctest)

add_qx_test(QuantumStateTest QuantumStateTest.cc .)
target_link_libraries(QuantumStateTest PRIVATE doctest)

add_qx_test(test_multiple_execution qxelarator/test_multiple_execution.cc qxelarator)