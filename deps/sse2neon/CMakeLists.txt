### Repo is https://github.com/DLTcollab/sse2neon

project(sse2neon)

add_library(sse2neon INTERFACE)
target_include_directories(sse2neon INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})