
#pragma once

#include "renderer.h"

#include "gl_utils.h"
#include "../window_handling/window_factory.h"
#include "ray_box_shader.h"
#include "camera.h"
#include "input_handler.h"
#include "ssbo.h"
#include "voxel.h"

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>
#include <memory>
#include <vector>

void Renderer::addWindow(std::unique_ptr<IWindow> window)
{
    mRenderWindow = std::move(window);
}

void Renderer::render()
{
    if (mRenderWindow == nullptr || !mRenderWindow->isWindowCreated())
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

    GLCall(glViewport(0, 0, static_cast<int>(mRenderWindow->resolution.x), static_cast<int>(mRenderWindow->resolution.y)));

    GLCall(glClearColor(0.05f, 0.05f, 0.07f, 1.0f)); // Set frameBuffer clear color

    Camera camera;

    float wndAspectRatio = mRenderWindow->resolution.x / mRenderWindow->resolution.y;
    InputHandlerGLFW inputHandler((GLFWwindow*)mRenderWindow->pWnd, wndAspectRatio, camera);

    // Voxel data
    glm::vec3 voxelRadius{ 0.5f, 0.5f, 0.5f };

    VoxelGPU vox1{};
    vox1.center = { 0,0,-5,0 };
    vox1.rotation = glm::mat4(1.0f);
    vox1.color = { 1,0,1,1 };

    VoxelGPU vox2{};
    vox2.center = { -2,0,-5,0 };
    vox2.rotation = glm::mat4(1.0f);
    vox2.color = { 0,1,0,1 };

    std::vector<VoxelGPU> voxels = { vox1, vox2 };

    ShaderStorageBuffer voxelsSSBO;
    voxelsSSBO.allocate(sizeof(VoxelGPU) * voxels.size(), voxels.data());
    voxelsSSBO.bind(0);

    RayBoxShader rayBoxShader("res/shaders/raybox.shader");

    initRays();

    // Get time delta
    float deltaTime = 0.0f;
    float lastTime = (float)glfwGetTime();

    // Render loop
    while(!mRenderWindow->shouldClose())
    {
        clear();

        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glm::mat4 invViewProj = inputHandler.calcInvViewProj(deltaTime);

        static float angle = 0.0f;
        angle += 15.0f * deltaTime;
        rotateVoxel(
            0,
            voxels,
            voxelsSSBO,
            angle,
            glm::vec3(0, 1, 0));

        draw(
            rayBoxShader,
            static_cast<int>(voxels.size()),
            voxelRadius,
            camera.pos,
            invViewProj,
            mRenderWindow->resolution);

        mRenderWindow->onUpdate();
    }
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(
    const RayBoxShader& shader,
    const int voxelCount,
    const glm::vec3& voxelRadius,
    const glm::vec3& camPos,
    const glm::mat4& invViewProj,
    const glm::vec2& scrRes) const
{
    shader.bind();

    shader.setCameraPos(camPos);
    shader.setInvViewProjMatrix(invViewProj);
    shader.setResolution(scrRes);
    shader.setVoxelRadius(voxelRadius);
    shader.setVoxelCount(voxelCount);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

// Initializes a dummy VAO required to issue draw calls and
// execute per-pixel ray generation in the fragment shader.
void Renderer::initRays() const
{
    GLuint dummyVAO = 0;
    GLCall(glGenVertexArrays(1, &dummyVAO));
    GLCall(glBindVertexArray(dummyVAO));
}

void Renderer::rotateVoxel(
    int voxelIdx,
    std::vector<VoxelGPU>& voxels,
    ShaderStorageBuffer& ssbo,
    float angleDeg,
    const glm::vec3& axis)
{
    if (voxelIdx < 0 || voxelIdx >= (int)voxels.size())
        return;

    float angleRad = glm::radians(angleDeg);

    // box -> world rotation
    glm::mat4 boxToWorld = glm::rotate(glm::mat4(1.0f), angleRad, glm::normalize(axis));

    // world -> box (if rot-only inverse = transpose)
    glm::mat4 worldToBox = glm::transpose(boxToWorld);

    // CPU voxel data update
    voxels[voxelIdx].rotation = worldToBox;

    // GPU voxel data update
    size_t offset = voxelIdx * sizeof(VoxelGPU);
    
    ssbo.update(
        static_cast<GLintptr>(offset),
        sizeof(VoxelGPU),
        &voxels[voxelIdx]);
}
