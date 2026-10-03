#include <type_traits>

#include <pixie/pxRect.hpp>

#include "check.hpp"

void TestRect() {
    pxRect r{ 1, 2, 3, 4 };
    PX_CHECK(r.Right() == 4 && r.Bottom() == 6);
    PX_CHECK(pxRect(pxPoint{ 1, 2 }, pxSize{ 3, 4 }) == r);
    PX_CHECK(pxRect{}.IsEmpty());
    PX_CHECK(!r.IsEmpty());

    // The left and top edges are inside, the right and bottom ones are not.
    PX_CHECK(r.Contains({ 1, 2 }));
    PX_CHECK(r.Contains({ 3.999, 5.999 }));
    PX_CHECK(!r.Contains({ 4, 2 }));
    PX_CHECK(!r.Contains({ 1, 6 }));
    PX_CHECK(!r.Contains({ 0.999, 2 }));
    PX_CHECK(!r.Contains({ 1, 1.999 }));

    pxRecti ri{ 0, 0, 2, 2 };
    PX_CHECK(ri.Contains({ 1, 1 }) && !ri.Contains({ 2, 1 }));

    pxRect fromInt = ri;
    PX_CHECK(fromInt == pxRect(0, 0, 2, 2));
    PX_CHECK(pxRecti(pxRect{ 1.9, 2.9, 3.9, 4.9 }) == pxRecti(1, 2, 3, 4));
    static_assert(std::is_convertible_v<pxRectf, pxRect> && !std::is_convertible_v<pxRect, pxRectf>);
}
