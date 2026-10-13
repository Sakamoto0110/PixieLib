#pragma once

#include <pixie/events/pxEventHandler.hpp>

// A second module for the events tests: a shared library that hides its symbols, as a DLL does on
// Windows, so it has its own copy of every template and inline variable of the events, each type's
// tag included (pxEventsAcrossModules.test.cpp).
#if defined(_WIN32)
#ifdef PX_OTHER_MODULE_BUILD
#define PX_OTHER_MODULE_API __declspec(dllexport)
#else
#define PX_OTHER_MODULE_API __declspec(dllimport)
#endif
#else
#define PX_OTHER_MODULE_API __attribute__((visibility("default")))
#endif

PX_OTHER_MODULE_API bool ForwardInOtherModule(pxEventHandler<int>& source, pxEventHandler<int>& target);
PX_OTHER_MODULE_API void MoveInOtherModule(pxEventHandler<int>& from, pxEventHandler<int>& to);
