cmake_minimum_required(VERSION 2.8)

project(L1H1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(OpenCV_DIR F:\\Privacy\\ProgramFile\\Github\\opencv\\install)
find_package(OpenCV REQUIRED)

add_executable(L1H1 src/L1H1.cpp)
target_link_libraries(L1H1 ${OpenCV_LIBS})
