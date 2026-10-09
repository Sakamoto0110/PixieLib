add_executable(pixie_tests
    main.cpp
    pxSize.test.cpp
    pxPoint.test.cpp
    pxRect.test.cpp
    pxRegion.test.cpp
    pxPadding.test.cpp
    pxColorRgba.test.cpp
    pxColorHsl.test.cpp
    pxToString.test.cpp
    pxCallback.test.cpp
    pxEventHandler.test.cpp
    pxSubscription.test.cpp
    pxEvent.test.cpp
)
target_link_libraries(pixie_tests PRIVATE pixie::pixie pixie::events)

if(PIXIE_MATH)
    target_sources(pixie_tests PRIVATE pxVec.test.cpp pxMat.test.cpp)
    target_link_libraries(pixie_tests PRIVATE pixie::math)
    target_compile_definitions(pixie_tests PRIVATE PIXIE_TEST_MATH)
endif()

if(MSVC)
    target_compile_options(pixie_tests PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(pixie_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Werror)
endif()

add_test(NAME pixie_tests COMMAND pixie_tests)
