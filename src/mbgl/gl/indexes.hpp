#pragma once

#include <mbgl/gl/bytes.hpp>
#include <mbgl/gl/primitive.hpp>

namespace mbgl {
namespace gl {

template <typename Primitive, typename T>
class IndexBuffer {
public:
    const UniqueBuffer& buffer;
    ElementLength indexCount;
    ByteOffset byteOffset;
};

class Unindexed {
public:
};

using Indexes = variant<
    Unindexed,
    IndexBuffer<Points, uint16_t>,
    IndexBuffer<Lines, uint16_t>,
    IndexBuffer<LineStrip, uint16_t>,
    IndexBuffer<Triangles, uint16_t>>;

} // namespace gl
} // namespace mbgl
