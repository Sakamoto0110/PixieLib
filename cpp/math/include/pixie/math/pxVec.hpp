#pragma once

#include <cstddef>
#include <string>
#include <type_traits>

#include <glm/glm.hpp>

#include <pixie/pxPoint.hpp>
#include <pixie/pxToString.hpp>

// The vectors are GLM's (6.1), under the names of PixieLib: the unsuffixed one is double, then f for
// float and i for int32_t (docs/notas.md, 4.1). The math is GLM's too: glm::dot, glm::cross,
// glm::length, glm::normalize, glm::mix... The C# PxVec types have the same layout and, for each of
// these, the same operation under the name System.Numerics gives it (docs/notas.md, 4.13).
//
// The layout is the contract with C# (docs/layout.md), so GLM has to keep its packed types: with
// GLM_FORCE_DEFAULT_ALIGNED_GENTYPES a vec3 takes 16 bytes instead of 12, and the static_asserts below
// stop the build.
using pxVec2  = glm::dvec2;
using pxVec2f = glm::vec2;
using pxVec2i = glm::ivec2;
using pxVec3  = glm::dvec3;
using pxVec3f = glm::vec3;
using pxVec3i = glm::ivec3;
using pxVec4  = glm::dvec4;
using pxVec4f = glm::vec4;
using pxVec4i = glm::ivec4;

// The text of a vector, the same as that of a primitive (4.10): "(1.5, -2, 0)". A function, since an
// alias cannot have a ToString method.
template<glm::length_t L, typename T, glm::qualifier Q>
std::string pxToString(const glm::vec<L, T, Q>& v) {
    if constexpr (L == 2)
        return pxTupleString(v.x, v.y);
    else if constexpr (L == 3)
        return pxTupleString(v.x, v.y, v.z);
    else
        return pxTupleString(v.x, v.y, v.z, v.w);
}

// A point and a vec2 of the same precision hold the same data, so each converts to the other, as a
// point and a size do: explicitly, through a function, since an alias cannot have a constructor
// (6.6). In C#, (PxVec2)point and (PxPoint)vector.
template<typename T>
constexpr glm::vec<2, T> pxToVec2(const pxPoint_t<T>& p) noexcept {
    return glm::vec<2, T>(p.x, p.y);
}

template<typename T, glm::qualifier Q>
constexpr pxPoint_t<T> pxToPoint(const glm::vec<2, T, Q>& v) noexcept {
    return pxPoint_t<T>(v.x, v.y);
}

// The layout is the contract with C# (docs/layout.md).
static_assert(std::is_standard_layout_v<pxVec3> && std::is_trivially_copyable_v<pxVec3>);
static_assert(std::is_standard_layout_v<pxVec4f> && std::is_trivially_copyable_v<pxVec4f>);
static_assert(sizeof(pxVec2) == 16 && sizeof(pxVec2f) == 8 && sizeof(pxVec2i) == 8);
static_assert(sizeof(pxVec3) == 24 && sizeof(pxVec3f) == 12 && sizeof(pxVec3i) == 12);
static_assert(sizeof(pxVec4) == 32 && sizeof(pxVec4f) == 16 && sizeof(pxVec4i) == 16);
static_assert(offsetof(pxVec3, z) == 16 && offsetof(pxVec3f, z) == 8 && offsetof(pxVec4i, w) == 12);
static_assert(alignof(pxVec3f) == 4 && alignof(pxVec4f) == 4,
              "GLM's aligned types change the layout (docs/layout.md)");
