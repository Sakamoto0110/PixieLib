#pragma once

#include <cstddef>
#include <type_traits>

#include "pxPrecision.hpp"
#include "pxSize.hpp"

// A position. A point and a size hold the same data, so each converts to the other, explicitly; a
// point moves by a size.
template<typename T>
struct pxPoint_t {
    static_assert(pxIsPrecision<T>, "pxPoint_t comes in double, float or int32_t");

    T x{};
    T y{};

    constexpr pxPoint_t() noexcept = default;
    constexpr pxPoint_t(T px, T py) noexcept : x(px), y(py) {}
    constexpr explicit pxPoint_t(const pxSize_t<T>& sz) noexcept : x(sz.width), y(sz.height) {}

    template<typename U>
    constexpr explicit(!pxIsLossless<U, T>) pxPoint_t(const pxPoint_t<U>& o) noexcept
        : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}

    constexpr explicit operator pxSize_t<T>() const noexcept { return { x, y }; }

    constexpr bool IsEmpty() const noexcept { return x == 0 && y == 0; }

    friend constexpr bool operator==(const pxPoint_t&, const pxPoint_t&) noexcept = default;

    friend constexpr pxPoint_t operator+(pxPoint_t a, pxPoint_t b) noexcept { return { a.x + b.x, a.y + b.y }; }
    friend constexpr pxPoint_t operator-(pxPoint_t a, pxPoint_t b) noexcept { return { a.x - b.x, a.y - b.y }; }
    friend constexpr pxPoint_t operator+(pxPoint_t pt, pxSize_t<T> sz) noexcept { return { pt.x + sz.width, pt.y + sz.height }; }
    friend constexpr pxPoint_t operator-(pxPoint_t pt, pxSize_t<T> sz) noexcept { return { pt.x - sz.width, pt.y - sz.height }; }
    friend constexpr pxPoint_t operator*(pxPoint_t pt, T k) noexcept { return { pt.x * k, pt.y * k }; }
    friend constexpr pxPoint_t operator*(T k, pxPoint_t pt) noexcept { return pt * k; }
    friend constexpr pxPoint_t operator/(pxPoint_t pt, T k) noexcept { return { pt.x / k, pt.y / k }; }
    friend constexpr pxPoint_t operator+(pxPoint_t pt) noexcept { return pt; }
    friend constexpr pxPoint_t operator-(pxPoint_t pt) noexcept { return { -pt.x, -pt.y }; }
};

using pxPoint  = pxPoint_t<double>;
using pxPointf = pxPoint_t<float>;
using pxPointi = pxPoint_t<std::int32_t>;

// The layout is the contract with C# (docs/layout.md).
static_assert(std::is_standard_layout_v<pxPoint> && std::is_trivially_copyable_v<pxPoint>);
static_assert(sizeof(pxPoint) == 16 && offsetof(pxPoint, y) == 8);
static_assert(sizeof(pxPointf) == 8 && offsetof(pxPointf, y) == 4);
static_assert(sizeof(pxPointi) == 8 && offsetof(pxPointi, y) == 4);
