#pragma once

#include <tuple>

template <class... args>
struct uplet
{
	std::tuple<args...> Tuple;

	template <class fonction>
	auto operator ()(const fonction& Fonction) const
	{
		return std::apply(Fonction, this->Tuple);
	}

	template <class fonction>
	auto operator >>(const fonction& Fonction) const
	{
		return std::apply(Fonction, this->Tuple);
	}
};