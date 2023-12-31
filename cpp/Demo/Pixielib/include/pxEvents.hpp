#pragma once

#include <stdio.h>

#define BACKTRACE1

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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                   *
 *                                                                                   *
 *                  IMPLEMENTATIONS AFTER ALL DECLARATIONS                           *
 *                                                                                   *
 *                                                                                   *
 *   1. Callback Objects ( Real implementation of IEventCallback interface )         *
 *      .1 - stCallback                                                              *
 *           .1 - Constructors                                                       *
 *           .2 - Interface implementation                                           *
 *      .2 - mbCallback                                                              *
 *           .1 - Constructors                                                       *
 *           .2 - Interface implementation                                           *
 *      .3 - lbCallback                                                              *
 *           .1 - Constructors                                                       *
 *           .2 - Interface implementation                                           *
 *      .4 - chCallback                                                              *
 *           .1 - Constructors                                                       *
 *           .2 - Interface implementation                                           *
 *                                                                                   *
 *                                                                                   *
 *   2. Callback ( Proxy to IEventCallback interface )                               *
 *      .1 - Constructors                                                            *
 *      .2 - Interface implementation                                                *
 *                                                                                   *
 *                                                                                   *
 *   3. EventHandler                                                                 *
 *      .1 - Type aliases                                                            *
 *      .2 - Public                                                                  *
 *           .1 - Constructors                                                       *
 *           .2 - Functions                                                          *
 *           .3 - Const functions                                                    *
 *           .4 - Operators                                                          *
 *      .3 - Private                                                                 *
 *           .1 - Functions                                                          *
 *                                                                                   *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                   *
  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




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

using uint = unsigned int;
using DWORD = unsigned long long;
using LPDWORD = DWORD*;

template<typename...Tx>
class IEventCallback {
public:
	virtual ~IEventCallback() = default;

    virtual IEventCallback* copy() { return 0; }
    explicit virtual operator DWORD() = 0;
    virtual void operator ()(Tx...) = 0;
    bool operator ==(IEventCallback* e) { return static_cast<DWORD>(*this) == static_cast<DWORD>(*e); }
    
 
};

// Used only if USE_STL is NOT defined
constexpr int MAX_CALLBACK_COUNT = 256;





template<typename...Tx>             class stCallback_t;
template<typename T, typename...Tx> class mbCallback_t;
template<typename T, typename...Tx> class lbCallback_t;
template<typename...Tx>             class chCallback_t;
template<typename...TArgs>          class pxCallback;
template<typename...TArgs>          class pxEventHandler;





template<typename...Tx>
class stCallback_t : public IEventCallback<Tx...> {
public:
    using pointer_to_function = void(*)(Tx...);
    stCallback_t(const pointer_to_function& fn);

    IEventCallback<Tx...>* copy() override;
    operator DWORD() override;
    void operator()(Tx...e) override;
private:
    pointer_to_function _fn;
};

 template<typename T, typename...Tx>
 class mbCallback_t : public IEventCallback<Tx...> {
 public:
     using pointer_to_member_function = void(T::*)(Tx...);
     using pointer_to_instance = T*;
     mbCallback_t(pointer_to_instance inst, const pointer_to_member_function& f);

     IEventCallback<Tx...>* copy() override;
     operator DWORD() override;
     void operator()(Tx...e) override;
 private:
     pointer_to_instance _inst;
     pointer_to_member_function _fn;
 };

 template<typename T,typename...Tx>
 class lbCallback_t : public IEventCallback<Tx...> {
     static_assert(std::is_invocable_v<T, Tx...>, "Template argument TCallable must be a callable object with TArgs...");
     static_assert(!std::is_same_v<pxEventHandler<Tx...>, T>,
         "EventHandler decayed into lbCallback_t, "
		 "this may happens when trying to join two event handlers with += operator, "
		 "if this is the case, use EventHandler::Join method instead");
     
 public:
     lbCallback_t(T* f);
     lbCallback_t(T& f);

     IEventCallback<Tx...>* copy() override;
     operator DWORD() override;
     void operator()(Tx...e) override;
 private:
     T* _fn;
 };
 template<typename...Tx>
 class chCallback_t : public IEventCallback<Tx...> {
 public:
 	chCallback_t(pxEventHandler<Tx...>* _handler);

 	IEventCallback<Tx...>* copy() override;
    operator DWORD() override;
    void operator()(Tx...e) override;
 private:
 	pxEventHandler<Tx...>* _e;
 };





/**
 * \class pxCallback
 * \brief Callback class
 * \tparam Tx Args
 */
template<typename... Tx>
class pxCallback : public IEventCallback<Tx...>{
public:
	using HCALLBACK = IEventCallback<Tx...>*;
    using pointer_to_function = typename stCallback_t<Tx...>::pointer_to_function;
    template<typename T> using pointer_to_member_function = typename mbCallback_t <T,Tx...> ::pointer_to_member_function;

    pxCallback(const pxCallback& o);
    pxCallback(pointer_to_function _call);
    template<typename T> pxCallback(T* _inst, pointer_to_member_function<T> _call);
    template<typename T> pxCallback(T _call);
    pxCallback(pxEventHandler<Tx...>* _handler);

	~pxCallback() override;


    void operator()(Tx...e) override;
    operator DWORD() override;
private:
    IEventCallback<Tx...>* ptr;
    explicit operator HCALLBACK() {
        pxTraceStart;
        pxTraceEnd;
        return ptr;

    }
public:
	
};


template<typename... TArgs>
class pxEventHandler {
protected:
   
public:
    using EventCallback = pxCallback<TArgs...>;
    using hCallback = EventCallback*;

	pxEventHandler() = default;
    pxEventHandler(const pxEventHandler& e);
    explicit pxEventHandler(pxEventHandler* e);
    explicit pxEventHandler(const EventCallback& cb);
    /*template<typename T, typename...Tx>
     EventHandler(const T& c, Tx...args);*/

public:
    void Join(const pxEventHandler& e);
	void Add(const pxEventHandler& e);
	void Remove(const pxEventHandler& e);
	void Invoke(TArgs...);

public:
    size_t GetSize() const;
    void Dump(); 

public:
    void operator +=(const EventCallback& e);
    void operator +=(pxEventHandler& e);
    void operator -=(const EventCallback& e);


private:
    void Release() ;
    void Trim(size_t startIndex);
    void Add(hCallback);
    void Remove(hCallback);

#ifdef USING_STL
	std::vector<hCallback> m_callbacks = std::vector<hCallback>();
#endif // USING_STL
#ifdef NOT_USING_STL
    int m_count = 0;
    IEventCallback** m_callbacks = new IEventCallback * [MAX_CALLBACK_COUNT];
#endif // NOT_USING_STL


};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         * 
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */






 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *   1. Concrete Callback                                                            *
  *      .1 - stCallback                                                              *
  *           .1 - Constructor                                                        *
  *                                                                                   *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename ... TArgs>
stCallback_t<TArgs...>::stCallback_t(const pointer_to_function& f) : _fn(f) {
    pxTraceStart;
    pxTraceEnd;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .1 - stCallback                                                              *
 *           .2 - IEventCallback interface implementation                            *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename ... TArgs>
IEventCallback<TArgs...>* stCallback_t<TArgs...>::copy() {
    return new stCallback_t(_fn);
}

template <typename ... TArgs>
stCallback_t<TArgs...>::operator DWORD() {
    return (DWORD)&_fn;
}

template <typename ... TArgs>
void stCallback_t<TArgs...>::operator()(TArgs... e) {
    (*_fn)(e...);
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .2 - mbCallback                                                              *
 *           .1 - Constructor                                                        *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename TInstance, typename ... TArgs>
mbCallback_t<TInstance, TArgs...>::mbCallback_t(pointer_to_instance inst, const pointer_to_member_function& f) : _inst(inst), _fn(f) {
    pxTraceStart;
    pxTraceEnd;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .2 - mbCallback                                                              *
 *           .2 - IEventCallback interface implementation                            *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename TInstance, typename ... TArgs>
IEventCallback<TArgs...>* mbCallback_t<TInstance, TArgs...>::copy() {
    return new mbCallback_t(_inst, _fn);
}

template <typename TInstance, typename ... TArgs>
mbCallback_t<TInstance, TArgs...>::operator DWORD() {
    return (DWORD)&_fn;
}

template <typename TInstance, typename ... TArgs>
void mbCallback_t<TInstance, TArgs...>::operator()(TArgs... e) {
    (_inst->*_fn)(e...);
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .1 - lbCallback                                                              *
 *           .1 - Constructor                                                        *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename TCallable, typename ... TArgs>
lbCallback_t<TCallable, TArgs...>::lbCallback_t(TCallable* f) : _fn(f) {
    pxTraceStart;
    pxTraceEnd;
}

template <typename TCallable, typename ... TArgs>
lbCallback_t<TCallable, TArgs...>::lbCallback_t(TCallable& f) : _fn(&f) {
    pxTraceStart;
    pxTraceEnd;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .1 - lbCallback                                                              *
 *           .2 - IEventCallback interface implementation                            *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename TCallable, typename ... TArgs>
IEventCallback<TArgs...>* lbCallback_t<TCallable, TArgs...>::copy() {
    return new lbCallback_t(_fn);
}

template <typename TCallable, typename ... TArgs>
lbCallback_t<TCallable, TArgs...>::operator DWORD() {
    return (DWORD)&_fn;
}

template <typename TCallable, typename ... TArgs>
void lbCallback_t<TCallable, TArgs...>::operator()(TArgs... e) {
    (*_fn)(e...);
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .1 - chCallback                                                              *
 *           .1 - Constructor                                                        *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename ... TArgs>
chCallback_t<TArgs...>::chCallback_t(pxEventHandler<TArgs...>* _handler) : _e(_handler) {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   1. Concrete Callback                                                            *
 *      .1 - chCallback                                                              *
 *           .2 - IEventCallback interface implementation                            *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <typename ... TArgs>
IEventCallback<TArgs...>* chCallback_t<TArgs...>::copy() {
    return new chCallback_t(_e);
}

template <typename ... TArgs>
chCallback_t<TArgs...>::operator DWORD() {
    return (DWORD)_e;
}

template <typename ... TArgs>
void chCallback_t<TArgs...>::operator()(TArgs... e) {
    (*_e).Invoke(e...);
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   2. Callback ( Proxy to IEventCallback interface )                               *
 *      .1 - Constructors                                                            *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template<typename...TArgs>
pxCallback<TArgs...>::pxCallback(const pxCallback& o) {
    ptr = o.ptr->copy();
}


template<typename...TArgs>
pxCallback<TArgs...>::pxCallback(pointer_to_function _call) : ptr(new stCallback_t(_call)) {
    pxTraceStart;
    pxTraceEnd;
}

template<typename...TArgs>
template<typename _TInstance>
pxCallback<TArgs...>::pxCallback(_TInstance* _inst, pointer_to_member_function< _TInstance> _call) : ptr(new mbCallback_t(_inst, _call)) {
    pxTraceStart;
    pxTraceEnd;
}
template<typename...TArgs>
template<typename _TCallable>
pxCallback<TArgs...>::pxCallback(_TCallable _call) : ptr(new lbCallback_t<_TCallable, TArgs...>(_call)) {

    pxTraceStart;
    pxTraceEnd;
}

template<typename...TArgs>
pxCallback<TArgs...>::pxCallback(pxEventHandler<TArgs...>* _handler) : ptr(new chCallback_t(_handler)) {
    pxTraceStart;
    pxTraceEnd;
}

template<typename...TArgs>
pxCallback<TArgs...>::~pxCallback() {
    delete(ptr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   2. Callback ( Proxy to IEventCallback interface )                               *
 *      .2 - Interface implementation                                                *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template<typename...TArgs>
void pxCallback<TArgs...>::operator()(TArgs...e) {
    (*ptr)(e...);
}

template<typename...TArgs>
pxCallback<TArgs...>::operator DWORD() {
    return (DWORD)*ptr;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   3. EventHandler                                                                 *
 *      .1 - type aliases                                                            *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *   3. EventHandler                                                                 *
  *      .2 - Public                                                                  *
  *           .1 - Constructors                                                       *
  *                                                                                   *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template<typename...TArgs>
pxEventHandler<TArgs...>::pxEventHandler(const pxEventHandler& e) {
    pxTraceStart;
    Release();
    for (size_t i = 0; i < e.GetSize(); i++) {
        this->Add(e.m_callbacks[i]);
    }
    pxTraceEnd;
}

template<typename...TArgs>
pxEventHandler<TArgs...>::pxEventHandler(const EventCallback& cb) : m_callbacks({ new pxCallback<TArgs...>(cb) }) {
    pxTraceStart; pxTraceEnd;
}

template<typename...TArgs>
pxEventHandler<TArgs...>::pxEventHandler(pxEventHandler* e) : m_callbacks({ new pxCallback<TArgs...>(e) }) {
    pxTraceStart; pxTraceEnd;
}

//template<typename...TArgs> template<typename T, typename...Tx>
//EventHandler<TArgs...>::EventHandler(const T& c, Tx...args) : m_callbacks({ new Callback<TArgs...>(c,args...) }) { pxTraceStart; pxTraceEnd; }
//



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   3. EventHandler                                                                 *
 *      .2 - Public                                                                  *
 *           .2 - Functions                                                          *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template<typename...TArgs>
void pxEventHandler<TArgs...>::Join(const pxEventHandler& e) {
    pxTraceStart;
    for (size_t i = 0; i < e.GetSize(); i++) {
        this->Add(e.m_callbacks[i]);
    }
    pxTraceEnd;
}

template<typename...TArgs>
void pxEventHandler<TArgs...>::Add(const pxEventHandler& e) {
    pxTraceStart;
    for (size_t i = 0; i < e.GetSize(); i++) {
        this->Add(e.m_callbacks[i]);
    }

    pxTraceEnd;
}

template<typename...TArgs>
void pxEventHandler< TArgs...>::Remove(const pxEventHandler& e) {
    pxTraceStart;

    for (size_t i = 0; i < e.GetSize(); i++) {
        this->Remove(e.m_callbacks[i]);
    }
    pxTraceEnd;
}

template<typename...TArgs>
void pxEventHandler<TArgs...>::Invoke(TArgs... _args) {
    pxTraceStart;

    for (size_t i = 0; i < this->GetSize(); i++) {
        if (m_callbacks[i] != 0) {
            (*m_callbacks[i])(_args...);
        }
    }

    pxTraceEnd;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   3. EventHandler                                                                 *
 *      .2 - Public                                                                  *
 *           .2 - Const Functions                                                    *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


template<typename...TArgs>
size_t pxEventHandler<TArgs...>::GetSize() const {
#ifdef USING_STL
    return m_callbacks.size();
#endif // USING_STL
#ifdef NOT_USING_STL
    return m_count;
#endif // NOT_USING_STL
}


template<typename...TArgs>
void pxEventHandler<TArgs...>::Dump() {
    printf("Starting to dump contents of EventHandler:\n");
    size_t actual_size = this->GetSize();
    for (size_t i = 0; i < actual_size; i++) {
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   3. EventHandler                                                                 *
 *      .2 - Public                                                                  *
 *           .4 - Operators                                                          *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


 /**
 * \brief Registers a new callback
 * \param e A Callback.
 * \see Callback class and constructors for more info
 */
template< typename...TArgs>
void pxEventHandler<TArgs...>::operator +=(const EventCallback& e) {
    pxTraceStart;
    this->Add(pxEventHandler(e));

    pxTraceEnd;
}

/**
 * \brief Operator for joining two EventHandlers together
 * \param e A different EventHandler
 */
template< typename...TArgs>
void pxEventHandler<TArgs...>::operator +=(pxEventHandler& e) {
    if (&e == this) {
        printf("Error: Joining an EventHandler with it self is not allowed\n");
        return;
    }
    pxTraceStart;
    this->Add(e);

    pxTraceEnd;
}
template< typename...TArgs>
void pxEventHandler<TArgs...>::operator -=(const EventCallback& e) {
    pxTraceStart;
    this->Remove(pxEventHandler(e));
    pxTraceEnd;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   3. EventHandler                                                                 *
 *      .3 - Private                                                                 *
 *           .1 - Functions                                                          *
 *                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


template< typename...TArgs>
void pxEventHandler<TArgs...>::Add(hCallback _newCallback) {
#ifdef NOT_USING_STL
    if (m_count >= MAX_CALLBACK_COUNT)  // Max size reached.        
        return;
#endif // NOT_USING_STL

    pxTraceStart;


    for (size_t i = 0; i < this->GetSize(); i++) {
        if (m_callbacks[i] != 0 && (*m_callbacks[i] == _newCallback)) { // Callback already exists            
            //printf("Unable to add %#X callback, it already exists (%#X)\n", (int)_newCallback, (int)m_callbacks[i]);
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
void pxEventHandler<TArgs...>::Remove(hCallback _callback) {
    pxTraceStart;
    // Removes the element ( if find )
    for (size_t i = 0; i < this->GetSize(); i++) {
        if (*m_callbacks[i] == _callback) {
            delete(m_callbacks[i]);
            m_callbacks[i] = 0;
            //printf("%#X Removed from the event list\n", _callback->Dump());
            Trim(i);
            break;
        }
    }
    delete(_callback);
    pxTraceEnd;
}

template<typename...TArgs>
void pxEventHandler<TArgs...>::Trim(size_t startIndex) {
#ifdef BACKTRACE
    printf("%s(int) called\n ", __FUNCTION__);
#endif // BACKTRACE
    if (startIndex < 0) {
        return;
    }

    const size_t actual_size = this->GetSize();
    for (size_t i = startIndex; i < actual_size; i++) {
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
void pxEventHandler<TArgs...>::Release() {
    pxTraceStart;
    for (int i = 0; i < GetSize(); i++) {
        hCallback cb = m_callbacks[i];
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







