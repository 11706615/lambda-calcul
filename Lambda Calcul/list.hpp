#pragma once

#include <vector>
#include <variant>
#include <tuple>

template <class objet>
concept iterable = requires(objet & Objet)
{
	{ Objet.begin() };
	{ Objet.end() };
};

template <class objet, class type = size_t>
concept indiçable = requires(objet & Objet, type Position)
{
	//std::is_same<decltype(Objet[Position]), element>::value;
	{ Objet[Position] };
};

template <class tableau>
requires indiçable<tableau>&& iterable<tableau>
int foo(const tableau& I)
{
	return I[3];
}


template <class objet, class retour, class... args>
concept appelable = requires (objet Objet, args&& ...Args)
{
	{ Objet(std::forward<args>(Args)...) };
};

template <class type>
class list
{
	std::vector<type> v;

public:

	list() = default;
	list(const list<type>& l) = default;

	list<type>& operator +=(const type& Type)
	{
		this->v.push_back(Type);

		return *this;
	}

	type operator --()
	{
		auto ret{ this->v.back() };

		this->v.pop_back();

		return ret;
	}

	template <class fonction, class... fonctions>
	requires appelable<fonction, void>
		auto match(const fonction& Fonction, fonctions&& ...Fonctions) const
	{
		if (this->v.empty())
		{
			return Fonction();
		}
		else
		{
			return this->match(std::forward<fonctions...>(Fonctions)...);
		}
	}

	template <class fonction, class... fonctions>
	requires appelable<fonction, void, type, list<type>>
		auto match(const fonction& Fonction, fonctions&& ...Fonctions) const
	{
		if (!this->v.empty())
		{
			type elem{ this->v.back() };
			list<type> l{ *this };
			l.v.pop_back();

			return Fonction(elem, l);
		}
		else
		{
			return this->match(std::forward<fonctions...>(Fonctions)...);
		}
	}

	void match() const
	{
		throw "match failure";
	}


	std::variant<std::tuple<list<type>>, std::tuple<type, list<type>>> matcher() const
	{
		if (this->v.empty())
		{
			return { std::make_tuple(list<type>(*this)) };
		}
		else
		{
			type elem{ this->v.back() };
			list<type> l{ *this };
			l.v.pop_back();

			return { std::make_tuple(elem, l) };
		}
	}

};