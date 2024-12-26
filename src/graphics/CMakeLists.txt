add_library(piston_graphics
        src/Model.cpp
        src/Device.cpp
        include/piston/graphics/Pipeline.hpp
        src/Pipeline.cpp
        include/piston/graphics/Renderer.hpp
        src/Renderer.cpp
        include/piston/graphics/SwapChain.hpp
        src/SwapChain.cpp
        src/Camera.cpp
)

target_include_directories(piston_graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_graphics PUBLIC piston_core piston_window glm Vulkan::Vulkan)

add_library(Piston::Graphics ALIAS piston_graphics)