#pragma once

#include "D2Structs.h"
#include <Units/Units.h>
#include <OBJECTS/ObjMode.h>
#include <D2DataTbls.h>

#pragma pack(1)

struct D2CubeItemStrc
{
	D2UnitStrc* pItem;						//0x00
	int32_t nClassId;						//0x04
	int32_t nItemLevel;						//0x08
};

typedef BOOL(__fastcall* SPECIALCUBEFN)(D2GameStrc*, D2UnitStrc*);

struct D2CubeTableStrc
{
	SPECIALCUBEFN pFunc;					//0x00
};

#pragma pack()

//D2Game.0x6FC8F3D0
int32_t __fastcall OBJECTS_OperateFunction32_Bank(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC8F450
int32_t __fastcall PLRTRADE_CheckCubeInput(D2GameStrc* pGame, D2InventoryStrc* pInventory, D2CubeMainTxt* pCubeMainTxt, int32_t nInputIndex, D2CubeItemStrc* pCubeItem, int32_t* a6);
//D2Game.0x6FC8FE40
int16_t __fastcall PLRTRADE_RollRandomItemClassOfSameType(D2GameStrc* pGame, int32_t nItemLevel, int32_t nItemId);
//D2Game.0x6FC90010
void __fastcall PLRTRADE_CreateCubeOutputs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CubeMainTxt* pCubeMainTxt, D2CubeItemStrc* pCubeItem);
//D2Game.0x6FC90A60
void __fastcall PLRTRADE_Free(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC90AE0
void __fastcall sub_6FC90AE0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3);
//D2Game.0x6FC90BE0
void __fastcall sub_6FC90BE0(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FC90C20
void __fastcall sub_6FC90C20(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC90D70
void __fastcall PLRTRADE_SendEventPacketToPlayer(D2UnitStrc* pPlayer, char nType, char* szSource);
//D2Game.0x6FC90DE0
void __fastcall PLRTRADE_TryToTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FC91050
void __fastcall sub_6FC91050(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, D2PlayerDataStrc* pPlayerData1, D2PlayerDataStrc* pPlayerData2, int32_t nTradeState);
//D2Game.0x6FC91250
int32_t __fastcall sub_6FC91250(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint16_t a3, int32_t a4);
//D2Game.0x6FC92130
void __fastcall PLRTRADE_HandleCubeInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC927D0
int32_t __fastcall PLRTRADE_AllocPlayerTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC92890
void __fastcall PLRTRADE_AddGold(D2UnitStrc* pUnit, int32_t nStat, int32_t nValue);
//D2Game.0x6FC92920
void __fastcall sub_6FC92920(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, D2PlayerDataStrc* pPlayerData1, D2PlayerDataStrc* pPlayerData2);
//D2Game.0x6FC92A90
int32_t __fastcall sub_6FC92A90(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FC92CF0
void __fastcall PLRTRADE_StopAllPlayerInteractions(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC92EE0
void __fastcall sub_6FC92EE0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC92EF0
void __fastcall sub_6FC92EF0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC92F10
void __fastcall sub_6FC92F10(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC931D0
void __fastcall sub_6FC931D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC933F0
int32_t __fastcall PLRTRADE_CopyTradeSaveDataToBuffer(D2UnitStrc* pUnit, uint8_t* pBuffer, int32_t nBufferSize);
//D2Game.0x6FC93430
int32_t __fastcall sub_6FC93430(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC93740
int32_t __fastcall sub_6FC93740(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC937A0
int32_t __fastcall sub_6FC937A0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC937F0
void __fastcall D2GAME_PLRTRADE_Last_6FC937F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t a4);
