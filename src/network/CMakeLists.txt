add_library(piston_network
        src/Network.cpp
)

target_include_directories(piston_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(piston_network PUBLIC piston_core)

add_library(Piston::Network ALIAS piston_network)