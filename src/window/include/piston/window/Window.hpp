//
// Created by cleme on 20/12/2024.
//

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>


namespace piston {

    class Window {
    public:
        Window(int a_width, int a_height, const std::string a_windowName);
        ~Window();
        void o_createWindowSurface(VkInstance a_instance, VkSurfaceKHR* a_surface) const;

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool o_shouldClose() const { return glfwWindowShouldClose(m_window); }

        VkExtent2D o_getExtent() const { return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; }

        bool o_wasWindowResized() const { return m_framebufferResized; }
        void o_resetWindowResizedFlag() { m_framebufferResized = false; }

        GLFWwindow* o_getGLFWwindow() const { return m_window; }

    private:
        GLFWwindow* m_window;

        int m_width;
        int m_height;
        bool m_framebufferResized = false;

        std::string m_windowName;

        void o_initWindow();
        static void os_framebufferResizeCallback(GLFWwindow* a_window, int a_width, int a_height);
    };
}

#endif //WINDOW_HPP
