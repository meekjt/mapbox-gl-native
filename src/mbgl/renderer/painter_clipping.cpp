#include <mbgl/renderer/painter.hpp>
#include <mbgl/renderer/paint_parameters.hpp>
#include <mbgl/style/source.hpp>
#include <mbgl/shader/shaders.hpp>
#include <mbgl/util/clip_id.hpp>
#include <mbgl/util/string.hpp>
#include <mbgl/gl/debugging.hpp>

namespace mbgl {

void Painter::renderClippingMask(const UnwrappedTileID& tileID, const ClipID& clip) {
    context.draw(
        gl::Depth::disabled(),
        gl::Stencil {
            gl::Stencil::Always(),
            static_cast<int32_t>(clip.reference.to_ulong()),
            0b11111111,
            gl::Stencil::Keep,
            gl::Stencil::Keep,
            gl::Stencil::Replace
        },
        gl::Color::disabled(),
        shaders->plain.bindUniforms(
           matrixForTile(tileID),
           Color {},
           0
        ),
        gl::Vertexes { tileTriangleVertexes },
        gl::Unindexed {},
        gl::Triangles {});
}

} // namespace mbgl
