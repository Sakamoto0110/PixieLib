#pragma once

#include <cstddef>
#include <string>
#include <type_traits>

#include "pxPoint.hpp"
#include "pxPrecision.hpp"
#include "pxRect.hpp"
#include "pxToString.hpp"

// A rectangle as two corners: (x1, y1) is inside, (x2, y2) is just outside, as in pxRect_t, where
// x2 is x + width. It converts to and from pxRect_t explicitly, because the sums and differences can
// round in floating point.
template<typename T>
struct pxRegion_t {
    static_assert(pxIsPrecision<T>, "pxRegion_t comes in double, float or int32_t");

    T x1{};
    T y1{};
    T x2{};
    T y2{};

    constexpr pxRegion_t() noexcept = default;
    constexpr pxRegion_t(T px1, T py1, T px2, T py2) noexcept : x1(px1), y1(py1), x2(px2), y2(py2) {}
    constexpr explicit pxRegion_t(const pxRect_t<T>& r) noexcept : x1(r.x), y1(r.y), x2(r.Right()), y2(r.Bottom()) {}

    template<typename U>
    constexpr explicit(!pxIsLossless<U, T>) pxRegion_t(const pxRegion_t<U>& o) noexcept
        : x1(static_cast<T>(o.x1)), y1(static_cast<T>(o.y1)),
          x2(static_cast<T>(o.x2)), y2(static_cast<T>(o.y2)) {}

    constexpr explicit operator pxRect_t<T>() const noexcept { return { x1, y1, Width(), Height() }; }

    constexpr T Width() const noexcept { return x2 - x1; }
    constexpr T Height() const noexcept { return y2 - y1; }

    constexpr bool IsEmpty() const noexcept { return x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0; }

    std::string ToString() const { return pxTupleString(x1, y1, x2, y2); }

    // The same edges as pxRect_t::Contains: x1 and y1 are inside, x2 and y2 are not.
    constexpr bool Contains(pxPoint_t<T> pt) const noexcept {
        return pt.x >= x1 && pt.x < x2 && pt.y >= y1 && pt.y < y2;
    }

    friend constexpr bool operator==(const pxRegion_t&, const pxRegion_t&) noexcept = default;
};

using pxRegion  = pxRegion_t<double>;
using pxRegionf = pxRegion_t<float>;
using pxRegioni = pxRegion_t<std::int32_t>;

// The layout is the contract with C# (docs/layout.md).
static_assert(std::is_standard_layout_v<pxRegion> && std::is_trivially_copyable_v<pxRegion>);
static_assert(sizeof(pxRegion) == 32 && offsetof(pxRegion, y1) == 8 && offsetof(pxRegion, x2) == 16 && offsetof(pxRegion, y2) == 24);
static_assert(sizeof(pxRegionf) == 16 && offsetof(pxRegionf, y2) == 12);
static_assert(sizeof(pxRegioni) == 16 && offsetof(pxRegioni, y2) == 12);
