add_library(piston_graphics
        src/Model.cpp
        src/Device.cpp
        src/Pipeline.cpp
        src/Renderer.cpp
        src/SwapChain.cpp
        src/Camera.cpp
)

target_include_directories(piston_graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_graphics PUBLIC piston_core piston_window glm Vulkan::Vulkan)

add_library(Piston::Graphics ALIAS piston_graphics)