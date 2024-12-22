add_library(piston_core
        src/Logger.cpp
)

target_include_directories(piston_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_core PUBLIC EnTT::EnTT)

add_library(Piston::Core ALIAS piston_core)