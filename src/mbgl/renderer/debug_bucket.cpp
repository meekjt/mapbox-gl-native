#include <mbgl/renderer/debug_bucket.hpp>
#include <mbgl/renderer/painter.hpp>
#include <mbgl/shader/plain_shader.hpp>
#include <mbgl/shader/plain_vertex.hpp>
#include <mbgl/geometry/debug_font_data.hpp>
#include <mbgl/util/string.hpp>

#include <mbgl/gl/gl.hpp>

#include <cmath>
#include <string>
#include <vector>

namespace mbgl {

std::vector<PlainVertex> buildTextVertexes(const OverscaledTileID& id,
                                                     const bool renderable,
                                                     const bool complete,
                                                     optional<Timestamp> modified,
                                                     optional<Timestamp> expires,
                                                     MapDebugOptions debugMode) {
    std::vector<PlainVertex> textPoints;

    auto addText = [&] (const std::string& text, double left, double baseline, double scale) {
        for (uint8_t c : text) {
            if (c < 32 || c >= 127)
                continue;

            optional<Point<int16_t>> prev;

            const glyph& glyph = simplex[c - 32];
            for (int32_t j = 0; j < glyph.length; j += 2) {
                if (glyph.data[j] == -1 && glyph.data[j + 1] == -1) {
                    prev = {};
                } else {
                    Point<int16_t> p {
                        int16_t(::round(left + glyph.data[j] * scale)),
                        int16_t(::round(baseline - glyph.data[j + 1] * scale))
                    };

                    if (prev) {
                        textPoints.emplace_back(prev->x, prev->y);
                        textPoints.emplace_back(p.x, p.y);
                    }

                    prev = p;
                }
            }

            left += glyph.width * scale;
        }
    };

    double baseline = 200;
    if (debugMode & MapDebugOptions::ParseStatus) {
        const std::string text = util::toString(id) + " - " +
                                 (complete ? "complete" : renderable ? "renderable" : "pending");
        addText(text, 50, baseline, 5);
        baseline += 200;
    }

    if (debugMode & MapDebugOptions::Timestamps && modified && expires) {
        const std::string modifiedText = "modified: " + util::iso8601(*modified);
        addText(modifiedText, 50, baseline, 5);

        const std::string expiresText = "expires: " + util::iso8601(*expires);
        addText(expiresText, 50, baseline + 200, 5);
    }

    return textPoints;
}

DebugBucket::DebugBucket(const OverscaledTileID& id,
                         const bool renderable_,
                         const bool complete_,
                         optional<Timestamp> modified_,
                         optional<Timestamp> expires_,
                         MapDebugOptions debugMode_,
                         gl::Context& context)
    : renderable(renderable_),
      complete(complete_),
      modified(std::move(modified_)),
      expires(std::move(expires_)),
      debugMode(debugMode_),
      textVertexes(context.createVertexBuffer(buildTextVertexes(id, renderable_, complete_, modified_, expires_, debugMode_))) {
}

void DebugBucket::drawLines(PlainShader& shader, gl::Context& context) {
    if (textVertexes.vertexCount.value != 0) {
        array.bind(shader, textVertexes, BUFFER_OFFSET_0, context);
        MBGL_CHECK_ERROR(glDrawArrays(GL_LINES, 0, textVertexes.vertexCount.value));
    }
}

void DebugBucket::drawPoints(PlainShader& shader, gl::Context& context) {
    if (textVertexes.vertexCount.value != 0) {
        array.bind(shader, textVertexes, BUFFER_OFFSET_0, context);
        MBGL_CHECK_ERROR(glDrawArrays(GL_POINTS, 0, textVertexes.vertexCount.value));
    }
}

} // namespace mbgl
