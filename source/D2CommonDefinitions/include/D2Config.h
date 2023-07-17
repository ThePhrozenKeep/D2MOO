#pragma once

#include <Windows.h>
#include <D2BasicTypes.h>
#include <D2BuildInformation.h>

#pragma pack(push, 1)

// Whole structure is used as string, values are encoded so that they are != 0
struct D2CharacterPreviewInfoStrc 
{
	uint16_t nVersion;					//0x00 lower byte is cleared if invalid data was found => empty string. Otherwise contains FOG_Encode14BitsToString(10)
	uint8_t pComponents[11];			//0x02
	uint8_t nClass;						//0x0D Encoded using +1
	uint8_t pComponentColors[11];		//0x0C
	uint8_t nLevel;						//0x19
	uint16_t nClientFlags;				//0x1A D2ClientSaveFlags. Encoded via FOG_Encode14BitsToString
	uint16_t nGuildFlags;				//0x1C Encoded via FOG_Encode14BitsToString
	uint8_t nGuildEmblemBgColor;		//0x1E
	uint8_t nGuildEmblemFgColor;		//0x1F
	uint8_t nGuildEmblemType;			//0x20 maps to D2DATA.MPQ/data/global/ui/Emblems/icon(nGuildEmblemType-1)a.dc6
	uint32_t szGuildTag;				//0x21
	uint8_t pad0x25;					//0x25
};
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
	char szRealm[24];
	D2CharacterPreviewInfoStrc tCharPreviewInfo;
	char szUnk[194];
	uint8_t _01D0[0x18];
	
	union
	{
		uint32_t dwCTemp;
		struct
		{
			uint8_t nUnk;
			uint8_t nCharacterClassId;		// D2C_PlayerClasses
			uint16_t nCharacterSaveFlags;	// D2PackedClientSaveFlags
		} unpackedCTemp;
	};

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
	uint32_t nArenaFlags; // D2GameFlags
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
