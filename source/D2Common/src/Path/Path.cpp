#include "Path/Path.h"

#include "D2Collision.h"
#include "D2DataTbls.h"
#include "D2Dungeon.h"
#include "D2Inventory.h"
#include <D2BitManip.h>
#include "D2Monsters.h"
#include "Path/Path_IDAStar.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include <DataTbls/MonsterIds.h>
#include <D2Math.h>
#include "Path/PathMisc.h"

static_assert(sizeof(D2DynamicPathStrc) == 512, "D2DynamicPathStrc size must match 1.10f");

static const D2C_CollisionPattern gaCollisionPatternsFromSize_6FDD1DE4[COLLISION_UNIT_SIZE_COUNT] =
{
	COLLISION_PATTERN_NONE,
	COLLISION_PATTERN_SMALL_UNIT_PRESENCE,
	COLLISION_PATTERN_SMALL_UNIT_PRESENCE,
	COLLISION_PATTERN_BIG_UNIT_PRESENCE
};

static const int dword_6FDD1E98[] =
{
	0, 1, 1, 1, 1, -1, -1, -1, -1, 0, 1, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, 0, 0, 0, 0, 0, 1, 1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0
};

static const uint32_t dword_6FDD1F88[PATH_NB_DIRECTIONS] =
{
	0x00000000, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 
	0x00000001, 0x00000004, 0x00000004, 0x00000004, 0x00000004, 0x00000004, 0x00000004, 0x00000004, 
	0x00000004, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
	0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 
	0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 
	0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 
	0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC ,0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

static const int gaPathTypeFlags_6FDD2088[] =
{
	0x21900,
	0x1900,
	0,
	0,
	0x60000,
	0,
	0,
	0x21900,
	0x1E600,
	0x1E800,
	0x60000,
	0x1E604,
	0,
	0x100,
	0x60000,
	0x800,
	0,
	0,
};

static const int32_t gaOffsetForPathType_6FDD20D0[] =
{
	{ 0},
	{ 0},
	{ 0},
	{ 0},
	{ 0},
	{ 2},
	{-2},
	{ 0},
	{ 0},
	{ 0},
	{ 0},
	{ 0},
	{-4},
	{ 0},
	{ 0},
	{ 0},
	{ 0},
	{ 0},
};
static_assert(ARRAY_SIZE(gaOffsetForPathType_6FDD20D0) == PATHTYPE_COUNT, "This array must have PATHTYPE_COUNT entries");

typedef int(__fastcall* PathFunctionType)(D2PathInfoStrc*);

//D2Common.0x6FDD1F40
static const PathFunctionType scpfnPathFunction[] = {
	sub_6FDA7970
	,sub_6FDA69E0
	,sub_6FDAA9F0
	,sub_6FDAB0B0
	,nullptr
	,sub_6FDAA9F0
	,sub_6FDAA9F0
	,sub_6FDAB130
	,sub_6FDAB270
	,sub_6FDAB1E0
	,nullptr
	,sub_6FDAB240
	,sub_6FDAB0C0
	,sub_6FDAA9F0
	,nullptr
	,sub_6FDAC270
	,sub_6FDA7970
	,nullptr
};
static_assert(ARRAY_SIZE(scpfnPathFunction) == PATHTYPE_COUNT, "This array must have PATHTYPE_COUNT entries");

void PATH_UpdateClientCoords(D2DynamicPathStrc* pDynamicPath)
{
	// To game "pixels"
	int nX = pDynamicPath->tGameCoords.dwPrecisionX >> 11;
	int nY = pDynamicPath->tGameCoords.dwPrecisionY >> 11;

	DUNGEON_GameToClientCoords(&nX, &nY);

	pDynamicPath->dwClientCoordX = nX;
	pDynamicPath->dwClientCoordY = nY;
}

//D2Common.0x6FDA8220
void __fastcall sub_6FDA8220(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->dwFlags &= (~PATH_UNKNOWN_FLAG_0x00001);

	if (pDynamicPath->pRoom)
	{
		for (int i = 0; i < pDynamicPath->dwPathPoints; ++i)
		{
			if (pDynamicPath->PathPoints[i].X < pDynamicPath->pRoom->nSubtileX)
			{
				pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
				break;
			}

			if (pDynamicPath->PathPoints[i].X >= pDynamicPath->pRoom->nSubtileX + pDynamicPath->pRoom->nSubtileWidth)
			{
				pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
				break;
			}

			if (pDynamicPath->PathPoints[i].Y < pDynamicPath->pRoom->nSubtileY)
			{
				pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
				break;
			}

			if (pDynamicPath->PathPoints[i].Y >= pDynamicPath->pRoom->nSubtileY + pDynamicPath->pRoom->nSubtileHeight)
			{
				pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
				break;
			}
		}
	}
}

//D2Common.0x6FDA82A0 (#10141)
void __stdcall PATH_GetClientCoordsVelocity(D2UnitStrc* pUnit, int* pX, int* pY)
{
	if (pUnit && UNITS_IsInMovingMode(pUnit) && pUnit->pDynamicPath->dwPathPoints)
	{
		*pX = pUnit->pDynamicPath->tVelocityVector.nX;
		*pY = pUnit->pDynamicPath->tVelocityVector.nY;
		*pX >>= 11;
		*pY >>= 11;
		DUNGEON_GameToClientCoords(pX, pY);
	}
	else
	{
		*pX = 0;
		*pY = 0;
	}
}

//D2Common.0x6FDA8320 (#10222)
void __stdcall PATH_AddCollisionFootprintForUnit(D2UnitStrc* pUnit)
{
	D2CoordStrc pCoords = {};

	UNITS_GetCoords(pUnit, &pCoords);

	switch (pUnit->dwUnitType)
	{
	case UNIT_MONSTER:
		COLLISION_SetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
		break;
	case UNIT_OBJECT:
		COLLISION_SetMaskWithSizeXY(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetUnitSizeY(pUnit), UNITS_GetCollisionType(pUnit));
		break;
	default:
		COLLISION_SetMaskWithSize(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetCollisionType(pUnit));
		break;
	}
}

//D2Common.0x6FDA8450 (#10223)
BOOL __stdcall PATH_RemoveCollisionFootprintForUnit(D2UnitStrc* pUnit, BOOL bForce)
{
	D2CoordStrc pCoords = {};
	UNITS_GetCoords(pUnit, &pCoords);

	switch (pUnit->dwUnitType)
	{
	case UNIT_PLAYER:
		if (bForce || (pUnit->dwAnimMode != PLRMODE_DEAD && pUnit->dwAnimMode != PLRMODE_DEATH))
		{
			COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
			return TRUE;
		}
		break;
	case UNIT_MONSTER:
		if (bForce || (pUnit->dwAnimMode != MONMODE_DEAD && pUnit->dwAnimMode != MONMODE_DEATH))
		{
			COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
			return TRUE;
		}
		break;
	case UNIT_OBJECT:
		if (bForce || UNITS_HasCollision(pUnit))
		{
			COLLISION_ResetMaskWithSizeXY(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetUnitSizeY(pUnit), UNITS_GetCollisionType(pUnit));
			return TRUE;
		}
		break;
	default:
		COLLISION_ResetMaskWithSize(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetCollisionType(pUnit));
		return TRUE;
	}
	return FALSE;
}

//D2Common.0x6FDA8600
int __stdcall D2Common_10142(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit, int a3)
{
	UNIMPLEMENTED();
	return 0;
}

//D2Common.0x6FDA8E30
//TODO: Find a name
int __fastcall sub_6FDA8E30(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit)
{
	int nXDistance = 0;
	int nYDistance = 0;
	int nOldX = 0;
	int nOldY = 0;
	char szText[256] = {};
	D2CoordStrc pCoords = {};

	nOldX = pDynamicPath->tGameCoords.wPosX;
	nOldY = pDynamicPath->tGameCoords.wPosY;

	pDynamicPath->dwCurrentPointIdx = 0;

	if (pDynamicPath->pTargetUnit)
	{
		UNITS_GetCoords(pDynamicPath->pTargetUnit, &pCoords);

		pDynamicPath->SP1.X = pCoords.nX;
		pDynamicPath->SP1.Y = pCoords.nY;
	}

	nXDistance = nOldX - pDynamicPath->SP1.X;
	if (nXDistance < 0)
	{
		nXDistance = -nXDistance;
	}

	D2_ASSERTM(nXDistance < 100, FOG_csprintf(szText, "Missile Firing too far : %d", pUnit ? pUnit->dwClassId : -1));

	nYDistance = nOldY - pDynamicPath->SP1.Y;
	if (nYDistance < 0)
	{
		nYDistance = -nYDistance;
	}

	D2_ASSERTM(nYDistance < 100, FOG_csprintf(szText, "Missile Firing too far : %d", pUnit ? pUnit->dwClassId : -1));

	if (pDynamicPath->SP1.X && pDynamicPath->SP1.Y)
	{
		pDynamicPath->PathPoints[0].X = pDynamicPath->SP1.X;
		pDynamicPath->PathPoints[0].Y = pDynamicPath->SP1.Y;

		pDynamicPath->dwPathPoints = 1;
		pDynamicPath->unk0x38 = 0;
		sub_6FDAC790(pDynamicPath, 1, 1);

		if (!(pDynamicPath->pRoom && DungeonTestRoomGame(pDynamicPath->pRoom, pDynamicPath->SP1.X, pDynamicPath->SP1.Y)))
		{
			pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
		}

		return pDynamicPath->dwPathPoints;
	}

	return 0;
}

//D2Common.0x6FDA8FE0
//TODO: Find a name
void __fastcall sub_6FDA8FE0(D2PathInfoStrc* pPathInfo)
{
	int nOffsetX = 0;
	int nOffsetY = 0;
	int nDiffX = 0;
	int nDiffY = 0;
	int nX = 0;
	int nY = 0;

	nX = pPathInfo->tTargetCoord.X;
	nY = pPathInfo->tTargetCoord.Y;

	nDiffX = nX - pPathInfo->pStartCoord.X;
	if (nDiffX < 0)
	{
		nDiffX = -nDiffX;
	}

	nDiffY = nY - pPathInfo->pStartCoord.Y;
	if (nDiffY < 0)
	{
		nDiffY = -nDiffY;
	}

	if (nDiffX < 5 && nDiffY < 5 && (nDiffX || nDiffY))
	{
		nOffsetX = dword_6FDD1E98[nDiffX + 9 * nDiffY];
		nOffsetY = dword_6FDD1E98[nDiffY + 9 * nDiffX];

		if (nDiffX <= nDiffY)
		{
			nDiffX = nDiffY;
		}

		while (nDiffX < 5)
		{
			nX += nOffsetX;
			nY += nOffsetY;

			if (COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pRoom, nX, nY, pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
			{
				break;
			}

			++nDiffX;
			pPathInfo->tTargetCoord.X = nX;
			pPathInfo->tTargetCoord.Y = nY;
		}
	}
}

//D2Common.0x6FDA90C0
//TODO: Find a name
uint8_t __fastcall sub_6FDA90C0(D2PathInfoStrc* pPathInfo)
{
	D2UnitStrc* pTargetUnit = pPathInfo->pDynamicPath->pTargetUnit;

	D2_ASSERT(UNITS_IsDoor(pTargetUnit));

	if (!UNITS_CheckIfObjectOrientationIs1(pTargetUnit))
	{
		if (pPathInfo->pDynamicPath->tGameCoords.wPosX >= UNITS_GetXPosition(pTargetUnit))
		{
			pPathInfo->pDynamicPath->SP1.X += 2;
		}
		else
		{
			pPathInfo->pDynamicPath->SP1.X -= 2;
		}
	}
	else
	{
		if (pPathInfo->pDynamicPath->tGameCoords.wPosY >= UNITS_GetYPosition(pTargetUnit))
		{
			pPathInfo->pDynamicPath->SP1.Y += 2;
		}
		else
		{
			pPathInfo->pDynamicPath->SP1.Y -= 2;
		}
	}

	return 2;
}

//D2Common.0x6FDA9190 (#10156)
void __stdcall PATH_FreeDynamicPath(void* pMemPool, D2DynamicPathStrc* pDynamicPath)
{
	D2_FREE_POOL(pMemPool, pDynamicPath);
}

//D2Common.0x6FDA91B0 (#11282)
//Unused
int __stdcall PATH_GetCollisionPatternFromMonStats2Txt(int nMonsterId)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
	if (pMonStats2TxtRecord && pMonStats2TxtRecord->nSizeX >= 0 && pMonStats2TxtRecord->nSizeX < COLLISION_UNIT_SIZE_COUNT)
	{
		D2C_CollisionPattern nCollisionPattern = gaCollisionPatternsFromSize_6FDD1DE4[pMonStats2TxtRecord->nSizeX];

		if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_REVIVE] || pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL])
		{
			if (!(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_ISATT]))
			{
				if (nCollisionPattern == COLLISION_PATTERN_SMALL_UNIT_PRESENCE)
				{
					nCollisionPattern = COLLISION_PATTERN_SMALL_PET_PRESENCE;
				}
				else if (nCollisionPattern == COLLISION_PATTERN_BIG_UNIT_PRESENCE)
				{
					nCollisionPattern = COLLISION_PATTERN_BIG_PET_PRESENCE;
				}
			}
		}

		return nCollisionPattern;
	}

	return COLLISION_PATTERN_SMALL_UNIT_PRESENCE;
}

//D2Common.0x6FDA9250 (#11281)
int __stdcall D2Common_11281_CollisionPatternFromSize(D2UnitStrc* pUnit, int nSize)
{
	if (nSize < 0 && nSize >= COLLISION_UNIT_SIZE_COUNT)
	{
		return COLLISION_PATTERN_SMALL_UNIT_PRESENCE;
	}
	
	const D2C_CollisionPattern nCollisionPattern = gaCollisionPatternsFromSize_6FDD1DE4[nSize];
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && MONSTERS_CanBeInTown(pUnit))
	{
		const D2MonStatsTxt* pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
		if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]))
		{
			if (nCollisionPattern == COLLISION_PATTERN_SMALL_UNIT_PRESENCE)
			{
				return COLLISION_PATTERN_SMALL_PET_PRESENCE;
			}
			else if (nCollisionPattern == COLLISION_PATTERN_BIG_UNIT_PRESENCE)
			{
				return COLLISION_PATTERN_BIG_PET_PRESENCE;
			}
		}
	}

	return nCollisionPattern;
}

//D2Common.0x6FDA92F0 (#10214)
//TODO: Find a name
void __stdcall D2Common_10214(D2UnitStrc* pUnit)
{
	if (pUnit->pDynamicPath)
	{
		if (UNITS_GetRoom(pUnit))
		{
			PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
			pUnit->pDynamicPath->dwCollisionPattern = D2Common_11281_CollisionPatternFromSize(pUnit, pUnit->pDynamicPath->dwUnitSize);
			PATH_AddCollisionFootprintForUnit(pUnit);
		}
		else
		{
			pUnit->pDynamicPath->dwCollisionPattern = D2Common_11281_CollisionPatternFromSize(pUnit, pUnit->pDynamicPath->dwUnitSize);
		}
	}
}

//D2Common.0x6FDA9480 (#10152)
void __stdcall PATH_AllocDynamicPath(void* pMemPool, D2RoomStrc* pRoom, int nX, int nY, D2UnitStrc* pUnit, BOOL bSetFlag)
{
	D2DynamicPathStrc* pDynamicPath = D2_CALLOC_STRC_POOL(pMemPool, D2DynamicPathStrc);

	pUnit->pDynamicPath = pDynamicPath;

	pDynamicPath->pUnit = pUnit;
	pDynamicPath->dwUnitSize = UNITS_GetUnitSizeX(pUnit);

	pDynamicPath->dwCollisionPattern = D2Common_11281_CollisionPatternFromSize(pUnit, pDynamicPath->dwUnitSize);

	pDynamicPath->dwVelocity = 2048;
	pDynamicPath->pRoom = pRoom;
	pDynamicPath->nStepNum = 0;
	pDynamicPath->tGameCoords.dwPrecisionX = PATH_ToFP16Center(nX);
	pDynamicPath->tGameCoords.dwPrecisionY = PATH_ToFP16Center(nY);

	pDynamicPath->nSavedStepsCount = 1;
	pDynamicPath->SavedSteps[0].X = nX;
	pDynamicPath->SavedSteps[0].Y = nY;

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		pDynamicPath->dwCollisionType = COLLIDE_PLAYER;
		pDynamicPath->unk0x50 = 0x1C09;
		PATH_SetType(pDynamicPath, PATHTYPE_UNKNOWN_7);
		pDynamicPath->nDistMax = 73;
		pDynamicPath->unk0x92 = 70;
	}
	else if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		pDynamicPath->dwCollisionType = COLLIDE_MONSTER;
		PATH_SetType(pDynamicPath, PATHTYPE_TOWARD);

		int nClassId = pUnit->dwClassId;

		D2MonStatsTxt* pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
		if (pMonStatsTxtRecord)
		{
			nClassId = pMonStatsTxtRecord->nBaseId;
		}

		if (nClassId == MONSTER_WRAITH1)
		{
			pDynamicPath->dwCollisionPattern = COLLISION_PATTERN_SMALL_NO_PRESENCE;
			pDynamicPath->unk0x50 = 0x804;
			pDynamicPath->nDistMax = 14;
		}
		else
		{
			if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_FLYING])
			{
				pDynamicPath->nDistMax = 14;
				pDynamicPath->unk0x50 = 0x1804;
			}
			else
			{
				pDynamicPath->nDistMax = 14;
				pDynamicPath->unk0x50 = (pMonStatsTxtRecord && (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_OPENDOORS]) != 0) ? 0x3401 : 0x3C01;
			}
		}
	}
	else if (pUnit->dwUnitType == UNIT_MISSILE)
	{
		pDynamicPath->dwCollisionType = COLLIDE_NONE;
		pDynamicPath->unk0x50 = 0;
		PATH_SetType(pDynamicPath, PATHTYPE_MISSILE);
	}

	if (pRoom)
	{
		PATH_AddCollisionFootprintForUnit(pUnit);
		UNITROOM_AddUnitToRoom(pUnit, pDynamicPath->pRoom);
	}
	
	PATH_UpdateClientCoords(pDynamicPath);
	
	if (bSetFlag)
	{
		pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00010;
	}
}

//D2Common.0x6FDA9720
//TODO: Find a name
void __fastcall sub_6FDA9720(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection)
{
	D2C_UnitTypes nUnitType;

	if (pDynamicPath->pUnit)
	{
		nUnitType = D2C_UnitTypes(pDynamicPath->pUnit->dwUnitType);
	}
	else
	{
		nUnitType = UNIT_TYPES_COUNT;
	}

	const uint8_t nNormalizedDirection = PATH_NormalizeDirection(nDirection);
	switch (nUnitType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
		pDynamicPath->nDirection = nNormalizedDirection;
		break;
	case UNIT_MISSILE:
		if (!(pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00040))
		{
			pDynamicPath->nDirection = nNormalizedDirection;
		}
		break;
	default:
		if (nNormalizedDirection != pDynamicPath->nNewDirection)
		{
			pDynamicPath->nNewDirection = nNormalizedDirection;
			pDynamicPath->nDiffDirection = dword_6FDD1F88[PATH_NormalizeDirection(nNormalizedDirection - pDynamicPath->nDirection)];
		}
		break;
	}
}

//D2Common.0x6FDA9770 (#10193)
//TODO: Check name
void __stdcall D2COMMON_10193_PATH_AdjustDirection(D2DynamicPathStrc* pDynamicPath)
{
	if ((pDynamicPath->nDirection != pDynamicPath->nNewDirection) && !(pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00040))
	{
		pDynamicPath->nDirection = PATH_NormalizeDirection(pDynamicPath->nDirection + pDynamicPath->nDiffDirection);

		const uint8_t dirDiff = PATH_NormalizeDirection(pDynamicPath->nDirection - pDynamicPath->nNewDirection);
		if (dirDiff > 31)
		{
			if (pDynamicPath->nDiffDirection > 63)
			{
				pDynamicPath->nDirection = pDynamicPath->nNewDirection;
			}
		}
		else if (dirDiff < 31)
		{
			if (pDynamicPath->nDiffDirection < 63)
			{
				pDynamicPath->nDirection = pDynamicPath->nNewDirection;
			}
		}
	}
}

//D2Common.0x6FDA97C0 (#10216)
//TODO: Find a name
void __stdcall D2Common_10216(D2DynamicPathStrc* pDynamicPath, int nX, int nY, int a4)
{
	if (pDynamicPath)
	{
		const uint8_t nNormalizedDirection = PATH_NormalizeDirection(PATH_ComputeDirectionFromPreciseCoords_6FDAC760(pDynamicPath->tGameCoords.dwPrecisionX, pDynamicPath->tGameCoords.dwPrecisionY, PATH_ToFP16Center(nX), PATH_ToFP16Center(nY)));
		if (a4)
		{
			pDynamicPath->nNewDirection = nNormalizedDirection;
			pDynamicPath->nDirection = nNormalizedDirection;
		}
		else
		{
			sub_6FDA9720(pDynamicPath, nNormalizedDirection);
		}
	}
}

//D2Common.0x6FDA9850 (#10228)
//TODO: Find a name
void __stdcall D2Common_10228(D2UnitStrc* pUnit)
{
	if (pUnit->pDynamicPath)
	{
		PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
	}
}

//D2Common.0x6FDA9870 (#10143)
void __stdcall PATH_SetUnitDeadCollision(D2UnitStrc* pUnit, BOOL bForGameLogic)
{
	D2_ASSERT(pUnit->pDynamicPath != nullptr);
	if (bForGameLogic || pUnit->dwUnitType != UNIT_MONSTER)
	{
		PATH_RemoveCollisionFootprintForUnit(pUnit, TRUE);
		COLLISION_SetMaskWithSizeXY(pUnit->pDynamicPath->pRoom, pUnit->pDynamicPath->tGameCoords.wPosX, pUnit->pDynamicPath->tGameCoords.wPosY, 3, 3, COLLIDE_CORPSE);
		D2Common_10233(pUnit->pDynamicPath);
	}
	else // Called from Client, related to corpse being displayed
	{
		PATH_RemoveCollisionFootprintForUnit(pUnit, TRUE);
		pUnit->pDynamicPath->dwCollisionPattern = COLLISION_PATTERN_SMALL_NO_PRESENCE;
		PATH_SetCollisionType(pUnit->pDynamicPath, COLLIDE_CORPSE);
		PATH_AddCollisionFootprintForUnit(pUnit);
		D2Common_10233(pUnit->pDynamicPath);
	}
}

//D2Common.0x6FDA98F0 (#10144)
void __stdcall PATH_SetUnitAliveCollision(D2UnitStrc* pUnit, BOOL bForGameLogic)
{
	if (bForGameLogic)
	{
		return;
	}
	//else: Called from Client to remove corpse collision

	D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_MONSTER);
	if (pUnit->pDynamicPath && pUnit->pDynamicPath->dwCollisionType == COLLIDE_CORPSE)
	{
		int nCollisionPattern = D2Common_11281_CollisionPatternFromSize(pUnit, UNITS_GetUnitSizeX(pUnit));

		if (pUnit->dwUnitType == UNIT_MONSTER)
		{
			const int nClassId = MONSTERS_GetBaseIdFromMonsterId(pUnit->dwClassId);

			if (nClassId == MONSTER_WRAITH1 || nClassId == MONSTER_BIRD1 || nClassId == MONSTER_BIRD2 || nClassId == MONSTER_PARROT)
			{
				nCollisionPattern = COLLISION_PATTERN_SMALL_NO_PRESENCE;
			}
		}

		if (UNITS_GetRoom(pUnit))
		{
			PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
			pUnit->pDynamicPath->dwCollisionPattern = nCollisionPattern;
			PATH_SetCollisionType(pUnit->pDynamicPath, COLLIDE_MONSTER);
			PATH_AddCollisionFootprintForUnit(pUnit);
		}
		else
		{
			pUnit->pDynamicPath->dwCollisionPattern = nCollisionPattern;
			PATH_SetCollisionType(pUnit->pDynamicPath, COLLIDE_MONSTER);
		}
	}
}

//D2Common.0x6FDA9A70 (#10146)
void __stdcall PATH_SetVelocity(D2DynamicPathStrc* pDynamicPath, int nVelocity, const char* szFile, int nLine)
{
	if (pDynamicPath)
	{
		if (nVelocity != pDynamicPath->dwVelocity)
		{
			pDynamicPath->unk0x38 = 15;
		}

		pDynamicPath->dwVelocity = nVelocity;
		pDynamicPath->dwMaxVelocity = nVelocity;
	}
	else
	{
		FOG_Trace("PathSetVelocity passed NULL hPath from FILE:%s  LINE:%d", szFile, nLine);
	}
}

//D2Common.0x6FDA9AB0 (#10147)
int __stdcall PATH_GetVelocity(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwVelocity;
}

//D2Common.0x6FDA9AC0 (#10148)
void __stdcall PATH_SetMaxVelocity(D2DynamicPathStrc* pDynamicPath, int nMaxVelocity)
{
	pDynamicPath->dwMaxVelocity = nMaxVelocity;
}

//D2Common.0x6FDA9AE0 (#10149)
int __stdcall PATH_GetMaxVelocity(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwMaxVelocity;
}

//D2Common.0x6FDA9AF0 (#10150)
void __stdcall PATH_SetAcceleration(D2DynamicPathStrc* pDynamicPath, int nAcceleration)
{
	pDynamicPath->dwAcceleration = nAcceleration;
}

//D2Common.0x6FDA9B10 (#10151)
int __stdcall PATH_GetAcceleration(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwAcceleration;
}

//D2Common.0x6FDA9B20 (#10153)
//TODO: Find a name
void __stdcall D2Common_10153(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->dwPathPoints = 0;
	pDynamicPath->dwFlags &= (~PATH_UNKNOWN_FLAG_0x00020);
}

//D2Common.0x6FDA9B40 (#10208)
//TODO: Check name
void __stdcall D2COMMON_10208_PathSetPathingFlag(D2DynamicPathStrc* pDynamicPath, BOOL bSet)
{
	if (bSet)
	{
		pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00020;
	}
	else
	{
		pDynamicPath->dwFlags &= (~PATH_UNKNOWN_FLAG_0x00020);
	}
}

//D2Common.0x6FDA9B70 (#10209)
//TODO: Check name
BOOL __stdcall D2COMMON_10209_PathCheckPathingFlag(D2DynamicPathStrc* pDynamicPath)
{
	return (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00020);
}

//D2Common.0x6FDA9B80 (#10154)
int __stdcall PATH_GetNumberOfPathPoints(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDA9B90 (#11291)
void __stdcall PATH_SetNumberOfPathPoints(D2DynamicPathStrc* pDynamicPath, int a2)
{
	if (a2 < D2DynamicPathStrc::MAXPATHLEN)
	{
		pDynamicPath->dwPathPoints = a2;
	}
	else
	{
		pDynamicPath->dwPathPoints = D2DynamicPathStrc::MAXPATHLEN - 1;
	}
}

//D2Common.0x6FDA9BC0 (#10155)
//TODO: Find a name
int __stdcall D2Common_10155(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwCurrentPointIdx;
}

//D2Common.0x6FDA9BD0 (#10157)
int __stdcall PATH_GetPathPoints(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints)
{
	*ppPathPoints = pDynamicPath->PathPoints;
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDA9BF0 (#10158)
uint8_t __stdcall PATH_GetDirection(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->nDirection;
	}

	return 0;
}

//D2Common.0x6FDA9C10 (#10159)
uint8_t __stdcall PATH_GetNewDirection(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nNewDirection;
}

//D2Common.0x6FDA9C20 (#10160)
//TODO: Check name
void __stdcall D2COMMON_10160_PathUpdateDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection)
{
	D2_ASSERT(nDirection < PATH_NB_DIRECTIONS);

	sub_6FDA9720(pDynamicPath, nDirection);
}

//D2Common.0x6FDA9C90 (#10161)
void __stdcall PATH_SetDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection)
{
	if (pDynamicPath)
	{
		pDynamicPath->nDirection = PATH_NormalizeDirection(nDirection);
		pDynamicPath->nNewDirection = PATH_NormalizeDirection(nDirection);
	}
}

//D2Common.0x6FDA9CB0 (#10162)
int __stdcall PATH_GetXPosition(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->tGameCoords.wPosX;
}

//D2Common.0x6FDA9CF0 (#10163)
int __stdcall PATH_GetYPosition(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->tGameCoords.wPosY;
}

//D2Common.0x6FDA9D30 (#10194)
int __stdcall PATH_GetPrecisionX(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->tGameCoords.dwPrecisionX;
}

//D2Common.0x6FDA9D60 (#10195)
int __stdcall PATH_GetPrecisionY(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->tGameCoords.dwPrecisionY;
}

//D2Common.0x6FDA9D90 (#10196)
void __stdcall PATH_SetPrecisionX(D2DynamicPathStrc* pDynamicPath, int nPrecisionX)
{
	pDynamicPath->tGameCoords.dwPrecisionX = nPrecisionX;
}

//D2Common.0x6FDA9DA0 (#10197)
void __stdcall PATH_SetPrecisionY(D2DynamicPathStrc* pDynamicPath, int nPrecisionY)
{
	pDynamicPath->tGameCoords.dwPrecisionY = nPrecisionY;
}

//D2Common.0x6FDA9DB0 (#10164)
int __stdcall PATH_GetClientCoordX(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwClientCoordX;
}

//D2Common.0x6FDC3CE0 (#10165)
int __stdcall PATH_GetClientCoordY(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwClientCoordY;
}

//D2Common.0x6FDA9DC0
void __fastcall PATH_SetClientCoordX(D2DynamicPathStrc* pDynamicPath, int nTargetX)
{
	pDynamicPath->dwClientCoordX = nTargetX;
}

//D2Common.0x6FDA9DD0
void __fastcall PATH_SetClientCoordY(D2DynamicPathStrc* pDynamicPath, int nTargetY)
{
	pDynamicPath->dwClientCoordY = nTargetY;
}

//D2Common.0x6FDA9DE0 (#10175)
//TODO: Check name
int __stdcall D2COMMON_10175_PathGetFirstPointX(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->SP1.X;
}

//D2Common.0x6FDA9DF0 (#10176)
//TODO: Check name
int __stdcall D2COMMON_10176_PathGetFirstPointY(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->SP1.Y;
}

//D2Common.0x6FDA9E00 (#10224)
//TODO: Find a name
int __stdcall D2Common_10224(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->SP3.X;
}

//D2Common.0x6FDA9E10 (#10225)
//TODO: Find a name
int __stdcall D2Common_10225(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->SP3.Y;
}

//D2Common.0x6FDA9E20 (#10177)
int __stdcall D2COMMON_10177_PATH_GetLastPointX(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath->dwPathPoints > 0)
	{
		return pDynamicPath->PathPoints[pDynamicPath->dwPathPoints-1].X;
	}

	return 0;
}

//D2Common.0x6FDA9E40 (#10178)
int __stdcall D2COMMON_10178_PATH_GetLastPointY(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath->dwPathPoints > 0)
	{
		return pDynamicPath->PathPoints[pDynamicPath->dwPathPoints-1].Y;
	}

	return 0;
}

//D2Common.0x6FDB9C10 (#10166)
D2RoomStrc* __stdcall PATH_GetRoom(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->pRoom;
}

//D2Common.0x6FDA9E60 (#10167)
void __stdcall PATH_SetRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom)
{
	pDynamicPath->pRoom = pRoom;
}

//D2Common.0x6FDA9E70 (#10168)
D2RoomStrc* __stdcall PATH_GetNextRoom(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->pRoomNext;
}

//D2Common.0x6FDA9E80 (#10169)
void __stdcall PATH_ClearNextRoom(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->pRoomNext = NULL;
}

//D2Common.0x6FDA9E90 (#10170)
//TODO: Check name
void __stdcall D2COMMON_10170_PathSetTargetPos(D2DynamicPathStrc* pDynamicPath, int nX, int nY)
{
	if (pDynamicPath)
	{
		pDynamicPath->SP1.X = nX;
		pDynamicPath->SP1.Y = nY;
		pDynamicPath->pTargetUnit = NULL;
	}
}

//D2Common.0x6FDA9EC0 (#10172)
//TODO: Find a name
BOOL __stdcall PATH_IsCurrentRoomInvalid(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwFlags & PATH_CURRENT_ROOM_INVALID;
}

//D2Common.0x6FDA9ED0 (#10173)
//TODO: Find a name
void __stdcall PATH_SetCurrentRoomInvalid(D2DynamicPathStrc* pDynamicPath, BOOL bSet)
{
	if (bSet)
	{
		pDynamicPath->dwFlags |= PATH_CURRENT_ROOM_INVALID;
	}
	else
	{
		pDynamicPath->dwFlags &= (~PATH_CURRENT_ROOM_INVALID);
	}
}

//D2Common.0x6FDA9F00 (#10145)
void __stdcall PATH_SetUnusedFlag_0x00004(D2DynamicPathStrc* pDynamicPath, BOOL bSet)
{
	if (bSet)
	{
		pDynamicPath->dwFlags |= PATH_UNUSED_FLAG_0x00004;
	}
	else
	{
		pDynamicPath->dwFlags &= (~PATH_UNUSED_FLAG_0x00004);
	}
}

//D2Common.0x6FDA9F30 (#10174)
BOOL __stdcall PATH_GetUnusedFlag_0x00004(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwFlags & PATH_UNUSED_FLAG_0x00004;
}

//D2Common.0x6FDA9F40 (#10179)
void __stdcall PATH_SetTargetUnit(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit)
{
	pDynamicPath->pTargetUnit = pUnit;

	if (pUnit)
	{
		pDynamicPath->dwTargetType = pUnit->dwUnitType;
		pDynamicPath->dwTargetId = pUnit->dwUnitId;
	}
}

//D2Common.0x6FDA9F60 (#10171)
void __stdcall PATH_GetTargetTypeAndGUID(D2DynamicPathStrc* pDynamicPath, int* pTargetType, D2UnitGUID* pTargetGUID)
{
	D2_ASSERT(pDynamicPath->pTargetUnit);

	*pTargetType = pDynamicPath->dwTargetType;
	*pTargetGUID = pDynamicPath->dwTargetId;
}

//D2Common.0x6FDA9FA0 (#10180)
D2UnitStrc* __stdcall PATH_GetTargetUnit(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->pTargetUnit;
	}

	return NULL;
}

//D2Common.0x6FDA9FC0 (#10181)
int __stdcall PATH_GetCollisionType(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->dwCollisionType;
	}
	
	// Note: this returns 0xFFFF not 0xFFFFFFFF because D2C_CollisionFlags is 16bits.
	return COLLIDE_ALL_MASK;
}

//D2Common.0x6FDA9FE0 (#10182)
void __stdcall PATH_SetCollisionType(D2DynamicPathStrc* pDynamicPath, int nCollisionType)
{
	if (pDynamicPath->pRoom)
	{
		if (pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_MISSILE)
		{
			COLLISION_ResetMaskWithSize(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY, pDynamicPath->dwUnitSize, pDynamicPath->dwCollisionType);
		}
		else
		{
			COLLISION_ResetMaskWithPattern(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY, pDynamicPath->dwCollisionPattern, pDynamicPath->dwCollisionType);
		}
	}

	pDynamicPath->dwCollisionType = nCollisionType;

	if (pDynamicPath->pRoom)
	{
		if (pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_MISSILE)
		{
			COLLISION_SetMaskWithSize(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY, pDynamicPath->dwUnitSize, nCollisionType);
		}
		else
		{
			COLLISION_SetMaskWithPattern(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY, pDynamicPath->dwCollisionPattern, nCollisionType);
		}
	}
}

//D2Common.0x6FDAA0C0 (#10183)
//TODO: Find a name
int __stdcall D2Common_10183(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->unk0x50;
}

//D2Common.0x6FDAA0D0 (#10184)
//TODO: Find a name
void __stdcall D2Common_10184(D2DynamicPathStrc* pDynamicPath, int a2)
{
	pDynamicPath->unk0x50 = a2;
}

//D2Common.0x6FDAA0E0 (#10185)
void __stdcall PATH_SetType(D2DynamicPathStrc* pDynamicPath, int nPathType)
{

	D2_ASSERT(!(pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER && nPathType == PATHTYPE_TOWARD));

	int nFlag = gaPathTypeFlags_6FDD2088[nPathType];
	if ((nFlag & PATH_UNKNOWN_FLAG_0x02000) && !(pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x04000))
	{
		pDynamicPath->dwPrevPathType = pDynamicPath->dwPathType;
	}

	if ((nFlag & PATH_UNKNOWN_FLAG_0x08000) && !(pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x10000))
	{
		pDynamicPath->unk0x80 = pDynamicPath->dwVelocity;
	}

	pDynamicPath->dwPathType = nPathType;
	pDynamicPath->dwFlags = nFlag | pDynamicPath->dwFlags & 0xFFF800FF;
	pDynamicPath->dwUnitTypeRelated = gaOffsetForPathType_6FDD20D0[nPathType];

	D2_ASSERT(pDynamicPath->dwPrevPathType != PATHTYPE_KNOCKBACK_CLIENT);
	D2_ASSERT(pDynamicPath->dwPrevPathType != PATHTYPE_KNOCKBACK_SERVER);
	D2_ASSERT((nPathType != PATHTYPE_MISSILE) || (pDynamicPath->nDistMax < D2DynamicPathStrc::MAXPATHLEN));
}

//D2Common.0x6FDAA1E0 (#10186)
void __stdcall PATH_ResetToPreviousType(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath->pUnit);

	if (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x08000)
	{
		pDynamicPath->dwVelocity = pDynamicPath->unk0x80;
	}

	if (pDynamicPath->pUnit->dwUnitType != UNIT_PLAYER)
	{
		if (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x02000)
		{
			PATH_SetType(pDynamicPath, pDynamicPath->dwPrevPathType);
		}
	}
	else
	{
		PATH_SetType(pDynamicPath, PATHTYPE_UNKNOWN_7);
	}
}

//D2Common.0x6FDAA240 (#10187)
int __stdcall PATH_GetType(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwPathType;
}

//D2Common.0x6FDAA250 (#10190)
void __stdcall _10190_PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nDistance)
{
	pDynamicPath->nDist = nDistance;
}

//D2Common.0x6FDAA270 (#10191)
uint8_t __stdcall _10191_PATH_GetDistance(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nDist;
}

//D2Common.0x6FDAA280 (#10188)
void __stdcall PATH_SetNewDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nNewDistance)
{
	if (pDynamicPath)
	{
		if (nNewDistance >= (D2DynamicPathStrc::MAXPATHLEN - 1))
		{
			nNewDistance = (D2DynamicPathStrc::MAXPATHLEN - 1);
		}

		pDynamicPath->nDistMax = nNewDistance;
		pDynamicPath->nDist = nNewDistance;
	}
}

//D2Common.0x6FDAA2B0 (#10189)
uint8_t __stdcall PATH_GetMaxDistance(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nDistMax;
}

//D2Common.0x6FDAA2C0 (#10201)
//TODO: Find a name
uint16_t __stdcall D2Common_10201(D2DynamicPathStrc* pDynamicPath)
{
	if (!pDynamicPath->dwVelocity)
	{
		pDynamicPath->unk0x54 = COLLISION_CheckMaskWithSize(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY, pDynamicPath->dwUnitSize, ~COLLIDE_CORPSE);
	}

	return pDynamicPath->unk0x54;
}

//D2Common.0x6FDAA300 (#10202)
//TODO: Find a name
uint16_t __stdcall D2Common_10202(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->unk0x54;
}

//D2Common.0x6FDAA310 (#10192)
//TODO: Check name
void __stdcall D2COMMON_10192_PathSetIDAMax(D2DynamicPathStrc* pDynamicPath, int a2)
{
	D2_ASSERT(pDynamicPath->dwPathType != PATHTYPE_IDASTAR);

	pDynamicPath->unk0x92 = a2;
}

//D2Common.0x6FDAA350 (#10198)
//TODO: Check name
int __stdcall D2COMMON_10198_PathGetSaveStep(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints)
{
	D2_ASSERT(pDynamicPath->dwFlags & PATH_SAVE_STEPS_MASK);

	*ppPathPoints = pDynamicPath->SavedSteps;
	return pDynamicPath->nSavedStepsCount;
}

//D2Common.0x6FDAA390 (#10199)
//TODO: Check name
int __stdcall D2COMMON_10199_PathGetSaveX(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath->dwFlags & PATH_SAVE_STEPS_MASK);

	if (pDynamicPath->nSavedStepsCount)
	{
		return pDynamicPath->SavedSteps[pDynamicPath->nSavedStepsCount -1].X;
	}

	return 0;
}

//D2Common.0x6FDAA3E0 (#10200)
//TODO: Check name
int __stdcall D2COMMON_10200_PathGetSaveY(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath->dwFlags & PATH_SAVE_STEPS_MASK);

	if (pDynamicPath->nSavedStepsCount)
	{
		return pDynamicPath->SavedSteps[pDynamicPath->nSavedStepsCount - 1].Y;
	}

	return 0;
}

//D2Common.0x6FDAA430 (#10203)
//TODO: Check name
void __stdcall D2COMMON_10203_PATH_SetRotateFlag(D2DynamicPathStrc* pDynamicPath, BOOL bReset)
{
	if (pDynamicPath)
	{
		if (bReset)
		{
			pDynamicPath->dwFlags &= (~PATH_UNKNOWN_FLAG_0x00040);
		}
		else
		{
			pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00040;
		}
	}
}

//D2Common.0x6FDAA460 (#10204)
//TODO: Check name
void __stdcall D2COMMON_10204_PATH_ClearPoint2(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->SP2.X = 0;
	pDynamicPath->SP2.Y = 0;
}

//D2Common.0x6FDAA480 (#10205)
void __stdcall PATH_SetStepNum(D2DynamicPathStrc* pDynamicPath, uint8_t nSteps)
{
	if (pDynamicPath)
	{
		if (nSteps > 0 && nSteps < PATH_MAX_STEPNUM)
		{
			pDynamicPath->nStepNum = nSteps - 1;
		}
		else
		{
			pDynamicPath->nStepNum = 0;
		}
	}
}

//D2Common.0x6FDAA4B0 (#10206)
int __stdcall PATH_GetStepNum(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nStepNum + 1;
}

//D2Common.0x6FDAA4C0 (#10207)
//TODO: Find a name
void __stdcall D2Common_10207(D2DynamicPathStrc* pDynamicPath, char a2, char a3)
{
	pDynamicPath->unk0x67 = a2;
	pDynamicPath->unk0x68[0] = a3;
}

//D2Common.0x6FDAA4E0 (#10217)
void __stdcall PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, int nDist)
{
	D2_ASSERT(nDist >= 0 && nDist <= 255);

	pDynamicPath->nDistance = nDist;
}

//D2Common.0x6FDAA520 (#10218)
int __stdcall PATH_GetDistance(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nDistance;
}

//D2Common.0x6FDAA530 (#10219)
void __stdcall PATH_AddToDistance(D2DynamicPathStrc* pDynamicPath, int nAddition)
{
	int nDistance = nAddition + pDynamicPath->nDistance;
	pDynamicPath->nDistance = D2Clamp(nDistance, 0, 0xFF);
}

//D2Common.0x6FDAA570 (#10210)
int __stdcall PATH_GetUnitCollisionPattern(D2UnitStrc* pUnit)
{
	return pUnit->pDynamicPath->dwCollisionPattern;
}

//D2Common.0x6FDAA580 (#10211)
void __stdcall PATH_SetUnitCollisionPattern(D2UnitStrc* pUnit, int nCollisionPattern)
{
	pUnit->pDynamicPath->dwCollisionPattern = nCollisionPattern;
}

//D2Common.0x6FDAA5A0 (#10212)
//TODO: Check name
void __stdcall D2COMMON_10212_PATH_SetMoveFlags(D2UnitStrc* pUnit, BOOL bSet)
{
	D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;

	D2_ASSERT(pDynamicPath);

	if (bSet)
	{
		pUnit->dwFlagEx |= UNITFLAGEX_UNK_PATH_RELATED;
		pDynamicPath->unk0x38 = 0;
	}
	else
	{
		pUnit->dwFlagEx &= (~UNITFLAGEX_UNK_PATH_RELATED);
		pDynamicPath->unk0x38 = 0;
	}
}

//D2Common.0x6FDAA600 (#10213)
//TODO: Find a name
void __stdcall D2Common_10213(D2UnitStrc* pUnit)
{
	D2UnitStrc* pTargetUnit = NULL;

	if (pUnit && !(pUnit->dwFlags & UNITFLAG_ISASYNC) && (pUnit->dwFlagEx & UNITFLAGEX_UNK_PATH_RELATED))
	{
		D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;
		D2_ASSERT(pDynamicPath);

		++pDynamicPath->unk0x38;

		if (pDynamicPath->unk0x38 > 1)
		{
			pDynamicPath->unk0x38 = 0;
			if (!D2Common_10227(pUnit))
			{
				if (D2Common_10236(pUnit, pDynamicPath->pTargetUnit != nullptr))
				{
					UNITS_ChangeAnimMode(pUnit, 2);
				}
			}
		}
	}
}

//D2Common.0x6FDAA6A0 (#10220)
int __stdcall PATH_ComputeSquaredDistance(int nX1, int nY1, int nX2, int nY2)
{
	const int deltaX = (nX2 - nX1);
	const int deltaY = (nY2 - nY1);
	return deltaX * deltaX + deltaY * deltaY;
}

//D2Common.0x6FDAA6D0 (#10221)
void __stdcall PATH_AddCollisionFootprintForOptionalUnit(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		PATH_AddCollisionFootprintForUnit(pUnit);
	}
}

//D2Common.0x6FDAA6F0 (#10237)
//TODO: Find a name
BOOL __stdcall D2Common_10237(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pDynamicPath)
	{
		return (pUnit->pDynamicPath->dwCurrentPointIdx < pUnit->pDynamicPath->dwPathPoints);
	}

	return FALSE;
}
