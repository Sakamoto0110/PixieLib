#pragma once

#include <cstddef>
#include <string>
#include <type_traits>

#include "pxPrecision.hpp"
#include "pxToString.hpp"

// A width and a height.
template<typename T>
struct pxSize_t {
    static_assert(pxIsPrecision<T>, "pxSize_t comes in double, float or int32_t");

    T width{};
    T height{};

    constexpr pxSize_t() noexcept = default;
    constexpr pxSize_t(T w, T h) noexcept : width(w), height(h) {}

    template<typename U>
    constexpr explicit(!pxIsLossless<U, T>) pxSize_t(const pxSize_t<U>& o) noexcept
        : width(static_cast<T>(o.width)), height(static_cast<T>(o.height)) {}

    constexpr bool IsEmpty() const noexcept { return width == 0 && height == 0; }

    std::string ToString() const { return pxTupleString(width, height); }

    friend constexpr bool operator==(const pxSize_t&, const pxSize_t&) noexcept = default;

    friend constexpr pxSize_t operator+(pxSize_t a, pxSize_t b) noexcept { return { a.width + b.width, a.height + b.height }; }
    friend constexpr pxSize_t operator-(pxSize_t a, pxSize_t b) noexcept { return { a.width - b.width, a.height - b.height }; }
    friend constexpr pxSize_t operator*(pxSize_t sz, T k) noexcept { return { sz.width * k, sz.height * k }; }
    friend constexpr pxSize_t operator*(T k, pxSize_t sz) noexcept { return sz * k; }
    friend constexpr pxSize_t operator/(pxSize_t sz, T k) noexcept { return { sz.width / k, sz.height / k }; }
    friend constexpr pxSize_t operator+(pxSize_t sz) noexcept { return sz; }
    friend constexpr pxSize_t operator-(pxSize_t sz) noexcept { return { -sz.width, -sz.height }; }
};

using pxSize  = pxSize_t<double>;
using pxSizef = pxSize_t<float>;
using pxSizei = pxSize_t<std::int32_t>;

// The layout is the contract with C# (docs/layout.md).
static_assert(std::is_standard_layout_v<pxSize> && std::is_trivially_copyable_v<pxSize>);
static_assert(sizeof(pxSize) == 16 && offsetof(pxSize, height) == 8);
static_assert(sizeof(pxSizef) == 8 && offsetof(pxSizef, height) == 4);
static_assert(sizeof(pxSizei) == 8 && offsetof(pxSizei, height) == 4);
