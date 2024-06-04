cmake_minimum_required(VERSION 3.28)
project(Workspace)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RELEASE_POSTFIX r)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(PACKAGE_LIBRARIES glm::glm glfw glad::glad assimp::assimp spdlog::spdlog)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_17)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wno-unused-parameter>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif (UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif ()


set(CMAKE_CXX_STANDARD 17)

set(ENGINE_NAME "Society")
set(GAME_NAME "Game")

add_subdirectory(Engine)
add_subdirectory(Game)
