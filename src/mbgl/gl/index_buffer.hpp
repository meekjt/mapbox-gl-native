#pragma once

#include <mbgl/gl/bytes.hpp>
#include <mbgl/gl/object.hpp>

namespace mbgl {
namespace gl {

template <class Index>
struct Line {
    static constexpr std::size_t IndexCount = 2;
    Index a;
    Index b;
};

template <class Index>
struct Triangle {
    static constexpr std::size_t IndexCount = 3;
    Index a;
    Index b;
    Index c;
};

template <class Primitive>
class IndexBuffer {
public:
    static constexpr std::size_t primitiveSize = sizeof(Primitive);
    ElementLength indexCount;
    UniqueBuffer buffer;
};

} // namespace gl
} // namespace mbgl
