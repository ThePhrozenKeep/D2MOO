#pragma once

#include <cstdint>
#include <type_traits>

template<class T>
constexpr const T& D2Clamp(const T& v, const T& low, const T& high)
{
    return (v < low) ? low : (high < v) ? high : v;
}

template<class T>
T D2Lerp(T v0, T v1, T t) {
    return (1 - t) * v0 + t * v1;
}


template <typename T> inline constexpr
int32_t D2signum(T x, std::false_type is_signed) {
    return T(0) < x;
}

template <typename T> inline constexpr
int32_t D2signum(T x, std::true_type is_signed) {
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int32_t D2signum(T x) {
    return D2signum(x, std::is_signed<T>());
}

