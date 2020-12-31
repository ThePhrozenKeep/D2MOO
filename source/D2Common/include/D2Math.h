#pragma once

#include <cstdint>

extern const uint32_t gdwBitMasks[];
extern const uint32_t gdwInvBitMasks[];

template<class T>
constexpr const T& D2Clamp(const T& v, const T& low, const T& high)
{
    return (v < low) ? low : (high < v) ? high : v;
}

template<class T>
T D2Lerp(T v0, T v1, T t) {
    return (1 - t) * v0 + t * v1;
}
