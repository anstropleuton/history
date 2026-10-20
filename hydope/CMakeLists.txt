cmake_minimum_required(VERSION 3.15)

project(Hydope)

find_package(raylib REQUIRED)

add_executable(hydope main.cpp pendulum.cpp utilities.cpp)
target_link_libraries(hydope PRIVATE raylib)
