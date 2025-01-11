add_library(piston_graphics
        src/Model.cpp
        src/Pipeline.cpp
        src/Renderer.cpp
        src/Camera.cpp
        src/Device.cpp
        src/Instance.cpp
)

target_include_directories(piston_graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_graphics PUBLIC piston_core piston_vk_interface piston_window glm Vulkan::Vulkan)

add_library(Piston::Graphics ALIAS piston_graphics)