#pragma once
#include "pxCorelib.h"
#include <cmath>


// # # # # # # # 
// vec 2 impl

namespace Vec2 {
	using Vec = pxVec2;
	using crefVec = const pxVec2&;

	static Vec Add(crefVec a, crefVec b) {
		return { a.x + b.x,a.y + b.y };
	}
	static Vec Sub(crefVec a, crefVec b) {
		return { a.x - b.x,a.y - b.y };
	}
	static Vec Mult(crefVec a, double scalar) {
		return { a.x * scalar, a.y * scalar };
	}
	static Vec Div(crefVec a, double value) {
		return { a.x / value, a.y / value};
	}

	static Vec operator +(crefVec a, crefVec b) {
		return Add(a, b);
	}
	static Vec operator -(crefVec a, crefVec b) {
		return Sub(a, b);
	}
	static Vec operator *(crefVec a, double scalar) {
		return Mult(a, scalar);
	}
	static Vec operator /(crefVec a, double value) {
		return Div(a, value);
	}

	static double Mag(crefVec a) {
		return std::sqrt(a.x * a.x + a.y * a.y);
	}

	static double Dot(crefVec a, crefVec b) {
		return a.x * b.x + a.y * b.y;
	}

	static double AngleBetween(crefVec a, crefVec b) {
		return acos(Dot(a, b) / (Mag(a) * Mag(b)));
	}

	static Vec Normalize(crefVec a) {
		return Div(a, Mag(a));
	}

}

