#pragma once

#include "pxEvents.hpp"





















#include "pxCorelib.h"
struct MouseEventArgs : public EventArgs {
    virtual const char* GetTypename() { return "MouseEventArgs"; }
    //MOUSE_BUTTON_LEFT   0x0
    //MOUSE_BUTTON_RIGHT  0x1
    //MOUSE_BUTTON_MIDDLE 0x2
    int m_button;
    pxPointf m_location;
    pxPointf m_delta;
    // not implemented
    int m_wDelta;

};

using MouseEventHandler = EventHandler_t<MouseEventArgs>;






#define COMPILE_EVENT_TESTS
#ifdef COMPILE_EVENT_TESTS
#include <conio.h>
#include "pxTests.hpp"

namespace EventTests {
    using namespace pxTests;
    


    template<typename TEventArgs>
    class EventHandlerTester :public MemberFunctionInvoker<EventHandler_t<void*, TEventArgs>> {
    public:
        using EventHandler_t = EventHandler_t<TEventArgs>;
        // using MemberFunctionInvoker<EventHandler_t<void*, TEventArgs>>::sig_v_crT;

        template<typename...Tx >
        static void Add_fn(EventHandler_t& e, Tx... obj) {

            // EventHandlerTester::template _call_any_fn_signature_<sig_v_crT,&Add>(e, obj...);
        }


        template<typename...Tx >
        static void Rem_fn(EventHandler_t& e, Tx... obj) {
            // Any_fn<&EventHandler_t::remove>(e, obj...);
            // Any_fn < &EventHandler_t::operator-=>(e, obj...);
        }

    };

    void UserTest();


    struct TestEventArgs : EventArgs {
        int some_data1;
        int some_data2;
        TestEventArgs() :some_data1(0), some_data2(1) {}
        TestEventArgs(int a, int b) :some_data1(a), some_data2(b) {}
        const char* GetTypename() override { return "TestEventArgs"; }

    };

    inline void fn1(void* s, TestEventArgs* e) {
        std::cout << '[' << (int)&fn1 << "] ";
        printf("fn1 called\n");
    }
    inline void fn2(void* s, TestEventArgs* e) {
        std::cout << '[' << (int)&fn2 << "] ";
        printf("fn2 called\n");
    }
    inline void fn3(void* s, TestEventArgs* e) {
        std::cout << '[' << (int)&fn3 << "] ";
        printf("fn3 called\n");
    }
    inline void fn4(void* s, TestEventArgs* e) {
        std::cout << '[' << (int)&fn4 << "] ";
        printf("fn4 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
    }
    inline void fn5(void* s, TestEventArgs* e) {
        std::cout << '[' << (int)&fn5 << "] ";
        printf("fn5 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
    }

    using TestEventHandler = EventHandler_t<void*, TestEventArgs*>;

    class Foo {
    public:
        int x;
        int y;
        TestEventHandler e;
        Foo() :x(0), y(0) {}
        Foo(int _x, int _y) :x(_x), y(_y) {}

        void func1(void* s, TestEventArgs* e) {
            std::cout << '[' << (&Foo::func1) << "] ";
            printf("Foo::func1 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
        }
        void func2(void* s, TestEventArgs* e) {
            std::cout << '[' << (&Foo::func2) << "] ";
            printf("Foo::func2 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
        }
    };




}



#endif // COMPILE_EVENT_TESTS



