#pragma once

#include <cstdint>
#include <type_traits>

// The precisions a primitive comes in. The unsuffixed alias is double, then f for float and i for
// int32_t (docs/notas.md, 4.1). The layout of each one is part of the contract with C#
// (docs/layout.md), so no other type is accepted.
template<typename T>
inline constexpr bool pxIsPrecision =
    std::is_same_v<T, double> || std::is_same_v<T, float> || std::is_same_v<T, std::int32_t>;

// A conversion between precisions is implicit only when nothing is lost (P8.5): a float and an
// int32_t fit in a double. Every other one is explicit and works like a static_cast (it truncates).
template<typename From, typename To>
inline constexpr bool pxIsLossless =
    std::is_same_v<From, To> ||
    (std::is_same_v<To, double> && (std::is_same_v<From, float> || std::is_same_v<From, std::int32_t>));
