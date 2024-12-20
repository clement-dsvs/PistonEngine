cmake_minimum_required(VERSION 3.29)
project(PistonEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# External Libraries
find_package(Vulkan REQUIRED)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
include(ImGui REQUIRED)

add_subdirectory(src)
add_subdirectory(apps)