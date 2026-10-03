#include <initializer_list>
#include <type_traits>

#include <pixie/pxRegion.hpp>

#include "check.hpp"

void TestRegion() {
    pxRegion rg{ 1, 2, 4, 6 };
    PX_CHECK(rg.Width() == 3 && rg.Height() == 4);
    PX_CHECK(pxRegion{}.IsEmpty());
    PX_CHECK(!rg.IsEmpty());

    // The same thing as a rectangle, converted explicitly both ways.
    PX_CHECK(pxRegion(pxRect{ 1, 2, 3, 4 }) == rg);
    PX_CHECK(static_cast<pxRect>(rg) == pxRect(1, 2, 3, 4));
    static_assert(!std::is_convertible_v<pxRect, pxRegion> && !std::is_convertible_v<pxRegion, pxRect>);

    // The same edges as pxRect_t::Contains.
    pxRect r{ 1, 2, 3, 4 };
    for (pxPoint pt : { pxPoint{ 1, 2 }, pxPoint{ 3.999, 5.999 }, pxPoint{ 4, 2 }, pxPoint{ 1, 6 }, pxPoint{ 0.5, 3 } })
        PX_CHECK(rg.Contains(pt) == r.Contains(pt));
    PX_CHECK(rg.Contains({ 1, 2 }) && !rg.Contains({ 4, 2 }) && !rg.Contains({ 1, 6 }));

    pxRegion fromInt = pxRegioni{ 0, 0, 2, 2 };
    PX_CHECK(fromInt == pxRegion(0, 0, 2, 2));
    static_assert(std::is_convertible_v<pxRegioni, pxRegion> && !std::is_convertible_v<pxRegion, pxRegioni>);
}
