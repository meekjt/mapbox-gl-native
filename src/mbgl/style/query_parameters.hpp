#pragma once

#include <mbgl/util/geo.hpp>

#include <unordered_set>
#include <vector>

namespace mbgl {

class TransformState;

namespace style {

class QueryParameters {
public:
    const ScreenLineString& geometry;
    const TransformState& transformState;
    const optional<std::vector<std::string>>& layerIDs;
    const optional<std::unordered_set<std::string>>& sourceIDs;
};

} // namespace style
} // namespace mbgl
