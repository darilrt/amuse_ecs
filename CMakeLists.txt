cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AmuseECS)
set(project_name AmuseECS)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(include)

add_executable(${project_name} ${SOURCES})