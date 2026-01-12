#pragma once

#include "glm/fwd.hpp"
#include "glad/glad.h"

#include <string>
#include <unordered_map>

class RayBoxShader
{
public:
    RayBoxShader(const std::string& shaderPath);
    ~RayBoxShader();

    void bind() const;
    void unbind() const;

    void setCameraPos(const glm::vec3& camPos) const;
    void setInvViewProjMatrix(const glm::mat4& invViewProj) const;
    void setResolution(const glm::vec2& scrRes) const;

private:
    std::string loadTextFile(const std::string& path);

    GLuint compileShader(GLenum shaderType, const std::string& source);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

    GLint getUniformLocation(const std::string& name) const;

private:
    GLuint mProgramID;
    std::string mShaderPath;
    mutable std::unordered_map<std::string, GLint> mUniformLocationCache;
};
