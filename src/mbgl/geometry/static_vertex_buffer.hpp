#pragma once

#include <mbgl/geometry/buffer.hpp>

#include <array>
#include <cstdint>

namespace mbgl {

class StaticRasterVertexBuffer : public Buffer<
    8, // bytes per vertex (4 * signed short == 8 bytes)
    gl::BufferType::Vertex,
    32 // default length
> {
public:
    using VertexType = int16_t;
    StaticRasterVertexBuffer(std::initializer_list<std::array<VertexType, 4>>);
};

} // namespace mbgl
