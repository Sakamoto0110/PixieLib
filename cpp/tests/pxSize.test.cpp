#include <type_traits>

#include <pixie/pxSize.hpp>

#include "check.hpp"

void TestSize() {
    pxSize sz{ 3, 4.5 };
    PX_CHECK(sz.width == 3 && sz.height == 4.5);
    PX_CHECK(pxSize{}.IsEmpty());
    PX_CHECK(!sz.IsEmpty());

    PX_CHECK(sz + pxSize(1, 1) == pxSize(4, 5.5));
    PX_CHECK(sz - pxSize(1, 1) == pxSize(2, 3.5));
    PX_CHECK(sz * 2 == pxSize(6, 9));
    PX_CHECK(2 * sz == pxSize(6, 9));
    PX_CHECK(sz / 2 == pxSize(1.5, 2.25));
    PX_CHECK(+sz == sz);
    PX_CHECK(-sz == pxSize(-3, -4.5));

    // Between precisions: implicit when nothing is lost, explicit (truncating) otherwise.
    pxSize fromFloat = pxSizef{ 1.5f, 2.5f };
    pxSize fromInt = pxSizei{ 3, 4 };
    PX_CHECK(fromFloat == pxSize(1.5, 2.5));
    PX_CHECK(fromInt == pxSize(3, 4));
    PX_CHECK(pxSizei(pxSize{ 2.9, -2.9 }) == pxSizei(2, -2));
    static_assert(std::is_convertible_v<pxSizef, pxSize> && std::is_convertible_v<pxSizei, pxSize>);
    static_assert(!std::is_convertible_v<pxSize, pxSizef> && !std::is_convertible_v<pxSize, pxSizei>);
    static_assert(!std::is_convertible_v<pxSizei, pxSizef> && !std::is_convertible_v<pxSizef, pxSizei>);
    static_assert(std::is_constructible_v<pxSizef, pxSize> && std::is_constructible_v<pxSizei, pxSize>);
}
