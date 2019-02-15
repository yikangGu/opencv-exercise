cmake_minimum_required(VERSION 2.8)

project(L1T1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

add_executable(L1T1 src/L1T1.cpp)
target_link_libraries(L1T1 ${OpenCV_LIBS})
