#include <iostream>
#include "include/pxEvents.hpp"
#include "include/pxEventsEx.hpp"
#include <random>
#include "include/pxContainers.hpp"
#include "include/_pxDebug.hpp"

void EventTests::UserTest() {

	TestEventArgs args{ 51,89346 };


	TestEventHandler e3 = {&fn1};

	Foo _f = Foo(0,0);
	auto l = [](void*, TestEventArgs*) {};
	TestEventHandler e1 = { &fn1 };
	

	// e1 += [](void*, TestEventArgs*) {std::cout << "lambdac all\n"; };
	e1 += {&_f, & Foo::func1};
	/*Run<10, 2>([&] {
		e1 += {&fn1};
		e1 -= {&fn1};
		});*/
	int x = 0;

	 // e2 += &l;
	// TestEventHandler e3 = TestEventHandler([&](void*, TestEventArgs*) {});
	//e1.Invoke(0, &args);
	

	
	
}


// helper union to cast pointer to member
template<typename classT, typename memberT>
union u_ptm_cast {
	memberT classT::* pmember;
	void* pvoid;
};
#include "include/pxStrings.hpp"
template<typename... Tx>
constexpr int ba(Tx...tx) {
	return 0;
}



int main() {

	// Lexer::AFUNCTION();
	// pxStringFormater::Parse("%i",0);
	// pxStringFormater::Parse("%i", 642);
	//std::cout << pxStringFormater::Parse("teste %i batata", 41) << "\n"; 
	

	EventTests::UserTest();
	system("pause");
	return 0;
}




