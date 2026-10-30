#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2DataTbls.h>
#include <D2StatList.h>


// StatFlags holds two bit arrays (one bit per state) whose size depends on the loaded states.txt (see STATLIST_AllocStatListEx)
DEFINE_DYNAMIC_ARRAY_COUNT("D2StatListExStrc.StatFlags", 2 * ((sgptDataTables->nStatesTxtRecordCount + 31) / 32))


BEGIN_VISIT(D2SLayerStatIdStrc)
	FIELD(nLayer)
	FIELD(nStat)
END_VISIT()

BEGIN_VISIT(D2StatStrc)
	FIELD(nLayer)
	FIELD(nStat)
	FIELD(nValue)
END_VISIT()

BEGIN_VISIT(D2StatsArrayStrc)
	DYNAMIC_ARRAY(pStat, nStatCount)
	FIELD(nStatCount)
	FIELD(nCapacity)
END_VISIT()

BEGIN_VISIT(D2ModStatsArrayStrc)
	DYNAMIC_ARRAY(pStat, nStatCount)
	FIELD(nStatCount)
	FIELD(nCapacity)
END_VISIT()

BEGIN_VISIT(D2StatListStrc)
	OMIT(pMemPool)
	POINTER(pUnit)
	FIELD(dwOwnerType)
	FIELD(dwOwnerId)
	FIELD(dwFlags)
	FIELD(dwStateNo)
	FIELD(dwExpireFrame)
	FIELD(dwSkillNo)
	FIELD(dwSLvl)
	OBJECT(Stats)
	POINTER(pPrevLink)
	POINTER(pNextLink)
	POINTER(pParent)
	FIELD(fpStatRemove)
END_VISIT()

BEGIN_VISIT(D2StatListExStrc)
	OMIT(pMemPool)
	POINTER(pUnit)
	FIELD(dwOwnerType)
	FIELD(dwOwnerId)
	FIELD(dwFlags)
	FIELD(dwStateNo)
	FIELD(dwExpireFrame)
	FIELD(dwSkillNo)
	FIELD(dwSLvl)
	OBJECT(Stats)
	POINTER(pPrevLink)
	POINTER(pNextLink)
	POINTER(pParent)
	FIELD(fpStatRemove)
	POINTER(pMyLastList)
	POINTER(pMyStats)
	POINTER(pOwner)
	OBJECT(FullStats)
	OBJECT(ModStats)
	DYNAMIC_ARRAY_KEY(StatFlags, "D2StatListExStrc.StatFlags")
	FIELD(pfOnValueChanged)
	//OMIT(pGame)
END_VISIT()
