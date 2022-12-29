#include "AI/AiUtil.h"

#include <algorithm>

#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Math.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/SkillsIds.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <D2StatList.h>

#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "AI/AiThink.h"
#include "GAME/Clients.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"
#include "SKILLS/Skills.h"


#pragma pack(push, 1)
struct D2PartyCallbackArgStrc
{
	D2UnitStrc* (__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*);
	D2GameStrc* pGame;
	D2UnitStrc* pUnit;
	void* unk0x0C;
};

struct D2UnkAiTableStrc
{
	int32_t unk0x00;
	D2UnitStrc* (__fastcall* unk0x04)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*);
};
#pragma pack(pop)


//Inlined in D2Game.0x6FCF1210 and D2Game.0x6FCF1310
int32_t __fastcall AIUTIL_GetDistanceToCoordinatesWithSize(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nSize)
{
	const int32_t nXFinal = std::abs(std::abs(CLIENTS_GetUnitX(pUnit) - nX) - nSize);
	const int32_t nYFinal = std::abs(std::abs(CLIENTS_GetUnitY(pUnit) - nY) - nSize);

	if (nXFinal <= nYFinal)
	{
		return (nXFinal + 2 * nYFinal) / 2;
	}
	else
	{
		return (nYFinal + 2 * nXFinal) / 2;
	}
}

//D2Game.0x6FCF1210
int32_t __fastcall AIUTIL_GetDistanceToCoordinates_FullUnitSize(D2UnitStrc* pTarget, D2UnitStrc* pSource)
{
	return AIUTIL_GetDistanceToCoordinatesWithSize(pTarget, CLIENTS_GetUnitX(pSource), CLIENTS_GetUnitY(pSource), UNITS_GetUnitSizeX(pTarget));
}

//D2Game.0x6FCF1310
int32_t __fastcall AIUTIL_GetDistanceToCoordinates_HalfUnitSize(D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	return AIUTIL_GetDistanceToCoordinatesWithSize(pUnit, nX, nY, ((uint32_t)UNITS_GetUnitSizeX(pUnit) >> 1) + 1);
}

//D2Game.0x6FCF13B0
int32_t __fastcall AIUTIL_GetDistanceToCoordinates_NoUnitSize(D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	const int32_t nXDiff = std::abs(CLIENTS_GetUnitX(pUnit) - nX);
	const int32_t nYDiff = std::abs(CLIENTS_GetUnitY(pUnit) - nY);

	if (nXDiff <= nYDiff)
	{
		return (nXDiff + 2 * nYDiff) / 2;
	}
	else
	{
		return (nYDiff + 2 * nXDiff) / 2;
	}
}

//D2Game.0x6FCF1440
uint32_t __fastcall AIUTIL_GetDistanceToCoordinates(D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->pDynamicPath);

	const int32_t nXDiff = std::abs(PATH_GetXPosition(pUnit->pDynamicPath) - nX);
	const int32_t nYDiff = std::abs(PATH_GetYPosition(pUnit->pDynamicPath) - nY);

	if (nXDiff <= nYDiff)
	{
		return (nXDiff + 2 * nYDiff) / 2;
	}
	else
	{
		return (nYDiff + 2 * nXDiff) / 2;
	}
}

//D2Game.0x6FCF14D0
int32_t __fastcall sub_6FCF14D0(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	constexpr uint8_t nOffsets[25] = { 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

	if (!pUnit1 || !pUnit2)
	{
		return 0;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit1, pUnit2);
	const int32_t nOffset = nDistance < std::size(nOffsets) ? nOffsets[nDistance] : 3;
	const int32_t nX2 = CLIENTS_GetUnitX(pUnit2);
	const int32_t nY2 = CLIENTS_GetUnitY(pUnit2);

	const int32_t nOffsetX = D2signum(CLIENTS_GetUnitX(pUnit1) - nX2) * nOffset;
	const int32_t nOffsetY = D2signum(CLIENTS_GetUnitY(pUnit1) - nY2) * nOffset;

	if (UNITS_TestCollisionByCoordinates(pUnit1, nX2, nY2, COLLIDE_BLOCK_PLAYER | COLLIDE_WALL | COLLIDE_DOOR)
		&& UNITS_TestCollisionByCoordinates(pUnit1, nX2 - nOffsetY, nOffsetX + nY2, COLLIDE_BLOCK_PLAYER | COLLIDE_WALL | COLLIDE_DOOR)
		&& UNITS_TestCollisionByCoordinates(pUnit1, nOffsetY + nX2, nY2 - nOffsetX, COLLIDE_BLOCK_PLAYER | COLLIDE_WALL | COLLIDE_DOOR))
	{
		return 0;
	}

	return 1;
}

//D2Game.0x6FCF16D0
D2UnitStrc* __fastcall sub_6FCF16D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2BaalThroneAiCallbackArgStrc* pArg = (D2BaalThroneAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || SUNIT_IsDead(pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget);
	if (nDistance >= pArg->nMaxDistance || sub_6FCBD900(pGame, pUnit, pTarget))
	{
		return nullptr;
	}

	++pArg->unk0x08;
	if (nDistance >= pArg->nDistance)
	{
		return nullptr;
	}

	pArg->nDistance = nDistance;
	pArg->pTarget = pTarget;
	return nullptr;
}

//D2Game.0x6FCF1740
D2UnitStrc* __fastcall sub_6FCF1740(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	UnkAiStrc5* pArg = (UnkAiStrc5*)pCallbackArg;
	if (pTarget && pTarget->dwUnitType == UNIT_MONSTER && pTarget->dwAnimMode == MONMODE_DEATH && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit) < pArg->nMaxDistance)
	{
		return pTarget;
	}

	return nullptr;
}

//D2Game.0x6FCF1780
D2UnitStrc* __fastcall sub_6FCF1780(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2UnkAiCallbackArgStrc* pArg = (D2UnkAiCallbackArgStrc*)pCallbackArg;

	if (!pTarget)
	{
		return nullptr;
	}

	if ((pTarget->dwUnitType != UNIT_PLAYER && pTarget->dwUnitType != UNIT_MONSTER) || SUNIT_IsDead(pTarget) || !(pTarget->dwFlags & UNITFLAG_CANBEATTACKED) || !sub_6FCBD900(pGame, pUnit, pTarget))
	{
		if (pTarget->dwUnitType == UNIT_MONSTER && (!pUnit || pUnit->dwUnitType != UNIT_PLAYER) && !STATLIST_GetUnitAlignment(pUnit) && pArg->unk0x08 && !MONSTERS_IsDead(pTarget))
		{
			D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pTarget);

			if (pAiControl->nAiFlags & 8 && pArg->unk0x10 == D2Common_10095(UNITS_GetRoom(pTarget), CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget)))
			{
				pAiControl->nAiFlags |= 8u;
				pArg->unk0x08 = 0;
			}
		}

		return nullptr;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);
	if (nDistance > pArg->nMaxDistance)
	{
		return nullptr;
	}

	int32_t nThreat = 0;
	if (pTarget->dwUnitType == UNIT_PLAYER)
	{
		nThreat = 14;
		if (nDistance >= pArg->nDistance)
		{
			return nullptr;
		}
	}
	else if (pTarget->dwUnitType == UNIT_MONSTER)
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
		if (pMonStatsTxtRecord)
		{
			nThreat = pMonStatsTxtRecord->nThreat;
			if (nThreat > 1)
			{
				if (nDistance >= pArg->nDistance)
				{
					return nullptr;
				}
			}
			else
			{
				if (nDistance >= pArg->nAlternativeDistance)
				{
					return nullptr;
				}
			}
		}
		else
		{
			nThreat = 0;
			if (nDistance >= pArg->nAlternativeDistance)
			{
				return nullptr;
			}
		}
	}

	if (pArg->unk0x08 && UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
	{
		return nullptr;
	}

	if (nThreat <= 1)
	{
		pArg->pAlternativeTarget = pTarget;
		pArg->nAlternativeDistance = nDistance;
	}
	else
	{
		pArg->pTarget = pTarget;
		pArg->nDistance = nDistance;
	}

	return nullptr;
}

//D2Game.0x6FCF1980
D2UnitStrc* __fastcall sub_6FCF1980(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2UnkAiCallbackArgStrc2* pArg = (D2UnkAiCallbackArgStrc2*)pCallbackArg;

	if (!sub_6FCF1A50(pGame, pUnit, pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);
	if (nDistance > 48)
	{
		return nullptr;
	}

	int32_t nThreat = 0;
	if (!pTarget)
	{
		nThreat = 0;
		if (nDistance >= pArg->nDistance)
		{
			return nullptr;
		}
	}
	else
	{
		if (pTarget->dwUnitType == UNIT_PLAYER)
		{
			nThreat = 14;
			if (nDistance >= pArg->nDistance)
			{
				return nullptr;
			}
		}
		else if (pTarget->dwUnitType == UNIT_MONSTER)
		{
			D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
			if (pMonStatsTxtRecord)
			{
				nThreat = pMonStatsTxtRecord->nThreat;
				if (nThreat > 1)
				{
					if (nDistance >= pArg->nDistance)
					{
						return nullptr;
					}
				}
				else
				{
					if (nDistance >= pArg->nAlternativeDistance)
					{
						return nullptr;
					}
				}
			}
			else
			{
				nThreat = 0;
				if (nDistance >= pArg->nAlternativeDistance)
				{
					return nullptr;
				}
			}
		}
		else
		{
			nThreat = 0;
			if (nDistance >= pArg->nAlternativeDistance)
			{
				return nullptr;
			}
		}
	}

	if (!UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
	{
		if (nThreat > 1)
		{
			pArg->pTarget = pTarget;
			pArg->nDistance = nDistance;
		}
		else
		{
			pArg->pAlternativeTarget = pTarget;
			pArg->nAlternativeDistance = nDistance;
		}
	}

	return nullptr;
}

//D2Game.0x6FCF1A50
int32_t __fastcall sub_6FCF1A50(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
	if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER) || !pTarget || (pTarget->dwUnitType != UNIT_PLAYER && pTarget->dwUnitType != UNIT_MONSTER) || SUNIT_IsDead(pTarget) || SUNIT_IsDead(pUnit))
	{
		return 0;
	}

	if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)) || !(pTarget->dwFlags & UNITFLAG_CANBEATTACKED))
	{
		return 0;
	}

	if (STATES_CheckState(pTarget, STATE_INVIS))
	{
		if (pTarget->dwUnitType != UNIT_PLAYER)
		{
			if (pTarget->dwUnitType == UNIT_MONSTER && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
			{
				return 0;
			}
		}
		else
		{
			if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < 80 && !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
			{
				return 0;
			}
		}
	}

	return sub_6FCBD900(pGame, pUnit, pTarget);
}

//D2Game.0x6FCF1B30
D2UnitStrc* __fastcall sub_6FCF1B30(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2AiCallback7ArgStrc* pArg = (D2AiCallback7ArgStrc*)pCallbackArg;

	if (pTarget == pArg->unk0x08 || !sub_6FCF1A50(pGame, pUnit, pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);
	if (nDistance > 48 || !pTarget)
	{
		return nullptr;
	}

	if (pTarget->dwUnitType == UNIT_PLAYER)
	{
		if (nDistance < pArg->nDistance && !UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
		{
			pArg->pTarget = pTarget;
			pArg->nDistance = nDistance;
		}
	}
	else if (pTarget->dwUnitType == UNIT_MONSTER)
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
		if (pMonStatsTxtRecord->nThreat > 1 && nDistance < pArg->nDistance && !UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
		{
			pArg->pTarget = pTarget;
			pArg->nDistance = nDistance;
		}
	}

	return nullptr;
}

//D2Game.0x6FCF1BD0
D2UnitStrc* __fastcall sub_6FCF1BD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2AiCallback11ArgStrc* pArg = (D2AiCallback11ArgStrc*)pCallbackArg;

	if (pTarget == pArg->unk0x08 || !pTarget || (pTarget->dwUnitType != UNIT_PLAYER && pTarget->dwUnitType != UNIT_MONSTER) || SUNIT_IsDead(pTarget) || !(pTarget->dwFlags & UNITFLAG_CANBEATTACKED) || !sub_6FCBD900(pGame, pUnit, pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);
	if (nDistance > 48)
	{
		return nullptr;
	}

	if (pTarget->dwUnitType == UNIT_PLAYER)
	{
		if (nDistance < pArg->nDistance && sub_6FCF14D0(pUnit, pTarget))
		{
			pArg->pTarget = pTarget;
			pArg->nDistance = nDistance;
		}
	}
	else if (pTarget->dwUnitType == UNIT_MONSTER)
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
		if (pMonStatsTxtRecord->nThreat > 1 && nDistance < pArg->nDistance && sub_6FCF14D0(pUnit, pTarget))
		{
			pArg->pTarget = pTarget;
			pArg->nDistance = nDistance;
		}
	}

	return nullptr;
}

//D2Game.0x6FCF1CB0
D2UnitStrc* __fastcall sub_6FCF1CB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2DoorObjectAiCallbackArgStrc* pArg = (D2DoorObjectAiCallbackArgStrc*)pCallbackArg;

	if (!pTarget || pTarget->dwUnitType != UNIT_OBJECT || !UNITS_IsDoor(pTarget) || pTarget->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pTarget, pUnit);
	if (nDistance >= pArg->nDistance)
	{
		return nullptr;
	}

	pArg->pDoor = pTarget;
	pArg->nDistance = nDistance;
	return nullptr;
}

//D2Game.0x6FCF1DC0
D2UnitStrc* __fastcall sub_6FCF1DC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2FallenShamanAiCallbackArgStrc* pArg = (D2FallenShamanAiCallbackArgStrc*)pCallbackArg;
	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget);
	if (nDistance <= pArg->nMaxDistance && pTarget && pTarget->dwUnitType == UNIT_MONSTER && pTarget->dwAnimMode == MONMODE_DEAD && pTarget->dwFlags & UNITFLAG_TARGETABLE && !STATES_CheckStateMaskUdeadOnUnit(pTarget) && AIGENERAL_GetMinionOwner(pTarget) == pUnit)
	{
		pArg->pTarget = pTarget;
		pArg->nDistance = nDistance;
		++pArg->nCounter;
	}

	return nullptr;
}

//D2Game.0x6FCF1E30
D2UnitStrc* __fastcall sub_6FCF1E30(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2VileMotherAiCallbackArgStrc* pArg = (D2VileMotherAiCallbackArgStrc*)pCallbackArg;
	if (pTarget && pTarget->dwUnitType == UNIT_MONSTER && pTarget->dwClassId == pArg->nLastInClass && !SUNIT_IsDead(pTarget) && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget) <= pArg->nMaxDistance)
	{
		++pArg->nCounter;
	}

	return nullptr;
}

//D2Game.0x6FCF1E80
D2UnitStrc* __fastcall sub_6FCF1E80(D2GameStrc* pGame, D2UnitStrc* pUnit, void* a3, D2UnitStrc*(__fastcall* a4)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*), int32_t nCallbackId)
{
	D2UnkAiTableStrc stru_6FD29600[13] =
	{
		{ 0, nullptr },
		{ 0, nullptr },
		{ 1, nullptr },
		{ 0, sub_6FCF16D0 },
		{ 0, sub_6FCF1740 },
		{ 2, sub_6FCF1780 },
		{ 0, sub_6FCF1980 },
		{ 0, sub_6FCF1B30 },
		{ 0, sub_6FCF1CB0 },
		{ 3, sub_6FCF1DC0 },
		{ 3, nullptr },
		{ 2, sub_6FCF1BD0 },
		{ 0, sub_6FCF1E30 },
	};

	D2UnitStrc*(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*) = stru_6FD29600[nCallbackId].unk0x04;

	if (!pfCallback)
	{
		pfCallback = a4;
		if (!a4)
		{
			return nullptr;
		}
	}

	switch (stru_6FD29600[nCallbackId].unk0x00)
	{
	case 0:
	{
		return AIUTIL_FindTargetInAdjacentRooms(pGame, pUnit, a3, pfCallback);
	}
	case 1:
	{
		D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
		if (pRoom)
		{
			D2RoomStrc** ppRoomList = nullptr;
			int32_t nNumRooms = 0;
			DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

			for (int32_t i = 0; i < nNumRooms; ++i)
			{
				for (int32_t j = 0; j < ppRoomList[i]->nNumClients; ++j)
				{
					D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(ppRoomList[i]->ppClients[j], 0);
					if (pPlayer)
					{
						D2UnitStrc* pTarget = pfCallback(pGame, pUnit, pPlayer, a3);
						if (pTarget)
						{
							return pTarget;
						}
					}
				}
			}
		}
		return nullptr;
	}
	case 2:
	{
		return AIUTIL_FindTargetInAdjacentActiveRooms(pGame, pUnit, a3, pfCallback);
	}
	case 3:
	{
		D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
		if (pOwner && pOwner->dwUnitType == UNIT_MONSTER)
		{
			D2PartyCallbackArgStrc partyCallbackArg = {};
			partyCallbackArg.pfCallback = pfCallback;
			partyCallbackArg.pGame = pGame;
			partyCallbackArg.pUnit = pUnit;
			partyCallbackArg.unk0x0C = a3;

			D2UnitStrc* pTarget = nullptr;
			AIGENERAL_ExecuteCallbackOnMinions(pOwner, &partyCallbackArg, &pTarget, sub_6FCF20E0);
			return pTarget;
		}

		return nullptr;
	}
	}

	return nullptr;
}

//D2Game.0x6FCF20E0
void __fastcall sub_6FCF20E0(D2UnitStrc* pUnit, void* pArg, void* ppUnit)
{
	D2PartyCallbackArgStrc* pPartyCallbackArg = (D2PartyCallbackArgStrc*)pArg;

	if (*(D2UnitStrc**)ppUnit)
	{
		return;
	}

	D2UnitStrc* pResult = pPartyCallbackArg->pfCallback(pPartyCallbackArg->pGame, pPartyCallbackArg->pUnit, pUnit, pPartyCallbackArg->unk0x0C);
	if (pResult)
	{
		*(D2UnitStrc**)ppUnit = pResult;
	}
}

//D2Game.0x6FCF2110
D2UnitStrc* __fastcall sub_6FCF2110(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t* pDistance, int32_t* pCombat)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return nullptr;
	}

	// TODO: v66, v74
	int32_t v66 = 0;
	int32_t v74 = 0;
	void* pVision = nullptr;
	if (pAiControl->nAiFlags & 0x40)
	{
		v66 = 1;
		pAiControl->nAiFlags &= 0xFFBF;
	}
	else if (!DUNGEON_CheckLOSDraw(pRoom))
	{
		v66 = (pAiControl->nAiFlags & 8) == 0;

		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
		{
			if (pUnit->pMonsterData)
			{
				pVision = (void*)pUnit->pMonsterData->pVision;
			}

			if (v66 && pVision)
			{
				v74 = *((uint32_t*)pVision + 9);
				v66 = v74 == 0;
			}
		}
	}

	D2UnitStrc* pTargetUnit = nullptr;
	int32_t nDistanceToTarget = 0;
	int32_t nCurrentDistance = INT_MAX;

	if (!sub_6FCF2920(pGame, pUnit, v66, 0, &pTargetUnit, &nDistanceToTarget))
	{
		if (STATLIST_GetUnitAlignment(pUnit))
		{
			D2UnkAiCallbackArgStrc arg = {};
			arg.pTarget = nullptr;
			arg.nDistance = INT_MAX;
			arg.unk0x08 = v66;
			arg.nMaxDistance = 35;
			arg.unk0x10 = D2Common_10095(pRoom, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
			arg.pAlternativeTarget = nullptr;
			arg.nAlternativeDistance = INT_MAX;

			if (sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 5) && arg.pAlternativeTarget && sub_6FCF27B0(pUnit, &arg.pTarget, &arg.nDistance, arg.pAlternativeTarget, arg.nAlternativeDistance))
			{
				pTargetUnit = arg.pAlternativeTarget;
				nDistanceToTarget = arg.nAlternativeDistance;
			}
			else
			{
				pTargetUnit = arg.pTarget;
				nDistanceToTarget = arg.nDistance;
			}
		}
		else
		{
			nDistanceToTarget = 35;

			if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData && pUnit->pMonsterData->pMonstatsTxt)
			{
				const int32_t nAiDistance = pUnit->pMonsterData->pMonstatsTxt->nAiDist[pGame->nDifficulty];
				if (nAiDistance)
				{
					nDistanceToTarget = nAiDistance;
				}
			}

			const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
			const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);

			D2TargetNodeStrc* pTargetNode = nullptr;
			for (int32_t i = 0; i < 8; ++i)
			{
				pTargetNode = pGame->pTargetNodes[i];
				if (pTargetNode)
				{
					D2_ASSERT(pTargetNode->pUnit && pTargetNode->pUnit->dwUnitType == UNIT_PLAYER);

					if (pUnit->nAct == pTargetNode->pUnit->nAct)
					{
						D2RoomStrc* pTargetRoom = UNITS_GetRoom(pTargetNode->pUnit);
						if (pTargetRoom && !DUNGEON_IsRoomInTown(pTargetRoom))
						{
							int32_t nDistance = AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTargetNode->pUnit, nUnitX, nUnitY);
							if (nDistance < nCurrentDistance)
							{
								nCurrentDistance = nDistance;
							}

							if (nDistance < 55)
							{
								if (SUNIT_IsDead(pTargetNode->pUnit))
								{
									nDistance = INT_MAX;
								}

								while (1)
								{
									if (nDistance < nDistanceToTarget && (!v66 || !UNITS_TestCollisionWithUnit(pUnit, pTargetNode->pUnit, COLLIDE_WALL)))
									{
										nDistanceToTarget = nDistance;
										pTargetUnit = pTargetNode->pUnit;
									}

									pTargetNode = pTargetNode->pNext;
									if (!pTargetNode)
									{
										break;
									}

									nDistance = AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTargetNode->pUnit, nUnitX, nUnitY);
								}
							}
						}
					}
				}
			}

			while (pTargetNode)
			{
				if (pUnit->nAct == pTargetNode->pUnit->nAct)
				{
					const int32_t nTemp = AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTargetNode->pUnit, nUnitX, nUnitY);
					if (nTemp < nDistanceToTarget && (!v66 || !UNITS_TestCollisionWithUnit(pUnit, pTargetNode->pUnit, COLLIDE_WALL)))
					{
						nDistanceToTarget = nTemp;
						pTargetUnit = pTargetNode->pUnit;
					}
				}

				pTargetNode = pTargetNode->pNext;
			}

			int32_t nMinDistance = INT_MAX;

			pTargetNode = pGame->pTargetNodes[9];
			if (pTargetNode)
			{
				D2UnitStrc* pPotentialTarget = nullptr;
				do
				{
					if (pUnit->nAct == pTargetNode->pUnit->nAct)
					{
						const int32_t nTemp = AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTargetNode->pUnit, nUnitX, nUnitY);
						if (nTemp < nMinDistance && (!v66 || !UNITS_TestCollisionWithUnit(pUnit, pTargetNode->pUnit, COLLIDE_WALL)))
						{
							nMinDistance = nTemp;
							pPotentialTarget = pTargetNode->pUnit;
						}
					}

					pTargetNode = pTargetNode->pNext;
				}
				while (pTargetNode);

				if (pPotentialTarget && sub_6FCF27B0(pUnit, &pTargetUnit, &nDistanceToTarget, pPotentialTarget, nMinDistance))
				{
					pTargetUnit = pPotentialTarget;
					nDistanceToTarget = nMinDistance;
				}
			}
		}
	}

	if (!pTargetUnit)
	{
		*pCombat = 0;
		*pDistance = nCurrentDistance;
		return nullptr;
	}

	if (STATLIST_GetUnitAlignment(pUnit) != 2)
	{
		pAiControl->nAiFlags |= 8u;

		if (pVision)
		{
			*((DWORD*)pVision + 9) = v74 == 0;
		}
	}

	*pCombat = UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0);
	*pDistance = nDistanceToTarget;
	return pTargetUnit;
}

//D2Game.0x6FCF27B0
int32_t __fastcall sub_6FCF27B0(D2UnitStrc* pUnit, D2UnitStrc** ppTarget, int32_t* pTargetDistance, D2UnitStrc* a4, int32_t a5)
{
	if ((pUnit && pUnit->dwUnitType == UNIT_PLAYER) || !a4)
	{
		return 0;
	}

	if (!*ppTarget)
	{
		return 1;
	}

	if (a5 > 5)
	{
		return 0;
	}

	const int32_t nPathType = PATH_GetType(pUnit->pDynamicPath);
	D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
	const int32_t nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
	const int32_t nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);

	PATH_SetTargetUnit(pUnit->pDynamicPath, *ppTarget);
	PATH_SetType(pUnit->pDynamicPath, 2u);
	D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
	PATH_SetTargetUnit(pUnit->pDynamicPath, pTarget);
	D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX, nY);
	PATH_SetType(pUnit->pDynamicPath, nPathType);

	if (PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
	{
		return 0;
	}

	D2AiCallback7ArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nDistance = INT_MAX;
	arg.unk0x08 = *ppTarget;

	sub_6FCF1E80(pUnit->pGame, pUnit, &arg, nullptr, 7);

	if (!arg.pTarget || arg.nDistance >= 20)
	{
		return 1;
	}

	*ppTarget = arg.pTarget;
	*pTargetDistance = arg.nDistance;

	return 0;
}

//D2Game.0x6FCF2920
int32_t __fastcall sub_6FCF2920(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4, D2UnitStrc** ppUnit, int32_t* pDistance)
{
	D2UnitStrc* pTarget = nullptr;
	int32_t nDistance = 0;

	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
	{
		return 0;
	}

	switch (sub_6FC61EC0(pUnit))
	{
	case 0:
	{
		return 0;
	}
	case 1:
	{
		pTarget = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, sub_6FC61EE0(pUnit));
		if (!pTarget)
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);

		if (a4 && UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		break;
	}
	case 2:
	{
		pTarget = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, sub_6FC61EE0(pUnit));
		if (!pTarget)
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);

		if (a4 && UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		break;
	}
	case 4:
	{
		pTarget = SUNIT_GetServerUnit(pGame, UNIT_MISSILE, sub_6FC61EE0(pUnit));
		if (!pTarget)
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pTarget, pUnit);

		if (a4 && UNITS_TestCollisionWithUnit(pTarget, pUnit, COLLIDE_WALL))
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		break;
	}
	case 3:
	{
		const int32_t nAlignment = STATLIST_GetUnitAlignment(pUnit);
		if (nAlignment == 1)
		{
			if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
			{
				sub_6FCBDD30(pUnit, 2, 1);
			}
			else
			{
				sub_6FCBDD30(pUnit, 0, 1);
			}
		}
		else
		{
			if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
			{
				if (nAlignment == 0)
				{
					sub_6FCBDD30(pUnit, 2, 1);
				}
				else if (nAlignment == 2)
				{
					sub_6FCBDD30(pUnit, 0, 1);
				}
			}
		}

		if (a4)
		{
			D2UnkAiCallbackArgStrc2 arg = {};
			arg.pTarget = nullptr;
			arg.nDistance = INT_MAX;
			arg.pAlternativeTarget = nullptr;
			arg.nAlternativeDistance = INT_MAX;

			sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 6);

			pTarget = arg.pTarget;
			nDistance = arg.nDistance;
		}
		else
		{
			D2UnkAiCallbackArgStrc arg = {};
			arg.pTarget = nullptr;
			arg.nDistance = INT_MAX;
			arg.unk0x08 = a3;
			arg.nMaxDistance = 35;
			arg.unk0x10 = D2Common_10095(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
			arg.pAlternativeTarget = nullptr;
			arg.nAlternativeDistance = INT_MAX;

			sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 5);

			pTarget = arg.pTarget;
			nDistance = arg.nDistance;
		}

		sub_6FCBDD30(pUnit, nAlignment, 1);

		if (!pTarget)
		{
			sub_6FC61F00(pUnit);
			return 0;
		}
		break;
	}
	default:
	{
		sub_6FC61F00(pUnit);
		return 0;
	}
	}

	switch (pTarget->dwUnitType)
	{
	case UNIT_PLAYER:
	case UNIT_MONSTER:
		if (SUNIT_IsDead(pTarget))
		{
			sub_6FC61F00(pUnit);
			return 0;
		}

		*ppUnit = pTarget;
		*pDistance = nDistance;
		return 1;

	case UNIT_MISSILE:
		*ppUnit = pTarget;
		*pDistance = nDistance;
		return 1;

	default:
		sub_6FC61F00(pUnit);
		return 0;
	}
}

//D2Game.0x6FCF2B80
D2UnitStrc* __fastcall AIUTIL_FindTargetInAdjacentRooms(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg, D2UnitStrc* (__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*))
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return nullptr;
	}

	D2RoomStrc** ppRoomList = nullptr;
	int32_t nNumRooms = 0;
	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	for (int32_t i = 0; i < nNumRooms; ++i)
	{
		for (D2UnitStrc* j = ppRoomList[i]->pUnitFirst; j; j = j->pRoomNext)
		{
			D2UnitStrc* pTarget = pfCallback(pGame, pUnit, j, pArg);
			if (pTarget)
			{
				return pTarget;
			}
		}
	}

	return nullptr;
}

//D2Game.0x6FCF2C00
D2UnitStrc* __fastcall AIUTIL_FindTargetInAdjacentActiveRooms(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg, D2UnitStrc*(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*))
{
	if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER) || SUNIT_IsDead(pUnit))
	{
		return nullptr;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return nullptr;
	}

	D2RoomStrc** ppRoomList = nullptr;
	int32_t nNumRooms = 0;
	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	for (int32_t i = 0; i < nNumRooms; ++i)
	{
		if (!DUNGEON_IsRoomInTown(ppRoomList[i]) && ppRoomList[i]->nNumClients)
		{
			for (D2UnitStrc* j = ppRoomList[i]->pUnitFirst; j; j = j->pRoomNext)
			{
				D2UnitStrc* pTarget = pfCallback(pGame, pUnit, j, pArg);
				if (pTarget)
				{
					return pTarget;
				}
			}
		}
	}

	return nullptr;
}

//D2Game.0x6FCF2CC0
D2UnitStrc* __fastcall sub_6FCF2CC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pDistance, int32_t* pInMeleeRange)
{
	D2UnitStrc* pTarget = nullptr;
	int32_t nDistance = 0;

	if (!sub_6FCF2920(pGame, pUnit, 0, 1, &pTarget, &nDistance))
	{
		D2UnkAiCallbackArgStrc2 arg = {};
		arg.pTarget = nullptr;
		arg.nDistance = INT_MAX;
		arg.pAlternativeTarget = nullptr;
		arg.nAlternativeDistance = INT_MAX;

		sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 6);

		if (sub_6FCF27B0(pUnit, &arg.pTarget, &arg.nDistance, arg.pAlternativeTarget, arg.nAlternativeDistance))
		{
			pTarget = arg.pAlternativeTarget;
			nDistance = arg.nAlternativeDistance;
		}
		else
		{
			pTarget = arg.pTarget;
			nDistance = arg.nDistance;
		}
	}

	if (pTarget)
	{
		if (pInMeleeRange)
		{
			*pInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);
		}

		if (pDistance)
		{
			*pDistance = nDistance;
		}

		return pTarget;
	}

	if (pInMeleeRange)
	{
		*pInMeleeRange = 0;
	}

	if (pDistance)
	{
		*pDistance = INT_MAX;
	}

	return nullptr;
}

//D2Game.0x6FCF2DF0
int32_t __fastcall AIUTIL_CheckAiControlFlag(D2AiControlStrc* pAiControl, uint16_t nFlag)
{
	return (pAiControl->nAiFlags & nFlag) != 0;
}

//D2Game.0x6FCF2E00
void __fastcall AIUTIL_ToggleAiControlFlag(D2AiControlStrc* pAiControl, uint16_t nFlag, int32_t bSet)
{
	if (bSet)
	{
		pAiControl->nAiFlags |= nFlag;
	}
	else
	{
		pAiControl->nAiFlags &= ~nFlag;
	}
}

//D2Game.0x6FCF2E20
void __fastcall AIUTIL_ToggleAiControlFlag0x20(D2UnitStrc* pUnit, int32_t bSet)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (pAiControl)
	{
		AIUTIL_ToggleAiControlFlag(pAiControl, 0x20, bSet);
	}
}

//D2Game.0x6FCF2E70
int32_t __fastcall sub_6FCF2E70(D2UnitStrc* pUnit)
{
	const uint32_t nAiState = MONSTER_GetAiState(pUnit);
	return nAiState == 3 || nAiState == 19;
}

//D2Game.0x6FCF2E90
int32_t __fastcall AIUTIL_CheckIfMonsterUsesSkill(D2UnitStrc* pUnit, int32_t nSkillId)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
	{
		return 0;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (!pMonStatsTxtRecord)
	{
		return 0;
	}

	for (int32_t i = 0; i < 8; ++i)
	{
		if (pMonStatsTxtRecord->nSkill[i] == nSkillId)
		{
			return 1;
		}
	}

	return 0;
}

//D2Game.0x6FCF2EF0
void __fastcall AIUTIL_SetOwnerGUIDAndType(D2UnitStrc* pUnit, D2UnitStrc* pOwner)
{
	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	if (!pAiControl)
	{
		return;
	}

	if (pOwner)
	{
		pAiControl->dwOwnerGUID = pOwner->dwUnitId;
		pAiControl->dwOwnerType = pOwner->dwUnitType;
	}
	else
	{
		pAiControl->dwOwnerGUID = -1;
		pAiControl->dwOwnerType = 1;
	}
}

//D2Game.0x6FCF2F30
void __fastcall AIUTIL_ApplyTerrorCurseState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nParam1, int32_t nDuration)
{
	if (SUNIT_IsDead(pTarget) || !AIUTIL_CanUnitSwitchAi(pTarget, 11))
	{
		return;
	}

	D2CurseStrc curse = {};
	curse.pUnit = pUnit;
	curse.pTarget = pTarget;
	curse.nSkillLevel = 1;
	curse.nDuration = nDuration;
	curse.nStat = -1;
	curse.nSkill = SKILL_TERROR;
	curse.nState = STATE_TERROR;
	sub_6FD10EC0(&curse);

	UNITS_SetTargetUnitForDynamicUnit(pTarget, pUnit);

	if (pTarget && pTarget->dwUnitType == UNIT_MONSTER && pTarget->pMonsterData)
	{
		AITHINK_ExecuteAiFn(pGame, pTarget, pTarget->pMonsterData->pAiControl, 11);
		AITACTICS_Idle(pGame, pTarget, 1);
	}
}

//D2Game.0x6FCF3000
int32_t __fastcall AIUTIL_CanUnitSwitchAi(D2UnitStrc* pUnit, int32_t nSpecialState)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || nSpecialState > 19 || STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE) || !UNITS_CanSwitchAI(pUnit->dwClassId))
	{
		return 0;
	}

	if (pUnit->dwFlags & UNITFLAG_CANBEATTACKED)
	{
		return 1;
	}

	if (!SUNIT_IsDead(pUnit) || MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_SUPERUNIQUE))
	{
		return 0;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);

	if (nSpecialState == 19 || !pMonStatsTxtRecord)
	{
		return 1;
	}

	return AITHINK_CanUnitSwitchAi(pUnit, pMonStatsTxtRecord, nSpecialState, 1);
}
