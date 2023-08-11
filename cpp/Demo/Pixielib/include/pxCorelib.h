#pragma once
#define PIXIE_LONG_TYPENAME Pixie
#define PIXIE_SHORT_TYPENAME px

#define MAKE_TYPENAME(name) name##_t
#define MAKE_PIXIE_LONG_TYPENAME(name) Pixie##name
#define MAKE_PIXIE_SHORT_TYPENAME(name) px##name


#define PARENS () 
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define PIXIE_TYPEDEF(name,...)									\
	MAKE_TYPENAME(name)											\
	EXPAND(PIXIE_TYPEDEF_HELPER(name, __VA_ARGS__))
#define PIXIE_TYPEDEF_HELPER(name, typename_maker_func, ...)    \
	typename_maker_func(name)                                   \
	__VA_OPT__(,PIXIE_TYPEDEF_AGAIN PARENS (name, __VA_ARGS__))
#define PIXIE_TYPEDEF_AGAIN() PIXIE_TYPEDEF_HELPER

template<typename T>
struct pxPoint_t {
	T x;
	T y;
	bool operator ==(const pxPoint_t& o) const { return x == o.x && y == o.y; }

};
using pxPoint  = pxPoint_t<int>;
using pxPointf = pxPoint_t<float>;
using pxPointd = pxPoint_t<double>;

template<typename T>
struct pxSize_t {
	T width;
	T height;
	bool operator ==(const pxSize_t& o) const { return width == o.width && height == o.height; }

};
using pxSize =  pxSize_t<int>;
using pxSizef = pxSize_t<float>;
using pxSized = pxSize_t<double>;

struct pxColor {
	unsigned char red = 255;
	unsigned char green = 255;
	unsigned char blue = 255;
	unsigned char alpha = 255;
};
inline void Show(const pxColor& c) {
	printf("red: %i, green: %i, blue: %i, alpha: %i\n", c.red, c.green, c.blue, c.alpha);
}
inline pxColor UColorToPixieColor(unsigned int color) {
	return  {
		 (unsigned char)((color & 0xFF000000) >> 24),
		 (unsigned char)((color & 0x00FF0000) >> 16),
		 (unsigned char)((color & 0x0000FF00) >> 8),
		 (unsigned char)((color & 0x000000FF) >> 0)
	};
}

inline unsigned int PixieColorToUColor(const pxColor& c) {
	return ((c.red & 0x0ff) << 24) | ((c.green & 0x0ff) << 16) | ((c.blue & 0x0ff) << 8) | ((c.alpha & 0x0ff) << 0);
}



struct pxVec2 {
	double x;
	double y;
};

struct pxVec3 {
	double x;
	double y;
	double z;
};

