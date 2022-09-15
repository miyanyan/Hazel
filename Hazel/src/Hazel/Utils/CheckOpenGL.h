#pragma once

#include <cstdio>
#include <string>

#include <glad/glad.h>

#include "Hazel/Core/Log.h"

namespace Hazel {

// copied from zeno: https://github.com/zenustech/zeno/tree/master/zeno/include/zeno/utils
static const char* get_opengl_error_string(GLenum err) {
    switch (err) {
#define PER_GL_ERR(x) \
    case x: return #x;
        PER_GL_ERR(GL_NO_ERROR)
        PER_GL_ERR(GL_INVALID_ENUM)
        PER_GL_ERR(GL_INVALID_VALUE)
        PER_GL_ERR(GL_INVALID_OPERATION)
        PER_GL_ERR(GL_INVALID_FRAMEBUFFER_OPERATION)
        PER_GL_ERR(GL_OUT_OF_MEMORY)
        PER_GL_ERR(GL_STACK_UNDERFLOW)
        PER_GL_ERR(GL_STACK_OVERFLOW)
#undef PER_GL_ERR
    }
    static char tmp[233];
    sprintf(tmp, "%d\n", err);
    return tmp;
}

static void _check_opengl_error(const char* file, int line, const char* hint) {
    auto err = glGetError();
#if defined(__GUNC__) || defined(__clang__)
    if (__builtin_expect(!!(err != GL_NO_ERROR), 0)) {
#else
    if (err != GL_NO_ERROR) {
#endif
        auto msg = get_opengl_error_string(err);
        HZ_CORE_ERROR(std::string(file) + ':' + std::to_string(line) + ": " + hint + ": " + msg);
    }
}


#ifdef ENABLE_OPENGL_CHECK
#define CHECK_GL(x)                                                     \
    do {                                                                \
        (x);                                                            \
        ::Hazel::_check_opengl_error(__FILE__, __LINE__, #x); \
    } while (0)
#else
#define CHECK_GL(x) (x)
#endif // ENABLE_OPENGL_CHECK

}
