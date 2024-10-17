
#include "../window_handling/window_factory.h"

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

class Renderer {
public:
    Renderer(std::unique_ptr<IWindow> window);

    void initRender();
    void clear() const;
    void draw(const Shader&, const VertexArray&, const IndexBuffer&) const;
    void draw(const Shader&, const VertexArray&) const;

private:
    std::unique_ptr<IWindow> mRenderWindow;
};
