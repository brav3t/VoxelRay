
#include "renderer.h"

#include "gl_utils.h"
#include "vertex_buffer.h"

Renderer::Renderer(std::unique_ptr<IWindow> window)
    : mRenderWindow(std::move(window)) {
}

void Renderer::initRender() {
    if (!mRenderWindow->isWindowCreated())
    {
        std::cout << "Render window is not created" << std::endl;
        return;
    }

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)mRenderWindow->getProcAddress()))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    float data_vertices[] = {
     -0.5f, -0.5f, // itt csak pozíciók a vertexek
      0.0f,  0.5f,
      0.5f, -0.5f
    };
    unsigned int data_size_in_bytes = 6 * sizeof(float);
    VertexBuffer vbo(data_vertices, data_size_in_bytes);

    while(!mRenderWindow->shouldClose()) {
        mRenderWindow->processInput();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        mRenderWindow->onUpdate();
    }
}
