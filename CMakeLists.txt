CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PROJECT_NAME "qx-simulator")
PROJECT(${PROJECT_NAME})

enable_testing()

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE(OPENMP_FOUND)
  message("ERROR: OpenMP could not be found.")
ENDIF(OPENMP_FOUND)

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  add_compile_options(-O3 -msse4.2 -std=c++11 -Wall -Wfatal-errors -DCG_BC -DQX_SPARSE_MV_MUL -D__BUILTIN_LINALG__ -DUSE_OPENMP -Wno-sign-compare -Wno-unused-variable -Wno-reorder)
  if(USE_GPERFTOOLS)
    add_compile_options(-g3)
  endif()
ENDIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  add_compile_options(-O3 -qopenmp -qopenmp-simd -g -traceback -debug all -qopt-report=5 -ipo -xHost -std=c++11 -no-prec-div -Wall -Wfatal-errors -DCG_BC -DQX_SPARSE_MV_MUL -D__BUILTIN_LINALG__ -DUSE_OPENMP -Wno-reorder -Wno-unused-variable -Wno-unused-function)
  if(USE_GPERFTOOLS)
    add_compile_options(-g -debug all -O3 -traceback)
  endif()
ENDIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CXX "Clang-omp++")
  add_compile_options(-O3 -std=c++11 -Wall -Wfatal-errors -Wno-unused-local-typedef -O3 -DCG_BC -DQX_SPARSE_MV_MUL -D__BUILTIN_LINALG__ -msse4.2 -DUSE_OPENMP -Wno-reorder -Wno-unneeded-internal-declaration -Wno-unused-variable -Wno-unused-private-field -Wno-deprecated-register -Wno-unused-function)
ENDIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")


add_compile_options(-DXPU_TIMER)

ADD_SUBDIRECTORY(
  src bin
)
