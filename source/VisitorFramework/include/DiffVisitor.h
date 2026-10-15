#pragma once


#include <array>
#include <cstring>
#include <ios>
#include <iostream>
#include <functional>
#include <ostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>

#include "DiffContext.h"
#include "DifferenceDescriptor.h"
#include "DifferenceType.h"
#include "FieldVisitor.h"
#include "VisitorTraits.h"


template<typename C, typename M>
M GetMemberType(M C::*);


template<typename T>
class DiffVisitor : public FieldVisitor<DiffVisitor<T>>
{
public:

	DiffVisitor(T& object, DiffContext& context, const std::string& parent_name = "") : m_object(object), m_context(context), m_parent_name(parent_name) {}

	void CompareWith(T& other)
	{
		VisitorTraits<T>::Visit(*this, other);

		for (const auto& child_visitor : child_visitors)
		{
			child_visitor();
		}
	}

	template<auto Member>
	void VisitObject(T& other, const char* name)
	{
		auto& object_member = m_object.*Member;
		auto& other_member = other.*Member;

		using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(object_member)>>;

		std::string field_name;
		if (m_parent_name != "")
		{
			field_name = m_parent_name + "->" + name;
		}
		else
		{
			field_name = name;
		}

		if (m_context.visited_pointers.find(&object_member) != m_context.visited_pointers.end())
		{
			return;
		}

		m_context.visited_pointers.insert(&object_member);

		child_visitors.push_back(
			[this, &object_member, &other_member, field_name]()
			{
				DiffVisitor<MemberType> child_visitor(const_cast<MemberType&>(object_member), m_context, field_name);
				child_visitor.CompareWith(const_cast<MemberType&>(other_member));
			}
		);
	}

	template<auto Member>
	void VisitField(T& other, const char* name)
	{
		auto& object_member = m_object.*Member;
		auto& other_member = other.*Member;

		if (m_context.visited_pointers.find(&other_member) != m_context.visited_pointers.end())
		{
			return;
		}

		m_context.visited_pointers.insert(&other_member);

		using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(object_member)>>;
		bool not_equal;
		if constexpr (std::is_floating_point_v<MemberType>)
		{
			not_equal = std::abs(other_member - object_member) > 0.0001;
		}
		else
		{
			not_equal = std::memcmp(&other_member, &object_member, sizeof(other_member)) != 0;
		}

		if (not_equal)
		{
			std::string field_name;
			if (m_parent_name != "")
			{
				field_name = m_parent_name + "->" + name;
			}
			else
			{
				field_name = name;
			}

			std::stringstream object_stream;
			if constexpr (std::is_integral_v<MemberType>)
			{
				object_stream << static_cast<int>(object_member);
			}
			else
			{
				object_stream << object_member;
			}

			std::stringstream other_stream;
			if constexpr (std::is_integral_v<MemberType>)
			{
				other_stream << static_cast<int>(other_member);
			}
			else
			{
				other_stream << other_member;
			}

			const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Field, field_name, object_stream.str(), other_stream.str() };

			m_context.differences.push_back(difference_descriptor);
		}
	}

	template<auto Member>
	void VisitPointer(T& other, const char* name)
	{
		std::string field_name;
		if (m_parent_name != "")
		{
			field_name = m_parent_name + "->" + name;
		}
		else
		{
			field_name = name;
		}

		auto* other_ptr = other.*Member;
		auto* object_ptr = m_object.*Member;

		if (other_ptr == nullptr || object_ptr == nullptr)
		{
			if (other_ptr != object_ptr)
			{
				std::stringstream object_stream;
				object_stream << std::hex << "0x" << (int)(object_ptr);

				std::stringstream other_stream;
				other_stream << std::hex << "0x" << (int)(other_ptr);

				const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Pointer, field_name, object_stream.str(), other_stream.str() };
				m_context.differences.push_back(difference_descriptor);
			}

			return;
		}

		using PointerType = decltype(GetMemberType(Member));

		using Pointee = std::remove_pointer_t<PointerType>;

		if (m_context.visited_pointers.find(object_ptr) != m_context.visited_pointers.end())
		{
			return;
		}

		m_context.visited_pointers.insert(object_ptr);

		child_visitors.push_back(
			[this, object_ptr, other_ptr, field_name]()
			{
				DiffVisitor<Pointee> child_visitor(*object_ptr, m_context, field_name);
				child_visitor.CompareWith(*other_ptr);
			}
		);
	}

	template<auto Member, typename T, size_t Rank>
	void VisitArrayElement(T& other, const char* member_name, const std::array<size_t, Rank>& indices)
	{
		auto& object_array = m_object.*Member;
		auto& other_array = other.*Member;

		VisitArrayElementImpl<0>(object_array, other_array, member_name, indices);
	}

	template<size_t Dimension, typename ObjectArray, typename OtherArray, size_t Rank>
	void VisitArrayElementImpl(ObjectArray& object_array, OtherArray& other_array, const char* member_name, const std::array<size_t, Rank>& indices)
	{
		const auto index = indices[Dimension];

		auto& object_el = object_array[index];
		auto& other_el = other_array[index];

		if constexpr (Dimension + 1 == Rank)
		{
			VisitElement(object_el, other_el, member_name, indices);
		}
		else
		{
			VisitArrayElementImpl<Dimension + 1>(object_el, other_el, member_name, indices);
		}
	}

	template<typename ObjectElement, typename OtherElement, size_t Rank>
	void VisitElement(ObjectElement& object_el, OtherElement& other_el, const char* member_name, std::array<size_t, Rank> indices)
	{
		using MemberType = std::remove_cv_t<std::remove_reference_t<ObjectElement>>;

		std::string field_name;

		if (m_parent_name != "")
		{
			field_name = m_parent_name + "->" + member_name;
		}
		else
		{
			field_name = member_name;
		}

		for (const auto index : indices)
		{
			field_name += "[" + std::to_string(index) + "]";
		}

		if constexpr (std::is_pointer_v<MemberType>)
		{
			auto* object_ptr = object_el;
			auto* other_ptr = other_el;

			if (other_ptr == nullptr || object_ptr == nullptr)
			{
				if (other_ptr != object_ptr)
				{
					std::stringstream object_stream;
					object_stream << std::hex << "0x" << reinterpret_cast<uintptr_t>(object_ptr);

					std::stringstream other_stream;
					other_stream << std::hex << "0x" << reinterpret_cast<uintptr_t>(other_ptr);

					const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Pointer, field_name, object_stream.str(), other_stream.str() };

					m_context.differences.push_back(difference_descriptor);
				}

				return;
			}

			using Pointee = std::remove_pointer_t<MemberType>;

			if (m_context.visited_pointers.find(object_ptr) != m_context.visited_pointers.end())
			{
				return;
			}

			m_context.visited_pointers.insert(object_ptr);

			child_visitors.push_back(
				[this, object_ptr, other_ptr, field_name]()
				{
					DiffVisitor<Pointee> child_visitor(*object_ptr, m_context, field_name);

					child_visitor.CompareWith(*other_ptr);
				}
			);
		}
		else if constexpr (std::is_arithmetic_v<MemberType>)
		{
			if (other_el != object_el)
			{
				std::stringstream object_stream;
				object_stream << object_el;

				std::stringstream other_stream;
				other_stream << other_el;

				const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Field, field_name, object_stream.str(), other_stream.str() };

				m_context.differences.push_back(difference_descriptor);
			}
		}
		else if constexpr (HasVisitorTraits_v<MemberType, decltype(*this)>)
		{
			if (m_context.visited_pointers.find(&object_el) != m_context.visited_pointers.end())
			{
				return;
			}

			m_context.visited_pointers.insert(&object_el);

			child_visitors.push_back(
				[this, &object_el, &other_el, field_name]()
				{
					DiffVisitor<MemberType> child_visitor(const_cast<MemberType&>(object_el), m_context, field_name);

					child_visitor.CompareWith(const_cast<MemberType&>(other_el));
				}
			);
		}
	}

	template<auto Member, typename T, typename ArrayType, size_t Dimension>
	void VisitArrayDimension(T& object, const char* member_name, std::array<size_t, std::rank_v<ArrayType>>& indices)
	{
		constexpr size_t Rank = std::rank_v<ArrayType>;
		constexpr size_t Count = std::extent_v<ArrayType, Dimension>;

		for (size_t i = 0; i < Count; ++i)
		{
			indices[Dimension] = i;

			if constexpr (Dimension + 1 == Rank)
			{
				Self().template VisitArrayElement<Member>(object, member_name, indices);
			}
			else
			{
				VisitArrayDimension<Member, T, ArrayType, Dimension + 1>(object, member_name, indices);
			}
		}
	}

private:
	T& m_object;
	DiffContext& m_context;
	std::string m_parent_name;

	std::vector<std::function<void()>> child_visitors;
};
