#include <mbgl/renderer/painter.hpp>
#include <mbgl/renderer/paint_parameters.hpp>

#include <mbgl/style/layers/background_layer.hpp>
#include <mbgl/style/layers/background_layer_impl.hpp>
#include <mbgl/shader/shaders.hpp>
#include <mbgl/sprite/sprite_atlas.hpp>
#include <mbgl/util/mat4.hpp>
#include <mbgl/util/tile_cover.hpp>

namespace mbgl {

using namespace style;

void Painter::renderBackground(PaintParameters&, const BackgroundLayer& layer) {
    // Note that for bottommost layers without a pattern, the background color is drawn with
    // glClear rather than this method.
    const BackgroundPaintProperties& properties = layer.impl->paint;

    if (!properties.backgroundPattern.value.to.empty()) {
        optional<SpriteAtlasPosition> imagePosA = spriteAtlas->getPosition(
            properties.backgroundPattern.value.from, SpritePatternMode::Repeating);
        optional<SpriteAtlasPosition> imagePosB = spriteAtlas->getPosition(
            properties.backgroundPattern.value.to, SpritePatternMode::Repeating);

        if (!imagePosA || !imagePosB)
            return;

        for (const auto& tileID : util::tileCover(state, state.getIntegerZoom())) {
            int32_t tileSizeAtNearestZoom = util::tileSize * state.zoomScale(state.getIntegerZoom() - tileID.canonical.z);

            spriteAtlas->bind(true, context, 0);

            context.draw(
                gl::Depth { gl::Depth::LessEqual, false, depthRangeForSublayer(0), },
                gl::Stencil::disabled(),
                colorForRenderPass(),
                shaders->pattern.bindUniforms(
                    matrixForTile(tileID),
                    *imagePosA, properties.backgroundPattern.value.fromScale,
                    *imagePosB, properties.backgroundPattern.value.toScale,
                    properties.backgroundOpacity,
                    properties.backgroundPattern.value.t,
                    1.0f / tileID.pixelsToTileUnits(1.0f, state.getIntegerZoom()),
                    Point<int32_t> {
                        int32_t(tileSizeAtNearestZoom * (tileID.canonical.x + tileID.wrap * state.zoomScale(tileID.canonical.z))),
                        int32_t(tileSizeAtNearestZoom * tileID.canonical.y)
                    }
                ),
                gl::Vertexes { tileTriangleVertexes },
                gl::Unindexed {},
                gl::TriangleStrip {});
        }
    } else {
        for (const auto& tileID : util::tileCover(state, state.getIntegerZoom())) {
            context.draw(
                gl::Depth { gl::Depth::LessEqual, false, depthRangeForSublayer(0), },
                gl::Stencil::disabled(),
                colorForRenderPass(),
                shaders->plain.bindUniforms(
                    matrixForTile(tileID),
                    properties.backgroundColor,
                    properties.backgroundOpacity
                ),
                gl::Vertexes { tileTriangleVertexes },
                gl::Unindexed {},
                gl::TriangleStrip {});
        }
    }
}

} // namespace mbgl
