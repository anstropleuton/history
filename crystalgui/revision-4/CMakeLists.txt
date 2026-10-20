cmake_minimum_required(VERSION 3.16)

project(CrystalGUI
	VERSION 0.0.1
	DESCRIPTION "CrystalGUI is a Crystal-clear Modern-style Retained-mode GUI framework for Raylib"
	LANGUAGES C
)

option(CGUI_BUILD_EXAMPLES "Build CrystalGUI Examples" OFF)
option(CGUI_BUILD_TESTS "Build CrystalGUI Tests" OFF)
option(CGUI_BUILD_EXPERIMENTS "Build CrystalGUI Experiments" OFF) # These are experiments intended to be used as one-off programs for quick prototyping and testing, and they may not even compile
option(BUILD_SHARED_LIBS "Build CrystalGUI as a shared library" OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(cgui_options INTERFACE)

target_compile_features(cgui_options INTERFACE c_std_99)

if (MSVC)
	# Hot reloading
	target_compile_options(cgui_options INTERFACE
		$<$<CONFIG:Debug>:/Wall>
		$<$<CONFIG:Debug>:/ZI>
	)

	target_link_options(cgui_options INTERFACE
		$<$<CONFIG:Debug>:/INCREMENTAL>
	)
else ()
	target_compile_options(cgui_options INTERFACE
		$<$<CONFIG:Debug>:-Wall>
	)
endif ()

target_link_libraries(cgui_options INTERFACE raylib)

if (NOT DEFINED CGUI_RAYLIB_DIR)
	set(CGUI_RAYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/raylib)
endif ()

find_package(raylib QUIET PATHS ${CGUI_RAYLIB_DIR})

if (NOT raylib_FOUND)
	if (EXISTS ${CGUI_RAYLIB_DIR}/CMakeLists.txt)
		message(STATUS "raylib not found, using raylib from 'external'...")
		add_subdirectory(${CGUI_RAYLIB_DIR})
	else ()
		message(STATUS "raylib not found, downloading and building raylib...")

		include(FetchContent)

		FetchContent_Declare(
			raylib
			GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
			GIT_TAG "master"
			GIT_SHALLOW ON
		)

		FetchContent_MakeAvailable(raylib)
	endif ()
else ()
	message(STATUS "raylib found in system path, using system-provided raylib...")
endif ()

add_subdirectory(src)

if (CGUI_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif ()

if (CGUI_BUILD_TESTS)
	add_subdirectory(tests)
endif ()

if (CGUI_BUILD_EXPERIMENTS)
	add_subdirectory(experiments)
endif ()
