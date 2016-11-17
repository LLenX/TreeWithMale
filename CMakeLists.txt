cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER  clang)

project(TreeWithMale)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/person PERSON_SOURCE)

include_directories(include/)

add_executable(TestGetAncestor
        ${PROJECT_SOURCE_DIR}/test/person/TestGetAncestor.cpp
        ${PERSON_SOURCE})

#add_executable(TreeWithMale ${SOURCE_FILES})