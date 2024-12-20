//
// Created by cleme on 20/12/2024.
//

#include "piston/window/Window.hpp"

#include <stdexcept>

#include <GLFW/glfw3.h>

piston::Window::Window(int a_width, int a_height, const std::string a_windowName) : m_window(nullptr), m_width(a_width), m_height(a_height), m_windowName(a_windowName)
{
    o_initWindow();
}

piston::Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void piston::Window::o_createWindowSurface(VkInstance a_instance, VkSurfaceKHR*a_surface) const {
    if (glfwCreateWindowSurface(a_instance, m_window, nullptr, a_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }
}

void piston::Window::o_initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw std::runtime_error("Error while creating window");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, os_framebufferResizeCallback);
}

void piston::Window::os_framebufferResizeCallback(GLFWwindow *a_window, const int a_width, const int a_height) {
    const auto l_window = static_cast<Window*>(glfwGetWindowUserPointer(a_window));
    l_window->m_framebufferResized = true;
    l_window->m_width = a_width;
    l_window->m_height = a_height;
}
