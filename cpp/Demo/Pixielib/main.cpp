#include <iostream>
#define COMPILE_EVENT_TESTS
#include "Events.hpp"



class A {
public:
	friend class B;
private :
	int x = 4;
};


class B {

public:
	A a = A();
	B() {
		a.x = 1;
	}

};

int main() {

	__RUN_EVENT_TEST();
	system("pause");
	return 0;
}