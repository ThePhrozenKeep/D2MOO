#pragma once


#include <cstdint>
#include <limits>


auto random_unsigned_integer(uint32_t min = 0, uint32_t max = std::numeric_limits<uint32_t>::max()) -> uint32_t;
auto random_signed_integer(int32_t min = std::numeric_limits<int32_t>::min(), int32_t max = std::numeric_limits<int32_t>::max()) -> int32_t;
