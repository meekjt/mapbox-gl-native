#include <mbgl/shader/plain_shader.hpp>
#include <mbgl/shader/fill.vertex.hpp>
#include <mbgl/shader/fill.fragment.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

PlainShader::PlainShader(gl::Context& context, Defines defines)
    : Shader(shaders::fill::name,
             shaders::fill::vertex,
             shaders::fill::fragment,
             context, defines) {
}

gl::Program PlainShader::bindUniforms(const mat4& matrix,
                                      const Color& color,
                                      float opacity) {
    u_matrix = matrix;
    u_color = color;
    u_opacity = opacity;
    return gl::Program { getID(), {} };
}

void PlainShader::bind(int8_t* offset) {
    MBGL_CHECK_ERROR(glEnableVertexAttribArray(a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(a_pos, 2, GL_SHORT, false, 0, offset));
}

} // namespace mbgl
