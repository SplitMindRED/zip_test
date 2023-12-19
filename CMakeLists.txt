cmake_minimum_required(VERSION 3.0.0)
project(zip_test VERSION 0.1.0)

set(CMAKE_BUILD_TYPE "Release")

find_package(OpenCV REQUIRED)
find_package(ZLIB)


include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(zip_test main.cpp)

target_link_libraries(zip_test pthread ZLIB::ZLIB ${OpenCV_LIBS})