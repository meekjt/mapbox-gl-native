#include <mbgl/shader/plain_shader.hpp>
#include <mbgl/shader/fill.vertex.hpp>
#include <mbgl/shader/fill.fragment.hpp>
#include <mbgl/shader/plain_vertex.hpp>

namespace mbgl {

PlainShader::PlainShader(gl::Context& context, Defines defines)
    : Shader(shaders::fill::name,
             shaders::fill::vertex,
             shaders::fill::fragment,
             context, defines) {
}

void PlainShader::bind(const gl::VertexBuffer<PlainVertex>&,
                       const int8_t* offset) {
    PlainVertex::bind(offset);
}

} // namespace mbgl
