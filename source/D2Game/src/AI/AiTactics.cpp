#include "AI/AiTactics.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>
#include <D2PacketDef.h>

#include <D2Chat.h>
#include <D2Collision.h>
#include <D2DataTbls.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <D2Skills.h>
#include <Units/UnitRoom.h>

#include "AI/AiUtil.h"
#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "QUESTS/Quests.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"

//D2Game.0x6FCCF9D0
D2UnitStrc* __fastcall sub_6FCCF9D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t* pDistance, int32_t* pCombat)
{
	D2UnitStrc* pResult = sub_6FCF2110(pGame, pUnit, pAiControl, pDistance, pCombat);
	if (pResult)
	{
		return pResult;
	}

	if ((sub_6FCF2E70(pUnit) && pUnit) || COLLISION_CheckAnyCollisionWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), PATH_GetUnitCollisionPattern(pUnit), COLLIDE_MISSILE))
	{
		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_WALK])
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
			return nullptr;
		}
	}

	const int32_t nDistance = *pDistance;
	int32_t nFrames = 0;
	if (nDistance >= 35)
	{
		nFrames = 25;
	}
	else if (nDistance >= 25)
	{
		nFrames = nDistance - 10;
	}
	else
	{
		nFrames = 10;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, nFrames);
	return nullptr;
}

//D2Game.0x6FCCFC00
D2UnitStrc* __fastcall sub_6FCCFC00(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t* pDistance, int32_t* pCombat)
{
	D2UnitStrc* pResult = sub_6FCF2110(pGame, pUnit, pAiControl, pDistance, pCombat);
	if (pResult)
	{
		return pResult;
	}

	if (COLLISION_CheckAnyCollisionWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), PATH_GetUnitCollisionPattern(pUnit), COLLIDE_MISSILE))
	{
		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_WALK])
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
			return nullptr;
		}
	}

	AITACTICS_Idle(pGame, pUnit, 20);
	return nullptr;
}

//D2Game.0x6FCCFD40
D2UnitStrc* __fastcall sub_6FCCFD40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t* pDistance, int32_t* pCombat, int32_t nMaxDistance)
{
	D2UnitStrc* pResult = sub_6FCF2110(pGame, pUnit, pAiControl, pDistance, pCombat);

	if (pResult && *pDistance <= nMaxDistance)
	{
		return pResult;
	}

	return nullptr;
}

//D2Game.0x6FCCFD70
D2UnitStrc* __fastcall sub_6FCCFD70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* bCloseToTarget)
{
	if (!UNITS_GetRoom(pUnit))
	{
		return nullptr;
	}

	UnkAiStrc1 pCallbackArg = {};
	pCallbackArg.pTarget = nullptr;
	pCallbackArg.nDistance = INT_MAX;
	sub_6FCF1E80(pGame, pUnit, &pCallbackArg, sub_6FCCFDE0, 2);

	if (!pCallbackArg.pTarget)
	{
		return pUnit;
	}

	if (pCallbackArg.nDistance < 4)
	{
		*bCloseToTarget = 1;
	}

	return pCallbackArg.pTarget;
}

//D2Game.0x6FCCFDE0
D2UnitStrc* __fastcall sub_6FCCFDE0(D2GameStrc* pGame, D2UnitStrc* pNPC, D2UnitStrc* pPlayer, void* pCallbackArg)
{
	UnkAiStrc1* pArg = (UnkAiStrc1*)pCallbackArg;

	if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
	{
		return nullptr;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pNPC, pPlayer);
	if (nDistance > 15)
	{
		return nullptr;
	}

	if (!pNPC)
	{
		pArg->pTarget = pPlayer;
		pArg->nDistance = nDistance;
		return pPlayer;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pNPC->dwClassId);
	if (!pMonStatsTxtRecord || !(gdwBitMasks[MONSTATSFLAGINDEX_INTERACT] & pMonStatsTxtRecord->dwMonStatsFlags))
	{
		pArg->pTarget = pPlayer;
		pArg->nDistance = nDistance;
		return pPlayer;
	}

	if (QUESTS_ActiveCycler(pGame, pPlayer, pNPC) && nDistance < pArg->nDistance)
	{
		pArg->pTarget = pPlayer;
		pArg->nDistance = nDistance;
		return pPlayer;
	}

	return nullptr;
}

//D2Game.0x6FCCFEA0
int32_t __fastcall AITACTICS_ChangeModeAndTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode, D2UnitStrc* pTargetUnit)
{
	D2ModeChangeStrc modeChange = {};

	MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
	modeChange.pTargetUnit = pTargetUnit;
	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FCCFEE0
int32_t __fastcall AITACTICS_ChangeModeAndTargetCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode, int32_t nX, int32_t nY)
{
	D2ModeChangeStrc modeChange = {};

	MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
	modeChange.nX = nX;
	modeChange.nY = nY;
	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FCCFF20
int32_t __fastcall AITACTICS_UseSequenceSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, D2UnitStrc* pTargetUnit, int32_t nX, int32_t nY)
{
	if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
	{
		return 0;
	}

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_SEQUENCE, &modeChange);
	UNITS_SetUsedSkill(pUnit, SKILLS_GetSkillById(pUnit, nSkillId, -1));

	PATH_SetStepNum(pUnit->pDynamicPath, 1);

	modeChange.pTargetUnit = pTargetUnit;
	modeChange.nX = nX;
	modeChange.nY = nY;

	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 0);
}

//D2Game.0x6FCCFFB0
int32_t __fastcall AITACTICS_UseSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nMode, int32_t nSkillId, D2UnitStrc* pTarget, int32_t nX, int32_t nY)
{
	if (!pUnit || nMode >= 16u)
	{
		return 0;
	}

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
	UNITS_SetUsedSkill(pUnit, SKILLS_GetSkillById(pUnit, nSkillId, -1));

	pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;

	PATH_SetStepNum(pUnit->pDynamicPath, 1);

	modeChange.pTargetUnit = pTarget;
	modeChange.nX = nX;
	modeChange.nY = nY;

	if (D2GAME_ModeChange_6FC65220(pGame, &modeChange, 0))
	{
		return 1;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
	return 0;
}

//D2Game.0x6FCD00A0
void __fastcall AITACTICS_IdleInNeutralMode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nFrames)
{
	if (!pUnit || pUnit->dwAnimMode != MONMODE_NEUTRAL)
	{
		D2ModeChangeStrc modeChange = {};
		MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_NEUTRAL, &modeChange);
		modeChange.pTargetUnit = pUnit;
		D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
	}

	AITACTICS_Idle(pGame, pUnit, nFrames);
}

//D2Game.0x6FCD0110
void __fastcall AITACTICS_Idle(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nFrames)
{
	if (!nFrames)
	{
		nFrames = 1;
	}

	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);
	EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, nFrames + pGame->dwGameFrame, 0, 0);
}

//D2Game.0x6FCD0150
void __fastcall sub_6FCD0150(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nFrames)
{
	if (!nFrames)
	{
		nFrames = 1;
	}

	const uint32_t dwEventFrame = EVENT_GetEventFrame(pGame, pUnit, UNITEVENTCALLBACK_AITHINK);
	if (!dwEventFrame || dwEventFrame == pGame->dwGameFrame || dwEventFrame >= nFrames + pGame->dwGameFrame)
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);
		EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, nFrames + pGame->dwGameFrame, 0, 0);
	}
}

//D2Game.0x6FCD01B0
void __fastcall AITACTICS_SetVelocity(D2UnitStrc* pUnit, int32_t a2, int32_t nVel, uint8_t a4)
{
	D2_ASSERT((nVel >= -126) && (nVel <= 126));

	if (a2 == 1)
	{
		a2 = 7;
	}

	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pUnit->pMonsterData->pAiParam, a2, nVel, a4);
	}
	else
	{
		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(nullptr, a2, nVel, a4);
	}
}

//D2Game.0x6FCD0220
int32_t __fastcall AITACTICS_WalkToTargetUnitWithFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, uint8_t nFlag)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, pTargetUnit, MONMODE_WALK, 0, 0, 1u, nFlag);
}

//D2Game.0x6FCD0240
int32_t __fastcall AITACTICS_MoveToTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nMode, int32_t nX, int32_t nY, uint8_t bStep, uint8_t nFlags)
{
	int32_t nNewMode = nMode;

	if (nMode == MONMODE_RUN && STATES_CheckState(pUnit, STATE_DECREPIFY))
	{
		D2GAME_MONSTERMODE_ResetVelocityParams_6FC62F90(pUnit);
		nNewMode = MONMODE_WALK;
	}

	PATH_SetStepNum(pUnit->pDynamicPath, bStep);

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, nNewMode, &modeChange);

	if (pTarget)
	{
		modeChange.pTargetUnit = pTarget;
	}
	else
	{
		modeChange.nX = nX;
		modeChange.nY = nY;
	}

	if (D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1))
	{
		return 1;
	}

	if (nFlags & 4)
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);
	}

	if (nFlags & 1)
	{
		D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
		if (pAiControl)
		{
			AIUTIL_ToggleAiControlFlag(pAiControl, 0x40u, 1);
		}
	}

	if (nFlags & 2)
	{
		if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) >= 70)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
		}

		return 1;
	}

	return 0;
}

//D2Game.0x6FCD03B0
int32_t __fastcall AITACTICS_WalkToTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, pTarget, MONMODE_WALK, 0, 0, 1u, 0);
}

//D2Game.0x6FCD03D0
int32_t __fastcall AITACTICS_RunToTargetUnitWithFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, uint8_t nFlags)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, pTarget, MONMODE_RUN, 0, 0, 1u, nFlags);
}

//D2Game.0x6FCD03F0
int32_t __fastcall AITACTICS_RunToTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, pTarget, MONMODE_RUN, 0, 0, 1u, 0);
}

//D2Game.0x6FCD0410
int32_t __fastcall sub_6FCD0410(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, uint8_t nFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pUnit->pMonsterData->pAiParam, 13, 0, 0);
	}

	return AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, nFlags);
}

//D2Game.0x6FCD0460
int32_t __fastcall AITACTICS_WalkToTargetCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_WALK, nX, nY, 1u, 0);
}

//D2Game.0x6FCD0480
int32_t __fastcall AITACTICS_RunToTargetCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_RUN, nX, nY, 1u, 0);
}

//D2Game.0x6FCD04A0
int32_t __fastcall AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_WALK, nX, nY, 1u, 4);
}

//D2Game.0x6FCD04C0
int32_t __fastcall AITACTICS_RunToTargetCoordinatesDeleteAiEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_RUN, nX, nY, 1u, 4);
}

//D2Game.0x6FCD04E0
int32_t __fastcall AITACTICS_WalkToTargetCoordinatesNoSteps(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_WALK, nX, nY, 0, 0);
}

//D2Game.0x6FCD0500
int32_t __fastcall AITACTICS_WalkToTargetUnitWithSteps(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, uint8_t bStep)
{
	if (!bStep)
	{
		bStep = 1;
	}

	return AITACTICS_MoveToTarget(pGame, pUnit, pTarget, MONMODE_WALK, 0, 0, bStep, 0);
}

//D2Game.0x6FCD0530
int32_t __fastcall AITACTICS_RunToTargetUnitWithSteps(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, uint8_t bStep)
{
	if (!bStep)
	{
		bStep = 1;
	}

	return AITACTICS_MoveToTarget(pGame, pUnit, pTarget, MONMODE_RUN, 0, 0, bStep, 0);
}

//D2Game.0x6FCD0560
int32_t __fastcall D2GAME_AICORE_Escape_6FCD0560(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nMaxDistance, int32_t bDeleteAiEventCallback)
{
	if (!pUnit || !pTarget)
	{
		return 0;
	}

	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);
	const int32_t nXSign = D2signum(nX - CLIENTS_GetUnitX(pTarget));
	const int32_t nYSign = D2signum(nY - CLIENTS_GetUnitY(pTarget));

	if (nMaxDistance > 5u)
	{
		D2AiParamStrc* pAiParam = nullptr;
		if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pAiParam = pUnit->pMonsterData->pAiParam;
		}

		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 0, 0, nMaxDistance);
	}

	return AITACTICS_MoveToTarget(pGame, pUnit, 0, MONMODE_WALK, nX + nXSign * nMaxDistance, nY + nYSign * nMaxDistance, 1u, bDeleteAiEventCallback != 0 ? 4 : 0);
}

//D2Game.0x6FCD06D0
int32_t __fastcall sub_6FCD06D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nMaxDistance, int32_t bDeleteAiEventCallback)
{
	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);
	const int32_t nXSign = D2signum(nX - CLIENTS_GetUnitX(pTarget));
	const int32_t nYSign = D2signum(nY - CLIENTS_GetUnitY(pTarget));

	if (nMaxDistance > 5u)
	{
		D2AiParamStrc* pAiParam = nullptr;
		if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pAiParam = pUnit->pMonsterData->pAiParam;
		}

		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 0, 0, nMaxDistance);
	}

	return AITACTICS_MoveToTarget(pGame, pUnit, 0, MONMODE_RUN, nX + nXSign * nMaxDistance, nY + nYSign * nMaxDistance, 1u, bDeleteAiEventCallback != 0 ? 4 : 0);
}

//D2Game.0x6FCD0840
int32_t __fastcall AITACTICS_WalkCloseToUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMaxDistance)
{
	int32_t nOffsetX = 0;
	int32_t nOffsetY = 0;

	const int32_t nTargetX = CLIENTS_GetUnitX(pUnit);
	const int32_t nTargetY = CLIENTS_GetUnitY(pUnit);

	if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
	{
		nOffsetX = nMaxDistance;
		nOffsetY = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDistance);
	}
	else
	{
		nOffsetX = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDistance);
		nOffsetY = nMaxDistance;
	}

	if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
	{
		nOffsetX = -nOffsetX;
	}

	if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
	{
		nOffsetY = -nOffsetY;
	}

	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_WALK, nTargetX + nOffsetX, nTargetY + nOffsetY, 1u, 0);
}

//D2Game.0x6FCD09D0
int32_t __fastcall sub_6FCD09D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nMaxDistance)
{
	return AITACTICS_WalkCloseToUnit(pGame, pUnit, nMaxDistance);
}

//D2Game.0x6FCD0B60
int32_t __fastcall D2GAME_AICORE_WalkToOwner_6FCD0B60(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nMaxDistance)
{
	if (!pOwner)
	{
		return AITACTICS_WalkCloseToUnit(pGame, pUnit, 2);
	}

	return sub_6FCD09D0(pGame, pUnit, pOwner, nMaxDistance);
}

//D2Game.0x6FCD0D00
int32_t __fastcall AITACTICS_RunCloseToTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nMaxDistance)
{
	int32_t nOffsetX = 0;
	int32_t nOffsetY = 0;

	const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
	const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);

	if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
	{
		nOffsetX = nMaxDistance;
		nOffsetY = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDistance);
	}
	else
	{
		nOffsetX = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDistance);
		nOffsetY = nMaxDistance;
	}

	if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
	{
		nOffsetX = -nOffsetX;
	}

	if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
	{
		nOffsetY = -nOffsetY;
	}

	return AITACTICS_MoveToTarget(pGame, pUnit, nullptr, MONMODE_RUN, nTargetX + nOffsetX, nTargetY + nOffsetY, 1u, 0);
}

//D2Game.0x6FCD0E80
int32_t __fastcall sub_6FCD0E80(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t a4, int32_t bDeleteAiEventCallback)
{
	//TODO: v8
	int32_t v8 = 0;
	if ((uint8_t)ITEMS_RollRandomNumber(&pUnit->pSeed) < 128u)
	{
		v8 = 5;
	}
	else
	{
		v8 = 6;
	}

	D2AiParamStrc* pAiParam = nullptr;
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		pAiParam = pUnit->pMonsterData->pAiParam;
	}

	D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, v8, 0, a4);

	return AITACTICS_MoveToTarget(pGame, pUnit, pTarget, MONMODE_WALK, 0, 0, 1u, bDeleteAiEventCallback != 0 ? 4 : 0);
}

//D2Game.0x6FCD0F10
void __fastcall AITACTICS_AddMessage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pScrollTarget, uint16_t wMessage, int32_t bScrollMessage)
{
	if (!wMessage || wMessage > USHRT_MAX)
	{
		return;
	}

	if (bScrollMessage)
	{
		D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pScrollTarget, __FILE__, __LINE__);

		D2GSPacketSrv27 packet27 = {};
		packet27.nHeader = 0x27;

		if (pUnit)
		{
			packet27.dwUnitGUID = pUnit->dwUnitId;
		}
		else
		{
			packet27.dwUnitGUID = -1;
		}

		packet27.nUnitType = UNIT_MONSTER;
		packet27.pMessageList.nCount = 1;
		packet27.pMessageList.pMessages[0].nMenu = 3;
		packet27.pMessageList.pMessages[0].nStringId = wMessage;
		D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet27, sizeof(packet27));
	}
	else
	{
		char szText[12] = {};
		sprintf_s(szText, "%d", wMessage);

		if (pUnit && pUnit->pHoverText)
		{
			CHAT_FreeHoverMsg(pGame->pMemoryPool, pUnit->pHoverText);
		}

		D2HoverTextStrc* pHoverMsg = CHAT_AllocHoverMsg(pGame->pMemoryPool, szText, pGame->dwGameFrame);
		if (pHoverMsg)
		{
			if (pUnit)
			{
				pUnit->pHoverText = pHoverMsg;
			}

			UNITROOM_RefreshUnit(pUnit);
			if (pUnit)
			{
				pUnit->dwFlags |= UNITFLAG_HASTXTMSG;
			}

			EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_FREEHOVER, CHAT_GetTimeoutFromHoverMsg(pHoverMsg), 0, 0);
		}
	}
}

//D2Game.0x6FCD1020
int32_t __fastcall AITACTICS_ChangeModeAndTargetCoordinatesOneStep(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nMode)
{
	D2ModeChangeStrc modeChange = {};

	MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
	PATH_SetStepNum(pUnit->pDynamicPath, 1u);
	modeChange.nX = nX;
	modeChange.nY = nY;
	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FCD1070
int32_t __fastcall AITACTICS_ChangeModeAndTargetCoordinatesNoStep(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nMode)
{
	D2ModeChangeStrc modeChange = {};

	MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
	PATH_SetStepNum(pUnit->pDynamicPath, 0);
	modeChange.nX = nX;
	modeChange.nY = nY;
	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FCD10C0
// TODO: Check name
int32_t __fastcall AITACTICS_MoveInRadiusToTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nMode, int32_t a5, int32_t a6)
{
	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget);
	const int32_t nSign = D2signum(nDistance - a6);
	const int32_t nDiff = std::min(std::abs(nDistance - a6), a5);

	const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
	const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);
	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	const int32_t nXDiff = std::abs(nTargetX - nX);
	const int32_t nYDiff = std::abs(nTargetY - nY);

	const int32_t nSum = std::max(nYDiff + nXDiff, nDiff);

	int32_t nXOffset = 0;
	int32_t nYOffset = 0;
	if (nSum > 0)
	{
		nXOffset = nDiff * nXDiff / nSum;
		nYOffset = nDiff * nYDiff / nSum;
		while (nXOffset + nYOffset < nDiff)
		{
			++nXOffset;
			++nYOffset;
		}
	}

	const int32_t nSignX = D2signum(nTargetX - nX);
	const int32_t nSignY = D2signum(nTargetY - nY);

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
	PATH_SetStepNum(pUnit->pDynamicPath, 1u);

	modeChange.nX = nX + nSign * nXOffset * nSignX;
	modeChange.nY = nY + nSign * nYOffset * nSignY;

	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FCD12C0
// TODO: Check name
int32_t __fastcall AITACTICS_WalkInRadiusToTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t a4, int32_t a5)
{
	return AITACTICS_MoveInRadiusToTarget(pGame, pUnit, pTarget, MONMODE_WALK, a4, a5);
}

//D2Game.0x6FCD12E0
int32_t __fastcall AITACTICS_WalkAroundTargetWithScaledDistance(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2UnitStrc* pTarget, int32_t nScale)
{
	const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pTarget);
	if (!nDistance)
	{
		return 0;
	}

	const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
	const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_WALK, &modeChange);
	modeChange.nX = nTargetX + (nScale * (CLIENTS_GetUnitX(pUnit) - nTargetX)) / nDistance;
	modeChange.nY = nTargetY + (nScale * (CLIENTS_GetUnitY(pUnit) - nTargetY)) / nDistance;
	return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FCD1430
D2UnitStrc* __fastcall AITACTICS_GetTargetMinion(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2UnitStrc* pTarget = SUNIT_GetLastAttacker(pGame, pUnit);
	if (!pTarget)
	{
		return nullptr;
	}

	switch (pTarget->dwUnitType)
	{
	case UNIT_PLAYER:
		if (pTarget->dwAnimMode == PLRMODE_DEATH || pTarget->dwAnimMode == PLRMODE_DEAD)
		{
			return nullptr;
		}

		return pTarget;

	case UNIT_MONSTER:
		if (pTarget->dwAnimMode == MONMODE_DEATH || pTarget->dwAnimMode == MONMODE_DEAD || !sub_6FCBD900(pGame, pUnit, pTarget))
		{
			return nullptr;
		}

		return pTarget;
	default:
		return nullptr;
	}
}

//D2Game.0x6FCD1490
void __fastcall AITACTICS_UseSkillInRange(D2UnitStrc* pUnit, int32_t nRange, uint16_t wSkillId, uint8_t nMode)
{
	const int32_t nX = CLIENTS_GetUnitX(pUnit) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRange) - nRange;
	const int32_t nY = CLIENTS_GetUnitY(pUnit) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRange) - nRange;

	AITACTICS_UseSkill(pUnit->pGame, pUnit, nMode, wSkillId, nullptr, nX, nY);
}
