#pragma once

#include <D2BasicTypes.h>

#include <D2Constants.h>

// Use for empty/duplicated functions
#define REMOVE_LATER_Trace FOG_Trace

// Can't concat with GCC/clang as __FUNCTION__ is a variable, not a litteral
#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__) || defined(__clang_analyzer__)
#define UNIMPLEMENTED() D2_ASSERTM(false, __FUNCTION__)
#else
#define UNIMPLEMENTED() D2_ASSERTM(false, __FUNCTION__ "is not implemented yet.")
#endif

#define D2_MAX_PATH 260


