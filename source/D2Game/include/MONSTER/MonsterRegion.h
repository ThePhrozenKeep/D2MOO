#pragma once

#include <Units/Units.h>
#include <Drlg/D2DrlgDrlg.h>

struct D2MonsterFormationStrc;

#pragma pack(1)

struct D2MonRegDataStrc
{
	uint16_t nMonHcIdx;							//0x00
	uint8_t nRarity;							//0x02
	uint8_t nComponentVariantsMax;				//0x03
	uint8_t nComponentVariants[3][16];			//0x04
};

struct D2MonsterRegionStrc
{
	uint8_t nAct;								//0x00
	uint8_t unk0x01[3];						//0x01
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
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

#pragma pack()

//D2Game.0x6FC66260
//1.14d: 0x0054DC40
int32_t __fastcall MONSTERREGION_FindSpawnLocation(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, int32_t* pX, int32_t* pY, int32_t bAvoidWarps);
//D2Game.0x6FC66560
//1.14d: 0x0054E600
D2UnitStrc* __fastcall D2GAME_SpawnPresetMonster_6FC66560(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC66FC0
//1.14d: 0x00451FE0
D2MonStats2Txt* __fastcall MONSTERREGION_GetMonStats2TxtRecord(int32_t nMonsterId);
//D2Game.0x6FC67010
//1.14d: 0x004638A0
uint32_t __fastcall MONSTERREGION_CheckMonStats2Flag(int32_t nMonsterId, int32_t nFlag);
//D2Game.0x6FC67080
//1.14d: 0x00463900
int32_t __fastcall MONSTERREGION_SanitizeMonsterId(int32_t nMonsterId);
//D2Game.0x6FC670A0
//1.14d: 0x0054E3A0
int32_t __fastcall MONSTERREGION_CanRetryPresetSpawn(int32_t nMonsterId);
//D2Game.0x6FC67190
void __fastcall D2GAME_PopulateRoom_6FC67190(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC67570
//1.14d: 0x0054DDC0
int32_t __fastcall MONSTERREGION_FindFormationSpawnLocation(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, D2MonsterFormationStrc* pFormation, int32_t* pX, int32_t* pY);
//D2Game.0x6FC677D0
//1.14d: 0x0054DF80
D2UnitStrc* __fastcall MONSTERREGION_SpawnMonsterGroup(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, uint8_t nMinGroupSize, uint8_t nMaxGroupSize);
//D2Game.0x6FC679F0
//1.14d: 0x0054F060
void __fastcall MONSTERREGION_TrySpawnWanderingMonster(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC67B90
//1.14d: 0x005479C0
void __fastcall MONSTERREGION_InitializeAll(void* pMemPool, D2MonsterRegionStrc** ppMonsterRegion, D2SeedStrc* pSeed, uint32_t nLowSeed, uint8_t nDifficulty, int32_t bExpansion);
//D2Game.0x6FC67F30
//1.14d: 0x00547B50
void __fastcall MONSTERREGION_FreeAll(void* pMemPool, D2MonsterRegionStrc** ppMonsterRegion);
//D2Game.0x6FC67F90
//1.14d: 0x00547BB0
D2MonsterRegionStrc* __fastcall MONSTERREGION_GetMonsterRegionFromLevelId(D2MonsterRegionStrc** ppMonsterRegion, int32_t nLevelId);
//D2Game.0x6FC67FA0
//1.14d: 0x00547BC0
D2MonRegDataStrc* __fastcall MONSTERREGION_GetMonsterComponentData(D2MonsterRegionStrc** ppMonsterRegion, D2ActiveRoomStrc* pRoom, D2UnitStrc* pUnit);
//D2Game.0x6FC68110
//1.14d: 0x00547D20
void __fastcall MONSTERREGION_InitializeForGame(D2GameStrc* pGame);
//D2Game.0x6FC68180
//1.14d: 0x00547D90
void __fastcall MONSTERREGION_RegisterSpawnedMonster(D2MonsterRegionStrc** ppMonsterRegion, D2ActiveRoomStrc* pRoom, D2UnitStrc* pUnit, int32_t bExcludeFromCount);
//D2Game.0x6FC681C0
//1.14d: 0x00547DD0
void __fastcall MONSTERREGION_UpdateSpawnCountForAlignmentChange(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit, int32_t nPreviousAlignment, int32_t nNewAlignment);
//D2Game.0x6FC68240
//1.14d: 0x00547E50
void __fastcall MONSTERREGION_RecordMonsterKill(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit);
//D2Game.0x6FC68280
//1.14d: 0x00547E90
void __fastcall MONSTERREGION_UndoMonsterKill(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit);
//D2Game.0x6FC682C0
//1.14d: 0x00547ED0
void __fastcall MONSTERREGION_AdjustCountsForInactiveMonster(D2MonsterRegionStrc** ppMonRegion, int32_t nSavedLevelId, int32_t nCurrentLevelId, int32_t nAlignment, int32_t bDead, int32_t bExcludeFromCount, D2UnitStrc* pUnit);
