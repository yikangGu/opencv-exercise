cmake_minimum_required(VERSION 2.8)

project(L3H2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

add_executable(L3H2 src/L3H2.cpp)
target_link_libraries(L3H2 ${OpenCV_LIBS})
