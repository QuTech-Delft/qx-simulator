cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
if(POLICY CMP0078)
    cmake_policy(SET CMP0078 OLD)
endif()
if(POLICY CMP0086)
    cmake_policy(SET CMP0086 OLD)
endif()

#=============================================================================#
# Configure, build, and link dependencies                                     #
#=============================================================================#

# Python development headers --------------------------------------------------

# CMake's Python-finding logic is dodgy at best at dealing with venvs, and
# venvs themselves are dodgy at best at dealing with the development headers
# and so on. "If you want something done, do it yourself," I guess.

# If there is no override, look for the Python executable first. If there's
# a virtualenv, look in it first and foremost. PYTHON_EXECUTABLE can also be
# overridden with -DPYTHON_EXECUTABLE= outright.
if(DEFINED ENV{VIRTUAL_ENV})
    find_program(PYTHON_EXECUTABLE python3 HINTS "$ENV{VIRTUAL_ENV}/bin")
else()
    find_program(PYTHON_EXECUTABLE python3)
endif()

# If we found it, we can ask it where its include directory is, using the
# sysconfig module.
if(PYTHON_EXECUTABLE)
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_paths()[\"include\"])"
        OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # On Windows, we need to explicitly link against the Python library as
    # well. Unfortunately we can't ask Python directly where this is, but
    # we can figure it out
    if(WIN32)
        execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/python_lib.py" "${CMAKE_BUILD_TYPE}"
            OUTPUT_VARIABLE PYTHON_LIBRARIES
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
endif()

# If we failed to find the headers ourselves, fall back to CMake's internal
# logic.
if(NOT PYTHON_INCLUDE_DIRS)
    message(WARNING "Could NOT find python3 include directory with custom logic, falling back to CMake internal")
    unset(PYTHON_INCLUDE_DIRS)
    if("${CMAKE_VERSION}" VERSION_LESS "3.12.0")
        find_package(PythonInterp 3 REQUIRED)
        if(${PYTHONINTERP_FOUND})
            find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
        endif()
    else()
        find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
        set(PYTHON_INCLUDE_DIRS "${Python3_INCLUDE_DIRS}")
        set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}")
    endif()
endif()

# Print the info we found for reference.
message(STATUS "Found python3 include directory at ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Found python3 executable at ${PYTHON_EXECUTABLE}")
message(STATUS "Found python3 libraries at ${PYTHON_LIBRARIES}")


# SWIG ------------------------------------------------------------------------

# Look for SWIG.
find_package(SWIG REQUIRED)

include(${SWIG_USE_FILE})


#=============================================================================#
# Build the SWIG module                                                       #
#=============================================================================#

# Configure SWIG.
set(SWIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/qxelarator.i")

set_source_files_properties("${SWIG_FILE}" PROPERTIES
    CPLUSPLUS ON
    INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:qx,INTERFACE_INCLUDE_DIRECTORIES>;${PYTHON_INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/../src"
    SWIG_FLAGS "-castmode;-keyword"
)

set(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")

# Run SWIG to generated the C++ source file and Python wrapper module.
swig_add_library(
    qxelarator
    TYPE MODULE
    LANGUAGE python
    SOURCES "${SWIG_FILE}"
)

# Compile the C++ source file and link it against the QX library.
if(WIN32)
    swig_link_libraries(qxelarator "${PYTHON_LIBRARIES}" qx)
else()
    swig_link_libraries(qxelarator qx)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(_qxelarator PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()
target_include_directories(_qxelarator PRIVATE "${PYTHON_INCLUDE_DIRS}")


#=============================================================================#
# Construct & install the complete Python module                              #
#=============================================================================#

# Install the shared object (*.pyd on Windows).
if("${QX_PYTHON_EXT}" STREQUAL "")
    install(
        TARGETS _qxelarator
        LIBRARY DESTINATION "${QX_PYTHON_DIR}"
    )
else()
    install(
        FILES "$<TARGET_FILE:_qxelarator>"
        DESTINATION "${QX_PYTHON_DIR}"
        RENAME "${QX_PYTHON_EXT}"
    )
endif()

# Install the generated wrapper module.
install(
    FILES "${CMAKE_SWIG_OUTDIR}/qxelarator.py"
    DESTINATION "${QX_PYTHON_DIR}"
)
