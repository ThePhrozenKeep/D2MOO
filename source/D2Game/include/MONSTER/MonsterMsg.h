#pragma once

#include <Units/Units.h>


//D2Game.0x6FC659E0
void __fastcall MONSTERMSG_SendMonsterUpdates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t bUnitInitialized);
//D2Game.0x6FC65C70
void __fastcall MONSTERMSG_SendMonsterModeUpdate(D2GameStrc* pGame, D2UnitStrc* pMonster, D2ClientStrc* pClient);
