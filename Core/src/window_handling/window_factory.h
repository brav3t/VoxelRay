#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>

class IWindow {
public:
    IWindow(int width, int height, const char* title)
        : resolution{ static_cast<float>(width), static_cast<float>(height) }, title(title), pWnd(nullptr)
    {
    }

    IWindow(const IWindow&) = delete;
    virtual ~IWindow() = default;
    IWindow& operator=(const IWindow&) = delete;

    inline virtual bool isWindowCreated() const { return pWnd != nullptr; }
    virtual bool shouldClose() const = 0;
    virtual void processInput() const = 0;
    virtual void onUpdate() const = 0;
    virtual void* getProcAddress() const = 0;

public:
    glm::vec2 resolution;
    std::string title;
    void* pWnd;
};

class IWindowFactory {
public:
    virtual std::unique_ptr<IWindow> CreateWindow(int width, int height, const char* title) = 0;
};
