#pragma once


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
