
#include <GAME/Clients.h>

#if defined(D2_VERSION_110F)
D2_ASSERT_SIZE_110F(D2ClientStrc, 0x510);
#elif defined(D2_VERSION_113C)
static_assert(sizeof(D2ClientStrc) == 0x518, "Size of D2ClientStrc does not match v1.13c");
#endif
