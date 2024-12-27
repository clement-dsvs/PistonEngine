//
// Created by cleme on 17/12/2024.
//

#include "piston/graphics/Camera.hpp"
#include "piston/graphics/Device.hpp"
#include "piston/graphics/Renderer.hpp"
#include "piston/window/Window.hpp"

int main() {
    piston::Window l_window{800, 800, "Vulkan Sandbox"};
    piston::Device l_device{l_window};
    piston::Renderer l_renderer{l_window, l_device};
    piston::Camera l_camera{};

    while (!l_window.o_shouldClose()) {
        glfwPollEvents();

        float aspect = l_renderer.getAspectRatio();
        l_camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100);

        if (auto l_commandBuffer = l_renderer.beginFrame())
        {
            l_renderer.beginSwapChainRenderPass(l_commandBuffer);

            // Draw Stuff

            l_renderer.endSwapChainRenderPass(l_commandBuffer);
            l_renderer.endFrame();
        }
    }


    vkDeviceWaitIdle(l_device.device());
}
