INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  xpu-0.1.5
  parser/libqasm/src/library
  qcode
)

ADD_SUBDIRECTORY(
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/libqasm/src/library
)

ADD_EXECUTABLE(qx-server qx_server.cc)
ADD_EXECUTABLE(qx-simulator-old simulator_old.cc)
ADD_EXECUTABLE(qx-simulator simulator.cc)

if(USE_GPERFTOOLS)
  target_link_libraries(qx-simulator profiler)
  message("-- Compiling with gperftools: yes")
else()
  message("-- Compiling with gperftools: no")
endif()

TARGET_LINK_LIBRARIES(qx-simulator lexgram)
