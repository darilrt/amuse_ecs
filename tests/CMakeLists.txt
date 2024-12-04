cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(amuse_ecs_tests)

include(FetchContent)
FetchContent_Declare(
    amuse_utest
    GIT_REPOSITORY https://github.com/darilrt/amuse_utest.git
)
FetchContent_MakeAvailable(amuse_utest)

add_executable(amuse_ecs_tests tests.cpp)
target_link_libraries(amuse_ecs_tests PRIVATE amuse_ecs)
target_link_libraries(amuse_ecs_tests PRIVATE amuse_utest)

add_test(NAME amuse_ecs_tests COMMAND amuse_ecs_tests)
