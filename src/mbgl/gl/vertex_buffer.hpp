#pragma once

#include <mbgl/gl/bytes.hpp>
#include <mbgl/gl/object.hpp>

namespace mbgl {
namespace gl {

template <class Vertex>
class VertexBuffer {
public:
    static constexpr std::size_t vertexSize = sizeof(Vertex);
    ElementLength vertexCount;
    UniqueBuffer buffer;
};

} // namespace gl
} // namespace mbgl
