#include <mbgl/shader/raster_vertex.hpp>
#include <mbgl/shader/shader.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

void RasterVertex::bind(const int8_t* offset) {
    constexpr GLint stride = sizeof(RasterVertex);
    static_assert(stride == 8, "expected RasterVertex size");

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_pos, 2, GL_SHORT, false, stride, offset + offsetof(RasterVertex, a_pos)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_texture_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_texture_pos, 2, GL_SHORT, false, stride, offset + offsetof(RasterVertex, a_texture_pos)));
}

} // namespace mbgl
