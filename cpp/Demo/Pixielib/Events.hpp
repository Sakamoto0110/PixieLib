#pragma once

#include <stdio.h>

#define BACKTRACE 

#ifndef USE_STL
#ifndef NOT_USE_STL
#define USE_STL // DEFAULT 
#endif // !NOT_USING_STL
#endif // !USING_STL


#ifdef USE_STL
#define USING_STL
#include <vector>
#undef NOT_USING_STL    
#endif // USE_STL

#ifdef NOT_USE_STL
#define NOT_USING_STL
#undef USING_STL    
#endif // NOT_USE_STL

class DUMMY_TYPE { /*  EMPTY CLASS  */ };


struct EventArgs {    
    virtual const char* GetTypename() { return "EventArgs"; }
};



namespace pxLogger {
    template<typename...Tx>
	void log(const char* str,Tx...format_args) {
#ifdef BACKTRACE
        printf(str, format_args...);
#endif

        
		
	}
    
    static int __depth = 0;
}

#define pxLog(str,...) pxLogger::log(str,__VA_ARGS__)
#define pxTraceStart  pxLog("%*s \n",(((pxLogger::__depth++)*4)+strlen(__FUNCTION__)),__FUNCTION__)
#define pxTraceEnd --pxLogger::__depth

template<typename...Tx>
class IEventCallback {
public:
	virtual ~IEventCallback() = default;

	virtual void operator ()(Tx...) = 0;
    virtual operator unsigned int() = 0;


    bool operator ==(IEventCallback* e) { return (int)*this == (int)*e; }
    
 
};

// Used only if USE_STL is NOT defined
constexpr int MAX_CALLBACK_COUNT = 256;



template<typename... TArgs> class EventHandler;


 template<typename...TArgs>
 class _stcall_t : public IEventCallback<TArgs...> {
 public:
     using _stCallback = void(*)(TArgs...);
 private:
     _stCallback _fn;
 public:
     _stcall_t(_stCallback& f) : _fn(f) {
         pxTraceStart;
         pxTraceEnd;
     }
     operator unsigned int() override { return (unsigned int)_fn; }
     void operator()(TArgs...e) override { (*_fn)(e...); }

 };
 template<typename TInstance, typename...TArgs>
 class _mbcall_t : public IEventCallback<TArgs...> {
 public:
     using _mbCallback = void(TInstance::*)(TArgs...);
 private:
     TInstance* _inst;
     _mbCallback _fn;
 public:
     _mbcall_t(TInstance* inst, _mbCallback& f) : _inst(inst), _fn(f) {
         pxTraceStart;
         pxTraceEnd;
     }
     operator unsigned int() override { return (unsigned int)&_fn; }
     void operator()(TArgs...e) override { (_inst->*_fn)(e...); }

 };
 template<typename TCallable,typename...TArgs>
 class _lbcall_t : public IEventCallback<TArgs...> {
     TCallable* _fn;
 public:
     _lbcall_t(TCallable* f) : _fn(f) {
         pxTraceStart;
         pxTraceEnd;
     }
     _lbcall_t(TCallable& f) : _fn(&f) {
         pxTraceStart;
         pxTraceEnd;
     }
     operator unsigned int() override { return (unsigned int)&_fn; }
     void operator()(TArgs...e) override { (*_fn)(e...); }

 };
 template<typename...TArgs>
 class _chcall_t : public IEventCallback<TArgs...> {
     EventHandler<TArgs...>* _e;
 public:
     _chcall_t(EventHandler<TArgs...>* _handler) : _e(_handler) { }
     operator unsigned int() override { return (unsigned int)_e; }
     void operator()(TArgs...e) override { (*_e).Invoke(e...); }
 };

template<typename... TArgs>
class Callback : public IEventCallback<TArgs...>{
public:
    
    /*class _stcall_t : public Callback::template IEventCallback<TArgs...> {
    public:
        using _stCallback = void(*)(TArgs...);
    private:
        _stCallback _fn;
    public:
        _stcall_t(_stCallback& f) : _fn(f) {
            pxTraceStart;
            pxTraceEnd;
        }
        operator unsigned int() override { return (unsigned int)_fn; }
        void operator()(TArgs...e) override { (*_fn)(e...); }

    };
    template<typename TInstance>
    class _mbcall_t : public Callback::template IEventCallback<TArgs...> {
    public:
        using _mbCallback = void(TInstance::*)(TArgs...);
    private:
        TInstance* _inst;
        _mbCallback _fn;
    public:
        _mbcall_t(TInstance* inst, _mbCallback& f) : _inst(inst), _fn(f) {
            pxTraceStart;
            pxTraceEnd;
        }
        operator unsigned int() override { return (unsigned int)&_fn; }
        void operator()(TArgs...e) override { (_inst->*_fn)(e...); }

    };
    template<typename TCallable>
    class _lbcall_t : public Callback::template IEventCallback<TArgs...> {
        TCallable* _fn;
    public:
        _lbcall_t(TCallable* f) : _fn(f) {
            pxTraceStart;
            pxTraceEnd;
        }
        _lbcall_t(TCallable& f) : _fn(&f) {
            pxTraceStart;
            pxTraceEnd;
        }
        operator unsigned int() override { return (unsigned int)&_fn; }
        void operator()(TArgs...e) override { (*_fn)(e...); }

    };
    
    class _chcall_t : public Callback::template IEventCallback<TArgs...> {
        EventHandler<TArgs...>* _e;
    	public:
        _chcall_t(EventHandler<TArgs...>* _handler) : _e(_handler) { }
        operator unsigned int() override { return (unsigned int)_e; }
        void operator()(TArgs...e) override { (*_e).Invoke(e...); }
    };*/






public:
	using CBINFO = int;
    using HCALLBACK = IEventCallback<TArgs...>*;
    using _stCallback = typename _stcall_t<TArgs...>::_stCallback;
    template<typename _TInstance> using _mbCallback = typename _mbcall_t < _TInstance, TArgs... > ::_mbCallback;
    static constexpr CBINFO STCALLBACK = 0;
    static constexpr CBINFO MBCALLBACK = 1;
    static constexpr CBINFO LBCALLBACK = 2;
    static constexpr CBINFO CHCALLBACK = 3;

 //   Callback(_stCallback _call) : _tag(STCALLBACK), _ptr{ .stcall = new _stcall_t(_call) } {
 //       pxTraceStart;
 //       pxTraceEnd;
 //   }
	//template<typename _TInstance>
 //   Callback(_TInstance* _inst, _mbCallback< _TInstance> _call) : _tag(MBCALLBACK), _ptr{.mbcall = new _mbcall_t(_inst, _call) } {
 //    	pxTraceStart;
 //       pxTraceEnd;//
 //   }
 //    template<typename _TCallable>
 //    Callback(_TCallable _call) : _tag(LBCALLBACK), _ptr{ .lbcall = (HCALLBACK)(new _lbcall_t<_TCallable,TArgs...>(_call)) } {
 //        
 //    	pxTraceStart;
 //        pxTraceEnd;
 //    }
 //   
 //    Callback(EventHandler<TArgs...>* _handler) : _tag(CHCALLBACK), _ptr{ .chcall = new _chcall_t(_handler) } {
 //        pxTraceStart;
 //        pxTraceEnd;
 //    }

    Callback(_stCallback _call) : _tag(STCALLBACK), ptr(new _stcall_t(_call)){
        pxTraceStart;
        pxTraceEnd;
    }
    template<typename _TInstance>
    Callback(_TInstance* _inst, _mbCallback< _TInstance> _call) : _tag(MBCALLBACK), ptr(new _mbcall_t(_inst, _call)){
        pxTraceStart;
        pxTraceEnd;
    }
    template<typename _TCallable>
    Callback(_TCallable _call) : _tag(LBCALLBACK) , ptr(new _lbcall_t<_TCallable, TArgs...>(_call)) {

        pxTraceStart;
        pxTraceEnd;
    }

    Callback(EventHandler<TArgs...>* _handler) : _tag(CHCALLBACK), ptr(new _chcall_t(_handler)) {
        pxTraceStart;
        pxTraceEnd;
    }

    void operator()(TArgs...e) override { (*ptr)(e...); }
    operator unsigned int() override { return (unsigned int)*ptr; }
    CBINFO _tag;
private:
    HCALLBACK ptr;
    union {
        HCALLBACK stcall;
        HCALLBACK mbcall;
        HCALLBACK lbcall;
        HCALLBACK chcall;
    }_ptr;
    explicit operator HCALLBACK() {
        pxTraceStart;
        pxTraceEnd;
    	return ptr;
        
    }

public:
	~Callback() override {
        pxTraceStart;
        delete(ptr);
        pxTraceEnd;
        return;
        
	}
};


/**
 * \class EventHandler
 * \brief The EventHandler class responsible to store events and fire then whenever needed. \n
 * The EventHandler will alocate a pointer to a Callback "proxy like" instance that holds the necessary data to fire an event.
 * \tparam TArgs Zero or more argument types that will be needed to fire an event.
 * 
 */
template<typename... TArgs>
class EventHandler {
protected:
   
public:
   
    //using ptCallback = typename Callback<TArgs...>::HCALLBACK;
    using ptCallback = Callback<TArgs...>*;

    EventHandler() = default;

    /**
     * \brief Constructor for COPYING the handlers into this
     * \param e EventHandler that will be copied
     */
    EventHandler(const EventHandler& e);

    /**
     * \brief Constructor for \u bind another EventHandler into this \n
     * bound EventHandlers may be used for chained events
     * \param e pointer to a different EventHandler
     */
    EventHandler(EventHandler* e);

    template<typename T, typename...Tx>
    EventHandler(const T& c, Tx...args);

public:
	void Add(const EventHandler& e);
	void Remove(const EventHandler& e);
	void Invoke(TArgs...);

public:
    size_t GetSize() const;
    void Dump(); 
	
public:
    EventHandler& operator  =(const EventHandler& e) {
        Release();
        pxTraceStart;
        for (int i = 0; i < e.GetSize(); i++) {
            this->Add(e.m_callbacks[i]);
        }
        pxTraceEnd;
        return *this;
    }

    void operator +=(const EventHandler& e) {
        pxTraceStart;
    	this->Add(e);
        pxTraceEnd;
    }

    void operator -=(const EventHandler& e) {
        pxTraceStart;
    	Remove(e);
        pxTraceEnd;
    }    
    
private:
    void Release();
    void Trim(int startIndex);
    void Add(ptCallback);
    void Remove(ptCallback);

#ifdef USING_STL
    std::vector<ptCallback> m_callbacks = std::vector<ptCallback>();
#endif // USING_STL
#ifdef NOT_USING_STL
    int m_count = 0;
    IEventCallback** m_callbacks = new IEventCallback * [MAX_CALLBACK_COUNT];
#endif // NOT_USING_STL


};
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #



template<typename...TArgs>
EventHandler<TArgs...>::EventHandler(const EventHandler& e) {
    *this = e;
}

template<typename...TArgs>
EventHandler<TArgs...>::EventHandler(EventHandler* e) : m_callbacks({ new Callback<TArgs...>(e) }) { pxTraceStart; pxTraceEnd; }

template<typename...TArgs> template<typename T, typename...Tx>
EventHandler<TArgs...>::EventHandler(const T& c, Tx...args) : m_callbacks({ new Callback<TArgs...>(c,args...) }) { pxTraceStart; pxTraceEnd; }



// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 



template<typename...TArgs>
inline void EventHandler<TArgs...>::Add(const EventHandler& e) {
    pxTraceStart;
    for (size_t i = 0; i < e.GetSize(); i++) {
        this->Add(e.m_callbacks[i]);
    }

    pxTraceEnd;
}

template<typename...TArgs>
inline void EventHandler< TArgs...>::Remove(const EventHandler& e) {
    pxTraceStart;

    for (size_t i = 0; i < e.GetSize(); i++) {
        this->Remove(e.m_callbacks[i]);
    }
    pxTraceEnd;
}

template<typename...TArgs>
inline void EventHandler<TArgs...>::Invoke(TArgs... _args) {
    pxTraceStart;

    for (size_t i = 0; i < this->GetSize(); i++) {
        if (m_callbacks[i] != 0) {
            (*m_callbacks[i])(_args...);
        }
    }

    pxTraceEnd;
}

template<typename...TArgs>
inline size_t EventHandler<TArgs...>::GetSize() const {
#ifdef USING_STL
    return m_callbacks.size();
#endif // USING_STL
#ifdef NOT_USING_STL
    return m_count;
#endif // NOT_USING_STL
}


template<typename...TArgs>
inline void EventHandler<TArgs...>::Dump() {
    printf("Starting to dump contents of EventHandler:\n");
    int actual_size = this->GetSize();
    for (int i = 0; i < actual_size; i++) {
        int callback_dump = 0;
        if (m_callbacks[i] != 0) {
            callback_dump = (int)m_callbacks[i];
        }
        printf("0x%.8X", callback_dump);
        if (i < actual_size - 1)
            printf(",");
        printf("\n");

    }
    printf("Dump finished.\n");
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 



template< typename...TArgs>
inline void EventHandler<TArgs...>::Add(ptCallback _newCallback) {
#ifdef NOT_USING_STL
    if (m_count >= MAX_CALLBACK_COUNT)  // Max size reached.        
        return;
#endif // NOT_USING_STL
    
    pxTraceStart;


    for (int i = 0; i < this->GetSize(); i++) {
        if (m_callbacks[i] != 0 && (*m_callbacks[i]== _newCallback)) { // Callback already exists            
            printf("Unable to add %#X callback, it already exists (%#X)\n", (int)_newCallback, (int)m_callbacks[i]);
            delete(_newCallback);
            pxTraceEnd;
            return;
        }
    }
    pxTraceEnd;
    // Matching callback not found
#ifdef USING_STL
    m_callbacks.push_back(_newCallback);
#endif // USING_STL
#ifdef NOT_USING_STL
    m_callbacks[m_count++] = _newCallback;
#endif // NOT_USING_STL

 //   printf("Added %#X callback.\n", _newCallback->Dump());

}

template<typename...TArgs>
inline void EventHandler<TArgs...>::Remove(ptCallback _callback) {
    pxTraceStart;
	int removed_index = -1;
    // Removes the element ( if find )
    for (int i = 0; i < this->GetSize(); i++) {
        if (*m_callbacks[i] == _callback) {
            delete(m_callbacks[i]);
            m_callbacks[i] = 0;
            removed_index = i;
           // printf("%#X Removed from the event list\n", _callback->Dump());
            Trim(i);
            break;
        }
    }
    delete(_callback);
    pxTraceEnd;
}

template<typename...TArgs>
inline void EventHandler<TArgs...>::Trim(int startIndex) {
#ifdef BACKTRACE
    printf("%s(int) called\n ", __FUNCTION__);
#endif // BACKTRACE
    if (startIndex < 0) {
        return;
    }

    int actual_size = this->GetSize();
    for (int i = startIndex; i < actual_size; i++) {
        if (m_callbacks[i] == 0)
            if (i < actual_size - 1)
                if (m_callbacks[i + 1] != 0) {
                    m_callbacks[i] = m_callbacks[i + 1];
                    m_callbacks[i + 1] = 0;
                }
    }

    // Shrinks counter or vector size
#ifdef USING_STL
    m_callbacks.resize(actual_size - 1);
#endif //USING_STL
#ifdef NOT_USING_STL
    m_count--;
#endif //NOT_USING_STL

}

template<typename...TArgs>
inline void EventHandler<TArgs...>::Release() {
    pxTraceStart;
    for (int i = 0; i < GetSize(); i++) {
        ptCallback cb = m_callbacks[i];
        
        delete(cb);
       
    }
    
#ifdef USING_STL
    m_callbacks.resize(0);
#endif // USING_STL
#ifdef NOT_USING_STL
    m_count = 0;
#endif // NOT_USING_STL
    pxTraceEnd;
}






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

using MouseEventHandler = EventHandler<MouseEventArgs>;






#define COMPILE_EVENT_TESTS
#ifdef COMPILE_EVENT_TESTS
#include <conio.h>
#include "pxTests.hpp"
using namespace pxTests;
namespace EventTests {
    struct TestEventArgs : EventArgs {
        int some_data1;
        int some_data2;
        TestEventArgs():some_data1(0),some_data2(1){}
        TestEventArgs(int a, int b):some_data1(a),some_data2(b){}
    	const char* GetTypename() override { return "TestEventArgs"; }

    };

    inline void fn1(void* s, TestEventArgs* e) {
        printf("fn1 called\n");
    }
    inline void fn2(void* s, TestEventArgs* e) {
        printf("fn2 called\n");
    }
    inline void fn3(void* s, TestEventArgs* e) {
        printf("fn3 called\n");
    }
    inline void fn4(void* s, TestEventArgs* e) {
        printf("fn4 called, %s: [%i, %i]\n",e->GetTypename(), e->some_data2,e->some_data1);
    }
    inline void fn5(void* s, TestEventArgs* e) {
        printf("fn5 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
    }

    using TestEventHandler = EventHandler<void*,TestEventArgs*>;

    class Foo {
    public:
        int x;
        int y;
        TestEventHandler e;
        Foo():x(0),y(0){}
        Foo(int _x, int _y):x(_x),y(_y){}

        void func1(void* s, TestEventArgs* e) {
            printf("Foo::func1 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
        }
        void func2(void* s, TestEventArgs* e) {
            printf("Foo::func2 called, %s: [%i, %i]\n", e->GetTypename(), e->some_data2, e->some_data1);
        }
    };


    template<typename TEventArgs>
    class EventHandlerTester :public MemberFunctionInvoker<EventHandler<void*, TEventArgs>> {
    public:
        using EventHandler_t = EventHandler<TEventArgs>;
        using MemberFunctionInvoker<EventHandler<void*, TEventArgs>>::sig_v_crT;

        template<typename...Tx >
        static void Add_fn(EventHandler_t& e, Tx... obj) {

            // EventHandlerTester::template _call_any_fn_signature_<sig_v_crT,&Add>(e, obj...);
        }


        template<typename...Tx >
        static void Rem_fn(EventHandler_t& e, Tx... obj) {
            Any_fn<&EventHandler_t::Remove>(e, obj...);
            Any_fn < &EventHandler_t::operator-=>(e, obj...);
        }

    };

    void UserTest();






}



#endif // COMPILE_EVENT_TESTS



