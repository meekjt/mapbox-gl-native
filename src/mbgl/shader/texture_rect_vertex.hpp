#pragma once

#include <mbgl/util/geometry.hpp>

#include <cmath>

namespace mbgl {

// A vertex that holds a position, offset and texture coordinate. Used for symbol layer icons and glyphs.
class TextureRectVertex {
public:
    TextureRectVertex(int16_t x, int16_t y, float ox, float oy, uint16_t tx, uint16_t ty, float minzoom, float maxzoom, float labelminzoom, uint8_t labelangle)
        : a_pos(x, y),
          a_offset(::round(ox * 64), ::round(oy * 64)),  // use 1/64 pixels for placement
          a_texture_pos(tx / 4, ty / 4),
          a_data {
              static_cast<uint8_t>(labelminzoom * 10), // 1/10 zoom levels: z16 == 160
              static_cast<uint8_t>(labelangle),
              static_cast<uint8_t>(minzoom * 10),
              static_cast<uint8_t>(::fmin(maxzoom, 25) * 10)
          } {
    }

    Point<int16_t> a_pos;
    Point<int16_t> a_offset;
    Point<uint16_t> a_texture_pos;
    uint8_t a_data[4];

    static void bind(const int8_t* offset);
};

} // namespace mbgl
