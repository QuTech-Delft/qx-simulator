CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

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

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wfatal-errors -O3 -DCG_BC -DQX_SPARSE_MV_MUL -D__BUILTIN_LINALG__ -msse4.2 -DUSE_OPENMP")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CXX "Clang-omp++")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -Wfatal-errors -Wno-unused-local-typedef -O3 -DCG_BC -DQX_SPARSE_MV_MUL -D__BUILTIN_LINALG__ -msse4.2 -DUSE_OPENMP -Wno-reorder -Wno-unneeded-internal-declaration -Wno-unused-variable -Wno-unused-private-field -Wno-deprecated-register -Wno-unused-function")
ENDIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DXPU_TIMER")

ADD_SUBDIRECTORY(
  src bin
)
