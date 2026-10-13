#include <utility>

#include "pxEventsOtherModule.hpp"

bool ForwardInOtherModule(pxEventHandler<int>& source, pxEventHandler<int>& target) {
    return source.Forward(target);
}

void MoveInOtherModule(pxEventHandler<int>& from, pxEventHandler<int>& to) {
    to = std::move(from);
}
