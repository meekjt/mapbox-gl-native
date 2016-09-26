#pragma once

#include <mbgl/util/geometry.hpp>

#include <cmath>

namespace mbgl {

class CollisionBoxVertex {
public:
    CollisionBoxVertex(int16_t x, int16_t y, float ox, float oy, float maxzoom, float placementZoom)
        : a_pos(x, y),
          a_extrude(::round(ox), ::round(oy)),
          a_data {
              static_cast<uint8_t>(maxzoom * 10),
              static_cast<uint8_t>(placementZoom * 10)
          } {
    }

    Point<int16_t> a_pos;
    Point<int16_t> a_extrude;
    uint8_t a_data[2];

    static void bind(const int8_t* offset);
};

} // namespace mbgl
