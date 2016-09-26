#pragma once

namespace mbgl {
namespace gl {

struct ElementOffset {
    std::size_t value;
};

struct ElementLength {
    std::size_t value;
};

struct ByteOffset {
    std::size_t value;
    operator const void* () const { return reinterpret_cast<const void*>(value); }
};

inline ByteOffset operator+(const ByteOffset& a, const ByteOffset& b) {
    return ByteOffset { a.value + b.value };
}

struct ByteLength {
    std::size_t value;
};

inline ByteLength operator+(const ByteLength& a, const ByteLength& b) {
    return ByteLength { a.value + b.value };
}

inline ByteOffset operator*(const ByteLength& a, const ElementOffset& b) {
    return ByteOffset { a.value * b.value };
}

} // namespace gl
} // namespace mbgl
