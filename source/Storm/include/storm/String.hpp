#pragma once

#include "storm/StormCompat.hpp"

// As with storm/Memory.hpp, the retail Storm string entry points are declared in
// D2MOO's own Storm.h.
#include <Storm.h>

#define STORM_MAX_PATH 260
#define STORM_MAX_STR 0x7FFFFFFF

// Storm.#588. Not implemented by D2MOO yet, and only reachable through the
// string-keyed TSHashTable overloads, which D2MOO does not instantiate.
uint32_t SStrHashHT(const char* string);