cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER  clang)

project(TreeWithMale)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB_RECURSE SOURCE_FILES *.cpp)
include_directories(include/)

add_executable(TreeWithMale ${SOURCE_FILES})