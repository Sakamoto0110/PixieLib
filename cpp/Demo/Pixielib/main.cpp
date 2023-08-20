#include <iostream>
#include "include/pxEvents.hpp"
#include "include/pxEventsEx.hpp"
#include <random>
#include "include/pxContainers.hpp"
#include "include/_pxDebug.hpp"

void EventTests::UserTest() {

	TestEventArgs args{ 51,89346 };

	TestEventHandler e1 = TestEventHandler();
	
	pxTests::Run<10000, 3>([&] {
		e1 = TestEventHandler(&fn1);
		e1 += fn2;
		e1 -= fn2;
		});

	

	
	
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
	
	int __ = 0;
	EventTests::UserTest();
	int _ = 0;
	system("pause");
	return 0;
}




