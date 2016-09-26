#pragma once

#include <mbgl/util/geometry.hpp>

namespace mbgl {

class RasterVertex {
public:
    Point<int16_t> a_pos;
    Point<int16_t> a_texture_pos;

    static void bind(const int8_t* offset);
};

} // namespace mbgl
