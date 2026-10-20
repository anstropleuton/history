cmake_minimum_required(VERSION 3.15)

project(Cyrine)

find_package(OpenSSL REQUIRED)

add_library(cyrine_lib cyrine.cpp)
target_compile_features(cyrine_lib PUBLIC cxx_std_23)
target_link_libraries(cyrine_lib PUBLIC OpenSSL::SSL OpenSSL::Crypto)

add_executable(cyrine main.cpp)
target_link_libraries(cyrine PRIVATE cyrine_lib)
