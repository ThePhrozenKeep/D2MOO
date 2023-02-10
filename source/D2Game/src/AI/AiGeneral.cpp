#include "AI/AiGeneral.h"

#include <Drlg/D2DrlgPreset.h>
#include <D2Monsters.h>

#include "AI/AiUtil.h"
#include "MONSTER/Monster.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"


//D2Game.0x6FCCED00
D2AiControlStrc* __fastcall AIGENERAL_AllocAiControl(D2GameStrc* pGame)
{
	D2AiControlStrc* pAiControl = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2AiControlStrc);
	pAiControl->dwOwnerTypeEx = UNIT_MONSTER;
	pAiControl->dwOwnerType = UNIT_MONSTER;
	pAiControl->dwOwnerGUIDEx = -1;
	pAiControl->dwOwnerGUID = -1;

	return pAiControl;
}

//D2Game.0x6FCCED40
void __fastcall AIGENERAL_SetAiControlParam(D2UnitStrc* pMonster, int32_t nIndex, int32_t nParamValue)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pMonster);
	if (!pAiControl)
	{
		return;
	}

	switch (nIndex)
	{
	case 1:
		pAiControl->dwAiParam[0] = nParamValue;
		break;

	case 2:
		pAiControl->dwAiParam[1] = nParamValue;
		break;

	case 3:
		pAiControl->dwAiParam[2] = nParamValue;
		break;
	}
}

//D2Game.0x6FCCED80
int32_t __fastcall AIGENERAL_GetAiControlParam(D2UnitStrc* pUnit, int32_t nIndex)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return 0;
	}

	switch (nIndex)
	{
	case 1:
		return pAiControl->dwAiParam[0];

	case 2:
		return pAiControl->dwAiParam[1];

	case 3:
		return pAiControl->dwAiParam[2];
	}

	return 0;
}

//D2Game.0x6FCCEDC0
D2AiCmdStrc* __fastcall AIGENERAL_AllocAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return nullptr;
	}

	D2AiCmdStrc* pAiCmd = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2AiCmdStrc);
	D2AiCmdStrc* pLastCmd = pAiControl->pLastCmd;
	if (pLastCmd)
	{
		D2AiCmdStrc* pCurrentCmd = pAiControl->pCurrentCmd;
		if (pLastCmd == pCurrentCmd)
		{
			pAiControl->pLastCmd = pAiCmd;
		}

		pAiCmd->pNextCmd = pCurrentCmd;
		pAiCmd->pPrevCmd = pAiControl->pCurrentCmd->pPrevCmd;
		pAiControl->pCurrentCmd->pPrevCmd = pAiCmd;
		pAiCmd->pPrevCmd->pNextCmd = pAiCmd;
	}
	else
	{
		pAiControl->pLastCmd = pAiCmd;
		pAiControl->pCurrentCmd = pAiCmd;
		pAiCmd->pNextCmd = pAiCmd;
		pAiCmd->pPrevCmd = pAiCmd;
	}

	return pAiCmd;
}

//D2Game.0x6FCCEE40
void __fastcall AIGENERAL_FreeCurrentAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return;
	}

	D2AiCmdStrc* pCurrentCmd = pAiControl->pCurrentCmd;
	if (!pCurrentCmd)
	{
		return;
	}

	if (pCurrentCmd == pCurrentCmd->pNextCmd)
	{
		pAiControl->pLastCmd = nullptr;
		pAiControl->pCurrentCmd = nullptr;
	}
	else
	{
		if (pAiControl->pLastCmd == pCurrentCmd)
		{
			pAiControl->pLastCmd = pCurrentCmd->pNextCmd;
		}

		pAiControl->pCurrentCmd = pCurrentCmd->pNextCmd;
		pCurrentCmd->pPrevCmd->pNextCmd = pCurrentCmd->pNextCmd;
		pCurrentCmd->pNextCmd->pPrevCmd = pCurrentCmd->pPrevCmd;
	}

	D2_FREE_POOL(pGame->pMemoryPool, pCurrentCmd);
}

//D2Game.0x6FCCEEB0
void __fastcall AIGENERAL_FreeAllAiCommands(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return;
	}

	while (pAiControl->pLastCmd)
	{
		AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
	}
}

//D2Game.0x6FCCEEF0
D2AiCmdStrc* __fastcall AIGENERAL_GetCurrentAiCommandFromUnit(D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return nullptr;
	}

	return pAiControl->pCurrentCmd;
}

//D2Game.0x6FCCEF10
D2AiCmdStrc* __fastcall AIGENERAL_GetAiCommandFromParam(D2UnitStrc* pUnit, int32_t nCmdParam, int32_t bSet)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return nullptr;
	}

	D2AiCmdStrc* pCurrentAiCmd = pAiControl->pCurrentCmd;
	if (!pCurrentAiCmd)
	{
		return nullptr;
	}

	D2AiCmdStrc* pAiCmd = pCurrentAiCmd->pNextCmd;
	while (pAiCmd->nCmdParam[0] != nCmdParam)
	{
		if (pAiCmd == pCurrentAiCmd)
		{
			break;
		}

		pAiCmd = pAiCmd->pNextCmd;
	}

	if (pAiCmd->nCmdParam[0] != nCmdParam)
	{
		return nullptr;
	}

	if (bSet)
	{
		pAiControl->pCurrentCmd = pAiCmd;
	}

	return pAiCmd;
}

//D2Game.0x6FCCEF70
void __fastcall AIGENERAL_AllocCommandsForMinions(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiCmdStrc* pAiCmd)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl || !pAiControl->pGame)
	{
		return;
	}

	D2AiControlStrc* pOwnerAiControl = AIGENERAL_GetAiControlFromUnit(SUNIT_GetServerUnit(pAiControl->pGame, pAiControl->dwOwnerTypeEx, pAiControl->dwOwnerGUIDEx));
	if (!pOwnerAiControl)
	{
		return;
	}

	for (D2MinionListStrc* pMinionList = pOwnerAiControl->pMinionList; pMinionList; pMinionList = pMinionList->pNext)
	{
		D2UnitStrc* pMinion = SUNIT_GetServerUnit(pOwnerAiControl->pGame, UNIT_MONSTER, pMinionList->dwMinionGUID);
		if (pMinion)
		{
			AIGENERAL_CopyAiCommand(pGame, pMinion, pAiCmd);
		}
	}
}

//D2Game.0x6FCCF050
D2AiCmdStrc* __fastcall AIGENERAL_CopyAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiCmdStrc* pAiCmd)
{
	D2AiCmdStrc* pCopy = AIGENERAL_AllocAiCommand(pGame, pUnit);
	memcpy(pCopy->nCmdParam, pAiCmd->nCmdParam, sizeof(pCopy->nCmdParam));

	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (pAiControl)
	{
		pAiControl->pCurrentCmd = pCopy;
	}

	return pCopy;
}

//D2Game.0x6FCCF090
D2AiCmdStrc* __fastcall AIGENERAL_SetCurrentAiCommand(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCmdParam, int32_t bSet)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return 0;
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, nCmdParam, bSet);
	if (!pAiCmd)
	{
		const int32_t nCmdParams[5] = { nCmdParam, 0, 0, 0, 0 };
		pAiControl->pCurrentCmd = AIGENERAL_AllocAiCommand(pGame, pUnit);
		memcpy(pAiControl->pCurrentCmd->nCmdParam, nCmdParams, sizeof(pAiControl->pCurrentCmd->nCmdParam));
	}

	return AIGENERAL_GetAiCommandFromParam(pUnit, nCmdParam, bSet);
}

//D2Game.0x6FCCF190
void __fastcall AIGENERAL_FreeAiControl(D2GameStrc* pGame, D2AiControlStrc* pAiControl)
{
	if (!pAiControl)
	{
		return;
	}

	if (pAiControl->pMapAi)
	{
		DRLGPRESET_FreeMapAI(pGame->pMemoryPool, pAiControl->pMapAi);
	}

	D2MinionListStrc* pNext = nullptr;
	for (D2MinionListStrc* i = pAiControl->pMinionList; i; i = pNext)
	{
		pNext = i->pNext;
		D2_FREE_POOL(pGame->pMemoryPool, i);
	}

	for (D2AiCmdStrc* i = pAiControl->pLastCmd; i; i = pAiControl->pLastCmd)
	{
		D2AiCmdStrc* pCurrentCmd = pAiControl->pCurrentCmd;
		if (!pCurrentCmd)
		{
			break;
		}

		if (pCurrentCmd == pCurrentCmd->pNextCmd)
		{
			pAiControl->pLastCmd = nullptr;
			pAiControl->pCurrentCmd = nullptr;
		}
		else
		{
			if (i == pCurrentCmd)
			{
				pAiControl->pLastCmd = pCurrentCmd->pNextCmd;
			}

			pAiControl->pCurrentCmd = pCurrentCmd->pNextCmd;

			pCurrentCmd->pPrevCmd->pNextCmd = pCurrentCmd->pNextCmd;
			pCurrentCmd->pNextCmd->pPrevCmd = pCurrentCmd->pPrevCmd;
		}

		D2_FREE_POOL(pGame->pMemoryPool, pCurrentCmd);
	}

	D2_FREE_POOL(pGame->pMemoryPool, pAiControl);
}

//D2Game.0x6FCCF240
D2MapAIStrc** __stdcall AIGENERAL_GetMapAiFromUnit(D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return nullptr;
	}

	return &pAiControl->pMapAi;
}

//D2Game.0x6FCCF270
void __fastcall AIGENERAL_SetOwnerData(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nOwnerGUID, int32_t nOwnerType, int32_t bSetFlag1, int32_t bSetFlag2)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);

	if (bSetFlag2)
	{
		AIUTIL_ToggleAiControlFlag(pAiControl, 2u, 1);
	}

	if (bSetFlag1)
	{
		AIUTIL_ToggleAiControlFlag(pAiControl, 1u, 1);
	}

	pAiControl->dwOwnerGUIDEx = nOwnerGUID;
	pAiControl->dwOwnerTypeEx = nOwnerType;
	pAiControl->pGame = pGame;
}

//D2Game.0x6FCCF2D0
void __fastcall AIGENERAL_GetOwnerData(D2UnitStrc* pUnit, int32_t* pUnitGUID, int32_t* pUnitType)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (pAiControl && pAiControl->pGame)
	{
		*pUnitGUID = pAiControl->dwOwnerGUIDEx;
		*pUnitType = pAiControl->dwOwnerTypeEx;
	}
	else
	{
		*pUnitGUID = -1;
	}
}

//D2Game.0x6FCCF320
D2UnitStrc* __fastcall AIGENERAL_GetMinionOwner(D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (pAiControl && pAiControl->pGame)
	{
		return SUNIT_GetServerUnit(pAiControl->pGame, pAiControl->dwOwnerTypeEx, pAiControl->dwOwnerGUIDEx);
	}

	return nullptr;
}

//D2Game.0x6FCCF360
void __fastcall AIGENERAL_AllocMinionList(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMinion)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	D2MinionListStrc* pMinionList = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2MinionListStrc);

	if (pMinion)
	{
		pMinionList->dwMinionGUID = pMinion->dwUnitId;
	}
	else
	{
		pMinionList->dwMinionGUID = -1;
	}

	pMinionList->pNext = pAiControl->pMinionList;
	pAiControl->pMinionList = pMinionList;
}

//D2Game.0x6FCCF3C0
void __fastcall AIGENERAL_FreeMinionList(D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl || !pAiControl->pGame)
	{
		return;
	}

	D2AiControlStrc* pOwnerAiControl = AIGENERAL_GetAiControlFromUnit(SUNIT_GetServerUnit(pAiControl->pGame, pAiControl->dwOwnerTypeEx, pAiControl->dwOwnerGUIDEx));

	D2MinionListStrc* pPreviousMinionList = nullptr;
	for (D2MinionListStrc* pMinionList = pOwnerAiControl->pMinionList; pMinionList; pMinionList = pMinionList->pNext)
	{
		if (pMinionList->dwMinionGUID == pUnit->dwUnitId)
		{
			D2UnitStrc* pMinion = SUNIT_GetServerUnit(pOwnerAiControl->pGame, UNIT_MONSTER, pMinionList->dwMinionGUID);
			if (pMinion && pMinion->dwUnitType == UNIT_MONSTER && pMinion->pMonsterData)
			{
				pMinion->pMonsterData->pAiControl->dwOwnerGUIDEx = -1;
				pMinion->pMonsterData->pAiControl->dwOwnerTypeEx = 1;
				pMinion->pMonsterData->pAiControl->pGame = pOwnerAiControl->pGame;
			}

			if (pPreviousMinionList)
			{
				pPreviousMinionList->pNext = pMinionList->pNext;
			}
			else
			{
				pOwnerAiControl->pMinionList = pMinionList->pNext;
			}

			D2_FREE_POOL(pOwnerAiControl->pGame->pMemoryPool, pMinionList);
			return;
		}

		pPreviousMinionList = pMinionList;
	}
}

//D2Game.0x6FCCF4B0
void __fastcall sub_6FCCF4B0(D2UnitStrc* pUnit)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
	{
		return;
	}

	D2AiControlStrc* pAiControl = pUnit->pMonsterData->pAiControl;
	if (!pAiControl || !pAiControl->pGame)
	{
		return;
	}

	D2UnitStrc* pOwner = SUNIT_GetServerUnit(pAiControl->pGame, pAiControl->dwOwnerTypeEx, pAiControl->dwOwnerGUIDEx);
	if (!pOwner || pOwner != pUnit)
	{
		return;
	}

	D2MinionListStrc* pMinionList = pAiControl->pMinionList;
	while (pMinionList)
	{
		D2MinionListStrc* pNext = pMinionList->pNext;
		D2UnitStrc* pMinion = SUNIT_GetServerUnit(pAiControl->pGame, UNIT_MONSTER, pMinionList->dwMinionGUID);
		if (pMinion && pMinion->dwUnitType == UNIT_MONSTER && pMinion->pMonsterData)
		{
			pMinion->pMonsterData->pAiControl->dwOwnerGUIDEx = -1;
			pMinion->pMonsterData->pAiControl->dwOwnerTypeEx = UNIT_MONSTER;
			pMinion->pMonsterData->pAiControl->pGame = pAiControl->pGame;
		}

		D2_FREE_POOL(pAiControl->pGame->pMemoryPool, pMinionList);
		pMinionList = pNext;
	}

	pAiControl->pMinionList = nullptr;
}

//D2Game.0x6FCCF590
void __fastcall AIGENERAL_FreeAllMinionLists(D2GameStrc* pGame, D2MinionListStrc* pMinionList)
{
	while (pMinionList)
	{
		D2MinionListStrc* pNext = pMinionList->pNext;
		D2_FREE_POOL(pGame->pMemoryPool, pMinionList);
		pMinionList = pNext;
	}
}

//D2Game.0x6FCCF5C0
void __fastcall AIGENERAL_ExecuteCallbackOnMinions(D2UnitStrc* pUnit, void* a2, void* a3, void(__fastcall* pfnParty)(D2UnitStrc*, void*, void*))
{
	D2_ASSERT(pfnParty);

	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl || !pAiControl->pGame)
	{
		return;
	}

	D2AiControlStrc* pOwnerAiControl = AIGENERAL_GetAiControlFromUnit(SUNIT_GetServerUnit(pAiControl->pGame, pAiControl->dwOwnerTypeEx, pAiControl->dwOwnerGUIDEx));
	for (D2MinionListStrc* pMinionList = pOwnerAiControl->pMinionList; pMinionList; pMinionList = pMinionList->pNext)
	{
		D2UnitStrc* pMinion = SUNIT_GetServerUnit(pOwnerAiControl->pGame, UNIT_MONSTER, pMinionList->dwMinionGUID);
		if (pMinion)
		{
			pfnParty(pMinion, a2, a3);
		}
	}
}

//D2Game.0x6FCCF680
void __fastcall AIGENERAL_GetAiControlInfo(D2UnitStrc* pUnit, int32_t* pOwnerGUID, int32_t* pOwnerType, int32_t* pAiControlFlag1, int32_t* pAiControlFlag2, D2MinionListStrc** ppMinionList)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (pAiControl && pAiControl->dwOwnerTypeEx == UNIT_MONSTER)
	{
		*pOwnerGUID = pAiControl->dwOwnerGUIDEx;
		*pOwnerType = pAiControl->dwOwnerTypeEx;
		*pAiControlFlag1 = AIUTIL_CheckAiControlFlag(pAiControl, 1u);
		*pAiControlFlag2 = AIUTIL_CheckAiControlFlag(pAiControl, 2u);
		*ppMinionList = pAiControl->pMinionList;
		pAiControl->dwOwnerGUIDEx = -1;
		pAiControl->pMinionList = nullptr;
	}
	else
	{
		*pOwnerGUID = -1;
	}
}

//D2Game.0x6FCCF710
void __fastcall AIGENERAL_SetAiControlInfo(D2GameStrc* pGame, D2UnitStrc* pUnit, DWORD nOwnerGUID, DWORD nOwnerType, int32_t bSetFlag1, int32_t bSetFlag2, D2MinionListStrc* pMinionList)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return;
	}

	AIGENERAL_FreeAllMinionLists(pGame, pAiControl->pMinionList);

	if (bSetFlag2)
	{
		AIUTIL_ToggleAiControlFlag(pAiControl, 2u, 1);
	}

	if (bSetFlag1)
	{
		AIUTIL_ToggleAiControlFlag(pAiControl, 1u, 1);
	}

	pAiControl->dwOwnerGUIDEx = nOwnerGUID;
	pAiControl->dwOwnerTypeEx = nOwnerType;
	pAiControl->pGame = pGame;
	pAiControl->pMinionList = pMinionList;
}

//D2Game.0x6FCCF7C0
void __fastcall AIGENERAL_UpdateMinionList(D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!AIUTIL_CheckAiControlFlag(pAiControl, 1u))
	{
		return;
	}

	if (!AIUTIL_CheckAiControlFlag(pAiControl, 2u))
	{
		for (D2MinionListStrc* i = pAiControl->pMinionList; i; i = i->pNext)
		{
			AIGENERAL_SetOwnerData(pAiControl->pGame, SUNIT_GetServerUnit(pAiControl->pGame, UNIT_MONSTER, i->dwMinionGUID), -1, UNIT_MONSTER, 0, 0);
		}
		return;
	}

	D2MinionListStrc* pMinionList = pAiControl->pMinionList;
	D2MinionListStrc* pList = pAiControl->pMinionList;

	while (pMinionList)
	{
		D2UnitStrc* pMinion = SUNIT_GetServerUnit(pAiControl->pGame, UNIT_MONSTER, pMinionList->dwMinionGUID);
		if (pMinion)
		{
			if (pMinion->dwUnitType == UNIT_MONSTER)
			{
				AIGENERAL_SetOwnerData(pAiControl->pGame, pMinion, pMinionList->dwMinionGUID, UNIT_MONSTER, 1, 1);
			}

			if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
			{
				AIGENERAL_SetOwnerData(pAiControl->pGame, pUnit, pMinionList->dwMinionGUID, UNIT_MONSTER, 0, 0);
			}

			if (pMinion->dwUnitType == UNIT_MONSTER)
			{
				D2AiControlStrc* pMinionAiControl = AIGENERAL_GetAiControlFromUnit(pMinion);

				D2MinionListStrc* pNewList = D2_CALLOC_STRC_POOL(pAiControl->pGame->pMemoryPool, D2MinionListStrc);
				pNewList->dwMinionGUID = pUnit ? pUnit->dwUnitId : -1;

				if (pMinionAiControl->pMinionList)
				{
					pNewList->pNext = pMinionAiControl->pMinionList;
				}
				pMinionAiControl->pMinionList = pNewList;
			}

			do
			{
				D2UnitStrc* pTemp = SUNIT_GetServerUnit(pAiControl->pGame, UNIT_MONSTER, pList->dwMinionGUID);
				if (pTemp && pTemp != pMinion)
				{
					AIGENERAL_SetOwnerData(pAiControl->pGame, pTemp, pMinionList->dwMinionGUID, UNIT_MONSTER, 0, 0);
					AIGENERAL_AllocMinionList(pAiControl->pGame, pMinion, pTemp);
				}
				pList = pList->pNext;
			}
			while (pList);
			return;
		}

		pMinionList = pMinionList->pNext;
	}
}

//D2Game.0x6FCCF9B0
int32_t __fastcall AIGENERAL_GetMinionSpawnClassId(D2UnitStrc* pUnit)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return 0;
	}

	return pAiControl->nMinionSpawnClassId;
}
