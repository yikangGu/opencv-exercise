cmake_minimum_required(VERSION 2.8)

project(L1H3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

add_executable(L1H3 src/L1H3.cpp)
target_link_libraries(L1H3 ${OpenCV_LIBS})
