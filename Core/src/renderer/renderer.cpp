
#pragma once

#include "renderer.h"

#include "../window_handling/window_factory.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "ray_box_shader.h"

#include "gl_utils.h"
#include <glad/glad.h>
#include <glm/fwd.hpp>

#include <iostream>
#include <utility>
#include <memory>

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

    GLuint dummyVAO = 0;
    GLCall(glGenVertexArrays(1, &dummyVAO));
    GLCall(glBindVertexArray(dummyVAO));

    RayBoxShader voxels("res/shaders/raybox.shader");

    glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);

    glm::mat4 invViewProj(1.0f);

    while(!mRenderWindow->shouldClose())
    {
        mRenderWindow->processInput();

        draw(voxels, cameraPosition, invViewProj, mRenderWindow->resolution);

        mRenderWindow->onUpdate();
    }
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


void Renderer::draw(const RayBoxShader& shader, const glm::vec3& camPos, const glm::mat4& invViewProj, const glm::vec2& scrRes) const
{
    shader.bind();

    shader.setCameraPos(camPos);
    shader.setInvViewProjMatrix(invViewProj);
    shader.setResolution(scrRes);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
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
