#include <cstdint>
#include <limits>
#include <type_traits>

#include <pixie/pxPoint.hpp>

#include "check.hpp"

namespace {

// Whether any of p * k, k * p and p / k compiles.
template<typename P, typename K>
concept Multipliable = requires(P p, K k) { p * k; } || requires(P p, K k) { k * p; } ||
                       requires(P p, K k) { p / k; };

}  // namespace

void TestPoint() {
    pxPoint pt{ 1.5, -2 };
    PX_CHECK(pt.x == 1.5 && pt.y == -2);
    PX_CHECK(pxPoint{}.IsEmpty());
    PX_CHECK(!pt.IsEmpty());

    PX_CHECK(pt + pxPoint(1, 1) == pxPoint(2.5, -1));
    PX_CHECK(pt - pxPoint(1, 1) == pxPoint(0.5, -3));
    PX_CHECK(pt + pxSize(2, 3) == pxPoint(3.5, 1));
    PX_CHECK(pt - pxSize(2, 3) == pxPoint(-0.5, -5));
    PX_CHECK(pt * 2 == pxPoint(3, -4));
    PX_CHECK(2 * pt == pxPoint(3, -4));
    PX_CHECK(pt / 2 == pxPoint(0.75, -1));
    PX_CHECK(+pt == pt);
    PX_CHECK(-pt == pxPoint(-1.5, 2));

    // A point and a size hold the same data; the conversion is explicit both ways.
    PX_CHECK(static_cast<pxSize>(pt) == pxSize(1.5, -2));
    PX_CHECK(pxPoint(pxSize{ 3, 4 }) == pxPoint(3, 4));
    static_assert(!std::is_convertible_v<pxPoint, pxSize> && !std::is_convertible_v<pxSize, pxPoint>);

    // Between precisions: implicit when nothing is lost, explicit (truncating) otherwise.
    pxPoint fromFloat = pxPointf{ 1.5f, 2.5f };
    pxPoint fromInt = pxPointi{ 3, 4 };
    PX_CHECK(fromFloat == pxPoint(1.5, 2.5));
    PX_CHECK(fromInt == pxPoint(3, 4));
    PX_CHECK(pxPointi(pxPoint{ 2.9, -2.9 }) == pxPointi(2, -2));
    static_assert(std::is_convertible_v<pxPointf, pxPoint> && std::is_convertible_v<pxPointi, pxPoint>);
    static_assert(!std::is_convertible_v<pxPoint, pxPointf> && !std::is_convertible_v<pxPoint, pxPointi>);
    static_assert(!std::is_convertible_v<pxPointi, pxPointf> && !std::is_convertible_v<pxPointf, pxPointi>);

    // Integer points divide like integers.
    PX_CHECK(pxPointi(7, -7) / 2 == pxPointi(3, -3));

    // Integer points wrap around, as in C# (5.2).
    pxPointi big{ INT32_MAX, INT32_MIN };
    PX_CHECK(big + pxPointi(1, -1) == pxPointi(INT32_MIN, INT32_MAX));
    PX_CHECK(big - pxPointi(-1, 1) == pxPointi(INT32_MIN, INT32_MAX));
    PX_CHECK(big * 2 == pxPointi(-2, 0));
    PX_CHECK(-big == pxPointi(-INT32_MAX, INT32_MIN));
    PX_CHECK(big + pxSizei(1, 0) == pxPointi(INT32_MIN, INT32_MIN));

    // To int32_t out of range: saturated, and NaN is 0 (5.3).
    double nan = std::numeric_limits<double>::quiet_NaN();
    PX_CHECK(pxPointi(pxPoint{ 3e9, -3e9 }) == pxPointi(INT32_MAX, INT32_MIN));
    PX_CHECK(pxPointi(pxPoint{ nan, -2147483648.9 }) == pxPointi(0, INT32_MIN));
    PX_CHECK(pxPointi(pxPointf{ 1e10f, -0.5f }) == pxPointi(INT32_MAX, 0));

    // Only the scalars C# converts implicitly (5.1): pxPointi * 0.5 does not compile.
    static_assert(!Multipliable<pxPointi, double> && !Multipliable<pxPointf, double>);
    static_assert(Multipliable<pxPoint, int> && Multipliable<pxPoint, float> && Multipliable<pxPointf, int>);
    static_assert(Multipliable<pxPointi, int> && Multipliable<pxPointi, short> && !Multipliable<pxPointi, unsigned>);
}
