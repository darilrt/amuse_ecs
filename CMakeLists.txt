cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(amuse_ecs)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(amuse_ecs STATIC ${SOURCES})
target_include_directories(amuse_ecs PUBLIC include)

if(MSVC)
    target_compile_options(amuse_ecs INTERFACE /W4)
else()
    target_compile_options(amuse_ecs INTERFACE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_subdirectory(tests)