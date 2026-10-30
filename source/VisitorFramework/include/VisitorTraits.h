#pragma once


#include <type_traits>
#include <utility>


template<typename T>
struct VisitorTraits
{
	// Marks the unspecialized template, so HasVisitorTraits can tell it apart from specializations
	using Unspecialized = void;

	template<class Visitor>
	static void Visit(Visitor&, T&)
	{
		static_assert(sizeof(T) == 0, "VisitorTraits<T> has not been specialized.");
	}
};


template<typename T, typename = void>
struct IsVisitorTraitsSpecialized : std::true_type {};

template<typename T>
struct IsVisitorTraitsSpecialized<T, std::void_t<typename VisitorTraits<T>::Unspecialized>> : std::false_type {};

template<typename T, typename Visitor, typename = void>
struct HasVisitorTraits : std::false_type{};

template<typename T, typename Visitor>
struct HasVisitorTraits<T, Visitor, std::void_t<decltype(VisitorTraits<T>::Visit(std::declval<Visitor&>(), std::declval<T&>()))>> : IsVisitorTraitsSpecialized<T> {};

template<typename T, typename Visitor>
inline constexpr bool HasVisitorTraits_v = HasVisitorTraits<T, Visitor>::value;
