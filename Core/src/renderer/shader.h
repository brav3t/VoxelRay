#pragma once

#include <string>
#include <unordered_map>

#include <glm/fwd.hpp>
#include "glad/glad.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	GLint getUniformLocation(const std::string& name) const;

private:
	std::string mFilePath;
	unsigned int mShaderID;
	mutable std::unordered_map<std::string, GLint> mUniformLocationCache;
};
