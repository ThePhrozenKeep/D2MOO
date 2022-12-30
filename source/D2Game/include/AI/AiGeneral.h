#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include "GAME/Game.h"

#pragma pack(1)

struct D2AiCmdStrc
{
	D2AiCmdStrc* pNextCmd;					//0x00
	D2AiCmdStrc* pPrevCmd;					//0x04
	int32_t nCmdParam[5];					//0x08
};

typedef void(__fastcall* AIPARAMFN)(D2GameStrc*, D2UnitStrc*, D2AiTickParamStrc*);

struct D2AiControlStrc
{
	uint32_t dwSpecialState;				//0x00
	AIPARAMFN pAiParamFn;					//0x04
	uint16_t nAiFlags;						//0x08
	uint8_t unk0x0A[2];						//0x0A
	D2UnitGUID dwOwnerGUID;					//0x0C
	uint32_t dwOwnerType;					//0x10
	int32_t dwAiParam[3];					//0x14
	D2AiCmdStrc* pCurrentCmd;				//0x20
	D2AiCmdStrc* pLastCmd;					//0x24
	D2GameStrc* pGame;						//0x28
	D2UnitGUID dwOwnerGUIDEx;				//0x2C
	uint32_t dwOwnerTypeEx;					//0x30
	D2MinionListStrc* pMinionList;			//0x34
	D2MapAIStrc* pMapAi;					//0x38
	//int32_t nTrapType;					//0x38
	int32_t nMinionSpawnClassId;			//0x3C
};

struct D2AiParamStrc
{
	//uint32_t unk0x00;						//0x00
	//void* pAiParamFn;						//0x04
	//uint16_t nAiFlags;					//0x08
	//uint8_t unk0x0A[16];					//0x0A
	//uint32_t dwMoveMethod;				//0x18
	//int32_t nSpeedBonus;					//0x1C
	//int32_t nSteps;						//0x20

	uint8_t unk0x00;
	uint8_t unk0x01;
	uint8_t unk0x02;
	uint8_t unk0x03;
	D2UnitStrc* pTarget;
	int32_t unk0x08;
	int32_t unk0x0C;
	int32_t unk0x10;
	int32_t unk0x14;
	int32_t unk0x18;
	int32_t nVelocity;
	uint8_t unk0x20;
	uint8_t unk0x21;
	uint8_t unk0x22;
	uint8_t unk0x23;
};

struct D2AiTableStrc
{
	int32_t unk0x00;						//0x00
	AIPARAMFN unk0x04;						//0x04
	AIPARAMFN pAiParamFn;					//0x08
	AIPARAMFN unk0x0C;						//0x0C
};

struct D2AiTickParamStrc
{
	D2AiControlStrc* pAiControl;			//0x00
	uint32_t unk0x04;						//0x04
	D2UnitStrc* pTarget;					//0x08
	uint32_t unk0x0C[2];					//0x0C
	int32_t nTargetDistance;				//0x14
	BOOL bCombat;							//0x18
	D2MonStatsTxt* pMonstatsTxt;			//0x1C
	D2MonStats2Txt* pMonstats2Txt;			//0x20
};

#pragma pack()

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
