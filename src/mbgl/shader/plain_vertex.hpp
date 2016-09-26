#pragma once

#include <mbgl/util/geometry.hpp>

namespace mbgl {

class PlainVertex {
public:
    Point<int16_t> a_pos;

    static void bind(const int8_t* offset);
};

} // namespace mbgl
