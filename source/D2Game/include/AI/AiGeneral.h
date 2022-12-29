#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include "GAME/Game.h"


//D2Game.0x6FCCED00
D2AiControlStrc* __fastcall AIGENERAL_AllocAiControl(D2GameStrc* pGame);
//D2Game.0x6FCCED40
void __fastcall AIGENERAL_SetAiControlParam(D2UnitStrc* pMonster, int32_t nIndex, int32_t nParamValue);
//D2Game.0x6FCCED80
int32_t __fastcall AIGENERAL_GetAiControlParam(D2UnitStrc* pUnit, int32_t nIndex);
//D2Game.0x6FCCEDC0
D2AiCmdStrc* __fastcall AIGENERAL_AllocAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCEE40
void __fastcall AIGENERAL_FreeCurrentAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCEEB0
void __fastcall AIGENERAL_FreeAllAiCommands(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCEEF0
D2AiCmdStrc* __fastcall AIGENERAL_GetCurrentAiCommandFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCCEF10
D2AiCmdStrc* __fastcall AIGENERAL_GetAiCommandFromParam(D2UnitStrc* pUnit, int32_t nCmdParam, int32_t bSet);
//D2Game.0x6FCCEF70
void __fastcall AIGENERAL_AllocCommandsForMinions(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiCmdStrc* pAiCmd);
//D2Game.0x6FCCF050
D2AiCmdStrc* __fastcall AIGENERAL_CopyAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiCmdStrc* pAiCmd);
//D2Game.0x6FCCF090
D2AiCmdStrc* __fastcall AIGENERAL_SetCurrentAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCmdParam, int32_t bSet);
//D2Game.0x6FCCF190
void __fastcall AIGENERAL_FreeAiControl(D2GameStrc* pGame, D2AiControlStrc* pAiControl);
//D2Game.0x6FCCF240
D2MapAIStrc** __stdcall AIGENERAL_GetMapAiFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCCF270
void __fastcall AIGENERAL_SetOwnerData(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nOwnerGUID, int32_t nOwnerType, int32_t bSetFlag1, int32_t bSetFlag2);
//D2Game.0x6FCCF2D0
void __fastcall AIGENERAL_GetOwnerData(D2UnitStrc* pUnit, int32_t* pUnitGUID, int32_t* pUnitType);
//D2Game.0x6FCCF320
D2UnitStrc* __fastcall AIGENERAL_GetMinionOwner(D2UnitStrc* pUnit);
//D2Game.0x6FCCF360
void __fastcall AIGENERAL_AllocMinionList(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMinion);
//D2Game.0x6FCCF3C0
void __fastcall AIGENERAL_FreeMinionList(D2UnitStrc* pUnit);
//D2Game.0x6FCCF4B0
void __fastcall sub_6FCCF4B0(D2UnitStrc* pUnit);
//D2Game.0x6FCCF590
void __fastcall AIGENERAL_FreeAllMinionLists(D2GameStrc* pGame, D2MinionListStrc* pMinionList);
//D2Game.0x6FCCF5C0
void __fastcall AIGENERAL_ExecuteCallbackOnMinions(D2UnitStrc* pUnit, void* a2, void* a3, void(__fastcall* pfnParty)(D2UnitStrc*, void*, void*));
//D2Game.0x6FCCF680
void __fastcall AIGENERAL_GetAiControlInfo(D2UnitStrc* pUnit, int32_t* pOwnerGUID, int32_t* pOwnerType, int32_t* pAiControlFlag1, int32_t* pAiControlFlag2, D2MinionListStrc** ppMinionList);
//D2Game.0x6FCCF710
void __fastcall AIGENERAL_SetAiControlInfo(D2GameStrc* pGame, D2UnitStrc* pUnit, DWORD nOwnerGUID, DWORD nOwnerType, int32_t bSetFlag1, int32_t bSetFlag2, D2MinionListStrc* pMinionList);
//D2Game.0x6FCCF7C0
void __fastcall AIGENERAL_UpdateMinionList(D2UnitStrc* pUnit);
//D2Game.0x6FCCF9B0
int32_t __fastcall AIGENERAL_GetMinionSpawnClassId(D2UnitStrc* pUnit);
