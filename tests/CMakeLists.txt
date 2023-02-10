cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

add_qx_test(BitsetTest BitsetTest.cpp .)
target_link_libraries(BitsetTest PRIVATE doctest)

add_qx_test(DenseUnitaryMatrixTest DenseUnitaryMatrixTest.cpp .)
target_link_libraries(DenseUnitaryMatrixTest PRIVATE doctest)

add_qx_test(SparseArrayTest SparseArrayTest.cpp .)
target_link_libraries(SparseArrayTest PRIVATE doctest)

add_qx_test(QuantumStateTest QuantumStateTest.cpp .)
target_link_libraries(QuantumStateTest PRIVATE doctest)

add_qx_test(ErrorModelsTest ErrorModelsTest.cpp .)
target_link_libraries(ErrorModelsTest PRIVATE doctest)

add_qx_test(RandomTest RandomTest.cpp .)
target_link_libraries(RandomTest PRIVATE doctest)

add_qx_test(IntegrationTest IntegrationTest.cpp .)
target_link_libraries(IntegrationTest PRIVATE doctest)