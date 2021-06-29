#pragma once

#include <tuple>

template <class o, class... x>
auto add(std::tuple<x...> t, o O)
{
	auto l = [O](x... X)
	{
		return std::make_tuple(X..., O);
	};

	return std::apply(l, t);
}


template <class ret, class... args>
struct fonction_impl
{
	virtual ret operator ()(args... Args) const = 0;
};

template <class tupple, class arg>
constexpr auto deduire_base() -> decltype(add(std::declval<tupple>(), std::declval<arg>()));

template <class tupple, class fonc, class arg, class... args>
struct fonction
{
	fonc Fonction;
	tupple Args;

	auto operator ()(arg Arg)
	{
		auto newArgs = add(Args, Arg);

		return fonction<decltype(newArgs), fonc, args...>{Fonction, newArgs};
	}
};



template <class tupple, class fonc>
struct fonction<tupple, fonc, void>
{
	fonc Fonction;
	tupple Args;

	auto operator ()()
	{
		return std::apply(Fonction, Args);
	}
};


template <class retour, class... arguments, class classe>
constexpr fonction<std::tuple<>, classe, arguments..., void> deduire_fonctor(const classe& Classe, retour(classe::* Methode)(arguments...) const);


template <class classe>
auto deduire(const classe& Classe) -> decltype(deduire_fonctor(Classe, &classe::operator()))
{
	return { Classe, std::make_tuple() };
}