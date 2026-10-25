#pragma once


#include <iostream>
#include <string>


#include <TestComparator.h>


template <typename T>
bool moo_check_eq(T& a, T& b, const char* context_title, std::string& diff_description)
{
	using Type = std::remove_cv_t<std::remove_reference_t<T>>;
	if constexpr (std::is_pointer_v<Type>)
	{
		using Comparator = TestComparator<std::remove_pointer_t<Type>>;
		if (a == nullptr && b == nullptr)
		{
			return true;
		}
		else if (a == nullptr || b == nullptr)
		{
			diff_description = std::string(context_title) + " - One of the pointers is null while the other is not.";
			return false;
		}
		return Comparator::compare(*a, *b, context_title, diff_description);
	}
	else
	{
		using Comparator = TestComparator<T>;
		return Comparator::compare(a, b, context_title, diff_description);
	}
}


#define MOO_CHECK_EQ(a, b, context_title)																\
	do {																								\
		std::string diff_description;																	\
		const auto are_objects_equal = moo_check_eq(a, b, context_title, diff_description);				\
		CHECK_MESSAGE(are_objects_equal, diff_description);												\
	} while (0);


// TODO: This can be removed eventually once all types are registered and can use MOO_CHECK_EQ
#define SKIP_MOO_CHECK_EQ(a, b, message)																\
	do {																								\
		(void)a;																						\
		(void)b;																						\
		(void)message;																					\
		std::cerr << "\nUse MOO_CHECK_EQ instead!\n";													\
		CHECK(false);																					\
	} while (0);
