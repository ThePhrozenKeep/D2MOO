#pragma once

#include <Units/Units.h>
#include <Drlg/D2DrlgDrlg.h>


#pragma pack(push, 1)
struct D2UnkMonCreateStrc
{
	D2GameStrc* pGame;						//0x00
	D2ActiveRoomStrc* pRoom;						//0x04
	D2RoomCoordListStrc* pRoomCoordList;	//0x08
	int32_t nMonsterId;						//0x0C
	int32_t nAnimMode;						//0x10
	int32_t nUnitGUID;						//0x14
	int32_t nX;								//0x18
	int32_t nY;								//0x1C
	int32_t field_20;						//0x20
	int16_t nFlags;							//0x24
};

struct D2UnkMonCreateStrc3
{
	int32_t nXOffset;						//0x00
	int32_t nYOffset;						//0x04
	int32_t nSpawnType;						//0x08
};

struct D2UnkMonCreateStrc2
{
	int32_t nRecordCount;					//0x00
	int32_t unk0x04;						//0x04
	uint8_t unk0x08;						//0x08
	uint8_t pad0x09[3];						//0x09
	D2UnkMonCreateStrc3 records[4];			//0x0C - Could be variable length, but at both locations where it's used, it's 4
};
#pragma pack(pop)


//D2Game.0x6FC68350
int32_t __fastcall sub_6FC68350(int32_t nMonsterId, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t a5);
//D2Game.0x6FC68630
int32_t __fastcall sub_6FC68630(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, D2UnitStrc* pTarget, int32_t nX, int32_t nY);
//D2Game.0x6FC68CC0
int32_t __fastcall MONSTERSPAWN_GetResurrectMode(D2UnitStrc* pUnit, int32_t a2);
//D2Game.0x6FC68D70
D2UnitStrc* __fastcall sub_6FC68D70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t a5, int16_t nFlags);
//D2Game.0x6FC68E30
D2UnitStrc* __fastcall D2GAME_SpawnNormalMonster_6FC68E30(D2UnkMonCreateStrc* pMonCreate);



//D2Game.0x6FC69B60
void __fastcall MONSTERSPAWN_EquipAncientBarbarians(D2UnitStrc* pUnit);
//D2Game.0x6FC69C00
void __fastcall sub_6FC69C00(int32_t a1, D2UnitStrc* a2);
//D2Game.0x6FC69F10
D2UnitStrc* __fastcall D2GAME_SpawnMonster_6FC69F10(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a7, int16_t nFlags);
//D2Game.0x6FC69F70
D2UnitStrc* __fastcall sub_6FC69F70(D2GameStrc* pGame, D2RoomCoordListStrc* pRoomCoordList, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t a6, int16_t nFlags);
//D2Game.0x6FC6A030
D2UnitStrc* __fastcall sub_6FC6A030(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a8, int16_t nFlags);
//D2Game.0x6FC6A090
D2UnitStrc* __fastcall sub_6FC6A090(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int16_t nFlags);
//D2Game.0x6FC6A0F0
D2UnitStrc* __fastcall sub_6FC6A0F0(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nMode, int32_t nUnitId, int32_t a8, int16_t nFlags);
//D2Game.0x6FC6A150
D2UnitStrc* __fastcall sub_6FC6A150(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t a5, int16_t nFlags);
//D2Game.0x6FC6A230
int32_t __fastcall sub_6FC6A230(D2GameStrc* pGame, D2UnitStrc* pOwner, int32_t nMonsterId, int32_t nAnimMode, int32_t a5, int32_t nCount, int16_t nFlags);
//D2Game.0x6FC6A350
int32_t __fastcall sub_6FC6A350(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, D2UnitStrc* pOwner, int32_t nClassId, int32_t nAnimMode, D2UnkMonCreateStrc2* a8, int16_t nFlags);
//D2Game.0x6FC6A810
int32_t __fastcall sub_6FC6A810(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t a3, int32_t a4, D2UnitStrc* pTargetUnit, int32_t a6, int16_t a7);
//D2Game.0x6FC6A8C0
int32_t __fastcall sub_6FC6A8C0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t nCount, int32_t a6, int16_t nFlags);
//D2Game.0x6FC6AA70
int32_t __fastcall MONSTERSPAWN_SpawnRandomMonsterForLevel(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY);
