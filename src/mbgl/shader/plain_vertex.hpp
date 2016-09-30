#pragma once

#include <mbgl/gl/attribute.hpp>

#include <array>
#include <cstdint>

namespace mbgl {

class PlainVertex {
public:
    PlainVertex(int16_t x, int16_t y)
        : a_pos { x, y } {}

    const int16_t a_pos[2];
};

template <class Shader>
struct gl::AttributeBindings<Shader, PlainVertex> {
    std::array<AttributeBinding, 1> operator()(const Shader& shader) {
        return {{
            MBGL_MAKE_ATTRIBUTE_BINDING(PlainVertex, shader, a_pos)
        }};
    };
};

} // namespace mbgl
