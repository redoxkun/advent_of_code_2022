cmake_minimum_required(VERSION 3.23)
project(advent_of_code_2022)

set(CMAKE_CXX_STANDARD 23)

find_package(fmt)

add_executable(day1 day1/main.cpp)
target_link_libraries(day1 fmt::fmt-header-only)
