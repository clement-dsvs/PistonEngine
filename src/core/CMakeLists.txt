add_library(piston_core
        src/Logger.cpp
)

target_include_directories(piston_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(Piston::Core ALIAS piston_core)