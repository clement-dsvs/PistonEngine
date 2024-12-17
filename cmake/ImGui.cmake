function(create_imgui_library)
    set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/extern/imgui")
    set(GLFW_DIR "${CMAKE_SOURCE_DIR}/extern/glfw")

    include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
    include_directories(${GLFW_DIR}/include)
    include_directories(${VULKAN_SDK_PATH}/include)

    add_library(ImGui STATIC
            ${IMGUI_DIR}/imgui.cpp
            ${IMGUI_DIR}/imgui_demo.cpp
            ${IMGUI_DIR}/imgui_draw.cpp
            ${IMGUI_DIR}/imgui_tables.cpp
            ${IMGUI_DIR}/imgui_widgets.cpp
            ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
            ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    )
    target_link_libraries(ImGui glfw Vulkan::Vulkan)
endfunction()

create_imgui_library()