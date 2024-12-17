add_library(PistonEngine STATIC PistonEngine.cpp
        utils/Logger.cpp)
target_include_directories(PistonEngine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(PistonEngine PRIVATE ${CMAKE_SOURCE_DIR}/src ${VULKAN_SDK_PATH}/include)

target_link_libraries(PistonEngine glfw Vulkan::Vulkan ImGui)