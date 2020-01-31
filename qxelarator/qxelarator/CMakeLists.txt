CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PROJECT_NAME "qxelarator")

PROJECT(${PROJECT_NAME})
SET(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})


FIND_PACKAGE(PythonInterp)
if (${PYTHONINTERP_FOUND})
    FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
endif()


## These are the include directories used by the compiler.
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../src
    ${PROJECT_SOURCE_DIR}/../../src/xpu-0.1.5
    ${PROJECT_SOURCE_DIR}/../../src/parser/libqasm/qasm_flex_bison/library
    ${PYTHON_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

LINK_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/../libqasmbuild/bin/parser/libqasm/qasm_flex_bison/library
  )

message(${PROJECT_SOURCE_DIR}/../libqasmbuild/bin/parser/libqasm/qasm_flex_bison/library)

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE(OPENMP_FOUND)
  message( WARNING "OpenMP could not be found.")
ENDIF(OPENMP_FOUND)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wfatal-errors -O3 -DCG_BC -DQX_SPARSE_MV_MUL -D__BUILTIN_LINALG__ -msse4.2 -DUSE_OPENMP")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wfatal-errors -ggdb")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -Wfatal-errors -Wno-unused-local-typedef -ggdb")
ENDIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

SET_SOURCE_FILES_PROPERTIES(qxelarator.i PROPERTIES CPLUSPLUS ON)
SET_PROPERTY(SOURCE qxelarator.i PROPERTY SWIG_FLAGS -castmode -modern -keyword)


if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
  SWIG_ADD_MODULE(qxelarator python qxelarator.i)
else()
  SWIG_ADD_LIBRARY( qxelarator
                    TYPE MODULE
                    LANGUAGE python
                    SOURCES qxelarator.i
                  )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SWIG_LINK_LIBRARIES(qxelarator lexgram)
    set_target_properties(_qxelarator PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
else ()
    SWIG_LINK_LIBRARIES(qxelarator ${PYTHON_LIBRARIES} lexgram)
endif()
