
#include "window_glfw.h"

#include "window_factory.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <memory>

WindowGLFW::WindowGLFW(int width, int height, const char* title)
    : IWindow(width, height, title)
{
    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (pWnd = glfwCreateWindow(width, height, title, NULL, NULL)) {
        glfwMakeContextCurrent((GLFWwindow*)pWnd);

        auto framebufferSizeCallback = [](GLFWwindow* window, int width, int height) {
            // make sure the viewport matches the new window dimensions; note that width and 
            // height will be significantly larger than specified on retina displays.
            glViewport(0, 0, width, height);
        };

        glfwSetFramebufferSizeCallback((GLFWwindow*)pWnd, framebufferSizeCallback);
    }
}

WindowGLFW::~WindowGLFW()
{
    glfwDestroyWindow((GLFWwindow*)pWnd);
}

bool WindowGLFW::shouldClose() const
{
    return glfwWindowShouldClose((GLFWwindow*)pWnd);
}

void WindowGLFW::processInput() const
{
    if (glfwGetKey((GLFWwindow*)pWnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose((GLFWwindow*)pWnd, true);
    }
}

void WindowGLFW::onUpdate() const
{
    glfwSwapBuffers((GLFWwindow*)pWnd);

    // poll IO events (keys pressed/released, mouse moved etc.)
    glfwPollEvents(); // It is not window based!
}

void* WindowGLFW::getProcAddress() const
{
    return glfwGetProcAddress;
}

WindowFactoryGLFW::WindowFactoryGLFW()
{
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Could not initialize glfw!");
    }
}

WindowFactoryGLFW::~WindowFactoryGLFW()
{
    glfwTerminate();
}

std::unique_ptr<IWindow> WindowFactoryGLFW::CreateWindow(int width, int height, const char* title)
{
    return std::make_unique<WindowGLFW>(width, height, title);
}
