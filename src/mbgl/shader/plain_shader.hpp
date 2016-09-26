#pragma once

#include <mbgl/shader/shader.hpp>
#include <mbgl/shader/uniform.hpp>
#include <mbgl/gl/program.hpp>
#include <mbgl/util/mat4.hpp>
#include <mbgl/util/color.hpp>

namespace mbgl {

class PlainShader : public Shader {
public:
    PlainShader(gl::Context&, Defines defines = None);

    gl::Program bindUniforms(const mat4& matrix,
                             const Color& color,
                             float opacity);

// TODO: private:
    void bind(int8_t* offset) final;

    UniformMatrix<4> u_matrix   = {"u_matrix",  *this};
    Uniform<Color>   u_color    = {"u_color",   *this};
    Uniform<float>   u_opacity  = {"u_opacity", *this};
};

} // namespace mbgl
