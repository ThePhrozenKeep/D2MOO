#pragma once

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

#if defined(_MSC_VER) && _MSC_VER < 1600
#define D2_TYPED_ENUM(NAME, UNDERTYPE) typedef UNDERTYPE NAME; enum
#else
#define D2_TYPED_ENUM(NAME, UNDERTYPE) enum NAME : UNDERTYPE
#endif

#if defined(_MSC_VER) && _MSC_VER < 1600
#define _Acquires_lock_(x)
#define _Releases_lock_(x)
#define _Requires_lock_held_(x)
#define _Analysis_assume_(x) ((void)0)
#define _Curr_

#define static_assert(expr, msg)

#define nullptr NULL

#define override
#define final

#define __vectorcall

#if defined(_MSC_VER) && _MSC_VER < 1600
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 0xFFFF
#endif
#endif

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

#include <climits>

#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif