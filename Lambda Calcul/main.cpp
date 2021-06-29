#include <concepts>
#include <iostream>

#include "list.hpp"
#include "uplet.hpp"
#include "variant.hpp"
#include "fonction.hpp"

void display(const list<int>& l)
{
	l.match([]()
		{
			std::cout << "[]";
		},	[](auto x, auto y)
		{
			std::cout << x << "::";

			display(y);
		});
};


auto ListeVide = uplet<>();

template <class element, class liste>
using cons = uplet<element, liste>;

template <class element, class liste>
cons<element, liste> Cons(const element& Element, const liste& Liste)
{
	return { {Element, Liste} };
}

using true_t = uplet<>;
using false_t = uplet<>;

auto True = true_t();
auto False = false_t();

template <class a, class b>
using et = uplet<a, b>;

template <class a, class b>
using ou = uplet<a, b>;

template <class a, class b>
uplet<a, b> Et(const a& A, const b& B)
{
	return { {A, B} };
}

template <class a, class b>
uplet<a, b> Ou(const a& A, const b& B)
{
	return { {A, B} };
}

struct eq_true : true_t {};
struct eq_false : false_t {};
using eq_bool = variant<eq_true, eq_false>;
struct eq_et : et <eq_bool, eq_bool>
{
	eq_et(eq_bool a, eq_bool b) : et <eq_bool, eq_bool>{ {a, b} } {};
};
struct eq_ou : ou <eq_bool, eq_bool>
{
	eq_ou(eq_bool a, eq_bool b) : ou <eq_bool, eq_bool>{ {a, b} } {};
};

using eq = variant <eq_true, eq_false, eq_et, eq_ou>;

eq_bool eq_boolTrue{ eq_true() };
eq_bool eq_boolFalse{ eq_false() };


int main()
{
	auto eb = Ou ((Et (True, False)), False);

	auto noteb =
		eb >> [](auto a, auto b)
		{
			return Et (a >> [](auto a, auto b) { return Ou (True, False); }, True);
		};

	auto Liste = Cons (3, Cons (4, Cons (6, ListeVide)));

	Liste >> [](auto e, auto l)
	{
		std::cout << e;

		l >> [](auto e, auto l)
		{
			std::cout << e;
		};
	};

	list<int> l;

	(l += 3) += 4;

	display(l);

	eq EqBooleenne{ eq_ou{ eq_boolFalse, eq_boolFalse } };

	auto eval = EqBooleenne
	| []()	// Cas True
	{
		return true;
	}
	| []()	// Cas False
	{
		return false;
	}
	| [](auto a, auto b)	// Cas Et
	{
		return a
		| [&b]()	// Cas true
		{
			return b
			| []()	// Cas true
			{
				return true;
			}
			| []()
			{
				return false;
			};
		}	// Cas False
		| []()
		{
			return false;
		};
	}
	| [](auto a, auto b)	// Cas Ou
	{
		return a
		| []()	// Cas true
		{
			return true;
		}
		| [&b]()	// Cas False
		{
			return b
			| []()	// Cas true
			{
				return true;
			}
			| []()	// Cas False
			{
				return false;
			};
		};
	};

	return 0;
}