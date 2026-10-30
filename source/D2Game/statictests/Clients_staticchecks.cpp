
#include <GAME/Clients.h>
#include <cstddef>

#if defined(D2_VERSION_110F)
static_assert(sizeof(D2ClientStrc) == 0x510, "Size of D2ClientStrc does not match v1.10f");
#elif defined(D2_VERSION_113C)
static_assert(sizeof(D2ClientStrc) == 0x518, "Size of D2ClientStrc does not match v1.13c");
#elif defined(D2_VERSION_114D)
static_assert(sizeof(D2ClientStrc) == 0x518, "Size of D2ClientStrc does not match v1.14d");
static_assert(offsetof(D2ClientStrc, bDisconnectRequested) == 0x504, "");
static_assert(offsetof(D2ClientStrc, nExpLoss) == 0x508, "");
static_assert(offsetof(D2ClientStrc, dwLocale) == 0x50C, "");
static_assert(offsetof(D2ClientStrc, dwLangId) == 0x510, "");
#endif
