#include <mbgl/shader/collision_box_vertex.hpp>
#include <mbgl/shader/shader.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

void CollisionBoxVertex::bind(const int8_t* offset) {
    constexpr GLint stride = sizeof(CollisionBoxVertex);
    static_assert(stride == 10, "expected CollisionBoxVertex size");

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_pos, 2, GL_SHORT, false, stride, offset + offsetof(CollisionBoxVertex, a_pos)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_extrude));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_extrude, 2, GL_SHORT, false, stride, offset + offsetof(CollisionBoxVertex, a_extrude)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_data));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_data, 2, GL_UNSIGNED_BYTE, false, stride, offset + offsetof(CollisionBoxVertex, a_data)));
}

} // namespace mbgl
