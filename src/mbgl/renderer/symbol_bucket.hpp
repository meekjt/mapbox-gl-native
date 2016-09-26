#pragma once

#include <mbgl/renderer/bucket.hpp>
#include <mbgl/map/mode.hpp>
#include <mbgl/gl/element_group.hpp>
#include <mbgl/gl/vertex_buffer.hpp>
#include <mbgl/gl/index_buffer.hpp>
#include <mbgl/shader/texture_rect_vertex.hpp>
#include <mbgl/shader/collision_box_vertex.hpp>
#include <mbgl/text/glyph_range.hpp>
#include <mbgl/style/layers/symbol_layer_properties.hpp>

#include <memory>
#include <vector>

namespace mbgl {

class SDFShader;
class IconShader;
class CollisionBoxShader;

class SymbolBucket : public Bucket {
public:
    SymbolBucket(const MapMode,
                 style::SymbolLayoutProperties,
                 bool sdfIcons,
                 bool iconsNeedLinear);

    void upload(gl::Context&) override;
    void render(Painter&, PaintParameters&, const style::Layer&, const RenderTile&) override;
    bool hasData() const override;
    bool hasTextData() const;
    bool hasIconData() const;
    bool hasCollisionBoxData() const;
    bool needsClipping() const override;

    void drawGlyphs(SDFShader&, gl::Context&, PaintMode);
    void drawIcons(SDFShader&, gl::Context&, PaintMode);
    void drawIcons(IconShader&, gl::Context&, PaintMode);
    void drawCollisionBoxes(CollisionBoxShader&, gl::Context&);

    const MapMode mode;
    const style::SymbolLayoutProperties layout;
    const bool sdfIcons;
    const bool iconsNeedLinear;

private:
    friend class SymbolLayout;

    typedef gl::ElementGroup<2> TextElementGroup;
    typedef gl::ElementGroup<4> IconElementGroup;
    typedef gl::ElementGroup<1> CollisionBoxElementGroup;

    struct TextBuffer {
        std::vector<TextureRectVertex> vertexes;
        std::vector<gl::Triangle<uint16_t>> triangles;
        std::vector<std::unique_ptr<TextElementGroup>> groups;

        optional<gl::VertexBuffer<TextureRectVertex>> vertexBuffer;
        optional<gl::IndexBuffer<gl::Triangle<uint16_t>>> indexBuffer;
    } text;

    struct IconBuffer {
        std::vector<TextureRectVertex> vertexes;
        std::vector<gl::Triangle<uint16_t>> triangles;
        std::vector<std::unique_ptr<IconElementGroup>> groups;

        optional<gl::VertexBuffer<TextureRectVertex>> vertexBuffer;
        optional<gl::IndexBuffer<gl::Triangle<uint16_t>>> indexBuffer;
    } icon;

    struct CollisionBoxBuffer {
        std::vector<CollisionBoxVertex> vertexes;
        std::vector<gl::Line<uint16_t>> lines;
        std::vector<std::unique_ptr<CollisionBoxElementGroup>> groups;

        optional<gl::VertexBuffer<CollisionBoxVertex>> vertexBuffer;
        optional<gl::IndexBuffer<gl::Line<uint16_t>>> indexBuffer;
    } collisionBox;
};

} // namespace mbgl
