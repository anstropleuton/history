cmake_minimum_required(VERSION 3.31.6)

project(BPP
        VERSION 0.1.0
        LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(FetchContent)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Dependencies)

set(BPP_SOURCES
        src/main.cpp
        src/utils.cpp
        src/ui/display.cpp
        src/ui/draw.cpp
        src/ui/icon.cpp
        src/ui/input.cpp
        src/ui/layout.cpp
        src/ui/manager.cpp
        src/ui/node.cpp
        src/ui/overlay.cpp
        src/ui/style.cpp
)

set(BPP_PRIVATE_HEADERS
        src/ui/ui.hpp
        src/utils.hpp
        src/ui/display.hpp
        src/ui/draw.hpp
        src/ui/icon.hpp
        src/ui/input.hpp
        src/ui/layout.hpp
        src/ui/manager.hpp
        src/ui/node.hpp
        src/ui/overlay.hpp
        src/ui/style.hpp
)

set(BPP_PUBLIC_HEADERS
        include/bpp/bpp.hpp
)

set(BPPUI_FILES
        ui/main_menu.bppui
)

add_executable(BPP)

target_compile_features(BPP PRIVATE cxx_std_23)

if (MSVC)
    target_compile_options(BPP PRIVATE
            /W4
            /permissive-
    )
else ()
    target_compile_options(BPP PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif ()

target_sources(BPP
        PRIVATE
        ${BPP_SOURCES}
)

target_sources(BPP
        PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src
        FILES ${BPP_PRIVATE_HEADERS}
)

target_sources(BPP
        PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES ${BPP_PUBLIC_HEADERS}
)

target_include_directories(BPP
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Dependencies
find_package(Python3 REQUIRED COMPONENTS Interpreter)

bpp_resolve_dependency(raylib
        "https://github.com/raysan5/raylib.git"
        "master"
)

target_link_libraries(BPP PRIVATE
        raylib
)

get_target_property(RAYLIB_INCLUDES raylib INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(BPP
        SYSTEM PRIVATE
        ${RAYLIB_INCLUDES}
)

# PhysX integration
bpp_resolve_noncmake_dependency(PhysX
        "https://github.com/NVIDIA-Omniverse/PhysX.git"
        "origin/5.4"
        PHYSX_ROOT
)

include("${PHYSX_ROOT}/physx/source/compiler/cmake/modules/GetCompilerAndPlatform.cmake")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TARGET_BUILD_PLATFORM "linux")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TARGET_BUILD_PLATFORM "windows")
endif ()

GetPlatformBinName(PLATFORM_BIN_NAME "64")
message(STATUS "Platform binary name: ${PLATFORM_BIN_NAME}")

target_include_directories(BPP PRIVATE "${PHYSX_ROOT}/physx/include")
target_link_directories(BPP PRIVATE
        "${PHYSX_ROOT}/physx/bin/${PLATFORM_BIN_NAME}/$<IF:$<CONFIG:Debug>,debug,release>"
)

target_link_libraries(BPP PRIVATE
        PhysXExtensions_static_64
        PhysX_static_64
        PhysXPvdSDK_static_64
        PhysXCommon_static_64
        PhysXFoundation_static_64
)

# Runtime resources
set(BPP_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(BPP_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

# BPP UI
set(BPPUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BPPUI_GENERATED_DIR "${BPP_GENERATED_DIR}")
set(BPPUI_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/tools/bppui_compiler.py")

set(BPPUI_GENERATED_SOURCES)
set(BPPUI_GENERATED_HEADERS)

foreach (BPPUI_FILE IN LISTS BPPUI_FILES)
    set(ABS_FILE "${BPPUI_SOURCE_DIR}/${BPPUI_FILE}")

    get_filename_component(STEM "${BPPUI_FILE}" NAME_WE)
    set(GEN_HPP "${BPPUI_GENERATED_DIR}/ui/${STEM}.hpp")
    set(GEN_CPP "${BPPUI_GENERATED_DIR}/ui/${STEM}.cpp")

    add_custom_command(
            OUTPUT "${GEN_HPP}" "${GEN_CPP}"

            COMMAND ${Python3_EXECUTABLE}
            "${BPPUI_COMPILER}"
            "${ABS_FILE}"
            -o "${BPPUI_GENERATED_DIR}"
            -r "${BPPUI_SOURCE_DIR}"

            DEPENDS
            "${ABS_FILE}"
            "${BPPUI_COMPILER}"

            VERBATIM
    )

    list(APPEND BPPUI_GENERATED_SOURCES "${GEN_CPP}")
endforeach()

add_custom_target(bppui_codegen
        DEPENDS
        ${BPPUI_GENERATED_HEADERS}
        ${BPPUI_GENERATED_SOURCES}
)

file(MAKE_DIRECTORY "${BPP_GENERATED_DIR}/bpp")

set(BPP_RESOURCE_HEADER
        "${BPP_GENERATED_DIR}/bpp/resources.hpp"
)

add_custom_command(
        OUTPUT "${BPP_RESOURCE_HEADER}"

        COMMAND ${CMAKE_COMMAND}
        -DRESOURCE_ROOT="${BPP_RESOURCE_DIR}"
        -DOUTPUT_FILE="${BPP_RESOURCE_HEADER}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateResources.cmake"

        DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateResources.cmake"
)

add_custom_target(bpp_resources_codegen
        DEPENDS "${BPP_RESOURCE_HEADER}"
)

add_dependencies(BPP bpp_resources_codegen bppui_codegen)

target_include_directories(BPP PRIVATE
        "${BPP_GENERATED_DIR}"
)

target_sources(BPP PRIVATE
        ${BPPUI_GENERATED_SOURCES}
)

add_custom_command(TARGET BPP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${BPP_RESOURCE_DIR}"
        "$<TARGET_FILE_DIR:BPP>/res"
)

install(DIRECTORY "${BPP_RESOURCE_DIR}/"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/BPP/res"
)

target_compile_definitions(BPP PRIVATE
        BPP_RESOURCE_PATH="$<IF:$<CONFIG:Debug>,${BPP_RESOURCE_DIR},${CMAKE_INSTALL_FULL_DATADIR}/BPP/res>"
)

install(DIRECTORY res/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/BPP/res
)

# Install executable + public headers
install(TARGETS BPP
        EXPORT BPPTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Package config
set(BPP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/BPP")

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BPPConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/BPPConfig.cmake"
        INSTALL_DESTINATION "${BPP_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/BPPConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMajorVersion
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/BPPConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/BPPConfigVersion.cmake"
        DESTINATION "${BPP_INSTALL_CMAKEDIR}"
)

install(EXPORT BPPTargets
        NAMESPACE BPP::
        DESTINATION "${BPP_INSTALL_CMAKEDIR}"
)

# CPack
set(CPACK_PACKAGE_NAME "BPP")
set(CPACK_PACKAGE_VENDOR "BPP")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)
