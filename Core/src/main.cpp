
#include "window_handling/window_glfw.h"
#include "renderer/renderer.h"

#include <utility>

int main()
{
    WindowFactoryGLFW factory;
    auto pWindow = factory.CreateWindow(1920, 1080, "voxel");

    Renderer renderer;

    renderer.addWindow(std::move(pWindow));

    renderer.render();

    return 0;
}
