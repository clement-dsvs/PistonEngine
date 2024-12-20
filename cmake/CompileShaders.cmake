# Find glslc shader compiler
find_program(GLSLC glslc HINTS
        $ENV{VULKAN_SDK}/bin
        ${VULKAN_SDK_PATH}/bin
)

if(NOT GLSLC)
    message(FATAL_ERROR "Failed to find glslc shader compiler")
endif()

# Function to compile shaders
function(compile_shader TARGET SHADER)
    # Get file name without extension
    get_filename_component(FILE_NAME ${SHADER} NAME)
    # Get relative path to shader file
    get_filename_component(SHADER_DIR ${SHADER} DIRECTORY)

    # Set output directory and file
    set(SPIRV_DIR "${CMAKE_SOURCE_DIR}/GlfwVulkan/assets/shaders")
    set(SPIRV "${SPIRV_DIR}/${FILE_NAME}.spv")

    # Create output directory
    file(MAKE_DIRECTORY ${SPIRV_DIR})

    # Add custom command to compile the shader
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSLC} -o ${SPIRV} ${SHADER}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${SHADER}"
    )

    # Add the output file to a list of target sources
    target_sources(${TARGET} PRIVATE ${SPIRV})
endfunction()