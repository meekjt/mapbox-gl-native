#include <mbgl/renderer/circle_bucket.hpp>
#include <mbgl/renderer/painter.hpp>
#include <mbgl/gl/gl.hpp>

#include <mbgl/shader/circle_shader.hpp>
#include <mbgl/style/layers/circle_layer.hpp>
#include <mbgl/util/constants.hpp>

namespace mbgl {

using namespace style;

CircleBucket::CircleBucket(MapMode mode_) : mode(mode_) {
}

CircleBucket::~CircleBucket() {
    // Do not remove. header file only contains forward definitions to unique pointers.
}

void CircleBucket::upload(gl::Context& context) {
    vertexBuffer = context.createVertexBuffer(std::move(vertexes));
    indexBuffer = context.createIndexBuffer(std::move(indexes));
    uploaded = true;
}

void CircleBucket::render(Painter& painter,
                        PaintParameters& parameters, 
                        const Layer& layer,
                        const RenderTile& tile) {
    painter.renderCircle(parameters, *this, *layer.as<CircleLayer>(), tile);
}

bool CircleBucket::hasData() const {
    return !triangleGroups.empty();
}

bool CircleBucket::needsClipping() const {
    return true;
}

void CircleBucket::addGeometry(const GeometryCollection& geometryCollection) {
    for (auto& circle : geometryCollection) {
        for(auto & geometry : circle) {
            auto x = geometry.x;
            auto y = geometry.y;

            // Do not include points that are outside the tile boundaries.
            // Include all points in Still mode. You need to include points from
            // neighbouring tiles so that they are not clipped at tile boundaries.
            if ((mode != MapMode::Still) &&
                (x < 0 || x >= util::EXTENT || y < 0 || y >= util::EXTENT)) continue;

            // this geometry will be of the Point type, and we'll derive
            // two triangles from it.
            //
            // ┌─────────┐
            // │ 4     3 │
            // │         │
            // │ 1     2 │
            // └─────────┘
            //
            vertexes.emplace_back(x, y, -1, -1); // 1
            vertexes.emplace_back(x, y, 1, -1); // 2
            vertexes.emplace_back(x, y, 1, 1); // 3
            vertexes.emplace_back(x, y, -1, 1); // 4

            if (!triangleGroups.size() || (triangleGroups.back()->vertex_length + 4 > 65535)) {
                // Move to a new group because the old one can't hold the geometry.
                triangleGroups.emplace_back(std::make_unique<TriangleGroup>());
            }

            TriangleGroup& group = *triangleGroups.back();
            uint16_t index = group.vertex_length;

            // 1, 2, 3
            // 1, 4, 3
            indexes.push_back({index,
                               static_cast<uint16_t>(index + 1),
                               static_cast<uint16_t>(index + 2)});
            indexes.push_back({index,
                               static_cast<uint16_t>(index + 3),
                               static_cast<uint16_t>(index + 2)});

            group.vertex_length += 4;
            group.elements_length += 2;
        }
    }
}

void CircleBucket::drawCircles(CircleShader& shader, gl::Context& context) {
    GLbyte* vertexIndex = BUFFER_OFFSET(0);
    GLbyte* elementsIndex = BUFFER_OFFSET(0);

    for (auto& group : triangleGroups) {
        assert(group);

        if (!group->elements_length) continue;

        group->array[0].bind(shader, *vertexBuffer, *indexBuffer, vertexIndex, context);

        MBGL_CHECK_ERROR(glDrawElements(GL_TRIANGLES, group->elements_length * 3, GL_UNSIGNED_SHORT, elementsIndex));

        vertexIndex += group->vertex_length * vertexBuffer->vertexSize;
        elementsIndex += group->elements_length * indexBuffer->primitiveSize;
    }
}

} // namespace mbgl
