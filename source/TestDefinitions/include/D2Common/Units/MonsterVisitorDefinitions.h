#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Units/Monster.h>


BEGIN_VISIT(D2InteractInfoStrc)
	POINTER(pUnit)
	FIELD(nInteract)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2MonsterInteractStrc)
	POINTER(pInteractInfo)
END_VISIT()

BEGIN_VISIT(D2UnkMonsterDataStrc)
	FIELD(unk0x00)
	FIELD(nLifePercentage)
	FIELD(unk0x08)
	FIELD(nCount)
	ARRAY(pad0x0D)
END_VISIT()

BEGIN_VISIT(D2MonsterDataStrc)
	POINTER(pMonstatsTxt)
	ARRAY(nComponent)
	FIELD(wNameSeed)
	FIELD(nTypeFlag)
	FIELD(dwDurielFlag)
	ARRAY(nMonUmod)
	FIELD(wBossHcIdx)
	//POINTER(pAiControl)
	//BEGIN_UNION()
		//POINTER(pAiParam)
		//POINTER(wszMonName)
	//END_UNION()
	POINTER(pMonInteract)
	FIELD(unk0x34)
	FIELD(unk0x38)
	OBJECT(unk0x3C)
	FIELD(unk0x4C)
	FIELD(pVision)
	FIELD(dwAiState)
	FIELD(dwTxtLevelNo)
	FIELD(wSummonerFlag)
	ARRAY(pad0x5E)
END_VISIT()
