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

// Recreating the way D2 applies multiplication overflows.
// Today it would be faster to simply always extend to 64 bits.
inline int D2_ApplyRatio(int32_t nValue, int32_t nMultiplier, int32_t nDivisor)
{
	if (nDivisor)
	{
		if (nValue <= 0x100'000)
		{
			if (nMultiplier <= 0x10'000)
			{
				return nMultiplier * nValue / nDivisor;
			}

			if (nDivisor <= (nMultiplier >> 4))
			{
				return nValue * (nMultiplier / nDivisor);
			}
		}
		else
		{
			if (nDivisor <= (nValue >> 4))
			{
				return nMultiplier * (nValue / nDivisor);
			}
		}

		return ((int64_t)nMultiplier * (int64_t)nValue) / nDivisor;
	}

	return 0;
}

inline int32_t D2_ComputePercentage(int32_t nValue, int32_t nPercentage)
{
	return D2_ApplyRatio(nValue, nPercentage, 100);
}
