#pragma once

#include <mbgl/map/transform_state.hpp>

#include <mbgl/tile/tile_id.hpp>

#include <mbgl/renderer/frame_history.hpp>
#include <mbgl/renderer/render_item.hpp>
#include <mbgl/renderer/bucket.hpp>

#include <mbgl/geometry/vao.hpp>
#include <mbgl/geometry/static_vertex_buffer.hpp>

#include <mbgl/gl/context.hpp>
#include <mbgl/gl/plain_vertex.hpp>

#include <mbgl/style/style.hpp>

#include <mbgl/util/noncopyable.hpp>
#include <mbgl/util/chrono.hpp>
#include <mbgl/util/constants.hpp>

#include <array>
#include <vector>
#include <set>
#include <map>

namespace mbgl {

class RenderTile;
class SpriteAtlas;
class GlyphAtlas;
class LineAtlas;
struct FrameData;
class Tile;

class DebugBucket;
class FillBucket;
class LineBucket;
class CircleBucket;
class SymbolBucket;
class RasterBucket;

class Shaders;
class SDFShader;
class PaintParameters;

struct ClipID;

namespace style {
class Style;
class Source;
class FillLayer;
class LineLayer;
class CircleLayer;
class SymbolLayer;
class RasterLayer;
class BackgroundLayer;
} // namespace style

struct FrameData {
    std::array<uint16_t, 2> framebufferSize = {{ 0, 0 }};
    TimePoint timePoint;
    float pixelRatio;
    MapMode mapMode;
    GLContextMode contextMode;
    MapDebugOptions debugOptions;
};

class Painter : private util::noncopyable {
public:
    Painter(const TransformState&);
    ~Painter();

    void render(const style::Style&,
                const FrameData&,
                SpriteAtlas& annotationSpriteAtlas);

    void cleanup();

    void renderClippingMask(const UnwrappedTileID&, const ClipID&);
    void renderTileDebug(const RenderTile&);
    void renderFill(PaintParameters&, FillBucket&, const style::FillLayer&, const RenderTile&);
    void renderLine(PaintParameters&, LineBucket&, const style::LineLayer&, const RenderTile&);
    void renderCircle(PaintParameters&, CircleBucket&, const style::CircleLayer&, const RenderTile&);
    void renderSymbol(PaintParameters&, SymbolBucket&, const style::SymbolLayer&, const RenderTile&);
    void renderRaster(PaintParameters&, RasterBucket&, const style::RasterLayer&, const RenderTile&);
    void renderBackground(PaintParameters&, const style::BackgroundLayer&);

    float saturationFactor(float saturation);
    float contrastFactor(float contrast);
    std::array<float, 3> spinWeights(float spin_value);

    bool needsAnimation() const;

private:
    std::vector<RenderItem> determineRenderOrder(const style::Style&);

    template <class Iterator>
    void renderPass(PaintParameters&,
                    RenderPass,
                    Iterator it, Iterator end,
                    uint32_t i, int8_t increment);

    void renderSDF(SymbolBucket&,
                   const RenderTile&,
                   float scaleDivisor,
                   std::array<float, 2> texsize,
                   SDFShader& sdfShader,
                   void (SymbolBucket::*drawSDF)(SDFShader&, gl::Context&, PaintMode),

                   // Layout
                   style::AlignmentType rotationAlignment,
                   style::AlignmentType pitchAlignment,
                   float layoutSize,

                   // Paint
                   float opacity,
                   Color color,
                   Color haloColor,
                   float haloWidth,
                   float haloBlur,
                   std::array<float, 2> translate,
                   style::TranslateAnchorType translateAnchor,
                   float paintSize);

    mat4 matrixForTile(const UnwrappedTileID&);
    Range<float> depthRangeForSublayer(int n) const;
    gl::Stencil stencilForClipping(const ClipID&) const;
    gl::Color colorForRenderPass() const;

    // TODO: remove
    void setDepthSublayer(int n);
    void setClipping(const ClipID&);

#ifndef NDEBUG
    PaintMode paintMode() const {
        return frame.debugOptions & MapDebugOptions::Overdraw ? PaintMode::Overdraw
                                                              : PaintMode::Regular;
    }
#else
    PaintMode paintMode() const {
        return PaintMode::Regular;
    }
#endif

    mat4 projMatrix;

    std::array<float, 2> pixelsToGLUnits;

    const mat4 identityMatrix = []{
        mat4 identity;
        matrix::identity(identity);
        return identity;
    }();

    const TransformState& state;

    FrameData frame;

    int indent = 0;

    gl::Context context;

    RenderPass pass = RenderPass::Opaque;

    int numSublayers = 3;
    uint32_t currentLayer;
    float depthRangeSize;
    const float depthEpsilon = 1.0f / (1 << 16);

    SpriteAtlas* spriteAtlas = nullptr;
    GlyphAtlas* glyphAtlas = nullptr;
    LineAtlas* lineAtlas = nullptr;

    FrameHistory frameHistory;

    std::unique_ptr<Shaders> shaders;
#ifndef NDEBUG
    std::unique_ptr<Shaders> overdrawShaders;
#endif

    gl::VertexBuffer<gl::PlainVertex> tileTriangleVertexes;
    gl::VertexBuffer<gl::PlainVertex> tileLineStripVertexes;

    StaticRasterVertexBuffer rasterBoundsBuffer {
        {{ 0, 0, 0, 0 }},
        {{ util::EXTENT, 0, 32767, 0 }},
        {{ 0, util::EXTENT, 0, 32767 }},
        {{ util::EXTENT, util::EXTENT, 32767, 32767 }},
    };

    VertexArrayObject tileBorderArray;
};

} // namespace mbgl
