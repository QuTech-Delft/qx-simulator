INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  qcode
  xpu-0.1.5
)

ADD_SUBDIRECTORY(parser)

ADD_EXECUTABLE(qx-server qx_server.cc)
ADD_EXECUTABLE(qx-simulator-old simulator_old.cc)
ADD_EXECUTABLE(qx-simulator simulator.cc)

ADD_DEPENDENCIES(qx-simulator cqasmlexgram)
TARGET_LINK_LIBRARIES(qx-simulator cqasmlexgram)
