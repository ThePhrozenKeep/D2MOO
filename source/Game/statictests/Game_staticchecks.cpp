#include <D2Config.h>
#include <Main.h>
#include <cstddef>

// Thanks to galaxyhaxz for providing the base to work on ! https://github.com/galaxyhaxz/d2src
#ifdef D2_VERSION_110F
static_assert(offsetof(D2ConfigStrc, szCharacterRealm) == 0x227, "");
static_assert(offsetof(D2ConfigStrc, szGamePassword) == 0x23F, "");

static_assert(offsetof(D2ConfigStrc, bSkipToBNet) == 0x357, "");
static_assert(offsetof(D2ConfigStrc, bShownLogo) == 0x359, "");


static_assert(offsetof(D2ConfigStrc, szCurrentChannelName) == 0x35B, "");
static_assert(offsetof(D2ConfigStrc, szDefaultChannelName) == 0x37B, "");
static_assert(offsetof(D2ConfigStrc, nComponents) == 0x39B, "");
static_assert(offsetof(D2ConfigStrc, nComponentsColors) == 0x3AB, "");
static_assert(offsetof(D2ConfigStrc, nCharacterLevel) == 0x3BB, "");
static_assert(offsetof(D2ConfigStrc, nAccountPasswordHash) == 0x3BD, "");
static_assert(offsetof(D2ConfigStrc, nSaveFlags) == 0x3C5, "");

D2_ASSERT_SIZE_110F(D2ConfigStrc,  0x3C7);
D2_ASSERT_SIZE_110F(D2CmdArgStrc, 0x3C);
#endif

