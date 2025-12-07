#pragma once

#include <Units/Units.h>
#include "MonsterRegion.h"

struct D2UnkMonCreateStrc2;


//D2Game.0x6FC62020
void __stdcall sub_6FC62020(D2SeedStrc* pSeed, D2MonRegDataStrc* pMonRegData, int32_t nCount);
//D2Game.0x6FC62420
int32_t __fastcall MONSTERCHOOSE_GetPresetMonsterId(D2GameStrc* pGame, D2MonsterRegionStrc* pMonsterRegion, D2ActiveRoomStrc* pRoom, D2MonStatsTxt** ppMonStatsTxtRecord, uint8_t nChance, int32_t bSpawnUMon);
//D2Game.0x6FC62640
D2UnkMonCreateStrc2* __fastcall sub_6FC62640(D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC62670
int32_t __fastcall MONSTERCHOOSE_GetBossSpawnType(D2MonsterRegionStrc* pMonsterRegion, D2ActiveRoomStrc* pRoom);
