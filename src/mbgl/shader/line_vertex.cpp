#include <mbgl/shader/line_vertex.hpp>
#include <mbgl/shader/shader.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

void LineVertex::bind(const int8_t* offset) {
    constexpr GLint stride = sizeof(LineVertex);
    static_assert(stride == 8, "expected LineVertex size");

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_pos, 2, GL_SHORT, false, stride, offset + offsetof(LineVertex, a_pos)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_data));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_data, 4, GL_UNSIGNED_BYTE, false, stride, offset + offsetof(LineVertex, a_data)));
}

} // namespace mbgl
