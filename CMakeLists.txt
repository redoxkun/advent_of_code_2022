cmake_minimum_required(VERSION 3.23)
project(advent_of_code_2022)

set(CMAKE_CXX_STANDARD 23)

find_package(fmt)

add_executable(day1 day1/main.cpp)
target_link_libraries(day1 fmt::fmt-header-only)

add_executable(day2 day2/main.cpp)
target_link_libraries(day2 fmt::fmt-header-only)

add_executable(day3 day3/main.cpp)
target_link_libraries(day3 fmt::fmt-header-only)

add_executable(day4 day4/main.cpp)
target_link_libraries(day4 fmt::fmt-header-only)

add_executable(day5 day5/main.cpp)
target_link_libraries(day5 fmt::fmt-header-only)

add_executable(day6 day6/main.cpp)
target_link_libraries(day6 fmt::fmt-header-only)

add_executable(day7 day7/main.cpp)
target_link_libraries(day7 fmt::fmt-header-only)
