#pragma once

#include <Windows.h>
#include <D2BasicTypes.h>
#include <D2BuildInformation.h>

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
	char szRealm[256];
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
	uint16_t wArena;
	uint32_t nArenaFlags;
	uint8_t nArenaTemplate;
	uint8_t _01E9[2]; // Related to Arena
	uint8_t nArenaDifficulty;
#ifndef VERSION_100 // TODO: figure out when this was added. Probably in 1.10
	BOOL(__stdcall *pAllowExpansionCallback)(void);
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
	struct BnClientInterface* pComInterface;	// Can be set by D2Launch to BnClient.dll's QueryInterface()
	uint32_t nTokenId;							// See D2Client.dll:CONFIG_ApplyNetwork_6FAABBF0
	char szCharacterRealm[24];
	char szGamePassword[24];
	char szGameStatstring[256]; // Description of the game
	uint8_t bSkipToBNet;

	uint8_t nLaunchType;
	uint8_t bShownLogo;
	uint8_t bUnk035A;
	char szCurrentChannelName[32];
	char szDefaultChannelName[32];
	uint8_t nComponents[16];
	uint8_t nComponentsColors[16];
	uint8_t nCharacterLevel;
	uint8_t bLadder;
	uint32_t nAccountPasswordHash;
	uint32_t nAccountPasswordLength;
	uint16_t nSaveFlags;
};
#pragma pack(pop)
