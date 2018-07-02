INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  qcode
  xpu-0.1.5
)


FIND_LIBRARY(liblexgram PATHS ${CMAKE_CURRENT_SOURCE_DIR})


ADD_EXECUTABLE(qx-server qx_server.cc)
ADD_EXECUTABLE(qx-simulator-old simulator_old.cc)
ADD_EXECUTABLE(qx-simulator simulator.cc)

TARGET_LINK_LIBRARIES(qx-simulator liblexgram)
