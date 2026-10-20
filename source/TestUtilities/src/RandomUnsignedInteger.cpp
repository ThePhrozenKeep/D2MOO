#include "RandomUnsignedInteger.h"


#include <cstdint>
#include <random>


auto random_unsigned_integer(uint32_t min, uint32_t max) -> uint32_t
{
	static std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<uint32_t> distribution(min, max);

	return distribution(generator);
}
