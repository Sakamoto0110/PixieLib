#include <cstring>
#include <new>
#include <type_traits>

#include <pixie/math/pxVec.hpp>

#include "check.hpp"

namespace {

template<typename V, typename K>
concept Multipliable = requires(V v, K k) { v * k; };

}  // namespace

void TestVec() {
    // A vector starts at zero, as in C# (GLM_FORCE_CTOR_INIT, from pixie::math), even on memory that
    // held something else.
    alignas(pxVec3) unsigned char memory[sizeof(pxVec3)];
    std::memset(memory, 0xFF, sizeof memory);
    PX_CHECK(*new (memory) pxVec3 == pxVec3(0, 0, 0));
    PX_CHECK(pxVec4i() == pxVec4i(0));

    pxVec3 a{ 1, 2, 3 };
    pxVec3 b{ 4, 5, 6 };
    PX_CHECK(a + b == pxVec3(5, 7, 9));
    PX_CHECK(b - a == pxVec3(3, 3, 3));
    PX_CHECK(a * b == pxVec3(4, 10, 18));
    PX_CHECK(a * 2.0 == pxVec3(2, 4, 6) && 2.0 * a == pxVec3(2, 4, 6));
    PX_CHECK(b / 2.0 == pxVec3(2, 2.5, 3));
    PX_CHECK(-a == pxVec3(-1, -2, -3));

    // The math is GLM's; these are the values the C# tests use too.
    PX_CHECK(glm::dot(a, b) == 32);
    PX_CHECK(glm::cross(pxVec3(1, 0, 0), pxVec3(0, 1, 0)) == pxVec3(0, 0, 1));
    PX_CHECK(glm::cross(a, b) == pxVec3(-3, 6, -3));
    PX_CHECK(glm::length(pxVec3(3, 4, 0)) == 5 && glm::distance(a, b) == glm::length(b - a));
    // GLM multiplies by 1 / length, so 3 / 5 comes out one unit in the last place off 0.6; the
    // results of a library are the library's (docs/layout.md, 3).
    PX_CHECK(glm::length(glm::normalize(pxVec3(3, 4, 0)) - pxVec3(0.6, 0.8, 0)) < 1e-15);
    PX_CHECK(glm::normalize(pxVec3(0, 0, 2)) == pxVec3(0, 0, 1));
    PX_CHECK(glm::normalize(pxVec3(3, 4, 0)).x == 3 * (1 / 5.0));

    // Two float results that System.Numerics would give one bit off (Vector2.Normalize divides,
    // Vector2.Lerp rounds in another order); the C# tests expect these same bits.
    PX_CHECK(glm::normalize(pxVec2f(11, 8)).x == 0.808736f);
    PX_CHECK(glm::mix(pxVec2f(7.6f), pxVec2f(4.8f), 0.22f).x == 6.9839993f);
    PX_CHECK(glm::mix(pxVec3(0, 0, 0), pxVec3(10, 20, 30), 0.25) == pxVec3(2.5, 5, 7.5));
    PX_CHECK(glm::min(a, pxVec3(2, 1, 3)) == pxVec3(1, 1, 3) && glm::max(a, pxVec3(2, 1, 3)) == pxVec3(2, 2, 3));
    PX_CHECK(glm::min(pxVec3(3, 2, 1), pxVec3(1, 2, 3)) == pxVec3(1, 2, 1));
    PX_CHECK(glm::max(pxVec3(3, 2, 1), pxVec3(1, 2, 3)) == pxVec3(3, 2, 3));
    PX_CHECK(glm::min(pxVec3i(3, 2, 1), pxVec3i(1, 2, 3)) == pxVec3i(1, 2, 1));
    PX_CHECK(glm::clamp(pxVec3(-1, 5, 2), pxVec3(0), pxVec3(3)) == pxVec3(0, 3, 2));
    PX_CHECK(glm::abs(pxVec3i(-1, 0, 2)) == pxVec3i(1, 0, 2));
    PX_CHECK(pxVec2f(1, 2) + pxVec2f(3, 4) == pxVec2f(4, 6));
    PX_CHECK(pxVec4(1, 2, 3, 4).w == 4 && glm::dot(pxVec4(1, 2, 3, 4), pxVec4(1)) == 10);

    // A conversion between precisions is explicit, even the one that loses nothing
    // (GLM_FORCE_EXPLICIT_CTOR, from pixie::math), and truncates.
    static_assert(!std::is_convertible_v<pxVec3f, pxVec3> && !std::is_convertible_v<pxVec3, pxVec3i>);
    static_assert(std::is_constructible_v<pxVec3, pxVec3f> && std::is_constructible_v<pxVec3i, pxVec3>);
    PX_CHECK(pxVec3i(pxVec3(2.9, -2.9, 0.5)) == pxVec3i(2, -2, 0));

    // The scalar has to be of the vector's own type: GLM is stricter than C#, where PxVec3 * 2 works.
    static_assert(Multipliable<pxVec3, double> && !Multipliable<pxVec3, int>);
    static_assert(Multipliable<pxVec3i, int> && !Multipliable<pxVec3i, double> && !Multipliable<pxVec3f, double>);

    // The text, the same as the primitives'.
    PX_CHECK(pxToString(pxVec3(1.5, -2, 0)) == "(1.5, -2, 0)");
    PX_CHECK(pxToString(pxVec2f(0.1f, 1e20f)) == "(0.1, 100000000000000000000)");
    PX_CHECK(pxToString(pxVec4i(1, -2, 3, 2147483647)) == "(1, -2, 3, 2147483647)");
}
