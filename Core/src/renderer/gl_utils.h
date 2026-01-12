
#pragma once

#include <glad/glad.h>

#include <iostream>

namespace gl_utils {
    inline void GLClearError() {
        while (glGetError() != GL_NO_ERROR);
    }

    inline bool GLLogCall(const char* function, const char* file, int line) {
        while (GLenum error = glGetError())
        {
            std::cout << "[OpenGL Error] (" << error << ")" << function <<
                " " << file << ":" << line << std::endl;
            return false;
        }
        return true;
    }
}

#define ASSERT(x) if (!(x)) __debugbreak(); // __debugbreak only for MSVSC
#define GLCall(x) gl_utils::GLClearError();\
    x;\
    ASSERT(gl_utils::GLLogCall(#x, __FILE__, __LINE__)) // __x__ all compilers shoud understand it
