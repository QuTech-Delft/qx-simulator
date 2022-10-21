cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(QX C CXX)

# If QX was already included elsewhere in the project, don't include it
# again. There should be only one place for it and one version per project.
if(NOT TARGET qx)

# Loads up the appropriate directories for installing stuff.
include(GNUInstallDirs)


#=============================================================================#
# Configuration options                                                       #
#=============================================================================#

# Library type option. Default is a shared object, because for CMake it doesn't
# matter, but outside of CMake dependency information is lost for static
# libraries. That requires the user to link all of QX's direct and transitive
# dependencies as well, which is terribly ugly. setup.py *has* to do this
# however, because "pip install ." builds this in a temporary directory, so the
# shared objects that get built and installed and are then depended on by the
# Python lib get deleted by pip after the install.
option(
    BUILD_SHARED_LIBS
    "Whether libraries should be built as a shared object or as a static library"
    OFF
)

# With what optimization level the library is to be built.
if(NOT CMAKE_BUILD_TYPE)
    set(
        CMAKE_BUILD_TYPE Release CACHE STRING
        "Type of build (Debug, Release)" FORCE
    )
    set_property(
        CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release"
    )
endif()

# Compatibility mode for building binaries that should work on basically any
# CPU, rather than trying to optimize for the host architecture.
option(
    QX_CPU_COMPATIBILITY_MODE
    "Don't assume availability of instruction set extensions beyond SSE3."
    OFF
)

# Whether tests should be built.
option(
    QX_BUILD_TESTS
    "Whether the tests should be built and added to `make test`"
    OFF
)

# Whether the Python module should be built. This should only be enabled for
# setup.py's builds.
option(
    QX_BUILD_PYTHON
    "Whether the Python module should be built"
    OFF
)
mark_as_advanced(QX_BUILD_PYTHON)

# Where the Python module should be built.
set(
    QX_PYTHON_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/python/qxelarator"
    CACHE STRING "Where to install the Python library"
)
mark_as_advanced(QX_PYTHON_DIR)

# Used to override the (base)name of the Python extension.
set(
    QX_PYTHON_EXT ""
    CACHE STRING "Basename for the Python extension, or \"\" to let CMake's SWIG implementation handle it"
)
mark_as_advanced(QX_PYTHON_EXT)

option(
    QX_USE_OPENMP
    "Whether OpenMP should be used"
    ON
)
mark_as_advanced(QX_USE_OPENMP)

if(APPLE AND QX_USE_OPENMP)
    message(WARNING "OpenMP unsupported on MacOS, deactivating")
    set(QX_USE_OPENMP OFF)
endif()

#=============================================================================#
# CMake weirdness and compatibility                                           #
#=============================================================================#

# On Windows builds, CMake complains that the CMake internal variable
# "CMAKE_MODULE_LINKER_FLAGS_MAINTAINER" is not defined *the first time you
# configure*. Weirdly, all is good with the world if you then just try again.
# It seems to have to do with the "maintainer" build type in MSVC, but there
# is no documentation whatsoever. In any case, this just mimics what CMake
# does automatically the second time you invoke it, so it works around the
# issue.
if(NOT DEFINED CMAKE_MODULE_LINKER_FLAGS_MAINTAINER)
    set(
        CMAKE_MODULE_LINKER_FLAGS_MAINTAINER ""
        CACHE STRING "Flags used by the linker during the creation of modules during MAINTAINER builds."
    )
endif()


#=============================================================================#
# Global build configuration                                                  #
#=============================================================================#

# Since we have multiple libraries to link together, we unfortunately have to
# worry about RPATH handling on Linux and OSX. See
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Windows weirdness: need a .lib file to link against a DLL at compile-time
# (I think), but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


#=============================================================================#
# QX library target                                                           #
#=============================================================================#

# Create the QX library. This will be built either as a shared object/DLL
# or as a static library based on BUILD_SHARED_LIBS; add_library switches
# automatically.
add_library(qx
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/core/register.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/core/linalg.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/core/gaussian.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/xpu/complex.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/xpu/net/active_socket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/xpu/net/basic_socket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/xpu/net/socket_exception.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/xpu/net/tcp_server_socket.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/xpu/net/tcp_socket.cc"
)

# This definition is used to define QX_DECLSPEC for __declspec. More info:
# https://docs.microsoft.com/en-us/cpp/cpp/declspec?view=vs-2019
target_compile_definitions(qx PRIVATE BUILDING_QX)

# FIXME: these should be removed or added to a global config header file!
target_compile_definitions(qx PUBLIC XPU_TIMER CG_BC QX_SPARSE_MV_MUL __BUILTIN_LINALG__)

if(QX_USE_OPENMP)
    target_compile_definitions(qx PUBLIC USE_OPENMP)
endif()

# Add the src directory as a private include path and include as the public
# include path.
target_include_directories(qx
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src/"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include/"
)

# Configure compilation.
set_property(TARGET qx PROPERTY POSITION_INDEPENDENT_CODE ON)
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(qx PRIVATE -Wall -Wfatal-errors -ggdb)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(qx PRIVATE -Wall -Wfatal-errors -ggdb -Wno-unused-local-typedef)
elseif(MSVC)
    target_compile_options(qx PRIVATE /MP /D_USE_MATH_DEFINES /EHsc /bigobj)
else()
    message(SEND_ERROR "Unknown compiler!")
endif()

# Enable optimizations only for release builds.
if(NOT MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    target_compile_options(qx PRIVATE -O3)
endif()

# Compiler-specific and architectural optimizations.
if(${QX_CPU_COMPATIBILITY_MODE})

    # Only assume x86-64, which has at least SSE3.
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(qx PUBLIC -march=nocona)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(qx PUBLIC -march=nocona)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        target_compile_options(qx PUBLIC /arch:SSE3)
    else()
        message(SEND_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()

else()

    # Use newer instruction set extensions when supported.
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(qx PUBLIC -march=native -mtune=native)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
        target_compile_options(qx PUBLIC -ipo -xHost -no-prec-div)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(qx PUBLIC -march=native -mtune=native)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        # NOTE: this is making assumptions about what the host can do...
        target_compile_options(qx PUBLIC /arch:AVX)
    else()
        message(SEND_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()

endif()


#=============================================================================#
# Configure, build, and link dependencies                                     #
#=============================================================================#

# OpenMP
if(QX_USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(qx PUBLIC OpenMP::OpenMP_CXX)
endif()

# libqasm
option(LIBQASM_COMPAT "" ON)
add_subdirectory(deps/libqasm)
target_link_libraries(qx PUBLIC cqasm)

# winsock
if(WIN32)
    target_link_libraries(qx PUBLIC wsock32 ws2_32)
endif()


#=============================================================================#
# Executables                                                                 #
#=============================================================================#

# qx-simulator
add_executable("qx-simulator" "${CMAKE_CURRENT_SOURCE_DIR}/src/qx-simulator/simulator.cc")
target_link_libraries(qx-simulator qx)

# qx-server
add_executable("qx-server" "${CMAKE_CURRENT_SOURCE_DIR}/src/qx-server/server.cc")
target_link_libraries(qx-server qx)


#=============================================================================#
# Testing                                                                     #
#=============================================================================#

# Include the tests directory if requested.
if(QX_BUILD_TESTS)
    enable_testing()

    # Convenience function to add a test.
    function(add_qx_test name source workdir)
        add_executable("${name}" "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
        target_link_libraries("${name}" PRIVATE qx)
        add_test(
            NAME "${name}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${workdir}"
            COMMAND "${name}"
        )
    endfunction()

    add_subdirectory(deps/doctest)

    # Include the directories containing tests.
    add_subdirectory(tests)

endif()


#=============================================================================#
# Python module                                                               #
#=============================================================================#

# Include the tests directory if requested.
if(QX_BUILD_PYTHON)
    add_subdirectory(python)
endif()


#=============================================================================#
# Installation                                                                #
#=============================================================================#

# Install targets for the QX library.
install(
    TARGETS qx
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.cc"
)
install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.cc"
)


endif() # NOT TARGET qx
