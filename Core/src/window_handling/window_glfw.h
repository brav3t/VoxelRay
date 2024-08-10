
#include "window_factory.h"

class WindowGLFW final : public IWindow {
public:
    WindowGLFW(int width, int height, const char* title);
    WindowGLFW(const WindowGLFW&) = delete;
    virtual ~WindowGLFW();
    WindowGLFW operator=(const WindowGLFW&) = delete;

    virtual bool shouldClose() const override;
    virtual void processInput() const override;
    virtual void onUpdate() const override;
    virtual void* getProcAddress() const override;
};

class WindowFactoryGLFW final : public IWindowFactory {
public:
    WindowFactoryGLFW();
    ~WindowFactoryGLFW();

    virtual std::unique_ptr<IWindow> CreateWindow(int width, int height, const char* title) override;
};
