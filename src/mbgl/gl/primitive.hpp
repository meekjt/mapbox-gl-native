#pragma once

namespace mbgl {
namespace gl {

class Points {
public:
    float pointSize;
};

class Lines {
public:
    float lineWidth;
};

class LineStrip {
public:
    float lineWidth;
};

class Triangles {
public:
};

class TriangleStrip {
public:
};

using Primitive = variant<
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip>;

} // namespace gl
} // namespace mbgl
