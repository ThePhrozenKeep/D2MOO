#pragma once

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
//1.14d: 0x00564CD0
int32_t __fastcall OBJECTS_OperateFunction32_Bank(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC8F450
//1.14d: 0x00565010
int32_t __fastcall PLRTRADE_CheckCubeInput(D2GameStrc* pGame, D2InventoryStrc* pInventory, D2CubeMainTxt* pCubeMainTxt, int32_t nInputIndex, D2CubeItemStrc* pCubeItem, int32_t* pUsedItems);
//D2Game.0x6FC8FE40
//1.14d: 0x00565930
int16_t __fastcall PLRTRADE_RollRandomItemClassOfSameType(D2GameStrc* pGame, int32_t nItemLevel, int32_t nItemId);
//D2Game.0x6FC90010
//1.14d: 0x00565AB0
void __fastcall PLRTRADE_CreateCubeOutputs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CubeMainTxt* pCubeMainTxt, D2CubeItemStrc* pCubeItem);
//D2Game.0x6FC90A60
//1.14d: 0x00566BF0
void __fastcall PLRTRADE_Free(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC90AE0
//1.14d: 0x005678F0
void __fastcall PLRTRADE_RestoreInventory(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnused);
//D2Game.0x6FC90BE0
//1.14d: 0x00566C80
void __fastcall PLRTRADE_IterateCallback_SendInventoryReset(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FC90C20
//1.14d: 0x00566CC0
void __fastcall PLRTRADE_RestoreTradeSnapshot(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC90D70
//1.14d: 0x00566D90
void __fastcall PLRTRADE_SendEventPacketToPlayer(D2UnitStrc* pPlayer, D2C_SRV2CLT5A_TYPES nType, char* szSource);
//D2Game.0x6FC90DE0
//1.14d: 0x00566E60
void __fastcall PLRTRADE_TryToTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FC91050
//1.14d: 0x005679E0
void __fastcall PLRTRADE_CancelTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, D2PlayerDataStrc* pPlayerData1, D2PlayerDataStrc* pPlayerData2, int32_t nTradeState);
//D2Game.0x6FC91250
//1.14d: 0x00568060
int32_t __fastcall PLRTRADE_HandleTradeButton(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint16_t nButton, int32_t nGoldAmount);
//D2Game.0x6FC92130
//1.14d: 0x005665F0
void __fastcall PLRTRADE_HandleCubeInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC927D0
//1.14d: 0x00566B30
int32_t __fastcall PLRTRADE_AllocPlayerTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC92890
//1.14d: 0x0053FF00
void __fastcall PLRTRADE_AddGold(D2UnitStrc* pUnit, int32_t nStat, int32_t nValue);
//D2Game.0x6FC92920
//1.14d: 0x00567B00
void __fastcall PLRTRADE_ResetTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, D2PlayerDataStrc* pPlayerData1, D2PlayerDataStrc* pPlayerData2);
//D2Game.0x6FC92A90
//1.14d: 0x005670E0
int32_t __fastcall PLRTRADE_TransferItems(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FC92CF0
//1.14d: 0x00568640
void __fastcall PLRTRADE_StopAllPlayerInteractions(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC92EE0
//1.14d: 0x00568760
void __fastcall PLRTRADE_StopInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC92EF0
//1.14d: 0x00567330
void __fastcall PLRTRADE_CloseCubeInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC92F10
//1.14d: 0x00568770
void __fastcall PLRTRADE_AddItemToTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC931D0
//1.14d: 0x00567400
void __fastcall PLRTRADE_RemoveItemFromTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC933F0
//1.14d: 0x005675E0
int32_t __fastcall PLRTRADE_CopyTradeSaveDataToBuffer(D2UnitStrc* pUnit, uint8_t* pBuffer, int32_t nBufferSize);
//D2Game.0x6FC93430
//1.14d: 0x00567620
int32_t __fastcall PLRTRADE_PrepareItemChange(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC93740
//1.14d: 0x00567840
int32_t __fastcall PLRTRADE_IsTradeInventoryEmpty(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC937A0
//1.14d: 0x005678A0
int32_t __fastcall D2GAME_PLRTRADE_IsInteractingWithPlayer(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC937F0
//1.14d: 0x005689D0
void __fastcall PLRTRADE_UpdateTradeState(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nUnused1, int32_t nUnused2);
