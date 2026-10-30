#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Skills.h>


BEGIN_VISIT(D2SkillCalcStrc)
	POINTER(pUnit)
	FIELD(nSkillId)
	FIELD(nSkillLevel)
END_VISIT()

BEGIN_VISIT(D2SkillStrc)
	POINTER(pSkillsTxt)
	POINTER(pNextSkill)
	FIELD(dwSkillMode)
	FIELD(dwFlags)
	ARRAY(unk0x10)
	//BEGIN_UNION()
		//FIELD(dwTargetInfo)
		//FIELD(nXpos)
		FIELD(nPar1)
	//END_UNION()
	//BEGIN_UNION()
		//FIELD(dwTargetType)
		//FIELD(nYpos)
		FIELD(nPar2)
	//END_UNION()
	//BEGIN_UNION()
		//FIELD(dwTargetGUID)
		FIELD(nPar3)
	//END_UNION()
	FIELD(nPar4)
	FIELD(nSkillLevel)
	FIELD(nLevelBonus)
	FIELD(nQuantity)
	FIELD(nOwnerGUID)
	FIELD(nCharges)
END_VISIT()

BEGIN_VISIT(D2SkillListStrc)
	OMIT(pMemPool)
	POINTER(pFirstSkill)
	POINTER(pLeftSkill)
	POINTER(pRightSkill)
	POINTER(pUsedSkill)
	FIELD(unk014)
END_VISIT()

BEGIN_VISIT(D2SkillTreeChartStrc)
	//POINTER(pCellFile)
	//POINTER(szFileName)
	//ARRAY(unk0x08)
	//FIELD(unk0x20)
END_VISIT()
