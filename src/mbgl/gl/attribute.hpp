#pragma once

#include <mbgl/gl/types.hpp>
#include <mbgl/gl/shader.hpp>

#include <vector>

namespace mbgl {
namespace gl {

template <typename T, std::size_t N>
class Attribute {
public:
    Attribute(const char* name, const Shader& shader)
        : location(shader.getAttributeLocation(name)) {}

    AttributeLocation location;
};

class AttributeBinding {
public:
    template <class Vertex, class T, std::size_t N, std::size_t O>
    AttributeBinding(const T (Vertex::*)[N], const Attribute<T, N>& attribute, std::integral_constant<std::size_t, O>)
        : location(attribute.location),
          type(DataTypeOf<T>::value),
          count(N),
          offset(O) {
        static_assert(std::is_standard_layout<Vertex>::value, "vertex type must use standard layout");
        static_assert(O % 4 == 0, "vertex attribute must be optimally aligned");
    }

    AttributeLocation location;
    DataType type;
    std::size_t count;
    std::size_t offset;
};

#define MBGL_MAKE_ATTRIBUTE_BINDING(Vertex, shader, name) \
    ::mbgl::gl::AttributeBinding(&Vertex::name, \
                                 shader.name, \
                                 std::integral_constant<std::size_t, offsetof(Vertex, name)>())

template <class Shader, class Vertex> struct AttributeBindings;

} // namespace gl
} // namespace mbgl
