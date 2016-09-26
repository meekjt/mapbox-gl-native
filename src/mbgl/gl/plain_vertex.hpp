#pragma once

#include <mbgl/util/geometry.hpp>
#include <mbgl/gl/vertexes.hpp>
#include <mbgl/shader/shader.hpp>

namespace mbgl {
namespace gl {

class PlainVertex {
public:
    Point<int16_t> a_pos;
};

template <>
struct VertexAttributes<PlainVertex> {
    std::vector<VertexAttribute> operator()() const {
        return {
            TypedVertexAttribute<2, int16_t> {
                Shader::a_pos,
                offsetof(PlainVertex, a_pos),
                sizeof(PlainVertex)
            }
        };
    };
};

} // namespace gl
} // namespace mbgl
