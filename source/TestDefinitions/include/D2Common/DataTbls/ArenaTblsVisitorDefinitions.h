#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/ArenaTbls.h>


BEGIN_VISIT(D2ArenaTxt)
	//BEGIN_UNION()
		FIELD(dwSuicide)
		FIELD(dwPlayerKill)
		FIELD(dwPlayerKillPercent)
		FIELD(dwMonsterKill)
		FIELD(dwPlayerDeath)
		FIELD(dwPlayerDeathPercent)
		FIELD(dwMonsterDeath)
		//ARRAY(dwScores)
	//END_UNION()
END_VISIT()

BEGIN_VISIT(D2CharItemStrc)
	FIELD(dwItemCode)
	FIELD(nBodyLoc)
	FIELD(nItemCount)
	FIELD(pad0x06)
END_VISIT()

BEGIN_VISIT(D2CharTemplateTxt)
	ARRAY(szName)
	FIELD(nClass)
	FIELD(nLevel)
	FIELD(nAct)
	FIELD(nStr)
	FIELD(nDex)
	FIELD(nInt)
	FIELD(nVit)
	FIELD(nMana)
	FIELD(nHitpoints)
	FIELD(nManaRegenBonus)
	FIELD(nVelocity)
	FIELD(nAttackRate)
	FIELD(nOtherRate)
	FIELD(pad0x2B)
	FIELD(dwRightSkill)
	ARRAY(dwSkill)
	ARRAY(dwSkillLevel)
	ARRAY(pItems)
END_VISIT()
