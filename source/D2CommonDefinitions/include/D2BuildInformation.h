#pragma once

#if !defined(D2_VERSION_MAJOR)
#define D2_VERSION_110F
#define D2_VERSION_MAJOR 1
#define D2_VERSION_MINOR 10
#define D2_VERSION_PATCH 'F'
#endif

#if !defined(D2_IS_MONOLITHIC)
# if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
#   define D2_IS_MONOLITHIC TRUE  // Everything is linked statically since 1.14a
# elif D2_VERSION_SHAREWARE
#   define D2_IS_MONOLITHIC TRUE  // But this was also the case for the sharewares !
# else
#   define D2_IS_MONOLITHIC FALSE // By default, we had .DLLs
# endif
#endif

#define D2_VERSION_EXPANSION 1

#define D2_HAS_OPENGL 0
#define D2_HAS_RAVE 0

// TODO: figure out when this was removed
#ifdef D2_VERSION_100
#define D2_HAS_MULTILAN 1
#else
#define D2_HAS_MULTILAN 0
#endif
