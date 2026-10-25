#pragma once


#include "DynamicArrayCountRegistry.h"


#define BEGIN_VISIT(TYPE)																				\
template <>																								\
struct TestComparator<TYPE> {																			\
	static bool compare(TYPE& a, TYPE& b, const char* context_title, std::string& diff_description)		\
	{																									\
		DiffContext context(context_title);																\
		DiffVisitor visitor(a, context);																\
		visitor.CompareWith(b);																			\
		diff_description = context.DifferenceDescription();												\
		return context.differences.empty();																\
	}																									\
};																										\
template<>																								\
struct VisitorTraits<TYPE>																				\
{																										\
	using Self = TYPE;																					\
    template<class Visitor>																				\
    static void Visit(Visitor& visitor, TYPE& object)													\
    {


#define END_VISIT()																						\
    }																									\
};


#define FIELD(NAME)																						\
    visitor.Field<&Self::NAME>(object, #NAME);


#define OBJECT(NAME)																					\
    visitor.Object<&Self::NAME>(object, #NAME);


#define POINTER(NAME)																					\
    visitor.Pointer<&Self::NAME>(object, #NAME);


#define ARRAY(NAME)																						\
    visitor.Array<&Self::NAME>(object, #NAME);


#define DYNAMIC_ARRAY(NAME, COUNT)																		\
    visitor.DynamicArray<&Self::NAME, &Self::COUNT>(object, #NAME);


// Visits a dynamic array whose count is looked up in the DynamicArrayCountRegistry under KEY.
#define DYNAMIC_ARRAY_KEY(NAME, KEY)																	\
    visitor.template DynamicArrayWithRegisteredCount<&Self::NAME>(object, #NAME, KEY);


#define DYNAMIC_ARRAY_COUNT_CONCAT_IMPL(A, B) A##B
#define DYNAMIC_ARRAY_COUNT_CONCAT(A, B) DYNAMIC_ARRAY_COUNT_CONCAT_IMPL(A, B)

// Statement: registers the current value of COUNT for KEY. Use it at runtime, e.g. inside a test.
#define REGISTER_DYNAMIC_ARRAY_COUNT(KEY, COUNT)														\
	DynamicArrayCountRegistry::Instance().Register(KEY, static_cast<size_t>(COUNT));


// Namespace scope: registers the expression COUNT_EXPR for KEY at static initialization.
// The expression is evaluated lazily each time the array is visited, so it may depend on global state
// which is only initialized later (e.g. loaded data tables).
#define DEFINE_DYNAMIC_ARRAY_COUNT(KEY, COUNT_EXPR)														\
	static const bool DYNAMIC_ARRAY_COUNT_CONCAT(_DynamicArrayCount_Registered_, __LINE__) =			\
		(DynamicArrayCountRegistry::Instance().Register(KEY, DynamicArrayCountRegistry::CountProvider([]() { return static_cast<size_t>(COUNT_EXPR); })), true);


#define OMIT(NAME)																						\
    static_assert(true, "Ignored field: " #NAME);


#define BEGIN_UNION(DISCRIMINATOR)																		\
	switch (object.DISCRIMINATOR)																		\
	{


#define UNION_FIELD(VALUE, NAME)																		\
	case VALUE:																							\
		FIELD(NAME)																						\
		break;


#define UNION_OBJECT(VALUE, NAME)																		\
	case VALUE:																							\
		OBJECT(NAME)																					\
		break;


#define UNION_POINTER(VALUE, NAME)																		\
	case VALUE:																							\
		POINTER(NAME)																					\
		break;


#define UNION_ARRAY(VALUE, NAME)																		\
	case VALUE:																							\
		ARRAY(NAME)																						\
		break;


#define END_UNION()																						\
	}
