#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Monsters.h>


BEGIN_VISIT(D2HirelingInitStrc)
	FIELD(nId)
	FIELD(nLevel)
	FIELD(nHitpoints)
	FIELD(nStrength)
	FIELD(nDexterity)
	FIELD(nGold)
	FIELD(nExperience)
	FIELD(nDefense)
	FIELD(nMinDamage)
	FIELD(nMaxDamage)
	FIELD(nShare)
	FIELD(nResist)
	FIELD(nHireDesc)
	FIELD(field_34)
	FIELD(nFlags)
END_VISIT()
