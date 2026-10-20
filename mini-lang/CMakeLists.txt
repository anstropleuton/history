cmake_minimum_required(VERSION 3.14)
project(mini_lang VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# enable warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()

option(ENABLE_SANITIZERS "Enable ASan/UBSan sanitizer flags" OFF)

if(ENABLE_SANITIZERS AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    message(STATUS "Sanitizers enabled")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Build library (all implementation files except main.cpp)
set(SRC_IMPL
    src/lexer.cpp
    src/parser.cpp
    src/typechecker.cpp
    src/interpreter.cpp
    src/ast_printer.cpp
)

add_library(mini_lib STATIC ${SRC_IMPL})
target_include_directories(mini_lib PUBLIC include)

add_executable(mini_lang src/main.cpp)
target_link_libraries(mini_lang PRIVATE mini_lib)

# --- doctest FetchContent for tests ---
include(FetchContent)
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.12 # choose an appropriate stable tag
)
FetchContent_MakeAvailable(doctest)

# Tests
enable_testing()
add_executable(tests
  tests/test_main.cpp
  tests/test_basic.cpp
  tests/test_control_flow.cpp
  tests/test_errors.cpp
  tests/test_functions.cpp
  tests/test_arrays.cpp
  tests/test_tuples.cpp
  tests/test_maps.cpp
  tests/test_util.h  # header-only helper is included in the target
)
target_link_libraries(tests PRIVATE mini_lib doctest::doctest)
add_test(NAME basic_tests COMMAND tests)
