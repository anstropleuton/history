cmake_minimum_required(VERSION 3.21)

project(
    glarens
    VERSION 0.1.0
    LANGUAGES CXX
)

include(GNUInstallDirs)

option(GLARENS_BUILD_TESTS "Build tests" OFF)
option(GLARENS_BUILD_EXAMPLES "Build examples" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompileShader)

file(GLOB_RECURSE GLARENS_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_library(glarens ${GLARENS_SOURCES})
add_library(glarens::glarens ALIAS glarens)

target_include_directories(glarens
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(glarens
    PUBLIC
        glm::glm
        SDL3::SDL3
)

if (GLARENS_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

if (GLARENS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(
    TARGETS glarens
    EXPORT glarensTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT glarensTargets
    FILE glarensTargets.cmake
    NAMESPACE glarens::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glarens
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/GlarensConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/glarensConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glarens
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/glarensConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/glarensConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/glarensConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glarens
)

include(CPack)

set(CPACK_PACKAGE_NAME "glarens")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP;TGZ")
