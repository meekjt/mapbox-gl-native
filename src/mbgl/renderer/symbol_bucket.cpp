#include <mbgl/renderer/symbol_bucket.hpp>
#include <mbgl/renderer/painter.hpp>
#include <mbgl/style/layers/symbol_layer.hpp>
#include <mbgl/shader/sdf_shader.hpp>
#include <mbgl/shader/icon_shader.hpp>
#include <mbgl/shader/collision_box_shader.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {

using namespace style;

SymbolBucket::SymbolBucket(const MapMode mode_,
                           style::SymbolLayoutProperties layout_,
                           bool sdfIcons_,
                           bool iconsNeedLinear_)
    : mode(mode_),
      layout(std::move(layout_)),
      sdfIcons(sdfIcons_),
      iconsNeedLinear(iconsNeedLinear_) {
}

void SymbolBucket::upload(gl::Context& context) {
    if (hasTextData()) {
        text.vertexBuffer = context.createVertexBuffer(std::move(text.vertexes));
        text.indexBuffer = context.createIndexBuffer(std::move(text.triangles));
    }

    if (hasIconData()) {
        icon.vertexBuffer = context.createVertexBuffer(std::move(icon.vertexes));
        icon.indexBuffer = context.createIndexBuffer(std::move(icon.triangles));
    }

    if (hasCollisionBoxData()) {
        collisionBox.vertexBuffer = context.createVertexBuffer(std::move(collisionBox.vertexes));
    }

    uploaded = true;
}

void SymbolBucket::render(Painter& painter,
                          PaintParameters& parameters,
                          const Layer& layer,
                          const RenderTile& tile) {
    painter.renderSymbol(parameters, *this, *layer.as<SymbolLayer>(), tile);
}

bool SymbolBucket::hasData() const {
    assert(false); // Should be calling SymbolLayout::hasSymbolInstances() instead.
    return false;
}

bool SymbolBucket::hasTextData() const {
    return !text.groups.empty();
}

bool SymbolBucket::hasIconData() const {
    return !icon.groups.empty();
}

bool SymbolBucket::hasCollisionBoxData() const {
    return !collisionBox.groups.empty();
}

bool SymbolBucket::needsClipping() const {
    return mode == MapMode::Still;
}

void SymbolBucket::drawGlyphs(SDFShader& shader,
                              gl::Context& context,
                              PaintMode paintMode) {
    GLbyte* vertex_index = BUFFER_OFFSET_0;
    GLbyte* elements_index = BUFFER_OFFSET_0;
    for (auto& group : text.groups) {
        assert(group);
        group->array[paintMode == PaintMode::Overdraw ? 1 : 0].bind(
            shader, *text.vertexBuffer, *text.indexBuffer, vertex_index, context);
        MBGL_CHECK_ERROR(glDrawElements(GL_TRIANGLES, group->elements_length * 3, GL_UNSIGNED_SHORT,
                                        elements_index));
        vertex_index += group->vertex_length * text.vertexBuffer->vertexSize;
        elements_index += group->elements_length * text.indexBuffer->primitiveSize;
    }
}

void SymbolBucket::drawIcons(SDFShader& shader,
                             gl::Context& context,
                             PaintMode paintMode) {
    GLbyte* vertex_index = BUFFER_OFFSET_0;
    GLbyte* elements_index = BUFFER_OFFSET_0;
    for (auto& group : icon.groups) {
        assert(group);
        group->array[paintMode == PaintMode::Overdraw ? 1 : 0].bind(
            shader, *icon.vertexBuffer, *icon.indexBuffer, vertex_index, context);
        MBGL_CHECK_ERROR(glDrawElements(GL_TRIANGLES, group->elements_length * 3, GL_UNSIGNED_SHORT,
                                        elements_index));
        vertex_index += group->vertex_length * icon.vertexBuffer->vertexSize;
        elements_index += group->elements_length * icon.indexBuffer->primitiveSize;
    }
}

void SymbolBucket::drawIcons(IconShader& shader,
                             gl::Context& context,
                             PaintMode paintMode) {
    GLbyte* vertex_index = BUFFER_OFFSET_0;
    GLbyte* elements_index = BUFFER_OFFSET_0;
    for (auto& group : icon.groups) {
        assert(group);
        group->array[paintMode == PaintMode::Overdraw ? 3 : 2].bind(
            shader, *icon.vertexBuffer, *icon.indexBuffer, vertex_index, context);
        MBGL_CHECK_ERROR(glDrawElements(GL_TRIANGLES, group->elements_length * 3, GL_UNSIGNED_SHORT,
                                        elements_index));
        vertex_index += group->vertex_length * icon.vertexBuffer->vertexSize;
        elements_index += group->elements_length * icon.indexBuffer->primitiveSize;
    }
}

void SymbolBucket::drawCollisionBoxes(CollisionBoxShader& shader,
                                      gl::Context& context) {
    GLbyte* vertex_index = BUFFER_OFFSET_0;
    for (auto& group : collisionBox.groups) {
        group->array[0].bind(shader, *collisionBox.vertexBuffer, vertex_index, context);
        MBGL_CHECK_ERROR(glDrawArrays(GL_LINES, 0, group->vertex_length));
    }
}

} // namespace mbgl
