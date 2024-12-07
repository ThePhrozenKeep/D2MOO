#pragma once

// NOLINTBEGIN
// Use this by default if the function will be in an import library
#define D2FUNC_DLL(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV DLL##_##NAME ARGS;

// Same as D2FUNC_DLL but without DLL name prefix
#define D2FUNC_DLL_NP(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV NAME ARGS;


template <typename T>
constexpr auto default_or_void() -> T { return {}; }
template <>
constexpr auto default_or_void<void>() -> void {}

#define D2FUNC_DLL_STUB(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV DLL##_##NAME ARGS {           \
		OutputDebugStringA(#DLL "_" #NAME);                     \
		return default_or_void<RETURN>();                      \
	}													   

// Same as D2FUNC_DLL_STUB but without DLL name prefix
#define D2FUNC_DLL_NP_STUB(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	DLL##_DLL_DECL RETURN CONV NAME ARGS {          \
		OutputDebugStringA(#NAME);                     \
		return default_or_void<RETURN>();             \
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