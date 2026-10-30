#pragma once


#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <functional>
#include <ostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include "DiffContext.h"
#include "DiffFieldPath.h"
#include "DifferenceDescriptor.h"
#include "DifferenceType.h"
#include "DynamicArrayCountRegistry.h"
#include "FieldVisitor.h"
#include "VisitorTraits.h"


template<typename C, typename M>
M GetMemberType(M C::*);


template<typename T>
class DiffVisitor : public FieldVisitor<DiffVisitor<T>>
{
public:

	DiffVisitor(T& object, DiffContext& context, const std::string& parent_name = "")
		: DiffVisitor(object, context, parent_name.empty() ? nullptr : std::make_shared<DiffFieldPath>(nullptr, parent_name))
	{
	}

	void CompareWith(T& other)
	{
		// No-op for child visitors, whose objects were marked before they were created
		MarkVisited(m_object, other);

		VisitorTraits<T>::Visit(*this, other);

		// Children are compared from an explicit stack instead of recursively, so long pointer chains cannot overflow the call stack.
				// They are pushed in reverse so they are compared in declaration order, depth first.
		m_context.pending_comparisons.insert(m_context.pending_comparisons.end(), child_visitors.rbegin(), child_visitors.rend());
		child_visitors.clear();

		// Only the outermost CompareWith() call works through the stack
		if (m_context.comparing)
		{
			return;
		}

		m_context.comparing = true;

		try
		{
			while (!m_context.pending_comparisons.empty())
			{
				const auto comparison = std::move(m_context.pending_comparisons.back());
				m_context.pending_comparisons.pop_back();

				comparison();
			}
		}
		catch (...)
		{
			m_context.pending_comparisons.clear();
			m_context.comparing = false;
			throw;
		}

		m_context.comparing = false;
	}

	template<auto Member>
	void VisitObject(T& other, const char* name)
	{
		auto& object_member = m_object.*Member;
		auto& other_member = other.*Member;

		using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(object_member)>>;

		CompareChild<MemberType>(object_member, other_member, name);
	}

	template<auto Member>
	void VisitField(T& other, const char* name)
	{
		auto& object_member = m_object.*Member;
		auto& other_member = other.*Member;

		if (!AreValuesEqual(object_member, other_member))
		{
			const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Field, FieldName(name), FormatValue(object_member), FormatValue(other_member) };

			m_context.differences.push_back(difference_descriptor);
		}
	}

	template<auto Member>
	void VisitPointer(T& other, const char* name)
	{
		auto* other_ptr = other.*Member;
		auto* object_ptr = m_object.*Member;

		if (other_ptr == nullptr || object_ptr == nullptr)
		{
			ReportPointerDifference(name, object_ptr, other_ptr);

			return;
		}

		using PointerType = decltype(GetMemberType(Member));

		using Pointee = std::remove_cv_t<std::remove_pointer_t<PointerType>>;

		CompareChild<Pointee>(*object_ptr, *other_ptr, name);
	}

	// Shadows FieldVisitor::DynamicArray() to report a difference when only one of the arrays is null or the counts differ.
	// Only the elements present in both arrays are compared.
	template<auto PointerMember, auto CountMember>
	void DynamicArray(T& other, const char* name)
	{
		using CountType = std::remove_cv_t<std::remove_reference_t<decltype(other.*CountMember)>>;

		static_assert(
			std::is_integral_v<CountType>,
			"DynamicArray() requires the count member to be an integral type."
		);

		if (CompareNullDynamicArrays<PointerMember>(other, name))
		{
			return;
		}

		const auto object_count = ToElementCount(m_object.*CountMember);
		const auto other_count = ToElementCount(other.*CountMember);

		if (object_count != other_count)
		{
			const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Field, FieldName(name), std::to_string(object_count) + " elements", std::to_string(other_count) + " elements" };
			m_context.differences.push_back(difference_descriptor);
		}

		this->template VisitDynamicArrayElements<PointerMember>(other, name, std::min(object_count, other_count));
	}

	// Shadows FieldVisitor::DynamicArrayWithRegisteredCount() to report a difference when only one of the arrays is null
	template<auto PointerMember>
	void DynamicArrayWithRegisteredCount(T& other, const char* name, std::string_view count_key)
	{
		if (CompareNullDynamicArrays<PointerMember>(other, name))
		{
			return;
		}

		this->template VisitDynamicArrayElements<PointerMember>(other, name, DynamicArrayCountRegistry::Instance().GetCount(count_key));
	}

	template<auto Member, typename Other, size_t Rank>
	void VisitArrayElement(Other& other, const char* member_name, const std::array<size_t, Rank>& indices)
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

		std::string name = member_name;

		for (const auto index : indices)
		{
			name += "[" + std::to_string(index) + "]";
		}

		if constexpr (std::is_pointer_v<MemberType>)
		{
			auto* object_ptr = object_el;
			auto* other_ptr = other_el;

			if (other_ptr == nullptr || object_ptr == nullptr)
			{
				ReportPointerDifference(name, object_ptr, other_ptr);

				return;
			}

			using Pointee = std::remove_cv_t<std::remove_pointer_t<MemberType>>;

			CompareChild<Pointee>(*object_ptr, *other_ptr, name);
		}
		else if constexpr (HasVisitorTraits_v<MemberType, decltype(*this)>)
		{
			CompareChild<MemberType>(object_el, other_el, name);
		}
		else if constexpr (std::is_trivially_copyable_v<MemberType>)
		{
			if (!AreValuesEqual(object_el, other_el))
			{
				const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Field, FieldName(name), FormatValue(object_el), FormatValue(other_el) };

				m_context.differences.push_back(difference_descriptor);
			}
		}
		else
		{
			static_assert(sizeof(MemberType) == 0, "Array elements must be pointers, trivially copyable or define VisitorTraits.");
		}
	}

private:
	template<typename>
	friend class DiffVisitor;

	DiffVisitor(T& object, DiffContext& context, std::shared_ptr<DiffFieldPath> path)
		: m_object(object), m_context(context), m_path(std::move(path))
	{
	}

	static constexpr double FloatingPointTolerance = 0.0001;

	template<typename Value, typename = void>
	struct IsStreamable : std::false_type {};

	template<typename Value>
	struct IsStreamable<Value, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const Value&>())>> : std::true_type {};

	// Values without VisitorTraits (e.g. structs used with FIELD()) are compared bytewise, including any padding
	template<typename Value>
	static bool AreValuesEqual(const Value& object_value, const Value& other_value)
	{
		if constexpr (std::is_array_v<Value>)
		{
			for (size_t i = 0; i < std::extent_v<Value>; ++i)
			{
				if (!AreValuesEqual(object_value[i], other_value[i]))
				{
					return false;
				}
			}

			return true;
		}
		else if constexpr (std::is_floating_point_v<Value>)
		{
			// Exact match first, so equal infinities compare equal
			if (object_value == other_value)
			{
				return true;
			}

			if (std::isnan(object_value) || std::isnan(other_value))
			{
				return std::isnan(object_value) && std::isnan(other_value);
			}

			return std::abs(object_value - other_value) <= FloatingPointTolerance;
		}
		else
		{
			return std::memcmp(&object_value, &other_value, sizeof(Value)) == 0;
		}
	}

	template<typename Value>
	static std::string FormatValue(const Value& value)
	{
		std::stringstream stream;

		if constexpr (std::is_array_v<Value>)
		{
			stream << "{ ";

			for (size_t i = 0; i < std::extent_v<Value>; ++i)
			{
				stream << (i == 0 ? "" : ", ") << FormatValue(value[i]);
			}

			stream << " }";
		}
		else if constexpr (std::is_integral_v<Value>)
		{
			// Unary + promotes character types to integers without truncating wider types
			stream << +value;
		}
		else if constexpr (std::is_enum_v<Value>)
		{
			stream << +static_cast<std::underlying_type_t<Value>>(value);
		}
		else if constexpr (std::is_floating_point_v<Value>)
		{
			// Enough digits to tell apart any two different values
			stream << std::setprecision(std::numeric_limits<Value>::max_digits10) << value;
		}
		else if constexpr (std::is_pointer_v<Value>)
		{
			// Also covers function pointers, which operator<< would print as a bool
			uintptr_t address = 0;
			std::memcpy(&address, &value, sizeof(value));
			stream << std::hex << "0x" << address;
		}
		else if constexpr (IsStreamable<Value>::value)
		{
			stream << value;
		}
		else
		{
			const auto* bytes = reinterpret_cast<const unsigned char*>(&value);

			stream << std::hex << std::setfill('0');

			for (size_t i = 0; i < sizeof(Value); ++i)
			{
				stream << (i == 0 ? "" : " ") << std::setw(2) << +bytes[i];
			}
		}

		return stream.str();
	}

	static std::string FormatPointer(const void* pointer)
	{
		std::stringstream stream;
		stream << std::hex << "0x" << reinterpret_cast<uintptr_t>(pointer);
		return stream.str();
	}

	template<typename Type>
	void MarkVisited(const Type& object, const Type& other)
	{
		const auto type = std::type_index(typeid(Type));

		m_context.visited_objects.emplace(DiffContext::VisitedKey{ &object, type }, &other);
		m_context.visited_other_objects.emplace(DiffContext::VisitedKey{ &other, type }, &object);
	}

	// Queues the comparison of two objects, unless they were already compared with each other.
	// An object that was already compared with a different object means the object graphs are shaped differently
	// (e.g. two pointers share an object on one side only), which is reported as a pointer difference.
	template<typename Type>
	void CompareChild(const Type& object, const Type& other, const std::string& name)
	{
		const auto type = std::type_index(typeid(Type));

		const auto object_it = m_context.visited_objects.find(DiffContext::VisitedKey{ &object, type });
		const auto other_it = m_context.visited_other_objects.find(DiffContext::VisitedKey{ &other, type });

		const bool object_visited = object_it != m_context.visited_objects.end();
		const bool other_visited = other_it != m_context.visited_other_objects.end();

		if (object_visited || other_visited)
		{
			if (!object_visited || !other_visited || object_it->second != &other)
			{
				auto object_value = FormatPointer(&object);
				if (object_visited)
				{
					object_value += " (compared with " + FormatPointer(object_it->second) + " before)";
				}

				auto other_value = FormatPointer(&other);
				if (other_visited)
				{
					other_value += " (compared with " + FormatPointer(other_it->second) + " before)";
				}

				const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Pointer, FieldName(name), object_value, other_value };
				m_context.differences.push_back(difference_descriptor);
			}

			return;
		}

		MarkVisited(object, other);

		// Captures the context instead of this, because the comparison runs after this visitor is gone
		child_visitors.push_back(
			[&context = m_context, &object, &other, path = std::make_shared<DiffFieldPath>(m_path, name)]()
			{
				DiffVisitor<Type> child_visitor(const_cast<Type&>(object), context, path);
				child_visitor.CompareWith(const_cast<Type&>(other));
			}
		);
	}

	std::string FieldName(const std::string& name) const
	{
		if (m_path != nullptr)
		{
			return m_path->ToString() + "->" + name;
		}

		return name;
	}

	// Reports a difference if exactly one of the pointers is null
	void ReportPointerDifference(const std::string& name, const void* object_ptr, const void* other_ptr)
	{
		if (object_ptr != other_ptr)
		{
			const auto difference_descriptor = DifferenceDescriptor{ DifferenceType::Pointer, FieldName(name), FormatPointer(object_ptr), FormatPointer(other_ptr) };
			m_context.differences.push_back(difference_descriptor);
		}
	}

	// Returns true if at least one of the arrays is null, reporting a difference if only one of them is
	template<auto PointerMember>
	bool CompareNullDynamicArrays(T& other, const char* name)
	{
		auto* other_ptr = other.*PointerMember;
		auto* object_ptr = m_object.*PointerMember;

		if (other_ptr != nullptr && object_ptr != nullptr)
		{
			return false;
		}

		ReportPointerDifference(name, object_ptr, other_ptr);

		return true;
	}

	// Negative counts are treated as empty arrays, like FieldVisitor::DynamicArray() does
	template<typename CountType>
	static size_t ToElementCount(CountType count)
	{
		if constexpr (std::is_signed_v<CountType>)
		{
			if (count < 0)
			{
				return 0;
			}
		}

		return static_cast<size_t>(count);
	}

	T& m_object;
	DiffContext& m_context;
	std::shared_ptr<DiffFieldPath> m_path;

	std::vector<std::function<void()>> child_visitors;
};
