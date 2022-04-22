//
// Created by Jacopo Beragnoli on 02/04/22.
//

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

// headers
#include "../../include/engine/Window.h"
#include "../../include/engine/exceptions/window/UnableToStartWindowException.h"
#include "../../include/engine/exceptions/window/UnableToInitGLFWException.h"
#include "../../include/engine/exceptions/window/FailedToCreateWindowSurfaceException.h"

namespace Engine {
    Window::Window(std::string &name, int width, int height) : name(name), width(width), height(height) {
        std::cout << "Starting the glfwWindow\n";
        initWindow();
    }

    Window::~Window() {
        std::cout << "Closing the glfwWindow...\n";
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    void Window::initWindow() {
        if (!glfwInit())
            throw Engine::Exceptions::UnableToInitGLFWException();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        this->glfwWindow = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        /* Window resize */
        glfwSetWindowUserPointer(glfwWindow, this);
        glfwSetFramebufferSizeCallback(glfwWindow, frameBufferResizedCallback);

        /* Create a windowed mode glfwWindow and its Vulkan context */
        if (!glfwWindow) {
            glfwTerminate();
            throw Engine::Exceptions::UnableToStartWindowException();
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::cout << "\nVulkan_extensions_supported: " << extensionCount << " extensions supported\n";
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(this->glfwWindow);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, glfwWindow, nullptr, surface) != VK_SUCCESS)
            throw Engine::Exceptions::FailedToCreateWindowSurfaceException();
    }

    bool Window::wasWindowResized() const {
        return frameBufferResized;
    }

    void Window::resetWindowResizedFlag() {
        frameBufferResized = false;
    }

    void Window::frameBufferResizedCallback(GLFWwindow *glfWwindow, int width, int height) {
        auto window = reinterpret_cast<Engine::Window *>(glfwGetWindowUserPointer(glfWwindow));
        window->frameBufferResized = true;
        window->width = width;
        window->height = height;
    }

    GLFWwindow *Window::getGlfwWindow() const {
        return glfwWindow;
    }
}