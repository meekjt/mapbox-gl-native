#include <mbgl/shader/texture_rect_vertex.hpp>
#include <mbgl/shader/shader.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

void TextureRectVertex::bind(const int8_t* offset) {
    constexpr GLint stride = sizeof(TextureRectVertex);
    static_assert(stride == 16, "expected TextureRectVertex size");

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_pos, 2, GL_SHORT, false, stride, offset + offsetof(TextureRectVertex, a_pos)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_offset));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_offset, 2, GL_SHORT, false, stride, offset + offsetof(TextureRectVertex, a_offset)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_texture_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_texture_pos, 2, GL_UNSIGNED_SHORT, false, stride, offset + offsetof(TextureRectVertex, a_texture_pos)));

    MBGL_CHECK_ERROR(glEnableVertexAttribArray(Shader::a_data));
    MBGL_CHECK_ERROR(glVertexAttribPointer(Shader::a_data, 4, GL_UNSIGNED_BYTE, false, stride, offset + offsetof(TextureRectVertex, a_data)));
}

} // namespace mbgl
