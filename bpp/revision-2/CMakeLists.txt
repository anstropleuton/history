cmake_minimum_required(VERSION 4.2)
project(BPP)

set(CMAKE_CXX_STANDARD 23)

add_executable(BPP
        src/main.cpp
        src/utils.hpp
        src/utils.cpp
        bppui/bppui.hpp
        bppui/display.hpp
        bppui/display.cpp
        bppui/draw.hpp
        bppui/draw.cpp
        bppui/icon.hpp
        bppui/icon.cpp
        bppui/input.hpp
        bppui/input.cpp
        bppui/layout.hpp
        bppui/layout.cpp
        bppui/manager.hpp
        bppui/manager.cpp
        bppui/node.hpp
        bppui/node.cpp
        bppui/overlay.hpp
        bppui/overlay.cpp
        bppui/style.hpp
        bppui/style.cpp
)

target_include_directories(BPP PRIVATE src bppui "${CMAKE_CURRENT_BINARY_DIR}/gen")

# Raylib
add_subdirectory("deps/raylib")
target_link_libraries(BPP PRIVATE raylib)

# NanoSVG
set(NANOSVG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/deps/nanosvg")
set(NANOSVG_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps/nanosvg-build")
set(NANOSVG_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps/nanosvg-install")
set(NANOSVG_CONFIG "${NANOSVG_INSTALL_DIR}/lib/cmake/NanoSVG/NanoSVGConfig.cmake")

if (NOT EXISTS "${NANOSVG_CONFIG}")
    execute_process(
            COMMAND "${CMAKE_COMMAND}" -S "${NANOSVG_SRC}" -B "${NANOSVG_BUILD_DIR}" "-DCMAKE_INSTALL_PREFIX=${NANOSVG_INSTALL_DIR}"
            RESULT_VARIABLE r1
    )
    if (NOT r1 EQUAL 0)
        message(FATAL_ERROR "NanoSVG configure failed")
    endif ()

    execute_process(
            COMMAND "${CMAKE_COMMAND}" --build "${NANOSVG_BUILD_DIR}" --target install
            RESULT_VARIABLE r2
    )
    if (NOT r2 EQUAL 0)
        message(FATAL_ERROR "NanoSVG install failed")
    endif ()
endif ()

list(PREPEND CMAKE_PREFIX_PATH "${NANOSVG_INSTALL_DIR}")
find_package(NanoSVG REQUIRED CONFIG)

target_link_libraries(BPP PRIVATE NanoSVG::nanosvg NanoSVG::nanosvgrast)

# PhysX
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PHYSX_BIN_NAME "linux.x86_64")
else ()
    set(PHYSX_BIN_NAME "windows.x86_64.vc143.mt")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PHYSX_BUILD_TYPE "debug")
else ()
    set(PHYSX_BUILD_TYPE "release")
endif ()

target_include_directories(BPP PRIVATE "deps/PhysX/physx/include")
target_link_directories(BPP PRIVATE "deps/PhysX/physx/bin/${PHYSX_BIN_NAME}/${PHYSX_BUILD_TYPE}")
target_link_libraries(BPP PRIVATE
        PhysXExtensions_static_64
        PhysX_static_64
        PhysXPvdSDK_static_64
        PhysXCommon_static_64
        PhysXFoundation_static_64
)

# Resources
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(RESOURCES_HEADER "${CMAKE_CURRENT_BINARY_DIR}/gen/resources.hpp")

file(GLOB_RECURSE RESOURCE_ENTRIES CONFIGURE_DEPENDS "${RESOURCES_DIR}/*")

add_custom_command(
        OUTPUT ${RESOURCES_HEADER}
        COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_resources.py" -i "${RESOURCES_DIR}" -o ${RESOURCES_HEADER}
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_resources.py" ${RESOURCE_ENTRIES}
)

target_sources(BPP PRIVATE ${RESOURCES_HEADER})

file(COPY "${RESOURCES_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
