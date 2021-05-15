#pragma once
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#define GLEW_ALREADY_INCLUDED


#define ASSERT(x) if(!(x)) //__debugbreak();
#define GL_Call(x) GL_ClearError();\
    x;\
    ASSERT(GL_LogCall(#x, __FILE__, __LINE__))

static void GL_ClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GL_LogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[GL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}