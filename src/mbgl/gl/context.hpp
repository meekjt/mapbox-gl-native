#pragma once

#include <mbgl/gl/object.hpp>
#include <mbgl/gl/state.hpp>
#include <mbgl/gl/value.hpp>
#include <mbgl/gl/depth.hpp>
#include <mbgl/gl/stencil.hpp>
#include <mbgl/gl/color.hpp>
#include <mbgl/gl/program.hpp>
#include <mbgl/gl/vertexes.hpp>
#include <mbgl/gl/indexes.hpp>
#include <mbgl/gl/primitive.hpp>
#include <mbgl/util/noncopyable.hpp>

#include <memory>
#include <vector>
#include <array>

namespace mbgl {
namespace gl {

constexpr size_t TextureMax = 64;

class Context : private util::noncopyable {
public:
    ~Context();

    UniqueProgram createProgram();
    UniqueShader createVertexShader();
    UniqueShader createFragmentShader();
    UniqueBuffer createBuffer();
    UniqueTexture createTexture();
    UniqueVertexArray createVertexArray();
    UniqueFramebuffer createFramebuffer();

    void uploadBuffer(BufferType, size_t, void*);

    template <class V, std::size_t N>
    VertexBuffer<V> createVertexBuffer(const std::array<V, N>& a) {
        return VertexBuffer<V> {
            ElementLength { N },
            createVertexBufferWithData(a.data(), a.size() * sizeof(V))
        };
    }

    template <class V>
    VertexBuffer<V> createVertexBuffer(const std::vector<V>& v) {
        return VertexBuffer<V> {
            ElementLength { v.size() },
            createVertexBufferWithData(v.data(), v.size() * sizeof(V))
        };
    }

    void clear(optional<mbgl::Color> color,
               optional<float> depth,
               optional<int32_t> stencil);

    void draw(const Depth&,
              const Stencil&,
              const Color&,
              const Program&,
              const Vertexes&,
              const Indexes&,
              const Primitive&);

    // Actually remove the objects we marked as abandoned with the above methods.
    // Only call this while the OpenGL context is exclusive to this thread.
    void performCleanup();

    // Drain pools and remove abandoned objects, in preparation for destroying the store.
    // Only call this while the OpenGL context is exclusive to this thread.
    void reset();

    bool empty() const {
        return pooledTextures.empty()
            && abandonedPrograms.empty()
            && abandonedShaders.empty()
            && abandonedBuffers.empty()
            && abandonedTextures.empty()
            && abandonedVertexArrays.empty()
            && abandonedFramebuffers.empty();
    }

    void resetState();

    void setDirtyState();

    State<value::StencilFunc> stencilFunc;
    State<value::StencilMask> stencilMask;
    State<value::StencilTest> stencilTest;
    State<value::StencilOp> stencilOp;
    State<value::DepthRange> depthRange;
    State<value::DepthMask> depthMask;
    State<value::DepthTest> depthTest;
    State<value::DepthFunc> depthFunc;
    State<value::Blend> blend;
    State<value::BlendFunc> blendFunc;
    State<value::BlendColor> blendColor;
    State<value::ColorMask> colorMask;
    State<value::Program> program;
    State<value::LineWidth> lineWidth;
    State<value::ActiveTexture> activeTexture;
    State<value::BindFramebuffer> bindFramebuffer;
    State<value::Viewport> viewport;
#if not MBGL_USE_GLES2
    State<value::PixelZoom> pixelZoom;
    State<value::RasterPos> rasterPos;
#endif // MBGL_USE_GLES2
    std::array<State<value::BindTexture>, 2> texture;
    State<value::BindVertexBuffer> vertexBuffer;
    State<value::BindElementBuffer> elementBuffer;
    State<value::BindVertexArray> vertexArrayObject;

private:
    UniqueBuffer createVertexBufferWithData(const void* data, std::size_t size);

    friend detail::ProgramDeleter;
    friend detail::ShaderDeleter;
    friend detail::BufferDeleter;
    friend detail::TextureDeleter;
    friend detail::VertexArrayDeleter;
    friend detail::FramebufferDeleter;

    std::vector<TextureID> pooledTextures;

    std::vector<ProgramID> abandonedPrograms;
    std::vector<ShaderID> abandonedShaders;
    std::vector<BufferID> abandonedBuffers;
    std::vector<TextureID> abandonedTextures;
    std::vector<VertexArrayID> abandonedVertexArrays;
    std::vector<FramebufferID> abandonedFramebuffers;
};

} // namespace gl
} // namespace mbgl
