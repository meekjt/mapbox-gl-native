#include <mbgl/shader/pattern_shader.hpp>
#include <mbgl/shader/pattern.vertex.hpp>
#include <mbgl/shader/pattern.fragment.hpp>
#include <mbgl/sprite/sprite_atlas.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

PatternShader::PatternShader(gl::Context& context, Defines defines)
    : Shader(shaders::pattern::name,
             shaders::pattern::vertex,
             shaders::pattern::fragment,
             context, defines) {
}


gl::Program PatternShader::bindUniforms(const mat4& matrix,
                                        const SpriteAtlasPosition& posA,
                                        float scaleA,
                                        const SpriteAtlasPosition& posB,
                                        float scaleB,
                                        float opacity,
                                        float mix,
                                        float tileUnitsToPixels,
                                        Point<int32_t> pixelCoord) {
    u_matrix = matrix;
    u_pattern_tl_a = posA.tl;
    u_pattern_tl_b = posB.tl;
    u_pattern_br_a = posA.br;
    u_pattern_br_b = posB.br;
    u_pattern_size_a = posA.size;
    u_pattern_size_b = posB.size;
    u_scale_a = scaleA;
    u_scale_b = scaleB;
    u_mix = mix;
    u_opacity = opacity;
    u_tile_units_to_pixels = tileUnitsToPixels;
    u_pixel_coord_upper = {{ float(pixelCoord.x >> 16), float(pixelCoord.y >> 16) }};
    u_pixel_coord_lower = {{ float(pixelCoord.x & 0xFFFF), float(pixelCoord.y & 0xFFFF) }};
    return gl::Program { getID(), {} };
}

void PatternShader::bind(GLbyte *offset) {
    MBGL_CHECK_ERROR(glEnableVertexAttribArray(a_pos));
    MBGL_CHECK_ERROR(glVertexAttribPointer(a_pos, 2, GL_SHORT, false, 0, offset));
}

} // namespace mbgl
