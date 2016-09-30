#pragma once

#include <mbgl/gl/bytes.hpp>
#include <mbgl/gl/object.hpp>

namespace mbgl {
namespace gl {

struct Line {
    static constexpr std::size_t IndexCount = 2;
    uint16_t a;
    uint16_t b;
};

struct Triangle {
    static constexpr std::size_t IndexCount = 3;
    uint16_t a;
    uint16_t b;
    uint16_t c;
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
