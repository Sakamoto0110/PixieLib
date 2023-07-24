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