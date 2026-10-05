#pragma once

// Compiler support shims for the vendored Storm container templates in this
// directory.
//
// These headers are a VC6-compatible reimplementation of the small subset of
// Squall (https://github.com/Lectem/squall) that D2MOO uses.
//
// Layout and behaviour must stay identical to Squall's: D2MOO inherits these
// classes inside structs whose sizes are asserted against the retail game.

#include <stddef.h>

#include <D2StdInt.h>

#include <new>