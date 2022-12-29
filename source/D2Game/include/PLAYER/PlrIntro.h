#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include <GAME/Game.h>

//D2Game.0x6FC7F000
void __fastcall PLRINTRO_SetQuestIntroFlag(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nNpcId);
//D2Game.0x6FC7F060
int32_t __fastcall PLRINTRO_GetQuestIntroFlag(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nNpcId);
//D2Game.0x6FC7F0C0
void __fastcall PLRINTRO_SetNpcIntroFlag(D2PlrIntroStrc* pPlayerIntro, int32_t nNpcId);
//D2Game.0x6FC7F100
int32_t __fastcall PLRINTRO_GetNpcIntroFlag(D2PlrIntroStrc* pPlayerIntro, int32_t nNpcId);
//D2Game.0x6FC7F140
void __fastcall PLRINTRO_MaskNpcIntroFlag(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcId);
//D2Game.0x6FC7F1A0
void __fastcall PLRINTRO_CopyQuestIntroFlagsToBuffer(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1B0
void __fastcall PLRINTRO_CopyNpcIntroFlagsToBuffer(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1C0
void __fastcall PLRINTRO_CopyBufferToQuestIntroFlags(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1D0
void __fastcall PLRINTRO_CopyBufferToNpcIntroFlags(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1E0
D2PlrIntroStrc* __fastcall PLRINTRO_Create(D2GameStrc* pGame);
//D2Game.0x6FC7F2A0
void __fastcall PLRINTRO_Destroy(D2GameStrc* pGame, D2PlrIntroStrc* pPlayerIntro);
