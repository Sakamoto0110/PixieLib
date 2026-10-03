#include <type_traits>

#include <pixie/pxPoint.hpp>

#include "check.hpp"

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
}
