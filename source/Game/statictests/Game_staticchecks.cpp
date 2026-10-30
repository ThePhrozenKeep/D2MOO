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
static_assert(sizeof(D2ConfigStrc) == 0x3C7, "");


static_assert(sizeof(D2CmdArgStrc) == 0x3C, "Check D2CmdArgStrc matches original size");
#elif defined(D2_VERSION_114D)
// Offsets from the 1.14d command line table (Game.exe 0x705040) and the launcher code
static_assert(sizeof(D2CmdArgStrc) == 0x5C, "Check D2CmdArgStrc matches original size");
static_assert(offsetof(D2CmdArgStrc, dwType) == 0x51, "");
static_assert(offsetof(D2CmdArgStrc, dwIndex) == 0x54, "");
static_assert(offsetof(D2CmdArgStrc, dwDefault) == 0x58, "");

static_assert(offsetof(D2ConfigStrc, bIsExpansion) == 0x0, "");
static_assert(offsetof(D2ConfigStrc, bCrashApp) == 0x4, "");
static_assert(offsetof(D2ConfigStrc, bWindow) == 0x8, "");
static_assert(offsetof(D2ConfigStrc, bNoFixedAspect) == 0x9, "");
static_assert(offsetof(D2ConfigStrc, b3DFX) == 0xA, "");
static_assert(offsetof(D2ConfigStrc, bOpenGL) == 0xB, "");
static_assert(offsetof(D2ConfigStrc, bRave) == 0xC, "");
static_assert(offsetof(D2ConfigStrc, bD3D) == 0xD, "");
static_assert(offsetof(D2ConfigStrc, bPerspective) == 0xE, "");
static_assert(offsetof(D2ConfigStrc, bQuality) == 0xF, "");
static_assert(offsetof(D2ConfigStrc, dwGamma) == 0x10, "");
static_assert(offsetof(D2ConfigStrc, bVSync) == 0x14, "");
static_assert(offsetof(D2ConfigStrc, dwFramerate) == 0x15, "");
static_assert(offsetof(D2ConfigStrc, dwGameType) == 0x19, "");
static_assert(offsetof(D2ConfigStrc, wJoinID) == 0x1D, "");
static_assert(offsetof(D2ConfigStrc, szGameName) == 0x1F, "");
static_assert(offsetof(D2ConfigStrc, szServerIP) == 0x37, "");
static_assert(offsetof(D2ConfigStrc, szBattleNetIP) == 0x4F, "");
static_assert(offsetof(D2ConfigStrc, szMCPIP) == 0x67, "");
static_assert(offsetof(D2ConfigStrc, bNoPK) == 0x83, "");
static_assert(offsetof(D2ConfigStrc, bOpenC) == 0x84, "");
static_assert(offsetof(D2ConfigStrc, bAmazon) == 0x85, "");
static_assert(offsetof(D2ConfigStrc, bPaladin) == 0x86, "");
static_assert(offsetof(D2ConfigStrc, bSorceress) == 0x87, "");
static_assert(offsetof(D2ConfigStrc, bNecromancer) == 0x88, "");
static_assert(offsetof(D2ConfigStrc, bBarbarian) == 0x89, "");
static_assert(offsetof(D2ConfigStrc, bInvincible) == 0x8C, "");
static_assert(offsetof(D2ConfigStrc, szName) == 0xBD, "");
static_assert(offsetof(D2ConfigStrc, szRealm) == 0xD5, "");
static_assert(offsetof(D2ConfigStrc, dwCTemp) == 0x1ED, "");
static_assert(offsetof(D2ConfigStrc, bNoMonsters) == 0x1F1, "");
static_assert(offsetof(D2ConfigStrc, dwMonsterClass) == 0x1F2, "");
static_assert(offsetof(D2ConfigStrc, bMonsterInfo) == 0x1F6, "");
static_assert(offsetof(D2ConfigStrc, dwMonsterDebug) == 0x1F7, "");
static_assert(offsetof(D2ConfigStrc, bRare) == 0x1FB, "");
static_assert(offsetof(D2ConfigStrc, bUnique) == 0x1FC, "");
static_assert(offsetof(D2ConfigStrc, dwAct) == 0x1FF, "");
static_assert(offsetof(D2ConfigStrc, bNoPreload) == 0x203, "");
static_assert(offsetof(D2ConfigStrc, bDirect) == 0x204, "");
static_assert(offsetof(D2ConfigStrc, bLowEnd) == 0x205, "");
static_assert(offsetof(D2ConfigStrc, bNoCompress) == 0x206, "");
static_assert(offsetof(D2ConfigStrc, wArena) == 0x207, "");
static_assert(offsetof(D2ConfigStrc, bTxt) == 0x215, "");
static_assert(offsetof(D2ConfigStrc, bLog) == 0x216, "");
static_assert(offsetof(D2ConfigStrc, bMsgLog) == 0x217, "");
static_assert(offsetof(D2ConfigStrc, bSafeMode) == 0x218, "");
static_assert(offsetof(D2ConfigStrc, bNoSave) == 0x219, "");
static_assert(offsetof(D2ConfigStrc, dwSeed) == 0x21A, "");
static_assert(offsetof(D2ConfigStrc, bCheats) == 0x21E, "");
static_assert(offsetof(D2ConfigStrc, bTeen) == 0x21F, "");
static_assert(offsetof(D2ConfigStrc, bNoSound) == 0x220, "");
static_assert(offsetof(D2ConfigStrc, bQuests) == 0x221, "");
static_assert(offsetof(D2ConfigStrc, bBuild) == 0x223, "");
static_assert(offsetof(D2ConfigStrc, bSoundBackground) == 0x224, "");
static_assert(offsetof(D2ConfigStrc, pComInterface) == 0x225, "");
static_assert(offsetof(D2ConfigStrc, bSkipToBNet) == 0x35D, "");
static_assert(offsetof(D2ConfigStrc, nLaunchType) == 0x35E, "");
static_assert(offsetof(D2ConfigStrc, bShownLogo) == 0x35F, "");
static_assert(offsetof(D2ConfigStrc, nAccountPasswordHash) == 0x3C3, "");
static_assert(offsetof(D2ConfigStrc, nAccountPasswordLength) == 0x3C7, "");
static_assert(offsetof(D2ConfigStrc, nSaveFlags) == 0x3CB, "");
static_assert(sizeof(D2ConfigStrc) == 0x3CD, "");
#endif

