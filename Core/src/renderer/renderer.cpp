
#include "renderer.h"

#include "gl_utils.h"

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


        mRenderWindow->onUpdate();
    }
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const Shader& shader, const VertexArray& vao, const IndexBuffer& ibo) const
{
    shader.bind();
    vao.bind();
    ibo.bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const Shader& shader, const VertexArray& vao) const
{
    shader.bind();
    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, vao.getCount());
}
