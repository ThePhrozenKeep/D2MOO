#pragma once

// Compiler shims for VC6 (Visual C++ 6.0), which ships no constexpr, nullptr,
// static_assert, override/final or __vectorcall, and always gives an unscoped enum a
// four-byte int. Inert on modern compilers. The _MSC_VER test describes the compiler,
// so it is not a build option.
//
// Reached two ways, which #pragma once makes equivalent: <D2StdInt.h> includes it,
// and the VC6 toolchain file force-includes it so vendored and third-party code gets
// it too. It is self-contained for that reason, since such TUs have no D2MOO include
// path. Nothing here replaces a CRT function: where D2MOO calls one, it calls the
// same one retail called.

// VC6 has no constexpr. For a namespace-scope integral constant in a header, `const`
// is an exact stand-in. Not valid for a constexpr function, constructor, or
// odr-used static member.
#if defined(_MSC_VER) && _MSC_VER < 1600
#define D2_CONSTEXPR const
#else
#define D2_CONSTEXPR constexpr
#endif

// Same for a free function, which is only ever called at run time here.
#if defined(_MSC_VER) && _MSC_VER < 1600
#define D2_CONSTEXPR_FN
#else
#define D2_CONSTEXPR_FN constexpr
#endif

// VC6 cannot fix an unscoped enum's underlying type, so it always gives one int --
// four bytes here. Several of these enums are stored in structs whose retail offsets
// assume the narrow width, and an int there shifts every following field. So on VC6
// the name becomes a typedef and the enumerators an anonymous enum; they land in the
// same scope either way, because D2MOO's enums are deliberately unscoped (see the
// 28612 suppression in D2BasicTypes.h). Values above INT_MAX still need a wrapper.
#if defined(_MSC_VER) && _MSC_VER < 1600
#define D2_TYPED_ENUM(NAME, UNDERTYPE) typedef UNDERTYPE NAME; enum
#else
#define D2_TYPED_ENUM(NAME, UNDERTYPE) enum NAME : UNDERTYPE
#endif

// No <sal.h>, so no annotation macros. They are documentation for the analyser and
// nothing more. _Analysis_assume_ must not evaluate its argument, as D2_ASSERT
// already does and would double any side effect.
#if defined(_MSC_VER) && _MSC_VER < 1600
#define _Acquires_lock_(x)
#define _Releases_lock_(x)
#define _Requires_lock_held_(x)
#define _Analysis_assume_(x) ((void)0)
#define _Curr_

// No static_assert until C++11; this is the C++98 check, a typedef over a negative
// array bound. The two-level paste keeps consecutive asserts from colliding, and the
// message is dropped, as C++98 had nowhere to put one.
#define D2_CAT2(a, b) a##b
#define D2_CAT(a, b) D2_CAT2(a, b)
#define static_assert(expr, msg) \
	typedef char D2_CAT(d2_static_assert_, __LINE__)[(expr) ? 1 : -1]

// No nullptr. NULL is 0 in that CRT, and nullptr is only ever used here to initialise
// or compare a pointer.
#define nullptr NULL

// No virt-specifiers; VC6 already derives the override check from the vtable.
#define override
#define final

// __vectorcall is post-VC6 and has no spelling here, so it expands away. Retail
// D2Common.0x6FD68400 was built by VC6, so the default convention is what it meant.
#define __vectorcall

// The MSVC600 SDK is the Windows 95/98 one and predates these; each value is the one
// the retail build compiled against.
#if defined(_MSC_VER) && _MSC_VER < 1600
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 0xFFFF
#endif
#endif

// VC6's <algorithm>/<cmath> spell the C++98 algorithms differently: max and min are
// _cpp_max/_cpp_min, and abs is only global. NOMINMAX keeps these clear of the Win32
// macros. std::size has no shim -- VC6 cannot deduce a non-type template parameter
// from an array-reference parameter -- so D2MOO uses ARRAY_SIZE instead.
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace std
{
using ::abs;

template <class T> inline const T& max(const T& a, const T& b)
{
	return _cpp_max(a, b);
}

template <class T> inline const T& min(const T& a, const T& b)
{
	return _cpp_min(a, b);
}
} // namespace std

// VC6's <climits> is fine; this just spares the many TUs that use INT_MAX and only
// get it transitively on a modern toolchain.
#include <climits>

// Added after VC6 shipped, and modern winbase.h defines it unconditionally, so it
// cannot be #ifdef'd.
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif