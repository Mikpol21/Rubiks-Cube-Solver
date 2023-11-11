cmake_minimum_required(VERSION 3.13)
project(Rubiks_Cube_Solver)

set(CMAKE_CXX_STANDARD 20)
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
find_package(Threads REQUIRED)

add_executable(
    rubiks_cube_test RubiksCubeTest.cpp RubiksCube.cpp
)

add_executable(
    main main.cpp RubiksCube.cpp
)

add_compile_definitions("SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build.")
    add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address)
    add_definitions(-DDEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build.")
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

target_link_libraries(
    rubiks_cube_test
    gtest
    gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(rubiks_cube_test)