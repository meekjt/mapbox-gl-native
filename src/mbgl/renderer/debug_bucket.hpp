#pragma once

#include <mbgl/map/mode.hpp>
#include <mbgl/util/chrono.hpp>
#include <mbgl/util/geometry.hpp>
#include <mbgl/util/optional.hpp>
#include <mbgl/util/noncopyable.hpp>
#include <mbgl/gl/vertexes.hpp>
#include <mbgl/gl/plain_vertex.hpp>

namespace mbgl {

class OverscaledTileID;

namespace gl {
class Context;
} // namespace gl

class DebugBucket : private util::noncopyable {
public:
    DebugBucket(const OverscaledTileID& id,
                bool renderable,
                bool complete,
                optional<Timestamp> modified,
                optional<Timestamp> expires,
                MapDebugOptions,
                gl::Context&);

    const bool renderable;
    const bool complete;
    const optional<Timestamp> modified;
    const optional<Timestamp> expires;
    const MapDebugOptions debugMode;

    const gl::VertexBuffer<gl::PlainVertex> textVertexes;
};

} // namespace mbgl
