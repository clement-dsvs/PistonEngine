add_executable(Sandbox src/main.cpp)
target_link_libraries(Sandbox PUBLIC Piston::Core Piston::Graphics Piston::Network Piston::Window)