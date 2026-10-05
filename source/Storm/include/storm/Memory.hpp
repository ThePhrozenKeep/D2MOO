#pragma once

#include "storm/StormCompat.hpp"

// The Storm allocator entry points already live in D2MOO's own Storm.h, with the
// retail ordinals. Squall redeclares its own copies; do not do that here.
#include <D2BasicTypes.h>
#include <Storm.h>