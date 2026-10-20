cmake_minimum_required(VERSION 3.30)

project(minesweeper
    VERSION 2.1.0.2
    DESCRIPTION "Minesweeper is a password manager for Anstro Pleuton's passwords."
    LANGUAGES CXX
)

option(MINESWEEPER_BUILD_TESTS "Build Minesweeper tests" OFF)
option(MINESWEEPER_BUILD_EXAMPLES "Build Minesweeper examples" OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/depman.cmake")

depman_declare(confer "https://github.com/anstropleuton/confer.git" origin/main)
depman_declare(raylib "https://github.com/raysan5/raylib.git" origin/master)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(DOXYGEN_MINESWEEPER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/minesweeper_config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeper_config.hpp"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkgconfig.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeper.pc"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/header.html.in"
    "${CMAKE_CURRENT_BINARY_DIR}/header.html"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/footer.html.in"
    "${CMAKE_CURRENT_BINARY_DIR}/footer.html"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeperConfig.cmake"
    @ONLY
)

# Taken from GLFW
# Only generate this target if no higher-level project already has
if (NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    )
endif()

set(MINESWEEPER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/minesweeper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/minui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/minui_layouts.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/minui_ms.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/flux.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/flux_builtins.cpp"
)
set(MINESWEEPER_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeper_config.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/minesweeper.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/minui.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/minui_ms.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/flux.hpp"
)
set(MINESWEEPER_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
set(MINESWEEPER_EXE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

add_library(minesweeper_lib)
target_compile_features(minesweeper_lib PUBLIC cxx_std_23)
target_sources(minesweeper_lib PRIVATE ${MINESWEEPER_SOURCES})
target_sources(minesweeper_lib PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${MINESWEEPER_INCLUDE_DIRS}
    FILES ${MINESWEEPER_HEADERS}
)

add_executable(minesweeper)
target_sources(minesweeper PRIVATE ${MINESWEEPER_EXE_SOURCES})
target_link_libraries(minesweeper PRIVATE minesweeper_lib)

depman_make_available(raylib)
target_link_libraries(minesweeper PRIVATE raylib)

if(MINESWEEPER_BUILD_TESTS)
    depman_make_available(confer)
    add_subdirectory(tests)
endif()

if(MINESWEEPER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/minesweeper.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeperConfigVersion.cmake"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeperConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/minesweeperConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/minesweeper"
)
install(
    TARGETS minesweeper
    EXPORT minesweeperTargets
    FILE_SET HEADERS
)
install(
    EXPORT minesweeperTargets
    NAMESPACE minesweeper::
    DESTINATION "${CMAKE_INSTALL_DATADIR}/minesweeper"
)
