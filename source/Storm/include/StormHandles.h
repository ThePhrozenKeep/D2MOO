#pragma once
// Note: This actually belongs to Storm.h or each type's respective header.
// However since Storm.h is currently gigantic, we define these handles here to reduce compile time and bloating the global namespace in every TU

#define  DECLARE_STRICT_HANDLE(name)        typedef struct name##__ { int unused; } *name

// Those are actully Storm.dll types
DECLARE_STRICT_HANDLE(HSFILE);
DECLARE_STRICT_HANDLE(HSARCHIVE);
