#pragma once

// For D2_CONSTEXPR, which the default_or_void wrappers below are spelled with.
#include <D2StdInt.h>

// NOLINTBEGIN
// Use this by default if the function will be in an import library
#define D2FUNC_DLL(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV DLL##_##NAME ARGS;

// Same as D2FUNC_DLL but without DLL name prefix
#define D2FUNC_DLL_NP(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV NAME ARGS;


#if defined(_MSC_VER) && _MSC_VER < 1600
// VC6 has no constexpr, no trailing return types and no explicit specialisation of a
// function template (C2556/C2371/C2912); a class template with an explicit void
// specialisation gives the same behaviour. A macro cannot do it, because the call
// site is spelled default_or_void<T>() and a function-like macro needs a '('.
template <typename T> struct D2DefaultOrVoid { static T Get() { return T(); } };
template <> struct D2DefaultOrVoid<void> { static void Get() {} };
#define D2_STUB_RETURN(T) D2DefaultOrVoid<T>::Get()
#else
template <typename T>
D2_CONSTEXPR auto default_or_void() -> T { return {}; }
template <>
D2_CONSTEXPR auto default_or_void<void>() -> void {}
#define D2_STUB_RETURN(T) default_or_void<T>()
#endif

// VC6 has no __declspec(noreturn).
#if defined(_MSC_VER) && _MSC_VER < 1600
#define D2_NORETURN
#else
#define D2_NORETURN __declspec(noreturn)
#endif

#define D2FUNC_DLL_STUB(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV DLL##_##NAME ARGS {           \
		OutputDebugStringA(#DLL "_" #NAME);                     \
		return D2_STUB_RETURN(RETURN);                      \
	}													   

// Void-returning variants. VC6 rejects `return expr;` in a void function (C2562) *and*
// `return;` in a non-void one (C2561), and the preprocessor cannot test RETURN to
// pick a spelling, so void functions use these instead of D2FUNC_DLL[_NP].
#define D2FUNC_DLL_VOID(DLL, NAME, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL void CONV DLL##_##NAME ARGS;

#define D2FUNC_DLL_VOID_STUB(DLL, NAME, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL void CONV DLL##_##NAME ARGS {          \
		OutputDebugStringA(#DLL "_" #NAME);                 \
	}

#define D2FUNC_DLL_NP_VOID(DLL, NAME, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL void CONV NAME ARGS;

#define D2FUNC_DLL_NP_VOID_STUB(DLL, NAME, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL void CONV NAME ARGS {                     \
		OutputDebugStringA(#NAME);                            \
	}

// Same as D2FUNC_DLL_STUB but without DLL name prefix
#define D2FUNC_DLL_NP_STUB(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV NAME ARGS {          \
		OutputDebugStringA(#NAME);                     \
		return D2_STUB_RETURN(RETURN);             \
	}													   


// Useful if you do not have an import library or want to use LoadLibray
#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	typedef RETURN(CONV* DLL##_##NAME##_t) ARGS; \
	static DLL##_##NAME##_t DLL##_##NAME = (DLL##_##NAME##_t)(uintptr_t(delayed##DLL##DllBaseGet()) + (OFFSET));

#define D2VAR(DLL, NAME, TYPE, OFFSET) \
	typedef TYPE DLL##_##NAME##_vt; \
	static DLL##_##NAME##_vt * DLL##_##NAME = (DLL##_##NAME##_vt *)(uintptr_t(delayed##DLL##DllBaseGet()) + (OFFSET));

#define D2PTR(DLL, NAME, TYPE, OFFSET) \
	static TYPE* DLL##_##NAME = (TYPE*)(uintptr_t(delayed##DLL##DllBaseGet()) + (OFFSET));

// NOLINTEND