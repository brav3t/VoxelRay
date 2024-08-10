
#include "window_handling/window_glfw.h"
#include "renderer/renderer.h"

int main() {
    WindowFactoryGLFW factory;
    auto pWindow = factory.CreateWindow(800, 600, "voxel");

    Renderer renderer(std::move(pWindow));
    renderer.initRender();

    return 0;
}
