#pragma once

#include <Windows.h>
#include <D2BasicTypes.h>


#define D2_VERSION_EXPANSION 1
#define D2_VERSION_MAJOR 1
#define D2_VERSION_MINOR 10
#define D2_HAS_OPENGL 0
#define D2_HAS_RAVE 0

#define D2_HAS_MULTILAN defined(D2_VERSION_100) // TODO: figure out when this was removed

#pragma pack(push, 1)
struct D2ConfigStrc
{
#if D2_VERSION_EXPANSION // Not present in old versions of the game such as 1.00
	BOOL bIsExpansion;
#endif
	uint8_t bWindow;
	uint8_t b3DFX;
	uint8_t bOpenGL;
	uint8_t bRave;
	uint8_t bD3D;
	uint8_t bPerspective;
	uint8_t bQuality;
	uint32_t dwGamma;
	uint8_t bVSync;
	uint32_t dwFramerate;
	uint32_t dwGameType;
	uint16_t wJoinID; // Dangerous ! We may be overwriting szGameName since code expects a 32bit value not 16!
	char szGameName[24];
	char szServerIP[24];
	char szBattleNetIP[24];
	char szMCPIP[24];
	uint8_t _0076[4];
	uint8_t bNoPK;
	uint8_t bOpenC;
	uint8_t bAmazon;
	uint8_t bPaladin;
	uint8_t bSorceress;
	uint8_t bNecromancer;
	uint8_t bBarbarian;
#if D2_VERSION_EXPANSION
	uint8_t bDruid;
	uint8_t bAssassin;
#endif
	uint8_t bInvincible;
	uint8_t _0082[48];
	char szName[24];
	uint8_t szRealm[256];
	uint8_t _01D0[0x18];
	uint32_t dwCTemp; // Arena mode character template
	uint8_t bNoMonsters;
	uint32_t dwMonsterClass;
	uint8_t bMonsterInfo;
	uint32_t dwMonsterDebug;
	uint8_t bRare;
	uint8_t bUnique;
	uint8_t _01DA[2]; // Possibly Set/Magic
	uint32_t dwAct;
	uint8_t bNoPreload;
	uint8_t bDirect;
	uint8_t bLowEnd;
	uint8_t bNoCompress;
	uint32_t dwArena;
	uint8_t _01E8[6]; // Related to Arena
#ifndef VERSION_100 // TODO: figure out when this was added. Probably in 1.10
	BOOL(*pAllowExpansionCallback)(void);
	uint8_t bTxt;
#endif
	uint8_t bLog;
	uint8_t bMsgLog;
	uint8_t bSafeMode;
	uint8_t bNoSave;
	uint32_t dwSeed;
	uint8_t bCheats;
#ifndef VERSION_100 // TODO: figure out when this was added
	uint8_t bTeen;
#endif
	uint8_t bNoSound;
	uint8_t bQuests;
	uint8_t _01F9;
	
#ifndef VERSION_100 // TODO: figure out when this was added. Probably in 1.10
	uint8_t bBuild;
#endif
	uint32_t dwComInt;
	uint8_t _0x0223[28]; // Related to ComInt
	char szPassword[24];
	uint8_t _0x0257[256];
	uint8_t bSkipToBNet;
	uint8_t _0333[112];
};
#pragma pack(pop)
