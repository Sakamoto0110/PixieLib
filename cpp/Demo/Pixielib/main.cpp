#include <iostream>
#include "Events.hpp"
#include "EnumEx.hpp"


class Koo {
public:
	void ab(void*, EventTests::TestEventArgs*) {
		std::cout << "koo:ab called\n";
	}
	void cd(void*, EventTests::TestEventArgs*) {
		std::cout << "koo:cd called\n";
	}
	void de(void*, EventTests::TestEventArgs*) {
		std::cout << "koo:de called\n";
	}
};

void f(void*, EventTests::TestEventArgs*) {

	std::cout << '[' << (int) & f << "] f called \n";
}

template<typename TInst,typename...Tx>
mbCallback_t<TInst,Tx...> _Callback(TInst*_inst,void(TInst::*_f)(Tx...)) {
	return mbCallback_t{ _inst, _f };
}

template<typename...Tx>
stCallback_t<Tx...> _Callback(void(* _f)(Tx...)) {
	return stCallback_t{ _f };
}

template<typename T,typename...Tx>
lbCallback_t<T,Tx...> _Callback(T t) {
	return lbCallback_t<T,Tx...>{ t };
}

void EventTests::UserTest() {

	

	TestEventArgs args{ 51,89346 };
	
	Foo _f = Foo(0,0);
	auto l = [](void*, TestEventArgs*) {};
	mbCallback_t<Foo, void*, TestEventArgs*> cc = { &_f,&Foo::func1 };
	TestEventHandler e1 = TestEventHandler({ &_f, &Foo::func1 });
	
	e1 += TestEventHandler::EventCallback(&_f, &Foo::func1);
	e1 += TestEventHandler::EventCallback{[](void*, TestEventArgs*) {std::cout << "lambdac all\n"; } };
	e1 += {[](void*, TestEventArgs*) {std::cout << "lambdac all\n"; } };
	e1 += {&_f, & Foo::func1};
	Run<10000, 5000>([&] {
		e1 += {&f};
		e1 -= {&f};
		});
	int x = 0;

	 // e2 += &l;
	// TestEventHandler e3 = TestEventHandler([&](void*, TestEventArgs*) {});
	e1.Invoke(0, &args);
	

	

	// e2.Invoke(0, &args);
	// EventHandler e2 = EventHandler();
	// e2 += &abc;

	// e += &fn4;
	// e.Invoke(0,&args);
	
}


int main() {
	


	EventTests::UserTest();
	system("pause");
	return 0;
}




