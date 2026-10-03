#pragma once

#include <cstddef>
#include <type_traits>

#include "pxPoint.hpp"
#include "pxPrecision.hpp"
#include "pxSize.hpp"

// A rectangle as a corner and a size. pxRegion_t holds the same thing as two corners.
template<typename T>
struct pxRect_t {
    static_assert(pxIsPrecision<T>, "pxRect_t comes in double, float or int32_t");

    T x{};
    T y{};
    T width{};
    T height{};

    constexpr pxRect_t() noexcept = default;
    constexpr pxRect_t(T px, T py, T w, T h) noexcept : x(px), y(py), width(w), height(h) {}
    constexpr pxRect_t(pxPoint_t<T> location, pxSize_t<T> size) noexcept
        : x(location.x), y(location.y), width(size.width), height(size.height) {}

    template<typename U>
    constexpr explicit(!pxIsLossless<U, T>) pxRect_t(const pxRect_t<U>& o) noexcept
        : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)),
          width(static_cast<T>(o.width)), height(static_cast<T>(o.height)) {}

    constexpr T Right() const noexcept { return x + width; }
    constexpr T Bottom() const noexcept { return y + height; }

    constexpr bool IsEmpty() const noexcept { return x == 0 && y == 0 && width == 0 && height == 0; }

    // The left and top edges are inside, the right and bottom ones are not.
    constexpr bool Contains(pxPoint_t<T> pt) const noexcept {
        return pt.x >= x && pt.x < Right() && pt.y >= y && pt.y < Bottom();
    }

    friend constexpr bool operator==(const pxRect_t&, const pxRect_t&) noexcept = default;
};

using pxRect  = pxRect_t<double>;
using pxRectf = pxRect_t<float>;
using pxRecti = pxRect_t<std::int32_t>;

// The layout is the contract with C# (docs/layout.md).
static_assert(std::is_standard_layout_v<pxRect> && std::is_trivially_copyable_v<pxRect>);
static_assert(sizeof(pxRect) == 32 && offsetof(pxRect, y) == 8 && offsetof(pxRect, width) == 16 && offsetof(pxRect, height) == 24);
static_assert(sizeof(pxRectf) == 16 && offsetof(pxRectf, height) == 12);
static_assert(sizeof(pxRecti) == 16 && offsetof(pxRecti, height) == 12);
