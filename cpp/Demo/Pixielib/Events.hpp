#pragma once

#include <stdio.h>


#ifndef USE_STL
#ifndef NOT_USE_STL
#define USE_STL            // DEFAULT 
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

class EventArgs {
public:
    EventArgs() {}

    virtual const char* GetTypename() { return "EventArgs"; }

};


typedef void(*f)(void*, EventArgs*);
typedef class IEventCallback {
public:
    virtual void operator ()(void*, EventArgs*) = 0;
    virtual bool operator ==(IEventCallback*) = 0;

    virtual int Dump() = 0;

};



template< class _T = DUMMY_TYPE, typename K = EventArgs>
class EventCallback : public IEventCallback {
private:
    enum CallbackType : int {
        MemberCallback = 0,
        StaticCallback = 1
    };

    using MemberCallback_t = void(_T::*)(void*, K*);
    using StaticCallback_t = void(*)(void*, K*);

public:
    struct {
        _T* hInstance;
        union {
            MemberCallback_t m_ptmf;
            StaticCallback_t s_ptf;
        };
    };

private:
    CallbackType GetCallbackType() { return (CallbackType)(hInstance == 0); }
    
public:
    EventCallback(void(*_callback)(void*, K*)) : hInstance(nullptr), s_ptf(_callback) { }
    EventCallback(_T* _instance, void(_T::* _callback)(void*, K*)) : hInstance(_instance), m_ptmf(_callback) { }

public:
    void operator ()(void* _sender, EventArgs* _args) override {
        switch (GetCallbackType()) {
        case StaticCallback: return (*s_ptf)(_sender, (K*)_args);
        case MemberCallback: return (hInstance->*m_ptmf)(_sender, (K*)_args);
        }
    }



    bool operator ==(IEventCallback* other) override {
        EventCallback* otherEventCallback = (EventCallback<_T,K>*)other;
        if (otherEventCallback == nullptr) return false;

        if (GetCallbackType() == StaticCallback && otherEventCallback->GetCallbackType() == StaticCallback)
            return this->s_ptf == otherEventCallback->s_ptf;
        else if (GetCallbackType() == MemberCallback && otherEventCallback->GetCallbackType() == MemberCallback)
            return this->m_ptmf == otherEventCallback->m_ptmf && this->hInstance == otherEventCallback->hInstance;
        else return false;

    }

    int Dump() override {
        switch (GetCallbackType()) {
        case StaticCallback: return (int)(&s_ptf);
        case MemberCallback: return (int)(&m_ptmf);
        }
    }

};


// Used only if USE_STL is NOT defined
constexpr int MAX_CALLBACK_COUNT = 256;


template<class TEventArgs>
class EventHandler {
   
public:
    EventHandler() { }
    

    EventHandler(void(*_callback)(void*, TEventArgs*)) {
        this->Add(new EventCallback(_callback));
    }

    template<typename T>
    EventHandler(T* _instance, void(T::* _callback)(void*, TEventArgs*)) {
        this->Add(new EventCallback(_instance, _callback));
    }

    // Ctor with default callback
    EventHandler(IEventCallback* _callback) { 
        if (_callback != 0) {
            Add(_callback);
        }
    }

public:
    void Add(IEventCallback*);
    void Add(const EventHandler& e) {
        for (IEventCallback* _callback : e.m_callbacks) {
            this->Add(_callback);
        }
    }
    void Remove(IEventCallback*);
    void Remove(const EventHandler& e) {
        for (IEventCallback* _callback : e.m_callbacks) {
            this->Remove(_callback);
        }
    }
    void Invoke(void*, TEventArgs*);

public:
    int GetSize() const;
    void Dump();

public:
    const EventHandler& operator=(const EventHandler& other) {
        Release();
        for (IEventCallback* cb : other.m_callbacks) {
            this->Add(cb);
        }
        return *this;
    }
    const EventHandler& operator +=(const EventHandler& e) {
        this->Add(e);
        return *this;
    }

    EventHandler& operator -=(const EventHandler& e) {
        Remove(e);
        return *this;
    }
    
    
private:
    void Release() {
        for (IEventCallback* cb : m_callbacks) {
            delete(cb);
        }
        m_callbacks.resize(0);
    }
    void Trim(int startIndex);


#ifdef USING_STL
    std::vector<IEventCallback*> m_callbacks = std::vector<IEventCallback*>();
#endif // USING_STL
#ifdef NOT_USING_STL
    int m_count = 0;
    IEventCallback** m_callbacks = new IEventCallback * [MAX_CALLBACK_COUNT];
#endif // NOT_USING_STL


};

template<class TEventArgs>
inline int EventHandler<TEventArgs>::GetSize() const {
#ifdef USING_STL
    return m_callbacks.size();
#endif // USING_STL
#ifdef NOT_USING_STL
    return m_count;
#endif // NOT_USING_STL
}

template<class TEventArgs>
inline void EventHandler<TEventArgs>::Add(IEventCallback* _newCallback) {
#ifdef NOT_USING_STL
    if (m_count >= MAX_CALLBACK_COUNT)  // Max size reached.        
        return;
#endif // NOT_USING_STL


    for (int i = 0; i < this->GetSize(); i++) {
        if (m_callbacks[i] != 0 && (*m_callbacks[i]== _newCallback)) { // Callback already exists            
            printf("Unable to add %#X callback, it already exists (%#X)\n", _newCallback->Dump(), m_callbacks[i]->Dump());
            delete(_newCallback);
            return;
        }
    }
    // Matching callback not found
#ifdef USING_STL
    m_callbacks.push_back(_newCallback);
#endif // USING_STL
#ifdef NOT_USING_STL
    m_callbacks[m_count++] = _newCallback;
#endif // NOT_USING_STL

 //   printf("Added %#X callback.\n", _newCallback->Dump());

}
template<class TEventArgs>
inline void EventHandler<TEventArgs>::Remove(IEventCallback* _callback) {
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

}
template<class TEventArgs>
inline void EventHandler<TEventArgs>::Trim(int startIndex) {
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

template<class TEventArgs>
inline void EventHandler<TEventArgs>::Invoke(void* _sender, TEventArgs* _args){
    bool need_to_delete_args = false;
    if (_args == 0) {
        _args = new EventArgs();
        need_to_delete_args = true;
    }
    for (int i = 0; i < this->GetSize(); i++) {
        if (m_callbacks[i] != 0) {
            (*m_callbacks[i])(_sender, _args);
        }
    }
    if (need_to_delete_args) {
        delete(_args);
    }
}

template<class TEventArgs>
inline void EventHandler<TEventArgs>::Dump() {
    printf("Starting to dump contents of EventHandler:\n");
    int actual_size = this->GetSize();
    for (int i = 0; i < actual_size; i++) {
        int callback_dump = 0;
        if (m_callbacks[i] != 0) {
            callback_dump = m_callbacks[i]->Dump();
        }
        printf("0x%.8X", callback_dump);
        if (i < actual_size - 1)
            printf(",");
        printf("\n");

    }
    printf("Dump finished.\n");
}

#ifdef COMPILE_EVENT_TESTS

class __FooChangedEventArgs : public EventArgs {
public:
    char propertyName;
    int oldVal;
    int newVal;

};

class __Foo {
public:
    EventHandler Foo_Changed = EventHandler();
    __Foo(int _x, int _y) : x(_x), y(_y) {
        
//        Foo_Changed.Add(new )
        Foo_Changed.Add(new EventCallback(this, &__Foo::OnFooChanged2));
        
      
    }

    void OnFooChanged(void* sender, __FooChangedEventArgs* args) {
        printf("Foo changed called from inside foo\n");
    }
    void OnFooChanged2(void* sender, __FooChangedEventArgs* args) {
        printf("Foo changed 2 called from inside foo\n");
    }
    void SetX(int val) {
        __FooChangedEventArgs args = __FooChangedEventArgs();
        args.oldVal = x;
        args.propertyName = 'x';
        args.newVal = val;
        x = val;
        Foo_Changed.Invoke(&args);
    }
public:
    int x = 90;
protected:
    int y = 50823;
private:
    int z = 41;
};

inline void FooEvent(void*, __FooChangedEventArgs* args) {
    printf("Foo event called. [%c, %i, %i] \n", args->propertyName, args->oldVal, args->newVal);
}
inline void DummyEvent(void*, EventArgs* args) {
    printf("dummy event called. \n");
}
inline void DummyEvent2(void*, EventArgs* args) {
    printf("dummy2 event called. \n");
}
inline void DummyEvent3(void*, EventArgs* args) {
    printf("dummy3 event called. \n");
}

inline void __RUN_EVENT_TEST() {
    __Foo foo = __Foo(13, 52);

    // Add event using method Add with callback object
    // Triggers the event
    foo.SetX(5416234);
    foo.Foo_Changed += &FooEvent;
    foo.Foo_Changed += &DummyEvent;
    foo.Foo_Changed += &DummyEvent2;
    foo.Foo_Changed += &DummyEvent3;

    foo.Foo_Changed -= &FooEvent;
    foo.Foo_Changed -= &DummyEvent;
    foo.Foo_Changed -= &DummyEvent2;
    // Dumps the contents of the event
    foo.Foo_Changed.Dump();

}



#endif // COMPILE_EVENT_TESTS