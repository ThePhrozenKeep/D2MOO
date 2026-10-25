#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Units/Item.h>


BEGIN_VISIT(D2ItemDataStrc)
	FIELD(dwQualityNo)
	OBJECT(pSeed)
	FIELD(dwOwnerGUID)
	FIELD(dwInitSeed)
	FIELD(dwCommandFlags)
	FIELD(dwItemFlags)
	ARRAY(dwRealmData)
	FIELD(dwActionStamp)
	FIELD(dwFileIndex)
	FIELD(dwItemLevel)
	FIELD(wItemFormat)
	FIELD(wRarePrefix)
	FIELD(wRareSuffix)
	FIELD(wAutoAffix)
	ARRAY(wMagicPrefix)
	ARRAY(wMagicSuffix)
	FIELD(nBodyLoc)
	FIELD(nInvPage)
	FIELD(nCellOverlap)
	FIELD(nItemCell)
	FIELD(nEarLvl)
	FIELD(nInvGfxIdx)
	ARRAY(szPlayerName)
	ARRAY(unk0x5A)
	OBJECT(pExtraData)
END_VISIT()
