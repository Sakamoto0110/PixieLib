#include <cstdio>

#include "check.hpp"

void TestSize();
void TestPoint();
void TestRect();
void TestRegion();
void TestPadding();
void TestColorRgba();
void TestColorHsl();
void TestToString();
void TestCallback();
void TestEventHandler();
void TestSubscription();
void TestEvent();

int main() {
    TestSize();
    TestPoint();
    TestRect();
    TestRegion();
    TestPadding();
    TestColorRgba();
    TestColorHsl();
    TestToString();
    TestCallback();
    TestEventHandler();
    TestSubscription();
    TestEvent();

    if (pxCheckFailures == 0)
        std::printf("all checks passed\n");
    else
        std::printf("%d check(s) failed\n", pxCheckFailures);
    return pxCheckFailures == 0 ? 0 : 1;
}
