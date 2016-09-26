#pragma once

#include <mbgl/gl/bytes.hpp>
#include <mbgl/gl/object.hpp>
#include <mbgl/util/variant.hpp>

namespace mbgl {
namespace gl {

template <class Vertex>
class VertexBuffer {
public:
    ElementLength vertexCount;
    UniqueBuffer buffer;
};

template <std::size_t N, class T>
class TypedVertexAttribute {
public:
    gl::AttributeLocation location;
    ByteOffset byteOffset;
    ByteLength byteStride;
};

using VertexAttribute = variant<
    TypedVertexAttribute<1, int8_t>,
    TypedVertexAttribute<2, int8_t>,
    TypedVertexAttribute<3, int8_t>,
    TypedVertexAttribute<4, int8_t>,
    TypedVertexAttribute<1, uint8_t>,
    TypedVertexAttribute<2, uint8_t>,
    TypedVertexAttribute<3, uint8_t>,
    TypedVertexAttribute<4, uint8_t>,
    TypedVertexAttribute<1, int16_t>,
    TypedVertexAttribute<2, int16_t>,
    TypedVertexAttribute<3, int16_t>,
    TypedVertexAttribute<4, int16_t>,
    TypedVertexAttribute<1, uint16_t>,
    TypedVertexAttribute<2, uint16_t>,
    TypedVertexAttribute<3, uint16_t>,
    TypedVertexAttribute<4, uint16_t>,
    TypedVertexAttribute<1, int32_t>,
    TypedVertexAttribute<2, int32_t>,
    TypedVertexAttribute<3, int32_t>,
    TypedVertexAttribute<4, int32_t>,
    TypedVertexAttribute<1, uint32_t>,
    TypedVertexAttribute<2, uint32_t>,
    TypedVertexAttribute<3, uint32_t>,
    TypedVertexAttribute<4, uint32_t>,
    TypedVertexAttribute<1, float>,
    TypedVertexAttribute<2, float>,
    TypedVertexAttribute<3, float>,
    TypedVertexAttribute<4, float>,
    TypedVertexAttribute<1, double>,
    TypedVertexAttribute<2, double>,
    TypedVertexAttribute<3, double>,
    TypedVertexAttribute<4, double>>;

template <class V> struct VertexAttributes;

class Vertexes {
public:
    template <class V>
    Vertexes(const VertexBuffer<V>& buffer_)
        : buffer(buffer_.buffer),
          vertexSize {sizeof(V)},
          vertexOffset {0},
          vertexCount(buffer_.vertexCount),
          attributes(VertexAttributes<V>()()) {}

    const UniqueBuffer& buffer;
    ByteLength vertexSize;
    ElementOffset vertexOffset;
    ElementLength vertexCount;
    std::vector<VertexAttribute> attributes;
};

} // namespace gl
} // namespace mbgl
