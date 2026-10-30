#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>
#include <TestComparator.h>


template<typename T>
struct DynamicArray
{
	T* data;
	int count;
};


#define DYNAMIC_ARRAY_TYPE(TYPE)																										\
	template <>																															\
	struct TestComparator<DynamicArray<TYPE>> {																							\
		static bool compare(DynamicArray<TYPE>& a, DynamicArray<TYPE>& b, const char* context_title, std::string& diff_description)		\
		{																																\
			DiffContext context(context_title);																							\
			DiffVisitor visitor(a, context);																							\
			visitor.CompareWith(b);																										\
			diff_description = context.DifferenceDescription();																			\
			return context.differences.empty();																							\
		}																																\
	};																																	\
	template<>																															\
	struct VisitorTraits<DynamicArray<TYPE>>																							\
	{																																	\
		using Self = DynamicArray<TYPE>;																								\
		template<class Visitor>																											\
		static void Visit(Visitor& visitor, DynamicArray<TYPE>& object)																	\
		{																																\
			visitor.template DynamicArray<&Self::data, &Self::count>(object, "data");													\
			visitor.template Field<&Self::count>(object, "count");																		\
		}																																\
	};