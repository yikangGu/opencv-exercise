cmake_minimum_required(VERSION 2.8)

project(L1H2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

add_executable(split src/split.cpp)
target_link_libraries(split ${OpenCV_LIBS})

add_executable(inRange src/inRange.cpp)
target_link_libraries(inRange ${OpenCV_LIBS})

add_executable(edge src/edge.cpp)
target_link_libraries(edge ${OpenCV_LIBS})
