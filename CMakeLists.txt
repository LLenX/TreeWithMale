cmake_minimum_required(VERSION 3.5)

project(TreeWithMale)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_BUILD_TYPE DEBUG)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/person PERSON_SOURCE)

include_directories(${PROJECT_SOURCE_DIR}/include/)

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_executable(TestPerson
            ${PROJECT_SOURCE_DIR}/test/person/PersonTest.cpp
            ${PERSON_SOURCE})

    set_target_properties(TestPerson PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)

    target_link_libraries(TestPerson gtest gtest_main pthread)

    add_custom_target(test_person
            COMMAND TestPerson
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin
            COMMENT "running test for person")

    add_executable(TestTree
            ${PROJECT_SOURCE_DIR}/test/TreeTest.cpp
            ${PERSON_SOURCE}
            ${PROJECT_SOURCE_DIR}/src/FamilyTree.cpp)
    set_target_properties(TestTree PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)
    target_link_libraries(TestTree gtest gtest_main pthread)

    add_custom_target(test_tree
            COMMAND TestTree
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin
            COMMENT "running test for tree")

    add_executable(PrintTree
            ${PROJECT_SOURCE_DIR}/test/utility/PrintTreeTest.cpp
            ${PERSON_SOURCE}
            ${PROJECT_SOURCE_DIR}/src/utility/PrintTree.cpp)
    set_target_properties(PrintTree PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)
    target_link_libraries(PrintTree)

    add_custom_target(print_tree
            COMMAND PrintTree
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin
            COMMENT "running test for tree")

endif ()

aux_source_directory(${PROJECT_SOURCE_DIR}/src/ SOURCE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/UI UI_SOURCE)

add_executable(TreeWithMale ${PERSON_SOURCE} ${SOURCE_FILES} ${UI_SOURCE})