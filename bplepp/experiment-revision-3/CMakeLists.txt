cmake_minimum_required(VERSION 3.31.5)

# Project name
project(bplepp)

# Define the source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE raylib/include)
target_include_directories(${PROJECT_NAME} PRIVATE PhysX/include)

# Definitions
if(CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
else()
target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Library directories and library files
if (WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE raylib/lib/windows)
    target_link_directories(${PROJECT_NAME} PRIVATE PhysX/lib/windows)
elseif (UNIX)
    target_link_directories(${PROJECT_NAME} PRIVATE raylib/lib/linux)
    target_link_directories(${PROJECT_NAME} PRIVATE PhysX/lib/linux)
endif()

# Link the libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib PhysXExtensions_static_64 PhysXPvdSDK_static_64 PhysX_static_64 PhysXVehicle_static_64 PhysXVehicle2_static_64 PhysXCharacterKinematic_static_64 PhysXCooking_static_64 PhysXCommon_static_64 PhysXFoundation_static_64)
