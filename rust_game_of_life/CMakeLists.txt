cmake_minimum_required(VERSION 3.15)

project(GameOfLife LANGUAGES CXX)

find_package(raylib REQUIRED)

add_executable(gameoflife gameoflife.cpp)
target_link_libraries(gameoflife PRIVATE raylib)
