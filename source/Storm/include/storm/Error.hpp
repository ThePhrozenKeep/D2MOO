#pragma once

#include "storm/StormCompat.hpp"

// As with storm/Memory.hpp, the retail Storm error entry points are declared in
// D2MOO's own Storm.h.
#include <Storm.h>

#if defined(NDEBUG)
#define STORM_ASSERT(x)                          \
	(void)0
#else
// Note: Squall's version also calls SErrPrepareAppFatal here, but D2MOO's Storm.h
// declares that one as (int, int) rather than (const char*, int32_t), so going
// straight to the fatal error display avoids the mismatch.
#define STORM_ASSERT(x)                          \
	if (!(x)) {                                  \
		SErrDisplayAppFatal(#x);                 \
	}                                            \
	(void)0
#endif