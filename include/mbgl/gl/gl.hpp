#pragma once

#include <mbgl/gl/types.hpp>

#include <stdexcept>

#if __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #elif TARGET_IPHONE_SIMULATOR
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #elif TARGET_OS_MAC
        #include <OpenGL/OpenGL.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
    #else
        #error Unsupported Apple platform
    #endif
#elif __ANDROID__ || MBGL_USE_GLES2
    #define GL_GLEXT_PROTOTYPES
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

namespace mbgl {
namespace gl {

struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

void checkError(const char *cmd, const char *file, int line);

#ifndef NDEBUG
#define MBGL_CHECK_ERROR(cmd) ([&]() { struct __MBGL_C_E { ~__MBGL_C_E() { ::mbgl::gl::checkError(#cmd, __FILE__, __LINE__); } } __MBGL_C_E; return cmd; }())
#else
#define MBGL_CHECK_ERROR(cmd) (cmd)
#endif

template <typename T> struct AttributeType;

template <> struct AttributeType<int8_t>   : std::integral_constant<GLenum, GL_BYTE> {};
template <> struct AttributeType<uint8_t>  : std::integral_constant<GLenum, GL_UNSIGNED_BYTE> {};
template <> struct AttributeType<int16_t>  : std::integral_constant<GLenum, GL_SHORT> {};
template <> struct AttributeType<uint16_t> : std::integral_constant<GLenum, GL_UNSIGNED_SHORT> {};
template <> struct AttributeType<int32_t>  : std::integral_constant<GLenum, GL_INT> {};
template <> struct AttributeType<uint32_t> : std::integral_constant<GLenum, GL_UNSIGNED_INT> {};
template <> struct AttributeType<float>    : std::integral_constant<GLenum, GL_FLOAT> {};

template <class V, class E, std::size_t N>
void bindVertexAttribute(AttributeLocation location, const E (V::*)[N], const int8_t* offset, std::size_t memberOffset) {
    MBGL_CHECK_ERROR(glEnableVertexAttribArray(location));
    MBGL_CHECK_ERROR(glVertexAttribPointer(location, N, AttributeType<E>::value, false, sizeof(V), offset + memberOffset));
}

// This has to be a macro because it uses the offsetof macro, which is the only legal way to get a member offset.
#define MBGL_BIND_VERTEX_ATTRIBUTE(VertexType, member, offset) \
    ::mbgl::gl::bindVertexAttribute(Shader::member, &VertexType::member, offset, offsetof(VertexType, member))

} // namespace gl
} // namespace mbgl
