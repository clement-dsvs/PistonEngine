add_library(piston_window
        src/Window.cpp
)

target_include_directories(piston_window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_window PUBLIC piston_core)

add_library(Piston::Window ALIAS piston_window)