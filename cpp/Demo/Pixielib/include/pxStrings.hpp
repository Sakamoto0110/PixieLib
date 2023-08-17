#pragma once

#include <string>
#include <array>
#define CH(x) #x[0]
#define lx_bool bool
#define lx_auto auto
#define lx_char char
#define lx_cch const char
#define lxExpr static constexpr
#define lxbExpr lxExpr  lx_bool

#define lxLiteral_v(tok) lxExpr lx_cch tok##_v = ATOM<CH(tok)>::value
#define lxLiteral(tok)  using tok = ATOM<CH(tok)>; lxLiteral_v(tok)
#define lxLiteralA(tok,tok_value)  using tok = ATOM<CH(tok_value)> 


struct ATOM_BASE{};
template<char ch>
struct ATOM : ATOM_BASE{
	ATOM() = delete;
	using type = ATOM<ch>;

	static constexpr char value = ch;
};
using ATOM_EMPTY_DEF = ATOM<-1>;
using ATOM_GROUP_DEF = ATOM<-2>;

template<typename...ATOMx>
struct ATOM_GROUP : ATOM_GROUP_DEF {

	static constexpr size_t size = sizeof...(ATOMx);

	// NOTE: THE FUNCTION THAT CALLS THIS FUNCTION MUST DELETE THE RESULTS POINTER
	template<typename Tx, typename...Ty, typename...Tz>
	static Tx* ExpandInto(Tx(*expr)(Ty...), Tz...args) {
		Tx* results = new Tx[size];
		if constexpr (std::is_invocable<decltype(expr), char, Ty...>()) {
			((*results++ = expr(ATOMx::value, args...)), ...);
		}
		else {
			std::cerr << "Failed to expand atom group. Potential undefined behavior up this point\n";
			for (size_t i = 0; i < size; i++)
				*results++ = expr(args...);
		}
		return results - size;

	}

	// Pointer free alternative for bool expressions	
	template<typename...Ty, typename...Tz>
	static bool ExpandInto(bool(*expr)(Ty...), Tz...args) {
		goto ignore_error;
		// this error check is broken. Reason: ?????
		if constexpr (std::is_invocable<decltype(expr), char, Ty...>()) {
			std::cerr << "Failed to expand atom group. Potential undefined behavior up this point\n";
			return 0;
		}
		ignore_error:
		bool result = false;
		((result |= expr(ATOMx::value, args...)), ...);
		return result;
	}


};

class Lexer {
public:
	

	lxbExpr IS_UPPERCASE(lx_cch ch) { return ch >= A::value && ch <= Z::value; }
	lxbExpr IS_LOWERCASE(lx_cch ch) { return ch >= a::value && ch <= z::value; }
	lxbExpr IS_NUM(lx_cch ch) { return ch >= ZERO::value && ch <= NINE::value; }
	lxbExpr IS_EQUALS(lx_cch a, lx_cch b) { return a == b; }
	lxbExpr is_equals(lx_cch a, lx_cch b) { return IS_EQUALS(a,b); }
	lxbExpr IS_DECLTOK(lx_cch ch) { return IS_EQUALS<PERCENT>(ch); }

	lxbExpr IS_TOK(lx_cch ch) {
		return IS_EQUALS<i>(ch);
	}
	
	
	

	template<typename ATOMx, typename ATOMy>
	static constexpr bool IS_EQUALS() { return ATOMx::value == ATOMy::value; }

	template<typename ATOMx, typename ATOMy>
	static constexpr bool IS_EQUALS_v = IS_EQUALS<ATOMx, ATOMy>();

	template<typename ATOM_or_ATOMGROUP>
	static constexpr bool IS_EQUALS(char ch) {
		if constexpr(IS_EQUALS_v<ATOM_GROUP_DEF, ATOM_or_ATOMGROUP>) {
			return ATOM_or_ATOMGROUP::ExpandInto(is_equals, ch);
		}
		return IS_EQUALS(ATOM_or_ATOMGROUP::value, ch);
	}
	static void	AFUNCTION() {
		std::cout << IS_EQUALS<ATOM_GROUP<A, B, C>>('B') << '\n';
	}
	// static constexpr auto PERCENT = '%';

	/*template<typename _A, typename _B >
	static constexpr bool IS_EQUALS(_A _a = _A::value, _B _b = _B::value) { return  IS_EQUALS(_a, _b); }
*/
	
	

	lxLiteralA(PERCENT, %);
	lxLiteralA(STAR, *);
	lxLiteralA(DOT, .);

	lxLiteralA(ZERO, 0);
	lxLiteralA(ONE, 1);
	lxLiteralA(TWO, 2);
	lxLiteralA(THREE, 3);
	lxLiteralA(FOUR, 4);
	lxLiteralA(FIVE, 5);
	lxLiteralA(SIX, 6);
	lxLiteralA(SEVEN, 7);
	lxLiteralA(EIGHT, 8);
	lxLiteralA(NINE, 9);
	lxLiteral(A);
	lxLiteral(B);
	lxLiteral(C);
	lxLiteral(D);
	lxLiteral(E);
	lxLiteral(F);
	lxLiteral(G);
	lxLiteral(H);
	lxLiteral(I);
	lxLiteral(J);
	lxLiteral(K);
	lxLiteral(L);
	lxLiteral(M);
	lxLiteral(N);
	lxLiteral(O);
	lxLiteral(P);
	lxLiteral(Q);
	lxLiteral(R);
	lxLiteral(S);
	lxLiteral(T);
	lxLiteral(U);
	lxLiteral(V);
	lxLiteral(W);
	lxLiteral(Y);
	lxLiteral(X);
	lxLiteral(Z);
	lxLiteral(a);
	lxLiteral(b);
	lxLiteral(c);
	lxLiteral(d);
	lxLiteral(e);
	lxLiteral(f);
	lxLiteral(g);
	lxLiteral(h);
	lxLiteral(i);
	lxLiteral(j);
	lxLiteral(k);
	lxLiteral(l);
	lxLiteral(m);
	lxLiteral(n);
	lxLiteral(o);
	lxLiteral(p);
	lxLiteral(q);
	lxLiteral(r);
	lxLiteral(s);
	lxLiteral(t);
	lxLiteral(u);
	lxLiteral(v);
	lxLiteral(w);
	lxLiteral(y);
	lxLiteral(x);
	 lxLiteral(z);
	
	

	
};


class pxStringFormater {
public:
	using string = std::string;

	static string tok_i(int value) {
		return std::to_string(value);
	}

	
	template<typename...Tx>
	struct TOKEN {
		inline static std::map<char, string(*)(Tx...)> token_map =
		{
			{'i', &tok_i}

		};
	};

	template<typename...Tx>
	static string Parse(string format, Tx... args) {
		std::array<Tx..., sizeof...(Tx)> arr = { args... };
		int arg_ptr = 0;
		for (int i = 0; i < format.size(); i++) {
			char ch = format[i];

			if(i != format.size()-1) {
				if(Lexer::IS_DECLTOK(ch)) {
					// i will point to % index
					// j will point to tok index, if find
					int j = i + 1;
					while (j < format.size() && !Lexer::IS_TOK(format[j])) { j++; }
					if(j >= format.size()) {
						// valid token not found
						break;
					}
					string before_tok = format.substr(0, i);
					// 
					string tok_str = format.substr(i + 1, j - i);
					string tok_value_str = TOKEN<Tx...>::token_map[format[j]](arr[arg_ptr++]);
					string after_tok = format.substr(j + 1, format.size());
					std::cout << "before_tok: _" << before_tok << "_\n";
					std::cout << "tok: _" << tok_str << "_\n";
					std::cout << "tok result: _" << tok_value_str << "_\n";
					std::cout << "after_tok: _" << after_tok << "_\n";
				}
			}
			
			//if (Lexer::IS_EQUALS<Lexer::PERCENT>(ch)) {
			//	if (i != format.size() - 1) {
			//		if (Lexer::IS_EQUALS<Lexer::i>(format[i + 1])) {
			//			string pre = format.substr(0, i);
			//			string value = std::to_string(arr[0]);
			//			string pos = format.substr(i + 2, format.size());
			//			return pre + value + pos;
			//		}
			//	}
			//}
		}
		return "";
	}

};


// template<typename _Tx, typename _Ty>
// static bool operator==(_Tx tx, _Ty ty) { return false; }