
#include "../window_handling/window_factory.h"

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"
#include "ray_box_shader.h"

#include <memory>
#include <glm/fwd.hpp>

class Renderer
{
public:
    void addWindow(std::unique_ptr<IWindow> window);
    void render();
    void clear() const;

    void draw(const RayBoxShader& shader, const glm::vec3& camPos, const glm::mat4& invViewProj, const glm::vec2& scrRes) const;
    void draw(const Shader&, const VertexArray&, const IndexBuffer&) const;
    void draw(const Shader&, const VertexArray&) const;

private:
    std::unique_ptr<IWindow> mRenderWindow;
};
