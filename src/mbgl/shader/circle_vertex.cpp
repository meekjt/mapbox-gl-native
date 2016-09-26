#include <mbgl/shader/circle_vertex.hpp>
#include <mbgl/shader/shader.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

void CircleVertex::bind(const int8_t* offset) {
    constexpr GLint stride = sizeof(CircleVertex);
    static_assert(stride == 4, "expected CircleVertex size");

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_pos, 2, GL_SHORT, false, stride, offset + offsetof(CircleVertex, a_pos)));
}

} // namespace mbgl
