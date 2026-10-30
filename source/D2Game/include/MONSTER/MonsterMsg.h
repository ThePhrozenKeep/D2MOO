#pragma once

#include <Units/Units.h>


//D2Game.0x6FC659E0
//1.14d: 0x00598220
void __fastcall MONSTERMSG_SendMonsterUpdates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t bUnitInitialized);
//D2Game.0x6FC65C70
//1.14d: 0x00597E20
void __fastcall MONSTERMSG_SendMonsterModeUpdate(D2GameStrc* pGame, D2UnitStrc* pMonster, D2ClientStrc* pClient);
