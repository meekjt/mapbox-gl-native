#include <mbgl/renderer/painter.hpp>
#include <mbgl/renderer/debug_bucket.hpp>
#include <mbgl/renderer/render_tile.hpp>
#include <mbgl/tile/tile.hpp>
#include <mbgl/shader/shaders.hpp>
#include <mbgl/util/string.hpp>
#include <mbgl/gl/debugging.hpp>
#include <mbgl/gl/gl.hpp>
#include <mbgl/util/color.hpp>

namespace mbgl {

void Painter::renderTileDebug(const RenderTile& renderTile) {
    if (frame.debugOptions == MapDebugOptions::NoDebug)
        return;

    MBGL_DEBUG_GROUP(std::string { "debug " } + util::toString(renderTile.id));

    auto draw = [&] (Color color, const auto& vertexes, auto primitive) {
        context.draw(gl::Depth::disabled(),
                     stencilForClipping(renderTile.clip),
                     gl::Color::unblended(),
                     shaders->plain.bindUniforms(
                        renderTile.matrix,
                        color,
                        1.0f
                     ),
                     gl::Vertexes { vertexes },
                     gl::Unindexed {},
                     primitive);
    };

    if (frame.debugOptions & (MapDebugOptions::Timestamps | MapDebugOptions::ParseStatus)) {
        Tile& tile = renderTile.tile;
        if (!tile.debugBucket || tile.debugBucket->renderable != tile.isRenderable() ||
            tile.debugBucket->complete != tile.isComplete() ||
            !(tile.debugBucket->modified == tile.modified) ||
            !(tile.debugBucket->expires == tile.expires) ||
            tile.debugBucket->debugMode != frame.debugOptions) {
            tile.debugBucket = std::make_unique<DebugBucket>(
                tile.id, tile.isRenderable(), tile.isComplete(), tile.modified,
                tile.expires, frame.debugOptions, context);
        }

        const auto& textVertexes = tile.debugBucket->textVertexes;

        draw(Color::white(), textVertexes, gl::Lines { 4.0f * frame.pixelRatio });
        draw(Color::black(), textVertexes, gl::Points { 2.0f });
        draw(Color::black(), textVertexes, gl::Lines { 2.0f * frame.pixelRatio });
    }

    if (frame.debugOptions & MapDebugOptions::TileBorders) {
        draw(Color::red(), tileLineStripVertexes, gl::LineStrip { 4.0f * frame.pixelRatio });
    }
}

} // namespace mbgl
