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

struct D2QuestGUIDStrc						//sizeof 0x84
{
	DWORD dwPlayerGUID[32];						//0x00
	short nGUIDCount;							//0x80
	BYTE pad0x82[2];							//0x82
};

struct D2PrisonOfIceRewardStrc				//sizeof 0x08
{
	const DWORD* dwItemCodes;					//0x00
	int nItemCount;								//0x04
};


//			ACT 1			//

//DenOfEvil
struct D2Act1Quest1Strc						//sizeof 0x8C
{
	D2QuestGUIDStrc pQuestGUID;					//0x00
	BYTE bFinished;								//0x84
	BYTE bEnteredDen;							//0x85
	BYTE bAkaraActivated;						//0x86
	BYTE bTimerActive;							//0x87
	int nMonstersLeft;							//0x88
};

//BloodRaven
struct D2Act1Quest2Strc						//sizeof 0x0C
{
	BYTE unk0x00;								//0x00 TODO
	BYTE unk0x01;								//0x01 TODO
	BYTE bBloodravenKilled;						//0x02
	BYTE bKashyaActivated;						//0x03
	int unk0x04;								//0x04 TODO
	int nBloodravenGUID;						//0x08
};

//Malus
struct D2Act1Quest3Strc						//sizeof 0xA4
{
	BYTE unk0x00;								//0x00 TODO
	BYTE bMalusInitialized;						//0x01
	BYTE bCharsiIntroActivated;					//0x02
	BYTE bCharsiEndActivated;					//0x03
	int nMalusGUID;								//0x04
	BYTE unk0x08;								//0x08 TODO
	BYTE unused0x09[9];							//0x09
	WORD unk0x12;								//0x12
	D2QuestGUIDStrc pGUID;						//0x14
	int nMalusObjectMode;						//0x98
	int nMalusItemsInGame;						//0x9C
	BYTE bMalusAcquired;						//0xA0
	BYTE bStartingPlayerHasMalus;				//0xA1
	BYTE pad0xA2[2];							//0xA2
};

//RescueCain
struct D2Act1Quest4Strc						//sizeof 0x1BC
{
	WORD nStoneOrder[5];						//0x00
	WORD unused0x0A;							//0x0A
	WORD nCurrentMonolithNo;					//0x0C
	BYTE pad0x0E[2];							//0x0E
	int nMonolithGUIDs[5];						//0x10
	int unused0x24;								//0x24
	int nInvisibleObjectGUID;					//0x28
	short nCairnStoneInteractions;				//0x2C
	BYTE pad0x2E[2];							//0x2E
	int nInifussTreeGUID;						//0x30
	int nCainGibbetGUID;						//0x34
	int nDecipheredScrollGUID;					//0x38
	int nPlayerGUID;							//0x3C
	int nCairnStoneAlphaGUID;					//0x40
	BYTE bOpenPortalTimerActive;				//0x44
	BYTE bPortalToTristramOpened;				//0x45
	BYTE bTimerActive;							//0x46
	BYTE bInifussTreeInitialized;				//0x47
	BYTE bCageModeChanged;						//0x48
	BYTE bInvisbleObjectInitialized;			//0x49
	BYTE bMonolithStonesOrdered;				//0x4A
	BYTE unk0x4B;								//0x4B TODO
	BYTE unk0x4C;								//0x4C TODO
	BYTE bActivateCairnStones;					//0x4D
	BYTE bDecipheredScrollAcquired;				//0x4E
	BYTE bOperatedWithLastCairnStone;			//0x4F
	BYTE unk0x50;								//0x50 TODO: Cain rescued?
	BYTE bCainInTownSpawned;					//0x51
	BYTE unk0x52;								//0x52 TODO: Cain not spawned?
	BYTE bAkaraIntroActivated;					//0x53
	int nCainGibbetObjectMode;					//0x54
	int unk0x58;								//0x58 TODO
	BYTE bCairnStonesActive[5];					//0x5C
	BYTE bCainRemovedFromTristram;				//0x61
	BYTE bCainNotCreated;						//0x62
	BYTE bQuestCompleteBefore;					//0x63
	BYTE bAkaraActivatedForScroll;				//0x64
	BYTE unused0x65;							//0x65
	BYTE bPortalFromTristramToTownSpawned;		//0x66
	BYTE bCainPortalCoordsInitialized;			//0x67
	int nCainMonsterInTownGUID;					//0x68
	int nCainObjectGUID;						//0x6C
	BYTE bCainObjectInitialized;				//0x70
	BYTE pad0x71[3];							//0x71
	D2UnitStrc* pPlayerInTristram;				//0x74
	BYTE bScrollAcquired;						//0x78
	BYTE pad0x79[3];							//0x79
	int nScrollsInGame;							//0x7C
	int nObjectUpdateInvocations;				//0x80
	D2CoordStrc pCainStartPositionCoords;		//0x84
	int nGoldPilesToDropFromWirtsBody;			//0x8C
	BYTE bWirtsBodyInitialized;					//0x90
	BYTE bChangePortalOutsideTownToSpecialMode;	//0x91
	BYTE bChangePortalInTownToSpecialMode;		//0x92
	BYTE bCainInTristramDeactivated;			//0x93
	BYTE unused0x94;							//0x94
	BYTE bCainPortalOutsideTownSpawned;			//0x95
	BYTE bCainPortalInTownSpawned;				//0x96
	BYTE unused0x97;							//0x97
	int nCainPortalOutsideTownGUID;				//0x98
	D2CoordStrc pCainPortalOutsideTownCoords;	//0x9C
	int nCainPortalInTownGUID;					//0xA4
	D2CoordStrc pCainPortalInTownCoords;		//0xA8
	BYTE unused0xB0[4];							//0xB0
	D2QuestGUIDStrc pQuestGUID;					//0xB4
	D2QuestGUIDStrc pGUID;						//0x138
};

//Countess
struct D2Act1Quest5Strc						//sizeof 0x120
{
	int nUnitGUIDs1[12];						//0x00 TODO: Which UnitGUIDs?
	short nUnitCount1;							//0x30
	BYTE pad0x32[2];							//0x32
	int nUnitGUIDs2[12];						//0x34 TODO: Which UnitGUIDs?
	short nUnitCount2;							//0x64
	BYTE pad0x66[2];							//0x66
	int nObjectGUIDs[8];						//0x68 TODO: Which objects?
	WORD nObjectCount;							//0x88
	BYTE pad0x8A[2];							//0x8A
	D2QuestGUIDStrc pGUID;						//0x8C
	D2CoordStrc pCoord;							//0x110
	BYTE bCountessKilled;						//0x118
	BYTE bDeathMissilesCreated;					//0x119
	BYTE bTriggerSeqFilter;						//0x11A
	BYTE bTowerTomeActivated;					//0x11B
	BYTE unk0x11C;								//0x11C TODO
	BYTE pad0x11D[3];							//0x11D
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
	BYTE bAndarielKilled;						//0x194
	BYTE bCainActivated;						//0x195
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
	BYTE bRadamentKilled;						//0x00
	BYTE pad0x01[3];							//0x01
	D2RoomStrc* pRoom;							//0x04
	BYTE bAtmaActivated;						//0x08
	BYTE unk0x09;								//0x09 TODO
	BYTE bStatusTimerStarted;					//0x0A
	BYTE pad0x0B;								//0x0B
	int nSkillBookDropCount;					//0x0C
	BYTE bRewardPending;						//0x10
	BYTE pad0x11[3];							//0x11
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
	BYTE bHoradricScrollDropped;				//0x28
	BYTE bCubeDropped;							//0x29
	BYTE bStaffCubed;							//0x2A
	BYTE unk0x2B;								//0x2B TODO
	int nPlayerGUID;							//0x2C
};

//TaintedSun
struct D2Act2Quest3Strc						//sizeof 0xA0
{
	BYTE unused0x00;							//0x00
	BYTE bDarkenTimerCreated;					//0x01
	BYTE bRewarded;								//0x02
	BYTE bBlackened;							//0x03
	BYTE bAltarDestroyed;						//0x04
	BYTE bAltarInitialized;						//0x05
	BYTE unk0x06;								//0x06 TODO
	BYTE pad0x07;								//0x07
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
	BYTE unused0x00[4];							//0x00
	BYTE bDrognanActivated;						//0x04
	BYTE pad0x05[3];							//0x05
	D2RoomStrc* pRoom;							//0x08
	BYTE bJerhynStartInitialized;				//0x0C
	BYTE bJerhynPalaceInitialized;				//0x0D
	BYTE bPalaceOpen;							//0x0E
	BYTE unk0x0F;								//0x0F TODO
	BYTE unk0x10;								//0x10 TODO
	BYTE bHaremBlockerInitialized;				//0x11
	BYTE unk0x12;								//0x12 TODO
	BYTE unk0x13;								//0x13 TODO
	BYTE unk0x14;								//0x14 TODO
	BYTE bJerhynCoordinatesStored;				//0x15
	BYTE bPortalToCanyonOpen;					//0x16
	BYTE unused0x17;							//0x17
	BYTE unk0x18;								//0x18 TODO
	BYTE unk0x19;								//0x19 TODO
	BYTE bPlayerCloseToHaremBlocker;			//0x1A
	BYTE pad0x1B;								//0x1B
	int nPlayerCloseToHaremBlockerGUID;			//0x1C
	D2CoordStrc pHaremBlockerCoords;			//0x20
	D2CoordStrc pJerhynCoords;					//0x28
	D2CoordStrc pGuardCoords;					//0x30
	int nHaremBlockerGUID;						//0x38
	int nJerhynGUID;							//0x3C
	WORD nHaremBlockerObjectMode;				//0x40
	WORD nPortalModeInSanctuary;				//0x42
	WORD nPortalModeInCellar;					//0x44
	BYTE bIsHaremBlockerNeutral;				//0x46
	BYTE pad0x47;								//0x47
};

//Summoner
struct D2Act2Quest5Strc						//sizeof 0x0C
{
	BYTE bSummonerKilled;						//0x00
	BYTE unk0x01;								//0x01 TODO
	BYTE pad0x02[2];							//0x02
	D2RoomStrc* pRoom;							//0x04
	BYTE bStatusTimerStarted;					//0x08
	BYTE unk0x09;								//0x09 TODO
	BYTE pad0x0A[2];							//0x0A
};

//Duriel
struct D2Act2Quest6Strc						//sizeof 0x68
{
	BYTE unk0x00;								//0x00 TODO
	BYTE bDurielKilled;							//0x01
	BYTE unk0x02;								//0x02 TOOO
	BYTE bTimerActive;							//0x03
	BYTE bHoradricOrificeInitialized;			//0x04
	BYTE bTyraelsDoorInitialized;				//0x05
	BYTE bUnusedObjectInitialized;				//0x06
	BYTE unused0x07;							//0x07
	BYTE bInitJerhynActivated;					//0x08
	BYTE bTyraelActivated;						//0x09
	BYTE bEndJerhynActivated;					//0x0A
	BYTE bDurielLairTombOpen;					//0x0B
	BYTE bDurielLairPortalNeedsToOpen;			//0x0C
	BYTE bObjectsNeedUpdate;					//0x0D
	BYTE bStaffItemsRemoved;					//0x0E
	BYTE bPortalToLutGholeinOpened;				//0x0F
	BYTE bHoradricStaffIsMissing;				//0x10
	BYTE pad0x11[3];							//0x11
	int nQuestStatus;							//0x14
	int nTyraelsDoorObjectMode;					//0x18
	int unk0x1C;								//0x1C
	int nHoradricOrificeGUID;					//0x20
	BYTE unused0x24[4];							//0x24
	int nUnusedObjectGUID;						//0x28
	int nTyraelsDoorGUID;						//0x2C
	BYTE unused0x30[4];							//0x30
	int nStaffTombLevelId;						//0x34
	BYTE bObjectIdsInitialized;					//0x38
	BYTE pad0x39;								//0x39
	WORD nArcaneThings;							//0x3A
	BYTE bPortalIsOpening;						//0x3C
	BYTE bRewardedOrCompletedBefore;			//0x3D
	BYTE pad0x3E[2];							//0x3E
	D2UnitStrc* pTyrael;						//0x40
	BYTE bCloseToTyrael;						//0x44
	BYTE pad0x45[3];							//0x45
	int nArcaneThingObjectIds[6];				//0x48
	D2RoomStrc* pDurielRoom;					//0x60
	int unused0x64;								//0x64
};

//Unused Gossip
struct D2Act2Quest7Strc						//sizeof 0x01
{
	BYTE unk0x00;								//0x00
};

//Greiz Gossip
struct D2Act2Quest8Strc						//sizeof 0x02
{
	BYTE unk0x00[2];							//0x00
};


//			ACT 3			//

//Hratli Gossip
struct D2Act3Quest0Strc						//sizeof 0x1C
{
	BYTE bHratliStartCreated;					//0x00
	BYTE bHratliEndCreated;						//0x01
	BYTE bHratliEndObjectInitialized;			//0x02
	BYTE bHratliStartIsPresent;					//0x03
	int nHratliX;								//0x04
	int nHratliY;								//0x08
	int nHratliGUID;							//0x0C
	BYTE bHratliMapAIChanged;					//0x10
	BYTE pad0x11[7];							//0x11
	D2MapAIStrc* pHratliMapAI;					//0x18
};

//LamEsen
struct D2Act3Quest1Strc						//sizeof 0xA0
{
	BYTE bCanGetReward;							//0x00
	BYTE bTomeDropped;							//0x01
	BYTE bTomeActive;							//0x02
	BYTE unused0x03;							//0x03
	BYTE bTomeAcquired;							//0x04
	BYTE bPlayerWithTomeDropped;				//0x05
	BYTE pad0x06[2];							//0x06
	int nTomesInGame;							//0x08
	BYTE bTomeBroughtToAlkor;					//0x0C
	BYTE pad0x0D[3];							//0x0D
	int nPlayerWithTomeGUID;					//0x10
	int nTomeGUID;								//0x14
	int nTomeObjectMode;						//0x18
	D2QuestGUIDStrc pGUID;						//0x1C
};

//Khalim
struct D2Act3Quest2Strc						//sizeof 0xB0
{
	BYTE bSewerStairsInitialized;				//0x00
	BYTE bInitiallyTalkedToCain;				//0x01
	BYTE pad0x02[2];							//0x02
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
	BYTE bGidbinnDropped;						//0x00
	BYTE bHratliActivated;						//0x01
	BYTE bBossHasSpawned;						//0x02
	BYTE bGidbinnObjectActivated;				//0x03
	BYTE bBossIsSpawning;						//0x04
	BYTE bSpawnTimerStarted;					//0x05
	BYTE bAltarCanBeActivated;					//0x06
	BYTE bGidbinnBroughtToOrmus;				//0x07
	int nGidbinnX;								//0x08
	int nGidbinnY;								//0x0C
	int nAltarX;								//0x10
	int nAltarY;								//0x14
	BYTE bGidbinnObjectInitialized;				//0x18
	BYTE pad0x19[3];							//0x19
	int nGidbinnsInGame;						//0x1C
	BYTE bPlayerWithGidbinnDropped;				//0x20
	BYTE pad0x21[3];							//0x21
	int nFetishBossGUID;						//0x24
	int nAltarGUID;								//0x28
	int nAltarObjectMode;						//0x2C
};

//GoldenBird
struct D2Act3Quest4Strc						//sizeof 0x1C
{
	BYTE bGoldenBirdBroughtToAlkor;				//0x00
	BYTE unk0x01;								//0x01 TODO
	BYTE unk0x02;								//0x02 TODO
	BYTE pad0x03;								//0x03
	int nBossGUID;								//0x04
	BYTE bTalkedToAlkor;						//0x08
	BYTE bTalkedToCainOnce;						//0x09
	BYTE bTalkedToCainTwice;					//0x0A
	BYTE bTalkedToMeshif;						//0x0B
	BYTE unk0x0C;								//0x0C TODO
	BYTE pad0x0D[3];							//0x0D
	int nGoldenBirdsInGame;						//0x10
	BYTE bJadeFigurineDropped;					//0x14
	BYTE bPlayerDroppedWithItem;				//0x15
	BYTE pad0x16[2];							//0x16
	int nCurrentQuestFlag;						//0x18
};

//Council
struct D2Act3Quest5Strc						//sizeof 0x50
{
	int nLastKilledMonsterGUID;					//0x00
	BYTE bOrmusActivated;						//0x04
	BYTE bMonstersSpawned;						//0x05
	BYTE pad0x06[2];							//0x06
	int bQuest1RewardGranted;					//0x08
	BYTE bOrbSmashed;							//0x0C
	BYTE bFlailDropped;							//0x0D
	BYTE bCubeDropped;							//0x0E
	BYTE pad0x0F;								//0x0F
	int nMonsterGUIDs[6];						//0x10
	BYTE bCompellingOrbSpawned;					//0x28
	BYTE pad0x29[3];							//0x29
	int nCompellingOrbGUID;						//0x2C
	int nSpawnCount;							//0x30
	int nMonstersLeftToKill;					//0x34
	int nHits;									//0x38
	int nFlailsMissing;							//0x3C
	int nHoradricCubesMissing;					//0x40
	BYTE unused0x44[12];						//0x44
};

//Mephisto/BlackTemple
struct D2Act3Quest6Strc						//sizeof 0x34
{
	BYTE bTimerStarted;							//0x00
	BYTE bHellGatePortalInitialized;			//0x01
	BYTE bMephistoBridgeInitialized;			//0x02
	BYTE bOrmusActivated;						//0x03
	int nHellGatePortalGUID;					//0x04
	int nMephistoBridgeGUID;					//0x08
	int nHellGatePortalObjectMode;				//0x0C
	int nMephistoBridgeObjectMode;				//0x10
	int nSoulStonesDropped;						//0x14
	BYTE bSoulStoneDropped;						//0x18
	BYTE pad0x19[3];							//0x19
	int nSoulstonesToDrop;						//0x1C
	BYTE bNatalyaSpawned;						//0x20
	BYTE pad0x21[3];							//0x21
	D2MapAIStrc* pNatalyaMapAI;					//0x24
	BYTE unused0x28[4];							//0x28
	int nNatalyaGUID;							//0x2C
	BYTE bNatalyaMapAIChanged;					//0x30
	BYTE pad0x31[3];							//0x31
};

//DarkWanderer
struct D2Act3Quest7Strc						//sizeof 0x14
{
	BYTE bDarkWandererInitialized;				//0x00
	BYTE bPrimaryGoalOpen;						//0x01
	BYTE bWandererCoordsCalculated;				//0x02
	BYTE pad0x03;								//0x03
	int nDarkWandererX;							//0x04
	int nDarkWandererY;							//0x08
	BYTE bVileDogsSpawned;						//0x0C
	BYTE bVileDogSpawnTimerCreated;				//0x0D
	BYTE pad0x0E[2];							//0x0E
	int nDarkWandererGUID;						//0x10
};


//			ACT 4			//

//Izual
struct D2Act4Quest1Strc						//sizeof 0x1C
{
	int unused0x00;								//0x00
	D2CoordStrc pCoords;						//0x04
	BYTE unk0x0C;								//0x0C TODO
	BYTE bIzualGhostIsSpawning;					//0x0D
	BYTE unused0x0E;							//0x0E
	BYTE unk0x0F;								//0x0F TODO
	BYTE bNeedToSpawnIzualGhost;				//0x10
	BYTE bTyraelActivated;						//0x11
	BYTE bIzualGhostActivated;					//0x12
	BYTE pad0x13;								//0x13
	D2UnitStrc* pTargetUnit;					//0x14
	BYTE bCloseToIzualGhost;					//0x18
	BYTE pad0x19[3];							//0x19
};

//Diablo
struct D2Act4Quest2Strc						//sizeof 0x4C
{
	BYTE bTalkedToTyrael;						//0x00
	BYTE bTimerCreated;							//0x01
	BYTE bNeedToSpawnDiablo;					//0x02
	BYTE unk0x03;								//0x03 TODO
	BYTE bNeedToEndGame;						//0x04
	BYTE bNeedToWarpPlayers;					//0x05
	BYTE bDiabloStartPointInitialized;			//0x06
	BYTE pad0x07;								//0x07
	int nDiabloStartPointObjectGUID;			//0x08
	BYTE bSealActivated[5];						//0x0C
	BYTE bDiabloSpawned;						//0x11
	BYTE unused0x12;							//0x12
	BYTE bSanctumCleared;						//0x13
	BYTE bDiabloKilled;							//0x14
	BYTE bClientsSaved;							//0x15
	BYTE pad0x16[2];							//0x16
	DWORD dwTickCount;							//0x18 TODO: What TickCount?
	int nPlayerCount;							//0x1C
	D2UnitStrc* unk0x20;						//0x20 TODO
	D2CoordStrc pSealCoords[3];					//0x24
	D2RoomStrc* pRoom;							//0x3C
	int nLastLastPortalObjectMode;				//0x40
	BYTE unk0x44;								//0x44 TODO
	BYTE bPortalToAct5Spawned;					//0x45
	BYTE pad0x46[2];							//0x46
	int nBossesKilled;							//0x48
};

//Hellforge
struct D2Act4Quest3Strc						//sizeof 0x20
{
	short nHellforgeObjectMode;					//0x00
	BYTE bCainActivated;						//0x02
	BYTE bRewardDropsPending;					//0x03
	BYTE bSoulstoneSmashed;						//0x04
	BYTE pad0x05[3];							//0x05
	int nGemDropTier;							//0x08
	int nHits;									//0x0C
	int nHellforgeHammersInGame;				//0x10
	int nPlayersInAct;							//0x14
	int unused0x18;								//0x18
	BYTE bSoulstoneAcquired;					//0x1C
	BYTE pad0x1D[3];							//0x1D
};


//			ACT 5			//

//Siege
struct D2Act5Quest1Strc						//sizeof 0x18
{
	D2MapAIStrc* pLarzukMapAI;					//0x00
	D2RoomStrc* pRoom;							//0x04
	BYTE bLarzukStartActivated;					//0x08
	BYTE unused0x09[7];							//0x09
	int nLarzukGUID;							//0x10
	BYTE unused0x14;							//0x14
	BYTE bLarzukSpawned;						//0x15
	BYTE bLarzukEndActivated;					//0x16
	BYTE bLarzukMapAIChanged;					//0x17
};

//RescueBarbarians
struct D2Act5Quest2Strc						//sizeof 0x160
{
	D2QuestGUIDStrc pGUID;						//0x00
	BYTE bQualKehkActivated;					//0x84
	BYTE unused0x85;							//0x85
	BYTE bWussiesSpawned[3];					//0x86
	BYTE pad0x89[3];							//0x89
	D2CoordStrc pWussieCoords[3];				//0x8C
	int nSpawnedWussies;						//0xA4
	int nKilledWussies;							//0xA8
	int nFreedWussies;							//0xAC
	int unk0xB0[15];							//0xB0 TODO: Some UnitGUIDs
	int nPortalGUIDs[3];						//0xEC
	BYTE bPortalSpawned[3];						//0xF8
	BYTE pad0xFB;								//0xFB
	int nPortalUpdateInvocations[3];			//0xFC
	BYTE unk0x108[3];							//0x108 TODO: Some boolean
	BYTE bChangeToSpecialObjectMode[3];			//0x10C
	BYTE unk0x10E[3];							//0x10E TODO: Some boolean
	BYTE pad0x111[3];							//0x111
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
	BYTE bDrehyaIcedMonsterSpawned;				//0x90
	BYTE bDrehyaMonsterInTownSpawned;			//0x91
	BYTE bNilathakOutsideTownSpawned;			//0x92
	BYTE bNihlathakMonsterInTownSpawned;		//0x93
	int nDrehyaMonsterInTownGUID;				//0x94
	int nDrehyaIcedMonsterGUID;					//0x98
	int NihlathakMonsterInTownGUID;				//0x9C
	int nNihlathakOutsideTownGUID;				//0xA0
	BYTE bFrozenAnyaObjectSpawned;				//0xA4
	BYTE pad0xA5[3];							//0xA5
	int nFrozenDrehyaObjectGUID;				//0xA8
	BYTE bMalahIntroActivated;					//0xAC
	BYTE bChangeToSpecialObjectModeOutsideTown;	//0xAD
	BYTE bChangeToSpecialObjectModeInTown;		//0xAE
	BYTE pad0xAF;								//0xAF
	BYTE unused0xB0;							//0xB0
	BYTE bDrehyaPortalOutsideTownSpawned;		//0xB1
	BYTE bDrehyaPortalInTownSpawned;			//0xB2
	BYTE unused0xB3;							//0xB3
	int nDrehyaPortalInTownGUID;				//0xB4
	BYTE unused0xB8[8];							//0xB8
	int nDrehyaPortalOutsideTownGUID;			//0xC0
	D2CoordStrc pDrehyaPortalCoords;			//0xC4
	BYTE unused0xCC[12];						//0xCC
	int nDefrostPotionsInGame;					//0xD8
	int nObjectUpdateInvocations;				//0xDC
	BYTE bDrehyaPortalCoordsInitialized;		//0xE0
	BYTE bDrehyaInTownInitialized;				//0xE1
	BYTE bDefrostPotionAcquired;				//0xE2
	BYTE bDrehyaIcedRemoved;					//0xE3
	int nDrehyaObjectInTownGUID;				//0xE4
	D2CoordStrc pDrehyaObjectInTownCoords;		//0xE8
	D2CoordStrc pDrehyaObjectOutsideTownCoords;	//0xF0
	int nDrehyaObjectOutsideTownGUID;			//0xF8
	int nFrozenAnyaObjectGUID;					//0xFC
	BYTE bTimerActive;							//0x100
	BYTE bMalahActivated;						//0x101
	BYTE bRewarded;								//0x102
	BYTE pad0x103;								//0x103
	D2MapAIStrc* pDrehyaMapAI;					//0x104
	D2MapAIStrc* pNilathakMapAI;				//0x108
	BYTE bDrehyaMapAIChanged;					//0x10C
	BYTE bNilathakMapAIChanged;					//0x10D
	BYTE pad0x10E[2];							//0x10E
	int nNihlathakObjectInTownGUID;				//0x110
};

//Nihlathak
struct D2Act5Quest4Strc						//sizeof 0x90
{
	D2QuestGUIDStrc pGUID;						//0x00
	BYTE unused0x84;							//0x84
	BYTE bTimerActive;							//0x85
	BYTE bDrehyaActivated;						//0x86
	BYTE bNeedsToOpenPortal;					//0x87
	BYTE bPortalCreated;						//0x88
	BYTE bWaypointNotActivated;					//0x89
	BYTE pad0x8A[2];							//0x8A
	D2RoomStrc* pRoom;							//0x8C
};

//Ancients
struct D2Act5Quest5Strc						//sizeof 0x64
{
	BYTE bAncientsDefeated;						//0x00
	BYTE bQualKehkActivated;					//0x01
	BYTE unused0x02[2];							//0x02
	BYTE bTimerActive;							//0x04
	BYTE unused0x05[7];							//0x05
	int nActivePortalCount;						//0x0C
	BYTE unk0x10;								//0x10 TODO
	BYTE bActivatable;							//0x11
	BYTE pad0x12[2];							//0x12
	int nAncientStatueGUID[3];					//0x14
	int nAncientMonsterGUID[3];					//0x20
	BYTE bAncientMonsterSpawned[3];				//0x2C
	BYTE pad0x2F;								//0x2F
	int nAncientStatueObjectMode[3];			//0x30
	BYTE bAncienStatueRespawned[3];				//0x3C
	BYTE pad0x3F;								//0x3F
	int nAncientsSpawned;						//0x40
	int nAncientsAlive;							//0x44
	BYTE bAncientsActivated;					//0x48
	BYTE unused0x49;							//0x49
	BYTE bQuestCompletedBefore;					//0x4A
	BYTE pad0x4B;								//0x4B
	int nAltarObjectMode;						//0x4C
	int nAltarGUID;								//0x50
	int nLivingPlayersOnSummit;					//0x54
	BYTE bSummitDoorInitialized;				//0x58
	BYTE pad0x59[3];							//0x59
	int nSummitDoorGUID;						//0x5C
	int nSummitDoorObjectMode;					//0x60
};

//Baal
struct D2Act5Quest6Strc						//sizeof 0xA8
{
	D2QuestGUIDStrc pGUID;						//0x00
	BYTE unused0x84;							//0x84
	BYTE unused0x85;							//0x85
	BYTE bWorldStoneChamberOpen;				//0x86
	BYTE pad0x87;								//0x87
	int nPlayersInWorldStoneChamber;			//0x88
	D2RoomStrc* pRoom;							//0x8C
	BYTE bQuestStarted;							//0x90
	BYTE pad0x91[3];							//0x91
	int nBaalPortalObjectMode;					//0x94
	BYTE bLastPortalOpened;						//0x98
	BYTE pad0x99[3];							//0x99
	int nLastPortalObjectMode;					//0x9C
	BYTE bZooInitialized;						//0xA0
	BYTE pad0xA1[3];							//0xA1
	WORD nZooMonsterId;							//0xA4
	BYTE pad0xA6[2];							//0xA6
};

#pragma pack()
