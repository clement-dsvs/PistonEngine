add_library(piston_graphics
        src/Graphics.cpp
)

target_include_directories(piston_graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_graphics PUBLIC piston_core)

add_library(Piston::Graphics ALIAS piston_graphics)