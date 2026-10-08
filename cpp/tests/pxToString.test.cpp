#include <limits>

#include <pixie/pxColorHsl.hpp>
#include <pixie/pxColorRgba.hpp>
#include <pixie/pxPadding.hpp>
#include <pixie/pxPoint.hpp>
#include <pixie/pxRect.hpp>
#include <pixie/pxRegion.hpp>
#include <pixie/pxSize.hpp>

#include "check.hpp"

void TestToString() {
    // The fields in order, in parentheses, separated by ", ".
    PX_CHECK(pxPoint(1.5, -2).ToString() == "(1.5, -2)");
    PX_CHECK(pxSize(3, 4.25).ToString() == "(3, 4.25)");
    PX_CHECK(pxRect(1, 2, 3, 4).ToString() == "(1, 2, 3, 4)");
    PX_CHECK(pxRegion(1, 2, 4, 6).ToString() == "(1, 2, 4, 6)");
    PX_CHECK(pxPadding(1, 2, 3, 4).ToString() == "(1, 2, 3, 4)");
    PX_CHECK(pxColorRgba(255, 0, 128).ToString() == "(255, 0, 128, 255)");
    PX_CHECK(pxColorHsl(120, 1, 0.5).ToString() == "(120, 1, 0.5, 255)");

    // Integers and bytes as numbers, never as characters.
    PX_CHECK(pxPointi(-3, 7).ToString() == "(-3, 7)");
    PX_CHECK(pxColorRgba(65, 66, 67, 0).ToString() == "(65, 66, 67, 0)");

    // The shortest form that reads back as the same value, in each precision: 0.1f is 0.1, not
    // 0.100000001490116; and never in scientific notation.
    PX_CHECK(pxPointf(0.1f, 2.5f).ToString() == "(0.1, 2.5)");
    PX_CHECK(pxPoint(0.1, 1.0 / 3).ToString() == "(0.1, 0.3333333333333333)");
    PX_CHECK(pxPoint(100000, 0.0000001).ToString() == "(100000, 0.0000001)");
    PX_CHECK(pxPoint(-0.0, 1e15).ToString() == "(-0, 1000000000000000)");

    // The longest double there is still fits.
    double tiny = std::numeric_limits<double>::denorm_min();
    PX_CHECK(pxPoint(tiny, -std::numeric_limits<double>::max()).ToString().size() == 1 + 326 + 2 + 310 + 1);
}
