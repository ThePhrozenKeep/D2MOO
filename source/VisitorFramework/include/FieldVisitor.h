#pragma once


#include <array>
#include <type_traits>

#include "VisitorTraits.h"


template<typename Derived>
class FieldVisitor
{
public:
	template<auto Member, typename T>
	void Object(T& object, const char* member_name)
	{
		using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(Get<Member>(object))>>;

		static_assert(
			HasVisitorTraits_v<MemberType, Derived>,
			"Object() only accepts types that defined the VisitorTraits trait."
		);

		Self().template VisitObject<Member>(object, member_name);
	}

	template<auto Member, typename T>
	void Field(T& object, const char* member_name)
	{
		using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(Get<Member>(object))>>;

		static_assert(
			std::is_trivially_copyable_v<MemberType>,
			"Field() only accepts trivially copyable types."
		);

		Self().template VisitField<Member>(object, member_name);
	}

	template<auto Member, typename T>
	void Pointer(T& object, const char* member_name)
	{
		using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(Get<Member>(object))>>;

		static_assert(
			std::is_pointer_v<MemberType>,
			"Pointer() only accepts pointer types."
		);

		Self().template VisitPointer<Member>(object, member_name);
	}

	template<auto Member, typename T>
	void Array(T& object, const char* member_name)
	{
		using ArrayType = std::remove_reference_t<decltype(Get<Member>(object))>;

		static_assert(
			std::is_array_v<ArrayType>,
			"Array() only accepts built-in array members."
		);

		std::array<size_t, std::rank_v<ArrayType>> indices{};

		VisitArrayDimension<Member>(object, member_name, indices);
	}

	template<auto Member, typename T, size_t Rank, size_t Dimension = 0>
	void VisitArrayDimension(T& object, const char* member_name, std::array<size_t, Rank>& indices)
	{
		using ArrayType = std::remove_reference_t<decltype(Get<Member>(object))>;

		constexpr size_t count = std::extent_v<ArrayType, Dimension>;

		for (auto i = 0; i < count; ++i)
		{
			indices[Dimension] = i;

			if constexpr (Dimension + 1 == Rank)
			{
				Self().template VisitArrayElement<Member>(object, member_name, indices);
			}
			else
			{
				VisitArrayDimension<Member, T, Rank, Dimension + 1>(object, member_name, indices);
			}
		}
	}

	template<auto PointerMember, auto CountMember, typename T>
	void DynamicArray(T& object, const char* member_name)
	{
		using PointerType = std::remove_cv_t<std::remove_reference_t<decltype(Get<PointerMember>(object))>>;

		static_assert(
			std::is_pointer_v<PointerType>,
			"DynamicArray() requires the first member to be a pointer."
		);

		using CountType = std::remove_cv_t<std::remove_reference_t<decltype(Get<CountMember>(object))>>;

		static_assert(
			std::is_integral_v<CountType>,
			"DynamicArray() requires the count member to be an integral type."
		);

		auto* data = Get<PointerMember>(object);
		auto count = Get<CountMember>(object);

		for (auto i = 0; i < count; ++i)
		{
			const auto indices = std::array<size_t, 1>{ static_cast<size_t>(i) };
			Self().template VisitArrayElement<PointerMember>(object, member_name, indices);
		}
	}

protected:
	Derived& Self()
	{
		return static_cast<Derived&>(*this);
	}

	const Derived& Self() const
	{
		return static_cast<const Derived&>(*this);
	}

	template<auto Member, typename T>
	decltype(auto) Get(T& object)
	{
		return (object.*Member);
	}

	template<typename Array, size_t Rank>
	decltype(auto) GetArrayElement(Array& array, const std::array<size_t, Rank>& indices)
	{
		return GetArrayElementImpl<0>(array, indices);
	}

	template<size_t Dimension, typename Array, size_t Rank>
	decltype(auto) GetArrayElementImpl(Array& array, const std::array<size_t, Rank>& indices)
	{
		if constexpr (Dimension + 1 == Rank)
		{
			return (array[indices[Dimension]]);
		}
		else
		{
			return GetArrayElementImpl<Dimension + 1>(array[indices[Dimension]], indices);
		}
	}
};
