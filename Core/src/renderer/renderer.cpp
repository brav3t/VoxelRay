
#pragma once

#include "renderer.h"

#include "gl_utils.h"
#include "../window_handling/window_factory.h"
#include "ray_box_shader.h"
#include "camera.h"
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

    Camera camera;

    double lastTime = glfwGetTime();
    double lastX = 0.0, lastY = 0.0;
    bool firstMouse = true;

    GLuint dummyVAO = 0;
    GLCall(glGenVertexArrays(1, &dummyVAO));
    GLCall(glBindVertexArray(dummyVAO));

    RayBoxShader rayBoxShader("res/shaders/raybox.shader");

    GLCall(glViewport(0, 0, static_cast<int>(mRenderWindow->resolution.x), static_cast<int>(mRenderWindow->resolution.y)));

    GLCall(glClearColor(0.05f, 0.05f, 0.07f, 1.0f));

    while(!mRenderWindow->shouldClose())
    {
        auto* wnd = (GLFWwindow*)mRenderWindow->pWnd;

        // Mouse look with right click
        if (glfwGetMouseButton(wnd, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double x, y;
            glfwGetCursorPos(wnd, &x, &y);
            if (firstMouse){ lastX = x; lastY = y; firstMouse = false; }

            float dx = float(x - lastX);
            float dy = float(lastY - y);
            lastX = x; lastY = y;

            const float sens = 0.08f;
            camera.addPitchYaw(dx * sens, dy * sens);
        }
        else
        {
            glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }

        // WASD move
        float speed = 3.0f;
        if (glfwGetKey(wnd, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) speed = 8.0f;

        // Get time delta
        double now = glfwGetTime();
        float dt = float(now - lastTime);
        lastTime = now;

        if (glfwGetKey(wnd, GLFW_KEY_W) == GLFW_PRESS) camera.pos += camera.forward() * speed * dt;
        if (glfwGetKey(wnd, GLFW_KEY_S) == GLFW_PRESS) camera.pos -= camera.forward() * speed * dt;
        if (glfwGetKey(wnd, GLFW_KEY_A) == GLFW_PRESS) camera.pos -= camera.right() * speed * dt;
        if (glfwGetKey(wnd, GLFW_KEY_D) == GLFW_PRESS) camera.pos += camera.right() * speed * dt;

        // update invViewProj
        float aspect = mRenderWindow->resolution.x / mRenderWindow->resolution.y;
        glm::mat4 view = camera.view();
        glm::mat4 proj = camera.proj(aspect);
        glm::mat4 invViewProj = glm::inverse(proj * view);

        clear();

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

        glm::vec3 voxelRadius{ 0.5f, 0.5f, 0.5f };

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
