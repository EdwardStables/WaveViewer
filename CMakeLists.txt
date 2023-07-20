cmake_minimum_required(VERSION 3.10)
project(WaveViewer)
set(CMAKE_CXX_STANDARD 17)

set(VCD_PARSER_DIR vcd_parse)

add_executable(wavey
    src/main.cpp
)

add_subdirectory(${VCD_PARSER_DIR})
target_link_libraries(wavey PUBLIC ${VCD_PARSER_DIR})

target_include_directories( wavey
  PRIVATE headers
  PRIVATE ${VCD_PARSER_DIR}/include
)



