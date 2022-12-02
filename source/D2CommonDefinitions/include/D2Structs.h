#pragma once

#include <D2BasicTypes.h>
#include <D2Seed.h>

#include "D2QuestDataEx.h"

using D2UnitGUID = uint32_t;
constexpr D2UnitGUID D2UnitInvalidGUID = (D2UnitGUID)-1;

#pragma pack(1)

struct D2ClientKeySkillStrc;
struct D2MercSaveDataStrc;
struct D2PathPointStrc;
struct D2SaveSkillKeyStrc;
struct D2SeedStrc;


struct D2AiCmdStrc;
struct D2AiControlStrc;
struct D2AiParamStrc;
struct D2AiTableStrc;
struct D2AiTickParamStrc;
struct D2AltDrawStrc;
struct D2AnimSeqStrc;
struct D2ArenaStrc;
struct D2ArenaUnitStrc;
struct D2AttackStrc;
struct D2AutomapCellStrc;
struct D2AutomapLayerStrc;
//struct D2AutoMapLayer2;
struct D2BinFieldStrc;
struct D2BitBufferStrc;
struct D2BnetClientDataStrc;
struct D2BodyLocCoordStrc;
struct D2BuySellTabStrc;
struct D2CallbackStrc;
struct D2CellFileStrc;
struct D2ClientStrc;
//struct D2CollMapStrc;
struct D2CombatStrc;
struct D2CompositStrc;
struct D2CorpseStrc;
struct D2CubeItemStrc;
struct D2CurseStrc;
struct D2DamageStrc;
//struct D2DrlgMiscDataStrc;
struct D2DrlgCoordStrc;
struct D2DrlgCoordsStrc;
struct D2RoomExStrc;
struct D2RoomStrc;
struct D2RoomTileStrc;
struct D2DrlgStrc;
struct D2DrlgTileDataStrc;
struct D2DrlgActStrc;
struct D2DrlgAnimTileGridStrc;
struct D2DrlgBuildStrc;
struct D2DrlgLogicalRoomInfoStrc;
struct D2DrlgDeleteStrc;
struct D2DrlgEnvironmentStrc;
struct D2DrlgFileStrc;
struct D2DrlgTileInfoStrc;
struct D2DrlgLevelStrc;
struct D2DrlgLevelLinkDataStrc;
struct D2DrlgLinkStrc;
struct D2DrlgLinkerParamsStrc;
struct D2DrlgMapStrc;
struct D2DrlgOrthStrc;
struct D2DrlgOutdoorGridStrc;
struct D2DrlgOutdoorInfoStrc;
struct D2DrlgOutdoorRoomStrc;
struct D2DrlgPresetInfoStrc;
struct D2DrlgPresetRoomStrc;
struct D2DrlgRGBStrc;
struct D2DrlgRoomTilesStrc;
struct D2DrlgTileGridStrc;
struct D2DrlgTileLinkStrc;
struct D2DrlgUnitsStrc;
struct D2DrlgVertexStrc;
struct D2DrlgWarpStrc;
struct D2DynamicPathStrc;
struct D2EnvironmentCycleStrc;
struct D2EventListStrc;
struct D2FieldStrc;
struct D2GameStrc;
struct D2GfxCellStrc;
struct D2GfxCellNodeStrc;
struct D2GfxDataStrc;
struct D2GfxHelperStrc;
struct D2GfxLightStrc;
struct D2GfxLightExStrc;
struct D2GfxSettingsStrc;
struct D2HirelingInitStrc;
struct D2HoverTextStrc;
struct D2ImmunityTableStrc;
struct D2InactiveItemNodeStrc;
struct D2InactiveMonsterNodeStrc;
struct D2InactiveUnitListStrc;
struct D2InactiveUnitNodeStrc;
struct D2InteractInfoStrc;
struct D2InventoryStrc;
struct D2InventoryGridStrc;
//struct D2InventoryInfoStrc;
//struct D2InventoryLayoutStrc;
struct D2InventoryNodeStrc;
struct D2ItemCalcStrc;
struct D2ItemCacheStrc;
struct D2ItemExtraDataStrc;
struct D2ItemDataStrc;
struct D2ItemDescDamageStrc;
struct D2ItemDescStatStringTableStrc;
struct D2ItemDropStrc;
struct D2ItemSaveStrc;
struct D2KeyConfigOptionStrc;
struct D2KeyConfigStrc;
struct D2MapAIPathPositionStrc;
struct D2MapAIStrc;
struct D2MenuEntryStrc;
struct D2MenuInfoStrc;
struct D2MenuItemStrc;
struct D2MenuUIStateSaveStrc;
struct D2MercDataStrc;
struct D2MinionListStrc;
struct D2MissileCalcStrc;
struct D2MissileDamageDataStrc;
struct D2MissileDataStrc;
struct D2MissileStrc;
struct D2MissileStreamStrc;
struct D2ModeChangeStrc;
struct D2MonStatsInitStrc;
struct D2MonsterDataStrc;
struct D2MonsterInteractStrc;
struct D2MonRegDataStrc;
struct D2MonsterRegionStrc;
struct D2NpcControlStrc;
struct D2NpcGambleStrc;
struct D2UnitProxyStrc;
struct D2NPCMessageListStrc;
struct D2NPCMessageStrc;
struct D2NPCMessageTableStrc;
struct D2NpcRecordStrc;
struct D2ObeliskPowerUpStrc;
struct D2ObjectRoomCoordStrc;
struct D2ObjectRegionStrc;
struct D2ObjectControlStrc;
struct D2ObjectDataStrc;
struct D2ObjInitFnStrc;
struct D2ObjOperateFnStrc;
struct D2PacketListStrc;
struct D2PacketTableStrc;
struct D2PlayerCountBonusStrc;
struct D2PlayerDataStrc;
struct D2PlayerTradeStrc;
struct D2PresetUnitStrc;
//struct D2PropertySetFuncStrc;
struct D2PropertyAssignStrc;
struct D2pSpellTblStrc;
struct D2QuestArgStrc;
struct D2QuestChainStrc;
struct D2QuestDataStrc;
struct D2QuestInitTableStrc;
struct D2QuestIntroTableStrc;
struct D2QuestInfoStrc;
struct D2QuestTimerStrc;
//struct D2QuestCltStrc;
//struct D2QuestSrvStrc;
struct D2QuestDescStrc;
struct D2QuestDescriptorStrc;
//struct D2QuestRecordStrc;
struct D2QuestUiButtonCoordStrc;
struct D2QuestUiStrc;
struct D2QuestUiTabStrc;
struct D2RenderCallbackStrc;
struct D2ResistanceRecordStrc;
struct D2RoomCollisionGridStrc;
//struct D2RoomCoordStrc;
struct D2RoomCoordListStrc;
struct D2RosterButtonStrc;
struct D2RosterControlStrc;
struct D2RosterCorpseStrc;
struct D2RosterInfoStrc;
struct D2RosterPetStrc;
struct D2RosterUnitStrc;
struct D2SaveHeaderStrc;
struct D2SaveLaunchStrc;
struct D2ServerPacketTableStrc;
struct D2ServerParamStrc;
struct D2ShrineTableStrc;
struct D2SkillCalcStrc;
struct D2SkillListStrc;
struct D2SkillStrc;
struct D2SkillTreeChartStrc;
struct D2SplittedTextStrc;
struct D2StatStrc;
struct D2StatsArrayStrc;
struct D2StatListStrc;
struct D2StatListExStrc;
struct D2StaticPathStrc;
struct D2SummonArgStrc;
struct D2TargetNodeStrc;
struct D2TaskStrc;
struct D2TblHeaderStrc;
struct D2TblNodeStrc;
struct D2TCStackStrc;
struct D2TextHeaderStrc;
struct D2TextNodeStrc;
struct D2TileStrc;
struct D2TileLibraryBlockStrc;
struct D2TileLibraryEntryStrc;
struct D2TileLibraryHashStrc;
struct D2TileLibraryHashNodeStrc;
struct D2TileLibraryHashRefStrc;
struct D2TileLibraryHeaderStrc;
struct D2TileRecordStrc;
struct D2TimerArgStrc;
struct D2TimerQueueStrc;
struct D2TimerStrc;
struct D2UnitStrc;
struct D2UnitFindArgStrc;
struct D2UnitFindDataStrc;
struct D2WarpTileStrc;
struct D2WaypointActTableStrc;
struct D2WaypointCoordTableStrc;
struct D2WaypointDataStrc;
struct D2WaypointFlagTableStrc;
struct D2WaypointTableStrc;
struct D2WeaponDescTblStrc;
struct D2WeaponSpeedTblStrc;
struct D2WindowPlacementStrc;
struct D2WinMsgStrc;


struct D2MonStatsTxt;
struct D2MonStats2Txt;
struct D2LvlWarpTxt;
struct D2DifficultyLevelsTxt;
struct D2LvlMazeTxt;
struct D2LvlPrestTxt;
struct D2ObjectsTxt;
struct D2ShrinesTxt;
struct D2SkillsTxt;
struct D2AnimDataRecordStrc;
struct D2InventoryGridInfoStrc;



////////////////////////////////

struct D2ClientKeySkillStrc
{
	int16_t nSkill;							//0x00
	int16_t __002;							//0x02
	uint32_t dwFlags;							//0x04
};

struct D2MercSaveDataStrc
{
	uint32_t dwFlags;							//0x00		//0xAF of D2SaveHeaderStrc
	uint32_t dwSeed;							//0x04		//0xB3
	uint16_t nName;								//0x08		//0xB7
	uint16_t nType;								//0x0A		//0xB9
	uint32_t dwExperience;						//0x0C		//0xBB
};

struct D2SaveSkillKeyStrc
{
	uint16_t nSkill;							//0x00
	union
	{
		struct
		{
			uint8_t nKey;						//0x02
			uint8_t nKeyEx;					//0x03
		};

		uint16_t nItemSlot;						//0x02
	};
};




struct D2AiCmdStrc
{
	D2AiCmdStrc* pNextCmd;					//0x00
	D2AiCmdStrc* pPrevCmd;					//0x04
	int32_t nCmdParam[5];						//0x08
};

typedef void(__fastcall* AIPARAMFN)(D2GameStrc*, D2UnitStrc*, D2AiTickParamStrc*);

struct D2AiControlStrc
{
	uint32_t dwSpecialState;					//0x00
	AIPARAMFN pAiParamFn;					//0x04
	uint16_t nAiFlags;							//0x08
	uint8_t unk0x0A[2];						//0x0A
	D2UnitGUID dwOwnerGUID;						//0x0C
	uint32_t dwOwnerType;						//0x10
	int32_t dwAiParam[3];						//0x14
	D2AiCmdStrc* pCurrentCmd;				//0x20
	D2AiCmdStrc* pLastCmd;					//0x24
	D2GameStrc* pGame;						//0x28
	D2UnitGUID dwOwnerGUIDEx;				//0x2C
	uint32_t dwOwnerTypeEx;					//0x30
	D2MinionListStrc* pMinionList;			//0x34
	D2MapAIStrc* pMapAi;					//0x38
	//int32_t nTrapType;							//0x38
	int32_t nMinionSpawnClassId;				//0x3C
};

struct D2AiParamStrc
{
	uint32_t unk0x00;							//0x00
	void* pAiParamFn;						//0x04
	uint16_t nAiFlags;							//0x08
	uint8_t unk0x0A[16];						//0x0A
	uint32_t dwMoveMethod;						//0x18
	int32_t nSpeedBonus;						//0x1C
	int32_t nSteps;								//0x20
};

struct D2AiTableStrc
{
	int32_t unk0x00;							//0x00
	AIPARAMFN unk0x04;						//0x04
	AIPARAMFN pAiParamFn;					//0x08
	AIPARAMFN unk0x0C;						//0x0C
};

struct D2AiTickParamStrc
{
	D2AiControlStrc* pAiControl;			//0x00
	uint32_t unk0x04;							//0x04
	D2UnitStrc* pTarget;					//0x08
	uint32_t unk0x0C[2];						//0x0C
	int32_t nTargetDistance;					//0x14
	BOOL bCombat;							//0x18
	D2MonStatsTxt* pMonstatsTxt;			//0x1C
	D2MonStats2Txt* pMonstats2Txt;			//0x20
};

struct D2AltDrawStrc
{
	int32_t nLeft;								//0x000
	int32_t nTop;								//0x004
	int32_t nRight;								//0x008
	int32_t nBottom;							//0x00C
	D2UnitStrc* pItem;						//0x010
	wchar_t wszName[128];					//0x014
	int32_t nRectColor;							//0x114
	int32_t nRectDrawMode;						//0x118
	int32_t nColor;								//0x11C
};

struct D2AnimSeqStrc
{
	uint32_t unk;
};

struct D2AnvilUIButtonStrc
{
	uint16_t nStringId;							//0x00
	int32_t nTextX;								//0x02
	int32_t nTextY;								//0x06
	int32_t nCellfileX;							//0x0A
	int32_t nCellfileY;							//0x0E
	int32_t nClickAreaXStart;					//0x12
	int32_t nClickAreaXEnd;						//0x16
	int32_t nClickAreaYStart;					//0x1A
	int32_t nClickAreaYEnd;						//0x1E
	int32_t nFrame;								//0x22
	uint8_t bButtonClicked;					//0x26
	uint8_t unk0x27;							//0x27
};


struct D2ArenaStrc
{
	int32_t nAlternateStartTown;				//0x00
	int32_t nType;								//0x04
	uint32_t fFlags;							//0x08
	int32_t nTemplate;							//0x0C - uint8_t with 3 pad
};

struct D2ArenaUnitStrc
{
	int32_t nScore;								//0x00
	BOOL bUpdateScore;						//0x04 
};

struct D2AttackStrc
{
	D2GameStrc* pGame;						//0x00
	D2DifficultyLevelsTxt* pTxt;			//0x04
	D2UnitStrc* pAttacker;					//0x08
	D2UnitStrc* pDefender;					//0x0C
	uint32_t dwAttackerType;					//0x10
	uint32_t dwDefenderType;					//0x14
	D2DamageStrc* pDamage;					//0x18
	uint32_t unk0x1C;							//0x1C
	int32_t nDamageReduction;					//0x20
	int32_t nMagicDamageReduction;				//0x24
};

struct D2AutomapCellStrc
{
	uint32_t fSaved;							//0x00
	uint16_t nCellNo;							//0x04
	uint16_t xPixel;							//0x06
	uint16_t yPixel;							//0x08
	uint16_t wWeight;							//0x0A
	D2AutomapCellStrc* pPrev;				//0x0C
	D2AutomapCellStrc* pNext;				//0x10
};

struct D2AutomapLayerStrc
{
	uint32_t nLayerNo;							//0x00
	uint32_t fSaved;							//0x04
	D2AutomapCellStrc* pFloors;				//0x08
	D2AutomapCellStrc* pWalls;				//0x0C
	D2AutomapCellStrc* pObjects;			//0x10
	D2AutomapCellStrc* pExtras;				//0x14
	D2AutomapLayerStrc* pNext;				//0x18
};
//
//struct D2AutoMapLayer2
//{
//	uint32_t _1[2];							//0x00
//	uint32_t nLayerNo;							//0x08
//};


struct D2BnetClientDataStrc
{
	uint32_t dwExpansion;						//0x000
	uint8_t nWindowed;							//0x004
	uint8_t nFixAspectRatio;					//0x005
	uint8_t n3DFXMode;							//0x006
	uint8_t nOpenGLMode;						//0x007
	uint8_t nRaveMode;							//0x008
	uint8_t nDirect3DMode;						//0x009
	uint8_t nUsePerspective;					//0x00A
	uint8_t nLowQuality;						//0x00B
	uint32_t dwGamma;							//0x00C
	uint8_t nVSync;							//0x010
	uint32_t dwFrameRate;						//0x011
	uint32_t dwGameType;						//0x015
	uint16_t nJoinId;							//0x019
	char szGameName[24];					//0x01B
	char szServerAddress[24];				//0x033
	char szBattleNetAddress[24];			//0x04B
	char szMCPAddress[24];					//0x063
	uint32_t __07B;							//0x07B
	uint8_t nNoPK;								//0x07F
	uint8_t nOpenCharacter;					//0x080
	uint8_t nClasses[7];						//0x081
	uint8_t nInvincible;						//0x088
	char szAccount[48];						//0x089
	char szCharName[24];					//0x0B9
	char szRealmName[32];					//0x0D1
	uint8_t __0F1[249];						//0x0F1
	uint8_t nCharClass;						//0x1EA
	uint8_t nCharFlags;						//0x1EB
	uint8_t nLastCompleteDiff;					//0x1EC
	uint8_t nNoMonsters;						//0x1ED
	uint8_t __1ED[23];							//0x1EE
	//uint8_t bDirect;							//0x200
	//uint8_t bNoCompress;						//0x202
	uint32_t dwArenaFlags;						//0x205
	uint8_t nTemplate;							//0x209
	uint16_t __20A;								//0x20A
	uint8_t nDifficulty;						//0x20C
	uint8_t __20D[52];							//0x20D
	//uint8_t bNoSound;						//0x21C
	char szGamePassword[24];				//0x241
	char szGameDesc[32];					//0x259
	uint8_t __279[226];						//0x279
	char szChannel[32];						//0x35B
	uint8_t __37B[64];							//0x37B
	uint8_t nCharacterLevel;					//0x3BB
	uint8_t nLadder;							//0x3BC
	uint32_t dwPasswordHash;					//0x3BD
	uint8_t nPasswordLength;					//0x3C1
	uint8_t __3C2[6];							//0x3C2
};

struct D2BodyLocCoordStrc
{
	int32_t nX;
	int32_t nXEnd;
	int32_t nY;
	int32_t nYEnd;
	char nRectangleSizeX;
	char nRectangleSizeY;
	int16_t unk0x12;
};

struct D2BuySellTabStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	uint16_t nStringIndex;						//0x06
	BOOL bSelected;							//0x0A
	BOOL bActive;							//0x0E
};

struct D2CallbackStrc
{
	int32_t nType;								//0x00
	uint32_t uMessage;							//0x04 - WM_XXX
	void* pfCallback;						//0x08
};

struct D2CellFileStrc
{
	uint32_t dwVersion;						//0x00
	struct 
	{
		uint16_t dwFlags;
		uint8_t mylastcol;
		uint8_t mytabno:1;
	};										//0x04
	uint32_t eFormat;							//0x08
	uint32_t dwTermination;					//0x0C
	int32_t nDirections;						//0x10
	int32_t nFrames;							//0x14
	D2GfxCellStrc* pGfxCells;				//0x18
};

struct D2ClientInfoStrc
{
	D2ClientInfoStrc* pSelf;				//0x00
	int32_t dwClientId;							//0x04
	uint32_t nFlags;					//0x08
	const char* szKickMessage;				//0x0C
	int32_t unk0x10;							//0x10
	uint32_t dwLastPacketResetTick;		//0x14
	uint32_t nPacketsPerSecond;			//0x18
	uint32_t dwNewGameTick;				//0x1C
	uint32_t dwRemoveTick;				//0x20
	int32_t unk0x24;							//0x24
	int32_t unk0x28;							//0x28
	uint32_t dwHackDetectionPacketTick;	//0x2C
	uint32_t nACDataCount;				//0x30
	int32_t unk0x34;							//0x34
	int32_t unk0x38;							//0x38
	uint32_t unk0x3C;					//0x3C
};

struct D2ClientStrc
{
	uint32_t dwClientId;						//0x00
	uint32_t dwClientState;					//0x04
	uint8_t unk0x08[2];						//0x08
	uint16_t nFlags;							//0x0A
	uint8_t unk0x0C;							//0x0C
	char szName[16];						//0x0D
	char szAccount[16];						//0x1D
	uint8_t unk0x2D[59];						//0x2D
	D2ClientInfoStrc* pClientInfo;			//0x68
	uint8_t unk0x6C[256];						//0x6C
	uint32_t dwUnitType;						//0x16C
	D2UnitGUID dwUnitGUID;						//0x170
	D2UnitStrc* pPlayer;					//0x174
	uint32_t __178;							//0x178
	D2SaveHeaderStrc* pSaveHeader;			//0x17C
	int32_t nSaveHeaderSize;					//0x180
	uint32_t unk0x184[9];						//0x184
	D2GameStrc* pGame;						//0x1A8
	uint8_t nAct;								//0x1AC
	uint8_t pad0x1AD[3];						//0x1AD
	uint32_t __1B0;							//0x1B0
	D2RoomStrc* pRoom;						//0x1B4
	void* pPacketData[3];					//0x1B8
	uint32_t __1C4[132];						//0x1C4
	uint32_t dwFlags;							//0x3D4
	uint32_t dwLastPacketTick;					//0x3D8
	D2ClientKeySkillStrc HotkeySkills[16];	//0x3DC
	BOOL bSwitchWeapon;						//0x45C
	uint32_t __45C[11];						//0x45C
	uint32_t dwInactivityTime;					//0x48C
	uint16_t nHitpoints;						//0x490
	uint16_t nManaPoints;						//0x492
	uint16_t nStaminaPoints;					//0x494
	uint8_t nPotionLifePercent;				//0x496
	uint8_t nPotionManaPercent;				//0x497
	uint16_t nPosX;								//0x498
	uint16_t nPosY;								//0x49A
	uint16_t nTargetOffsetX;					//0x49C
	uint16_t nTargetOffsetY;					//0x49E
	uint32_t dwBeltGold;						//0x4A0
	uint32_t dwExperience;						//0x4A4
	D2ClientStrc* pNext;					//0x4A8
	D2ClientStrc* pListNext;				//0x4AC
	D2ClientStrc* pNextByName;				//0x4B0
	uint32_t __4B4[19];						//0x4B4
	uint32_t dwSentPings;						//0x500
	int32_t nExpLoss;							//0x504
	uint32_t dwExpLost;						//0x508
	uint32_t dwLangId;							//0x50C
	uint32_t __510[2];							//0x510
};

//struct D2CollMapStrc 
//{
//	uint32_t dwPosGameX;						//0x00
//	uint32_t dwPosGameY;						//0x04
//	uint32_t dwSizeGameX;						//0x08
//	uint32_t dwSizeGameY;						//0x0C
//	uint32_t dwPosRoomX;						//0x10
//	uint32_t dwPosRoomY;						//0x14
//	uint32_t dwSizeRoomX;						//0x18
//	uint32_t dwSizeRoomY;						//0x1C
//	uint16_t* pMapStart;						//0x20
//	uint16_t* pMapEnd;							//0x24
//};

struct D2DamageStrc
{
	uint32_t dwHitFlags;						//0x00
	uint16_t wResultFlags;						//0x04
	uint16_t wExtra;							//0x06
	uint32_t dwPhysDamage;						//0x08
	uint32_t dwEnDmgPct;						//0x0C
	uint32_t dwFireDamage;						//0x10
	uint32_t dwBurnDamage;						//0x14
	uint32_t dwBurnLen;						//0x18
	uint32_t dwLtngDamage;						//0x1C
	uint32_t dwMagDamage;						//0x20
	uint32_t dwColdDamage;						//0x24
	uint32_t dwPoisDamage;						//0x28
	uint32_t dwPoisLen;						//0x2C
	uint32_t dwColdLen;						//0x30
	uint32_t dwFrzLen;							//0x34
	uint32_t dwLifeLeech;						//0x38
	uint32_t dwManaLeech;						//0x3C
	uint32_t dwStamLeech;						//0x40
	uint32_t dwStunLen;						//0x44
	uint32_t dwAbsLife;						//0x48
	uint32_t dwDmgTotal;						//0x4C
	uint32_t unk0x50;							//0x50
	uint32_t dwPiercePct;						//0x54
	uint32_t dwDamageRate;						//0x58
	uint32_t unk0x5C;							//0x5C
	uint32_t dwHitClass;						//0x60
	uint8_t nHitClassActiveSet;				//0x64
	char nConvType;							//0x65
	uint8_t unk0x66[2];						//0x66
	uint32_t dwConvPct;						//0x68
	uint8_t unk0x6C[4];						//0x6C
};

struct D2CombatStrc
{
	D2GameStrc* pGame;						//0x00
	uint32_t dwAttackerType;				//0x04
	uint32_t dwAttackerId;					//0x08
	uint32_t dwDefenderType;				//0x0C
	uint32_t dwDefenderId;					//0x10
	D2DamageStrc tDamage;					//0x14
	D2CombatStrc* pNext;					//0x84
};


struct D2CorpseStrc
{
	uint32_t unk0x00;							//0x00
	uint32_t dwUnitId;							//0x04
	uint32_t unk0x08;							//0x08
	D2CorpseStrc* pNextCorpse;				//0x0C
};

struct D2CubeItemStrc
{
	D2UnitStrc* pItem;						//0x00
	int32_t nClassId;							//0x04
	int32_t nItemLevel;							//0x08
};

typedef BOOL(__fastcall* SPECIALCUBEFN)(D2GameStrc*, D2UnitStrc*);

struct D2CubeTableStrc
{
	SPECIALCUBEFN pFunc;					//0x00
};

struct D2CurseStrc
{
	D2UnitStrc* pUnit;						//0x00
	D2UnitStrc* pTarget;					//0x04
	int32_t nSkill;								//0x08
	int32_t nSkillLevel;						//0x0C
	int32_t nDuration;							//0x10
	int32_t nStat;								//0x14
	int32_t nStatValue;							//0x18
	int32_t nState;								//0x1C
	void* pStateFunc;						//0x20
};

//struct D2DrlgMiscDataStrc
//{
//	uint32_t unk0x00[5];						//0x00
//	uint32_t dwFlags;							//0x14
//	int32_t nDirection;							//0x18
//	uint32_t unk0x1C[36];						//0x1C
//	char nActNo;							//0xAC
//	uint8_t pad0xAD[3];						//0xAD
//	uint32_t unk0xB0[2];						//0xB0
//	D2DrlgMiscDataStrc* unk0xB8;			//0xB8
//	D2DrlgMiscDataStrc* unk0xBC;			//0xBC
//	uint32_t unk0xC0[11];						//0xC0
//};

struct D2EventListStrc
{
	char nEvent;							//0x00
	char pad0x01;							//0x01
	char nFlags;							//0x02
	char pad0x03;							//0x03
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
	int32_t unk0x0C;							//0x0C
	int32_t unk0x10;							//0x10
	void* pfEventFunc;						//0x14
	D2EventListStrc* pPrev;					//0x18
	D2EventListStrc* pNext;					//0x1C
};

struct D2FieldStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
};

struct D2GameStrc
{
	uint32_t __0000[4];						//0x00
	D2GameStrc* pNext;						//0x10
	uint32_t __0014;							//0x14
	LPCRITICAL_SECTION lpCriticalSection;	//0x18
	void* pMemoryPool;						//0x1C
	uint32_t __0020[2];						//0x20
	uint16_t nServerToken;						//0x28
	char szGameName[16];					//0x2A
	char szGamePassword[16];				//0x3A
	char szGameDesc[32];					//0x4A
	uint8_t nGameType;							//0x6A
	uint16_t __006B;							//0x6B
	uint8_t nDifficulty;						//0x6D
	uint16_t __006E;							//0x6E
	BOOL bExpansion;						//0x70
	uint32_t dwGameType;						//0x74
	uint16_t wItemFormat;						//0x78
	uint16_t unk0x7A;							//0x7A
	uint32_t dwInitSeed;						//0x7C
	uint32_t dwObjSeed;						//0x80
	uint32_t InitSeed;							//0x84
	D2ClientStrc* pClientList;				//0x88
	uint32_t nClients;							//0x8C
	uint32_t dwSpawnedPlayers;					//0x90
	uint32_t dwSpawnedMonsters;				//0x94
	uint32_t dwSpawnedObjects;					//0x98
	uint32_t dwSpawnedMissiles;				//0x9C
	uint32_t dwSpawnedItems;					//0xA0
	uint32_t dwSpawnedTiles;					//0xA4
	uint32_t dwGameFrame;						//0xA8
	uint32_t unk0xAC[3];						//0xAC
	D2TimerQueueStrc* pTimerQueue;			//0xB8
	D2DrlgActStrc* pAct[5];					//0xBC
	D2SeedStrc pGameSeed;					//0xD0
	D2InactiveUnitListStrc* pInactiveUnitList[5];//0xD8
	uint32_t dwMonSeed;						//0xEC
	D2MonsterRegionStrc* pMonReg[1024];		//0xF0
	D2ObjectControlStrc* pObjectControl;	//0x10F0
	D2QuestInfoStrc* pQuestControl;			//0x10F4
	D2TargetNodeStrc* pUnitNodes[10];		//0x10F8
	D2UnitStrc* pUnitList[5][128];			//0x1120
	void* pTileList;						//0x1B20
	uint32_t dwUniqueFlags[128];				//0x1B24
	D2NpcControlStrc* pNpcControl;			//0x1D24
	D2ArenaStrc* pArenaCtrl;				//0x1D28
	void* pPartyControl;					//0x1D2C
	uint8_t nBossFlagList[64];					//0x1D30
	uint32_t dwMonModeData[17];				//0x1D70
	uint32_t nMonModeData;						//0x1DB4
	uint32_t unk0x1DB8[3];						//0x1DB8
	uint32_t nSyncTimer;						//0x1DC4
};

struct D2GfxCellStrc
{
	BOOL bFlip;								//0x00
	uint32_t dwWidth;							//0x04
	uint32_t dwHeight;							//0x08
	int32_t nXOffset;							//0x0C
	int32_t nYOffset;							//0x10
	uint32_t unk0x14;							//0x14
	D2GfxCellNodeStrc* pCellNode;			//0x18
	uint32_t dwLength;							//0x1C
	uint8_t* pPixels;							//0x20
};

struct D2GfxCellNodeStrc
{
	uint32_t unk;
};

struct D2GfxDataStrc
{
	D2GfxCellStrc* pCurrentCell;			//0x00
	D2CellFileStrc* pCellFile;				//0x04
	uint32_t nFrame;							//0x08
	uint32_t nDirection;						//0x0C
	int32_t nMaxDirections;						//0x10
	int32_t nMaxFrames;							//0x14
	uint32_t fFlags;							//0x18
	uint8_t fState;							//0x1C
	union									//0x1D
	{
		uint8_t nComponent;			
		uint8_t fItemFlags;			
	};
	uint8_t unk0x1E;							//0x1E - padding no doubt
	uint8_t unk0x1F;							//0x1F
	int32_t nUnitType;							//0x20
	int32_t nUnitIndex;							//0x24
	int32_t nMode;								//0x28
	int32_t nOverlay;							//0x2C
	union
	{
		struct
		{
			char szToken[4];				//0x30
			char szComponent[4];			//0x34
			char szArmorType[4];			//0x38 - lit, med, hvy
			char szMode[4];					//0x3C
			char szHitClass[4];				//0x40
		};
		uint32_t dwName[5];					//0x30
	};
	char* pName;							//0x44
};

struct D2GfxHelperStrc
{
	void(__fastcall *pfFillYBufferTable)(void *ppvBits, int32_t nWidth, int32_t nHeight, int32_t a4);
	void(__fastcall *pfDrawVisTile)(int32_t a1, int32_t a2, int32_t a3, int32_t a4);
	void(__fastcall *field_8)(int32_t a1, int32_t a2, int32_t a3, int32_t a4);
	void(__fastcall *pfDrawGroundTile)(int32_t a1, int32_t a2, int32_t a3, int32_t a4);
	void(__fastcall *pfDrawWallTile)(int32_t a1, int32_t a2, int32_t a3, int32_t a4);
	void(__fastcall *pfDrawBlendedVisTile)(int32_t a1, int32_t a2, int32_t a3, int32_t a4);
	void(__fastcall *pfDrawRoofTile)(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5);
};

struct D2GfxLightStrc
{
	uint8_t nIntensity;						//0x00
	uint8_t nRed;								//0x01
	uint8_t nGreen;							//0x02
	uint8_t nBlue;								//0x03
};

struct D2GfxLightExStrc
{
	uint8_t nIntensity;						//0x00
	uint8_t nRed;								//0x01
	uint8_t nGreen;							//0x02
	uint8_t nBlue;								//0x03
	int32_t nX;									//0x04
	int32_t nY;									//0x08
};

struct D2GfxSettingsStrc
{
	BOOL bPerspectiveEnabled;				//0x00
	BOOL bPerspectiveCapable;				//0x04
	BOOL bLowQuality;						//0x08
	int32_t nGamma;								//0x0C
	BOOL bVSync;							//0x10
	BOOL bBlendedShadows;					//0x14
};

struct D2ImmunityTableStrc
{
	int32_t nStat;								//0x00
	int16_t nStringId;						//0x04
	int32_t nColor;								//0x06
};

struct D2InactiveItemNodeStrc
{
	D2InactiveItemNodeStrc* pNext;			//0x00
	int32_t nFrame;								//0x04
	int32_t nOwnerId;							//0x08
	uint16_t nBitstreamSize;					//0x0C
	uint8_t pBitstream;						//0x0E - dynamic size dependent on item serialization
};

struct D2InactiveMonsterNodeStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	int32_t nClassId;							//0x08
	int32_t nUnitId;							//0x0C
	int32_t nUnitFlags;							//0x10
	int32_t nUnitFlagsEx;						//0x14
	int32_t nTypeFlags;							//0x18
	int32_t unk0x1C;							//0x1C
	D2MinionListStrc* pMinionList;			//0x20
	D2MapAIStrc* pMapAI;					//0x24
	int32_t nSpecialAiState;					//0x28
	int32_t nLevelId;							//0x2C
	uint16_t nNameSeed;							//0x30
	uint8_t nMonUMods[9];						//0x32
	uint8_t unk0x3B;							//0x3B
	uint16_t nBossHcIdx;						//0x3C
	uint16_t unk0x3E;							//0x3E
	int32_t nExperience;						//0x40
	int32_t nMaxHitpoints;						//0x44
	int32_t nHitpoints;							//0x48
	int32_t nCmdParam1;							//0x4C
	int32_t nCmdParam2;							//0x50
	int32_t nGameFrame;							//0x54
	D2InactiveMonsterNodeStrc* pNext;		//0x58
};

struct D2InactiveUnitListStrc
{
	int32_t nX;											//0x00
	int32_t nY;											//0x04
	D2InactiveItemNodeStrc* pInactiveItems;			//0x08
	D2InactiveMonsterNodeStrc* pInactiveMonsters;	//0x0C
	D2InactiveUnitNodeStrc* pInactiveUnits;			//0x10
	D2InactiveUnitListStrc* pNext;					//0x14
};

struct D2InactiveUnitNodeStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	int32_t nUnitType;							//0x08
	int32_t nClassId;							//0x0C
	int32_t nAnimMode;							//0x10
	int32_t nGameFrame;							//0x14
	int32_t nUnitFlags;							//0x18
	int32_t nUnitFlagsEx;						//0x1C
	int32_t nUnitId;							//0x20
	int32_t nFrame;								//0x24
	uint8_t nInteractType;						//0x28
	uint8_t unk0x29;							//0x29
	uint16_t unk0x2A;							//0x2A
	int32_t nDropItemCode;						//0x2C
	D2InactiveUnitNodeStrc* pNext;			//0x30
};

struct D2InteractInfoStrc
{
	D2UnitStrc* pUnit;						//0x00
	int32_t nInteract;							//0x04
	D2InteractInfoStrc* pNext;				//0x08
};



//struct D2InventoryInfoStrc
//{
//	int32_t nLocation;							//0x00
//	int32_t nMaxXCells;							//0x04
//	int32_t nMaxYCells;							//0x08
//};

//struct D2InventoryLayoutStrc
//{
//	uint8_t nSlotWidth;						//0x00
//	uint8_t nSlotHeight;						//0x01
//	uint8_t unk1;								//0x02
//	uint8_t unk2;								//0x03
//	uint32_t dwLeft;							//0x04
//	uint32_t dwRight;							//0x08
//	uint32_t dwTop;							//0x0C
//	uint32_t dwBottom;							//0x10
//	uint8_t nSlotPixelWidth;					//0x14
//	uint8_t nSlotPixelHeight;					//0x15
//};

struct D2InventoryNodeStrc
{
	int32_t nItemId;							//0x00
	D2InventoryNodeStrc* pNext;				//0x04
};

struct D2ItemCacheStrc
{
	char nMin;								//0x00
	char nMax;								//0x01
	char nMagicMin;							//0x02
	char nMagicMax;							//0x03
	uint32_t dwCode;							//0x04
	int32_t nMagicLevel;						//0x08
};


struct D2ItemDescDamageStrc
{
	int32_t nMin;								//0x00
	int32_t nMax;								//0x04
	int32_t nLength;							//0x08
	int32_t nCount;								//0x0C
	BOOL bBonus;							//0x10
	int32_t unk0x14;							//0x14
};

struct D2ItemDescStatStringTableStrc
{
	int32_t nStatId;							//0x00
	int32_t nDescFunc;							//0x04
	uint16_t wPosStringId;						//0x08
	uint16_t wNegStringId;						//0x0A
	int32_t nDescVal;							//0x0C
};

struct D2ItemDropStrc
{
	D2UnitStrc* pUnit;						//0x00
	D2SeedStrc* pSeed;						//0x04
	D2GameStrc* pGame;						//0x08
	int32_t nItemLvl;							//0x0C
	uint32_t unk0x10;							//0x10
	int32_t nId;								//0x14
	int32_t nSpawnType;							//0x18 [3 for ground spawn, 4 for inv spawn]
	int32_t nX;									//0x1C
	int32_t nY;									//0x20
	D2RoomStrc* pRoom;						//0x24
	uint16_t wUnitInitFlags;					//0x28
	uint16_t wItemFormat;						//0x2A [ptGame0x0x78]
	BOOL bForce;							//0x2C
	int32_t nQuality;							//0x30
	int32_t nQuantity;							//0x34
	int32_t nMinDur;							//0x38
	int32_t nMaxDur;							//0x3C
	int32_t nItemIndex;							//0x40
	uint32_t dwFlags1;							//0x44 - itemflag override (used when force is true)
	uint32_t dwSeed;							//0x48 - overrides the seed, used when force is true
	uint32_t dwItemSeed;						//0x4C - overrides the item seed when force is true
	int32_t eEarLvl;							//0x50
	int32_t nQtyOverride;						//0x54
	char szName[16];						//0x58
	int32_t nPrefix[3];							//0x68
	int32_t nSuffix[3];							//0x74
	uint32_t dwFlags2;							//0x80
};


struct D2KeyConfigOptionStrc
{
	uint16_t nStringIndex;						//0x00
	void* pCallback;						//0x02
	int32_t nColor;								//0x06
	int32_t unk0x0A[4];							//0x0A
};

struct D2KeyConfigStrc
{
	int32_t nConfig;							//0x00
	uint16_t nStringIndex;						//0x04
	int32_t unk0x06;							//0x06
};

struct D2MapAIPathPositionStrc
{
	int32_t nMapAIAction;						//0x00
	int32_t nX;									//0x04
	int32_t nY;									//0x08
};

struct D2MapAIStrc
{
	int32_t nPathNodes;							//0x00
	D2MapAIPathPositionStrc* pPosition;		//0x04
};


typedef int32_t(__fastcall* MENUENABLED)();
typedef void(__fastcall* MENUSELECT)(D2MenuItemStrc*, D2WinMsgStrc*);
typedef void(__fastcall* MENUOPTION)(D2MenuItemStrc*, int32_t);
typedef void(__fastcall* MENUUPDATE)(D2MenuItemStrc*, int32_t);

struct D2MenuEntryStrc
{
	int32_t nType;								//0x00
	int32_t nLeft;								//0x04
	int32_t nTop;								//0x08
	int32_t nWidth;								//0x0C
	int32_t nHeight;							//0x10
	int32_t __014;								//0x14
	int32_t nStrIndex;							//0x18
	D2GfxDataStrc* pGfxData;				//0x1C
	int32_t (__stdcall* pBtnFunction)(void*);	//0x20
	uint32_t __024[2];							//0x24
	int32_t nFont;								//0x2C
};

struct D2MenuInfoStrc
{
	int32_t nItemCount;							//0x00
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
	int32_t unk0x0C;							//0x0C
	int32_t unk0x10;							//0x10
	int32_t unk0x14;							//0x14
};

struct D2MenuItemStrc
{
	uint32_t dwType;							//0x00
	BOOL bExpansion;						//0x04
	int32_t nHeight;							//0x08
	char szImage[260];						//0x0C
	MENUENABLED pfEnabled;					//0x110
	MENUSELECT pfSelect;					//0x114			
	MENUOPTION pfOption;					//0x118
	MENUUPDATE pfUpdate;					//0x11C
	uint32_t dwCount;							//0x120
	uint32_t dwMoveCount;						//0x124
	uint32_t dwSliderType;						//0x128
	char szChoices[4][260];					//0x12C
	D2CellFileStrc* pImage;					//0x53C
	D2CellFileStrc* pChoices[4];			//0x540
};

struct D2MenuUIStateSaveStrc
{
	BOOL bCloseWhenOpen;					//0x00
	BOOL bSaveUIState;						//0x04
	int32_t nUIState;							//0x08
};

struct D2MercDataStrc
{
	int16_t nMercName;						//0x00
	int16_t nPad;								//0x02
	uint32_t dwSeed;							//0x04
	BOOL bHired;							//0x08
	BOOL bAvailable;						//0x0C
};

struct D2MinionListStrc
{
	D2UnitGUID dwMinionGUID;						//0x00
	D2MinionListStrc* pNext;				//0x04
};

struct D2ModeChangeStrc
{
	int32_t nMode;								//0x00
	D2UnitStrc* pUnit;						//0x04
	D2UnitStrc* pTargetUnit;				//0x08
	int32_t nX;									//0x0C
	int32_t nY;									//0x10
	uint8_t unk0x14[4];						//0x14
	int32_t unk0x18;							//0x18
	int32_t unk0x1C;							//0x1C
};

struct D2MonRegDataStrc
{
	uint16_t nMonHcIdx;							//0x00
	uint8_t nRarity;							//0x02
	uint8_t unk0x03[49];						//0x03
};

struct D2MonsterRegionStrc
{
	uint8_t nAct;								//0x00
	uint8_t unk0x01[3];						//0x01
	int32_t unk0x04;							//0x04
	uint8_t unk0x08[4];						//0x08
	int32_t unk0x0C;							//0x0C
	uint8_t nMonCount;							//0x10
	uint8_t nTotalRarity;						//0x11
	uint8_t nSpawnCount;						//0x12
	uint8_t unk0x13;							//0x13
	D2MonRegDataStrc pMonData[13];			//0x14
	uint32_t dwMonDen;							//0x2B8
	uint8_t nBossMin;							//0x2BC
	uint8_t nBossMax;							//0x2BD
	uint8_t nMonWander;						//0x2BE
	uint8_t unk0x2BF;							//0x2BF
	uint32_t dwlevel;							//0x2C0
	uint32_t unk0x2C4;							//0x2C4
	uint32_t dwUniqueCount;					//0x2C8
	uint32_t dwMonSpawnCount;					//0x2CC
	uint32_t dwMonKillCount;					//0x2D0
	int32_t unk0x2D4;							//0x2D4
	uint8_t nQuest;							//0x2D8
	uint8_t unk0x2D9[3];						//0x2D9
	uint32_t dwDungeonLevel;					//0x2DC
	uint32_t dwDungeonLevelEx;					//0x2E0
};

struct D2NpcControlStrc
{
	int32_t nArraySize;							//0x00
	D2NpcRecordStrc* pFirstRecord;			//0x04
	D2SeedStrc pSeed;						//0x08
	int32_t unk0x10;							//0x10
};

struct D2NpcGambleStrc
{
	D2InventoryStrc* pInventory;			//0x00
	D2UnitGUID dwGUID;							//0x04
	D2NpcGambleStrc* pNext;					//0x08
};

struct D2UnitProxyStrc
{
	D2ItemCacheStrc* pItemCache;			//0x00
	int32_t nItems;								//0x04
	uint32_t* pPermCache;						//0x08
	int32_t nPerms;								//0x0C
};

struct D2NPCMessageListStrc
{
	uint16_t nMessageIndexes[8];				//0x00
};

struct D2NPCMessageStrc
{
	int32_t nNPCNo;								//0x00
	int16_t nStringIndex;						//0x04
	int16_t nPad;								//0x06
	BOOL nMenu;								//0x08
};

struct D2NPCMessageTableStrc
{
	D2NPCMessageStrc pMessages[16];			//0x00
	int32_t nMessages;							//0xC0
};

struct D2NpcRecordStrc
{
	int32_t nNPC;								//0x00
	D2InventoryStrc* pInventory;			//0x04
	D2NpcGambleStrc* pGamble;				//0x08
	BOOL bGambleInit;						//0x0C
	D2MercDataStrc* pMercData;				//0x10
	void* pEvent;							//0x14
	void* pVendorChain;						//0x18
	BOOL bTrading;							//0x1C

	//union
	//{
		struct
		{
			//union
			//{
			//	bool bFlags[8];				//0x20
				struct
				{
					bool bVendorInit;		//0x20
					bool bHireInit;			//0x21
					uint8_t nAct;				//0x22
					bool bTrader;			//0x23
					bool bLevelRefresh;		//0x24
					bool bInited;			//0x25
					bool bForceVendor;		//0x26
					bool bRefreshInventory;	//0x27
				};
			//};

			uint32_t dwTicks;					//0x28
			D2UnitProxyStrc pProxy;			//0x2C
			uint32_t dwUnk;					//0x3C
			D2UnitGUID dwNPCGUID;				//0x40
		};
		//D2NPCTradeStrc pTrade;            //0x20
	//};
};

typedef int32_t(__fastcall* OBELISKPOWERUP)(D2GameStrc*, D2UnitStrc*, int32_t);

struct D2ObeliskPowerUpStrc
{
	OBELISKPOWERUP pPowerUpCallback;		//0x00
	uint32_t nChance;					//0x04
	int32_t nValue;								//0x08
};

typedef void(__fastcall* OBJINITFN)(D2ObjInitFnStrc*);

struct D2ObjInitFnStrc
{
	D2GameStrc* pGame;						//0x00
	D2UnitStrc* pObject;					//0x04
	D2RoomStrc* pRoom;						//0x08
	D2ObjectControlStrc* pObjectregion;		//0x0C
	D2ObjectsTxt* pObjectTxt;				//0x10
	int32_t nX;									//0x14
	int32_t nY;									//0x18
};

typedef BOOL(__fastcall* OBJOPERATEFN)(D2ObjOperateFnStrc*, int32_t);

struct D2ObjOperateFnStrc
{
	D2GameStrc* pGame;						//0x00
	D2UnitStrc* pObject;					//0x04
	D2UnitStrc* pPlayer;					//0x08
	D2ObjectControlStrc* pObjectregion;		//0x0C
	int32_t nObjectIdx;							//0x10
};

struct D2PacketListStrc
{
	int32_t nTotal;								//0x00
	int32_t nUsed;								//0x04
	int32_t unk0x08;							//0x08
	void* unk0x0C;							//0x0C
};

struct D2PacketTableStrc
{
	void* pCallback1;						//0x00
	int32_t nSize;								//0x04
	void* pCallback2;						//0x08
};

typedef BOOL(__fastcall* SPELLPREPARE)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, D2UnitStrc*, int32_t, int32_t, int32_t);
typedef BOOL(__fastcall* SPELLDO)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, D2UnitStrc*, int32_t, int32_t, int32_t);

struct D2pSpellTblStrc
{
	SPELLPREPARE pfSpellPrepare;			//0x00
	SPELLDO pfSpellDo;						//0x04
};

struct D2QuestArgStrc
{
	D2GameStrc* pGame;						//0x00
	int32_t nEvent;								//0x04
	D2UnitStrc* pTarget;					//0x08
	D2UnitStrc* pPlayer;					//0x0C
	uint32_t unk0x10;							//0x10
	union
	{
		struct
		{
			void* pTextControl;				//0x14
			uint32_t dw18;						//0x18
		};
		struct
		{
			int16_t nNPCNo;					//0x14
			uint16_t unk0x16;					//0x16
			int16_t nMessageIndex;			//0x18
			uint16_t unk0x1A;					//0x1A
		};
		struct
		{
			int32_t nOldLevel;					//0x14
			int32_t nNewLevel;					//0x18
		};
	};
};

struct D2QuestChainStrc
{
	D2QuestDataStrc* pQuestData;			//0x00
	D2QuestChainStrc* pNext;				//0x04
};

typedef void(__fastcall* QUESTINIT)(D2QuestDataStrc*);
typedef void(__fastcall* QUESTCALLBACK)(D2QuestDataStrc*, D2QuestArgStrc*);
typedef bool(__fastcall* QUESTSTATUS)(D2QuestDataStrc*, D2UnitStrc*, D2BitBufferStrc*, D2BitBufferStrc*, uint8_t*);
typedef bool(__fastcall* QUESTUPDATE)(D2GameStrc*, D2QuestDataStrc*);
typedef bool(__fastcall* QUESTACTIVE)(D2QuestDataStrc*, int32_t, D2UnitStrc*, D2BitBufferStrc*, D2UnitStrc*);
typedef int32_t(__fastcall* QUESTSEQ)(D2QuestDataStrc*, D2UnitStrc*, D2BitBufferStrc*, D2BitBufferStrc*, uint8_t*);

struct D2QuestDataStrc
{
	int32_t nQuestNo;							//0x00 - internal
	D2GameStrc* pGame;						//0x04
	char nActNo;							//0x08
	bool bNotIntro;							//0x09 - set to false for intro quests, it could also be for already completed...
	bool bActive;							//0x0A - confirmed
	uint8_t fLastState;						//0x0B - previous quest state
	uint8_t fState;							//0x0C - main quest state
	char nInitNo;							//0x0D
	uint16_t dw0E;								//0x0E 
	int32_t nSeqId;								//0x10 - nInitNo
	uint32_t dwFlags;							//0x14
	void* pQuestDataEx;						//0x18 - union of 0x29 structs
	union
	{
		struct
		{
			uint32_t nPlayerGUID[32];			//0x1C - players that have entered the quest zone
			uint16_t nPlayerCount;				//0x9C
		};
		D2QuestGUIDStrc pGUID;				//0x1C
	};
	uint16_t dw9E;								//0x9E 
	QUESTCALLBACK pfCallback[15];			//0xA0
	D2NPCMessageTableStrc* pNPCMessages;	//0xDC
	int32_t nQuest;								//0xE0 - index in quest flag bit array
	QUESTSTATUS pfStatusFilter;				//0xE4
	QUESTACTIVE pfActiveFilter;				//0xE8
	void* pfSeqFilter;						//0xEC
	D2QuestDataStrc* pPrev;					//0xF0
};

struct D2QuestInitTableStrc
{
	QUESTINIT pfInit;						//0x00
	uint8_t nAct;								//0x04
	uint8_t pad0x05[3];						//0x05
	uint32_t nVersion;							//0x08
	bool bNoSetState;						//0x0C - used by the sequences for quest init flags
	uint8_t pad0x0D[3];						//0x0D
	int32_t nChainNo;							//0x10 - quest data internal chain id
	uint32_t nQuestNo;							//0x14 - pQuestData flag no
};

struct D2QuestIntroTableStrc
{
	QUESTINIT pfInit;						//0x00
	uint8_t nAct;								//0x04
	uint8_t pad0x05[3];						//0x05
}; 

struct D2QuestInfoStrc
{
	D2QuestDataStrc* pLastQuest;			//0x00
	BOOL bExecuting;						//0x04
	BOOL bPickedSet;						//0x08
	D2BitBufferStrc* pQuestFlags;			//0x0C
	D2QuestTimerStrc* pTimer;				//0x10
	uint32_t dwTick;							//0x14
	D2SeedStrc pSeed;						//0x18
	uint8_t unk0x20;							//0x20
	uint8_t unk0x21[3];						//0x21
};

struct D2QuestTimerStrc
{
	QUESTUPDATE pfUpdate;					//0x00
	D2QuestDataStrc* pQuest;				//0x04
	uint32_t dwTicks;							//0x08
	uint32_t dwTimeout;						//0x0C
	D2QuestTimerStrc* pNext;				//0x10
};

//struct D2QuestCltStrc
//{
//	uint8_t nQuest[3];							//0x00
//	uint8_t nFlag[3];							//0x03
//	uint8_t nQuestHistory[3][72];				//0x06
//	uint8_t nQuestInitFn[3];					//
//	uint8_t nQuestFn[3];						//
//};
//
//struct D2QuestSrvStrc
//{
//	uint8_t nQuest[3];							//0x00
//	uint8_t nFlag[3];							//0x03
//	uint8_t nQuestHistory[3][72];				//0x06
//	uint8_t nQuestInitFn[3];					//
//	uint8_t nQuestFn[3];						//
//};

struct D2QuestDescStrc
{
	uint16_t wTblTitle;							//0x00
	uint16_t wTblInitDesc;						//0x02
};

struct D2QuestDescriptorStrc
{
	uint8_t bActive;							//0x00
	uint8_t nQuestNo;							//0x01
	uint8_t nPosition;							//0x02
	uint8_t nTab;								//0x03
	uint16_t* pStringIds;						//0x04
	int32_t nQuestFlag;							//0x08
	int32_t nArrayId;							//0x0C
};

//struct D2QuestRecordStrc
//{
//	uint32_t unk;
//};

struct D2QuestUiButtonCoordStrc
{
	int32_t nCellfileX;							//0x00
	int32_t nCellfileY;							//0x04
	int32_t nClickX;							//0x08
	int32_t nClickY;							//0x0C
};

struct D2QuestUiStrc
{
	uint8_t field_0;							//0x00
	int32_t nQuest;								//0x01
	int16_t nQuestTitleStringId;				//0x05
	wchar_t wszQuestDesc[300];				//0x07
	int16_t field_25F;						//0x25F
	uint8_t nPosition;							//0x261
	uint8_t nQuestNo;							//0x262
	uint8_t field_263;							//0x263
	uint8_t field_264;							//0x264
	uint8_t field_265;							//0x265
	int32_t nQuestState;						//0x266
};

struct D2QuestUiTabStrc
{
	int32_t nStartQuest;						//0x00
	int32_t nEndQuest;							//0x04
};

struct D2RenderCallbackStrc
{
	BOOL(__fastcall *pfInitialize)(HINSTANCE);
	BOOL(__fastcall *pfInitPerspective)(D2GfxSettingsStrc* pSettings, D2GfxHelperStrc* pHelpers);
	BOOL(__fastcall *pfRelease)();
	BOOL(__fastcall *pfCreateWindow)(HWND hWnd, int32_t nResolutionMode);
	BOOL(__fastcall *pfDestroyWindow)();
	void(__fastcall *pfEndCutScene)(HWND hWnd, int32_t nResolutionMode, int32_t nWindowState);
	BOOL(__fastcall *pfBeginScene)(BOOL bClear, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
	BOOL(__fastcall *pfEndScene1)();
	BOOL(__fastcall *pfEndScene2)();
	BOOL(__fastcall *pfResizeWindow)(HWND hWnd, BOOL bForceResize);
	BOOL(__fastcall *pfGetBackBuffer)(uint8_t *pBuffer);
	BOOL(__fastcall *pfActivateWindow)();
	BOOL(__fastcall *pfSetOption)(int32_t nOption, int32_t nValue);
	BOOL(__fastcall *pfBeginCutScene)();
	void(__fastcall *pfPlayCutScene)(const char *szFile, int32_t nResolutionMode, void *pfFrame);
	BOOL(__fastcall *pfCheckCutScene)();
	void(__fastcall *pfDecodeSmacker)(const char *szSmacker, uint8_t *pBuffer, int32_t nVersion);
	void(__fastcall *pfPlayerSmacker)(void *pContext);
	void(__fastcall *pfCloseSmacker)(void *pContext);
	int32_t* (__fastcall *pfGetRenderStatistics)();
	int32_t(__fastcall *pfGetScreenSize)(int32_t *pWidth, int32_t *pHeight);
	void(__fastcall *pfUpdateScaleFactor)(int32_t nScaleFactor);
	BOOL(__fastcall *pfSetGamma)(int32_t nGamma);
	int32_t(__fastcall *pfCheckGamma)();
	void(__fastcall *pfSetPerspectiveScale)(int32_t nScaleX, int32_t nScaleY);
	void(__fastcall *pfAdjustPerspectivePosition)(int32_t nPosX, int32_t nPosY, int32_t nBais, int32_t *pXAdjust, int32_t *pYAdjust);
	void(__fastcall *pfPerspectiveScalePosition)(int32_t nPosX, int32_t nPosY, int32_t nAngle, int32_t *pXAdjust, int32_t *pYAdjust, BOOL bOrder);
	void(__fastcall *pfSetDefaultPerspectiveFactor)();
	void(__fastcall *pfSetPalette)(LPPALETTEENTRY pPalette);
	void(__fastcall *pfSetPaletteTable)(LPPALETTEENTRY *pPaletteTable);
	void(__fastcall *pfSetGlobalLight)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
	BOOL(__fastcall *pfDrawGroundTile)(D2TileLibraryEntryStrc *pTile, D2GfxLightExStrc *pLight, int32_t nPosX, int32_t nPosY, int32_t nWorldXpos, int32_t nWorldYpos, uint8_t nAlpha, int32_t nScreenPanels, void *pTileData);
	void(__fastcall *pfDrawPerspectiveImage)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, int32_t nDrawMode, int32_t nScreenMode, uint8_t *pPalette);
	void(__fastcall *pfDrawImage)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, int32_t nDrawMode, uint8_t *pPalette);
	void(__fastcall *pfDrawShiftedImage)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, int32_t nDrawMode, int32_t nGlobalPaletteShift);
	void(__fastcall *pfDrawVerticalCropImage)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY, int32_t nSkipLines, int32_t nDrawLines, int32_t nDrawMode);
	void(__fastcall *pfDrawShadow)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY);
	void(__fastcall *pfDrawImageFast)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY, uint8_t nPaletteIndex);
	void(__fastcall *pfDrawClippedImage)(D2GfxDataStrc *pData, int32_t nPosX, int32_t nPosY, void *pCropRect, int32_t nDrawMode);
	BOOL(__fastcall *pfDrawWallTile)(D2TileLibraryEntryStrc *pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc *pLight, int32_t nScreenPanels);
	BOOL(__fastcall *pfDrawTransWallTile)(D2TileLibraryEntryStrc *pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc *pLight, int32_t nScreenPanels, uint8_t nAlpha);
	BOOL(__fastcall *pfDrawShadowTile)(D2TileLibraryEntryStrc *pTile, int32_t nPosX, int32_t nPosY, int32_t nDrawMode, int32_t nScreenPanels);
	void(__fastcall *pfDrawRect)(RECT *pRect, uint8_t nPaletteIndex);
	void(__fastcall *pfDrawRectEx)(RECT *pRect, uint8_t nPaletteIndex);
	void(__fastcall *pfDrawSolidRect)(RECT *pRect, uint8_t nPaletteIndex);
	void(__fastcall *pfDrawSolidSquare)(POINT *pPoint, uint8_t nSize, uint8_t nPaletteIndex);
	void(__fastcall *pfDrawSolidRectEx)(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, int32_t nDrawMode);
	void(__fastcall *pfDrawSolidRectAlpha)(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha);
	void(__fastcall *pfDrawLine)(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha);
	void(__fastcall *pfClearScreen)(BOOL bPartial);
	void(__fastcall *pfDrawString)(int32_t nPosX, int32_t nPosY, const char *szFormat, va_list va);
	void(__fastcall *pfDrawLight)(uint32_t *pLight, uint32_t *pPlayerLight, int32_t nPosX, int32_t nPosY);
	void(__fastcall *pfDebugFillBackBuffer)(int32_t nPosX, int32_t nPosY);
	void(__fastcall *pfClearCaches)();
};

struct D2ResistanceRecordStrc
{
	int32_t nDamageDataOffset;					//0x00
	int32_t nBaseResistStat;					//0x04
	int32_t nMaxResistStat;						//0x08
	int32_t nPierceStat;						//0x0C
	int32_t nAbsorbPercentStat;					//0x10
	int32_t nAbsorbFlatStat;					//0x14
	int32_t nReductionType;						//0x18
	BOOL bHealAttacker;						//0x1C
	int32_t nType;								//0x20
	char* szName;							//0x24
};


struct D2SaveHeaderStrc
{
	uint32_t dwHeaderMagic;					//0x000
	uint32_t dwVersion;						//0x004
	uint32_t dwSize;							//0x008
	uint32_t dwChecksum;						//0x00C
	uint32_t dwWeaponSet;						//0x010
	char szName[16];						//0x014
	uint32_t dwSaveFlags;						//0x024
	uint8_t nClass;							//0x028
	uint8_t nStats;							//0x029
	uint8_t nSkills;							//0x02A
	uint8_t nLevel;							//0x02B
	uint32_t dwCreateTime;						//0x02C
	uint32_t dwLasTime;						//0x030
	uint32_t dwPlayTime;						//0x034
	D2SaveSkillKeyStrc SkillKeys[16];		//0x038
	D2SaveSkillKeyStrc ButtonSkills[4];		//0x078
	uint8_t nComponent[16];					//0x088
	uint8_t nCompColor[16];					//0x098
	uint8_t nTown[3];							//0x0A8
	uint32_t dwMapSeed;						//0x0AB
	D2MercSaveDataStrc MercSaveData;		//0x0AF
	uint8_t nRealmData[16];					//0x0BF
	uint8_t nSaveField;						//0x0CF
	uint32_t dwLastLevel;						//0x0D0
	uint32_t dwLastTown;						//0x0D4
	uint8_t nLastDifficulty;					//0x0D8
};

struct D2SaveLaunchStrc
{
	uint16_t unk0x00;					//0x00
	uint8_t pComponents[11];			//0x02
	uint8_t nClass;					//0x0D
	uint8_t pComponentColors[11];		//0x0C
	uint8_t nLevel;					//0x19
	uint16_t unk0x1A;					//0x1A
	uint16_t unk0x1C;					//0x1C
	uint8_t unk0x1E;					//0x1E
	uint8_t unk0x1F;					//0x1F
	uint8_t unk0x20;					//0x20
	uint8_t unk0x21;					//0x21
};

typedef int32_t(__fastcall* SERVERPACKETCALLBACK)(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, size_t nPacketSize);

struct D2ServerPacketTableStrc
{
	SERVERPACKETCALLBACK pfCallback;		//0x00
	BOOL b;									//0x04
};

struct D2ServerParamStrc
{
	int32_t unk0x00;							//0x00
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
	int32_t unk0x0C;							//0x0C
};

typedef void(__fastcall* SHRINECALLBACK)(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);

struct D2ShrineTableStrc
{
	SHRINECALLBACK pfShrineCallback;		//0x00
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
};

struct D2SplittedTextStrc
{
	wchar_t* wszLine;						//0x00
	D2SplittedTextStrc* pNext;				//0x04
};

struct D2SummonArgStrc
{
	uint32_t dwFlags;							//0x00
	D2UnitStrc* pOwner;						//0x04
	int32_t nHcIdx;								//0x08
	int32_t nSpecialAiState;					//0x0C
	int32_t nMonMode;							//0x10
	D2CoordStrc pPosition;					//0x14		
	int32_t nPetType;							//0x1C
	int32_t nPetMax;							//0x20
};

struct D2TargetNodeStrc
{
	D2UnitStrc* pUnit;						//0x00
	int32_t unk0x04;							//0x04
	D2TargetNodeStrc* pNext;				//0x08
};

struct D2TaskStrc
{
	int32_t unk0x00;							//0x00
	int32_t nType;								//0x04
	int32_t unk0x08;							//0x08
	int32_t unk0x0C;							//0x0C
};

struct D2TblHeaderStrc
{
	uint16_t nCRC;								//0x00
	uint16_t nNumElements;						//0x02
	int32_t nHashTableSize;						//0x04
	uint8_t nVersion;							//0x08
	uint32_t dwIndexStart;						//0x09
	uint32_t dwNumLoops;						//0x0D
	uint32_t dwFileSize;						//0x11
};

struct D2TblNodeStrc
{
	uint8_t bUsed;								//0x00
	uint16_t nIndexNo;							//0x01
	uint32_t dwHashValue;						//0x03
	uint32_t dwStringOffset;					//0x07
	uint32_t dwKeyOffset;						//0x0B
	uint16_t nNameLen;							//0x0F
};

struct D2TCStackStrc
{
	int32_t nPicks;								//0x00
	int16_t nMagic;							//0x04
	int16_t nRare;							//0x06
	int16_t nSet;								//0x08
	int16_t nUnique;							//0x0A
	int16_t nSuperior;						//0x0C
	int16_t nNormal;							//0x0E
	int32_t unk0x10;							//0x10
};


struct D2WarpTileStrc
{
	D2RoomExStrc* pRoomEx;					//0x00
	D2WarpTileStrc* pNext;					//0x04
	BOOL bSelectable;						//0x08
	uint32_t __00C;							//0x0C
	D2LvlWarpTxt* pLvlWarpTxt;				//0x10
	uint32_t __014;							//0x14
};

struct D2WeaponDescTblStrc
{
	int32_t nItemType;							//0x00
	uint16_t wDescString;						//0x04
};

struct D2WeaponSpeedTblStrc
{
	int32_t nId;								//0x00
	uint16_t wDescString;						//0x02
};

struct D2WindowPlacementStrc
{
	HWND hWnd;								//0x00
	WINDOWPLACEMENT windowPlacement;		//0x04
};

struct D2WinMsgStrc
{
	HWND hWnd;								//0x00
	uint32_t uMessage;							//0x04
	union									//0x08
	{
		WPARAM  wParam;
		int32_t	  nKey;
	};						
	union									//0x0C
	{
		struct
		{
			uint16_t	nXpos;  
			uint16_t	nYpos;
		};
		LPARAM   lParam;
	};						
	uint32_t dwCommandSource;					//0x10 HIWORD(wParam) when uMessage == WM_COMMAND
	uint32_t dwArg;							//0x14
	BOOL bReturn;							//0x18
	LRESULT lResult;						//0x1C
};

#pragma pack()
