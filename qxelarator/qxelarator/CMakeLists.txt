CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PROJECT_NAME "qxelarator")

PROJECT(${PROJECT_NAME})
SET(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})



# FIND_PACKAGE(PythonLibs)
function(find_python)
    FIND_PACKAGE(PythonInterp)
    FIND_PACKAGE(PythonLibs)
    execute_process(
            COMMAND python -c "from __future__ import print_function; import numpy; print(numpy.get_include())"
            OUTPUT_VARIABLE Python_NumPy_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
            COMMAND python -c "from sysconfig import get_paths as gp; print(gp()['include'])"
            OUTPUT_VARIABLE Python_INCLUDE_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(COMMAND python -c "import sys; print(sys.executable)"
            RESULT_VARIABLE retval
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            OUTPUT_VARIABLE python_full_path
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    INCLUDE_DIRECTORIES(BEFORE ${Python_NumPy_PATH} ${Python_INCLUDE_DIR})
    LINK_LIBRARIES(${PYTHON_LIBRARY})
    MESSAGE(STATUS "Python_NumPy_PATH='${Python_NumPy_PATH}'")
    MESSAGE(STATUS "Python_INCLUDE_DIR='${Python_INCLUDE_DIR}'")
    MESSAGE(STATUS "PYTHON_LIBRARY='${PYTHON_LIBRARY}'")

endfunction(find_python)


find_python()


EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_config_var('LDSHARED'))"
        RESULT_VARIABLE PYTHON_CVPY_PROCESS
        OUTPUT_VARIABLE PYTHON_LDSHARED
        OUTPUT_STRIP_TRAILING_WHITESPACE)
IF ("${PYTHON_LDSHARED}" MATCHES "dynamic_lookup")
    MESSAGE("Using static linking for Python")
    SET(PYTHON_LINK_LIBRARIES "NO")
ELSE()
    SET(PYTHON_LINK_LIBRARIES "${PYTHON_LIBRARIES}")
ENDIF()




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
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wfatal-errors -ggdb")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -Wfatal-errors -Wno-unused-local-typedef -ggdb")
ENDIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

SET_SOURCE_FILES_PROPERTIES(qxelarator.i PROPERTIES CPLUSPLUS ON)
SET_PROPERTY(SOURCE qxelarator.i PROPERTY SWIG_FLAGS -castmode -modern -keyword)

# SWIG_ADD_MODULE(qxelarator python qxelarator.i)
SWIG_ADD_LIBRARY( qxelarator
                  TYPE MODULE
                  LANGUAGE python
                  SOURCES qxelarator.i
                )

SWIG_LINK_LIBRARIES(qxelarator ${PYTHON_LIBRARY} lexgram)
# SWIG_LINK_LIBRARIES(qxelarator ${PYTHON_LIBRARIES} lexgram)

# IF (NOT PYTHON_LINK_LIBRARIES)
#     IF (APPLE)
#         SET_TARGET_PROPERTIES(qxelarator PROPERTIES LINK_FLAGS "-Wl,-flat_namespace,-undefined,dynamic_lookup")
#     ENDIF()
# ENDIF()


# add_library(qx INTERFACE)
# set_target_properties(qx PROPERTIES
#     INTERFACE_LINK_LIBRARIES
#         lexgram
#     INTERFACE_LINK_LIBRARIES
#         qxelarator
# )


