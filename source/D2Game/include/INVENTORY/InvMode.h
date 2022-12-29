#pragma once

#include "D2Structs.h"
#include <Units/Units.h>


//D2Game.0x6FC40FB0
void __fastcall D2GAME_INVMODE_First_6FC40FB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, D2InventoryStrc* pInventory);
//D2Game.0x6FC410E0
int32_t __fastcall sub_6FC410E0(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc* pUnused, D2ClientStrc* pClient);
//D2Game.0x6FC415F0
void __fastcall D2GAME_ResetSomeItemFlags_6FC415F0(D2GameStrc* pGame, D2UnitStrc* pItem);
//D2Game.0x6FC41660
int32_t __fastcall sub_6FC41660(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC416D0
void __fastcall D2GAME_INVMODE_Last_6FC416D0(D2GameStrc* pGame, D2UnitStrc* pUnit);
