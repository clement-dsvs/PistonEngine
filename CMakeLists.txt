cmake_minimum_required(VERSION 3.30)
project(PistonEngine)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(Vulkan REQUIRED)
add_subdirectory(extern/glfw)
include(ImGui REQUIRED)

add_subdirectory(src)
add_subdirectory(apps)