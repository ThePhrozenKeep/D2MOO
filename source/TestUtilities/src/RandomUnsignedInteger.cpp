#include "RandomUnsignedInteger.h"


#include <cstdint>
#include <random>


static std::mt19937& random_generator()
{
	static std::mt19937 generator(std::random_device{}());
	return generator;
}

auto random_unsigned_integer(uint32_t min, uint32_t max) -> uint32_t
{
	std::uniform_int_distribution<uint32_t> distribution(min, max);

	return distribution(random_generator());
}

auto random_signed_integer(int32_t min, int32_t max) -> int32_t
{
	std::uniform_int_distribution<int32_t> distribution(min, max);

	return distribution(random_generator());
}
