#pragma once

#include <Units/Units.h>

#include <GAME/Game.h>

//D2Game.0x6FC7F000
//1.14d: 0x00572360
void __fastcall PLRINTRO_SetQuestIntroFlag(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nNpcId);
//D2Game.0x6FC7F060
//1.14d: 0x005723C0
int32_t __fastcall PLRINTRO_GetQuestIntroFlag(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nNpcId);
//D2Game.0x6FC7F0C0
//1.14d: 0x00572420
void __fastcall PLRINTRO_SetNpcIntroFlag(D2PlrIntroStrc* pPlayerIntro, int32_t nNpcId);
//D2Game.0x6FC7F100
//1.14d: 0x00572470
int32_t __fastcall PLRINTRO_GetNpcIntroFlag(D2PlrIntroStrc* pPlayerIntro, int32_t nNpcId);
//D2Game.0x6FC7F140
//1.14d: 0x005724C0
void __fastcall PLRINTRO_MaskNpcIntroFlag(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcId);
//D2Game.0x6FC7F1A0
//1.14d: 0x00572520
void __fastcall PLRINTRO_CopyQuestIntroFlagsToBuffer(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1B0
//1.14d: 0x00572530
void __fastcall PLRINTRO_CopyNpcIntroFlagsToBuffer(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1C0
//1.14d: 0x00572540
void __fastcall PLRINTRO_CopyBufferToQuestIntroFlags(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1D0
//1.14d: 0x00572550
void __fastcall PLRINTRO_CopyBufferToNpcIntroFlags(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer);
//D2Game.0x6FC7F1E0
//1.14d: 0x00572560
D2PlrIntroStrc* __fastcall PLRINTRO_Create(D2GameStrc* pGame);
//D2Game.0x6FC7F2A0
//1.14d: 0x00572620
void __fastcall PLRINTRO_Destroy(D2GameStrc* pGame, D2PlrIntroStrc* pPlayerIntro);
