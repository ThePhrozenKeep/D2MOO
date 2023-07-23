#pragma once

#include <Units/Units.h>
#include <Drlg/D2DrlgDrlg.h>

struct D2UnkMonCreateStrc2;

#pragma pack(1)

struct D2MonRegDataStrc
{
	uint16_t nMonHcIdx;							//0x00
	uint8_t nRarity;							//0x02
	uint8_t unk0x03;							//0x03
	uint8_t unk0x04[3][16];						//0x04
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
int32_t __fastcall sub_6FC66260(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nSuperUniqueId, int32_t* pX, int32_t* pY, int32_t a7);
//D2Game.0x6FC66560
D2UnitStrc* __fastcall D2GAME_SpawnPresetMonster_6FC66560(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC66FC0
D2MonStats2Txt* __fastcall MONSTERREGION_GetMonStats2TxtRecord(int32_t nMonsterId);
//D2Game.0x6FC67010
uint32_t __fastcall MONSTERREGION_CheckMonStats2Flag(int32_t nMonsterId, int32_t nFlag);
//D2Game.0x6FC67080
int32_t __fastcall MONSTERREGION_SanitizeMonsterId(int32_t nMonsterId);
//D2Game.0x6FC670A0
int32_t __fastcall sub_6FC670A0(int32_t nMonsterId);
//D2Game.0x6FC67190
void __fastcall D2GAME_PopulateRoom_6FC67190(D2GameStrc* pGame, D2RoomStrc* pRoom);
//D2Game.0x6FC67570
int32_t __fastcall sub_6FC67570(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, D2UnkMonCreateStrc2* a5, int32_t* pX, int32_t* pY);
//D2Game.0x6FC677D0
D2UnitStrc* __fastcall sub_6FC677D0(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, uint8_t nMin, uint8_t nMax);
//D2Game.0x6FC679F0
void __fastcall sub_6FC679F0(D2GameStrc* pGame, D2RoomStrc* pRoom);
//D2Game.0x6FC67B90
void __fastcall MONSTERREGION_InitializeAll(void* pMemPool, D2MonsterRegionStrc** ppMonsterRegion, D2SeedStrc* pSeed, uint32_t nLowSeed, uint8_t nDifficulty, int32_t bExpansion);
//D2Game.0x6FC67F30
void __fastcall MONSTERREGION_FreeAll(void* pMemPool, D2MonsterRegionStrc** ppMonsterRegion);
//D2Game.0x6FC67F90
D2MonsterRegionStrc* __fastcall MONSTERREGION_GetMonsterRegionFromLevelId(D2MonsterRegionStrc** ppMonsterRegion, int32_t nLevelId);
//D2Game.0x6FC67FA0
D2MonRegDataStrc* __fastcall sub_6FC67FA0(D2MonsterRegionStrc** ppMonsterRegion, D2RoomStrc* pRoom, D2UnitStrc* pUnit);
//D2Game.0x6FC68110
void __fastcall sub_6FC68110(D2GameStrc* pGame);
//D2Game.0x6FC68180
void __fastcall sub_6FC68180(D2MonsterRegionStrc** ppMonsterRegion, D2RoomStrc* pRoom, D2UnitStrc* pUnit, int32_t bToggleFlag);
//D2Game.0x6FC681C0
void __fastcall sub_6FC681C0(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit, int32_t nPreviousAlignment, int32_t nNewAlignment);
//D2Game.0x6FC68240
void __fastcall sub_6FC68240(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit);
//D2Game.0x6FC68280
void __fastcall sub_6FC68280(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit);
//D2Game.0x6FC682C0
void __fastcall sub_6FC682C0(D2MonsterRegionStrc** ppMonRegion, int32_t nLevelId1, int32_t nLevelId2, int32_t nAlignment, int32_t bDead, int32_t a6, D2UnitStrc* pUnit);
