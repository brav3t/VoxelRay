
#include "ray_box_shader.h"

#include "gl_utils.h"

#include <glm/glm.hpp>
#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/fwd.hpp>

static const char* kFullscreenVS = R"(
#version 450 core

out vec2 vUV;

void main()
{
    const vec2 pos[3] = vec2[3](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);

    vUV = 0.5 * (gl_Position.xy + vec2(1.0));
})";

RayBoxShader::RayBoxShader(const std::string& shaderPath)
    : mProgramID(0)
    , mShaderPath(shaderPath)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, kFullscreenVS);

    std::string fragSource = loadTextFile(shaderPath);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragSource);

    mProgramID = linkProgram(vertexShader, fragmentShader);

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
}

RayBoxShader::~RayBoxShader()
{
    if (mProgramID == 0)
        return;

    GLCall(glDeleteProgram(mProgramID));
}

void RayBoxShader::bind() const
{
    GLCall(glUseProgram(mProgramID));
}

void RayBoxShader::unbind() const
{
    GLCall(glUseProgram(0));
}

void RayBoxShader::setCameraPos(const glm::vec3& camPos) const
{
    GLint loc = getUniformLocation("uCameraPos");
    glUniform3f(loc, camPos.x, camPos.y, camPos.z);
}

void RayBoxShader::setInvViewProjMatrix(const glm::mat4& invViewProj) const
{
    GLint loc = getUniformLocation("uInvViewProj");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &invViewProj[0][0]);
}

void RayBoxShader::setResolution(const glm::vec2& scrRes) const
{
    GLint loc = getUniformLocation("uResolution");
    glUniform2f(loc, scrRes.x, scrRes.y);
}

void RayBoxShader::setVoxelRadius(const glm::vec3& voxelRadius) const
{
    GLint loc = getUniformLocation("uVoxelRadius");
    glUniform3f(loc, voxelRadius.x, voxelRadius.y, voxelRadius.z);
}

void RayBoxShader::setVoxelCount(int count) const
{
    GLint loc = getUniformLocation("uVoxelCount");
    glUniform1i(loc, count);
}

std::string RayBoxShader::loadTextFile(const std::string& path)
{
    std::ifstream fileStream(path);
    if (!fileStream.is_open())
    {
        std::cerr << "Failed to open file: " << path << "\n";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();

    return buffer.str();
}

GLuint RayBoxShader::compileShader(GLenum shaderType, const std::string& source)
{
    GLuint shaderID = glCreateShader(shaderType);
    const char* src = source.c_str();
    GLCall(glShaderSource(shaderID, 1, &src, nullptr));
    GLCall(glCompileShader(shaderID));

    GLint result = GL_FALSE;
    GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));
    if (result != GL_TRUE)
    {
        GLint len = 0;
        GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len));
        std::string log(len, '\0');
        GLCall(glGetShaderInfoLog(shaderID, len, &len, log.data()));

        std::cerr << "RayBoxShader: compile failed ("
            << (shaderType == GL_VERTEX_SHADER ? "VS" : "FS")
            << ")\n" << log << "\n";
        GLCall(glDeleteShader(shaderID));
        return 0;
    }

    return shaderID;
}

GLuint RayBoxShader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint programID = glCreateProgram();
    GLCall(glAttachShader(programID, vertexShader)); 
    GLCall(glAttachShader(programID, fragmentShader));
    GLCall(glLinkProgram(programID));

    GLint result = GL_FALSE;
    GLCall(glGetProgramiv(programID, GL_LINK_STATUS, &result));
    if (result != GL_TRUE)
    {
        GLint len = 0;
        GLCall(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len));
        std::string log(len, '\0');
        GLCall(glGetProgramInfoLog(programID, len, &len, log.data()));

        std::cerr << "RayBoxShader: link failed\n" << log << "\n";
        GLCall(glDeleteProgram(programID));
        return 0;
    }

#ifdef _DEBUG
    GLCall(glValidateProgram(programID));
#endif

    return programID;
}

GLint RayBoxShader::getUniformLocation(const std::string& name) const
{
    auto it = mUniformLocationCache.find(name);
    if (it != mUniformLocationCache.end()) return it->second;

    GLint loc = glGetUniformLocation(mProgramID, name.c_str());
    mUniformLocationCache[name] = loc;
    return loc;
}
