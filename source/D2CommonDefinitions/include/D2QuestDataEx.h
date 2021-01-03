#pragma once

struct D2MapAIStrc;
struct D2RoomStrc;
struct D2UnitStrc;


#pragma pack(1)

struct D2CoordStrc							//sizeof 0x08
{
	int nX;										//0x00
	int nY;										//0x04
};

enum D2QuestStateFlagIds
{
	QUESTSTATEFLAG_A1Q0 = 0,
	QUESTSTATEFLAG_A1Q1 = 1,
	QUESTSTATEFLAG_A1Q2 = 2,
	QUESTSTATEFLAG_A1Q3 = 3,
	QUESTSTATEFLAG_A1Q4 = 4,
	QUESTSTATEFLAG_A1Q5 = 5,
	QUESTSTATEFLAG_A1Q6 = 6,

	QUESTSTATEFLAG_A1COMPLETED = 7,

	QUESTSTATEFLAG_A2Q0 = 8,
	QUESTSTATEFLAG_A2Q1 = 9,
	QUESTSTATEFLAG_A2Q2 = 10,
	QUESTSTATEFLAG_A2Q3 = 11,
	QUESTSTATEFLAG_A2Q4 = 12,
	QUESTSTATEFLAG_A2Q5 = 13,
	QUESTSTATEFLAG_A2Q6 = 14,

	QUESTSTATEFLAG_A2COMPLETED = 15,

	QUESTSTATEFLAG_A3Q0 = 16,
	QUESTSTATEFLAG_A3Q1 = 17,
	QUESTSTATEFLAG_A3Q2 = 18,
	QUESTSTATEFLAG_A3Q3 = 19,
	QUESTSTATEFLAG_A3Q4 = 20,
	QUESTSTATEFLAG_A3Q5 = 21,
	QUESTSTATEFLAG_A3Q6 = 22,

	QUESTSTATEFLAG_A3COMPLETED = 23,

	QUESTSTATEFLAG_A4Q0 = 24,
	QUESTSTATEFLAG_A4Q1 = 25,
	QUESTSTATEFLAG_A4Q2 = 26,
	QUESTSTATEFLAG_A4Q3 = 27,

	QUESTSTATEFLAG_A4COMPLETED = 28,

	QUESTSTATEFLAG_A1Q7 = 29,

	QUESTSTATEFLAG_A2Q7 = 30,
	QUESTSTATEFLAG_A2Q8 = 31,

	QUESTSTATEFLAG_A3Q7 = 32,

	QUESTSTATEFLAG_A4Q4 = 33,

	//Unused: 34

	QUESTSTATEFLAG_A5Q1 = 35,
	QUESTSTATEFLAG_A5Q2 = 36,
	QUESTSTATEFLAG_A5Q3 = 37,
	QUESTSTATEFLAG_A5Q4 = 38,
	QUESTSTATEFLAG_A5Q5 = 39,
	QUESTSTATEFLAG_A5Q6 = 40,

	QUESTSTATEFLAG_A1INTRO = 41,
	QUESTSTATEFLAG_A2INTRO = 41,
	QUESTSTATEFLAG_A3INTRO = 41,
	QUESTSTATEFLAG_A5INTRO = 41,
};

enum D2Quests
{
	QUEST_A1Q0_WARRIVGOSSIP = 0,
	QUEST_A1Q1_DENOFEVIL = 1,
	QUEST_A1Q2_BLOODRAVEN = 2,
	QUEST_A1Q3_MALUS = 3,
	QUEST_A1Q4_CAIN = 4,
	QUEST_A1Q5_COUNTESS = 5,
	QUEST_A1Q6_ANDARIEL = 6,

	QUEST_A2Q0_JERHYNGOSSIP = 7,
	QUEST_A2Q1_RADAMENT = 8,
	QUEST_A2Q2_HORADRICSTAFF = 9,
	QUEST_A2Q3_TAINTEDSUN = 10,
	QUEST_A2Q4_HORAZONTOME = 11,
	QUEST_A2Q5_SUMMONER = 12,
	QUEST_A2Q6_DURIEL = 13,

	QUEST_A3Q0_HRATLIGOSSIP = 14,
	QUEST_A3Q1_LAMESENTOME = 15,
	QUEST_A3Q2_KHALIMFLAIL = 16,
	QUEST_A3Q3_GIDBINN = 17,
	QUEST_A3Q4_GOLDENBIRD = 18,
	QUEST_A3Q5_TRAVINCAL = 19,
	QUEST_A3Q6_MEPHISTO = 20,

	QUEST_A4Q0_TYRAELGOSSIP = 21,
	QUEST_A4Q1_IZUAL = 22,
	QUEST_A4Q2_DIABLO = 23,
	QUEST_A4Q3_HELLFORGE = 24,

	QUEST_A1Q1EX_NAVI = 25,

	QUEST_A2Q7_UNUSED_GUARDGOSSIP = 26,
	QUEST_A2Q8_UNUSED_GUARDGOSSIP = 27,

	QUEST_A3Q7_DARKWANDERER = 28,

	QUEST_A4Q4_MALACHAI = 29,

	//TODO: 30 unused?

	QUEST_A5Q1_SHENK = 31,
	QUEST_A5Q2_RESCUESOLDIERS = 32,
	QUEST_A5Q3_PRISONOFICE = 33,
	QUEST_A5Q4_NIHLATHAK = 34,
	QUEST_A5Q5_ANCIENTS = 35,
	QUEST_A5Q6_BAAL = 36
};
struct D2QuestGUIDStrc						//sizeof 0x84
{
	uint32_t dwPlayerGUID[32];						//0x00
	short nGUIDCount;							//0x80
	uint8_t pad0x82[2];							//0x82
};

struct D2PrisonOfIceRewardStrc				//sizeof 0x08
{
	const uint32_t* dwItemCodes;					//0x00
	int nItemCount;								//0x04
};


//			ACT 1			//

//DenOfEvil
struct D2Act1Quest1Strc						//sizeof 0x8C
{
	D2QuestGUIDStrc pQuestGUID;					//0x00
	uint8_t bFinished;								//0x84
	uint8_t bEnteredDen;							//0x85
	uint8_t bAkaraActivated;						//0x86
	uint8_t bTimerActive;							//0x87
	int nMonstersLeft;							//0x88
};

//BloodRaven
struct D2Act1Quest2Strc						//sizeof 0x0C
{
	uint8_t unk0x00;								//0x00 TODO
	uint8_t unk0x01;								//0x01 TODO
	uint8_t bBloodravenKilled;						//0x02
	uint8_t bKashyaActivated;						//0x03
	int unk0x04;								//0x04 TODO
	int nBloodravenGUID;						//0x08
};

//Malus
struct D2Act1Quest3Strc						//sizeof 0xA4
{
	uint8_t unk0x00;								//0x00 TODO
	uint8_t bMalusInitialized;						//0x01
	uint8_t bCharsiIntroActivated;					//0x02
	uint8_t bCharsiEndActivated;					//0x03
	int nMalusGUID;								//0x04
	uint8_t unk0x08;								//0x08 TODO
	uint8_t unused0x09[9];							//0x09
	uint16_t unk0x12;								//0x12
	D2QuestGUIDStrc pGUID;						//0x14
	int nMalusObjectMode;						//0x98
	int nMalusItemsInGame;						//0x9C
	uint8_t bMalusAcquired;						//0xA0
	uint8_t bStartingPlayerHasMalus;				//0xA1
	uint8_t pad0xA2[2];							//0xA2
};

//RescueCain
struct D2Act1Quest4Strc						//sizeof 0x1BC
{
	uint16_t nStoneOrder[5];						//0x00
	uint16_t unused0x0A;							//0x0A
	uint16_t nCurrentMonolithNo;					//0x0C
	uint8_t pad0x0E[2];							//0x0E
	int nMonolithGUIDs[5];						//0x10
	int unused0x24;								//0x24
	int nInvisibleObjectGUID;					//0x28
	short nCairnStoneInteractions;				//0x2C
	uint8_t pad0x2E[2];							//0x2E
	int nInifussTreeGUID;						//0x30
	int nCainGibbetGUID;						//0x34
	int nDecipheredScrollGUID;					//0x38
	int nPlayerGUID;							//0x3C
	int nCairnStoneAlphaGUID;					//0x40
	uint8_t bOpenPortalTimerActive;				//0x44
	uint8_t bPortalToTristramOpened;				//0x45
	uint8_t bTimerActive;							//0x46
	uint8_t bInifussTreeInitialized;				//0x47
	uint8_t bCageModeChanged;						//0x48
	uint8_t bInvisbleObjectInitialized;			//0x49
	uint8_t bMonolithStonesOrdered;				//0x4A
	uint8_t unk0x4B;								//0x4B TODO
	uint8_t unk0x4C;								//0x4C TODO
	uint8_t bActivateCairnStones;					//0x4D
	uint8_t bDecipheredScrollAcquired;				//0x4E
	uint8_t bOperatedWithLastCairnStone;			//0x4F
	uint8_t unk0x50;								//0x50 TODO: Cain rescued?
	uint8_t bCainInTownSpawned;					//0x51
	uint8_t unk0x52;								//0x52 TODO: Cain not spawned?
	uint8_t bAkaraIntroActivated;					//0x53
	int nCainGibbetObjectMode;					//0x54
	int unk0x58;								//0x58 TODO
	uint8_t bCairnStonesActive[5];					//0x5C
	uint8_t bCainRemovedFromTristram;				//0x61
	uint8_t bCainNotCreated;						//0x62
	uint8_t bQuestCompleteBefore;					//0x63
	uint8_t bAkaraActivatedForScroll;				//0x64
	uint8_t unused0x65;							//0x65
	uint8_t bPortalFromTristramToTownSpawned;		//0x66
	uint8_t bCainPortalCoordsInitialized;			//0x67
	int nCainMonsterInTownGUID;					//0x68
	int nCainObjectGUID;						//0x6C
	uint8_t bCainObjectInitialized;				//0x70
	uint8_t pad0x71[3];							//0x71
	D2UnitStrc* pPlayerInTristram;				//0x74
	uint8_t bScrollAcquired;						//0x78
	uint8_t pad0x79[3];							//0x79
	int nScrollsInGame;							//0x7C
	int nObjectUpdateInvocations;				//0x80
	D2CoordStrc pCainStartPositionCoords;		//0x84
	int nGoldPilesToDropFromWirtsBody;			//0x8C
	uint8_t bWirtsBodyInitialized;					//0x90
	uint8_t bChangePortalOutsideTownToSpecialMode;	//0x91
	uint8_t bChangePortalInTownToSpecialMode;		//0x92
	uint8_t bCainInTristramDeactivated;			//0x93
	uint8_t unused0x94;							//0x94
	uint8_t bCainPortalOutsideTownSpawned;			//0x95
	uint8_t bCainPortalInTownSpawned;				//0x96
	uint8_t unused0x97;							//0x97
	int nCainPortalOutsideTownGUID;				//0x98
	D2CoordStrc pCainPortalOutsideTownCoords;	//0x9C
	int nCainPortalInTownGUID;					//0xA4
	D2CoordStrc pCainPortalInTownCoords;		//0xA8
	uint8_t unused0xB0[4];							//0xB0
	D2QuestGUIDStrc pQuestGUID;					//0xB4
	D2QuestGUIDStrc pGUID;						//0x138
};

//Countess
struct D2Act1Quest5Strc						//sizeof 0x120
{
	int nUnitGUIDs1[12];						//0x00 TODO: Which UnitGUIDs?
	short nUnitCount1;							//0x30
	uint8_t pad0x32[2];							//0x32
	int nUnitGUIDs2[12];						//0x34 TODO: Which UnitGUIDs?
	short nUnitCount2;							//0x64
	uint8_t pad0x66[2];							//0x66
	int nObjectGUIDs[8];						//0x68 TODO: Which objects?
	uint16_t nObjectCount;							//0x88
	uint8_t pad0x8A[2];							//0x8A
	D2QuestGUIDStrc pGUID;						//0x8C
	D2CoordStrc pCoord;							//0x110
	uint8_t bCountessKilled;						//0x118
	uint8_t bDeathMissilesCreated;					//0x119
	uint8_t bTriggerSeqFilter;						//0x11A
	uint8_t bTowerTomeActivated;					//0x11B
	uint8_t unk0x11C;								//0x11C TODO
	uint8_t pad0x11D[3];							//0x11D
};

//Andariel
struct D2Act1Quest6Strc						//sizeof 0x198
{
	D2QuestGUIDStrc pGUID1;						//0x00
	D2QuestGUIDStrc pGUID2;						//0x84
	D2QuestGUIDStrc pGUID3;						//0x108
	int nAndarielGUID;							//0x18C
	short unk0x190;								//0x190 TODO
	short nTimerInvocations;					//0x192
	uint8_t bAndarielKilled;						//0x194
	uint8_t bCainActivated;						//0x195
	short pad0x196;								//0x196
};


//			ACT 2			//

//Jerhyn Gossip
struct D2Act2Quest0Strc						//sizeof 0x84
{
	D2QuestGUIDStrc pGUID;						//0x00
};

//Radament
struct D2Act2Quest1Strc						//sizeof 0x14
{
	uint8_t bRadamentKilled;						//0x00
	uint8_t pad0x01[3];							//0x01
	D2RoomStrc* pRoom;							//0x04
	uint8_t bAtmaActivated;						//0x08
	uint8_t unk0x09;								//0x09 TODO
	uint8_t bStatusTimerStarted;					//0x0A
	uint8_t pad0x0B;								//0x0B
	int nSkillBookDropCount;					//0x0C
	uint8_t bRewardPending;						//0x10
	uint8_t pad0x11[3];							//0x11
};

//HoradricStaff
struct D2Act2Quest2Strc						//sizeof 0x30
{
	int unused0x00[5];							//0x00
	int nItemDropCount;							//0x14
	int nBaseStaffs;							//0x18
	int nCubedStaffs;							//0x1C
	int nHoradricCubes;							//0x20
	int nAmulets;								//0x24
	uint8_t bHoradricScrollDropped;				//0x28
	uint8_t bCubeDropped;							//0x29
	uint8_t bStaffCubed;							//0x2A
	uint8_t unk0x2B;								//0x2B TODO
	int nPlayerGUID;							//0x2C
};

//TaintedSun
struct D2Act2Quest3Strc						//sizeof 0xA0
{
	uint8_t unused0x00;							//0x00
	uint8_t bDarkenTimerCreated;					//0x01
	uint8_t bRewarded;								//0x02
	uint8_t bBlackened;							//0x03
	uint8_t bAltarDestroyed;						//0x04
	uint8_t bAltarInitialized;						//0x05
	uint8_t unk0x06;								//0x06 TODO
	uint8_t pad0x07;								//0x07
	int nAltarMode;								//0x08
	int nAltarGUID;								//0x0C
	D2RoomStrc* pRoom;							//0x10
	D2QuestGUIDStrc pGUID;						//0x14
	int nAltarLevelId;							//0x98
	int nViperAmuletDropCount;					//0x9C
};

//HorazonTome
struct D2Act2Quest4Strc						//sizeof 0x48
{
	uint8_t unused0x00[4];							//0x00
	uint8_t bDrognanActivated;						//0x04
	uint8_t pad0x05[3];							//0x05
	D2RoomStrc* pRoom;							//0x08
	uint8_t bJerhynStartInitialized;				//0x0C
	uint8_t bJerhynPalaceInitialized;				//0x0D
	uint8_t bPalaceOpen;							//0x0E
	uint8_t unk0x0F;								//0x0F TODO
	uint8_t unk0x10;								//0x10 TODO
	uint8_t bHaremBlockerInitialized;				//0x11
	uint8_t unk0x12;								//0x12 TODO
	uint8_t unk0x13;								//0x13 TODO
	uint8_t unk0x14;								//0x14 TODO
	uint8_t bJerhynCoordinatesStored;				//0x15
	uint8_t bPortalToCanyonOpen;					//0x16
	uint8_t unused0x17;							//0x17
	uint8_t unk0x18;								//0x18 TODO
	uint8_t unk0x19;								//0x19 TODO
	uint8_t bPlayerCloseToHaremBlocker;			//0x1A
	uint8_t pad0x1B;								//0x1B
	int nPlayerCloseToHaremBlockerGUID;			//0x1C
	D2CoordStrc pHaremBlockerCoords;			//0x20
	D2CoordStrc pJerhynCoords;					//0x28
	D2CoordStrc pGuardCoords;					//0x30
	int nHaremBlockerGUID;						//0x38
	int nJerhynGUID;							//0x3C
	uint16_t nHaremBlockerObjectMode;				//0x40
	uint16_t nPortalModeInSanctuary;				//0x42
	uint16_t nPortalModeInCellar;					//0x44
	uint8_t bIsHaremBlockerNeutral;				//0x46
	uint8_t pad0x47;								//0x47
};

//Summoner
struct D2Act2Quest5Strc						//sizeof 0x0C
{
	uint8_t bSummonerKilled;						//0x00
	uint8_t unk0x01;								//0x01 TODO
	uint8_t pad0x02[2];							//0x02
	D2RoomStrc* pRoom;							//0x04
	uint8_t bStatusTimerStarted;					//0x08
	uint8_t unk0x09;								//0x09 TODO
	uint8_t pad0x0A[2];							//0x0A
};

//Duriel
struct D2Act2Quest6Strc						//sizeof 0x68
{
	uint8_t unk0x00;								//0x00 TODO
	uint8_t bDurielKilled;							//0x01
	uint8_t unk0x02;								//0x02 TOOO
	uint8_t bTimerActive;							//0x03
	uint8_t bHoradricOrificeInitialized;			//0x04
	uint8_t bTyraelsDoorInitialized;				//0x05
	uint8_t bUnusedObjectInitialized;				//0x06
	uint8_t unused0x07;							//0x07
	uint8_t bInitJerhynActivated;					//0x08
	uint8_t bTyraelActivated;						//0x09
	uint8_t bEndJerhynActivated;					//0x0A
	uint8_t bDurielLairTombOpen;					//0x0B
	uint8_t bDurielLairPortalNeedsToOpen;			//0x0C
	uint8_t bObjectsNeedUpdate;					//0x0D
	uint8_t bStaffItemsRemoved;					//0x0E
	uint8_t bPortalToLutGholeinOpened;				//0x0F
	uint8_t bHoradricStaffIsMissing;				//0x10
	uint8_t pad0x11[3];							//0x11
	int nQuestStatus;							//0x14
	int nTyraelsDoorObjectMode;					//0x18
	int unk0x1C;								//0x1C
	int nHoradricOrificeGUID;					//0x20
	uint8_t unused0x24[4];							//0x24
	int nUnusedObjectGUID;						//0x28
	int nTyraelsDoorGUID;						//0x2C
	uint8_t unused0x30[4];							//0x30
	int nStaffTombLevelId;						//0x34
	uint8_t bObjectIdsInitialized;					//0x38
	uint8_t pad0x39;								//0x39
	uint16_t nArcaneThings;							//0x3A
	uint8_t bPortalIsOpening;						//0x3C
	uint8_t bRewardedOrCompletedBefore;			//0x3D
	uint8_t pad0x3E[2];							//0x3E
	D2UnitStrc* pTyrael;						//0x40
	uint8_t bCloseToTyrael;						//0x44
	uint8_t pad0x45[3];							//0x45
	int nArcaneThingObjectIds[6];				//0x48
	D2RoomStrc* pDurielRoom;					//0x60
	int unused0x64;								//0x64
};

//Unused Gossip
struct D2Act2Quest7Strc						//sizeof 0x01
{
	uint8_t unk0x00;								//0x00
};

//Greiz Gossip
struct D2Act2Quest8Strc						//sizeof 0x02
{
	uint8_t unk0x00[2];							//0x00
};


//			ACT 3			//

//Hratli Gossip
struct D2Act3Quest0Strc						//sizeof 0x1C
{
	uint8_t bHratliStartCreated;					//0x00
	uint8_t bHratliEndCreated;						//0x01
	uint8_t bHratliEndObjectInitialized;			//0x02
	uint8_t bHratliStartIsPresent;					//0x03
	int nHratliX;								//0x04
	int nHratliY;								//0x08
	int nHratliGUID;							//0x0C
	uint8_t bHratliMapAIChanged;					//0x10
	uint8_t pad0x11[7];							//0x11
	D2MapAIStrc* pHratliMapAI;					//0x18
};

//LamEsen
struct D2Act3Quest1Strc						//sizeof 0xA0
{
	uint8_t bCanGetReward;							//0x00
	uint8_t bTomeDropped;							//0x01
	uint8_t bTomeActive;							//0x02
	uint8_t unused0x03;							//0x03
	uint8_t bTomeAcquired;							//0x04
	uint8_t bPlayerWithTomeDropped;				//0x05
	uint8_t pad0x06[2];							//0x06
	int nTomesInGame;							//0x08
	uint8_t bTomeBroughtToAlkor;					//0x0C
	uint8_t pad0x0D[3];							//0x0D
	int nPlayerWithTomeGUID;					//0x10
	int nTomeGUID;								//0x14
	int nTomeObjectMode;						//0x18
	D2QuestGUIDStrc pGUID;						//0x1C
};

//Khalim
struct D2Act3Quest2Strc						//sizeof 0xB0
{
	uint8_t bSewerStairsInitialized;				//0x00
	uint8_t bInitiallyTalkedToCain;				//0x01
	uint8_t pad0x02[2];							//0x02
	int nSewerStairsObjectGUID;					//0x04
	int nSewerStairsObjectMode;					//0x08
	int nItemDropCount;							//0x0C
	int nEyesDropped;							//0x10
	int nBrainsDropped;							//0x14
	int nHeartsDropped;							//0x18
	int nFlailsDropped;							//0x1C
	int nCubedFlails;							//0x20
	bool bEyeDropped;							//0x24
	bool bBrainDropped;							//0x25
	bool bHeartDropped;							//0x26
	bool bFlailDropped;							//0x27
	int unused0x28;								//0x28
	D2QuestGUIDStrc pGUID;						//0x2C
};

//Gidbinn
struct D2Act3Quest3Strc						//sizeof 0x30
{
	uint8_t bGidbinnDropped;						//0x00
	uint8_t bHratliActivated;						//0x01
	uint8_t bBossHasSpawned;						//0x02
	uint8_t bGidbinnObjectActivated;				//0x03
	uint8_t bBossIsSpawning;						//0x04
	uint8_t bSpawnTimerStarted;					//0x05
	uint8_t bAltarCanBeActivated;					//0x06
	uint8_t bGidbinnBroughtToOrmus;				//0x07
	int nGidbinnX;								//0x08
	int nGidbinnY;								//0x0C
	int nAltarX;								//0x10
	int nAltarY;								//0x14
	uint8_t bGidbinnObjectInitialized;				//0x18
	uint8_t pad0x19[3];							//0x19
	int nGidbinnsInGame;						//0x1C
	uint8_t bPlayerWithGidbinnDropped;				//0x20
	uint8_t pad0x21[3];							//0x21
	int nFetishBossGUID;						//0x24
	int nAltarGUID;								//0x28
	int nAltarObjectMode;						//0x2C
};

//GoldenBird
struct D2Act3Quest4Strc						//sizeof 0x1C
{
	uint8_t bGoldenBirdBroughtToAlkor;				//0x00
	uint8_t unk0x01;								//0x01 TODO
	uint8_t unk0x02;								//0x02 TODO
	uint8_t pad0x03;								//0x03
	int nBossGUID;								//0x04
	uint8_t bTalkedToAlkor;						//0x08
	uint8_t bTalkedToCainOnce;						//0x09
	uint8_t bTalkedToCainTwice;					//0x0A
	uint8_t bTalkedToMeshif;						//0x0B
	uint8_t unk0x0C;								//0x0C TODO
	uint8_t pad0x0D[3];							//0x0D
	int nGoldenBirdsInGame;						//0x10
	uint8_t bJadeFigurineDropped;					//0x14
	uint8_t bPlayerDroppedWithItem;				//0x15
	uint8_t pad0x16[2];							//0x16
	int nCurrentQuestFlag;						//0x18
};

//Council
struct D2Act3Quest5Strc						//sizeof 0x50
{
	int nLastKilledMonsterGUID;					//0x00
	uint8_t bOrmusActivated;						//0x04
	uint8_t bMonstersSpawned;						//0x05
	uint8_t pad0x06[2];							//0x06
	int bQuest1RewardGranted;					//0x08
	uint8_t bOrbSmashed;							//0x0C
	uint8_t bFlailDropped;							//0x0D
	uint8_t bCubeDropped;							//0x0E
	uint8_t pad0x0F;								//0x0F
	int nMonsterGUIDs[6];						//0x10
	uint8_t bCompellingOrbSpawned;					//0x28
	uint8_t pad0x29[3];							//0x29
	int nCompellingOrbGUID;						//0x2C
	int nSpawnCount;							//0x30
	int nMonstersLeftToKill;					//0x34
	int nHits;									//0x38
	int nFlailsMissing;							//0x3C
	int nHoradricCubesMissing;					//0x40
	uint8_t unused0x44[12];						//0x44
};

//Mephisto/BlackTemple
struct D2Act3Quest6Strc						//sizeof 0x34
{
	uint8_t bTimerStarted;							//0x00
	uint8_t bHellGatePortalInitialized;			//0x01
	uint8_t bMephistoBridgeInitialized;			//0x02
	uint8_t bOrmusActivated;						//0x03
	int nHellGatePortalGUID;					//0x04
	int nMephistoBridgeGUID;					//0x08
	int nHellGatePortalObjectMode;				//0x0C
	int nMephistoBridgeObjectMode;				//0x10
	int nSoulStonesDropped;						//0x14
	uint8_t bSoulStoneDropped;						//0x18
	uint8_t pad0x19[3];							//0x19
	int nSoulstonesToDrop;						//0x1C
	uint8_t bNatalyaSpawned;						//0x20
	uint8_t pad0x21[3];							//0x21
	D2MapAIStrc* pNatalyaMapAI;					//0x24
	uint8_t unused0x28[4];							//0x28
	int nNatalyaGUID;							//0x2C
	uint8_t bNatalyaMapAIChanged;					//0x30
	uint8_t pad0x31[3];							//0x31
};

//DarkWanderer
struct D2Act3Quest7Strc						//sizeof 0x14
{
	uint8_t bDarkWandererInitialized;				//0x00
	uint8_t bPrimaryGoalOpen;						//0x01
	uint8_t bWandererCoordsCalculated;				//0x02
	uint8_t pad0x03;								//0x03
	int nDarkWandererX;							//0x04
	int nDarkWandererY;							//0x08
	uint8_t bVileDogsSpawned;						//0x0C
	uint8_t bVileDogSpawnTimerCreated;				//0x0D
	uint8_t pad0x0E[2];							//0x0E
	int nDarkWandererGUID;						//0x10
};


//			ACT 4			//

//Izual
struct D2Act4Quest1Strc						//sizeof 0x1C
{
	int unused0x00;								//0x00
	D2CoordStrc pCoords;						//0x04
	uint8_t unk0x0C;								//0x0C TODO
	uint8_t bIzualGhostIsSpawning;					//0x0D
	uint8_t unused0x0E;							//0x0E
	uint8_t unk0x0F;								//0x0F TODO
	uint8_t bNeedToSpawnIzualGhost;				//0x10
	uint8_t bTyraelActivated;						//0x11
	uint8_t bIzualGhostActivated;					//0x12
	uint8_t pad0x13;								//0x13
	D2UnitStrc* pTargetUnit;					//0x14
	uint8_t bCloseToIzualGhost;					//0x18
	uint8_t pad0x19[3];							//0x19
};

//Diablo
struct D2Act4Quest2Strc						//sizeof 0x4C
{
	uint8_t bTalkedToTyrael;						//0x00
	uint8_t bTimerCreated;							//0x01
	uint8_t bNeedToSpawnDiablo;					//0x02
	uint8_t unk0x03;								//0x03 TODO
	uint8_t bNeedToEndGame;						//0x04
	uint8_t bNeedToWarpPlayers;					//0x05
	uint8_t bDiabloStartPointInitialized;			//0x06
	uint8_t pad0x07;								//0x07
	int nDiabloStartPointObjectGUID;			//0x08
	uint8_t bSealActivated[5];						//0x0C
	uint8_t bDiabloSpawned;						//0x11
	uint8_t unused0x12;							//0x12
	uint8_t bSanctumCleared;						//0x13
	uint8_t bDiabloKilled;							//0x14
	uint8_t bClientsSaved;							//0x15
	uint8_t pad0x16[2];							//0x16
	uint32_t dwTickCount;							//0x18 TODO: What TickCount?
	int nPlayerCount;							//0x1C
	D2UnitStrc* unk0x20;						//0x20 TODO
	D2CoordStrc pSealCoords[3];					//0x24
	D2RoomStrc* pRoom;							//0x3C
	int nLastLastPortalObjectMode;				//0x40
	uint8_t unk0x44;								//0x44 TODO
	uint8_t bPortalToAct5Spawned;					//0x45
	uint8_t pad0x46[2];							//0x46
	int nBossesKilled;							//0x48
};

//Hellforge
struct D2Act4Quest3Strc						//sizeof 0x20
{
	short nHellforgeObjectMode;					//0x00
	uint8_t bCainActivated;						//0x02
	uint8_t bRewardDropsPending;					//0x03
	uint8_t bSoulstoneSmashed;						//0x04
	uint8_t pad0x05[3];							//0x05
	int nGemDropTier;							//0x08
	int nHits;									//0x0C
	int nHellforgeHammersInGame;				//0x10
	int nPlayersInAct;							//0x14
	int unused0x18;								//0x18
	uint8_t bSoulstoneAcquired;					//0x1C
	uint8_t pad0x1D[3];							//0x1D
};


//			ACT 5			//

//Siege
struct D2Act5Quest1Strc						//sizeof 0x18
{
	D2MapAIStrc* pLarzukMapAI;					//0x00
	D2RoomStrc* pRoom;							//0x04
	uint8_t bLarzukStartActivated;					//0x08
	uint8_t unused0x09[7];							//0x09
	int nLarzukGUID;							//0x10
	uint8_t unused0x14;							//0x14
	uint8_t bLarzukSpawned;						//0x15
	uint8_t bLarzukEndActivated;					//0x16
	uint8_t bLarzukMapAIChanged;					//0x17
};

//RescueBarbarians
struct D2Act5Quest2Strc						//sizeof 0x160
{
	D2QuestGUIDStrc pGUID;						//0x00
	uint8_t bQualKehkActivated;					//0x84
	uint8_t unused0x85;							//0x85
	uint8_t bWussiesSpawned[3];					//0x86
	uint8_t pad0x89[3];							//0x89
	D2CoordStrc pWussieCoords[3];				//0x8C
	int nSpawnedWussies;						//0xA4
	int nKilledWussies;							//0xA8
	int nFreedWussies;							//0xAC
	int unk0xB0[15];							//0xB0 TODO: Some UnitGUIDs
	int nPortalGUIDs[3];						//0xEC
	uint8_t bPortalSpawned[3];						//0xF8
	uint8_t pad0xFB;								//0xFB
	int nPortalUpdateInvocations[3];			//0xFC
	uint8_t unk0x108[3];							//0x108 TODO: Some boolean
	uint8_t bChangeToSpecialObjectMode[3];			//0x10C
	uint8_t unk0x10E[3];							//0x10E TODO: Some boolean
	uint8_t pad0x111[3];							//0x111
	int unk0x114[3];							//0x114 TODO: Some Counter
	int nFreedWussieUnitGUIDs[15];				//0x120
	int nWussiesInRangeToDoor;					//0x15C
};

//PrisonOfIce/Anya
struct D2Act5Quest3Strc						//sizeof 0x114
{
	D2QuestGUIDStrc pGUID;						//0x00
	int unk0x84;								//0x84 TODO
	int unk0x88;								//0x88 TODO
	int nTimerInvocations;						//0x8C
	uint8_t bDrehyaIcedMonsterSpawned;				//0x90
	uint8_t bDrehyaMonsterInTownSpawned;			//0x91
	uint8_t bNilathakOutsideTownSpawned;			//0x92
	uint8_t bNihlathakMonsterInTownSpawned;		//0x93
	int nDrehyaMonsterInTownGUID;				//0x94
	int nDrehyaIcedMonsterGUID;					//0x98
	int NihlathakMonsterInTownGUID;				//0x9C
	int nNihlathakOutsideTownGUID;				//0xA0
	uint8_t bFrozenAnyaObjectSpawned;				//0xA4
	uint8_t pad0xA5[3];							//0xA5
	int nFrozenDrehyaObjectGUID;				//0xA8
	uint8_t bMalahIntroActivated;					//0xAC
	uint8_t bChangeToSpecialObjectModeOutsideTown;	//0xAD
	uint8_t bChangeToSpecialObjectModeInTown;		//0xAE
	uint8_t pad0xAF;								//0xAF
	uint8_t unused0xB0;							//0xB0
	uint8_t bDrehyaPortalOutsideTownSpawned;		//0xB1
	uint8_t bDrehyaPortalInTownSpawned;			//0xB2
	uint8_t unused0xB3;							//0xB3
	int nDrehyaPortalInTownGUID;				//0xB4
	uint8_t unused0xB8[8];							//0xB8
	int nDrehyaPortalOutsideTownGUID;			//0xC0
	D2CoordStrc pDrehyaPortalCoords;			//0xC4
	uint8_t unused0xCC[12];						//0xCC
	int nDefrostPotionsInGame;					//0xD8
	int nObjectUpdateInvocations;				//0xDC
	uint8_t bDrehyaPortalCoordsInitialized;		//0xE0
	uint8_t bDrehyaInTownInitialized;				//0xE1
	uint8_t bDefrostPotionAcquired;				//0xE2
	uint8_t bDrehyaIcedRemoved;					//0xE3
	int nDrehyaObjectInTownGUID;				//0xE4
	D2CoordStrc pDrehyaObjectInTownCoords;		//0xE8
	D2CoordStrc pDrehyaObjectOutsideTownCoords;	//0xF0
	int nDrehyaObjectOutsideTownGUID;			//0xF8
	int nFrozenAnyaObjectGUID;					//0xFC
	uint8_t bTimerActive;							//0x100
	uint8_t bMalahActivated;						//0x101
	uint8_t bRewarded;								//0x102
	uint8_t pad0x103;								//0x103
	D2MapAIStrc* pDrehyaMapAI;					//0x104
	D2MapAIStrc* pNilathakMapAI;				//0x108
	uint8_t bDrehyaMapAIChanged;					//0x10C
	uint8_t bNilathakMapAIChanged;					//0x10D
	uint8_t pad0x10E[2];							//0x10E
	int nNihlathakObjectInTownGUID;				//0x110
};

//Nihlathak
struct D2Act5Quest4Strc						//sizeof 0x90
{
	D2QuestGUIDStrc pGUID;						//0x00
	uint8_t unused0x84;							//0x84
	uint8_t bTimerActive;							//0x85
	uint8_t bDrehyaActivated;						//0x86
	uint8_t bNeedsToOpenPortal;					//0x87
	uint8_t bPortalCreated;						//0x88
	uint8_t bWaypointNotActivated;					//0x89
	uint8_t pad0x8A[2];							//0x8A
	D2RoomStrc* pRoom;							//0x8C
};

//Ancients
struct D2Act5Quest5Strc						//sizeof 0x64
{
	uint8_t bAncientsDefeated;						//0x00
	uint8_t bQualKehkActivated;					//0x01
	uint8_t unused0x02[2];							//0x02
	uint8_t bTimerActive;							//0x04
	uint8_t unused0x05[7];							//0x05
	int nActivePortalCount;						//0x0C
	uint8_t unk0x10;								//0x10 TODO
	uint8_t bActivatable;							//0x11
	uint8_t pad0x12[2];							//0x12
	int nAncientStatueGUID[3];					//0x14
	int nAncientMonsterGUID[3];					//0x20
	uint8_t bAncientMonsterSpawned[3];				//0x2C
	uint8_t pad0x2F;								//0x2F
	int nAncientStatueObjectMode[3];			//0x30
	uint8_t bAncienStatueRespawned[3];				//0x3C
	uint8_t pad0x3F;								//0x3F
	int nAncientsSpawned;						//0x40
	int nAncientsAlive;							//0x44
	uint8_t bAncientsActivated;					//0x48
	uint8_t unused0x49;							//0x49
	uint8_t bQuestCompletedBefore;					//0x4A
	uint8_t pad0x4B;								//0x4B
	int nAltarObjectMode;						//0x4C
	int nAltarGUID;								//0x50
	int nLivingPlayersOnSummit;					//0x54
	uint8_t bSummitDoorInitialized;				//0x58
	uint8_t pad0x59[3];							//0x59
	int nSummitDoorGUID;						//0x5C
	int nSummitDoorObjectMode;					//0x60
};

//Baal
struct D2Act5Quest6Strc						//sizeof 0xA8
{
	D2QuestGUIDStrc pGUID;						//0x00
	uint8_t unused0x84;							//0x84
	uint8_t unused0x85;							//0x85
	uint8_t bWorldStoneChamberOpen;				//0x86
	uint8_t pad0x87;								//0x87
	int nPlayersInWorldStoneChamber;			//0x88
	D2RoomStrc* pRoom;							//0x8C
	uint8_t bQuestStarted;							//0x90
	uint8_t pad0x91[3];							//0x91
	int nBaalPortalObjectMode;					//0x94
	uint8_t bLastPortalOpened;						//0x98
	uint8_t pad0x99[3];							//0x99
	int nLastPortalObjectMode;					//0x9C
	uint8_t bZooInitialized;						//0xA0
	uint8_t pad0xA1[3];							//0xA1
	uint16_t nZooMonsterId;							//0xA4
	uint8_t pad0xA6[2];							//0xA6
};

#pragma pack()
