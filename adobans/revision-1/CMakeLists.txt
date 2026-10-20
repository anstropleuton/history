cmake_minimum_required(VERSION 3.15)

project(Adobans1
    DESCRIPTION "Adobans v1 is a small programming language designed to be a subset of Engenide."
    LANGUAGES CXX
)

set(ADOBANS1_SOURCES
    "src/evaluator.cpp"
    "src/operator.cpp"
    "src/parser.cpp"
    "src/tokenizer.cpp"
)

set(ADOBANS1_EXE_SOURCES
    "src/main.cpp"
)

add_library(adobans1 ${ADOBANS1_SOURCES})
target_compile_features(adobans1 PUBLIC cxx_std_23)
target_include_directories(adobans1 PUBLIC "include")

add_executable(adobans ${ADOBANS1_EXE_SOURCES})
target_link_libraries(adobans PRIVATE adobans1)
