
#include "shader.h"

#include "gl_utils.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: mFilePath(filepath)
	, mShaderID(0) {
    ShaderProgramSource source = parseShader(filepath);
    mShaderID = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(mShaderID));
}

void Shader::bind() const {
    GLCall(glUseProgram(mShaderID));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value) {
    GLint location = getUniformLocation(name);
    GLCall(glUniform1i(location, value));
}

void Shader::setUniform1f(const std::string& name, float value) {
    GLint location = getUniformLocation(name);
    GLCall(glUniform1f(location, value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLint location = getUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLint location = getUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

GLint Shader::getUniformLocation(const std::string& name) const {
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
        return mUniformLocationCache[name];

    GLint location = glGetUniformLocation(mShaderID, name.c_str());
    mUniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // Delete the intermediate 
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
