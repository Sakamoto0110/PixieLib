#pragma once

#include <cstddef>
#include <string>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <pixie/math/pxVec.hpp>

// The matrices are GLM's (6.1), under the names of PixieLib: the unsuffixed one is double, f is
// float, and there is no int one. A vector is a column, multiplied on the right (M * v), and the
// memory is column-major, what GLSL and glUniformMatrix4fv take without transposing (1.12): m[3] is
// the fourth column, where glm::translate puts the translation, and in M1 * M2 * v the M2 is applied
// first. The math is GLM's too: glm::transpose, glm::determinant, glm::translate, glm::rotate,
// glm::scale, glm::lookAt, glm::ortho, glm::perspective. The projections are OpenGL's: right-handed,
// with the depth in [-1, 1] (1.13). The C# PxMat types have the same layout and the same functions,
// with the same bits (docs/notas.md, 4.14).
//
// With GLM_FORCE_CTOR_INIT, from pixie::math, a matrix starts as the identity, as GLSL's mat4(1);
// the default of the C# struct is zero (6.3).
using pxMat3  = glm::dmat3;
using pxMat3f = glm::mat3;
using pxMat4  = glm::dmat4;
using pxMat4f = glm::mat4;

namespace pxMathDetail {
// GLM's SIMD inverse of a float mat4, in math/src/pxMat.cpp: it only exists with GLM_FORCE_INTRINSICS,
// which takes the constexpr out of every GLM type, so it is compiled apart, behind raw floats (6.2).
void InverseSimd(const float* in, float* out) noexcept;
}  // namespace pxMathDetail

// The inverse of a matrix, glm::inverse with the same bits. On x86-64, a float mat4 goes through the
// SIMD version of GLM, which keeps the layout of pxMat4f and takes about half the time; the double
// one and the mat3 have no SIMD version in GLM.
template<glm::length_t L, typename T, glm::qualifier Q>
glm::mat<L, L, T, Q> pxInverse(const glm::mat<L, L, T, Q>& m) {
#if defined(__x86_64__) || defined(_M_X64)
    if constexpr (L == 4 && std::is_same_v<T, float>) {
        glm::mat<4, 4, float, Q> result;
        pxMathDetail::InverseSimd(&m[0][0], &result[0][0]);
        return result;
    }
#endif
    return glm::inverse(m);
}

// The orthographic projection of the 2D (1.13): the primitives have the origin at the top left and
// Y down, as the editor, and OpenGL has Y up. (0, 0) goes to the top left corner of the screen, (-1, 1),
// and (width, height) to the bottom right, (1, -1). It is glm::ortho(0, width, height, 0): z comes
// out negated, with no near and far planes.
template<typename T>
    requires std::is_floating_point_v<T>
glm::mat<4, 4, T> pxOrtho2D(T width, T height) {
    return glm::ortho(T(0), width, height, T(0));
}

// The text of a matrix, column by column, as it is in memory: "((1, 0, 0), (0, 1, 0), (0, 0, 1))".
template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
std::string pxToString(const glm::mat<C, R, T, Q>& m) {
    std::string out = "(";
    for (glm::length_t c = 0; c < C; ++c) {
        if (c > 0)
            out += ", ";
        out += pxToString(m[c]);
    }
    out += ')';
    return out;
}

// The layout is the contract with C# (docs/layout.md): the columns one after the other, with no gap.
static_assert(std::is_standard_layout_v<pxMat4> && std::is_trivially_copyable_v<pxMat4>);
static_assert(std::is_standard_layout_v<pxMat3f> && std::is_trivially_copyable_v<pxMat3f>);
static_assert(sizeof(pxMat3) == 72 && sizeof(pxMat3f) == 36);
static_assert(sizeof(pxMat4) == 128 && sizeof(pxMat4f) == 64);
static_assert(alignof(pxMat4f) == 4, "GLM's aligned types change the layout (docs/layout.md)");
