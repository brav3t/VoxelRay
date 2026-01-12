#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#include <glm/glm.hpp>

struct alignas(16) VoxelGPU
{
    glm::vec4 center;   // xyz
    glm::vec4 radius;   // xyz
    glm::mat4 rotation; // world->box
    glm::vec4 color;    // rgba
};
