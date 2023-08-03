cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(QX C CXX)

# If QX was already included elsewhere in the project, don't include it
# again. There should be only one place for it and one version per project.
if(NOT TARGET qx)

# Loads up the appropriate directories for installing stuff.
include(GNUInstallDirs)

# Packages
include(FetchContent)


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

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

option(
    QX_CPU_COMPATIBILITY_MODE
    "Produce binaries that would work on any CPU, instead of just the one of the machine on which the code is built"
    OFF
)

option(
    QX_BUILD_TESTS
    "Whether the tests should be built and added to `make test`"
    OFF
)

option(
    QX_BUILD_PYTHON
    "Whether the Python module should be built"
    OFF
)
mark_as_advanced(QX_BUILD_PYTHON)

set(
    QX_PYTHON_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/python/qxelarator"
    CACHE STRING "Where to install the Python library"
)
mark_as_advanced(QX_PYTHON_DIR)

set(
    QX_PYTHON_EXT ""
    CACHE STRING "Basename for the Python extension, or \"\" to let CMake's SWIG implementation handle it"
)
mark_as_advanced(QX_PYTHON_EXT)

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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Windows weirdness: need a .lib file to link against a DLL at compile-time
# (I think), but only the .dll is generated when there are no exported symbols.
# This sort of fixes that (https://stackoverflow.com/questions/1941443)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


#=============================================================================#
# Dependencies                                                                #
#=============================================================================#

message(STATUS "Fetching abseil")
FetchContent_Declare(abseil
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG "c2435f8342c2d0ed8101cb43adfd605fdc52dca2"
)
FetchContent_MakeAvailable(abseil)

message(STATUS "Fetching cqasm")
FetchContent_Declare(cqasm
    GIT_REPOSITORY https://github.com/QuTech-Delft/libqasm.git
    GIT_TAG "8d0b4ed5e3a30aa6fde3b3aaf20a207b5d07876e"
)
FetchContent_MakeAvailable(cqasm)


#=============================================================================#
# QX library target                                                           #
#=============================================================================#

add_library(qx
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/SimulationResult.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/Circuit.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/ErrorModels.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/Qxelarator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/Random.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/Simulator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/LibqasmInterface.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/qx/Gates.cpp"
)

target_include_directories(qx
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(qx PRIVATE -Wall -Wfatal-errors)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(qx PRIVATE -Wall -Wfatal-errors -Wno-unused-local-typedef)
elseif(MSVC)
    target_compile_options(qx PRIVATE /MP /EHsc /bigobj)
else()
    message(SEND_ERROR "Unknown compiler!")
endif()

if(!${QX_CPU_COMPATIBILITY_MODE})
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(qx PUBLIC -march=native)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
        target_compile_options(qx PUBLIC -ipo -xHost -no-prec-div)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(qx PUBLIC -march=native)
    endif()
endif()


#=============================================================================#
# Configure, build, and link dependencies                                     #
#=============================================================================#

option(LIBQASM_COMPAT "" ON)
target_link_libraries(qx PUBLIC cqasm)

set(ABSL_PROPAGATE_CXX_STD ON)
target_link_libraries(qx PUBLIC absl::flat_hash_map)

#=============================================================================#
# Executables                                                                 #
#=============================================================================#

add_executable("qx-simulator" "${CMAKE_CURRENT_SOURCE_DIR}/src/qx-simulator/Simulator.cpp")
target_link_libraries(qx-simulator qx)

#=============================================================================#
# Testing                                                                     #
#=============================================================================#

if(QX_BUILD_TESTS)
    enable_testing()
    include(CTest)
    include(GoogleTest)
    set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
    add_subdirectory(tests)
endif()


#=============================================================================#
# Python module                                                               #
#=============================================================================#

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
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.cpp"
)
install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.cpp"
)


endif()  # NOT TARGET qx
