
#pragma once

#include <memory>

class IWindow {
public:
    IWindow(int width, int height, const char* title)
        : width(width), height(height), title(title), pWnd(nullptr) {
    }
    IWindow(const IWindow&) = delete;
    virtual ~IWindow() = default;
    IWindow operator=(const IWindow&) = delete;

    inline virtual bool isWindowCreated() const { return pWnd != nullptr; }
    virtual bool shouldClose() const = 0;
    virtual void processInput() const = 0;
    virtual void onUpdate() const = 0;
    virtual void* getProcAddress() const = 0;

public:
    int width;
    int height;
    const char* title;
    void* pWnd;
};

class IWindowFactory {
public:
    virtual std::unique_ptr<IWindow> CreateWindow(int width, int height, const char* title) = 0;
};
