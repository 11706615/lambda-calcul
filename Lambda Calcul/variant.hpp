#pragma once

#include <variant>

template <size_t Index, class fonction, class variant, size_t Total>
struct variant_rec
{
	//static_assert(Index + 1 < Total);

	fonction Fonction;
	variant Variant;

	template <class fonc>
	auto operator ()(const fonc& NewFonction) const
	{
		auto Variant = this->Variant;
		auto Fonction = this->Fonction;
		auto Index1 = Index;

		auto ret = [Fonction, NewFonction, Index1, Variant]()
		{
			if (Variant.index() == Index)
			{
				return std::get<Index>(Variant)(NewFonction);
			}
			else
			{
				return Fonction();
			}
		};

		return variant_rec<Index + 1, decltype(ret), decltype(this->Variant), Total - 1>(ret, Variant);
	}

	template <class fonc>
	auto operator |(const fonc& NewFonction) const
	{
		auto Variant = this->Variant;
		auto Fonction = this->Fonction;
		auto Index1 = Index;

		auto ret = [Fonction, NewFonction, Index1, Variant]()
		{
			if (Variant.index() == Index)
			{
				return std::get<Index>(Variant)(NewFonction);
			}
			else
			{
				return Fonction();
			}
		};

		return variant_rec<Index + 1, decltype(ret), decltype(this->Variant), Total - 1>(ret, Variant);
	}
};

template <size_t Index, class fonction, class variant>
struct variant_rec<Index, fonction, variant, 1>
{
	fonction Fonction;
	variant Variant;

	//static constexpr auto Total = Index + 1;

	template <class fonc>
	auto operator ()(const fonc& NewFonction) const
	{
		auto Variant = this->Variant;
		auto Fonction = this->Fonction;

		if (Variant.index() == Index)
		{
			return std::get<Index>(Variant)(NewFonction);
		}
		else
		{
			return Fonction();
		}
	}

	template <class fonc>
	auto operator |(const fonc& NewFonction) const
	{
		auto Variant = this->Variant;
		auto Fonction = this->Fonction;

		if (Variant.index() == Index)
		{
			return std::get<Index>(Variant)(NewFonction);
		}
		else
		{
			return Fonction();
		}
	}
};

template <class... args>
struct variant
{
	std::variant<args...> Variant;

	template <class fonction>
	auto operator ()(const fonction& Fonction)
	{
		auto Variant = this->Variant;

		auto ret = [Fonction, Variant]()
		{
			if (Variant.index() == 0)
			{
				return std::get<0>(Variant)(Fonction);
			}
			else
			{
				throw 1;
			}
		};

		return variant_rec < 1, decltype(ret), decltype(this->Variant), sizeof...(args) - 1>(ret, Variant);
	}

	template <class fonction>
	auto operator |(const fonction& Fonction)
	{
		auto Variant = this->Variant;

		auto ret = [Fonction, Variant]()
		{
			if (Variant.index() == 0)
			{
				return std::get<0>(Variant)(Fonction);
			}
			else
			{
				throw 1;
			}
		};

		return variant_rec < 1, decltype(ret), decltype(this->Variant), sizeof...(args) - 1>(ret, Variant);
	}
};