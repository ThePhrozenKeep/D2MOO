#pragma once

// Compiler support shims for the vendored Storm container templates in this
// directory.
//
// These headers are a VC6-compatible reimplementation of the small subset of
// Squall (https://github.com/Lectem/squall) that D2MOO uses. See the README in
// source/Storm/squall-compat.md for why they exist.
//
// Layout and behaviour must stay identical to Squall's: D2MOO inherits these
// classes inside structs whose sizes are asserted against the retail game.

#include <stddef.h>

// VC6 predates <cstdint>. Define the fixed-width types it never shipped.
// Redefining a typedef with an identical underlying type is legal, so this is
// safe even when the SDK's basetsd.h already provided them.
#if defined(_MSC_VER) && _MSC_VER < 1600
typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef signed __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef signed __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
#else
#include <cstdint>
#endif

#include <new>