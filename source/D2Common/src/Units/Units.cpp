#include "Units/Units.h"

#include "D2Collision.h"
#include "D2Composit.h"
#include "D2DataTbls.h"
#include "Drlg/D2DrlgMaze.h"
#include "D2Dungeon.h"
#include "D2Inventory.h"
#include "D2Items.h"
#include "D2Monsters.h"
#include "Path/Path.h"
#include "Path/Path_IDAStar.h"
#include "D2QuestRecord.h"
#include "D2Seed.h"
#include "D2Skills.h"
#include "D2States.h"
#include "D2StatList.h"
#include "Units/UnitRoom.h"
#include "D2Waypoints.h"
#include <D2BitManip.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <D2Math.h>

//D2Common.0x6FDBD520 (#10457)
uint8_t __stdcall UNITS_GetDirection(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM)
	{
		return pUnit->pStaticPath->nDirection;
	}
	else
	{
		return PATH_GetDirection(pUnit->pDynamicPath);
	}
}

//D2Common.0x6FDBD570 (#10320)
D2SkillStrc* __stdcall UNITS_GetStartSkill(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	return SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills);
}

//D2Common.0x6FDBD5B0 (#10321)
D2SkillStrc* __stdcall UNITS_GetGetLeftSkill(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	return SKILLS_GetLeftSkillFromSkillList(pUnit->pSkills);
}

//D2Common.0x6FDBD5F0 (#10322)
D2SkillStrc* __stdcall UNITS_GetRightSkill(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	return SKILLS_GetRightSkillFromSkillList(pUnit->pSkills);
}

//D2Common.0x6FDBD630 (#10324)
void __stdcall UNITS_SetUsedSkill(D2UnitStrc* pUnit, D2SkillStrc* pUsedSkill)
{
	D2_ASSERT(pUnit);
	SKILLS_SetUsedSkillInSkillList(pUnit->pSkills, pUsedSkill);
}

//D2Common.0x6FDBD670 (#10323)
D2SkillStrc* __stdcall UNITS_GetUsedSkill(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	return SKILLS_GetUsedSkillFromSkillList(pUnit->pSkills);
}

//D2Common.0x6FDBD6B0 (#11259)
D2UnitStrc* __stdcall UNITS_AllocUnit(void* pMemPool, int nUnitType)
{
	D2UnitStrc* pUnit = (D2UnitStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2UnitStrc), __FILE__, __LINE__, 0);
	memset(pUnit, 0x00, sizeof(D2UnitStrc));

	pUnit->pMemoryPool = pMemPool;
	pUnit->dwUnitType = nUnitType;

	if (nUnitType == UNIT_MONSTER)
	{
		pUnit->pMonsterData = (D2MonsterDataStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2MonsterDataStrc), __FILE__, __LINE__, 0);
		memset(pUnit->pMonsterData, 0x00, sizeof(D2MonsterDataStrc));
	}

	return pUnit;
}

//D2Common.0x6FDBD720 (#11260)
void __stdcall UNITS_FreeUnit(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		STATLIST_FreeStatListEx(pUnit);
		SKILLS_FreeSkillList(pUnit);

		if (pUnit->dwUnitType == UNIT_MONSTER)
		{
			FOG_FreeServerMemory(pUnit->pMemoryPool, pUnit->pMonsterData, __FILE__, __LINE__, 0);
		}

		pUnit->dwUnitType = 6;
		pUnit->dwUnitId = -1;

		FOG_FreeServerMemory(pUnit->pMemoryPool, pUnit, __FILE__, __LINE__, 0);
	}
}

//D2Common.0x6FDBD780 (#10327)
int __stdcall UNITS_GetPrecisionX(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath->nXPos << 16;

	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		return PATH_GetPrecisionX(pUnit->pDynamicPath);

	default:
		return 0;
	}
}

//D2Common.0x6FDBD7D0 (#10330)
int __stdcall UNITS_GetPrecisionY(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);


	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath->nYPos << 16;

	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		return PATH_GetPrecisionY(pUnit->pDynamicPath);

	default:
		return 0;
	}
}

//D2Common.0x6FDBD820 (#10328)
void __stdcall UNITS_SetXForStaticUnit(D2UnitStrc* pUnit, int nX)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		pUnit->pStaticPath->nXPos = nX;
		break;

	default:
		D2_UNREACHABLE;
	}
}

//D2Common.0x6FDBD890 (#10331)
void __stdcall UNITS_SetYForStaticUnit(D2UnitStrc* pUnit, int nY)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		pUnit->pStaticPath->nYPos = nY;
		break;

	default:
		D2_UNREACHABLE;
	}
}

//D2Common.0x6FDBD900 (#10336)
int __stdcall UNITS_GetUnitSizeX(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2MissilesTxt* pMissilesTxtRecord = NULL;

	if (pUnit)
	{
		switch (pUnit->dwUnitType)
		{
		case UNIT_PLAYER:
			return 2;

		case UNIT_MONSTER:
			pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
			if (pMonStats2TxtRecord)
			{
				return pMonStats2TxtRecord->nSizeX;
			}
			return 0;

		case UNIT_OBJECT:
			return pUnit->pObjectData->pObjectTxt->dwSizeX;

		case UNIT_ITEM:
			return 1;

		case UNIT_MISSILE:
			pMissilesTxtRecord = DATATBLS_GetMissilesTxtRecord(pUnit->dwClassId);
			if (pMissilesTxtRecord)
			{
				return pMissilesTxtRecord->nSize;
			}
			return 0;

		default:
			return 0;
		}
	}
	
	return 0;
}

//D2Common.0x6FDBDA00 (#10337)
int __stdcall UNITS_GetUnitSizeY(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2MissilesTxt* pMissilesTxtRecord = NULL;

	if (pUnit)
	{
		switch (pUnit->dwUnitType)
		{
		case UNIT_PLAYER:
			return 2;

		case UNIT_MONSTER:
			pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
			if (pMonStats2TxtRecord)
			{
				return pMonStats2TxtRecord->nSizeY;
			}
			return 0;

		case UNIT_OBJECT:
			return pUnit->pObjectData->pObjectTxt->dwSizeY;

		case UNIT_ITEM:
			return 1;

		case UNIT_MISSILE:
			pMissilesTxtRecord = DATATBLS_GetMissilesTxtRecord(pUnit->dwClassId);
			if (pMissilesTxtRecord)
			{
				return pMissilesTxtRecord->nSize;
			}
			return 0;

		default:
			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDBDB10 (#10333)
int __stdcall UNITS_GetTargetX(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath->nTargetX;

	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		return PATH_GetTargetX(pUnit->pDynamicPath);

	default:
		return 0;
	}
}

//D2Common.0x6FDBDB60 (#10334)
int __stdcall UNITS_GetTargetY(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath->nTargetY;

	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		return PATH_GetTargetY(pUnit->pDynamicPath);

	default:
		return 0;
	}
}

//D2Common.0x6FDBDBB0 (#10411)
int __stdcall UNITS_GetAbsoluteXDistance(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	int nDistance = 0;
	int nX1 = 0;
	int nX2 = 0;

	switch (pUnit1->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		nX1 = pUnit1->pStaticPath->nXPos;
		break;

	default:
		if (pUnit1->pDynamicPath)
		{
			nX1 = PATH_GetXPosition(pUnit1->pDynamicPath);
		}
		else
		{
			nX1 = 0;
		}
		break;
	}

	switch (pUnit2->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		nX2 = pUnit2->pStaticPath->nXPos;
		break;

	default:
		if (pUnit2->pDynamicPath)
		{
			nX2 = PATH_GetXPosition(pUnit2->pDynamicPath);
		}
		else
		{
			nX2 = 0;
		}
		break;
	}

	nDistance = nX1 - nX2;
	if (nDistance < 0)
	{
		nDistance = -nDistance;
	}

	return nDistance;
}

//D2Common.0x6FDBDC20 (#10412)
int __stdcall UNITS_GetAbsoluteYDistance(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	int nDistance = 0;
	int nY1 = 0;
	int nY2 = 0;

	switch (pUnit1->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		nY1 = pUnit1->pStaticPath->nYPos;
		break;

	default:
		if (pUnit1->pDynamicPath)
		{
			nY1 = PATH_GetYPosition(pUnit1->pDynamicPath);
		}
		else
		{
			nY1 = 0;
		}
		break;
	}

	switch (pUnit2->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		nY2 = pUnit2->pStaticPath->nYPos;
		break;

	default:
		if (pUnit2->pDynamicPath)
		{
			nY2 = PATH_GetYPosition(pUnit2->pDynamicPath);
		}
		else
		{
			nY2 = 0;
		}
		break;
	}

	nDistance = nY1 - nY2;
	if (nDistance < 0)
	{
		nDistance = -nDistance;
	}

	return nDistance;
}

//D2Common.0x6FDBDC90 (#10340)
void __stdcall UNITS_SetTargetX(D2UnitStrc* pUnit, int nTargetX)
{
	D2_ASSERT(pUnit);

	PATH_SetTargetX(pUnit->pDynamicPath, nTargetX);
}

//D2Common.0x6FDBDCD0 (#10341)
void __stdcall UNITS_SetTargetY(D2UnitStrc* pUnit, int nTargetY)
{
	D2_ASSERT(pUnit);

	PATH_SetTargetY(pUnit->pDynamicPath, nTargetY);
}

//D2Common.0x6FDBDD10 (#10332)
void __stdcall UNITS_GetCoords(D2UnitStrc* pUnit, D2CoordStrc* pCoord)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		pCoord->nX = pUnit->pStaticPath->nXPos;
		pCoord->nY = pUnit->pStaticPath->nYPos;
		break;

	default:
		if (pUnit->pDynamicPath)
		{
			pCoord->nX = PATH_GetXPosition(pUnit->pDynamicPath);
			pCoord->nY = PATH_GetYPosition(pUnit->pDynamicPath);
		}
		else
		{
			pCoord->nX = 0;
			pCoord->nY = 0;
		}
		break;
	}
}

//D2Common.0x6FDBDDA0 (#10335)
void __stdcall UNITS_GetTargetCoords(D2UnitStrc* pUnit, D2CoordStrc* pTargetCoords)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		pTargetCoords->nX = pUnit->pStaticPath->nTargetX;
		pTargetCoords->nY = pUnit->pStaticPath->nTargetY;
		break;

	default:
		pTargetCoords->nX = PATH_GetTargetX(pUnit->pDynamicPath);
		pTargetCoords->nY = PATH_GetTargetY(pUnit->pDynamicPath);
		break;
	}
}

//D2Common.0x6FDBDE10 (#10338)
int __fastcall UNITS_GetCollisionType(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
		if (pUnit->pObjectData->pObjectTxt->nIsDoor)
		{
			if (pUnit->pObjectData->pObjectTxt->nBlocksVis)
			{
				return 2054;
			}
			else
			{
				return pUnit->pObjectData->pObjectTxt->nBlockMissile != 0 ? 2052 : 1024;
			}
		}
		else
		{
			if (pUnit->pObjectData->pObjectTxt->nSubClass & OBJSUBCLASS_TOWNPORTAL)
			{
				return 32768;
			}
			else
			{
				return pUnit->pObjectData->pObjectTxt->nBlockMissile != 0 ? 1028 : 1024;
			}
		}

	case UNIT_ITEM:
		return 512;

	case UNIT_TILE:
		return 1;

	default:
		return PATH_GetCollisionType(pUnit->pDynamicPath);
	}
}

//D2Common.0x6FDBDEC0 (#10352)
void __stdcall UNITS_FreeCollisionPath(D2UnitStrc* pUnit)
{
	D2CoordStrc pCoords = {};

	UNITS_GetCoords(pUnit, &pCoords);

	COLLISION_ResetMaskWithSizeXY(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetUnitSizeY(pUnit), UNITS_GetCollisionType(pUnit));
}

//D2Common.0x6FDBE060 (#10351)
void __stdcall UNITS_BlockCollisionPath(D2UnitStrc* pUnit, D2RoomStrc* pRoom, int nX, int nY)
{
	int nSizeX = 0;
	int nSizeY = 0;

	D2_ASSERT(pUnit);

	nSizeX = UNITS_GetUnitSizeX(pUnit);
	nSizeY = UNITS_GetUnitSizeY(pUnit);

	if (nSizeX && nSizeY)
	{
		COLLISION_SetMaskWithSizeXY(pRoom, nX, nY, nSizeX, nSizeY, UNITS_GetCollisionType(pUnit));
	}
}

//D2Common.0x6FDBE1A0 (#10350)
void __stdcall UNITS_InitializeStaticPath(D2UnitStrc* pUnit, D2RoomStrc* pRoom, int nX, int nY)
{
	D2_ASSERT(pUnit);

	pUnit->pStaticPath->nDirection = 0;
	pUnit->pStaticPath->nXPos = nX;
	pUnit->pStaticPath->nYPos = nY;
	pUnit->pStaticPath->pRoom = pRoom;

	DUNGEON_WorldToScreenCoords(&nX, &nY);

	pUnit->pStaticPath->nTargetX = nX;
	pUnit->pStaticPath->nTargetY = nY;
}

//D2Common.0x6FDBE210 (#10343)
void __stdcall UNITS_ResetRoom(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		pUnit->pStaticPath->pRoom = NULL;
		break;

	default:
		if (pUnit->pDynamicPath)
		{
			PATH_SetRoom(pUnit->pDynamicPath, NULL);
		}
		break;
	}
}

//D2Common.0x6FDBE270 (#10342)
D2RoomStrc* __stdcall UNITS_GetRoom(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath->pRoom;

	default:
		if (pUnit->pDynamicPath)
		{
			return PATH_GetRoom(pUnit->pDynamicPath);
		}
		else
		{
			return NULL;
		}
		break;
	}
}

//D2Common.0x6FDBE2D0 (#10344)
void __stdcall UNITS_SetTargetUnitForDynamicUnit(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT((pUnit->dwUnitType == UNIT_PLAYER) || (pUnit->dwUnitType == UNIT_MONSTER) || (pUnit->dwUnitType == UNIT_MISSILE));
	PATH_SetTargetUnit(pUnit->pDynamicPath, pTargetUnit);
}

//D2Common.0x6FDBE330 (#10345)
int __stdcall UNITS_GetTargetTypeFromDynamicUnit(D2UnitStrc* pUnit)
{
	int nUnitType = 0;
	int nUnitId = 0;
	D2_ASSERT(pUnit);
	D2_ASSERT((pUnit->dwUnitType == UNIT_PLAYER) || (pUnit->dwUnitType == UNIT_MONSTER) || (pUnit->dwUnitType == UNIT_MISSILE));

	PATH_GetTargetTypeAndGUID(pUnit->pDynamicPath, &nUnitType, &nUnitId);
	return nUnitType;
}

//D2Common.0x6FDBE3A0 (#10346)
int __stdcall UNITS_GetTargetGUIDFromDynamicUnit(D2UnitStrc* pUnit)
{
	int nUnitType = 0;
	int nUnitId = 0;
	D2_ASSERT(pUnit);
	D2_ASSERT((pUnit->dwUnitType == UNIT_PLAYER) || (pUnit->dwUnitType == UNIT_MONSTER) || (pUnit->dwUnitType == UNIT_MISSILE));

	PATH_GetTargetTypeAndGUID(pUnit->pDynamicPath, &nUnitType, &nUnitId);
	return nUnitId;
}

//D2Common.0x6FDBE410 (#10347)
void __stdcall UNITS_SetTargetUnitForPlayerOrMonster(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT((pUnit->dwUnitType == UNIT_PLAYER) || (pUnit->dwUnitType == UNIT_MONSTER));

	PATH_SetTargetUnit(pUnit->pDynamicPath, pTargetUnit);
}

//D2Common.0x6FDBE470 (#10354)
void __stdcall UNITS_GetRunAndWalkSpeedForPlayer(int nUnused, int nCharId, int* pWalkSpeed, int* pRunSpeed)
{
	if (nCharId >= 0 && nCharId < sgptDataTables->nCharStatsTxtRecordCount)
	{
		*pWalkSpeed = sgptDataTables->pCharStatsTxt[nCharId].nWalkSpeed;
		*pRunSpeed = sgptDataTables->pCharStatsTxt[nCharId].nRunSpeed;
	}
}

//D2Common.0x6FDBE4C0 (#10325)
void __stdcall UNITS_SetAnimData(D2UnitStrc* pUnit, int nUnitType, int nClassId, int nMode)
{
	D2_ASSERT(pUnit);
	pUnit->pAnimData = DATATBLS_GetAnimDataRecord(pUnit, nClassId, nMode, nUnitType, pUnit->pInventory);
}

//Helper function
void __stdcall UNITS_CharacterStartRunningOrKnockback(D2UnitStrc* pUnit, int nClassId)
{
	D2StatListStrc* pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, STATLIST_TEMPONLY, 0, pUnit->dwUnitType, pUnit->dwUnitId);
	if (D2CharStatsTxt* pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(nClassId))
	{
		if (pCharStatsTxtRecord->nWalkSpeed != 0)
		{
			STATLIST_SetStat(pStatList, STAT_VELOCITYPERCENT, 100 * pCharStatsTxtRecord->nRunSpeed / pCharStatsTxtRecord->nWalkSpeed - 100, 0);
			D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
		}
	}
}

//D2Common.0x6FDBE510 (#10349)
void __stdcall UNITS_SetAnimStartFrame(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	pUnit->nActionFrame = 0;
	pUnit->dwGFXcurrentFrame = UNITS_GetFrameBonus(pUnit) << 8;

	int nUnitType = pUnit->dwUnitType;
	int nClassId = pUnit->dwClassId;
	int nNewMode = pUnit->dwAnimMode;

	D2COMMON_11013_ConvertMode(pUnit, &nUnitType, &nClassId, &nNewMode, __FILE__, __LINE__);

	switch (nUnitType)
	{
	case UNIT_PLAYER:
	{
		if (nNewMode == PLRMODE_RUN || nNewMode == PLRMODE_KNOCKBACK)
		{
			UNITS_CharacterStartRunningOrKnockback(pUnit, nClassId);
		}
		if (nNewMode == PLRMODE_SEQUENCE)
		{
			UNITS_InitializeSequence(pUnit);
			D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
		}
		else
		{
			pUnit->pAnimSeq = nullptr;
			D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
			pUnit->dwFrameCount = pUnit->pAnimData->dwFrames << 8;
		}
		break;
	}
	case UNIT_MONSTER:
	{
		if (pUnit->dwUnitType == UNIT_PLAYER && STATES_IsUnitShapeShifted(pUnit) && (nNewMode == MONMODE_KNOCKBACK || nNewMode == MONMODE_RUN))
		{
			UNITS_CharacterStartRunningOrKnockback(pUnit, pUnit->dwClassId);
		}
		if (nNewMode == MONMODE_SEQUENCE)
		{
			UNITS_InitializeSequence(pUnit);
			D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
		}
		else
		{
			D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
			pUnit->pAnimSeq = nullptr;
			pUnit->dwFrameCount = pUnit->pAnimData->dwFrames << 8;
		}
		break;
	}
	case UNIT_OBJECT:
	{
#define NUM_OBJECTMODES 8
		D2_ASSERT(nNewMode < NUM_OBJECTMODES);

		D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
		pUnit->dwFrameCount = pObjectsTxtRecord->dwFrameCnt[nNewMode];
		pUnit->dwGFXcurrentFrame = uint16_t(pObjectsTxtRecord->nStart[nNewMode]) << 8;
		int16_t wFrameDelta = pObjectsTxtRecord->wFrameDelta[nNewMode];

		if (pObjectsTxtRecord->nSync)
		{
			pUnit->wAnimSpeed = wFrameDelta;
		}
		else
		{
			const int nFrameDeltaDividedBy8 = wFrameDelta / 8;
			int uRandom = 0;
			if (nFrameDeltaDividedBy8 > 0)
			{
				uRandom = SEED_RollLimitedRandomNumber(&pUnit->pSeed, nFrameDeltaDividedBy8);
			}
			const int newAnimSpeed = uRandom + wFrameDelta - (wFrameDelta / 16);
			pUnit->wAnimSpeed = static_cast<int16_t>(D2Clamp(newAnimSpeed, 0, 0x7FFF));
		}
		break;
	}
	case UNIT_MISSILE:
	{
		if (D2MissilesTxt* pMissilesTxtRecord = DATATBLS_GetMissilesTxtRecord(pUnit->dwClassId))
		{
			pUnit->wAnimSpeed = ((int16_t)pMissilesTxtRecord->wAnimRate << 8) / 1024;
			pUnit->dwFrameCount = pMissilesTxtRecord->nAnimLen << 8;
		}
		break;
	}
	case UNIT_ITEM:
	{
		if (nNewMode == IMODE_ONGROUND)
		{
			pUnit->wAnimSpeed = 0;
			pUnit->dwFrameCount = 4352;
			pUnit->dwGFXcurrentFrame = 4096;
		}
		else if (nNewMode == IMODE_DROPPING)
		{
			pUnit->wAnimSpeed = 256;
			pUnit->dwFrameCount = 4352;
		}
		break;
	}
	default:
		break;
	}
}

//D2Common.0x6FDBEA60 (#10348)
BOOL __stdcall UNITS_ChangeAnimMode(D2UnitStrc* pUnit, int nMode)
{
	if (pUnit && pUnit->dwUnitType != UNIT_TILE)
	{
		if (pUnit->dwAnimMode != nMode)
		{
			UNITROOM_RefreshUnit(pUnit);
			pUnit->dwAnimMode = nMode;
			pUnit->dwFlags |= UNITFLAG_DOUPDATE;
			D2Common_10515(pUnit);
			UNITS_SetAnimStartFrame(pUnit);
			return TRUE;
		}

		if (pUnit->dwUnitType != UNIT_MONSTER || pUnit->dwAnimMode != MONMODE_NEUTRAL)
		{
			UNITROOM_RefreshUnit(pUnit);
			pUnit->dwFlags |= UNITFLAG_DOUPDATE;
		}
	}

	return FALSE;
}

//D2Common.0x6FDBEAD0 (#10355)
//TODO: Find a name
int __stdcall D2Common_10355(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM)
	{
		return pUnit->pStaticPath->unk0x1D[0];
	}
	else
	{
		return D2Common_10172(pUnit->pDynamicPath);
	}
}

//D2Common.0x6FDBEB20 (#10356)
//TODO: Find a name
void __stdcall D2Common_10356(D2UnitStrc* pUnit, int a2)
{
	D2_ASSERT(pUnit);

	switch (pUnit->dwUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		pUnit->pStaticPath->unk0x1D[0] = a2 != 0;
		return;

	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		D2Common_10173(pUnit->pDynamicPath, a2);
		return;

	default:
		return;
	}
}

//D2Common.0x6FDBEB80 (#10357)
void __stdcall UNITS_RefreshInventory(D2UnitStrc* pUnit, BOOL bSetFlag)
{
	if (bSetFlag)
	{
		UNITROOM_RefreshUnit(pUnit);
	}

	if (pUnit)
	{
		if (!bSetFlag)
		{
			pUnit->dwFlagEx &= ~UNITFLAGEX_HASINV;
			return;
		}
		else
		{
			pUnit->dwFlagEx |= UNITFLAGEX_HASINV;
		}
	}

	if (bSetFlag && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		pUnit->dwFlagEx |= UNITFLAGEX_UPDATEINV;
	}
}

//D2Common.0x6FDBEBE0 (#10409)
int __stdcall UNITS_GetInventoryRecordId(D2UnitStrc* pUnit, int nInvPage, BOOL bLoD)
{
	static const int nInventoryRecordMappings[NUMBER_OF_PLAYERCLASSES] =
	{
		INVENTORYRECORD_AMAZON,
		INVENTORYRECORD_SORCERESS,
		INVENTORYRECORD_NECROMANCER,
		INVENTORYRECORD_PALADIN,
		INVENTORYRECORD_BARBARIAN,
		INVENTORYRECORD_DRUID,
		INVENTORYRECORD_ASSASSIN
	};
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		switch ((uint8_t)nInvPage)
		{
		case INVPAGE_EQUIP:
			return INVENTORYRECORD_TRADE_PAGE_1;

		case INVPAGE_TRADE:
			return INVENTORYRECORD_TRADE_PAGE_2;

		case INVPAGE_STASH:
			return bLoD != 0 ? INVENTORYRECORD_BIG_BANK_PAGE_1 : INVENTORYRECORD_BANK_PAGE_1;

		case INVPAGE_CUBE:
			return INVENTORYRECORD_TRANSMOGRIFY_BOX_PAGE_1;

		default:
			if (pUnit->dwClassId >= 0 && pUnit->dwClassId < ARRAY_SIZE(nInventoryRecordMappings))
			{
				return nInventoryRecordMappings[pUnit->dwClassId];
			}
			return -1;
		}
	}
	else if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		return INVENTORYRECORD_MONSTER;
	}
	else if (pUnit->dwUnitType == UNIT_OBJECT)
	{
		if (pUnit->dwClassId == OBJECT_GUILD_VAULT)
		{
			return INVENTORYRECORD_GUILD_VAULT_PAGE_1;
		}
		else if (pUnit->dwClassId == OBJECT_TROPHY_CASE)
		{
			return INVENTORYRECORD_TROPHY_CASE_PAGE_1;
		}
	}

	return -1;
}

//D2Common.0x6FDBECD0 (#10383)
D2GfxLightStrc* __stdcall UNITS_GetLightMap(D2UnitStrc* pUnit)
{
	D2GfxLightStrc* pLight = NULL;
	D2_ASSERT(pUnit);

	pLight = pUnit->pLight;
	pUnit->pLight = NULL;

	return pLight;
}

//D2Common.0x6FDBED10 (#10369)
int __stdcall UNITS_GetAnimOrSeqMode(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		if (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pAnimSeq)
		{
			return pUnit->dwAnimMode;
		}
		else
		{
			return pUnit->dwSeqMode;
		}
	}

	return 0;
}

//D2Common.0x6FDBED40 (#10370)
void __stdcall UNITS_SetAnimOrSeqMode(D2UnitStrc* pUnit, int nAnimMode)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pAnimSeq)
	{
		pUnit->dwAnimMode = nAnimMode;
	}
	else
	{
		pUnit->dwSeqMode = nAnimMode;
	}
}

//D2Common.0x6FDBED90 (#10371)
void __stdcall UNITS_InitializeSequence(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		if (pUnit->pAnimSeq = DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit))
		{
			pUnit->dwSeqFrameCount = D2Common_10683(pUnit);
			pUnit->dwSeqFrame = 0;
			pUnit->dwAnimSpeed = 256;
			pUnit->dwFrameCount = D2Common_10684(pUnit) << 8;

			unsigned nMode = 0, nFrame = 0;
			int nDirection = 0, nEvent = 0;
			D2Common_10685(pUnit->pAnimSeq, pUnit->dwSeqFrame, 0, &nMode, &nFrame, &nDirection, &nEvent);

			pUnit->dwSeqMode = nMode;
			pUnit->nActionFrame = nEvent;
			pUnit->dwGFXcurrentFrame = nFrame << 8;
			pUnit->dwFlags |= UNITFLAG_SQGFXCHANGE;
		}
	}
}

//D2Common.0x6FDBEE20 (#10372)
void __stdcall UNITS_SetAnimationFrame(D2UnitStrc* pUnit, int nFrame)
{
	D2_ASSERT(pUnit);

	pUnit->dwSeqFrame = (nFrame << 8) + (pUnit->dwSeqFrame & 0xFF);
}

//D2Common.0x6FDBEE60 (#10373)
void __stdcall UNITS_StopSequence(D2UnitStrc* pUnit)
{
	unsigned int nOldFrame = 0;
	unsigned int nOldMode = 0;
	unsigned int nNewMode = 0;
	unsigned int nCounter = 0;
	unsigned int nFrame = 0;
	int nDirection = 0;
	int nEvent = 0;

	pUnit->nActionFrame = 0;

	if (pUnit->pAnimSeq)
	{
		nOldFrame = pUnit->dwSeqFrame;

		pUnit->dwFlags &= ~UNITFLAG_SQGFXCHANGE;
		pUnit->dwSeqFrame += pUnit->dwAnimSpeed;

		if (pUnit->dwSeqFrame >= pUnit->dwSeqFrameCount)
		{
			pUnit->dwSeqFrame -= pUnit->dwSeqFrameCount;
		}

		pUnit->dwFrameCount -= pUnit->dwAnimSpeed;

		nOldMode = pUnit->dwSeqMode;
		D2Common_10685(pUnit->pAnimSeq, pUnit->dwSeqFrame, nOldFrame, &nNewMode, &nFrame, &nDirection, &nEvent);

		pUnit->dwSeqMode = nNewMode;
		pUnit->dwGFXcurrentFrame = nFrame << 8;
		pUnit->nActionFrame = nEvent;

		if (nNewMode != nOldMode)
		{
			pUnit->dwFlags |= UNITFLAG_SQGFXCHANGE;
		}
	}
	else
	{
		nFrame = pUnit->dwGFXcurrentFrame >> 8;
		if (pUnit->wAnimSpeed >= 256)
		{
			++nFrame;
		}

		pUnit->dwGFXcurrentFrame += pUnit->wAnimSpeed;
		
		while (pUnit->dwGFXcurrentFrame >= pUnit->dwFrameCount)
		{
			nCounter = nFrame;
			while (nCounter < (pUnit->dwFrameCount >> 8))
			{
				UNITS_SetAnimActionFrame(pUnit, nCounter);

				++nCounter;
			}

			nFrame = 0;
			pUnit->dwGFXcurrentFrame -= pUnit->dwFrameCount;
			pUnit->dwGFXcurrentFrame += (UNITS_GetFrameBonus(pUnit) << 8);
		}
		
		nCounter = nFrame;
		while (nCounter <= (pUnit->dwGFXcurrentFrame >> 8))
		{
			UNITS_SetAnimActionFrame(pUnit, nCounter);

			++nCounter;
		}
	}
}

//D2Common.0x6FDBEFF0 (#10374)
void __stdcall UNITS_UpdateFrame(D2UnitStrc* pUnit)
{
	pUnit->dwGFXcurrentFrame = pUnit->wAnimSpeed + pUnit->dwGFXcurrentFrame;

	if ((int)pUnit->dwGFXcurrentFrame >= (int)pUnit->dwFrameCount)
	{
		pUnit->dwGFXcurrentFrame = pUnit->wAnimSpeed + pUnit->dwGFXcurrentFrame - pUnit->dwFrameCount;
	}
}

//D2Common.0x6FDBF020 (#10375)
//TODO: Check name
void __stdcall D2COMMON_10375_UNITS_SetFrameNonRate(D2UnitStrc* pUnit, int nRate, int nFailRate)
{
	pUnit->dwGFXcurrentFrame += pUnit->wAnimSpeed;

	if ((int)pUnit->dwGFXcurrentFrame >= nFailRate << 8)
	{
		pUnit->dwGFXcurrentFrame += pUnit->wAnimSpeed + (nRate << 8) - (nFailRate << 8);
	}
}


static int dword_6FDD2C28[] =
{
	0, 0, 0, 0,
};

static int dword_6FDD2C38[] =
{
	1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};

static int dword_6FDD2DB8[] =
{
	0, 0, 0, 0,
};

static int dword_6FDD2DC8[] =
{
	1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

static int dword_6FDD2EF8[] =
{
	0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0x78, 0x5D, 1, 0x78, 0x63, 1, 0x78, 0x69, 1, 0x78, 0x66, 1, 0x96, 0x60, 0
};

__forceinline void __fastcall UNITS_UpdateAttackAnimRateAndVelocity(D2UnitStrc* pUnit)
{
	D2UnitStrc* pRightWeapon = NULL;
	D2UnitStrc* pLeftWeapon = NULL;
	int nFasterAttackRate = 0;
	int nRightAttackRate = 0;
	int nLeftAttackRate = 0;
	int nAttackRate = 0;
	int nBaseSpeed = 0;
	int nAnimSpeed = 0;
	int nRate = 0;

	UNITS_MergeDualWieldWeaponStatLists(pUnit, 1);

	nFasterAttackRate = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_FASTERATTACKRATE, 0);

	if (nFasterAttackRate)
	{
		nFasterAttackRate = 120 * nFasterAttackRate / (nFasterAttackRate + 120);
	}

	nAttackRate = STATLIST_GetUnitStatUnsigned(pUnit, STAT_ATTACKRATE, 0);
	nRate = nFasterAttackRate + nAttackRate;

	if (UNITS_CanDualWield(pUnit))
	{
		pRightWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
		if (!ITEMS_CanBeEquipped(pRightWeapon))
		{
			pRightWeapon = 0;
		}

		pLeftWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
		if (!ITEMS_CanBeEquipped(pLeftWeapon))
		{
			pLeftWeapon = 0;
		}

		if (ITEMS_CheckItemTypeId(pRightWeapon, ITEMTYPE_WEAPON) && ITEMS_CheckItemTypeId(pLeftWeapon, ITEMTYPE_WEAPON))
		{
			nRightAttackRate = STATLIST_GetUnitStatUnsigned(pRightWeapon, STAT_ATTACKRATE, 0);
			nLeftAttackRate = STATLIST_GetUnitStatUnsigned(pLeftWeapon, STAT_ATTACKRATE, 0);

			nRate += (nRightAttackRate + nLeftAttackRate) / 2 - nRightAttackRate;
		}
	}

	if (!pUnit->dwUnitType && pUnit->dwAnimMode == PLRMODE_SEQUENCE)
	{
		nRate -= 30;
	}

	if (nRate >= 15)
	{
		if (nRate > 175)
		{
			nRate = 175;
		}
	}
	else
	{
		nRate = 15;
	}

	if (!STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_MELEEONLY))
	{
		nBaseSpeed = pUnit->pAnimData->dwAnimSpeed;
	}
	else
	{
		nBaseSpeed = D2Common_11043(pUnit);
		if (!nBaseSpeed)
		{
			nBaseSpeed = pUnit->pAnimData->dwAnimSpeed;
		}
	}

	nAnimSpeed = nRate * nBaseSpeed / 100;
	if (nAnimSpeed <= 0)
	{
		nAnimSpeed = 0;
	}
	else if (nAnimSpeed >= 32767)
	{
		nAnimSpeed = 32767;
	}

	pUnit->dwAnimSpeed = nAnimSpeed;
	pUnit->wAnimSpeed = nAnimSpeed;
}

__forceinline void __fastcall UNITS_UpdateBlockAnimRateAndVelocity(D2UnitStrc* pUnit)
{
	int nFasterBlockRate = 0;
	int nBlockRateBonus = 0;
	int nAnimSpeed = 0;

	nFasterBlockRate = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_FASTERBLOCKRATE, 0);
	if (nFasterBlockRate)
	{
		nFasterBlockRate = 120 * nFasterBlockRate / (nFasterBlockRate + 120);
	}

	if (STATES_CheckState(pUnit, STATE_HOLYSHIELD))
	{
		nBlockRateBonus = 100;
	}
	else
	{
		nBlockRateBonus = 50;
	}

	nAnimSpeed = (pUnit->pAnimData->dwAnimSpeed * (nBlockRateBonus + nFasterBlockRate)) / 100;
	if (nAnimSpeed < 1)
	{
		nAnimSpeed = 1;
	}
	else if (nAnimSpeed >= 32767)
	{
		nAnimSpeed = 32767;
	}

	pUnit->wAnimSpeed = nAnimSpeed;
}

__forceinline void __fastcall UNITS_UpdateCastAnimRateAndVelocity(D2UnitStrc* pUnit)
{
	int nFasterCastRate = 0;
	int nAnimSpeed = 0;
	int nRate = 0;

	nFasterCastRate = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_FASTERCASTRATE, 0);
	if (nFasterCastRate)
	{
		nFasterCastRate = (120 * nFasterCastRate) / (nFasterCastRate + 120);
	}

	nRate = nFasterCastRate + 100;
	if (nRate > 175)
	{
		nRate = 175;
	}

	nAnimSpeed = nRate * pUnit->pAnimData->dwAnimSpeed / 100;
	if (nAnimSpeed <= 0)
	{
		nAnimSpeed = 0;
	}
	else if (nAnimSpeed >= 32767)
	{
		nAnimSpeed = 32767;
	}

	pUnit->wAnimSpeed = nAnimSpeed;
	pUnit->dwAnimSpeed = nAnimSpeed;
}

__forceinline void __fastcall UNITS_UpdateGetHitAnimRateAndVelocity(D2UnitStrc* pUnit)
{
	int nFasterGetHitRate = 0;
	int nAnimSpeed = 0;

	nFasterGetHitRate = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_FASTERGETHITRATE, 0);
	if (nFasterGetHitRate)
	{
		nFasterGetHitRate = 120 * nFasterGetHitRate / (nFasterGetHitRate + 120);
	}

	nAnimSpeed = (pUnit->pAnimData->dwAnimSpeed * (nFasterGetHitRate + 50)) / 100;

	if (nAnimSpeed <= 0)
	{
		nAnimSpeed = 0;
	}
	else if (nAnimSpeed >= 32767)
	{
		nAnimSpeed = 32767;
	}

	pUnit->wAnimSpeed = nAnimSpeed;
}

__forceinline void __fastcall UNITS_UpdateKnockbackAnimRateAndVelocity(D2UnitStrc* pUnit, int nAnimMode, int nUnitType, int nClassId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = 0;
	int nAnimSpeed = 0;

	if (nUnitType == UNIT_PLAYER)
	{
		if (nAnimMode == PLRMODE_RUN)
		{
			nAnimSpeed = 101;
		}
		else
		{
			nAnimSpeed = 213;
		}
	}
	else if (nUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
		if (nAnimMode == MONMODE_RUN)
		{
			nAnimSpeed = pMonStatsTxtRecord->unk0x38;
		}
		else
		{
			nAnimSpeed = pMonStatsTxtRecord->unk0x36;
		}
	}

	if (nAnimSpeed <= 0)
	{
		nAnimSpeed = 0;
	}
	else if (nAnimSpeed >= 32767)
	{
		nAnimSpeed = 32767;
	}

	pUnit->wAnimSpeed = nAnimSpeed;
	PATH_SetVelocity(pUnit->pDynamicPath, 4096, __FILE__, __LINE__);
}

//TODO: v77
__forceinline void __fastcall UNITS_UpdateOtherAnimRateAndVelocity(D2UnitStrc* pUnit, int nAnimMode, int nUnitType, int nClassId)
{
	int v77 = 0;
	int nOtherAnimRate = 0;
	int nAnimSpeed = 0;

	if (nUnitType == UNIT_PLAYER)
	{
		v77 = dword_6FDD2C38[5 * nAnimMode];
	}
	else if (nUnitType == UNIT_MONSTER)
	{
		if (nClassId >= 410)
		{
			v77 = dword_6FDD2EF8[5 * nAnimMode + 4];
		}
		else
		{
			v77 = dword_6FDD2DC8[5 * nAnimMode];
		}
	}

	if (v77)
	{
		nAnimSpeed = pUnit->pAnimData->dwAnimSpeed;
	}
	else
	{
		nOtherAnimRate = STATLIST_GetUnitStatUnsigned(pUnit, STAT_OTHER_ANIMRATE, 0);
		if (nOtherAnimRate >= 15)
		{
			if (nOtherAnimRate > 175)
			{
				nOtherAnimRate = 175;
			}
		}
		else
		{
			nOtherAnimRate = 15;
		}

		nAnimSpeed = (nOtherAnimRate * pUnit->pAnimData->dwAnimSpeed) / 100;
	}

	if (nAnimSpeed <= 0)
	{
		nAnimSpeed = 0;
	}
	else if (nAnimSpeed >= 32767)
	{
		nAnimSpeed = 32767;
	}

	pUnit->wAnimSpeed = nAnimSpeed;
}

//TODO: ...
__forceinline void __fastcall UNITS_UpdateRunWalkAnimRateAndVelocity(D2UnitStrc* pUnit, int nAnimMode, int nUnitType, int nClassId, char* szFile, int nLine)
{
	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nFasterMoveVelocity = 0;
	int nVelocityPercent = 0;
	int nAnimSpeed = 0;
	int nVelocity = 0;
	int nBase = 0;
	int v53 = 0;
	int v54 = 0;
	char v60[4] = {};

	if (pUnit->pDynamicPath)
	{
		nFasterMoveVelocity = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_FASTERMOVEVELOCITY, 0);
		if (nFasterMoveVelocity)
		{
			nFasterMoveVelocity = 150 * nFasterMoveVelocity / (nFasterMoveVelocity + 150);
		}

		nVelocityPercent = nFasterMoveVelocity + STATLIST_GetUnitStatUnsigned(pUnit, STAT_VELOCITYPERCENT, 0);
		if (nVelocityPercent < 25)
		{
			nVelocityPercent = 25;
		}

		if (nUnitType == UNIT_PLAYER)
		{
			if (nAnimMode == PLRMODE_RUN)
			{
				nBase = 101;
			}
			else
			{
				nBase = 213;
			}
		}
		else if (nUnitType == UNIT_MONSTER)
		{
			pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
			if (nAnimMode == MONMODE_RUN)
			{
				nBase = pMonStatsTxtRecord->unk0x38;
			}
			else
			{
				nBase = pMonStatsTxtRecord->unk0x36;
			}
		}

		v53 = (signed int)((unsigned __int64)(1374389535i64 * nVelocityPercent * nBase) >> 32) >> 5;
		v54 = (unsigned int)((unsigned __int64)(1374389535i64 * nVelocityPercent * nBase) >> 32) >> 31;

		nAnimSpeed = v54 + v53;

		if (nAnimSpeed < 0 || nAnimSpeed > 32767)
		{
			FOG_WriteToLogFile("UnitUpdateAnimRateAndVel(): bad velocity:%d  (TYPE:%d  CLASS:%d)  FILE:%s  LINE:%d", nAnimSpeed, nUnitType, nClassId, szFile, nLine);
		}

		if (nAnimSpeed <= 0)
		{
			nAnimSpeed = 0;
		}
		else if (nAnimSpeed >= 32767)
		{
			nAnimSpeed = 32767;
		}

		pUnit->wAnimSpeed = nAnimSpeed;

		if (nUnitType == UNIT_PLAYER)
		{
			pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(nClassId);
			*(uint32_t*)v60 = 0;
			*(uint16_t*)&v60[1] = pCharStatsTxtRecord->nWalkSpeed;
			nVelocity = *(uint32_t*)v60;
		}
		else if (nUnitType == UNIT_MONSTER)
		{
			nVelocity = DATATBLS_GetMonStatsTxtRecord(nClassId)->nVelocity << 8;
		}

		PATH_SetVelocity(pUnit->pDynamicPath, nVelocityPercent * nVelocity / 100, __FILE__, __LINE__);
	}
	else
	{
		FOG_WriteToLogFile("UnitUpdateAnimRateAndVel(): NULL path (TYPE:%d  CLASS:%d)  FILE:%s  LINE:%d", nUnitType, nClassId, szFile, nLine);
	}
}

//D2Common.0x6FDBF050
//TODO: Check everything related to this function
void __stdcall D2COMMON_10376_UpdateAnimRateAndVelocity(D2UnitStrc* pUnit, char* szFile, int nLine)
{
	D2SkillStrc* v10 = NULL;
	int nUnitType = 0;
	int nAnimMode = 0;
	int nClassId = 0;
	int v11 = 0;
	D2SkillsTxt* v12 = NULL;
	int* v43 = NULL;
	D2SkillStrc* v44 = NULL;
	int v45 = 0;
	int* v61 = NULL;
	D2MonStatsTxt* v41 = NULL;
	D2SkillStrc* v62 = NULL;
	int v63 = 0;

	if (!pUnit)
	{
		FOG_WriteToLogFile("UnitUpdateAnimRateAndVel(): NULL unit  FILE:%s  LINE:%d", szFile, nLine);
		return;
	}

	nUnitType = pUnit->dwUnitType;
	if (nUnitType >= 5)
	{
		FOG_WriteToLogFile("UnitUpdateAnimRateAndVel(): invalid unit (TYPE:%d)  FILE:%s  LINE:%d", nUnitType, szFile, nLine);
		return;
	}

	if (nUnitType != UNIT_ITEM)
	{
		nClassId = pUnit->dwClassId;
		nAnimMode = pUnit->dwAnimMode;
		D2COMMON_11013_ConvertMode(pUnit, &nUnitType, &nClassId, &nAnimMode, __FILE__, __LINE__);

		pUnit->pAnimData = DATATBLS_GetAnimDataRecord(pUnit, nClassId, nAnimMode, nUnitType, pUnit->pInventory);

		switch (nUnitType)
		{
		case UNIT_PLAYER:
			if (nAnimMode == PLRMODE_CAST)
			{
				return UNITS_UpdateCastAnimRateAndVelocity(pUnit);
			}
			else if (nAnimMode == PLRMODE_SEQUENCE)
			{
				v10 = SKILLS_GetUsedSkillFromSkillList(pUnit->pSkills);
				if (v10)
				{
					v11 = SKILLS_GetSkillIdFromSkill(v10, __FILE__, __LINE__);
					v12 = DATATBLS_GetSkillsTxtRecord(v11);

					if (v12 && v12->nSeqTrans == 10)
					{
						return UNITS_UpdateCastAnimRateAndVelocity(pUnit);
					}
				}
			}
			else if (nAnimMode == PLRMODE_BLOCK)
			{
				return UNITS_UpdateBlockAnimRateAndVelocity(pUnit);
			}
			else if (nAnimMode == PLRMODE_GETHIT)
			{
				return UNITS_UpdateGetHitAnimRateAndVelocity(pUnit);
			}
			else if (nAnimMode == PLRMODE_KNOCKBACK)
			{
				return UNITS_UpdateKnockbackAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId);
			}

			v43 = &dword_6FDD2C28[5 * nAnimMode];

			if (v43[1])
			{
				return UNITS_UpdateRunWalkAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId, szFile, nLine);
			}

			if (v43[0])
			{
				v44 = UNITS_GetUsedSkill(pUnit);
				if (v44)
				{
					v45 = SKILLS_GetFlags(v44);
					if (v45 & 1 && !(v45 & 0x100))
					{
						return UNITS_UpdateRunWalkAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId, szFile, nLine);
					}
				}
			}

			v61 = &dword_6FDD2C28[5 * nAnimMode];

			if (v61[3])
			{
				return UNITS_UpdateAttackAnimRateAndVelocity(pUnit);
			}

			if (v61[2])
			{
				v62 = SKILLS_GetUsedSkillFromSkillList(pUnit->pSkills);
				if (v62)
				{
					v63 = SKILLS_GetSkillIdFromSkill(v62, __FILE__, __LINE__);
					if (gdwBitMasks[SKILLSFLAGINDEX_USEATTACKRATE] & DATATBLS_GetSkillsTxtRecord(v63)->dwFlags[0])
					{
						return UNITS_UpdateAttackAnimRateAndVelocity(pUnit);
					}
				}
			}

			return UNITS_UpdateOtherAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId);

		case UNIT_MONSTER:
			if (nAnimMode == MONMODE_CAST)
			{
				return UNITS_UpdateCastAnimRateAndVelocity(pUnit);
			}
			else if (nAnimMode == MONMODE_BLOCK)
			{
				return UNITS_UpdateBlockAnimRateAndVelocity(pUnit);
			}
			else if (nAnimMode == MONMODE_GETHIT)
			{
				return UNITS_UpdateGetHitAnimRateAndVelocity(pUnit);
			}
			else if (nAnimMode == MONMODE_KNOCKBACK)
			{
				return UNITS_UpdateKnockbackAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId);
			}

			v41 = DATATBLS_GetMonStatsTxtRecord(nClassId);
			if (v41)
			{
				if (gdwBitMasks[MONSTATSFLAGINDEX_NPC] & v41->dwMonStatsFlags)
				{
					if (nAnimMode != MONMODE_WALK && nAnimMode != MONMODE_RUN)
					{
						goto LABEL_75;
					}

					return UNITS_UpdateRunWalkAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId, szFile, nLine);
				}
			}

			if (nClassId >= 410)
			{
				v43 = &dword_6FDD2EF8[5 * nAnimMode];
			}
			else
			{
				v43 = &dword_6FDD2DB8[5 * nAnimMode];
			}

			if (v43[1])
			{
				return UNITS_UpdateRunWalkAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId, szFile, nLine);
			}

			if (v43[0])
			{
				v44 = UNITS_GetUsedSkill(pUnit);
				if (v44)
				{
					v45 = SKILLS_GetFlags(v44);
					if (v45 & 1 && !(v45 & 0x100))
					{
						return UNITS_UpdateRunWalkAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId, szFile, nLine);
					}
				}
			}

LABEL_75:
			if (nClassId >= 410)
			{
				v61 = &dword_6FDD2EF8[5 * nAnimMode];
			}
			else
			{
				v61 = &dword_6FDD2DB8[5 * nAnimMode];
			}

			if (v61[3])
			{
				return UNITS_UpdateAttackAnimRateAndVelocity(pUnit);
			}

			if (v61[2])
			{
				v62 = SKILLS_GetUsedSkillFromSkillList(pUnit->pSkills);
				if (v62)
				{
					v63 = SKILLS_GetSkillIdFromSkill(v62, __FILE__, __LINE__);
					if (gdwBitMasks[SKILLSFLAGINDEX_USEATTACKRATE] & DATATBLS_GetSkillsTxtRecord(v63)->dwFlags[0])
					{
						return UNITS_UpdateAttackAnimRateAndVelocity(pUnit);
					}
				}
			}

			return UNITS_UpdateOtherAnimRateAndVelocity(pUnit, nAnimMode, nUnitType, nClassId);

		default:
			FOG_Assertion("(eTOU == UNIT_PLAYER) || (eTOU == UNIT_MONSTER)", __FILE__, __LINE__);
			exit(-1);
		}
	}
}


////TODO:Remove
//D2FUNC(D2COMMON, 10376, void, __stdcall, (D2UnitStrc*, char*, int), 0x7F050)
//void __stdcall D2COMMON_10376_UpdateAnimRateAndVelocity(D2UnitStrc* pUnit, char* szFile, int nLine)
//{
//	return D2COMMON_10376(pUnit, szFile, nLine);
//}
///////////////


//D2Common.0x6FDBF8D0 (#10377)
void __stdcall UNITS_SetAnimationSpeed(D2UnitStrc* pUnit, int nSpeed)
{
	if (nSpeed > 0)
	{
		if (nSpeed >= 32767)
		{
			pUnit->wAnimSpeed = 32767;
		}
		else
		{
			pUnit->wAnimSpeed = nSpeed;
		}
	}
	else
	{
		pUnit->wAnimSpeed = 0;
	}
}

//D2Common.0x6FDBF910 (#10378)
int __stdcall UNITS_IsAtEndOfFrameCycle(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	
	if (pUnit->pAnimSeq)
	{
		return (int)pUnit->dwFrameCount <= 0;
	}
	else
	{
		return (int)(pUnit->dwGFXcurrentFrame + pUnit->wAnimSpeed) >= (int)pUnit->dwFrameCount;
	}
}

//D2Common.0x6FDBF970 (#10379)
void __stdcall UNITS_GetShiftedFrameMetrics(D2UnitStrc* pUnit, int* pFrameNo, int* pFrameCount)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_OBJECT)
	{
		*pFrameNo = pUnit->dwGFXcurrentFrame >> 8;
		*pFrameCount = pUnit->pObjectData->pObjectTxt->dwFrameCnt[pUnit->dwAnimMode];
	}
	else
	{
		*pFrameNo = pUnit->dwGFXcurrentFrame >> 8;
		*pFrameCount = pUnit->dwFrameCount >> 8;
	}
}

//D2Common.0x6FDBF9E0 (#10380)
void __stdcall UNITS_GetFrameMetrics(D2UnitStrc* pUnit, int* pFrame, int* pFrameCount)
{
	if (pUnit)
	{
		if (pUnit->dwUnitType == UNIT_OBJECT)
		{
			*pFrame = pUnit->dwGFXcurrentFrame;
			*pFrameCount = pUnit->pObjectData->pObjectTxt->dwFrameCnt[pUnit->dwAnimMode];
		}
		else
		{
			*pFrame = pUnit->dwGFXcurrentFrame;
			*pFrameCount = pUnit->dwFrameCount;
		}
	}
	else
	{
		*pFrame = 0;
		*pFrameCount = 0;
	}
}

//D2Common.0x6FDBFA40 (#10381)
void __stdcall UNITS_SetAnimActionFrame(D2UnitStrc* pUnit, int nFrame)
{
	if (pUnit && pUnit->pAnimData)
	{
		if (nFrame < 144)
		{
			if (pUnit->pAnimData->pFrameFlags[nFrame] == 1)
			{
				pUnit->nActionFrame = 1;
			}
			else if (pUnit->pAnimData->pFrameFlags[nFrame] == 2)
			{
				pUnit->nActionFrame = 2;
			}
			else if (pUnit->pAnimData->pFrameFlags[nFrame] == 3)
			{
				pUnit->nActionFrame = 3;
			}
			else if (pUnit->pAnimData->pFrameFlags[nFrame] == 4)
			{
				pUnit->nActionFrame = 4;
			}
		}
	}
}

//D2Common.0x6FDBFA90 (#10382)
int __stdcall UNITS_GetEventFrameInfo(D2UnitStrc* pUnit, int nFrame)
{
	BOOL bSequence = FALSE;
	unsigned int nMode = 0;
	unsigned int nTemp = 0;
	int nDirection = 0;
	int nEvent = 0;
	D2_ASSERT(pUnit);

	if (nFrame >= 144)
	{
		return 0;
	}

	if (pUnit->dwUnitType != UNIT_PLAYER)
	{
		if (pUnit->dwUnitType != UNIT_MONSTER)
		{
			return pUnit->pAnimData->pFrameFlags[nFrame];
		}
	}
	else if (pUnit->dwAnimMode == PLRMODE_SEQUENCE)
	{
		bSequence = TRUE;
	}

	if (pUnit->dwAnimMode != MONMODE_SEQUENCE && !bSequence)
	{
		return pUnit->pAnimData->pFrameFlags[nFrame];
	}

	D2Common_10685(DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit), nFrame << 8, nFrame << 8, &nMode, &nTemp, &nDirection, &nEvent);
	return nEvent;
}

//D2Common.0x6FDBFB40 (#10410)
BOOL __stdcall UNITS_HasCollision(D2UnitStrc* pUnit)
{
	if (pUnit->dwUnitType == UNIT_OBJECT)
	{
		return pUnit->pObjectData->pObjectTxt->nHasCollision[pUnit->dwAnimMode];
	}

	return TRUE;
}

//D2Common.0x6FDBFB70 (#10358)
D2SkillStrc* __stdcall UNITS_GetSkillFromSkillId(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	D2SkillStrc* pSkill = NULL;

	D2_ASSERT(pSkillsTxtRecord);
	D2_ASSERT(pUnit);

	pSkill = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills);
	if (pSkill)
	{
		while (pSkill->pSkillsTxt != pSkillsTxtRecord)
		{
			pSkill = pSkill->pNextSkill;

			if (!pSkill)
			{
				return NULL;
			}
		}

		return pSkill;
	}

	return NULL;
}

//D2Common.0x6FDBFC10 (#10392)
BOOL __stdcall UNITS_IsDoor(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_OBJECT)
	{
		return pUnit->pObjectData->pObjectTxt->nIsDoor;
	}

	return FALSE;
}

//D2Common.0x6FDBFC50
bool __fastcall UNITS_CheckIfObjectOrientationIs1(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	return pUnit->pObjectData->pObjectTxt->nOrientation == 1;
}

//D2Common.0x6FDBFC90 (#10393)
BOOL __stdcall UNITS_IsShrine(D2UnitStrc* pUnit)
{
	if (pUnit->dwUnitType == UNIT_OBJECT && pUnit->pObjectData->pObjectTxt->nSubClass & OBJSUBCLASS_SHRINE)
	{
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDBFCB0 (#10394)
D2ObjectsTxt* __stdcall UNITS_GetObjectTxtRecordFromObject(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType != UNIT_OBJECT)
	{
		FOG_10024_PacketAssertion(0, __FILE__, __LINE__);
		exit(-1);
	}

	return pUnit->pObjectData->pObjectTxt;
}

//D2Common.0x6FDBFD00 (#10395)
D2ShrinesTxt* __stdcall UNITS_GetShrineTxtRecordFromObject(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);


	if (pUnit->dwUnitType != UNIT_OBJECT)
	{
		FOG_10024_PacketAssertion(0, __FILE__, __LINE__);
		exit(-1);
	}

	return pUnit->pObjectData->pShrineTxt;
}

//D2Common.0x6FDBFD50 (#10396)
void __stdcall UNITS_SetShrineTxtRecordInObjectData(D2UnitStrc* pUnit, D2ShrinesTxt* pShrinesTxtRecord)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType != UNIT_OBJECT)
	{
		FOG_10024_PacketAssertion(0, __FILE__, __LINE__);
		exit(-1);
	}

	pUnit->pObjectData->pShrineTxt = pShrinesTxtRecord;
}

//D2Common.0x6FDBFDB0 (#10413)
void __stdcall UNITS_UpdateDirectionAndSpeed(D2UnitStrc* pUnit, int nX, int nY)
{
	if (pUnit->pDynamicPath)
	{
		D2Common_10216(pUnit->pDynamicPath, nX, nY, 0);
	}
}

//D2Common.0x6FDBFDD0 (#10414)
int __stdcall UNITS_GetNewDirection(D2UnitStrc* pUnit)
{
	int nX = 0;
	int nY = 0;
	D2CoordStrc pCoords = {};
	D2_ASSERT(pUnit);

	UNITS_GetCoords(pUnit, &pCoords);

	nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
	nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);

	if (nX != pCoords.nX || nY != pCoords.nY)
	{		
		return D2Common_10215(pCoords.nX, pCoords.nY, nX, nY);
	}
	else
	{
		return PATH_GetNewDirection(pUnit->pDynamicPath);
	}
}

//D2Common.0x6FDBFF20 (#10416)
void __stdcall UNITS_StoreOwnerTypeAndGUID(D2UnitStrc* pUnit, int nOwnerType, int nOwnerId)
{
	UNITS_StoreOwnerInfo(pUnit, nOwnerType, nOwnerId);
}

//D2Common.0x6FDBFF40
void __fastcall UNITS_StoreOwnerInfo(D2UnitStrc* pUnit, int nOwnerType, int nOwnerId)
{
	D2StatListStrc* pStatList = NULL;

	if (pUnit)
	{
		pUnit->dwOwnerType = nOwnerType;
		pUnit->dwOwnerGUID = nOwnerId;

		if (pUnit->pStatListEx)
		{
			STATES_ToggleState(pUnit, STATE_SOURCEUNIT, TRUE);
			pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_SOURCEUNIT);
			if (!pStatList)
			{
				pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0, 0, pUnit->dwUnitType, pUnit->dwUnitId);
				if (!pStatList)
				{
					return;
				}

				STATLIST_SetState(pStatList, STATE_SOURCEUNIT);
				D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
			}

			STATLIST_SetStatIfListIsValid(pStatList, STAT_SOURCE_UNIT_TYPE, nOwnerType, 0);
			STATLIST_SetStatIfListIsValid(pStatList, STAT_SOURCE_UNIT_ID, nOwnerId, 0);
		}

		pUnit->dwFlagEx |= UNITFLAGEX_STOREOWNERINFO;
	}
}

//D2Common.0x6FDBFFE0 (#10415)
void __stdcall UNITS_StoreOwner(D2UnitStrc* pUnit, D2UnitStrc* pOwner)
{
	D2StatListStrc* pStatList = NULL;

	if (pOwner)
	{
		UNITS_StoreOwnerInfo(pUnit, pOwner->dwUnitType, pOwner->dwUnitId);
	}
	else
	{
		if (pUnit)
		{
			pUnit->dwOwnerType = 0;
			pUnit->dwOwnerGUID = 0;

			if (pUnit->pStatListEx)
			{
				STATES_ToggleState(pUnit, STATE_SOURCEUNIT, FALSE);

				pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_SOURCEUNIT);
				if (pStatList)
				{
					D2Common_10474(pUnit, pStatList);
					STATLIST_FreeStatList(pStatList);
				}
			}

			pUnit->dwFlagEx &= ~UNITFLAGEX_STOREOWNERINFO;
		}
	}
}

//D2Common.0x6FDC0060 (#10417)
void __stdcall UNITS_StoreLastAttacker(D2UnitStrc* pUnit, D2UnitStrc* pKiller)
{
	D2_ASSERT(pUnit);

	if (pKiller)
	{
		pUnit->dwKillerType = pKiller->dwUnitType;
		pUnit->dwKillerGUID = pKiller->dwUnitId;
		pUnit->dwFlagEx |= UNITFLAGEX_STORELASTATTACKER;
	}
	else
	{
		pUnit->dwKillerType = 0;
		pUnit->dwKillerGUID = 0;
		pUnit->dwFlagEx &= ~UNITFLAGEX_STORELASTATTACKER;
	}
}

//D2Common.0x6FDC00E0 (#10418)
int __stdcall UNITS_GetDirectionToCoords(D2UnitStrc* pUnit, int nNewX, int nNewY)
{
	D2CoordStrc pCoords = {};

	UNITS_GetCoords(pUnit, &pCoords);

	return D2Common_10215(pCoords.nX, pCoords.nY, nNewX, nNewY);
}

//D2Common.0x6FDC0160 (#10437)
void __stdcall UNITS_SetOverlay(D2UnitStrc* pUnit, int nOverlay, int nUnused)
{
	D2StatListStrc* pStatList = NULL;

	if (nOverlay >= 0 && nOverlay < sgptDataTables->nOverlayTxtRecordCount)
	{
		pStatList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 0x80);
		if (!pStatList)
		{
			if (pUnit)
			{
				pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0x80, 0, pUnit->dwUnitType, pUnit->dwUnitId);
			}
			else
			{
				pStatList = STATLIST_AllocStatList(NULL, 0x80, 0, 6, -1);
			}

			D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
		}

		D2Common_10534(pUnit);
		D2Common_10531_SetStatInStatListLayer0(pStatList, STAT_UNIT_DOOVERLAY, (uint16_t)nOverlay, nUnused);
		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDC01F0 (#10367)
int __stdcall UNITS_GetBeltType(D2UnitStrc* pUnit)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_ITEM);

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pUnit->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->nBelt;
}

//D2Common.0x6FDC0260 (#10368)
int __stdcall UNITS_GetCurrentLifePercentage(D2UnitStrc* pUnit)
{
	int nMaxHp = 0;
	int nHp = 0;

	nHp = STATLIST_GetUnitStatUnsigned(pUnit, STAT_HITPOINTS, 0) >> 8;
	nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit) >> 8;
	if (nMaxHp)
	{
		return 100 * nHp / nMaxHp;
	}

	return 0;
}

//D2Common.0x6FDC02A0 (#10359)
BOOL __stdcall UNITS_IsSoftMonster(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SOFT])
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FDC0320 (#10420)
void __stdcall UNITS_AllocPlayerData(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(!pUnit->pPlayerData);

	pUnit->pPlayerData = (D2PlayerDataStrc*)FOG_AllocServerMemory(pUnit->pMemoryPool, sizeof(D2PlayerDataStrc), __FILE__, __LINE__, 0);
	memset(pUnit->pPlayerData, 0x00, sizeof(D2PlayerDataStrc));
	pUnit->pPlayerData->szName[0] = 0;

	for (int i = 0; i < ARRAY_SIZE(pUnit->pPlayerData->pQuestData); ++i)
	{
		pUnit->pPlayerData->pQuestData[i] = QUESTRECORD_AllocRecord(pUnit->pMemoryPool);
		pUnit->pPlayerData->pWaypointData[i] = WAYPOINTS_AllocWaypointData(pUnit->pMemoryPool);
	}

	pUnit->pPlayerData->nPortalFlags = DUNGEON_GetPortalFlagFromLevelId(LEVEL_ROGUEENCAMPMENT);
	pUnit->pPlayerData->dwTradeTick = 0;

	//D2COMMON_11296_Return(pUnit);
}

//D2Common.0x6FDC03F0 (#10421)
void __stdcall UNITS_FreePlayerData(void* pMemPool, D2UnitStrc* pPlayer)
{
	D2_ASSERT(pPlayer);
	D2_ASSERT(pPlayer->pPlayerData);

	for (int i = 0; i < ARRAY_SIZE(pPlayer->pPlayerData->pQuestData); ++i)
	{
		QUESTRECORD_FreeRecord(pMemPool, pPlayer->pPlayerData->pQuestData[i]);
		WAYPOINTS_FreeWaypointData(pMemPool, pPlayer->pPlayerData->pWaypointData[i]);

	}

	//D2COMMON_10916_Return();

	FOG_FreeServerMemory(pMemPool, pPlayer->pPlayerData, __FILE__, __LINE__, 0);
	pPlayer->pPlayerData = NULL;
}

//D2Common.0x6FDC04A0 (#10422)
void __stdcall UNITS_SetNameInPlayerData(D2UnitStrc* pUnit, char* szName)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->pPlayerData);
	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);

	strcpy_s(pUnit->pPlayerData->szName, szName);
}

//D2Common.0x6FDC0530 (#10423)
char* __stdcall UNITS_GetPlayerName(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->pPlayerData);
	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);

	return pUnit->pPlayerData->szName;
}

//D2Common.0x6FDC05B0 (#10424)
D2PlayerDataStrc* __stdcall UNITS_GetPlayerData(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);

	return pUnit->pPlayerData;
}

//D2Common.0x6FDC0600 (#10425)
void __stdcall UNITS_SetPlayerPortalFlags(D2UnitStrc* pUnit, int nPortalFlags)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);

	pUnit->pPlayerData->nPortalFlags = nPortalFlags;
}

//D2Common.0x6FDC0660 (#10426)
int __stdcall UNITS_GetPlayerPortalFlags(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);

	return pUnit->pPlayerData->nPortalFlags;
}

//D2Common.0x6FDC06C0 (#10353)
uint32_t __stdcall UNITS_GetNameOffsetFromObject(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_OBJECT)
	{
		return pUnit->pObjectData->pObjectTxt->dwNameOffset;
	}

	return 0;
}

//D2Common.0x6FDC0700 (#10427)
uint8_t __stdcall UNITS_GetObjectPortalFlags(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->dwUnitType == UNIT_OBJECT);
	return pUnit->pObjectData->nPortalFlags;
}

//D2Common.0x6FDC0760 (#10428)
void __stdcall UNITS_SetObjectPortalFlags(D2UnitStrc* pUnit, uint8_t nPortalFlag)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->dwUnitType == UNIT_OBJECT);
	pUnit->pObjectData->nPortalFlags = nPortalFlag;
}

//D2Common.0x6FDC07C0 (#10429)
BOOL __stdcall UNITS_CheckObjectPortalFlag(D2UnitStrc* pUnit, uint8_t nFlag)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->dwUnitType == UNIT_OBJECT);

	return (nFlag & pUnit->pObjectData->nPortalFlags) != 0;
}

//D2Common.0x6FDC0820 (#10430)
int __stdcall UNITS_GetOverlayHeight(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (pUnit)
	{
		if (pUnit->dwUnitType == UNIT_PLAYER)
		{
			return 1;
		}
		else if (pUnit->dwUnitType == UNIT_MONSTER)
		{
			pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
			if (pMonStats2TxtRecord)
			{
				return pMonStats2TxtRecord->nOverlayHeight - 1;
			}
		}
	}

	return 0;
}

//D2Common.0x6FDC08B0 (#10431)
int __stdcall UNITS_GetDefense(D2UnitStrc* pUnit)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	D2UnitStrc* pShieldItem = NULL;
	int nArmorOverridePercent = 0;
	int nDefenseBonus = 0;
	int nTotalDefense = 0;
	int nArmorPercent = 0;
	int nSkillLevel = 0;
	int nSkillId = 0;
	int nDefense = 0;

	nDefense = STATLIST_GetUnitStatUnsigned(pUnit, STAT_DEXTERITY, 0) / 4 + STATLIST_GetUnitStatUnsigned(pUnit, STAT_ARMORCLASS, 0);
	nArmorPercent = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_ARMOR_PERCENT, 0) + STATLIST_GetUnitStatSigned(pUnit, STAT_SKILL_ARMOR_PERCENT, 0);

	if (STATES_CheckState(pUnit, STATE_HOLYSHIELD))
	{
		pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_HOLYSHIELD);
		if (pStatList)
		{
			nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
			nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);

			pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
			if (pSkillsTxtRecord && nSkillLevel > 0)
			{
				INVENTORY_GetEquippedShield(pUnit->pInventory, &pShieldItem);

				if (pShieldItem)
				{
					nArmorPercent += SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
				}
			}
		}
	}

	if (nDefense <= 0)
	{
		nDefenseBonus = nDefense * nArmorPercent / -100;
	}
	else
	{
		nDefenseBonus = nDefense * nArmorPercent / 100;
	}

	nTotalDefense = nDefenseBonus + nDefense;
	nArmorOverridePercent = STATLIST_GetUnitStatUnsigned(pUnit, STAT_ARMOR_OVERRIDE_PERCENT, 0);

	if (!nArmorOverridePercent)
	{
		return nTotalDefense;
	}

	return nTotalDefense + (int)((long long int)nTotalDefense * (long long int)nArmorOverridePercent / 100);
}

//D2Common.0x6FDC0AC0 (#10432)
int __stdcall UNITS_GetAttackRate(D2UnitStrc* pAttacker)
{
	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	int nAttackRate = 0;
	int nDexterity = 0;
	int nToHit = 0;

	D2_ASSERT(pAttacker && pAttacker->dwUnitType == UNIT_PLAYER);

	nToHit = STATLIST_GetUnitStatUnsigned(pAttacker, STAT_TOHIT, 0);
	nDexterity = STATLIST_GetUnitStatUnsigned(pAttacker, STAT_DEXTERITY, 0);
	nAttackRate = nToHit + 5 * (nDexterity - 7);

	if (pAttacker->dwUnitType == UNIT_PLAYER && pAttacker->dwClassId >= 0 && pAttacker->dwClassId < sgptDataTables->nCharStatsTxtRecordCount)
	{
		pCharStatsTxtRecord = &sgptDataTables->pCharStatsTxt[pAttacker->dwClassId];
		if (pCharStatsTxtRecord)
		{
			return nAttackRate + pCharStatsTxtRecord->dwToHitFactor;
		}
	}

	return nAttackRate;
}

//D2Common.0x6FDC0B60 (#10433)
//TODO: v8
int __stdcall UNITS_GetBlockRate(D2UnitStrc* pUnit, BOOL bExpansion)
{
	static const int dword_6FDE5CEC[] =
	{
		' hct'
	};

	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nBlockChance = 0;
	int nClassId = 0;
	int nCounter = 0;
	int nItemId = 0;
	int nLevel = 0;
	int v8 = 0;

	if (!pUnit)
	{
		return 0;
	}

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		if (INVENTORY_GetEquippedShield(pUnit->pInventory, NULL) && pUnit->dwClassId >= 0 && pUnit->dwClassId < sgptDataTables->nCharStatsTxtRecordCount)
		{
			pCharStatsTxtRecord = &sgptDataTables->pCharStatsTxt[pUnit->dwClassId];

			nBlockChance = pCharStatsTxtRecord->nBlockFactor + STATLIST_GetUnitStatUnsigned(pUnit, STAT_TOBLOCK, 0);
			if (bExpansion)
			{
				nLevel = STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
				if (nLevel <= 1)
				{
					nLevel = 1;
				}

				nBlockChance = nBlockChance * (STATLIST_GetUnitStatUnsigned(pUnit, STAT_DEXTERITY, 0) - 15) / (2 * nLevel);
			}

			if (nBlockChance > 75)
			{
				nBlockChance = 75;
			}
			return nBlockChance;
		}
	}
	else if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		nClassId = pUnit->dwClassId;

		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
		if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NOSHLDBLOCK]))
		{
			if (nClassId < 0 || nClassId >= sgptDataTables->nMonStatsTxtRecordCount)
			{
				nClassId = -1;
			}

			switch (nClassId)
			{
			case MONSTER_DIABLO:
			case MONSTER_DOOMKNIGHT1:
			case MONSTER_DIABLOCLONE:
				nBlockChance = STATLIST_GetUnitStatUnsigned(pUnit, STAT_TOBLOCK, 0);
				if (nBlockChance > 75)
				{
					nBlockChance = 75;
				}
				return nBlockChance;

			case MONSTER_ACT3HIRE:
				return nBlockChance;

			default:
				v8 = D2Common_11069(pUnit, 7, pUnit->pMonsterData ? pUnit->pMonsterData->nComponent[7] : 0);
				if (v8)
				{
					nCounter = 0;
					while (nCounter < ARRAY_SIZE(dword_6FDE5CEC))
					{
						if (v8 == dword_6FDE5CEC[nCounter])
						{
							return nBlockChance;
						}

						++nCounter;
					}

					pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(v8, &nItemId);
					if (pItemsTxtRecord && pItemsTxtRecord->wType[0] == ITEMTYPE_SHIELD)
					{
						nBlockChance = STATLIST_GetUnitStatUnsigned(pUnit, STAT_TOBLOCK, 0);
						if (nBlockChance > 75)
						{
							nBlockChance = 75;
						}
					}
				}

				return nBlockChance;
			}
		}

		nBlockChance = STATLIST_GetUnitStatUnsigned(pUnit, STAT_TOBLOCK, 0);
		if (nBlockChance > 75)
		{
			nBlockChance = 75;
		}
		return nBlockChance;
	}

	return 0;
}

//D2Common.0x6FDC0DA0 (#10434)
//TODO: Find a name, v14
D2UnitStrc* __stdcall D2Common_10434(D2UnitStrc* pUnit, BOOL a2)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pSkill = NULL;
	D2UnitStrc* pHandItem = NULL;
	D2UnitStrc* pWeapon = NULL;
	D2UnitStrc* pItem = NULL;
	int nBodyLoc = 0;
	int nFrame = 0;
	int v14; // ecx@22
	BOOL bIsLeftHandItem = FALSE;


	if (!pUnit->pInventory)
	{
		return NULL;
	}

	INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pItem, &nBodyLoc, &bIsLeftHandItem);

	if (!UNITS_CanDualWield(pUnit))
	{
		return pItem;
	}

	pSkill = SKILLS_GetUsedSkillFromSkillList(pUnit->pSkills);
	if (!pSkill)
	{
		return pItem;
	}

	pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
	if (!pSkillsTxtRecord)
	{
		return pItem;
	}

	pWeapon = NULL;
	switch (pSkillsTxtRecord->nWeapSel)
	{
	case 1:
		pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_LEFTHAND);
		break;
	
	case 2:
		pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_RIGHTHAND);
		pHandItem = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_LEFTHAND);

		if (!pWeapon || !ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON) || pHandItem && ITEMS_CheckItemTypeId(pHandItem, ITEMTYPE_WEAPON) && SKILLS_GetFlags(pSkill) & 0x2000)
		{
			pWeapon = pHandItem;
		}

		break;

	case 3:
		nFrame = pUnit->dwSeqFrame >> 8;

		if (pUnit->dwFlagEx & UNITFLAGEX_SERVERUNIT)
		{
			v14 = nFrame % 2;
		}
		else
		{
			v14 = nFrame > 5;
		}

		if (nFrame && (!a2 || v14))
		{
			pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_LEFTHAND);
		}
		else
		{
			pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_RIGHTHAND);
		}
		break;

	case 4:
		pItem = NULL;
		break;

	default:
		pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_RIGHTHAND);
		if (!pWeapon || !ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON))
		{
			pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_LEFTHAND);
		}

		break;
	}

	if (!ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON))
	{
		return pItem;
	}

	if (!ITEMS_CanBeEquipped(pWeapon))
	{
		return pItem;
	}

	return pWeapon;
}

//D2Common.0x6FDC0F70 (#10435)
D2UnitStrc* __stdcall UNITS_GetEquippedWeaponFromMonster(D2UnitStrc* pMonster)
{
	D2UnitStrc* pItem = NULL;
	BOOL bIsLeftHandItem = FALSE;
	int nBodyLoc = 0;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pInventory)
	{
		if (INVENTORY_GetEquippedWeapon(pMonster->pInventory, &pItem, &nBodyLoc, &bIsLeftHandItem))
		{
			return pItem;
		}
	}

	return NULL;
}

//D2Common.0x6FDC0FC0 (#10436)
int __stdcall UNITS_GetFrameBonus(D2UnitStrc* pUnit)
{
	static const int gaClassesWeaponFrameBonus_6FDD3078[NUM_WEAPON_CLASSES][NUMBER_OF_PLAYERCLASSES] =
	{
		//AMA,SOR,NEC,PAL,BAR,DRU,ASS
		{   1,  1,  0,  0,  0,  0,  0}, // WEAPONCLASS_NONE
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_BOW
		{   2,  2,  0,  0,  0,  0,  0}, // WEAPONCLASS_1HS
		{   2,  2,  0,  0,  0,  0,  0}, // WEAPONCLASS_1HT
		{   2,  2,  0,  0,  0,  0,  0}, // WEAPONCLASS_STF
		{   2,  2,  0,  0,  0,  0,  0}, // WEAPONCLASS_2HS
		{   2,  2,  0,  0,  0,  0,  0}, // WEAPONCLASS_2HT
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_XBW
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_1JS
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_1JT
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_1SS
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_1ST
		{   0,  0,  0,  0,  0,  0,  0}, // WEAPONCLASS_HT1
		{   0,  0,  0,  0,  0,  0,  0}  // WEAPONCLASS_HT2
	};
	
	int nClassId = -1;
	int nType = UNIT_COUNT;
	int nMode = 0;
	if (pUnit)
	{
		nClassId = pUnit->dwClassId;
		nType = pUnit->dwUnitType;
		nMode = pUnit->dwAnimMode;
	}		

	D2COMMON_11013_ConvertMode(pUnit, &nType, &nClassId, &nMode, __FILE__, __LINE__);

	if (nType != UNIT_PLAYER)
	{
		return 0;
	}


	int nWeaponClass = WEAPONCLASS_NONE;
	switch (nMode)
	{
	case PLRMODE_ATTACK1:
	case PLRMODE_ATTACK2:
		if (D2UnitStrc* pItem = D2Common_10434(pUnit, 1))
		{
			nWeaponClass = ITEMS_GetWeaponClassId(pItem);
		}
		break;
	case PLRMODE_SPECIAL3:
	case PLRMODE_SPECIAL4:
		if (!UNITS_CanDualWield(pUnit))
		{
			return 0;
		}

		if (D2UnitStrc* pItem = D2Common_10434(pUnit, 1))
		{
			nWeaponClass = ITEMS_GetWeaponClassId(pItem);
		}
		break;
	default:
		return 0;
	}

	D2_ASSERT(0 <= nClassId && nClassId < NUMBER_OF_PLAYERCLASSES);
	D2_ASSERT(0 <= nWeaponClass && nWeaponClass < NUM_WEAPON_CLASSES);
	return gaClassesWeaponFrameBonus_6FDD3078[nWeaponClass][nClassId];
}

//D2Common.0x6FDC1120 (#10360)
int __stdcall UNITS_GetMeleeRange(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	BOOL bIsLeftHandItem = FALSE;
	int nWeaponClass = 0;
	int nBodyLoc = 0;

	D2_ASSERT(pUnit);

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		if (INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pUnit, &nBodyLoc, &bIsLeftHandItem))
		{
			return ITEMS_GetMeleeRange(pUnit);
		}		
	}
	else if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
		if (pMonStats2TxtRecord)
		{
			if (pMonStats2TxtRecord->nMeleeRng == 255)
			{
				COMPOSIT_GetWeaponClassId(pUnit, pUnit->pInventory, &nWeaponClass, -1, 1);
				return nWeaponClass == WEAPONCLASS_2HT ? 2 : 0;
			}

			return pMonStats2TxtRecord->nMeleeRng;
		}
	}

	return 0;
}

//D2Common.0x6FDC1230 (#10364)
BOOL __stdcall UNITS_TestCollisionByCoordinates(D2UnitStrc* pUnit, int nX, int nY, int nFlags)
{
	D2RoomStrc* pRoom = NULL;
	D2CoordStrc pCoords = {};

	if (!pUnit)
	{
		return FALSE;
	}

	pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return FALSE;
	}

	UNITS_GetCoords(pUnit, &pCoords);

	return UNITS_TestCollision(pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), nX, nY, 2, pRoom, nFlags);
}

//D2Common.0x6FDC13D0
BOOL __fastcall UNITS_TestCollision(int nX1, int nY1, int nSize1, int nX2, int nY2, int nSize2, D2RoomStrc* pRoom, int nCollisionType)
{
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};
	int nXDiff = 0;
	int nYDiff = 0;

	if (nSize1 >= 3)
	{
		nSize1 = 2;
	}

	if (nSize2 >= 3)
	{
		nSize2 = 2;
	}

	nXDiff = nX2 - nX1;
	if (nXDiff < 0)
	{
		nXDiff = -nXDiff;
	}

	nYDiff = nY2 - nY1;
	if (nYDiff < 0)
	{
		nYDiff = -nYDiff;
	}

	if (nXDiff + nYDiff < nSize1 + nSize2)
	{
		return FALSE;
	}

	if (nSize1 || nSize2)
	{
		if (nXDiff >= nYDiff)
		{
			if (nX2 <= nX1)
			{
				nX1 -= nSize1;
				nX2 += nSize2;
			}
			else
			{
				nX1 += nSize1;
				nX2 -= nSize2;
			}
		}

		if (nYDiff >= nXDiff)
		{
			if (nY2 <= nY1)
			{
				nY1 -= nSize1;
				nY2 += nSize2;
			}
			else
			{
				nY1 += nSize1;
				nY2 -= nSize2;
			}
		}
	}

	pCoords1.nX = nX1;
	pCoords1.nY = nY1;
	pCoords2.nX = nX2;
	pCoords2.nY = nY2;

	return D2Common_11263(pRoom, &pCoords1, &pCoords2, nCollisionType);
}

//D2Common.0x6FDC14C0 (#10362)
BOOL __stdcall UNITS_TestCollisionWithUnit(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int nCollisionType)
{
	D2RoomStrc* pRoom = NULL;
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};

	D2_ASSERT(pUnit1);
	D2_ASSERT(pUnit2);

	pRoom = UNITS_GetRoom(pUnit1);
	if (!pRoom)
	{
		return FALSE;
	}

	UNITS_GetCoords(pUnit1, &pCoords1);
	UNITS_GetCoords(pUnit2, &pCoords2);

	return UNITS_TestCollision(pCoords1.nX, pCoords1.nY, UNITS_GetUnitSizeX(pUnit1), pCoords2.nX, pCoords2.nY, UNITS_GetUnitSizeX(pUnit2), pRoom, nCollisionType);
}

//D2Common.0x6FDC1760
void __fastcall UNITS_ToggleUnitFlag(D2UnitStrc* pUnit, int nFlag, BOOL bSet)
{
	if (pUnit)
	{
		if (bSet)
		{
			pUnit->dwFlags |= nFlag;
		}
		else
		{
			pUnit->dwFlags &= ~nFlag;
		}
	}
}

//D2Common.0x6FDC1790 (#10363)
//TODO: v4 + v26
BOOL __stdcall UNITS_TestCollisionBetweenInteractingUnits(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int nCollisionType)
{
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};
	D2RoomStrc* pRoom = NULL;
	BOOL bResult = FALSE;
	int v4 = 0;
	int v26 = 0;

	if (!pUnit1 || !pUnit2)
	{
		return FALSE;
	}

	pRoom = UNITS_GetRoom(pUnit1);
	if (!pRoom)
	{
		return FALSE;
	}

	UNITS_GetCoords(pUnit1, &pCoords1);
	UNITS_GetCoords(pUnit2, &pCoords2);

	if (UNITS_GetCollisionType(pUnit1) & nCollisionType)
	{
		v4 = D2Common_10223(pUnit1, 0);
	}

	if (UNITS_GetCollisionType(pUnit2) & nCollisionType)
	{
		v26 = D2Common_10223(pUnit2, 0);
	}

	bResult = UNITS_TestCollision(pCoords1.nX, pCoords1.nY, UNITS_GetUnitSizeX(pUnit1), pCoords2.nX, pCoords2.nY, UNITS_GetUnitSizeX(pUnit2), pRoom, nCollisionType);

	if (v4)
	{
		D2Common_10222(pUnit1);
	}

	if (v26)
	{
		D2Common_10222(pUnit2);
	}

	return bResult;
}

//D2Common.0x6FDC1A70 (#10361)
BOOL __stdcall UNITS_IsInMeleeRange(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int nRangeBonus)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nDistance = 0;
	int nClassId = 0;

	if (pUnit1 && pUnit2)
	{
		if (pUnit2->dwUnitType == UNIT_MONSTER)
		{
			nClassId = MONSTERS_GetBaseIdFromMonsterId(pUnit2->dwClassId);

			if (nClassId == MONSTER_TENTACLE1 || nClassId == MONSTER_TENTACLEHEAD1)
			{
				if (UNITS_GetMeleeRange(pUnit1) + 8 > D2Common_10399(pUnit1, pUnit2))
				{
					return TRUE;
				}
			}
		}

		nDistance = D2Common_10399(pUnit1, pUnit2);
		if (nDistance <= 0)
		{
			return 1;
		}

		if (UNITS_GetMeleeRange(pUnit1) + nRangeBonus + 1 >= nDistance)
		{
			return UNITS_TestCollisionWithUnit(pUnit1, pUnit2, 0x804) == 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FDC1B40 (#10318)
BOOL __stdcall UNITS_IsInMovingMode(D2UnitStrc* pUnit)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nMonsterBaseId = 0;
	int nAnimMode = 0;

	if (pUnit)
	{
		if (pUnit->dwUnitType == UNIT_PLAYER)
		{
			if (pUnit->pAnimSeq)
			{
				nAnimMode = pUnit->dwSeqMode;
			}
			else
			{
				nAnimMode = pUnit->dwAnimMode;
			}

			if (nAnimMode >= PLRMODE_WALK)
			{
				if (nAnimMode <= PLRMODE_RUN)
				{
					return TRUE;
				}

				if (nAnimMode == PLRMODE_TWALK)
				{
					return TRUE;
				}
			}
		}
		else if (pUnit->dwUnitType == UNIT_MONSTER)
		{
			switch (pUnit->dwAnimMode)
			{
			case MONMODE_SKILL1:
			case MONMODE_SKILL2:
				nMonsterBaseId = MONSTERS_GetBaseIdFromMonsterId(pUnit->dwClassId);

				if (nMonsterBaseId == MONSTER_VULTURE1)
				{
					return TRUE;
				}

				return FALSE;

			case MONMODE_WALK:
			case MONMODE_RUN:
				return TRUE;

			default:
				return FALSE;
			}
		}
		D2_ASSERT(pUnit->dwUnitType != UNIT_MISSILE);
	}

	return FALSE;
}

//D2Common.0x6FDC1C30 (#10319)
BOOL __stdcall UNITS_IsInMovingModeEx(D2UnitStrc* pUnit)
{
	return UNITS_IsInMovingMode(pUnit) != 0;
}

//D2Common.0x6FDC1C50 (#10365)
int __fastcall UNITS_GetHitClass(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2UnitStrc* pItem = NULL;

	if (pUnit)
	{
		if (pUnit->dwUnitType == UNIT_PLAYER)
		{
			pItem = D2Common_10434(pUnit, 0);
			if (pItem)
			{
				return ITEMS_GetHitClassFromItem(pItem);
			}

			return 1;
		}
		else if (pUnit->dwUnitType == UNIT_MONSTER)
		{
			pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
			if (pMonStats2TxtRecord)
			{
				return pMonStats2TxtRecord->nHitClass;
			}
		}
	}

	return 0;
}

//D2Common.0x6FDC1CE0 (#10366)
int __fastcall UNITS_GetWeaponClass(D2UnitStrc* pUnit)
{
	D2UnitStrc* pItem = NULL;

	pItem = D2Common_10434(pUnit, 1);
	if (pItem)
	{
		return ITEMS_GetWeaponClassId(pItem);
	}

	return 0;
}

//D2Common.0x6FDC1D00 (#10438)
unsigned int __stdcall UNITS_GetHealingCost(D2UnitStrc* pUnit)
{
	int nShiftedLifeDifference = 0;
	int nShiftedManaDifference = 0;
	int nMaxLife = 0;
	int nMaxMana = 0;
	int nLife = 0;
	int nMana = 0;

	nMaxLife = STATLIST_GetMaxLifeFromUnit(pUnit);
	nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
	nLife = STATLIST_GetUnitStatUnsigned(pUnit, STAT_HITPOINTS, 0);
	nMana = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MANA, 0);

	if (nMaxLife > nLife)
	{
		nShiftedLifeDifference = (nMaxLife - nLife) >> 8;
	}

	if (nMaxMana > nMana)
	{
		nShiftedManaDifference = (nMaxMana - nMana) >> 8;
	}

	if (nShiftedLifeDifference || nShiftedManaDifference)
	{
		return (unsigned int)((nShiftedLifeDifference + nShiftedManaDifference) * STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0)) >> 2;
	}

	return 0;
}

//D2Common.0x6FDC1D90 (#10439)
unsigned int __stdcall UNITS_GetInventoryGoldLimit(D2UnitStrc* pUnit)
{
	return 10000 * STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
}

//D2Common.0x6FDC1DB0 (#10440)
//TODO: v5
void __stdcall UNITS_MergeDualWieldWeaponStatLists(D2UnitStrc* pUnit, int a2)
{
	D2UnitStrc* v5 = NULL;
	D2UnitStrc* pPrimaryWeapon = NULL;
	D2UnitStrc* pSecondaryWeapon = NULL;

	if (!UNITS_CanDualWield(pUnit))
	{
		return;
	}

	v5 = D2Common_10434(pUnit, a2);
	pPrimaryWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
	if (v5)
	{
		if (v5 == pPrimaryWeapon)
		{
			pSecondaryWeapon = INVENTORY_GetSecondaryWeapon(pUnit->pInventory);
			if (pPrimaryWeapon && STATLIST_GetOwner(pPrimaryWeapon, 0))
			{
				STATLIST_MergeStatLists(pUnit, pPrimaryWeapon, 1);
			}

			if (pSecondaryWeapon && STATLIST_GetOwner(pSecondaryWeapon, 0))
			{
				STATLIST_MergeStatLists(pUnit, pSecondaryWeapon, 0);
			}
		}
		else
		{
			if (STATLIST_GetOwner(v5, 0))
			{
				STATLIST_MergeStatLists(pUnit, v5, 1);
			}

			if (pPrimaryWeapon && STATLIST_GetOwner(pPrimaryWeapon, 0))
			{
				STATLIST_MergeStatLists(pUnit, pPrimaryWeapon, 0);
			}
		}
	}
	else
	{
		if (pPrimaryWeapon && STATLIST_GetOwner(pPrimaryWeapon, 0))
		{
			STATLIST_MergeStatLists(pUnit, pPrimaryWeapon, 1);
		}
	}
}

//D2Common.0x6FDC1EE0
D2MonStats2Txt* __fastcall UNITS_GetMonStats2TxtRecord(int nRecordId)
{
	if (nRecordId >= 0 && nRecordId < sgptDataTables->nMonStats2TxtRecordCount)
	{
		return &sgptDataTables->pMonStats2Txt[nRecordId];
	}

	return NULL;
}

//D2Common.0x6FDC1F10 (#10442)
uint8_t __stdcall UNITS_GetItemComponentId(D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	int nBodyloc = 0;
	int nClassId = 0;

	nBodyloc = ITEMS_GetBodyLocation(pItem);
	if (nBodyloc != BODYLOC_LARM && nBodyloc != BODYLOC_RARM)
	{
		return ITEMS_GetComponent(pItem);
	}

	pRightHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
	if (!ITEMS_CanBeEquipped(pRightHandItem))
	{
		pRightHandItem = NULL;
	}

	pLeftHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
	if (!ITEMS_CanBeEquipped(pLeftHandItem))
	{
		pLeftHandItem = NULL;
	}

	if (!pRightHandItem || !pLeftHandItem || ITEMS_CheckWeaponIfTwoHanded(pRightHandItem) || ITEMS_CheckWeaponIfTwoHanded(pLeftHandItem))
	{
		return ITEMS_GetComponent(pItem);
	}

	nClassId = pUnit->dwClassId;
	if (pUnit->dwUnitType != UNIT_PLAYER)
	{
		if (pUnit->dwUnitType != UNIT_MONSTER || nClassId < MONSTER_SHADOWWARRIOR || nClassId > MONSTER_SHADOWMASTER)
		{
			return ITEMS_GetComponent(pItem);
		}
	}
	else
	{
		if (nClassId != PCLASS_BARBARIAN && nClassId != PCLASS_ASSASSIN)
		{
			return ITEMS_GetComponent(pItem);
		}
	}

	if (pItem == INVENTORY_GetLeftHandWeapon(pUnit->pInventory))
	{
		return ITEMS_GetComponent(pItem);
	}

	return 6;
}

//D2Common.0x6FDC1FE0
D2MonStats2Txt* __fastcall UNITS_GetMonStats2TxtRecordFromMonsterId(int nMonsterId)
{
	int nMonStatsEx = DATATBLS_GetMonStatsTxtRecord(nMonsterId)->wMonStatsEx;

	if (nMonStatsEx >= 0 && nMonStatsEx < sgptDataTables->nMonStats2TxtRecordCount)
	{
		return &sgptDataTables->pMonStats2Txt[nMonStatsEx];
	}

	return NULL;
}

//D2Common.0x6FDC2030 (#10443)
void __stdcall UNITS_InitRightSkill(D2UnitStrc* pUnit)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		pSkill = SKILLS_GetRightSkillFromSkillList(pUnit->pSkills);

		pUnit->pPlayerData->nRightSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		pUnit->pPlayerData->nRightSkillFlags = SKILLS_GetOwnerGUIDFromSkill(pSkill);
	}
}

//D2Common.0x6FDC20A0 (#10444)
void __stdcall UNITS_InitLeftSkill(D2UnitStrc* pUnit)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		pSkill = SKILLS_GetLeftSkillFromSkillList(pUnit->pSkills);

		pUnit->pPlayerData->nLeftSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		pUnit->pPlayerData->nLeftSkillFlags = SKILLS_GetOwnerGUIDFromSkill(pSkill);
	}
}

//D2Common.0x6FDC2110 (#10445)
void __stdcall UNITS_InitSwitchRightSkill(D2UnitStrc* pUnit)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		pSkill = SKILLS_GetRightSkillFromSkillList(pUnit->pSkills);

		pUnit->pPlayerData->nSwitchRightSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		pUnit->pPlayerData->nSwitchRightSkillFlags = SKILLS_GetOwnerGUIDFromSkill(pSkill);
	}
}

//D2Common.0x6FDC2180 (#10446)
void __stdcall UNITS_InitSwitchLeftSkill(D2UnitStrc* pUnit)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		pSkill = SKILLS_GetLeftSkillFromSkillList(pUnit->pSkills);

		pUnit->pPlayerData->nSwitchLeftSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		pUnit->pPlayerData->nSwitchLeftSkillFlags = SKILLS_GetOwnerGUIDFromSkill(pSkill);
	}
}

//D2Common.0x6FDC21F0 (#10447)
void __stdcall UNITS_GetRightSkillData(D2UnitStrc* pUnit, int* pRightSkillId, int* pRightSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		*pRightSkillId = pUnit->pPlayerData->nRightSkillId;
		*pRightSkillFlags = pUnit->pPlayerData->nRightSkillFlags;
	}
	else
	{
		*pRightSkillId = 0;
		*pRightSkillFlags = -1;
	}
}

//D2Common.0x6FDC2250 (#10448)
void __stdcall UNITS_GetLeftSkillData(D2UnitStrc* pUnit, int* pLeftSkillId, int* pLeftSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		*pLeftSkillId = pUnit->pPlayerData->nLeftSkillId;
		*pLeftSkillFlags = pUnit->pPlayerData->nLeftSkillFlags;
	}
	else
	{
		*pLeftSkillId = 0;
		*pLeftSkillFlags = -1;
	}
}

//D2Common.0x6FDC22B0 (#10449)
void __stdcall UNITS_GetSwitchRightSkillDataResetRightSkill(D2UnitStrc* pUnit, int* pSwitchRightSkillId, int* pSwitchRightSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		*pSwitchRightSkillId = pUnit->pPlayerData->nSwitchRightSkillId;
		*pSwitchRightSkillFlags = pUnit->pPlayerData->nSwitchRightSkillFlags;

		pUnit->pPlayerData->nRightSkillId = 0;
		pUnit->pPlayerData->nRightSkillFlags = -1;
		pUnit->pPlayerData->nWeaponGUID = -1;
		pUnit->pPlayerData->unk0x94[0] = 0;
	}
	else
	{
		*pSwitchRightSkillId = 0;
		*pSwitchRightSkillFlags = -1;
	}
}

//D2Common.0x6FDC2330 (#10450)
void __stdcall UNITS_GetSwitchLeftSkillDataResetLeftSkill(D2UnitStrc* pUnit, int* pSwitchLeftSkillId, int* pSwitchLeftSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		*pSwitchLeftSkillId = pUnit->pPlayerData->nSwitchLeftSkillId;
		*pSwitchLeftSkillFlags = pUnit->pPlayerData->nSwitchLeftSkillFlags;

		pUnit->pPlayerData->nLeftSkillId = 0;
		pUnit->pPlayerData->nLeftSkillFlags = -1;
		pUnit->pPlayerData->nWeaponGUID = -1;
		pUnit->pPlayerData->unk0x94[0] = 0;
	}
	else
	{
		*pSwitchLeftSkillId = 0;
		*pSwitchLeftSkillFlags = -1;
	}
}

//D2Common.0x6FDC23B0 (#10451)
void __stdcall UNITS_GetSwitchLeftSkillData(D2UnitStrc* pUnit, int* pSwitchLeftSkillId, int* pSwitchLeftSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		*pSwitchLeftSkillId = pUnit->pPlayerData->nSwitchLeftSkillId;
		*pSwitchLeftSkillFlags = pUnit->pPlayerData->nSwitchLeftSkillFlags;
	}
	else
	{
		*pSwitchLeftSkillId = 0;
		*pSwitchLeftSkillFlags = -1;
	}
}

//D2Common.0x6FDC2420 (#10452)
void __stdcall UNITS_GetSwitchRightSkillData(D2UnitStrc* pUnit, int* pSwitchRightSkillId, int* pSwitchRightSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		*pSwitchRightSkillId = pUnit->pPlayerData->nSwitchRightSkillId;
		*pSwitchRightSkillFlags = pUnit->pPlayerData->nSwitchRightSkillFlags;
	}
	else
	{
		*pSwitchRightSkillId = 0;
		*pSwitchRightSkillFlags = -1;
	}
}

//D2Common.0x6FDC2490 (#10453)
void __stdcall UNITS_SetSwitchLeftSkill(D2UnitStrc* pUnit, int nSwitchLeftSkillId, int nSwitchLeftSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		pUnit->pPlayerData->nSwitchLeftSkillId = nSwitchLeftSkillId;
		pUnit->pPlayerData->nSwitchLeftSkillFlags = nSwitchLeftSkillFlags;
	}
}

//D2Common.0x6FDC24E0 (#10454)
void __stdcall UNITS_SetSwitchRightSkill(D2UnitStrc* pUnit, int nSwitchRightSkillId, int nSwitchRightSkillFlags)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		pUnit->pPlayerData->nSwitchRightSkillId = nSwitchRightSkillId;
		pUnit->pPlayerData->nSwitchRightSkillFlags = nSwitchRightSkillFlags;
	}
}

//D2Common.0x6FDC2530 (#10455)
void __stdcall UNITS_SetWeaponGUID(D2UnitStrc* pUnit, D2UnitStrc* pWeapon)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		D2_ASSERT(pUnit->pPlayerData);

		if (pWeapon)
		{
			D2_ASSERT(pWeapon->dwUnitType == UNIT_ITEM);

			pUnit->pPlayerData->nWeaponGUID = pWeapon->dwUnitId;
		}
		else
		{
			pUnit->pPlayerData->nWeaponGUID = 0;
		}
	}
}

//D2Common.0x6FDC25B0 (#10456)
int __stdcall UNITS_GetWeaponGUID(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);
	D2_ASSERT(pUnit->pPlayerData);

	return pUnit->pPlayerData->nWeaponGUID;
}

//D2Common.0x6FDC2630 (#10339)
unsigned int __stdcall UNITS_GetStashGoldLimit(D2UnitStrc* pUnit)
{
	int nMultiplier = 0;
	int nLevel = 0;

	nLevel = STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
	if (nLevel <= 30)
	{
		nMultiplier = nLevel / 10 + 1;
	}
	else
	{
		nMultiplier = nLevel / 2 + 1;
	}

	return 50000 * nMultiplier;
}

//D2Common.0x6FDC2680 (#10317)
BOOL __fastcall UNITS_CanSwitchAI(int nMonsterId)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	if (pMonStatsTxtRecord)
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecord(pMonStatsTxtRecord->wMonStatsEx);
		if (pMonStats2TxtRecord && (pMonStats2TxtRecord->dwModeFlags & MONSTATS2MODEFLAG_WL))
		{
			if (!(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_BOSS]))
			{
				return (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_SWITCHAI]) != 0;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FDC2720 (#10458)
void __fastcall UNITS_SetTimerArg(D2UnitStrc* pUnit, D2TimerArgStrc* pTimerArg)
{
	D2_ASSERT(pUnit);

	pUnit->pTimerParams = pTimerArg;
}

//D2Common.0x6FDC2750 (#10459)
D2TimerArgStrc* __fastcall UNITS_GetTimerArg(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	return (D2TimerArgStrc*)pUnit->pTimerParams;
}

//D2Common.0x6FDC2780 (#10460)
void __stdcall UNITS_AllocStaticPath(D2UnitStrc* pUnit)
{
	if (!pUnit->pStaticPath)
	{
		pUnit->pStaticPath = (D2StaticPathStrc*)FOG_AllocServerMemory(pUnit->pMemoryPool, sizeof(D2StaticPathStrc), __FILE__, __LINE__, 0);
		memset(pUnit->pStaticPath, 0x00, sizeof(D2StaticPathStrc));
	}
}

//D2Common.0x6FDC27C0 (#10461)
void __stdcall UNITS_FreeStaticPath(D2UnitStrc* pUnit)
{
	if (pUnit->pStaticPath)
	{
		FOG_FreeServerMemory(pUnit->pMemoryPool, pUnit->pStaticPath, __FILE__, __LINE__, 0);
		pUnit->pStaticPath = NULL;
	}
}

//D2Common.0x6FDC27F0 (#10462)
BOOL __stdcall UNITS_CanDualWield(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		switch (pUnit->dwUnitType)
		{
		case UNIT_PLAYER:
			if (pUnit->dwClassId == PCLASS_BARBARIAN || pUnit->dwClassId == PCLASS_ASSASSIN)
			{
				return TRUE;
			}
			break;

		case UNIT_MONSTER:
			if (pUnit->dwClassId == MONSTER_SHADOWWARRIOR || pUnit->dwClassId == MONSTER_SHADOWMASTER)
			{
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

//D2Common.0x6FDC2860 (#11238)
BOOL __stdcall UNITS_IsCorpseUseable(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->dwAnimMode == MONMODE_DEAD)
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
		if (pMonStats2TxtRecord)
		{
			if (!(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_DEADCOL]))
			{
				if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_CORPSESEL])
				{
					return STATES_CheckStateMaskUdeadOnUnit(pUnit) == 0;
				}
			}
		}
	}

	return 0;
}

//D2Common.0x6FDC2910 (#11307)
BOOL __stdcall UNITS_IsObjectInInteractRange(D2UnitStrc* pUnit, D2UnitStrc* pObject)
{
	int nSizeX2 = 0;
	int nSizeY1 = 0;
	int nSizeY2 = 0;
	int nX1 = 0;
	int nY1 = 0;
	int nX2 = 0;
	int nY2 = 0;
	int nX = 0;
	int nY = 0;

	if (!pObject || pObject->dwUnitType != UNIT_OBJECT)
	{
		return FALSE;
	}

	if (!D2Common_10399(pUnit, pObject))
	{
		return TRUE;
	}

	nX1 = UNITS_GetXPosition(pUnit);
	nY1 = UNITS_GetYPosition(pUnit);

	nX2 = UNITS_GetXPosition(pObject);
	nY2 = UNITS_GetYPosition(pObject);

	nSizeX2 = UNITS_GetUnitSizeX(pObject);
	nSizeY2 = UNITS_GetUnitSizeY(pObject);

	nX = nX2 - nSizeX2 / 2;
	nY = nY2 - nSizeY2 / 2;

	if (nSizeX2 >= 1 && nSizeY2 >= 1)
	{
		if (nX1 >= nX - 2 && nX1 <= nSizeX2 + nX + 2 && nY1 >= nY - 2 && nY1 <= nSizeY2 + nY + 2)
		{
			nSizeY1 = UNITS_GetUnitSizeY(pUnit);

			if (nSizeY1 > 2)
			{
				return TRUE;
			}

			if (nY1 >= nY - 1)
			{
				if (nY1 <= nSizeY2 + nY + 1)
				{
					return TRUE;
				}

				if (nX1 >= nX - 1)
				{
					if (nX1 > nSizeX2 + nX + 1)
					{
						return FALSE;
					}

					return TRUE;
				}
			}
			else
			{
				if (nX1 >= nX - 1 && nX1 <= nSizeX2 + nX + 1)
				{
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	if (nX1 < nX - 1 || nX1 > nX + 1 || nY1 < nY - 1 || nY1 > nY + 1)
	{
		return FALSE;
	}

	return TRUE;
}

//D2Common.0x6FDC2C80
D2CharStatsTxt* __fastcall UNITS_GetCharStatsTxtRecord(int nRecordId)
{
	if (nRecordId >= 0 && nRecordId < sgptDataTables->nCharStatsTxtRecordCount)
	{
		return &sgptDataTables->pCharStatsTxt[nRecordId];
	}

	return NULL;
}

//D2Common.0x6FDC2CB0 (#10399)
//TODO: Find a name
int __stdcall D2Common_10399(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	static const int dword_6FDD3200[] =
	{
		-1, -1, -1, 0, 2, 4, 6, 8,
		-1, -1, 0, 1, 2, 4, 6, 8,
		-1, 0, 0, 2, 3, 5, 7, 8,
		0, 1, 2, 2, 4, 5, 7, 8,
		2, 2, 3, 4, 5, 6, 7, 9,
		4, 4, 5, 5, 6, 7, 8, 9,
		6, 6, 7, 7, 7, 8, 10, 10,
		8, 8, 8, 8, 9, 9, 10, 11,
	};

	int nAbsoluteXDiff = 0;
	int nAbsoluteYDiff = 0;
	int nSizeDiff = 0;
	int nSizeX1 = 0;
	int nSizeX2 = 0;
	int nDist = 0;
	int nX = 0;
	int nY = 0;

	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};

	UNITS_GetCoords(pUnit1, &pCoords1);
	UNITS_GetCoords(pUnit2, &pCoords2);

	nAbsoluteXDiff = pCoords2.nX - pCoords1.nX;
	if (nAbsoluteXDiff < 0)
	{
		nAbsoluteXDiff = -nAbsoluteXDiff;
	}

	nAbsoluteYDiff = pCoords2.nY - pCoords1.nY;
	if (nAbsoluteYDiff < 0)
	{
		nAbsoluteYDiff = -nAbsoluteYDiff;
	}

	nSizeX1 = UNITS_GetUnitSizeX(pUnit1);
	nSizeX2 = UNITS_GetUnitSizeX(pUnit2);

	if (nAbsoluteXDiff >= 8 || nAbsoluteYDiff >= 8 || nSizeX1 >= 4 || nSizeX2 >= 4)
	{
		nSizeDiff = ((unsigned int)nSizeX2 >> 1) + ((unsigned int)nSizeX1 >> 1);
		if (nSizeDiff < 0)
		{
			nSizeDiff = -nSizeDiff;
		}

		nX = nAbsoluteXDiff - nSizeDiff;
		nY = nAbsoluteYDiff - nSizeDiff;
		if (nX < 0)
		{
			nX = 0;
		}

		if (nY < 0)
		{
			nY = 0;
		}

		nDist = nY + 2 * nX;
		if (nX <= nY)
		{
			nDist = nX + 2 * nY;
		}
	}
	else
	{
		nDist = dword_6FDD3200[nAbsoluteXDiff + 8 * nAbsoluteYDiff];
		if (nDist >= 0)
		{
			if (nSizeX1 == 3 || nSizeX2 == 3)
			{
				--nDist;
			}

			if (nDist < 0)
			{
				nDist = 0;
			}

			if (nSizeX1 <= 1 || nSizeX2 <= 1)
			{
				++nDist;
			}
		}
		else
		{
			return 0;
		}
	}

	return nDist;
}

//D2Common.0x6FDC2E40 (#10397)
int __stdcall UNITS_GetDistanceToOtherUnit(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};
	int nDistanceX = 0;
	int nDistanceY = 0;
	int nSize = 0;
	int nX = 0;
	int nY = 0;

	UNITS_GetCoords(pUnit1, &pCoords1);
	UNITS_GetCoords(pUnit2, &pCoords2);

	nDistanceX = pCoords2.nX - pCoords1.nX;
	if (nDistanceX < 0)
	{
		nDistanceX = -nDistanceX;
	}

	nDistanceY = pCoords2.nY - pCoords1.nY;
	if (nDistanceY < 0)
	{
		nDistanceY = -nDistanceY;
	}

	nSize = UNITS_GetUnitSizeX(pUnit1) / -2 - UNITS_GetUnitSizeX(pUnit2) / 2;

	nX = nSize + nDistanceX;
	nY = nSize + nDistanceY;

	if (nX < 0)
	{
		nX = 0;
	}

	if (nY < 0)
	{
		nY = 0;
	}

	if (nX <= nY)
	{
		return (nX + 2 * nY) / 2;
	}
	else
	{
		return (nY + 2 * nX) / 2;
	}
}

//D2Common.0x6FDC2F50 (#10398)
int __stdcall UNITS_GetDistanceToCoordinates(D2UnitStrc* pUnit, int nX, int nY)
{
	D2CoordStrc pCoords = {};
	int nDistanceX = 0;
	int nDistanceY = 0;

	UNITS_GetCoords(pUnit, &pCoords);

	nDistanceX = nX - pCoords.nX;
	if (nDistanceX < 0)
	{
		nDistanceX = -nDistanceX;
	}

	nDistanceY = nY - pCoords.nY;
	if (nDistanceY < 0)
	{
		nDistanceY = -nDistanceY;
	}

	if (nDistanceX < 0)
	{
		nDistanceX = 0;
	}
	
	if (nDistanceY < 0)
	{
		nDistanceY = 0;
	}

	if (nDistanceX <= nDistanceY)
	{
		return (nDistanceX + 2 * nDistanceY) / 2;
	}
	else
	{
		return (nDistanceY + 2 * nDistanceX) / 2;
	}
}

//D2Common.0x6FDC2FF0 (#10400)
BOOL __stdcall UNITS_IsInRange(D2UnitStrc* pUnit, D2CoordStrc* pCoord, int nDistance)
{
	D2CoordStrc pCoords = {};
	int nDistanceX = 0;
	int nDistanceY = 0;

	UNITS_GetCoords(pUnit, &pCoords);

	nDistanceX = pCoord->nX - pCoords.nX;
	if (nDistanceX < 0)
	{
		nDistanceX = -nDistanceX;
	}

	nDistanceY = pCoord->nY - pCoords.nY;
	if (nDistanceY < 0)
	{
		nDistanceY = -nDistanceY;
	}

	return nDistanceX * nDistanceX + nDistanceY * nDistanceY <= nDistance * nDistance;
}

//Used in D2Common.#10406 and D2Common.#10407
//TODO: Find a name
D2UnitStrc* __stdcall D2Common_10407_Impl(D2RoomStrc* pRoom, int nX, int nY, int(__fastcall* pCallback)(D2UnitStrc*, D2UnitStrc*), D2UnitStrc* a5, int a6, D2UnitStrc* a7)
{
	D2DrlgCoordsStrc pDrlgCoords = {};
	D2RoomStrc** ppRoomList = NULL;
	D2UnitStrc* pUnit = NULL;
	int nNumRooms = 0;
	int nAbsDiffX = 0;
	int nAbsDiffY = 0;
	int nUnitX = 0;
	int nUnitY = 0;
	int nSize = 0;

	if (!pRoom || a6 <= 0 || a6 >= 4)
	{
		return NULL;
	}

	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	if (nNumRooms <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < nNumRooms; ++i)
	{
		DUNGEON_GetRoomCoordinates(ppRoomList[i], &pDrlgCoords);

		if (nX + 2 >= pDrlgCoords.dwSubtilesLeft || nX - 2 <= pDrlgCoords.dwSubtilesLeft + pDrlgCoords.dwSubtilesWidth)
		{
			if (nY + 2 >= pDrlgCoords.dwSubtilesTop || nY - 2 <= pDrlgCoords.dwSubtilesTop + pDrlgCoords.dwSubtilesHeight)
			{
				pUnit = ppRoomList[i]->pUnitFirst;
				while (pUnit)
				{
					if ((pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwAnimMode != PLRMODE_DEATH && pUnit->dwAnimMode != PLRMODE_DEAD)
						|| (pUnit->dwUnitType == UNIT_MONSTER && pUnit->dwAnimMode != MONMODE_DEATH && pUnit->dwAnimMode != MONMODE_DEAD)
						|| pUnit->dwUnitType == UNIT_MISSILE)
					{
						nSize = UNITS_GetUnitSizeX(pUnit);

						if (nSize > 0)
						{
							if (nSize > 3)
							{
								nSize = 3;
							}

							nUnitX = UNITS_GetXPosition(pUnit);
							nUnitY = UNITS_GetYPosition(pUnit);

							switch (nSize + 3 * (a6 - 1))
							{
							case 1:
								if (nX == nUnitX && nY == nUnitY)
								{
									if (pUnit != a7 && pCallback(pUnit, a5))
									{
										return pUnit;
									}
								}
								break;

							case 2:
							case 4:
								nAbsDiffX = nX - nUnitX;
								if (nAbsDiffX < 0)
								{
									nAbsDiffX = -nAbsDiffX;
								}

								nAbsDiffY = nY - nUnitY;
								if (nY - nUnitY < 0)
								{
									nAbsDiffY = -nAbsDiffY;
								}

								if (nAbsDiffX + nAbsDiffY <= 1)
								{
									if (pUnit != a7 && pCallback(pUnit, a5))
									{
										return pUnit;
									}
								}
								break;

							case 5:
								nAbsDiffX = nX - nUnitX;
								if (nAbsDiffX < 0)
								{
									nAbsDiffX = -nAbsDiffX;
								}

								nAbsDiffY = nY - nUnitY;
								if (nAbsDiffY < 0)
								{
									nAbsDiffY = -nAbsDiffY;
								}

								if (nAbsDiffX + nAbsDiffY <= 2)
								{
									if (pUnit != a7 && pCallback(pUnit, a5))
									{
										return pUnit;
									}
								}
								break;

							case 3:
							case 7:
								if (nX >= nUnitX - 1 && nX <= nUnitX + 1 && nY >= nUnitY - 1 && nY <= nUnitY + 1)
								{
									if (pUnit != a7 && pCallback(pUnit, a5))
									{
										return pUnit;
									}
								}
								break;

							case 6:
							case 8:
								nAbsDiffX = nX - nUnitX;
								if (nAbsDiffX < 0)
								{
									nAbsDiffX = -nAbsDiffX;
								}

								nAbsDiffY = nY - nUnitY;
								if (nAbsDiffY < 0)
								{
									nAbsDiffY = -nAbsDiffY;
								}

								if ((nAbsDiffX <= 2 && nAbsDiffY <= 1) || (nAbsDiffY <= 2 && nAbsDiffX <= 1))
								{
									if (pUnit != a7 && pCallback(pUnit, a5))
									{
										return pUnit;
									}
								}
								break;

							case 9:
								if (nX >= nUnitX - 2 && nX <= nUnitX + 2 && nY >= nUnitY - 2 && nY <= nUnitY + 2)
								{
									if (pUnit != a7 && pCallback(pUnit, a5))
									{
										return pUnit;
									}
								}
								break;

							default:
								break;
							}
						}
					}

					pUnit = pUnit->pRoomNext;
				}
			}
		}
	}

	return NULL;
}

//D2Common.0x6FDC3090 (#10406)
//TODO: Find a name
D2UnitStrc* __stdcall D2Common_10406(D2UnitStrc* pUnit, int (__fastcall* pCallback)(D2UnitStrc*, D2UnitStrc*), D2UnitStrc* a3)
{
	return D2Common_10407_Impl(UNITS_GetRoom(pUnit), UNITS_GetXPosition(pUnit), UNITS_GetYPosition(pUnit), pCallback, a3, UNITS_GetUnitSizeX(pUnit), pUnit);
}

//D2Common.0x6FDC33C0 (#10407)
//TODO: Find a name
D2UnitStrc* __stdcall D2Common_10407(D2RoomStrc* pRoom, int nX, int nY, int (__fastcall* pCallback)(D2UnitStrc*, D2UnitStrc*), D2UnitStrc* a5, int a6)
{
	return D2Common_10407_Impl(pRoom, nX, nY, pCallback, a5, a6, NULL);
}

//D2Common.0x6FDC3680 (#10419)
void __fastcall UNITS_SetInteractData(D2UnitStrc* pUnit, int nSkillId, int nUnitType, int nUnitGUID)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->pPlayerData)
	{
		pUnit->pPlayerData->unk0xA0[44] = 1;
		pUnit->pPlayerData->unk0xA0[45] = nSkillId;
		pUnit->pPlayerData->unk0xA0[46] = nUnitType;
		pUnit->pPlayerData->unk0xA0[47] = nUnitGUID;
	}
}
