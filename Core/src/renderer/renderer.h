#pragma once

#include "../window_handling/window_factory.h"

#include "ray_box_shader.h"
#include "voxel.h"

#include <memory>
#include <glm/fwd.hpp>
#include "ssbo.h"
#include <vector>

class Renderer
{
public:
    void addWindow(std::unique_ptr<IWindow> window);

    void render();

    void clear() const;
    void draw(
        const RayBoxShader&,
        const int voxelCount,
        const glm::vec3& voxelRadius,
        const glm::vec3& camPos,
        const glm::mat4& invViewProj,
        const glm::vec2& scrRes) const;

    void initRays() const;

    void rotateVoxel(
        int voxelIdx,
        std::vector<VoxelGPU>& voxels,
        ShaderStorageBuffer& ssbo,
        float angleDeg,
        const glm::vec3& axis);

private:
    std::unique_ptr<IWindow> mRenderWindow;
};
