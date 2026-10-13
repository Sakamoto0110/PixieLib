#include <utility>

#include <pixie/events/pxEventHandler.hpp>

#include "check.hpp"
#include "pxEventsOtherModule.hpp"

// A handler linked in one module and copied or moved in another, as between an engine and a DLL of
// its own: each module has its own tag per type, so the link cannot be told apart by its type.
void TestAcrossModules() {
    int hits = 0;
    auto count = [&hits](int value) { hits += value; };

    // Linked there, then checked, copied and moved here.
    {
        pxEventHandler<int> source, target;
        target += count;
        PX_CHECK(ForwardInOtherModule(source, target));
        PX_CHECK(!target.Forward(source));
        pxEventHandler<int> copy = source;
        PX_CHECK(copy.IsEmpty());
        pxEventHandler<int> moved = std::move(target);
        source(1);
        PX_CHECK(hits == 1);
        PX_CHECK(source.Unforward(moved) && source.IsEmpty());
    }

    // Linked here, then moved there.
    {
        pxEventHandler<int> source, target, other;
        target += count;
        PX_CHECK(source.Forward(target));
        MoveInOtherModule(target, other);
        source(2);
        PX_CHECK(hits == 3);
        PX_CHECK(source.Unforward(other) && source.IsEmpty());
    }
}
