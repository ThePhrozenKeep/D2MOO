#include <Path/PathUtil.h>
#include <Units/Units.h>
#include <Fog.h>

//1.10f: D2Common.0x6FDAB890
//1.13c: D2Common.0x6FD8E080
void __fastcall PATHUtil_AdvanceTowardsTarget_6FDAB890(D2DynamicPathStrc* ptPath)
{
	D2_ASSERT(ptPath);
	D2UnitStrc* ptTarget = ptPath->pTargetUnit;
	D2_ASSERT(ptTarget);
	uint8_t nDirection = PATH_GetDirection(ptTarget->pDynamicPath);
	int nSpeed = ptPath->dwSpeed;
	uint16_t v4 = (uint16_t)nSpeed << 8;
	const float nDistance = nSpeed;
	const uint16_t nAngleRad512 = (8 * nDirection - v4) & 0x1FF;
	ptPath->SP1.X += (int64_t)(FOG_10083_Cos_LUT(nAngleRad512) * nDistance);
	ptPath->SP1.Y += (int64_t)(FOG_10084_Sin_LUT(nAngleRad512) * nDistance);
}

//1.10f: D2Common.0x6FDAB940
//1.13c: D2Common.0x6FD8E310
void __fastcall sub_6FDAB940(D2PathPointStrc* pOutPathPoint, D2DynamicPathStrc* ptPath)
{
	D2_ASSERT(ptPath);
	D2_ASSERT(ptPath->pTargetUnit);
	D2UnitStrc* pTargetUnit = ptPath->pTargetUnit;
	D2C_UnitTypes dwTargetUnitType = (D2C_UnitTypes)pTargetUnit->dwUnitType;

	if (dwTargetUnitType == UNIT_OBJECT || dwTargetUnitType == UNIT_ITEM || dwTargetUnitType == UNIT_TILE)
	{
		pOutPathPoint->X = pTargetUnit->pStaticPath->tGameCoords.nX;
		pOutPathPoint->Y = pTargetUnit->pStaticPath->tGameCoords.nY;
	}
	else
	{
		if (D2DynamicPathStrc* pTargetPath = pTargetUnit->pDynamicPath)
		{
			pOutPathPoint->X = PATH_GetXPosition(pTargetPath);
			pOutPathPoint->Y = PATH_GetYPosition(pTargetPath);
		}
		else
		{
			*pOutPathPoint = { 0,0 };
		}
	}

	if (ptPath->dwSpeed)
	{
		PATHUtil_AdvanceTowardsTarget_6FDAB890(ptPath);
	}
}
