cmake_minimum_required(VERSION 3.10)
project(WaveViewer)
set(CMAKE_CXX_STANDARD 17)

set(VCD_PARSER_DIR vcd_parse)

add_executable(wavey
    src/main.cpp
    src/wave_viewer.cpp
    src/manager.cpp
    src/pane_base.cpp
    src/panes.cpp
    src/olcPixelGameEngine.cpp
)

add_subdirectory(${VCD_PARSER_DIR})
target_link_libraries(wavey PUBLIC ${VCD_PARSER_DIR})

target_include_directories( wavey
  PRIVATE headers
  PRIVATE ${VCD_PARSER_DIR}/include
)

find_package(X11 REQUIRED)
target_link_libraries(wavey PUBLIC X11::X11)
include_directories(${X11_INCLUDE_DIRS})
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(wavey PUBLIC ${OPENGL_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(wavey PUBLIC Threads::Threads)
include_directories(${Threads_INCLUDE_DIRS})
find_package(PNG REQUIRED)
target_link_libraries(wavey PUBLIC PNG::PNG)
include_directories(${PNG_INCLUDE_DIRS})
target_link_libraries(wavey PUBLIC stdc++fs)

