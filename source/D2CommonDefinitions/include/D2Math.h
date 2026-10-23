#pragma once

#include <D2StdInt.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
// VC6 has no <type_traits>: no true_type/false_type, no is_signed, no SFINAE.
// Dispatch on `T(-1) < T(0)`, which is true for signed types and false for
// unsigned types.
template <bool Signed> struct D2SignedTag
{
};

template <typename T> int32_t D2signum(T x, D2SignedTag<false>)
{
    return T(0) < x;
}

template <typename T> int32_t D2signum(T x, D2SignedTag<true>)
{
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline int32_t D2signum(T x)
{
    return D2signum(x, D2SignedTag<(T(-1) < T(0))>());
}

#else
#include <type_traits>

template <typename T> inline constexpr
int32_t D2signum(T x, std::false_type is_signed)
{
    return T(0) < x;
}

template <typename T> inline constexpr
int32_t D2signum(T x, std::true_type is_signed)
{
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int32_t D2signum(T x)
{
    return D2signum(x, std::is_signed<T>());
}
#endif

template<class T>
D2_CONSTEXPR_FN const T& D2Clamp(const T& v, const T& low, const T& high)
{
    return (v < low) ? low : (high < v) ? high : v;
}

template<class T>
T D2Lerp(T v0, T v1, T t) {
    return (1 - t) * v0 + t * v1;
}
