#pragma once

#include <mbgl/gl/types.hpp>
#include <mbgl/util/variant.hpp>

#include <vector>

namespace mbgl {
namespace gl {

template <typename T>
class UniformScalar {
public:
    gl::UniformLocation location;
    T value;
};

template <std::size_t N, typename T>
class UniformVector {
public:
    gl::UniformLocation location;
    std::array<T, N> value;
};

template <std::size_t N>
class UniformMatrix {
public:
    gl::UniformLocation location;
    std::array<float, N*N> value;
};

class UniformTexture2d {
public:
    gl::UniformLocation location;
    gl::TextureID texture;
};

using Uniform = variant<
    UniformScalar<int32_t>,
    UniformScalar<float>,
    UniformVector<2, float>,
    UniformVector<3, float>,
    UniformVector<4, float>,
    UniformMatrix<2>,
    UniformMatrix<3>,
    UniformMatrix<4>,
    UniformTexture2d>;

class Program {
public:
    gl::ProgramID program;
    std::vector<Uniform> uniforms;
};

} // namespace gl
} // namespace mbgl
