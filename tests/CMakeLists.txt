cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

add_qx_test(BitsetTest BitsetTest.cpp .)
add_qx_test(DenseUnitaryMatrixTest DenseUnitaryMatrixTest.cpp .)
add_qx_test(SparseArrayTest SparseArrayTest.cpp .)
add_qx_test(QuantumStateTest QuantumStateTest.cpp .)
add_qx_test(ErrorModelsTest ErrorModelsTest.cpp .)
add_qx_test(RandomTest RandomTest.cpp .)
add_qx_test(IntegrationTest IntegrationTest.cpp .)
