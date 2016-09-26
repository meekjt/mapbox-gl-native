#pragma once

#include <mbgl/util/geometry.hpp>

namespace mbgl {

class CircleVertex {
public:
    /*
     * @param {number} x vertex position
     * @param {number} y vertex position
     * @param {number} ex extrude normal
     * @param {number} ey extrude normal
     */
    CircleVertex(int16_t x, int16_t y, float ex, float ey)
        : a_pos((x * 2) + ((ex + 1) / 2), (y * 2) + ((ey + 1) / 2)) {
    }

    Point<int16_t> a_pos;

    static void bind(const int8_t* offset);
};

} // namespace mbgl
