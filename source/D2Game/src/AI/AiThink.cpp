#include "AI/AiThink.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2PacketDef.h>
#include <UselessOrdinals.h>

#include <D2Collision.h>
#include <D2Combat.h>
#include <D2DataTbls.h>
#include <D2Monsters.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/SkillsIds.h>
#include <D2Dungeon.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2Skills.h>
#include <Units/UnitFinds.h>
#include <Units/UnitRoom.h>
#include <D2QuestRecord.h>

#include "AI/AiBaal.h"
#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "QUESTS/ACT2/A2Q1.h"
#include "QUESTS/ACT2/A2Q4.h"
#include "QUESTS/ACT2/A2Q5.h"
#include "QUESTS/ACT2/A2Q6.h"
#include "QUESTS/ACT3/A3Q3.h"
#include "QUESTS/ACT3/A3Q4.h"
#include "QUESTS/ACT3/A3Q7.h"
#include "QUESTS/ACT4/A4Q1.h"
#include "QUESTS/ACT5/A5Q1.h"
#include "QUESTS/ACT5/A5Q2.h"
#include "QUESTS/ACT5/A5Q3.h"
#include "QUESTS/ACT5/A5Q4.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "QUESTS/ACT5/A5Q6.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitInactive.h"
#include "UNIT/SUnitMsg.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillDruid.h"
#include "SKILLS/Skills.h"


#pragma pack(push, 1)
struct D2SandRaiderAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
};

struct D2VultureAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	int32_t nLifePercentage;
};

struct D2HighPriestAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nLifePercentage;
	int32_t nMaxDistance;
};

struct D2ZakarumPriestAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nCounter;
	int32_t nMaxLifePercentage;
	int32_t nMaxDistance;
};

struct D2PantherAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
};

struct D2MinionSpawnerAiCallbackArgStrc
{
	int32_t nMinions;
};

struct D2RegurgitatorAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
};

struct D2SiegeBeastTowerAiCallbackArgStrc
{
	int32_t nMaxDistance;
};

struct D2GreaterMummyAiCallbackArgStrc
{
	D2UnitStrc* pReviveTarget;
	D2UnitStrc* pHealTarget;
	int32_t nHealReviveCounter;
	int32_t nCounter;
	int32_t nMaxDistance;
	int32_t bRadament;
	int32_t bNormal;
};

struct D2FetishShamanAiCallbackArgStrc
{
	D2UnitStrc* pClosestDeadTarget;
	int32_t nSquaredDistanceToClosestDeadTarget;
	int32_t nMaxSearchRangeSquared;
	D2UnitStrc* pClosestAliveTarget;
	int32_t nSquaredDistanceToClosestAliveTarget;
	int32_t nAliveTargets;
	int32_t nHealCapability;
};

struct D2MephistoAiCallbackArgStrc
{
	D2UnitStrc* unk0x00;
	int32_t unk0x04;
	int32_t unk0x08;
	D2UnitStrc* unk0x0C;
	int32_t unk0x10;
	D2UnitStrc* unk0x14;
	int32_t unk0x18;
	int32_t nCounter;
	int32_t unk0x20;
	int32_t unk0x24;
};

struct D2OblivionKnightAiCallbackArgStrc
{
	D2UnitStrc* unk0x00;
	D2UnitStrc* unk0x04;
	int32_t unk0x08;
	int32_t unk0x0C;
	int32_t unk0x10;
};

struct D2BaalToStairsAiCallbackArgStrc
{
	int32_t nMaxDistance;
};

struct D2PutridDefilerAiCallbackArgStrc
{
	int32_t nMaxDistance;
};

struct D2NihlathakOverseerAiCallbackArgStrc
{
	int32_t nMaxDistance;
	int32_t nDistantMaxDistance;
	int32_t nMaxLifePercentage;
	D2UnitStrc* pTarget;
	D2UnitStrc* pDistantTarget;
	int32_t nCounter;
};

struct D2ShadowMasterAiCallbackArgStrc
{
	D2UnitStrc* unk0x00;
	D2UnitStrc* unk0x04;
	int32_t unk0x08;
	int32_t unk0x0C;
	D2UnitStrc* unk0x10;
	int32_t unk0x14;
	int32_t unk0x18;
	int32_t unk0x1C;
	int32_t unk0x20;
	D2UnitStrc* unk0x24;
};

struct D2ShadowMasterSkillStrc
{
	D2UnitStrc* pTarget;
	int32_t nSkillId;
	int32_t nChance;
	int32_t nX;
	int32_t nY;
};

struct D2PetMoveStrc
{
	D2UnitStrc* pUnit;
	D2UnitStrc* pTarget;
	int32_t nCounter;
};
#pragma pack(pop)


// Inlined helper function
static int16_t AI_GetParamValue(D2GameStrc* pGame, D2AiTickParamStrc* pAiTickParam, int32_t nParamId)
{
	return pAiTickParam->pMonstatsTxt->wAiParam[nParamId][pGame->nDifficulty];
}

static uint32_t AI_RollPercentage(D2UnitStrc* pUnit)
{
	return SEED_RollPercentage(&pUnit->pSeed);
}

// Inlined helper function
static bool AIRollChanceParam(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, int32_t nParamId)
{
	return AI_RollPercentage(pUnit) < AI_GetParamValue(pGame, pAiTickParam, nParamId);
}

// Inlined helper function

static void AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, int nAttack1Or2Param)
{
	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, nAttack1Or2Param))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
	}
	else
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
	}
}

// Inlined
static D2StatListStrc* GetAuraStateStatList(D2UnitStrc* pUnit, D2SkillsTxt* pSkillsTxtRecord)
{
	if (pSkillsTxtRecord->nAuraState > 0 && STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState) != 0)
	{
		return STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
	}
	return nullptr;
}

void __fastcall AITHINK_Fn000(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
}

//D2Game.0x6FCD1660
void __fastcall D2GAME_AI_SpecialState02_6FCD1660(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (sub_6FCF2E70(pUnit) || pAiTickParam->nTargetDistance < 4)
	{
		AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_NONE);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->nTargetDistance > 10 && !pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pAiTickParam->pTarget, 12u);
		return;
	}

	if ((uint8_t)ITEMS_RollRandomNumber(&pUnit->pSeed) < 77u)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
		return;
	}

	if ((uint8_t)ITEMS_RollRandomNumber(&pUnit->pSeed) < 26u)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}


enum D2C_SkeletonAIParams
{
	SKELETON_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	SKELETON_AI_PARAM_STALL_TIME = 1,
	SKELETON_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	SKELETON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT = 3,
};

//D2Game.0x6FCD1750
void __fastcall AITHINK_Fn002_Skeleton(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETON_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, SKELETON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETON_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, (4 | 2 | 1));
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SKELETON_AI_PARAM_STALL_TIME));
}

enum D2C_ZombieAIParams
{
	ZOMBIE_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	ZOMBIE_AI_PARAM_AWARE_DISTANCE = 1,
	// ZOMBIE_AI_PARAM_2 = 2, // unused
	ZOMBIE_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT = 3,
};

//D2Game.0x6FCD1880
void __fastcall AITHINK_Fn003_Zombie(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, ZOMBIE_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
	}
	else
	{
		if (sub_6FCF2E70(pUnit)
			|| pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, ZOMBIE_AI_PARAM_AWARE_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, ZOMBIE_AI_PARAM_APPROACH_CHANCE_PCT)
			|| DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit)) == LEVEL_BURIALGROUNDS)
		{
			AITACTICS_SetVelocity(pUnit, 0, 100, 0);
			AITACTICS_RunToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
		}
	}
}

enum D2C_BigHeadAIParams
{
	BIGHEAD_AI_PARAM_HURT_PCT = 0,
	BIGHEAD_AI_PARAM_CIRCLE_CHANCE_PCT = 1,
	BIGHEAD_AI_PARAM_FIRE_WHILE_HEALTHY_CHANCE_PCT = 2,
	BIGHEAD_AI_PARAM_FIRE_WHILE_HURT_CHANCE_PCT = 3,
};

//D2Game.0x6FCD1990
void __fastcall AITHINK_Fn004_Bighead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nTargetDistance = pAiTickParam->nTargetDistance;
	int32_t bInMeleeRange = 0;

	if (!pAiTickParam->bCombat && sub_6FCF2E70(pUnit))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
		return;
	}

	if (UNITS_GetCurrentLifePercentage(pUnit) >= AI_GetParamValue(pGame, pAiTickParam, BIGHEAD_AI_PARAM_HURT_PCT))
	{
		// Healthy
		if (pAiTickParam->bCombat)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		if (nTargetDistance < 15 && sub_6FCF2CC0(pGame, pUnit, &nTargetDistance, &bInMeleeRange) && AIRollChanceParam(pGame, pUnit, pAiTickParam, BIGHEAD_AI_PARAM_FIRE_WHILE_HEALTHY_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
	}
	else
	{
		// Hurt
		if (nTargetDistance >= 3)
		{
			if (nTargetDistance > 15)
			{
				AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 6u);
				return;
			}

			if (sub_6FCF2CC0(pGame, pUnit, &nTargetDistance, &bInMeleeRange) && AIRollChanceParam(pGame, pUnit, pAiTickParam, BIGHEAD_AI_PARAM_FIRE_WHILE_HURT_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
				return;
			}

			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BIGHEAD_AI_PARAM_CIRCLE_CHANCE_PCT))
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
		}
		else
		{
			AITACTICS_SetVelocity(pUnit, 0, 50, 0);
			if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 5u, 1))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			}
		}
	}
}

enum D2C_BloodHawkAIParams
{
	BLOODHAWK_AI_PARAM_CHARGE_CHANCE_PCT = 0,
	BLOODHAWK_AI_PARAM_WANDER_CHANCE_PCT = 1,
	BLOODHAWK_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	BLOODHAWK_AI_PARAM_RUN_VELOCITY = 3,
	BLOODHAWK_AI_PARAM_CHARGE_VELOCITY = 4,
};

//D2Game.0x6FCD1BA0
void __fastcall AITHINK_Fn005_BloodHawk(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[0] == 1 && pAiTickParam->bCombat)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 0;

	if (pAiTickParam->bCombat)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, BLOODHAWK_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 0, AI_GetParamValue(pGame, pAiTickParam, BLOODHAWK_AI_PARAM_RUN_VELOCITY), 0);

			if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 4u, 1))
			{
				return;
			}
		}

		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BLOODHAWK_AI_PARAM_CHARGE_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 0, AI_GetParamValue(pGame, pAiTickParam, BLOODHAWK_AI_PARAM_CHARGE_VELOCITY), pAiTickParam->nTargetDistance);
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		return;
	}

	if (pAiTickParam->nTargetDistance <= 3)
	{
		AITACTICS_SetVelocity(pUnit, 0, AI_GetParamValue(pGame, pAiTickParam, BLOODHAWK_AI_PARAM_RUN_VELOCITY), 0);

		if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 4u, 1))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}

		return;
	}

	if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, BLOODHAWK_AI_PARAM_WANDER_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 0, 0, 0);
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
	}
	else
	{
		AITACTICS_SetVelocity(pUnit, 0, -50, 0);
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
	}
}

enum D2C_FallenAIParams
{
	FALLEN_AI_PARAM_COMMAND_ATTACK_CHANCE_PCT = 0,
	FALLEN_AI_PARAM_APPROACH_DISTANCE = 1,
	FALLEN_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	FALLEN_AI_PARAM_ATTACK1_OR_2_CHANCE_PCT = 3,
};

//D2Game.0x6FCD1D50
void __fastcall AITHINK_Fn006_Fallen(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pUnit || pUnit->dwAnimMode == MONMODE_DEATH || pUnit->dwAnimMode == MONMODE_DEAD)
	{
		return;
	}

	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	D2ActiveRoomStrc** ppRoomList = nullptr;
	int32_t nNumRooms = 0;
	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	int32_t bBreak = 0;
	for (int32_t j = 0; j < nNumRooms; ++j)
	{
		for (int32_t i = 0; i < 4; ++i)
		{
			D2UnitStrc* pCorpse = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, ppRoomList[j]->nLastDeadGUIDs[i]);
			if (pCorpse && pCorpse->dwAnimMode == MONMODE_DEATH && (uint32_t)AIUTIL_GetDistanceToCoordinates_NoUnitSize(pCorpse, nX, nY) < 15)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 1;
				AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
				AITACTICS_SetVelocity(pUnit, 0, 50, 0);

				if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 12, 1))
				{
					if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 20) < 1)
					{
						SUNIT_AttachSound(pUnit, 17, 0);
					}

					return;
				}

				bBreak = 1;
				break;
			}
		}

		if (bBreak)
		{
			break;
		}
	}

	if (pUnit->dwAnimMode != MONMODE_NEUTRAL)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (!pCurrentAiCmd)
	{
		if (!pAiTickParam->bCombat && sub_6FCF2E70(pUnit))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
			return;
		}

		if (pAiTickParam->nTargetDistance < 15 && AIGENERAL_GetMinionOwner(pUnit) == pUnit && AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLEN_AI_PARAM_COMMAND_ATTACK_CHANCE_PCT))
		{
			D2AiCmdStrc aiCmd = {};
			aiCmd.nCmdParam[0] = 1;
			AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
			AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL2, pAiTickParam->pTarget);
			return;
		}

		if (!pAiTickParam->bCombat)
		{
			if (pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, FALLEN_AI_PARAM_APPROACH_DISTANCE))
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
				return;
			}

			if (AI_RollPercentage(pUnit) < 30)
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
				return;
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			return;
		}

		if (!pAiTickParam->pAiControl->dwAiParam[0])
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLEN_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				if (AI_RollPercentage(pUnit) < 30)
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL2, pAiTickParam->pTarget);
					return;
				}

				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
				return;
			}
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, FALLEN_AI_PARAM_ATTACK1_OR_2_CHANCE_PCT);
		return;
	}

	if (pCurrentAiCmd->nCmdParam[0] != 1)
	{
		AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLEN_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, FALLEN_AI_PARAM_ATTACK1_OR_2_CHANCE_PCT);
		return;
	}

	if (!AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0))
	{
		AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
	}
}

enum D2C_BruteAIParams
{
	//BRUTE_AI_PARAM_UNUSED = 0,
	BRUTE_AI_PARAM_CIRCLE_CHANCE_PCT = 1, // Unused
	BRUTE_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	BRUTE_AI_PARAM_ATTACK1_OR_2_CHANCE_PCT = 3,
};

//D2Game.0x6FCD2220
void __fastcall AITHINK_Fn007_Brute(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BRUTE_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, BRUTE_AI_PARAM_ATTACK1_OR_2_CHANCE_PCT);
		}
		else
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BRUTE_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
		}
		return;
	}

	const int32_t nSpeedMalus = D2Clamp(UNITS_GetCurrentLifePercentage(pUnit), 40, 100);
	AITACTICS_SetVelocity(pUnit, 0, 100 - nSpeedMalus, 0);
	sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 7);
}

enum D2C_SandRaiderAIParams
{
	SANDRAIDER_AI_PARAM_HURT_PCT = 0,
	SANDRAIDER_AI_PARAM_CIRCLE_CHANCE_PCT = 1,
	SANDRAIDER_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	SANDRAIDER_AI_PARAM_APPROACH = 3,
	SANDRAIDER_AI_PARAM_CHARGE_DURATION = 4,
	SANDRAIDER_AI_PARAM_CHARGE_COLOR = 5,
	SANDRAIDER_AI_PARAM_ATTACK2_OR_1_CHANCE_PCT = 6,
};

//D2Game.0x6FCD2370
void __fastcall AITHINK_Fn008_SandRaider(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		STATES_ToggleState(pUnit, STATE_BLUE, 0);
		STATES_ToggleState(pUnit, STATE_RED, 0);
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
	}

	++pAiTickParam->pAiControl->dwAiParam[0];

	const int nChargeColor = AI_GetParamValue(pGame, pAiTickParam, SANDRAIDER_AI_PARAM_CHARGE_COLOR);
	if (pAiTickParam->pAiControl->dwAiParam[0] == AI_GetParamValue(pGame, pAiTickParam, SANDRAIDER_AI_PARAM_CHARGE_DURATION))
	{
		UNITS_SetOverlay(pUnit, nChargeColor == 1 ? 150 : 46, 0);

		AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiTickParam->pMonstatsTxt->nAIdel[pGame->nDifficulty] + 1);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] > AI_GetParamValue(pGame, pAiTickParam, SANDRAIDER_AI_PARAM_CHARGE_DURATION))
	{
		STATES_ToggleState(pUnit, nChargeColor == 1 ? STATE_BLUE : STATE_RED, 1);

		pAiTickParam->pAiControl->dwAiParam[1] = 1;
	}

	if (pAiTickParam->pAiControl->dwAiParam[2] < 7 && UNITS_GetCurrentLifePercentage(pUnit) < AI_GetParamValue(pGame, pAiTickParam, SANDRAIDER_AI_PARAM_HURT_PCT))
	{
		D2SandRaiderAiCallbackArgStrc arg = {};
		arg.pTarget = nullptr;
		arg.nDistance = INT_MAX;
		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_SandRaider, 1);

		if (arg.pTarget)
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, arg.pTarget, 0);
			return;
		}

		++pAiTickParam->pAiControl->dwAiParam[2];
	}

	if (pAiTickParam->nTargetDistance > 4 && !pAiTickParam->pAiControl->dwAiParam[1] && AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDRAIDER_AI_PARAM_CIRCLE_CHANCE_PCT))
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pAiControl->dwAiParam[1] == 1)
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				return;
			}
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDRAIDER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDRAIDER_AI_PARAM_ATTACK2_OR_1_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			return;
		}
	}
	else
	{
		if (pAiTickParam->pAiControl->dwAiParam[1] || AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDRAIDER_AI_PARAM_APPROACH))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
			return;
		}
	}

	const int32_t nChargeDuration = AI_GetParamValue(pGame, pAiTickParam, SANDRAIDER_AI_PARAM_CHARGE_DURATION);
	const int32_t nParam = std::max(24 - nChargeDuration, 6);
	if (pAiTickParam->pAiControl->dwAiParam[0] > nParam + nChargeDuration)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

//D2Game.0x6FCD2680
D2UnitStrc* __fastcall AITHINK_TargetCallback_SandRaider(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2SandRaiderAiCallbackArgStrc* pArg = (D2SandRaiderAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL || MONSTERS_IsDead(pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);
	if (nDistance >= pArg->nDistance)
	{
		return nullptr;
	}

	pArg->pTarget = pTarget;
	pArg->nDistance = nDistance;
	return nullptr;
}

enum D2C_WraitAIParams
{
	WRAITH_AI_PARAM_APPROACH = 0,
	WRAITH_AI_PARAM_STALL_DURATION = 1,
	WRAITH_AI_PARAM_ATTACK_CHANCE_PCT = 2,
};

//D2Game.0x6FCD27A0
void __fastcall AITHINK_Fn009_Wraith(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, WRAITH_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, WRAITH_AI_PARAM_APPROACH))
		{
			AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 12, 0);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, WRAITH_AI_PARAM_STALL_DURATION));
}

enum D2C_CorruptRogueAIParams
{
	CORRUPTROGUE_AI_PARAM_APPROACH = 0,
	CORRUPTROGUE_AI_PARAM_STALL_DURATION = 1,
	CORRUPTROGUE_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	CORRUPTROGUE_AI_PARAM_RUN_VELOCITY = 3,
	CORRUPTROGUE_AI_PARAM_RUN_CHANCE_PCT = 4,
};

//D2Game.0x6FCD2850
void __fastcall AITHINK_Fn010_CorruptRogue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2PlayerCountBonusStrc playerCountBonus = {};
	MONSTER_GetPlayerCountBonus(pGame, &playerCountBonus, UNITS_GetRoom(pUnit), pUnit);

	if (pAiTickParam->nTargetDistance <= 20 - 3 * playerCountBonus.nDifficulty)
	{
		if (pAiTickParam->bCombat)
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTROGUE_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CORRUPTROGUE_AI_PARAM_STALL_DURATION));
			}
			return;
		}
		else
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTROGUE_AI_PARAM_APPROACH))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CORRUPTROGUE_AI_PARAM_STALL_DURATION));
				return;
			}
			else if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTROGUE_AI_PARAM_RUN_CHANCE_PCT))
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
				return;
			}
		}
	}

	AITACTICS_SetVelocity(pUnit, 13, AI_GetParamValue(pGame, pAiTickParam, CORRUPTROGUE_AI_PARAM_RUN_VELOCITY), 0);
	AITACTICS_RunToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 3u);
}

enum D2C_BaboonAIParams
{
	BABOON_AI_PARAM_HURT_PCT = 0,
	BABOON_AI_PARAM_CIRCLE_CHANCE_PCT = 1,
	BABOON_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	BABOON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT = 3,
	BABOON_AI_PARAM_REGEN_BONUS = 4,
};

//D2Game.0x6FCD2A00
void __fastcall AITHINK_Fn011_Baboon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nVel = 0;

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);

	if (pAiTickParam->pMonstatsTxt->nVelocity > 0)
	{
		const int32_t nRatio = 100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity;
		if (nRatio >= 100)
		{
			nVel = std::min(nRatio - 100, 120);
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[1] = 0;

		--pAiTickParam->pAiControl->dwAiParam[0];

		if (!pAiTickParam->pAiControl->dwAiParam[0] || nLifePercentage > 75)
		{
			STATLIST_SetUnitStat(pUnit, STAT_HPREGEN, STATLIST_UnitGetStatValue(pUnit, STAT_HPREGEN, 0) - pAiTickParam->pAiControl->dwAiParam[2], 0);
		}

		if (pAiTickParam->bCombat)
		{
			if (AI_RollPercentage(pUnit) < 33)
			{
				AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, BABOON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
				return;
			}
		}
		else
		{
			if (nLifePercentage > 75)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				AITACTICS_SetVelocity(pUnit, 13, nVel, 0);
				sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 7);
				return;
			}
		}

		if (pAiTickParam->nTargetDistance >= 24 && !sub_6FCF2E70(pUnit))
		{
			if (AI_RollPercentage(pUnit) < 33)
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
			return;
		}

		AITACTICS_SetVelocity(pUnit, 2, nVel, 0);

		if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 15, 1))
		{
			if (!pAiTickParam->bCombat)
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
				return;
			}

			AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, BABOON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
		}
		return;
	}

	if (!pAiTickParam->bCombat)
	{
		sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (sub_6FCF2E70(pUnit))
	{
		if (nLifePercentage < AI_GetParamValue(pGame, pAiTickParam, BABOON_AI_PARAM_HURT_PCT))
		{
			if (AI_RollPercentage(pUnit) < 50)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = ITEMS_RollRandomNumber(&pUnit->pSeed) % 5u + 2;

				const int32_t nHpRegen = STATLIST_UnitGetStatValue(pUnit, STAT_HPREGEN, 0);
				if (nHpRegen)
				{
					pAiTickParam->pAiControl->dwAiParam[2] = (nHpRegen * AI_GetParamValue(pGame, pAiTickParam, BABOON_AI_PARAM_REGEN_BONUS)) / 8;
					STATLIST_SetUnitStat(pUnit, STAT_HPREGEN, nHpRegen + pAiTickParam->pAiControl->dwAiParam[2], 0);
				}
				else
				{
					pAiTickParam->pAiControl->dwAiParam[2] = 0;
				}

				AITACTICS_SetVelocity(pUnit, 2, nVel, 0);
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 0xFu, 0);
				return;
			}
		}

		if (!pAiTickParam->pAiControl->dwAiParam[1])
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 1;
			AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, BABOON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
			return;
		}

		if (AI_RollPercentage(pUnit) < 20)
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
			return;
		}
	}

	if (!pAiTickParam->pAiControl->dwAiParam[1] || (AIRollChanceParam(pGame, pUnit, pAiTickParam, BABOON_AI_PARAM_ATTACK_CHANCE_PCT)))
	{
		pAiTickParam->pAiControl->dwAiParam[1] = 1;
		AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, BABOON_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BABOON_AI_PARAM_CIRCLE_CHANCE_PCT))
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

enum D2C_QuillRatAIParams
{
	QUILLRAT_AI_PARAM_ACTIVATE_DISTANCE = 0,
	QUILLRAT_AI_PARAM_SHOOT_CHANCE_PCT = 1,
	//QUILLRAT_AI_PARAM_UNUSED = 2,
	QUILLRAT_AI_PARAM_WALK_DISTANCE = 3,
};

//D2Game.0x6FCD2E80
void __fastcall AITHINK_Fn014_QuillRat(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (pCurrentAiCmd)
	{
		if (SUNIT_GetServerUnit(pGame, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
			return;
		}

		AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
	}

	if (pAiTickParam->bCombat)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (sub_6FCF2E70(pUnit))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
		return;
	}

	if (pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, QUILLRAT_AI_PARAM_ACTIVATE_DISTANCE))
	{
		const int nWalkDist = AI_GetParamValue(pGame, pAiTickParam, QUILLRAT_AI_PARAM_WALK_DISTANCE);
		if (nWalkDist >= 3)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, nWalkDist);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 3);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, QUILLRAT_AI_PARAM_SHOOT_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
		return;
	}

	const int nWalkDist = AI_GetParamValue(pGame, pAiTickParam, QUILLRAT_AI_PARAM_WALK_DISTANCE);
	if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, nWalkDist, 1))
	{
		if (pAiTickParam->nTargetDistance < 4)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			return;
		}

		if (nWalkDist >= 3)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, nWalkDist);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 3);
		}
	}
}

enum D2C_FallenShamanAIParams
{
	FALLENSHAMAN_AI_PARAM_RESURRECT_AND_COMMAND_CHANCE_PCT = 0,
	FALLENSHAMAN_AI_PARAM_SHOOT_CHANCE_PCT = 1,
	FALLENSHAMAN_AI_PARAM_MELEE_AND_CIRCLE_CHANCE_PCT = 2,
	FALLENSHAMAN_AI_PARAM_RESURRECT_DISTANCE = 3,
	FALLENSHAMAN_AI_PARAM_SHOOT_DISTANCE = 4,
};

//D2Game.0x6FCD2FF0
void __fastcall AITHINK_Fn013_FallenShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;

	if (pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLENSHAMAN_AI_PARAM_MELEE_AND_CIRCLE_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	D2FallenShamanAiCallbackArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nCounter = 0;
	arg.nMaxDistance = AI_GetParamValue(pGame, pAiTickParam, FALLENSHAMAN_AI_PARAM_RESURRECT_DISTANCE) * AI_GetParamValue(pGame, pAiTickParam, FALLENSHAMAN_AI_PARAM_RESURRECT_DISTANCE);

	if (!MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE) || MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_CHAMPION))
	{
		sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 9);
	}
	else
	{
		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_FallenShaman, 1);
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLENSHAMAN_AI_PARAM_RESURRECT_AND_COMMAND_CHANCE_PCT))
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 1;
		AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
	}

	if (arg.pTarget && arg.nCounter && AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLENSHAMAN_AI_PARAM_RESURRECT_AND_COMMAND_CHANCE_PCT) && sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], arg.pTarget, 0, 0))
	{
		AITACTICS_UseSequenceSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], arg.pTarget, 0, 0);
		return;
	}

	if (pAiTickParam->pTarget && nDistance < AI_GetParamValue(pGame, pAiTickParam, FALLENSHAMAN_AI_PARAM_SHOOT_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLENSHAMAN_AI_PARAM_SHOOT_CHANCE_PCT))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
		return;
	}

	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	if (pTarget && nDistance < AI_GetParamValue(pGame, pAiTickParam, FALLENSHAMAN_AI_PARAM_SHOOT_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLENSHAMAN_AI_PARAM_SHOOT_CHANCE_PCT))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pTarget, 0, 0);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FALLENSHAMAN_AI_PARAM_MELEE_AND_CIRCLE_CHANCE_PCT))
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCD32E0
D2UnitStrc* __fastcall AITHINK_TargetCallback_FallenShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2FallenShamanAiCallbackArgStrc* pArg = (D2FallenShamanAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER)
	{
		return nullptr;
	}

	int32_t nClassId = pTarget->dwClassId;
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
	if (pMonStatsTxtRecord)
	{
		nClassId = pMonStatsTxtRecord->nBaseId;
	}

	if ((nClassId != MONSTER_FALLEN1 && nClassId != MONSTER_FALLENSHAMAN1)
		|| STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL
		|| !(pTarget->dwFlags & UNITFLAG_TARGETABLE)
		|| STATES_CheckStateMaskUdeadOnUnit(pTarget)
		|| pTarget->dwAnimMode != MONMODE_DEAD
		|| STATES_CheckStateMaskHideOnUnit(pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);
	if (nDistance > pArg->nMaxDistance || MONSTERUNIQUE_CheckMonTypeFlag(pTarget, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_CHAMPION))
	{
		return nullptr;
	}

	pArg->pTarget = pTarget;
	pArg->nDistance = nDistance;
	++pArg->nCounter;
	return nullptr;
}

enum D2C_SandMaggotAIParams
{
	SANDMAGGOT_AI_PARAM_LAY_CHANCE_PCT = 0,
	SANDMAGGOT_AI_PARAM_SPIT_CHANCE_PCT = 1,
	SANDMAGGOT_AI_PARAM_NUMBER_OF_EGGS = 2,
	SANDMAGGOT_AI_PARAM_MELEE_CHANCE_PCT = 3,
	SANDMAGGOT_AI_PARAM_MIN_UP_DOWN_TIME = 4,
};

//D2Game.0x6FCD34A0
void __fastcall D2GAME_AI_Unk015_6FCD34A0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 14, 0);
	if (pAiCmd->nCmdParam[4] == 1 && pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], SUNIT_GetTargetUnit(pGame, pUnit), 0, 0);
		sub_6FCD0150(pGame, pUnit, 30);
		pAiCmd->nCmdParam[4] = 0;
		return;
	}

	const int32_t nParam = pAiTickParam->pAiControl->dwAiParam[2];
	AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, pAiTickParam->pAiControl->nAiSpecialState);
	pAiTickParam->pAiControl->dwAiParam[2] = nParam;
	sub_6FCD0150(pGame, pUnit, 1);
}

//D2Game.0x6FCD3540
void __fastcall AITHINK_Fn015_SandMaggot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	const int32_t nParam = pAiTickParam->pAiControl->dwAiParam[0];

	if (nParam >= 3)
	{
		if (!pAiTickParam->pTarget)
		{
			if (!pTarget || nDistance >= 16)
			{
				if (nParam == 3)
				{
					sub_6FCD0150(pGame, pUnit, 20);
					return;
				}
			}
		}

		if (nParam == 3)
		{
			if (pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1])
			{
				if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
					sub_6FCD0150(pGame, pUnit, 25);
					pAiTickParam->pAiControl->dwAiParam[0] = 1;
					pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOT_AI_PARAM_MIN_UP_DOWN_TIME);
					return;
				}
			}

			sub_6FCD0150(pGame, pUnit, 20);
			return;
		}
	}
	else
	{
		if (!pAiTickParam->pTarget)
		{
			if ((!pTarget || nDistance > 10) && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && pAiTickParam->pMonstatsTxt->nSkill[1] >= 0)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], 0, 0, 0);
				sub_6FCD0150(pGame, pUnit, 30);
				pAiTickParam->pAiControl->dwAiParam[0] = 3;
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOT_AI_PARAM_MIN_UP_DOWN_TIME);
				return;
			}
		}
	}

	if (UNITS_GetCurrentLifePercentage(pUnit) < 25 && pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && nDistance < 7 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && AI_RollPercentage(pUnit) < 20)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
		sub_6FCD0150(pGame, pUnit, 30);
		pAiTickParam->pAiControl->dwAiParam[0] = 3;
		pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOT_AI_PARAM_MIN_UP_DOWN_TIME);
		return;
	}

	if (pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDMAGGOT_AI_PARAM_MELEE_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (pTarget && nDistance < 15 && AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDMAGGOT_AI_PARAM_SPIT_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pTarget);
		return;
	}

	if (AI_RollPercentage(pUnit) < 20)
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 6u, 0);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[2] >= AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOT_AI_PARAM_NUMBER_OF_EGGS) || !AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDMAGGOT_AI_PARAM_LAY_CHANCE_PCT))
	{
		sub_6FCD0150(pGame, pUnit, 12);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] == 2 && pAiTickParam->pMonstatsTxt->nSkill[2] >= 0)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		++pAiTickParam->pAiControl->dwAiParam[2];
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
		sub_6FCD0150(pGame, pUnit, 20);
		return;
	}

	sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 6u, 0);
	pAiTickParam->pAiControl->dwAiParam[0] = 2;
}

enum D2C_ClawViperAIParams
{
	CLAWVIPER_AI_PARAM_CHARGE_CHANCE_PCT = 0,
	CLAWVIPER_AI_PARAM_CHARGE_DISTANCE = 1,
	CLAWVIPER_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	CLAWVIPER_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT = 3,
	CLAWVIPER_AI_PARAM_STALL_DURATION = 4,
	CLAWVIPER_AI_PARAM_CHARGE_COLOR = 5,
};

//D2Game.0x6FCD3900
void __fastcall AITHINK_Fn016_ClawViper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int nChargeColor = AI_GetParamValue(pGame, pAiTickParam, CLAWVIPER_AI_PARAM_CHARGE_COLOR);
	if (pAiTickParam->pAiControl->dwAiParam[0] && nChargeColor != 0)
	{
		STATES_ToggleState(pUnit, nChargeColor == 2 ? STATE_RED : STATE_BLUE, 0);
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CLAWVIPER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, CLAWVIPER_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CLAWVIPER_AI_PARAM_STALL_DURATION));
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, CLAWVIPER_AI_PARAM_CHARGE_DISTANCE)
		&& AIRollChanceParam(pGame, pUnit, pAiTickParam, CLAWVIPER_AI_PARAM_CHARGE_CHANCE_PCT)
		&& sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, CLIENTS_GetUnitX(pAiTickParam->pTarget), CLIENTS_GetUnitY(pAiTickParam->pTarget)))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);

		if (nChargeColor != 0)
		{
			STATES_ToggleState(pUnit, nChargeColor == 2 ? STATE_RED : STATE_BLUE, 0);
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		return;
	}

	if (AI_RollPercentage(pUnit) < 50)
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CLAWVIPER_AI_PARAM_STALL_DURATION));
}

enum D2C_ClawViperExAIParams
{
	CLAWVIPEREX_AI_PARAM_CHARGE_CHANCE_PCT = 0,
	CLAWVIPEREX_AI_PARAM_CHARGE_DISTANCE = 1,
	CLAWVIPEREX_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	CLAWVIPEREX_AI_PARAM_SHOOT_CHANCE_PCT = 3,
	CLAWVIPEREX_AI_PARAM_STALL_DURATION = 4,
	CLAWVIPEREX_AI_PARAM_CHARGE_COLOR = 5,
	CLAWVIPEREX_AI_PARAM_SHOOT_DISTANCE = 6,
	CLAWVIPEREX_AI_PARAM_SHOOT_TIMER = 7,
};

//D2Game.0x6FCD3B90
void __fastcall AITHINK_Fn142_ClawViperEx(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int nChargeColor = AI_GetParamValue(pGame, pAiTickParam, CLAWVIPER_AI_PARAM_CHARGE_COLOR);
	if (pAiTickParam->pAiControl->dwAiParam[0] && nChargeColor != 0)
	{
		STATES_ToggleState(pUnit, nChargeColor == 2 ? STATE_RED : STATE_BLUE, 0);
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CLAWVIPEREX_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CLAWVIPEREX_AI_PARAM_STALL_DURATION));
		return;
	}

	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, CLAWVIPEREX_AI_PARAM_SHOOT_DISTANCE))
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CLAWVIPEREX_AI_PARAM_SHOOT_CHANCE_PCT))
		{
			if (pAiTickParam->pAiControl->dwAiParam[1] < pGame->dwGameFrame)
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, CLAWVIPEREX_AI_PARAM_SHOOT_TIMER);
				return;
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CLAWVIPEREX_AI_PARAM_STALL_DURATION));
			return;
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 
		&& pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, CLAWVIPEREX_AI_PARAM_CHARGE_DISTANCE) 
		&& AIRollChanceParam(pGame, pUnit, pAiTickParam, CLAWVIPEREX_AI_PARAM_CHARGE_CHANCE_PCT)
		&& sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, CLIENTS_GetUnitX(pAiTickParam->pTarget), CLIENTS_GetUnitY(pAiTickParam->pTarget)))
	{

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);

		if (nChargeColor != 0)
		{
			STATES_ToggleState(pUnit, nChargeColor == 2 ? STATE_RED : STATE_BLUE, 0);
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 1;
	}
	else
	{

		if (AI_RollPercentage(pUnit) >= 50)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CLAWVIPEREX_AI_PARAM_STALL_DURATION));
		}
		else
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		}
	}
}

enum D2C_SandLeaperAIParams
{
	SANDLEAPER_AI_PARAM_LEAP_CHANCE_PCT = 0,
	SANDLEAPER_AI_PARAM_ATTACK_CHANCE_PCT = 1,
	SANDLEAPER_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	SANDLEAPER_AI_PARAM_CIRCLE_CHANCE_PCT = 3,
};

//D2Game.0x6FCD3E70
void __fastcall AITHINK_Fn017_SandLeaper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance < 5
		&& AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDLEAPER_AI_PARAM_LEAP_CHANCE_PCT)
		&& sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDLEAPER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (pAiTickParam->nTargetDistance > 10)
		{
			AITACTICS_SetVelocity(pUnit, 0, 75, 0);
			D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pAiTickParam->pTarget, 5u);
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDLEAPER_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SANDLEAPER_AI_PARAM_CIRCLE_CHANCE_PCT))
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

enum D2C_PantherWomanAIParams
{
	PANTHERWOMAN_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	PANTHERWOMAN_AI_PARAM_ATTACK_CHANCE_PCT = 1,
	PANTHERWOMAN_AI_PARAM_PACK_DISTANCE = 2,
	PANTHERWOMAN_AI_PARAM_STALL_DURATION = 3,
	PANTHERWOMAN_AI_PARAM_REGROUP_FROM_MELEE = 4, // unused
};

//D2Game.0x6FCD4050
void __fastcall AITHINK_Fn018_PantherWoman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, PANTHERWOMAN_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, PANTHERWOMAN_AI_PARAM_STALL_DURATION));
		}
	}
	else if (AIRollChanceParam(pGame, pUnit, pAiTickParam, PANTHERWOMAN_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 0, 75, 0);
		sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 7);
	}
	else
	{
		D2PantherAiCallbackArgStrc arg = {};
		arg.pTarget = nullptr;
		arg.nDistance = INT_MAX;
		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Panther, 1);

		if (arg.pTarget && arg.nDistance > AI_GetParamValue(pGame, pAiTickParam, PANTHERWOMAN_AI_PARAM_PACK_DISTANCE) * AI_GetParamValue(pGame, pAiTickParam, PANTHERWOMAN_AI_PARAM_PACK_DISTANCE))
		{
			AITACTICS_SetVelocity(pUnit, 0, 75, 0);
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, arg.pTarget, 7);
		}
		else
		{
			if (AI_RollPercentage(pUnit) >= 25)
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, PANTHERWOMAN_AI_PARAM_STALL_DURATION));
			}
			else
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
			}
		}
	}
}

//D2Game.0x6FCD41F0
D2UnitStrc* __fastcall AITHINK_TargetCallback_Panther(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2PantherAiCallbackArgStrc* pArg = (D2PantherAiCallbackArgStrc*)pCallbackArg;

	if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER || pUnit == pTarget)
	{
		return nullptr;
	}

	int32_t nBaseId = -1;
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
	{
		nBaseId = pUnit->dwClassId;
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
		if (pMonStatsTxtRecord)
		{
			nBaseId = pMonStatsTxtRecord->nBaseId;
		}
	}

	int32_t nTargetBaseId = pTarget->dwClassId;
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nTargetBaseId);
	if (pMonStatsTxtRecord)
	{
		nTargetBaseId = pMonStatsTxtRecord->nBaseId;
	}

	if (nBaseId != nTargetBaseId || MONSTERS_IsDead(pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);
	if (nDistance >= pArg->nDistance)
	{
		return nullptr;
	}

	pArg->pTarget = pTarget;
	pArg->nDistance = nDistance;
	return nullptr;
}

enum D2C_Goatman_SwarmAIParams
{
	GOATMAN_SWARM_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	GOATMAN_SWARM_AI_PARAM_STALL_DURATION = 1,
	GOATMAN_SWARM_AI_PARAM_ATTACK_CHANCE_PCT = 2,
};

//D2Game.0x6FCD4390
void __fastcall AITHINK_Fn012_019_Goatman_Swarm(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, GOATMAN_SWARM_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, GOATMAN_SWARM_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, GOATMAN_SWARM_AI_PARAM_STALL_DURATION));
}

enum D2C_ScarabAIParams
{
	SCARAB_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	SCARAB_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT = 1,
	SCARAB_AI_PARAM_STALL_DURATION = 2,
	SCARAB_AI_PARAM_JAB_CHANCE_PCT = 3,
	SCARAB_AI_PARAM_COMMAND_CHANCE_PCT = 4,
};

//D2Game.0x6FCD4440
void __fastcall AITHINK_Fn020_Scarab(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (!pCurrentAiCmd && pAiTickParam->nTargetDistance < 20 && AIGENERAL_GetMinionOwner(pUnit) == pUnit && AIRollChanceParam(pGame, pUnit, pAiTickParam, SCARAB_AI_PARAM_COMMAND_CHANCE_PCT))
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 1;
		AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
		pCurrentAiCmd = &aiCmd;
	}

	if (pCurrentAiCmd && pCurrentAiCmd->nCmdParam[0] == 1)
	{
		if (pAiTickParam->bCombat && pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
		{
			AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		}
		else
		{
			AITACTICS_SetVelocity(pUnit, 2, 100, 0);
			if (!AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0))
			{
				AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
			}
		}
		return;
	}

	if (!pAiTickParam->bCombat)
	{
		if (pAiTickParam->pAiControl->dwAiParam[0])
		{
			AITACTICS_SetVelocity(pUnit, 2, 0, 4u);
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);

			if (AI_RollPercentage(pUnit) > 10)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
			}
		}
		else
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SCARAB_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, SCARAB_AI_PARAM_JAB_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, SCARAB_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SCARAB_AI_PARAM_STALL_DURATION));
}

enum D2C_MummyAIParams
{
	MUMMY_AI_PARAM_AWAKE_DISTANCE = 0,
	MUMMY_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	MUMMY_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	MUMMY_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT = 3,
	MUMMY_AI_PARAM_STALL_DURATION = 4,
};

//D2Game.0x6FCD4720
void __fastcall AITHINK_Fn021_Mummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (sub_6FCF2E70(pUnit) && !pAiTickParam->bCombat)
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, MUMMY_AI_PARAM_AWAKE_DISTANCE))
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, MUMMY_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MUMMY_AI_PARAM_STALL_DURATION));
		}
		return;
	}

	if (!pAiTickParam->bCombat)
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, MUMMY_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MUMMY_AI_PARAM_STALL_DURATION));
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	AITACTICS_ChangeModeAndTargetUnitToAttack1Or2(pGame, pUnit, pAiTickParam, MUMMY_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT);
}

enum D2C_GreatMummyAIParams
{
	GREATMUMMY_AI_PARAM_MELEE_BREATHE_CHANCE_PCT = 0,
	GREATMUMMY_AI_PARAM_RAISE_CHANCE_PCT = 1,
	GREATMUMMY_AI_PARAM_HEAL_CHANCE_PCT = 2,
	GREATMUMMY_AI_PARAM_SHOOT_CHANCE_PCT = 3,
	GREATMUMMY_AI_PARAM_RAISE_RANGE = 4,
};

//D2Game.0x6FCD48B0
void __fastcall AITHINK_Fn022_GreaterMummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, GREATMUMMY_AI_PARAM_MELEE_BREATHE_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	int32_t nDistance = pAiTickParam->nTargetDistance;
	if (nDistance < 5 && AIRollChanceParam(pGame, pUnit, pAiTickParam, GREATMUMMY_AI_PARAM_MELEE_BREATHE_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
		return;
	}

	D2GreaterMummyAiCallbackArgStrc arg = {};
	arg.nMaxDistance = AI_GetParamValue(pGame, pAiTickParam, GREATMUMMY_AI_PARAM_RAISE_RANGE) * AI_GetParamValue(pGame, pAiTickParam, GREATMUMMY_AI_PARAM_RAISE_RANGE);

	if (pUnit && pUnit->dwClassId == MONSTER_RADAMENT)
	{
		ACT2Q1_OnRadamentActivated(pGame, pUnit);

		arg.bRadament = 1;
		arg.nMaxDistance = (AI_GetParamValue(pGame, pAiTickParam, GREATMUMMY_AI_PARAM_RAISE_RANGE) + 10) * (AI_GetParamValue(pGame, pAiTickParam, GREATMUMMY_AI_PARAM_RAISE_RANGE) + 10);
		if (pGame->nDifficulty == DIFFMODE_NORMAL)
		{
			arg.bNormal = 1;
		}
	}

	sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_GreaterMummy, 1);
	
	D2UnitStrc* pHealTarget = arg.pHealTarget;
	if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && pHealTarget && AIRollChanceParam(pGame, pUnit, pAiTickParam, GREATMUMMY_AI_PARAM_HEAL_CHANCE_PCT))
	{
		UNITS_SetTargetUnitForDynamicUnit(pUnit, pHealTarget);
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pHealTarget, 0, 0);
		return;
	}

	D2UnitStrc* pReviveTarget = arg.pReviveTarget;
	if (pReviveTarget && AIRollChanceParam(pGame, pUnit, pAiTickParam, GREATMUMMY_AI_PARAM_RAISE_CHANCE_PCT) && sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pReviveTarget, 0, 0))
	{
		UNITS_SetTargetUnitForDynamicUnit(pUnit, pReviveTarget);
		AITACTICS_UseSequenceSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pReviveTarget, 0, 0);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, GREATMUMMY_AI_PARAM_SHOOT_CHANCE_PCT))
	{
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
		if (pTarget)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pTarget, 0, 0);
			return;
		}
	}

	if (arg.nCounter <= 0)
	{
		AITACTICS_SetVelocity(pUnit, 0, 50, 0);
		AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 3u);
		return;
	}

	if (AI_RollPercentage(pUnit) < 50)
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 6);
}

//D2Game.0x6FCD4C10
D2UnitStrc* __fastcall AITHINK_TargetCallback_GreaterMummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2GreaterMummyAiCallbackArgStrc* pArg = (D2GreaterMummyAiCallbackArgStrc*)pCallbackArg;

	if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER || pUnit == pTarget || !STATLIST_AreUnitsAligned(pUnit, pTarget) || STATLIST_GetUnitAlignment(pTarget) == UNIT_ALIGNMENT_GOOD || !(pTarget->dwFlags & UNITFLAG_TARGETABLE) || STATES_CheckStateMaskUdeadOnUnit(pTarget))
	{
		return 0;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);

	if (!pArg->bRadament)
	{
		if (!(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_LUNDEAD]))
		{
			return 0;
		}
	}
	else
	{
		if (!MONSTERS_IsUndead(pTarget))
		{
			return 0;
		}
	}

	if ((!pArg->bNormal && MONSTERUNIQUE_CheckMonTypeFlag(pTarget, MONTYPEFLAG_UNIQUE)) || AITHINK_GetSquaredDistance(pUnit, pTarget) > pArg->nMaxDistance)
	{
		return 0;
	}

	++pArg->nCounter;
	if (pTarget->dwAnimMode == MONMODE_DEAD && !STATES_CheckStateMaskHideOnUnit(pTarget))
	{
		pArg->pReviveTarget = pTarget;
		++pArg->nHealReviveCounter;
		return 0;
	}

	const int32_t nHitpoints = STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0);
	if (STATLIST_GetMaxLifeFromUnit(pTarget) != nHitpoints && pTarget->dwAnimMode != MONMODE_DEATH)
	{
		pArg->pHealTarget = pTarget;
		++pArg->nHealReviveCounter;
	}

	return 0;
}

enum D2C_VultureAIParams
{
	VULTURE_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	VULTURE_AI_PARAM_STALL_DURATION = 1,
	VULTURE_AI_PARAM_WOUNDED_PCT = 2,
	VULTURE_AI_PARAM_CIRCLE_CHANCE_PCT = 3,
	VULTURE_AI_PARAM_MOVE_CHANCE_PCT = 4,
};

//D2Game.0x6FCD4DD0
void __fastcall AITHINK_Fn023_Vulture(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nParam = pAiTickParam->pAiControl->dwAiParam[0];

	if (!pAiTickParam->pTarget)
	{
		if (pAiTickParam->pAiControl->dwAiParam[0] < 1)
		{
			AITACTICS_Idle(pGame, pUnit, 12);
			return;
		}

		if (sub_6FCD55D0(pGame, pUnit))
		{
			AITACTICS_MoveInRadiusToTarget(pGame, pUnit, pUnit, MONMODE_SKILL2, 2, 3);
			AITACTICS_Idle(pGame, pUnit, 12);
			pAiTickParam->pAiControl->dwAiParam[0] = -1;
			return;
		}
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pAiTickParam->pTarget);

	if (pAiTickParam->pTarget && UNITS_GetRoom(pAiTickParam->pTarget) != UNITS_GetRoom(pUnit) && nDistance > 144)
	{
		if (nParam < 1)
		{
			AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 9, 0);
			return;
		}

		if (sub_6FCD55D0(pGame, pUnit))
		{
			AITACTICS_MoveInRadiusToTarget(pGame, pUnit, pUnit, MONMODE_SKILL2, 2, 3);
			AITACTICS_Idle(pGame, pUnit, 12);
			pAiTickParam->pAiControl->dwAiParam[0] = -1;
			return;
		}
	}

	if (!nParam && !AIGENERAL_GetMinionOwner(pUnit) && pAiTickParam->pTarget && nDistance > 144 && AI_RollPercentage(pUnit) < 60)
	{
		pUnit->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);
		COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_NO_PATH);
		COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_MONSTER);

		PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
		PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
		AITACTICS_Idle(pGame, pUnit, 12);

		pAiTickParam->pAiControl->dwAiParam[0] = (ITEMS_RollRandomNumber(&pUnit->pSeed) & 7) + 24;
		AITACTICS_MoveInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, MONMODE_SKILL1, 8, 8);
		AITACTICS_Idle(pGame, pUnit, 12);
		return;
	}

	D2VultureAiCallbackArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nDistance = 121;
	arg.nLifePercentage = AI_GetParamValue(pGame, pAiTickParam, VULTURE_AI_PARAM_WOUNDED_PCT);
	sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Vulture, 1);

	if (nParam > 1)
	{
		if (!arg.pTarget && (pAiTickParam->nTargetDistance >= 6 || AI_RollPercentage(pUnit) >= 15))
		{
			pUnit->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);
			COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_NO_PATH);
			COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_MONSTER);

			PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
			PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
			AITACTICS_Idle(pGame, pUnit, 12);

			const int32_t nCurrentX = CLIENTS_GetUnitX(pUnit);
			const int32_t nCurrentY = CLIENTS_GetUnitY(pUnit);

			if (pAiTickParam->pAiControl->dwAiParam[1] && pAiTickParam->pAiControl->dwAiParam[2] && (uint32_t)AIUTIL_GetDistanceToCoordinates(pUnit, pAiTickParam->pAiControl->dwAiParam[1], pAiTickParam->pAiControl->dwAiParam[2]) > 1)
			{
				D2CoordStrc coord = {};
				coord.nX = pAiTickParam->pAiControl->dwAiParam[1];
				coord.nY = pAiTickParam->pAiControl->dwAiParam[2];

				if (pAiTickParam->pTarget && COLLISION_GetFreeCoordinates(UNITS_GetRoom(pAiTickParam->pTarget), &coord, UNITS_GetUnitSizeX(pUnit), 0xFFFF, 1) || COLLISION_GetFreeCoordinates(UNITS_GetRoom(pUnit), &coord, UNITS_GetUnitSizeX(pUnit), 0xFFFF, 1))
				{
					pAiTickParam->pAiControl->dwAiParam[1] = coord.nX;
					pAiTickParam->pAiControl->dwAiParam[2] = coord.nY;
				}

				--pAiTickParam->pAiControl->dwAiParam[0];
				AITACTICS_SetVelocity(pUnit, 7, 0, 0);
				AITACTICS_ChangeModeAndTargetCoordinatesOneStep(pGame, pUnit, pAiTickParam->pAiControl->dwAiParam[1], pAiTickParam->pAiControl->dwAiParam[2], 8);
				AITACTICS_Idle(pGame, pUnit, 12);
				return;
			}

			int32_t nMax = 2 * (nParam + 8);
			int32_t nX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMax) - (nParam + 8);
			int32_t nY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMax) - (nParam + 8);
			nMax = D2Clamp(nMax, 12, 36);

			while (AIUTIL_GetDistanceToCoordinates(pUnit, nX, nY) < nMax)
			{
				if (nX < nCurrentX)
				{
					--nX;
				}
				else if (nX > nCurrentX)
				{
					++nX;
				}

				if (nY < nCurrentY)
				{
					--nY;
				}
				else if (nY > nCurrentY)
				{
					++nY;
				}

				if (nX == nCurrentX && nY == nCurrentY)
				{
					++nX;
					++nY;
				}
			}

			pAiTickParam->pAiControl->dwAiParam[1] = nX;
			pAiTickParam->pAiControl->dwAiParam[2] = nY;
			AITACTICS_SetVelocity(pUnit, 1, 0, 0);
			AITACTICS_ChangeModeAndTargetCoordinatesOneStep(pGame, pUnit, nX, nY, 8);
			AITACTICS_Idle(pGame, pUnit, 12);
			--pAiTickParam->pAiControl->dwAiParam[0];
			return;
		}

		nParam = 1;
		pAiTickParam->pAiControl->dwAiParam[0] = 1;

		if (sub_6FCD55D0(pGame, pUnit))
		{
			AITACTICS_MoveInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, MONMODE_SKILL2, 2, 3);
			AITACTICS_Idle(pGame, pUnit, 12);
			pAiTickParam->pAiControl->dwAiParam[0] = -1;
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 8;
		AITACTICS_Idle(pGame, pUnit, 12);
	}
	else if (nParam >= 1)
	{
		if (sub_6FCD55D0(pGame, pUnit))
		{
			AITACTICS_MoveInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, MONMODE_SKILL2, 2, 3);
			AITACTICS_Idle(pGame, pUnit, 12);
			pAiTickParam->pAiControl->dwAiParam[0] = -1;
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 8;
		AITACTICS_Idle(pGame, pUnit, 12);
	}

	if (!pAiTickParam->bCombat)
	{
		if (nParam != -1)
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, VULTURE_AI_PARAM_MOVE_CHANCE_PCT))
			{
				AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, VULTURE_AI_PARAM_STALL_DURATION));
				return;
			}
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VULTURE_AI_PARAM_CIRCLE_CHANCE_PCT))
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 6u, 0);
		}
		else
		{
			AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 9, 0);
		}
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		AITACTICS_Idle(pGame, pUnit, 12);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VULTURE_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
	}
	else
	{
		AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, VULTURE_AI_PARAM_STALL_DURATION));
	}
}

//D2Game.0x6FCD55D0
int32_t __fastcall sub_6FCD55D0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_ISVALIDTARGET | UNITFLAG_CANBEATTACKED | UNITFLAG_TARGETABLE;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	if (PATH_GetUnitCollisionPattern(pUnit) == COLLISION_PATTERN_SMALL_UNIT_PRESENCE || !sub_6FCBDFE0(pGame, pUnit, pRoom, nX, nY, 0, 0))
	{
		return 0;
	}

	COLLISION_SetMaskWithPattern(pRoom, nX, nY, 1, COLLIDE_MONSTER);
	PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_UNIT_PRESENCE);
	PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_MASK_MONSTER_PATH);
	AITACTICS_Idle(pGame, pUnit, 12);
	return 1;
}

//D2Game.0x6FCD5710
D2UnitStrc* __fastcall AITHINK_TargetCallback_Vulture(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2VultureAiCallbackArgStrc* pArg = (D2VultureAiCallbackArgStrc*)pCallbackArg;

	if (pTarget && (pTarget->dwUnitType == UNIT_PLAYER || pTarget->dwUnitType == UNIT_MONSTER) && pUnit != pTarget && !MONSTERS_IsDead(pTarget))
	{
		const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);

		if (nDistance <= pArg->nDistance && STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0) <= pArg->nLifePercentage * STATLIST_GetMaxLifeFromUnit(pTarget) / 100)
		{
			pArg->pTarget = pTarget;
			pArg->nDistance = nDistance;
		}
	}

	return 0;
}

enum D2C_MosquitoAIParams
{
	MOSQUITO_AI_PARAM_MIN_SUCK_LOOP = 0,
	MOSQUITO_AI_PARAM_SUCK_LOOP_RNG = 1,
	MOSQUITO_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	MOSQUITO_AI_PARAM_SUCK_OR_ATTACK_CHANCE_PCT = 3,
	MOSQUITO_AI_PARAM_MAX_AMBIENT_LOOP = 4,
};

//D2Game.0x6FCD5850
void __fastcall AITHINK_Fn024_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, MOSQUITO_AI_PARAM_ATTACK_CHANCE_PCT) || !pAiTickParam->pAiControl->dwAiParam[1])
		{
			++pAiTickParam->pAiControl->dwAiParam[1];

			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, MOSQUITO_AI_PARAM_SUCK_OR_ATTACK_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			return;
		}

		if (AI_RollPercentage(pUnit) > 20)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 2;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
	}

	switch (pAiTickParam->pAiControl->dwAiParam[0])
	{
	case 0:
		AITACTICS_SetVelocity(pUnit, 13, 100, 0);
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		break;

	case 1:
		AITACTICS_SetVelocity(pUnit, 0, 50, 0);
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);

		++pAiTickParam->pAiControl->dwAiParam[1];

		if (pAiTickParam->pAiControl->dwAiParam[1] > AI_GetParamValue(pGame, pAiTickParam, MOSQUITO_AI_PARAM_MAX_AMBIENT_LOOP))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
		}
		break;

	case 2:
		AITACTICS_SetVelocity(pUnit, 2, 100, 0);
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 0xAu, 1);
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		break;

	default:
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		break;
	}
}

enum D2C_WillowispAIState
{
	WILLOWISP_AI_STATE_IDLE = 0,
	WILLOWISP_AI_STATE_MOVING = 1,
	WILLOWISP_AI_STATE_CAN_CAST = 2,
	WILLOWISP_AI_STATE_CAN_ATTACK_HTH = 3,
	WILLOWISP_AI_STATE_ATTEMPT_RITUAL_OR_BAPTISM = 4,
	// Pentagram baptism that gives MF%
	WILLOWISP_AI_STATE_BAPTISM = 5,
	// Ritual that showcases names of developpers
	WILLOWISP_AI_STATE_RITUAL_SHAPE_D = 6, // David Brevik ?
	WILLOWISP_AI_STATE_RITUAL_SHAPE_E = 7, // Eric Schaefer ?
	WILLOWISP_AI_STATE_RITUAL_SHAPE_M = 8, // Max Schaefer ?
	WILLOWISP_AI_STATE_RITUAL_SHAPE_HORNS = 9, // Demon symbol
	WILLOWISP_AI_STATE_RITUAL_SHAPE_R = 10, // Rick Seis ?
	WILLOWISP_AI_STATE_RITUAL_SHAPE_P = 11, // Phil Shenk ?
	WILLOWISP_AI_STATE_RITUAL_SHAPE_S = 12, // Stieg Hedlund ?
	WILLOWISP_AI_STATE_RITUAL_FIRST_SHAPE = WILLOWISP_AI_STATE_RITUAL_SHAPE_D,
	WILLOWISP_AI_STATE_RITUAL_LAST_SHAPE = WILLOWISP_AI_STATE_RITUAL_SHAPE_S,
	WILLOWISP_AI_STATE_RITUAL_SHAPE_COUNT =  WILLOWISP_AI_STATE_RITUAL_LAST_SHAPE - WILLOWISP_AI_STATE_RITUAL_FIRST_SHAPE + 1
};

enum D2C_WillowispAIParams
{
	WILLOWISP_AI_PARAM_CAST_CHANCE_PCT = 0,
	WILLOWISP_AI_PARAM_MELEE_CHANCE_PCT = 1,
	WILLOWISP_AI_PARAM_APPROACH_CHANCE_PCT = 2,
};

enum D2C_WillowispAIConstants
{
	WILLOWISP_ATTACKS_PER_SHAPE = 3,
	WHISP_RITUAL_FORMATION_COUNT = 4,
	WHISP_BAPTISM_FORMATION_COUNT = 5,
	WHISP_RITUAL_BAPTISM_COOLDOWN_IN_FRAMES = 1800,
	WHISP_RITUAL_MF_BUFF_DURATION_IN_FRAMES = 24 * 60 * 60 * 20, // 20 hours
	
};


// inlined 
static void AITHINK_Fn025_Willowisp_State_NotMoving(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nAIState = pAiTickParam->pAiControl->dwAiParam[0];
	if (pAiTickParam->bCombat)
	{
		if (nAIState == WILLOWISP_AI_STATE_CAN_ATTACK_HTH || AIRollChanceParam(pGame, pUnit, pAiTickParam, WILLOWISP_AI_PARAM_MELEE_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_IDLE;
			return;
		}
	}
	else
	{
		if (nAIState == WILLOWISP_AI_STATE_CAN_CAST || AIRollChanceParam(pGame, pUnit, pAiTickParam, WILLOWISP_AI_PARAM_CAST_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_CAST, pAiTickParam->pTarget);
			pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_IDLE;
			return;
		}
	}

	pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_MOVING;
	pAiTickParam->pAiControl->dwAiParam[1] = 3; // Iterations to stay in state moving
	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, WILLOWISP_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
	}
	else
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
	}
}

// inlined 
static void AITHINK_Fn025_Willowisp_State_Moving(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const bool bFinishedMoving = pAiTickParam->pAiControl->dwAiParam[1] <= 0;

	if (bFinishedMoving && pAiTickParam->bCombat)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_ATTACK_HTH;
	}
	else if(bFinishedMoving && AIRollChanceParam(pGame, pUnit, pAiTickParam, WILLOWISP_AI_PARAM_CAST_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_CAST;
	}
	else
	{
		--pAiTickParam->pAiControl->dwAiParam[1];
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_MOVING;

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, WILLOWISP_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
		}
	}
}


// inlined 
static void AITHINK_Fn025_Willowisp_State_AttemptRitual(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitFindArgStrc unitFindArg = {};
	unitFindArg.nX = CLIENTS_GetUnitX(pUnit);
	unitFindArg.nY = CLIENTS_GetUnitY(pUnit);
	unitFindArg.nFlags = 1411;
	unitFindArg.nSize = 32;

	D2UnitFindDataStrc unitFindData = {};
	UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, UNITS_GetRoom(pUnit), unitFindArg.nX, unitFindArg.nY, 32, AITHINK_UnitFindCallback_Willowisp, &unitFindArg);
	UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);

	switch (unitFindData.nIndex)
	{
	case WHISP_RITUAL_FORMATION_COUNT:
	{
		for (int32_t i = 0; i < WHISP_RITUAL_FORMATION_COUNT; ++i)
		{
			AIGENERAL_SetAiControlParam(unitFindData.pUnitsArray[i], 1, WILLOWISP_AI_STATE_RITUAL_FIRST_SHAPE);
			AIGENERAL_SetAiControlParam(unitFindData.pUnitsArray[i], 2, 0);
			AIGENERAL_SetAiControlParam(unitFindData.pUnitsArray[i], 3, i + 1);
			AITACTICS_IdleInNeutralMode(pGame, unitFindData.pUnitsArray[i], 337 - pGame->dwGameFrame % 337);
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 337 - pGame->dwGameFrame % 337);
		break;
	}
	case WHISP_BAPTISM_FORMATION_COUNT:
	{
		for (int32_t i = 0; i < WHISP_BAPTISM_FORMATION_COUNT; ++i)
		{
			AIGENERAL_SetAiControlParam(unitFindData.pUnitsArray[i], 1, WILLOWISP_AI_STATE_BAPTISM);
			AIGENERAL_SetAiControlParam(unitFindData.pUnitsArray[i], 2, 0);
			AIGENERAL_SetAiControlParam(unitFindData.pUnitsArray[i], 3, i + 1);
			AITACTICS_IdleInNeutralMode(pGame, unitFindData.pUnitsArray[i], 337 - pGame->dwGameFrame % 337);
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 337 - pGame->dwGameFrame % 337);
		break;
	}
	default:
	{
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_CAST;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
		break;
	}
	}
	UNITFINDS_FreeUnitFindData(&unitFindData);
}

// inlined
static bool GetCloseForRitualOrBaptism(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
	const int32_t nMaxDistance = 3;
	if (AIUTIL_GetDistanceToCoordinates(pUnit, nX, nY) <= nMaxDistance)
	{
		return true;
	}
	else
	{
		if (AI_RollPercentage(pUnit) < 35)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, nMaxDistance + 1);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetCoordinatesNoStep(pGame, pUnit, nX, nY, MONMODE_WALK);
		}
		return false;
	}
}

// inlined 
static void AITHINK_Fn025_Willowisp_State_Baptism(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[2] > WHISP_BAPTISM_FORMATION_COUNT)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_CAST;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
		return;
	}

	D2_ASSERT(pAiTickParam->pAiControl->dwAiParam[2] >= 1);

	static const D2CoordStrc aBaptismCoordsOffsets[6] = {
		{},			// 0 unused
		{ -5, -5 }, // 1
		{  3, -5 }, // 2
		{  6 , 3 }, // 3
		{  3 , 6 }, // 4
		{ -5 , 3 }, // 5
	};

	const int32_t nX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + aBaptismCoordsOffsets[pAiTickParam->pAiControl->dwAiParam[2]].nX;
	const int32_t nY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + aBaptismCoordsOffsets[pAiTickParam->pAiControl->dwAiParam[2]].nY;

	if (GetCloseForRitualOrBaptism(pGame, pUnit, nX, nY))
	{
		const int32_t nFrame = pGame->dwGameFrame % 67;
		if (nFrame)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 67 - nFrame);
			return;
		}

		if (pAiTickParam->pAiControl->dwAiParam[1] >= WILLOWISP_ATTACKS_PER_SHAPE)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_CAST;
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + WHISP_RITUAL_BAPTISM_COOLDOWN_IN_FRAMES;
		}
		else
		{
			static const D2CoordStrc aBaptismTargetOffsets[6] = {
				{},         // 0 unused
				{  6,  3 }, // 1
				{  3,  6 }, // 2
				{ -5,  3 }, // 3
				{ -5, -5 }, // 4
				{  3, -5 }, // 5
			};
			const D2CoordStrc tBaptismTargetOffset = aBaptismTargetOffsets[pAiTickParam->pAiControl->dwAiParam[2]];
			if (tBaptismTargetOffset.nX || tBaptismTargetOffset.nY)
			{
				const int32_t nTargetX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + tBaptismTargetOffset.nX;
				const int32_t nTargetY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + tBaptismTargetOffset.nY;

				AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_CAST, nTargetX, nTargetY);
				// Next attack
				++pAiTickParam->pAiControl->dwAiParam[1];
				return;
			}

			// Next attack
			++pAiTickParam->pAiControl->dwAiParam[1];
		}
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 337 - pGame->dwGameFrame % 337);
		return;
	}
}

// inlined 
static void AITHINK_Fn025_Willowisp_State_Ritual(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	// 4 whisps ritual
	// => dwAiParam[0] == shape ( 6 - 12 )
	// => dwAiParam[1] == fire lightning iteration ( 0 - 2 )
	// => dwAiParam[2] == whisp id ( 1 - 4 )

	const int32_t nWhispIndex = pAiTickParam->pAiControl->dwAiParam[2] - 1;
	const int32_t nWhispFormationIndex = pAiTickParam->pAiControl->dwAiParam[0] - WILLOWISP_AI_STATE_RITUAL_FIRST_SHAPE;
	
	if (nWhispIndex >= WHISP_RITUAL_FORMATION_COUNT) // New whisp tried to enter ritual, go back to normal AI after idling
	{
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_CAST;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
		return;
	}

	static const D2CoordStrc aRitualCoordsOffsets[WILLOWISP_AI_STATE_RITUAL_SHAPE_COUNT][WHISP_RITUAL_FORMATION_COUNT] = {
		{{ -10 ,  -3 }, {   1 ,  -5 }, {  6 ,  0 }, {  4 ,  10 }},
		{{  -9 ,  -2 }, {  -6 , -11 }, { -2 , -5 }, {  6 ,   1 }},
		{{   0 ,  13 }, { -12 ,   0 }, {  7 ,  5 }, { -3 , -11 }},
		{{ -15 ,  -3 }, {  -6 ,  -3 }, { -4 , -5 }, { -6 , -13 }},
		{{ -12 ,  -2 }, {  -6 ,   2 }, { -5 ,  5 }, {  1 ,  11 }},
		{{ -13 ,  -6 }, {  -8 , -11 }, { -7 ,  0 }, {  1 ,   8 }},
		{{  -5 ,  -8 }, {  -8 ,   1 }, {  1 , -6 }, {  6 ,   9 }}
	};

	int32_t nX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + aRitualCoordsOffsets[nWhispFormationIndex][nWhispIndex].nX;
	int32_t nY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + aRitualCoordsOffsets[nWhispFormationIndex][nWhispIndex].nY;

	if (GetCloseForRitualOrBaptism(pGame, pUnit, nX, nY))
	{
		if (const int32_t nFrame = pGame->dwGameFrame % 67)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 67 - nFrame);
			return;
		}

		if (pAiTickParam->pAiControl->dwAiParam[1] >= 3)
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
			++pAiTickParam->pAiControl->dwAiParam[0];

			if (pAiTickParam->pAiControl->dwAiParam[0] - 6 > 6) // Finished last shape, go back to normal AI
			{
				pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_CAN_CAST;
				pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + WHISP_RITUAL_BAPTISM_COOLDOWN_IN_FRAMES;
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, 337 - pGame->dwGameFrame % 337);

			if (pAiTickParam->pTarget->dwUnitType == UNIT_PLAYER && !SUNIT_IsDead(pAiTickParam->pTarget))
			{
				const uint32_t nBuffEndFrame = pGame->dwGameFrame + WHISP_RITUAL_MF_BUFF_DURATION_IN_FRAMES;
				if (D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, STATLIST_NEWLENGTH, nBuffEndFrame, pAiTickParam->pTarget->dwUnitType, pAiTickParam->pTarget->dwUnitId))
				{
					STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_MAGICBONUS, 50 * (pGame->nDifficulty + 1), 0);
					D2COMMON_10476(pStatList, nBuffEndFrame);
					EVENT_SetEvent(pGame, pAiTickParam->pTarget, EVENTTYPE_REMOVESTATE, nBuffEndFrame, 0, 0);
					D2COMMON_10475_PostStatToStatList(pAiTickParam->pTarget, pStatList, 1);
				}
			}
			return;
		}

		static const D2CoordStrc aRitualTargetsOffsets[WILLOWISP_AI_STATE_RITUAL_SHAPE_COUNT][WHISP_RITUAL_FORMATION_COUNT] = {
			{{   1,  -5}, {   6,  0},{  4,  10}, { -10, -3}},
			{{   1,   8}, {  -9, -2},{ -5,   4}, {   1,  8}},
			{{ -12,   0}, {   7,  5},{ -3, -11}, {  11, -2}},
			{{   7,   9}, { -15, -3},{ -6, -13}, {   9,  7}},
			{{  -5,  -7}, {  -5, -7},{  9,   0}, { -12, -2}},
			{{  -8, -11}, {  -2, -6},{ -2,  -6}, { -13, -6}},
			{{  -8,   1}, {   1, -6},{  6,   9}, {   0,  0}}
		};

		D2CoordStrc tRitualTargetOffset = aRitualTargetsOffsets[nWhispFormationIndex][nWhispIndex];

		if (tRitualTargetOffset.nX == 0 && tRitualTargetOffset.nY == 0)
		{
			++pAiTickParam->pAiControl->dwAiParam[1];
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 67 - pGame->dwGameFrame % 67);
			return;
		}

		nX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + tRitualTargetOffset.nX;
		nY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + tRitualTargetOffset.nY;

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_CAST, nX, nY);
		++pAiTickParam->pAiControl->dwAiParam[1];
		return;
	}
}

// inlined
static void AITHINK_Fn025_Willowisp_CheckConditionsForRitualOrBaptism(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nAIState = pAiTickParam->pAiControl->dwAiParam[0];
	if (pAiTickParam->pTarget														// We have a target player
		&& pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[2]				// Ritual / Baptism cooldown elapsed
		&& AITHINK_GetSquaredDistance(pUnit, pAiTickParam->pTarget) < (32 * 32)		// Player close enough
		&& nAIState < WILLOWISP_AI_STATE_ATTEMPT_RITUAL_OR_BAPTISM					// Not already doing a ritual
		&& (ITEMS_RollRandomNumber(&pUnit->pSeed) % 1000) <= pGame->nDifficulty + 2 // 0.2% to 0.5% chance to initiate sequence
		)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = WILLOWISP_AI_STATE_ATTEMPT_RITUAL_OR_BAPTISM;
	}
}

//D2Game.0x6FCD5A60
void __fastcall AITHINK_Fn025_Willowisp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	AITHINK_Fn025_Willowisp_CheckConditionsForRitualOrBaptism(pGame, pUnit, pAiTickParam);

	switch (const uint32_t nAIState = pAiTickParam->pAiControl->dwAiParam[0])
	{
	case WILLOWISP_AI_STATE_IDLE:
	case WILLOWISP_AI_STATE_CAN_CAST:
	case WILLOWISP_AI_STATE_CAN_ATTACK_HTH:
		AITHINK_Fn025_Willowisp_State_NotMoving(pGame, pUnit, pAiTickParam);
		return;
	case WILLOWISP_AI_STATE_MOVING:
		AITHINK_Fn025_Willowisp_State_Moving(pGame, pUnit, pAiTickParam);
		return;
	case WILLOWISP_AI_STATE_ATTEMPT_RITUAL_OR_BAPTISM:
		AITHINK_Fn025_Willowisp_State_AttemptRitual(pGame, pUnit, pAiTickParam);
		return;
	case WILLOWISP_AI_STATE_BAPTISM:
		AITHINK_Fn025_Willowisp_State_Baptism(pGame, pUnit, pAiTickParam);
		return;
	case WILLOWISP_AI_STATE_RITUAL_FIRST_SHAPE:
	default: // Ritual >= WILLOWISP_AI_STATE_RITUAL_FIRST_SHAPE
		AITHINK_Fn025_Willowisp_State_Ritual(pGame, pUnit, pAiTickParam);
		return;
	}
}

//D2Game.0x6FCD6960
int32_t __fastcall AITHINK_UnitFindCallback_Willowisp(D2UnitStrc* pUnit, D2UnitFindArgStrc* pUnitFindArg)
{
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && !MONSTERS_IsDead(pUnit))
	{
		int32_t nClassId = pUnit->dwClassId;
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
		if (pMonStatsTxtRecord)
		{
			nClassId = pMonStatsTxtRecord->nBaseId;
		}

		if (nClassId == MONSTER_WILLOWISP1)
		{
			return 1;
		}
	}

	return 0;
}

enum D2C_ArachAIParams
{
	ARACH_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	ARACH_AI_PARAM_CIRCLE_IN_MELEE_CHANCE_PCT = 1,
	ARACH_AI_PARAM_ENGAGE_CHANCE_PCT = 2,
	ARACH_AI_PARAM_RUN_DISTANCE = 3,
	ARACH_AI_PARAM_HURT_PCT = 4
};

//D2Game.0x6FCD69E0
void __fastcall AITHINK_Fn026_Arach(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);

	if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
	{
		pAiTickParam->pAiControl->dwAiParam[1] = 0;

		if (nLifePercentage > 75)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;

			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, ARACH_AI_PARAM_ENGAGE_CHANCE_PCT))
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 6u, 0);
				return;
			}

			pAiTickParam->pAiControl->dwAiParam[0] = 2;
			sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 0);
			return;
		}

		if (!pAiTickParam->bCombat || AI_GetParamValue(pGame, pAiTickParam, ARACH_AI_PARAM_ATTACK_CHANCE_PCT) <= 25 || (AI_RollPercentage(pUnit) >= AI_GetParamValue(pGame, pAiTickParam, ARACH_AI_PARAM_ATTACK_CHANCE_PCT) - 25))
		{
			if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, ARACH_AI_PARAM_RUN_DISTANCE) || sub_6FCF2E70(pUnit))
			{
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			}
			else
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 12, 0);
			}
			return;
		}

		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (!pAiTickParam->bCombat)
	{
		if (sub_6FCF2E70(pUnit) || pAiTickParam->pAiControl->dwAiParam[1] == 1)
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 1;
		}
		else
		{
			++pAiTickParam->pAiControl->dwAiParam[2];

			if (pAiTickParam->pAiControl->dwAiParam[2] > 20)
			{
				pAiTickParam->pAiControl->dwAiParam[2] = 0;
			}

			pAiTickParam->pAiControl->dwAiParam[1] = 0;

			if (pAiTickParam->pAiControl->dwAiParam[2] != 1 || !AIRollChanceParam(pGame, pUnit, pAiTickParam, ARACH_AI_PARAM_ENGAGE_CHANCE_PCT))
			{
				if (AI_RollPercentage(pUnit) >= 20)
				{
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
				}
				else
				{
					AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
				}
				return;
			}

			pAiTickParam->pAiControl->dwAiParam[1] = 1;
		}

		sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 0);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 2;

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ARACH_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (nLifePercentage >= AI_GetParamValue(pGame, pAiTickParam, ARACH_AI_PARAM_HURT_PCT))
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, 1))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		else
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		}
	}
	else
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;

		if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || STATES_CheckState(pUnit, 22))
		{
			D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 8u, 0);
		}
		else
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, 0, 0);
		}
	}
}

enum D2C_ThornHulkAIParams
{
	THORNHULK_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	THORNHULK_AI_PARAM_ATTACK_2_OR_1_CHANCE_PCT = 1,
	THORNHULK_AI_PARAM_CIRCLE_IN_MELEE_CHANCE_PCT = 2,
	THORNHULK_AI_PARAM_FRENZY_CHANCE_PCT = 3,
	THORNHULK_AI_PARAM_FRENZY_SPEED = 4,
	THORNHULK_AI_PARAM_CONSECUTIVE_ATTACK2 = 5
};

//D2Game.0x6FCD6D60
void __fastcall AITHINK_Fn027_ThornHulk(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->bCombat || !pAiTickParam->pTarget)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] > 0)
	{
		AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);

		const int32_t nEventFrame = EVENT_GetEventFrame(pGame, pUnit, EVENTTYPE_ENDANIM);
		if (nEventFrame > pGame->dwGameFrame)
		{
			AITACTICS_Idle(pGame, pUnit, nEventFrame + AI_GetParamValue(pGame, pAiTickParam, THORNHULK_AI_PARAM_FRENZY_SPEED) - pGame->dwGameFrame);
		}

		--pAiTickParam->pAiControl->dwAiParam[0];

		if (pAiTickParam->pAiControl->dwAiParam[0] <= 0)
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 3;
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, 0))
	{
		if (pAiTickParam->pAiControl->dwAiParam[1] > 0 || !AIRollChanceParam(pGame, pUnit, pAiTickParam, THORNHULK_AI_PARAM_FRENZY_CHANCE_PCT))
		{
			--pAiTickParam->pAiControl->dwAiParam[1];

			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, THORNHULK_AI_PARAM_ATTACK_2_OR_1_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			}
		}
		else
		{
			AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);

			const int32_t nEventFrame = EVENT_GetEventFrame(pGame, pUnit, EVENTTYPE_ENDANIM);
			if (nEventFrame > pGame->dwGameFrame)
			{
				AITACTICS_Idle(pGame, pUnit, nEventFrame + AI_GetParamValue(pGame, pAiTickParam, THORNHULK_AI_PARAM_FRENZY_SPEED) - pGame->dwGameFrame);
			}

			pAiTickParam->pAiControl->dwAiParam[0] = AI_GetParamValue(pGame, pAiTickParam, THORNHULK_AI_PARAM_CONSECUTIVE_ATTACK2);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, THORNHULK_AI_PARAM_CIRCLE_IN_MELEE_CHANCE_PCT))
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

enum D2C_VampireAIParams
{
	VAMPIRE_AI_PARAM_MELEE_CHANCE_PCT = 0,
	VAMPIRE_AI_PARAM_CAST_CHANCE_PCT = 1,
	VAMPIRE_AI_PARAM_ACTIVE_DISTANCE = 2,
	VAMPIRE_AI_PARAM_UPGRADE_CAST_CHANCE_PCT = 3,
	VAMPIRE_AI_PARAM_SPELL_FLAGS = 4,
};

//D2Game.0x6FCD6FD0
void __fastcall AITHINK_Fn028_Vampire(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const uint16_t nSpellFlags = AI_GetParamValue(pGame, pAiTickParam, VAMPIRE_AI_PARAM_SPELL_FLAGS);

	if (pAiTickParam->pAiControl->dwAiParam[2] > 0)
	{
		--pAiTickParam->pAiControl->dwAiParam[2];
	}

	int32_t nDistance = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	if (sub_6FCF2E70(pUnit))
	{
		if (!pAiTickParam->pAiControl->dwAiParam[0])
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
		}

		if (pAiTickParam->nTargetDistance < 30 && pAiTickParam->nTargetDistance > pAiTickParam->pAiControl->dwAiParam[1])
		{
			pAiTickParam->pAiControl->dwAiParam[1] = pAiTickParam->nTargetDistance;
		}

		if (pAiTickParam->bCombat)
		{
			if (AI_RollPercentage(pUnit) <= 30 && nSpellFlags & 1)
			{
				if (AI_RollPercentage(pUnit) < 50)
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				}
				else
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
				}
				return;
			}

			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
	if (pAiTickParam->pAiControl->dwAiParam[0] == 2)
	{
		if (nLifePercentage >= 75)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}

		if (pAiTickParam->nTargetDistance < 14 || pAiTickParam->nTargetDistance <= pAiTickParam->pAiControl->dwAiParam[1])
		{
			int32_t nVel = 0;
			if (pAiTickParam->pMonstatsTxt->nVelocity > 0)
			{
				nVel = D2Clamp(100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity - 100, 0, 120);
			}

			AITACTICS_SetVelocity(pUnit, 0, nVel, 0);

			if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 8u, 1))
			{
				return;
			}
		}

		if (pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, VAMPIRE_AI_PARAM_ACTIVE_DISTANCE))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			return;
		}

		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, 1))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			return;
		}

		if ((nSpellFlags & (1 << 1)) && pAiTickParam->pAiControl->dwAiParam[2] <= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, VAMPIRE_AI_PARAM_UPGRADE_CAST_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[2] = 11;
			return;
		}

		if ((nSpellFlags & (1 << 2)) && pAiTickParam->pAiControl->dwAiParam[2] <= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, VAMPIRE_AI_PARAM_UPGRADE_CAST_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[2] = 11;
			return;
		}

		if ((nSpellFlags & (1 << 0)) && pTarget && nDistance <= 20)
		{
			if (AI_RollPercentage(pUnit) < 50)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
			}
			else
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
			}
			return;
		}

		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		return;
	}

	if (nLifePercentage < 33)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 2;
		if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 8u, 0))
		{
			return;
		}
	}

	if (pAiTickParam->bCombat)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VAMPIRE_AI_PARAM_MELEE_CHANCE_PCT))
		{
			if (!(nSpellFlags & (1 << 0)) || !pTarget || AI_RollPercentage(pUnit) > 30)
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
				return;
			}

			if (nDistance <= 20)
			{
				if (AI_RollPercentage(pUnit) < 50)
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
				}
				else
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
				}
				return;
			}
		}

		if (AI_RollPercentage(pUnit) < 33)
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		return;
	}

	if (pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, VAMPIRE_AI_PARAM_ACTIVE_DISTANCE))
	{
		if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 1;

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VAMPIRE_AI_PARAM_CAST_CHANCE_PCT))
	{
		if ((nSpellFlags & (1 << 1)) && pAiTickParam->pAiControl->dwAiParam[2] <= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, VAMPIRE_AI_PARAM_UPGRADE_CAST_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[2] = 11;
			return;
		}

		if ((nSpellFlags & (1 << 2)) && pAiTickParam->pAiControl->dwAiParam[2] <= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, VAMPIRE_AI_PARAM_UPGRADE_CAST_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[2] = 11;
			return;
		}

		if (!(nSpellFlags & (1 << 0)) || !pTarget || nDistance > 20)
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}

		if (AI_RollPercentage(pUnit) >= 75)
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			return;
		}

		if (AI_RollPercentage(pUnit) < 50)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		}
		else
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
		}
		return;
	}

	if (pAiTickParam->nTargetDistance > 20)
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (pAiTickParam->nTargetDistance < 9 && AI_RollPercentage(pUnit) < 50)
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 8u, 0);
		return;
	}

	if (AI_RollPercentage(pUnit) < 50)
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
	}
}

//D2Game.0x6FCD76F0
void __fastcall D2GAME_AI_Unk029_6FCD76F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pUnit && pUnit->dwAnimMode == MONMODE_SKILL4)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		AITACTICS_Idle(pGame, pUnit, 1);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] == 4)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL3, 0);
		return;
	}

	AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, pAiTickParam->pAiControl->nAiSpecialState);
	AITACTICS_IdleInNeutralMode(pGame, pUnit, 1);
}

enum D2C_BatDemonAIParams
{
	BATDEMON_AI_PARAM_HURT_PCT = 0,
	BATDEMON_AI_PARAM_DISENGAGE_ON_HIT_CHANCE_PCT = 1,
	BATDEMON_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	BATDEMON_AI_PARAM_LIGHTNING_OR_ATTACK_1_CHANCE_PCT = 3,
	BATDEMON_AI_PARAM_REGEN_ROOSTED = 4,
};

//D2Game.0x6FCD7760
void __fastcall AITHINK_Fn029_BatDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
	const int32_t nAiState = pAiTickParam->pAiControl->dwAiParam[0];

	if (nAiState && nAiState != 4)
	{
		switch (nAiState)
		{
		case 1u:
			if (pAiTickParam->pAiControl->dwAiParam[1] > 1 && (pAiTickParam->bCombat || pAiTickParam->nTargetDistance < 7 || sub_6FCF2E70(pUnit) || pAiTickParam->nTargetDistance < 14 && nLifePercentage > 50))
			{
				if (pAiTickParam->pAiControl->dwAiParam[2])
				{
					STATLIST_SetUnitStat(pUnit, STAT_HPREGEN, STATLIST_UnitGetStatValue(pUnit, STAT_HPREGEN, 0) - pAiTickParam->pAiControl->dwAiParam[2], 0);
				}

				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL2, 0);
				AITACTICS_Idle(pGame, pUnit, 6);
				pAiTickParam->pAiControl->dwAiParam[0] = 3;
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL4, 0);

				if (pAiTickParam->pAiControl->dwAiParam[1])
				{
					AITACTICS_Idle(pGame, pUnit, 15);
				}
				else
				{
					AITACTICS_Idle(pGame, pUnit, 10);
				}

				++pAiTickParam->pAiControl->dwAiParam[1];
			}
			break;

		case 2u:
			if (nLifePercentage < AI_GetParamValue(pGame, pAiTickParam, BATDEMON_AI_PARAM_HURT_PCT) && D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 15, 0))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 4;
			}
			else
			{
				if (AI_RollPercentage(pUnit) >= 33)
				{
					if (AI_RollPercentage(pUnit) >= 15)
					{
						AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
					}
					else
					{
						AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
					}
				}
				else
				{
					AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
					pAiTickParam->pAiControl->dwAiParam[0] = 3;
				}
			}
			break;

		case 3u:
			if (nLifePercentage < AI_GetParamValue(pGame, pAiTickParam, BATDEMON_AI_PARAM_HURT_PCT) && D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 0xFu, 0))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 4;
			}
			else
			{
				if (pAiTickParam->bCombat)
				{
					if (pAiTickParam->pAiControl->dwAiParam[1] <= 0)
					{
						if (sub_6FCF2E70(pUnit) && AIRollChanceParam(pGame, pUnit, pAiTickParam, BATDEMON_AI_PARAM_DISENGAGE_ON_HIT_CHANCE_PCT) && D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 12, 0))
						{
							pAiTickParam->pAiControl->dwAiParam[0] = 2;
						}
						else
						{
							if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, BATDEMON_AI_PARAM_ATTACK_CHANCE_PCT))
							{
								AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
							}
							else
							{
								if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BATDEMON_AI_PARAM_LIGHTNING_OR_ATTACK_1_CHANCE_PCT))
								{
									AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
								}
								else
								{
									AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
								}
							}
						}
					}
					else
					{
						AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
						pAiTickParam->pAiControl->dwAiParam[1] = 0;
					}
				}
				else
				{
					AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
				}
			}
			break;

		default:
			if (pAiTickParam->nTargetDistance >= 15)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 4;
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
			else
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
				pAiTickParam->pAiControl->dwAiParam[0] = 3;
			}
			break;
		}
	}
	else
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL3, 0);
		AITACTICS_Idle(pGame, pUnit, 8);
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;

		const int32_t nHpRegen = STATLIST_UnitGetStatValue(pUnit, STAT_HPREGEN, 0);
		if (nHpRegen)
		{
			pAiTickParam->pAiControl->dwAiParam[2] = (nHpRegen * AI_GetParamValue(pGame, pAiTickParam, BATDEMON_AI_PARAM_REGEN_ROOSTED)) / 8;
			STATLIST_SetUnitStat(pUnit, STAT_HPREGEN, nHpRegen + pAiTickParam->pAiControl->dwAiParam[2], 0);
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
		}
	}
}

enum D2C_FetishAIParams
{
	FETISH_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	FETISH_AI_PARAM_STALL_DURATION = 1,
	FETISH_AI_PARAM_ATTACK_LOOP = 2,
	FETISH_AI_PARAM_WEAK_PCT = 3,
};

//D2Game.0x6FCD7BA0
void __fastcall AITHINK_Fn030_Fetish(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (pAiCmd)
	{
		if (pAiCmd->nCmdParam[0] == 1 || pAiCmd->nCmdParam[0] == 14)
		{
			D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, pAiCmd->nCmdParam[2], pAiCmd->nCmdParam[1]);
			if (pTarget)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				AITACTICS_SetVelocity(pUnit, 13, 50, 0);
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 0);
				AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
				return;
			}
		}

		AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
	}

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pAiTickParam->pTarget);

	switch (pAiTickParam->pAiControl->dwAiParam[0])
	{
	case 0:
		if (pAiTickParam->bCombat)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;

			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FETISH_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, 1));
			}
			return;
		}

		AITACTICS_SetVelocity(pUnit, 13, 50, 0);
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;

	case 1:
		++pAiTickParam->pAiControl->dwAiParam[1];

		if (pAiTickParam->pAiControl->dwAiParam[1] > AI_GetParamValue(pGame, pAiTickParam, FETISH_AI_PARAM_ATTACK_LOOP) && nLifePercentage > AI_GetParamValue(pGame, pAiTickParam, FETISH_AI_PARAM_WEAK_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 2;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
			AITACTICS_SetVelocity(pUnit, 2, 50, 0);
			D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 14, 1);
			return;
		}

		if (pAiTickParam->bCombat)
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FETISH_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, FETISH_AI_PARAM_STALL_DURATION));
			}
			return;
		}

		AITACTICS_SetVelocity(pUnit, 13, 50, 0);
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;

	case 2:
		if (pAiTickParam->nTargetDistance <= 12)
		{
			AITACTICS_SetVelocity(pUnit, 2, 50, 0);
			if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 14, 1))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
			return;
		}

		++pAiTickParam->pAiControl->dwAiParam[1];

		if (pAiTickParam->pAiControl->dwAiParam[1] > 1)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
		}

		if (AI_RollPercentage(pUnit) >= 20)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		}
		return;

	default:
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}
}

enum D2C_HellMeteorAIParams
{
	HELLMETEOR_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	HELLMETEOR_AI_PARAM_STALL_DURATION = 1,
	HELLMETEOR_AI_PARAM_RANGE = 2,
};

//D2Game.0x6FCD7EB0
void __fastcall AITHINK_Fn033_HellMeteor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, HELLMETEOR_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		const int32_t nRange = AI_GetParamValue(pGame, pAiTickParam, HELLMETEOR_AI_PARAM_RANGE);

		const int32_t nX = CLIENTS_GetUnitX(pUnit) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRange) - nRange;
		const int32_t nY = CLIENTS_GetUnitY(pUnit) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRange) - nRange;

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, nX, nY);
		return;
	}
	
	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, HELLMETEOR_AI_PARAM_STALL_DURATION));
}

enum D2C_AndarielAIParams
{
	ANDARIEL_AI_PARAM_SPRAY_IN_MELEE_CHANCE_PCT = 0,
	ANDARIEL_AI_PARAM_STALL_CHANCE_PCT = 1,
	ANDARIEL_AI_PARAM_FIRE_OR_ENGAGE_CHANCE_PCT = 2,
	ANDARIEL_AI_PARAM_SPRAY_FROM_DISTANCE_CHANCE_PCT = 3,
};

//D2Game.0x6FCD8090
void __fastcall AITHINK_Fn034_Andariel(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, ANDARIEL_AI_PARAM_SPRAY_IN_MELEE_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANDARIEL_AI_PARAM_STALL_CHANCE_PCT))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANDARIEL_AI_PARAM_FIRE_OR_ENGAGE_CHANCE_PCT))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, ANDARIEL_AI_PARAM_SPRAY_FROM_DISTANCE_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	AITACTICS_SetVelocity(pUnit, 1, 0, 0);
	AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
}

enum D2C_CorruptArcherAIParams
{
	CORRUPTARCHER_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	CORRUPTARCHER_AI_PARAM_SHOOT_CHANCE_PCT = 1,
	CORRUPTARCHER_AI_PARAM_STALL_DURATION = 2,
	CORRUPTARCHER_AI_PARAM_RUN_CHANCE_PCT = 3,
	CORRUPTARCHER_AI_PARAM_ALWAYS_RUN_DISTANCE = 4,
	CORRUPTARCHER_AI_PARAM_USE_SKILL_2_CHANCE_PCT = 5,
	CORRUPTARCHER_AI_PARAM_USE_SKILL_3_CHANCE_PCT = 6,
	CORRUPTARCHER_AI_PARAM_WALK_TOW_DISTANCE = 7,
};

//D2Game.0x6FCD8260
void __fastcall AITHINK_Fn035_CorruptArcher(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	if (!pTarget)
	{
		if (AI_RollPercentage(pUnit) >= 50)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_STALL_DURATION));
		}
		else
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
		}
		return;
	}

	if (!pAiTickParam->bCombat && sub_6FCF2E70(pUnit))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		return;
	}

	if (nDistance < 6 && AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTARCHER_AI_PARAM_RUN_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 0, 100, 0);
		if (sub_6FCD06D0(pGame, pUnit, pTarget, 12, 1))
		{
			return;
		}
	}

	if (AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_WALK_TOW_DISTANCE) > 0 && nDistance > AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_WALK_TOW_DISTANCE)
		&& AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTARCHER_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 0, 10, 0);
		AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pTarget, AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_WALK_TOW_DISTANCE));
		return;
	}

	if (nDistance > AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_ALWAYS_RUN_DISTANCE))
	{
		AITACTICS_SetVelocity(pUnit, 0, 100, 0);
		AITACTICS_RunToTargetUnitWithSteps(pGame, pUnit, pTarget, AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_ALWAYS_RUN_DISTANCE));
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTARCHER_AI_PARAM_SHOOT_CHANCE_PCT))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTARCHER_AI_PARAM_USE_SKILL_2_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTARCHER_AI_PARAM_USE_SKILL_3_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
			return;
		}

		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CORRUPTARCHER_AI_PARAM_STALL_DURATION));
}

enum D2C_CorruptLancerAIParams
{
	CORRUPTLANCER_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	CORRUPTLANCER_AI_PARAM_ATTACK_CHANCE_PCT = 1,
	CORRUPTLANCER_AI_PARAM_STALL_DURATION = 2,
	CORRUPTLANCER_AI_PARAM_RUN_CHANCE_PCT = 3,
	CORRUPTLANCER_AI_PARAM_ALWAYS_RUN_DISTANCE = 4,
	CORRUPTLANCER_AI_PARAM_USE_SKILL_1_CHANCE_PCT = 5,
	CORRUPTLANCER_AI_PARAM_USE_SKILL_2_CHANCE_PCT = 6,
	CORRUPTLANCER_AI_PARAM_USE_SKILL_3_CHANCE_PCT = 7,
};

//D2Game.0x6FCD85B0
void __fastcall AITHINK_Fn036_CorruptLancer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, CORRUPTLANCER_AI_PARAM_ALWAYS_RUN_DISTANCE))
	{
		PATH_SetStepNum(pUnit->pDynamicPath, pAiTickParam->pMonstats2Txt->nMeleeRng);
		AITACTICS_SetVelocity(pUnit, 13, 100, 0);
		AITACTICS_RunToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, pAiTickParam->pMonstats2Txt->nMeleeRng);
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (!pAiTickParam->pAiControl->dwAiParam[0] && !AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTLANCER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CORRUPTLANCER_AI_PARAM_STALL_DURATION));
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 0;

		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTLANCER_AI_PARAM_USE_SKILL_1_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		}
		else if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTLANCER_AI_PARAM_USE_SKILL_2_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
		}
		else if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTLANCER_AI_PARAM_USE_SKILL_3_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTLANCER_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		PATH_SetStepNum(pUnit->pDynamicPath, pAiTickParam->pMonstats2Txt->nMeleeRng);

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CORRUPTLANCER_AI_PARAM_RUN_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 13, 100, 0);
			AITACTICS_RunToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, pAiTickParam->pMonstats2Txt->nMeleeRng);
			return;
		}

		AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 3u);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CORRUPTLANCER_AI_PARAM_STALL_DURATION));
}

enum D2C_SkeletonBowAIParams
{
	SKELETONBOW_AI_PARAM_SHOOT_CHANCE_PCT = 0,
	SKELETONBOW_AI_PARAM_STALL_DURATION = 1,
	SKELETONBOW_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	SKELETONBOW_AI_PARAM_WALK_STEPS = 3,
	SKELETONBOW_AI_PARAM_TARGET_DISTANCE = 4,
};

//D2Game.0x6FCD88C0
void __fastcall AITHINK_Fn037_SkeletonBow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;

	if (sub_6FCF2E70(pUnit))
	{
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
		if (pTarget)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}
	}

	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	if (pTarget && nDistance < 20)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONBOW_AI_PARAM_SHOOT_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}

		if (AI_RollPercentage(pUnit) < 20)
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SKELETONBOW_AI_PARAM_STALL_DURATION));
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONBOW_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, AI_GetParamValue(pGame, pAiTickParam, SKELETONBOW_AI_PARAM_WALK_STEPS), AI_GetParamValue(pGame, pAiTickParam, SKELETONBOW_AI_PARAM_TARGET_DISTANCE));
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
}

enum D2C_MaggotLarvaAIParams
{
	MAGGOTLARVA_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	MAGGOTLARVA_AI_PARAM_ATTACK_RECOVERY_DURATION = 1,
	MAGGOTLARVA_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	MAGGOTLARVA_AI_PARAM_STALL_DURATION = 3,
	MAGGOTLARVA_AI_PARAM_CIRCLE_CHANCE_PCT = 4, // unused
};

//D2Game.0x6FCD8A60
void __fastcall AITHINK_Fn038_MaggotLarva(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pAiControl->dwAiParam[0] || !AIRollChanceParam(pGame, pUnit, pAiTickParam, MAGGOTLARVA_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MAGGOTLARVA_AI_PARAM_ATTACK_RECOVERY_DURATION));
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 0;

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, MAGGOTLARVA_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 1);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MAGGOTLARVA_AI_PARAM_STALL_DURATION));
}

enum D2C_PinHeadAIParams
{
	PINHEAD_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	PINHEAD_AI_PARAM_ATTACK_STALL_DURATION = 1,
	PINHEAD_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	PINHEAD_AI_PARAM_IDLE_STALL_DURATION = 3,
	PINHEAD_AI_PARAM_SMITE_CHANCE_PCT = 4,
	PINHEAD_AI_PARAM_SKILL2_CHANCE_PCT = 5, // Unused in original game, skill not set
};

//D2Game.0x6FCD8B60
void __fastcall AITHINK_Fn039_PinHead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pAiControl->dwAiParam[0] && !AIRollChanceParam(pGame, pUnit, pAiTickParam, PINHEAD_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, PINHEAD_AI_PARAM_ATTACK_STALL_DURATION));
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 1;

		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, PINHEAD_AI_PARAM_SMITE_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		}
		else if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, PINHEAD_AI_PARAM_SKILL2_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 0;

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, PINHEAD_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, PINHEAD_AI_PARAM_IDLE_STALL_DURATION));
}

enum D2C_MaggotEggAIParams
{
	MAGGOTEGG_AI_PARAM_STALL_DURATION = 0,
	MAGGOTEGG_AI_PARAM_HATCH_CHANCE_PCT = 1,
};

//D2Game.0x6FCD8D20
void __fastcall AITHINK_Fn040_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
		{
			SUNITDMG_KillMonster(pGame, pUnit, SUNIT_GetTargetUnit(pGame, pUnit), 1);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MAGGOTEGG_AI_PARAM_STALL_DURATION));
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, MAGGOTEGG_AI_PARAM_HATCH_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MAGGOTEGG_AI_PARAM_STALL_DURATION));
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			return;
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
	{
		SUNITDMG_KillMonster(pGame, pUnit, SUNIT_GetTargetUnit(pGame, pUnit), 1);
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MAGGOTEGG_AI_PARAM_STALL_DURATION));
}

//D2Game.0x6FCD8E10
void __fastcall D2GAME_AI_Unk043_045_121_6FCD8E10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;
	pAiTickParam->pAiControl->dwAiParam[1] = 0;
}

enum D2C_FoulCrowNestAIParams
{
	FOULCROWNEST_AI_PARAM_SPAWN_INTERVAL = 0,
	FOULCROWNEST_AI_PARAM_NUM_TO_SPAWN = 2,
};

//D2Game.0x6FCD8E30
void __fastcall AITHINK_Fn043_FoulCrowNest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->nTargetDistance > 20)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] < AI_GetParamValue(pGame, pAiTickParam, FOULCROWNEST_AI_PARAM_NUM_TO_SPAWN))
	{
		const int32_t nFrameDiff = std::abs(pGame->dwGameFrame - pAiTickParam->pAiControl->dwAiParam[0]);

		if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || nFrameDiff < AI_GetParamValue(pGame, pAiTickParam, FOULCROWNEST_AI_PARAM_SPAWN_INTERVAL))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed) % 10 + 20);
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;

		if (sub_6FC68350(MONSTER_CROWNEST1, UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitX(pUnit), 0))
		{
			++pAiTickParam->pAiControl->dwAiParam[1];
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed) % 10 + 20);
		return;
	}

	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_NOTC;
	}

	AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
}

enum D2C_DurielAIParams
{
	DURIEL_AI_PARAM_HOLY_FREEZE_LEVEL = 0,
	DURIEL_AI_PARAM_SMITE_CHANCE_PCT = 1,
	DURIEL_AI_PARAM_JAB_CHANCE_PCT = 2,
	DURIEL_AI_PARAM_ATTACK2_CHANCE_PCT = 3,
	DURIEL_AI_PARAM_CHARGE_CHANCE_PCT = 4,
};

//D2Game.0x6FCD8FE0
void __fastcall AITHINK_Fn044_Duriel(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pMonstatsTxt->nSkill[3] >= 0 && !UNITS_GetRightSkill(pUnit))
	{
		D2GAME_SetSkills_6FD14C60(pUnit, pAiTickParam->pMonstatsTxt->nSkill[3], AI_GetParamValue(pGame, pAiTickParam, DURIEL_AI_PARAM_HOLY_FREEZE_LEVEL), 1);
		D2GAME_AssignSkill_6FD13800(pUnit, 0, pAiTickParam->pMonstatsTxt->nSkill[3], -1);
	}

	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, DURIEL_AI_PARAM_SMITE_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
		}
		else if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, DURIEL_AI_PARAM_JAB_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
		}
		else if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DURIEL_AI_PARAM_ATTACK2_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, DURIEL_AI_PARAM_CHARGE_CHANCE_PCT))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
	}
	else
	{
		AITACTICS_SetVelocity(pUnit, 13, 0, 0);
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
	}
}

enum D2C_SarcophagusAIParams
{
	SARCOPHAGUS_AI_PARAM_SPAWN_INTERVAL = 0,
	SARCOPHAGUS_AI_PARAM_NUM_TO_SPAWN = 2,
};

//D2Game.0x6FCD91F0
void __fastcall AITHINK_Fn045_Sarcophagus(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->nTargetDistance > 25)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] <= AI_GetParamValue(pGame, pAiTickParam, SARCOPHAGUS_AI_PARAM_NUM_TO_SPAWN))
	{
		const int32_t nFrameDiff = std::abs(pGame->dwGameFrame - pAiTickParam->pAiControl->dwAiParam[0]);

		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && nFrameDiff >= AI_GetParamValue(pGame, pAiTickParam, SARCOPHAGUS_AI_PARAM_SPAWN_INTERVAL))
		{
			if (sub_6FC68350(MONSTER_SARCOPHAGUS, UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;
				++pAiTickParam->pAiControl->dwAiParam[1];
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed) % 10 + 20);
		return;
	}

	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_NOTC;
	}

	AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
}

enum D2C_ElementalBeastAIParams
{
	ELEMENTALBEAST_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	ELEMENTALBEAST_AI_PARAM_ACTIVATION_DISTANCE = 1,
	ELEMENTALBEAST_AI_PARAM_STALL_DURATION = 2,
};

//D2Game.0x6FCD93A0
void __fastcall AITHINK_Fn046_ElementalBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	switch (pAiTickParam->pAiControl->dwAiParam[0])
	{
	case 0:
		if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, ELEMENTALBEAST_AI_PARAM_ACTIVATION_DISTANCE) || sub_6FCF2E70(pUnit))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			return;
		}
		break;

	case 1:
		if (pAiTickParam->bCombat)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
			EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + 1, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[0] = 2;
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ELEMENTALBEAST_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 8u);
		}
		return;

	case 2:
		SUNITDMG_KillMonster(pGame, pUnit, pAiTickParam->pTarget, 1);
		return;

	default:
		break;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, ELEMENTALBEAST_AI_PARAM_STALL_DURATION));
}

enum D2C_FlyingScimitarAIParams
{
	FLYINGSCIMITAR_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	FLYINGSCIMITAR_AI_PARAM_ATTACK_CHANCE_PCT = 1,
	FLYINGSCIMITAR_AI_PARAM_STALL_DURATION = 2,
	FLYINGSCIMITAR_AI_PARAM_CIRCLE_CHANCE_PCT = 3,
};

//D2Game.0x6FCD94D0
void __fastcall AITHINK_Fn047_FlyingScimitar(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FLYINGSCIMITAR_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FLYINGSCIMITAR_AI_PARAM_CIRCLE_CHANCE_PCT))
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 2u, 0);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FLYINGSCIMITAR_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 8, 1);
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FLYINGSCIMITAR_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, FLYINGSCIMITAR_AI_PARAM_STALL_DURATION));
}

enum D2C_ZakarumZealotAIParams
{
	ZAKARUMZEALOT_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	ZAKARUMZEALOT_AI_PARAM_ATTACK2_CHANCE_PCT = 1,
	ZAKARUMZEALOT_AI_PARAM_HURT_PCT = 2,
	ZAKARUMZEALOT_AI_PARAM_RUN_CHANCE_PCT = 3,
};

//D2Game.0x6FCD9640
void __fastcall AITHINK_Fn048_ZakarumZealot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nVel = 0;

	D2UnitStrc* pTarget = pAiTickParam->pTarget;

	if (pAiTickParam->pMonstatsTxt->nVelocity > 0)
	{
		const int32_t nRatio = 100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity;
		if (nRatio > 100)
		{
			nVel = std::min(nRatio - 100, 120);
		}
	}

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
	if (pAiTickParam->pTarget)
	{
		if (pAiTickParam->pTarget->dwUnitType == UNIT_MONSTER)
		{
			int32_t nUnitGUID = 0;
			int32_t nUnitType = 0;
			AIGENERAL_GetOwnerData(pAiTickParam->pTarget, &nUnitGUID, &nUnitType);

			if (nUnitType == UNIT_PLAYER)
			{
				pTarget = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
			}
		}

		if (pTarget && pTarget->dwUnitType == UNIT_PLAYER && DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == ACT_III && QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pTarget)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q5, 0))
		{
			AITACTICS_SetVelocity(pUnit, 0, nVel, 0);

			if (!sub_6FCD06D0(pGame, pUnit, pAiTickParam->pTarget, 8u, 1))
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
			}
			return;
		}
	}

	if (sub_6FCF2E70(pUnit))
	{
		if (!pAiTickParam->pAiControl->dwAiParam[1] && nLifePercentage < AI_GetParamValue(pGame, pAiTickParam, ZAKARUMZEALOT_AI_PARAM_HURT_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;

			AITACTICS_SetVelocity(pUnit, 0, nVel, 0);
			if (sub_6FCD06D0(pGame, pUnit, pAiTickParam->pTarget, 8u, 1))
			{
				return;
			}

			pAiTickParam->pAiControl->dwAiParam[1] = 5;
		}

		if (COLLISION_CheckMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_MISSILE))
		{
			if (pAiTickParam->bCombat)
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
				return;
			}

			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[1])
	{
		--pAiTickParam->pAiControl->dwAiParam[1];
	}

	if (!pAiTickParam->bCombat)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMZEALOT_AI_PARAM_RUN_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 0, nVel, 0);
			AITACTICS_RunToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0])
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMZEALOT_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;

			if (AI_RollPercentage(pUnit) >= 80)
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
			return;
		}
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 1;

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMZEALOT_AI_PARAM_ATTACK2_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
	}
	else
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
	}
}

enum D2C_ZakarumPriestAIParams
{
	ZAKARUMPRIEST_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	ZAKARUMPRIEST_AI_PARAM_BLIZZARD_CHANCE_PCT = 1,
	ZAKARUMPRIEST_AI_PARAM_LIGHTNING_CHANCE_PCT = 2,
	ZAKARUMPRIEST_AI_PARAM_CAST_CHANCE_PCT = 3,
	ZAKARUMPRIEST_AI_PARAM_SPELL_TIMER = 4,
	ZAKARUMPRIEST_AI_PARAM_HEAL_RANGE = 5,
};

//D2Game.0x6FCD9A10
void __fastcall AITHINK_Fn049_ZakarumPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
	if (pAiTickParam->bCombat || sub_6FCF2E70(pUnit))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && nLifePercentage < 33 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[0])
		{
			pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + 4 * AI_GetParamValue(pGame, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_SPELL_TIMER);

			int32_t nX = CLIENTS_GetUnitX(pUnit);
			int32_t nY = CLIENTS_GetUnitY(pUnit);
			const int32_t nTargetX = CLIENTS_GetUnitX(pAiTickParam->pTarget);
			const int32_t nTargetY = CLIENTS_GetUnitY(pAiTickParam->pTarget);

			int32_t nShift = 0;
			if (pAiTickParam->bCombat)
			{
				nShift = 2;
			}

			nX = ((nTargetX - nX) << nShift) + nTargetX;
			nY = ((nTargetY - nY) << nShift) + nTargetY;

			if (sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[2], 0, nX, nY))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], 0, nX, nY);
				return;
			}
		}

		if (pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}

	D2ZakarumPriestAiCallbackArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nCounter = 0;
	arg.nMaxLifePercentage = INT_MAX;
	arg.nMaxDistance = AI_GetParamValue(pGame, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_HEAL_RANGE) * AI_GetParamValue(pGame, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_HEAL_RANGE);
	sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_ZakarumPriest, 1);

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && arg.pTarget && AI_RollPercentage(pUnit) < 25)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], arg.pTarget, 0, 0);
		return;
	}

	if (UNITS_TestCollisionWithUnit(pUnit, pAiTickParam->pTarget, COLLIDE_MISSILE_BARRIER))
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_ATTACK_CHANCE_PCT) && pAiTickParam->pMonstatsTxt->nSkill[3] >= 0
			&& pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_BLIZZARD_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_SPELL_TIMER);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_CAST_CHANCE_PCT))
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[3] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_BLIZZARD_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_SPELL_TIMER);
				return;
			}

			if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[2] && AIRollChanceParam(pGame, pUnit, pAiTickParam, ZAKARUMPRIEST_AI_PARAM_LIGHTNING_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + 20;
				return;
			}
		}
	}

	if (AI_RollPercentage(pUnit) >= 30)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
	}
	else
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
	}
}

//D2Game.0x6FCD9F10
D2UnitStrc* __fastcall AITHINK_TargetCallback_ZakarumPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2ZakarumPriestAiCallbackArgStrc* pArg = (D2ZakarumPriestAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || !STATLIST_AreUnitsAligned(pUnit, pTarget) || STATLIST_GetUnitAlignment(pTarget) == UNIT_ALIGNMENT_GOOD || MONSTERS_IsDead(pTarget))
	{
		return nullptr;
	}

	int32_t nBaseId = pTarget->dwClassId;
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
	if (pMonStatsTxtRecord)
	{
		nBaseId = pMonStatsTxtRecord->nBaseId;
	}

	if ((nBaseId != MONSTER_ZEALOT1 && nBaseId != MONSTER_CANTOR1) || AITHINK_GetSquaredDistance(pUnit, pTarget) > pArg->nMaxDistance)
	{
		return nullptr;
	}

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pTarget);
	if (nLifePercentage > 60)
	{
		return nullptr;
	}

	if (pArg->nMaxLifePercentage <= 0)
	{
		pArg->nMaxLifePercentage = INT_MAX;
	}

	++pArg->nCounter;

	if (nLifePercentage < pArg->nMaxLifePercentage)
	{
		pArg->pTarget = pTarget;
	}

	return nullptr;
}

enum D2C_MephistoAIParams
{
	MEPHISTO_AI_PARAM_STATE_2_OR_3_CHANCE_PCT = 0,
	MEPHISTO_AI_PARAM_1 = 1,
	MEPHISTO_AI_PARAM_2 = 2,
};

//D2Game.0x6FCDA0C0
D2UnitStrc* __fastcall AITHINK_FindTargetForMephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MonStatsTxt* pMonStatsTxtRecord, D2UnitStrc* a4)
{
	D2MephistoAiCallbackArgStrc arg = {};
	arg.unk0x04 = INT_MAX;
	arg.unk0x10 = INT_MAX;
	arg.unk0x18 = INT_MAX;

	sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Mephisto, 1);

	if (!arg.unk0x0C || AI_RollPercentage(pUnit) >= pMonStatsTxtRecord->wAiParam[MEPHISTO_AI_PARAM_1][pGame->nDifficulty])
	{
		if (!arg.unk0x14 || AI_RollPercentage(pUnit) >= pMonStatsTxtRecord->wAiParam[MEPHISTO_AI_PARAM_2][pGame->nDifficulty])
		{
			return a4;
		}
	}

	return arg.unk0x0C;
}

//D2Game.0x6FCDA190
D2UnitStrc* __fastcall AITHINK_TargetCallback_Mephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2MephistoAiCallbackArgStrc* pArg = (D2MephistoAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || SUNIT_IsDead(pTarget) || !pTarget || !(pTarget->dwFlags & UNITFLAG_CANBEATTACKED) || !sub_6FCBD900(pGame, pUnit, pTarget))
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);

	if (nDistance > 1024)
	{
		return nullptr;
	}

	++pArg->nCounter;

	if (nDistance <= 225)
	{
		++pArg->unk0x08;
	}

	if (nDistance < pArg->unk0x04)
	{
		pArg->unk0x00 = pTarget;
		pArg->unk0x04 = nDistance;
	}

	const int32_t nLife = STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0) >> 8;
	if (nLife < pArg->unk0x10)
	{
		pArg->unk0x0C = pTarget;
		pArg->unk0x10 = nLife;
	}

	if (nDistance < pArg->unk0x18 && pUnit->dwUnitType == UNIT_PLAYER)
	{
		pArg->unk0x14 = pTarget;
		pArg->unk0x18 = nDistance;
	}

	return nullptr;
}

//D2Game.0x6FCDA300
void __fastcall AITHINK_Fn050_Mephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
	const int32_t nLifeChance = std::max((100 - nLifePercentage) / 5, 0);
	int32_t nAiState = pAiTickParam->pAiControl->dwAiParam[2];

	if (pAiTickParam->bCombat)
	{
		if (AI_RollPercentage(pUnit) <= AI_GetParamValue(pGame, pAiTickParam, MEPHISTO_AI_PARAM_STATE_2_OR_3_CHANCE_PCT))
		{
			nAiState = 2;
			pAiTickParam->pAiControl->dwAiParam[0] = ITEMS_RollRandomNumber(&pUnit->pSeed) % 3 + 3;
		}
		else
		{
			nAiState = 3;
		}
	}
	else
	{
		if (pAiTickParam->nTargetDistance <= 20)
		{
			if (!pAiTickParam->pAiControl->dwAiParam[2])
			{
				if (nLifePercentage <= 20 && pAiTickParam->nTargetDistance < 5 && (AI_RollPercentage(pUnit) < 40))
				{
					nAiState = 1;
				}
				else
				{
					if (AI_RollPercentage(pUnit) >= nLifeChance + 50)
					{
						if (AI_RollPercentage(pUnit) >= 65)
						{
							AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
						}
						else
						{
							if (AI_RollPercentage(pUnit) < 65 || pAiTickParam->nTargetDistance > 5)
							{
								sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
							}
							else
							{
								AITACTICS_SetVelocity(pUnit, 0, 0, 4u);
								AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
							}
						}

						pAiTickParam->pAiControl->dwAiParam[2] = 0;
						return;
					}

					nAiState = 2;
					pAiTickParam->pAiControl->dwAiParam[0] = ITEMS_RollRandomNumber(&pUnit->pSeed) % 3 + 3;
				}
			}
		}
		else
		{
			nAiState = 4;
		}
	}

	switch (nAiState)
	{
	case 0:
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		pAiTickParam->pAiControl->dwAiParam[2] = 4;
		return;

	case 1:
		nAiState = 0;
		AITACTICS_SetVelocity(pUnit, 0, 50, 0);

		if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 8u, 1))
		{
			pAiTickParam->pAiControl->dwAiParam[2] = nAiState;
			return;
		}

		if (pAiTickParam->bCombat)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);

			pAiTickParam->pAiControl->dwAiParam[2] = nAiState;
			return;
		}

		break;

	case 2:
		break;

	case 3:
		if (AI_RollPercentage(pUnit) >= nLifeChance + 80)
		{
			AITACTICS_SetVelocity(pUnit, 0, 50, 0);

			if (!sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 1))
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 12);
			}
		}
		else
		{
			if (AI_RollPercentage(pUnit) >= 80 - nLifeChance)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
		}

		pAiTickParam->pAiControl->dwAiParam[2] = 0;
		return;

	case 4:
		AITACTICS_SetVelocity(pUnit, 0, 50, 0);

		if (!D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pAiTickParam->pTarget, 6u))
		{
			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

			if (!AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 12, 6))
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 12);
			}
		}

		pAiTickParam->pAiControl->dwAiParam[2] = 0;
		return;


	default:
		if (AI_RollPercentage(pUnit) >= 65)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			if (AI_RollPercentage(pUnit) < 65 || pAiTickParam->nTargetDistance > 5)
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			}
			else
			{
				AITACTICS_SetVelocity(pUnit, 0, 0, 4u);
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			}
		}

		pAiTickParam->pAiControl->dwAiParam[2] = 0;
		return;
	}

	--pAiTickParam->pAiControl->dwAiParam[0];

	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		nAiState = 0;
	}

	if (!pAiTickParam->pAiControl->dwAiParam[1])
	{
		pAiTickParam->pAiControl->dwAiParam[1] = 2;

		if (!sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 1))
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 12);
		}

		pAiTickParam->pAiControl->dwAiParam[2] = nAiState;
		return;
	}

	++pAiTickParam->pAiControl->dwAiParam[1];

	D2UnitStrc* pTarget = AITHINK_FindTargetForMephisto(pGame, pUnit, pAiTickParam->pMonstatsTxt, pAiTickParam->pTarget);

	int32_t nValidSkills = 0;
	while (nValidSkills < 8 && pAiTickParam->pMonstatsTxt->nSkill[nValidSkills] >= 0)
	{
		++nValidSkills;
	}

	if (nValidSkills <= 0)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
	}
	else
	{
		const int32_t nSkillChance = 100 / nValidSkills;
		const uint32_t nRand = AI_RollPercentage(pUnit);

		int32_t bUseBlizzard = 0;
		if (pGame->nDifficulty && pTarget && UNITS_TestCollisionWithUnit(pUnit, pTarget, COLLIDE_MISSILE_BARRIER))
		{
			bUseBlizzard = 1;
		}

		if (pGame->nDifficulty && (bUseBlizzard || pAiTickParam->nTargetDistance > 30))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[5], pAiTickParam->pMonstatsTxt->nSkill[5], pTarget, 0, 0);
		}
		else if (pAiTickParam->nTargetDistance < 15 && nRand < nSkillChance && pGame->nDifficulty)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pTarget, 0, 0);
		}
		else if (nRand < 2 * nSkillChance)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
		}
		else if (nRand < 3 * nSkillChance)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pTarget, 0, 0);
		}
		else
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		}
	}

	if (AI_RollPercentage(pUnit) < 50 - nLifeChance)
	{
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = nAiState;
}

//D2Game.0x6FCDA910
void __fastcall D2GAME_AI_Unk052_6FCDA910(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nAiSpecialState = pAiTickParam->pAiControl->nAiSpecialState;

	if (nAiSpecialState == AISPECIALSTATE_DIMVISION || nAiSpecialState == AISPECIALSTATE_TERROR || pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || pAiTickParam->pAiControl->dwAiParam[2] < 2)
	{
		AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, pAiTickParam->pAiControl->nAiSpecialState);
		AITACTICS_Idle(pGame, pUnit, 1);
		return;
	}

	AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);

	pAiTickParam->pAiControl->dwAiParam[2] = 1;

	if (pUnit)
	{
		pUnit->dwFlags &= 0xFFFFFFF1;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	COLLISION_ResetMask(pRoom, nX, nY, COLLIDE_NO_PATH);
	COLLISION_ResetMask(pRoom, nX, nY, COLLIDE_MONSTER);
	PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
	PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
	AITACTICS_Idle(pGame, pUnit, 12);
}

enum D2C_FrogDemonAIParams
{
	FROGDEMON_AI_PARAM_ATTACK_MELEE_CHANCE_PCT = 0,
	FROGDEMON_AI_PARAM_SHOOT_MELEE_CHANCE_PCT = 1,
	FROGDEMON_AI_PARAM_CIRCLE_MELEE_CHANCE_PCT = 2,
	FROGDEMON_AI_PARAM_CIRCLE_RANGED_CHANCE_PCT = 3,
	FROGDEMON_AI_PARAM_SHOOT_CHANCE_PCT = 4,
	FROGDEMON_AI_PARAM_SHOOT_DISTANCE = 5,
	FROGDEMON_AI_PARAM_STALL_DURATION = 6,
	FROGDEMON_AI_PARAM_EMERGE_DISTANCE = 7,
};

//D2Game.0x6FCDAAA0
void __fastcall AITHINK_Fn052_FrogDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	switch (pAiTickParam->pAiControl->dwAiParam[2])
	{
	case 0:
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance > 12)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			AITACTICS_Idle(pGame, pUnit, 8);
			pAiTickParam->pAiControl->dwAiParam[2] = 1;

			pUnit->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);

			COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_NO_PATH);
			COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_MONSTER);
			PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
			PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
			AITACTICS_Idle(pGame, pUnit, 12);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && sub_6FCD55D0(pGame, pUnit))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pUnit, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[2] = 2;
			return;
		}

		if (!COLLISION_CheckMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_WALL | COLLIDE_OBJECT | COLLIDE_DOOR))
		{
			pAiTickParam->pAiControl->dwAiParam[2] = 2;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 12);
		return;

	case 1:
		if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0)
		{
			if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, FROGDEMON_AI_PARAM_EMERGE_DISTANCE) && sub_6FCD55D0(pGame, pUnit))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pUnit, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[2] = 2;
				return;
			}

			if (pAiTickParam->nTargetDistance < 20 && pAiTickParam->pAiControl->dwAiParam[1] > 64 && sub_6FCD55D0(pGame, pUnit))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pUnit, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[2] = 2;
				return;
			}
		}

		if (pUnit)
		{
			pUnit->dwFlags &= 0xFFFFFFF1;
		}

		COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_NO_PATH);
		COLLISION_ResetMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), COLLIDE_MONSTER);
		PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
		PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
		AITACTICS_Idle(pGame, pUnit, 12);
		AITACTICS_Idle(pGame, pUnit, 24);
		pAiTickParam->pAiControl->dwAiParam[2] = 1;
		++pAiTickParam->pAiControl->dwAiParam[1];
		return;

	default:
		if (!pAiTickParam->pTarget)
		{
			AITACTICS_Idle(pGame, pUnit, 32);
			return;
		}

		if (pAiTickParam->bCombat)
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FROGDEMON_AI_PARAM_SHOOT_MELEE_CHANCE_PCT))
			{
				if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FROGDEMON_AI_PARAM_ATTACK_MELEE_CHANCE_PCT))
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
					return;
				}

				if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FROGDEMON_AI_PARAM_CIRCLE_MELEE_CHANCE_PCT))
				{
					AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, 6));
					return;
				}

				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
				return;
			}
		}
		else
		{
			if (pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, FROGDEMON_AI_PARAM_SHOOT_DISTANCE))
			{
				if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FROGDEMON_AI_PARAM_CIRCLE_RANGED_CHANCE_PCT))
				{
					AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 4u);
					return;
				}

				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
				return;
			}

			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FROGDEMON_AI_PARAM_SHOOT_CHANCE_PCT))
			{
				if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FROGDEMON_AI_PARAM_CIRCLE_RANGED_CHANCE_PCT))
				{
					sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
					return;
				}

				AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, FROGDEMON_AI_PARAM_STALL_DURATION));
				return;
			}
		}

		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
	}
}

enum D2C_SummonerAIParams
{
	SUMMONER_AI_PARAM_CAST_CHANCE_PCT = 0,
	SUMMONER_AI_PARAM_WEAKEN_CHANCE_PCT = 1,
	SUMMONER_AI_PARAM_PREF_ELEMENT_CHANCE_PCT = 2,
	SUMMONER_AI_PARAM_NOVA_TIMER = 3,
	SUMMONER_AI_PARAM_FIREWALL_TIMER = 4,
	SUMMONER_AI_PARAM_WALK_AWAY_CHANCE_PCT = 5,
	SUMMONER_AI_PARAM_NOVA_DISTANCE = 6,
	SUMMONER_AI_PARAM_MISSILE_DISTANCE = 7,
};

//D2Game.0x6FCDAFC0
void __fastcall AITHINK_Fn053_Summoner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{	
	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		ACT2Q5_OnSummonerActivated(pGame);
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
	}

	if (pAiTickParam->nTargetDistance < 5 && AIRollChanceParam(pGame, pUnit, pAiTickParam, SUMMONER_AI_PARAM_WALK_AWAY_CHANCE_PCT))
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 6u, 0);
	}

	int32_t bUseColdSkill = STATLIST_UnitGetStatValue(pAiTickParam->pTarget, STAT_FIRERESIST, 0) >= STATLIST_UnitGetStatValue(pAiTickParam->pTarget, STAT_COLDRESIST, 0);

	const int32_t nX = CLIENTS_GetUnitX(pAiTickParam->pTarget);
	const int32_t nY = CLIENTS_GetUnitY(pAiTickParam->pTarget);

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUMMONER_AI_PARAM_CAST_CHANCE_PCT))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[4] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, SUMMONER_AI_PARAM_WEAKEN_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pAiTickParam->pTarget, nX, nY);
			return;
		}

		int32_t nDistance = 0;
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, nullptr);

		if (AI_RollPercentage(pUnit) > AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_PREF_ELEMENT_CHANCE_PCT))
		{
			bUseColdSkill = (bUseColdSkill == 0);
		}

		if (bUseColdSkill)
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_NOVA_DISTANCE))
			{
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_NOVA_TIMER);
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, nX, nY);
				return;
			}

			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pTarget && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_MISSILE_DISTANCE))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, nX, nY);
				return;
			}
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[3] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[2])
		{
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_FIREWALL_TIMER);
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, nX, nY);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && pTarget && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_MISSILE_DISTANCE))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pTarget, nX, nY);
			return;
		}

		if (!bUseColdSkill && pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_NOVA_DISTANCE))
		{
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SUMMONER_AI_PARAM_NOVA_TIMER);
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, nX, nY);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[4] >= 0)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pAiTickParam->pTarget, nX, nY);
			return;
		}
	}

	AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
}

enum D2C_IzualAIParams
{
	IZUAL_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	IZUAL_AI_PARAM_ENGAGE_CHANCE_PCT = 1,
	IZUAL_AI_PARAM_NOVA_AT_RANGE_CHANCE_PCT = 2,
	IZUAL_AI_PARAM_NOVA_IN_MELEE_CHANCE_PCT = 3,
	IZUAL_AI_PARAM_POST_NOVA_DOLDRUMS = 4,
	IZUAL_AI_PARAM_NUM_SWINGS = 5,
};

//D2Game.0x6FCDB3E0
void __fastcall AITHINK_Fn055_Izual(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nX = CLIENTS_GetUnitX(pAiTickParam->pTarget);
	const int32_t nY = CLIENTS_GetUnitY(pAiTickParam->pTarget);

	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		ACT4Q1_OnIzualActivated(pGame);
	}

	if (pAiTickParam->pAiControl->dwAiParam[1])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiTickParam->pAiControl->dwAiParam[1]);
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		return;
	}

	if (!pAiTickParam->bCombat)
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance < 10)
		{
			if (pAiTickParam->pAiControl->dwAiParam[2] > 0)
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
				return;
			}

			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, IZUAL_AI_PARAM_NOVA_AT_RANGE_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, nX, nY);
				pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, IZUAL_AI_PARAM_POST_NOVA_DOLDRUMS);
				pAiTickParam->pAiControl->dwAiParam[2] = AI_GetParamValue(pGame, pAiTickParam, IZUAL_AI_PARAM_NUM_SWINGS);
				return;
			}
		}

		if (pAiTickParam->pAiControl->dwAiParam[2] <= 0)
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, IZUAL_AI_PARAM_ENGAGE_CHANCE_PCT))
			{
				if (pAiTickParam->nTargetDistance <= 10)
				{
					AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiTickParam->pMonstatsTxt->nAIdel[pGame->nDifficulty]);
				}
				else
				{
					AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 6, 9);
				}
				return;
			}
		}

		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[2] <= 0)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, IZUAL_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
			if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || !AIRollChanceParam(pGame, pUnit, pAiTickParam, IZUAL_AI_PARAM_NOVA_IN_MELEE_CHANCE_PCT))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiTickParam->pMonstatsTxt->nAIdel[pGame->nDifficulty]);
			}
			else
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, nX, nY);
				pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, IZUAL_AI_PARAM_POST_NOVA_DOLDRUMS);
				pAiTickParam->pAiControl->dwAiParam[2] = AI_GetParamValue(pGame, pAiTickParam, IZUAL_AI_PARAM_NUM_SWINGS);
			}
			return;
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[2] > 0)
	{
		--pAiTickParam->pAiControl->dwAiParam[2];
	}

	AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
}

enum D2C_TentacleAIParams
{
	TENTACLE_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	TENTACLE_AI_PARAM_SUBMERGE_CHANCE_PCT = 1,
	TENTACLE_AI_PARAM_SUBMERGE_DURATION_SECONDS = 2,
	TENTACLE_AI_PARAM_EMERGE_DURATION_SECONDS = 3,
	TENTACLE_AI_PARAM_STALL_DURATION = 4,
	TENTACLE_AI_PARAM_ACTIVE_DISTANCE = 5,
};

//D2Game.0x6FCDB720
void __fastcall AITHINK_Fn056_Tentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		SUNITDMG_KillMonster(pGame, pUnit, 0, 1);
		return;
	}

	if (pOwner->dwAnimMode == MONMODE_DEAD && AI_RollPercentage(pUnit) < 40)
	{
		SUNITDMG_KillMonster(pGame, pUnit, SUNIT_GetTargetUnit(pGame, pOwner), 1);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		if (!pAiTickParam->pAiControl->dwAiParam[2])
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			AITACTICS_Idle(pGame, pUnit, 8);
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_SUBMERGE_DURATION_SECONDS);
			pAiTickParam->pAiControl->dwAiParam[2] = 1;
			return;
		}

		if (pAiTickParam->pAiControl->dwAiParam[2] == 2 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1])
		{
			if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_ACTIVE_DISTANCE)
				|| !pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, TENTACLE_AI_PARAM_SUBMERGE_CHANCE_PCT)
				|| pOwner->dwAnimMode == MONMODE_SEQUENCE && (AI_RollPercentage(pUnit) < 50))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				AITACTICS_Idle(pGame, pUnit, 8);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_SUBMERGE_DURATION_SECONDS);
				pAiTickParam->pAiControl->dwAiParam[2] = 1;
				return;
			}
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0)
	{
		if (pAiTickParam->pAiControl->dwAiParam[2] != 1)
		{
			if (pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, TENTACLE_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_STALL_DURATION));
			}
			return;
		}

		if (pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1])
		{
			if (pAiTickParam->bCombat || pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_ACTIVE_DISTANCE) || pOwner->dwAnimMode != MONMODE_SEQUENCE && (AI_RollPercentage(pUnit) < 5))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pUnit, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_EMERGE_DURATION_SECONDS);
				pAiTickParam->pAiControl->dwAiParam[2] = 2;
				return;
			}
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[2] == 1)
	{
		AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_STALL_DURATION));
		return;
	}

	if (pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, TENTACLE_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_STALL_DURATION));
}

//D2Game.0x6FCDBAA0
void __fastcall AITHINK_Fn057_TentacleHead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		if (!pAiTickParam->pAiControl->dwAiParam[2])
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			AITACTICS_Idle(pGame, pUnit, 8);
			pAiTickParam->pAiControl->dwAiParam[2] = 1;
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_SUBMERGE_DURATION_SECONDS);
			return;
		}

		if (pAiTickParam->pAiControl->dwAiParam[2] == 2 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1])
		{
			if (nDistance > AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_ACTIVE_DISTANCE))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				AITACTICS_Idle(pGame, pUnit, 20);
				pAiTickParam->pAiControl->dwAiParam[2] = 1;
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_SUBMERGE_DURATION_SECONDS);
				return;
			}

			if (!pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, 1))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				AITACTICS_Idle(pGame, pUnit, 20);
				pAiTickParam->pAiControl->dwAiParam[2] = 1;
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_SUBMERGE_DURATION_SECONDS);
				return;
			}
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && pAiTickParam->pAiControl->dwAiParam[2] == 1 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1] && (pAiTickParam->bCombat || nDistance < AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_ACTIVE_DISTANCE)))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pUnit, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[2] = 2;
		pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_EMERGE_DURATION_SECONDS);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[2] != 1)
	{
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, TENTACLE_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}
	}

	AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TENTACLE_AI_PARAM_STALL_DURATION));
}

enum D2C_GargoyleTrapAIParams
{
	GORGOYLETRAP_AI_PARAM_SHOOT_DISTANCE = 0,
	GORGOYLETRAP_AI_PARAM_SHOOT_CHANCE_PCT = 1,
	GORGOYLETRAP_AI_PARAM_FIRE_RECOVERY = 2,
	GORGOYLETRAP_AI_PARAM_STALL_DURATION = 3,
};

//D2Game.0x6FCDBCE0
void __fastcall AITHINK_Fn063_GargoyleTrap(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const char byte_6FD29400[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0
	};

	const char byte_6FD29440[] =
	{
		31, 49, 0, 17
	};

	if (pAiTickParam->pAiControl->dwAiParam[0] > 0)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiTickParam->pAiControl->dwAiParam[0]);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}

	UNITS_SetTargetUnitForDynamicUnit(pUnit, pAiTickParam->pTarget);

	const int32_t nTargetX = CLIENTS_GetUnitX(pAiTickParam->pTarget);
	const int32_t nTargetY = CLIENTS_GetUnitY(pAiTickParam->pTarget);
	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);
	const int32_t nXDiff = std::abs(nTargetX - nX);
	const int32_t nYDiff = std::abs(nTargetY - nY);

	if (nXDiff >= nYDiff)
	{
		PATH_SetDirection(pUnit->pDynamicPath, byte_6FD29440[byte_6FD29400[UNITS_GetDirectionToCoords(pUnit, nTargetX, nY)]]);
	}
	else
	{
		PATH_SetDirection(pUnit->pDynamicPath, byte_6FD29440[byte_6FD29400[UNITS_GetDirectionToCoords(pUnit, nX, nTargetY)]]);
	}

	if (nXDiff < 6 || nYDiff < 6)
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, GORGOYLETRAP_AI_PARAM_SHOOT_DISTANCE)
			&& AIRollChanceParam(pGame, pUnit, pAiTickParam, GORGOYLETRAP_AI_PARAM_SHOOT_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[0] = AI_GetParamValue(pGame, pAiTickParam, GORGOYLETRAP_AI_PARAM_FIRE_RECOVERY);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, GORGOYLETRAP_AI_PARAM_STALL_DURATION));
}

enum D2C_SkeletonMageAIParams
{
	SKELETONMAGE_AI_PARAM_SHOOT_CHANCE_PCT = 0,
	SKELETONMAGE_AI_PARAM_APPROACH_DISTANCE = 1,
	SKELETONMAGE_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	SKELETONMAGE_AI_PARAM_TOO_CLOSE_DISTANCE = 3,
	SKELETONMAGE_AI_PARAM_WALK_AWAY_CHANCE_PCT = 4,
	SKELETONMAGE_AI_PARAM_FIRE_DIST = 5,
	SKELETONMAGE_AI_PARAM_CIRCLE_CHANCE_PCT = 6,
	SKELETONMAGE_AI_PARAM_STALL_DURATION = 7,
};

//D2Game.0x6FCDBF20
void __fastcall AITHINK_Fn064_SkeletonMage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	if (pTarget)
	{
		if (nDistance > AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_APPROACH_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONMAGE_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 0, 10, 0);
			AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pTarget, AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_APPROACH_DISTANCE));
			return;
		}

		if (nDistance <= AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_TOO_CLOSE_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONMAGE_AI_PARAM_WALK_AWAY_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 0, 25, 0);
			if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pTarget, 5u, 1))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			return;
		}

		if (nDistance < AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_FIRE_DIST) && AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONMAGE_AI_PARAM_SHOOT_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}
	}

	if (nDistance <= AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_APPROACH_DISTANCE) || !AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONMAGE_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, SKELETONMAGE_AI_PARAM_CIRCLE_CHANCE_PCT))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_STALL_DURATION));
		}
		else
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		}
	}
	else
	{
		AITACTICS_SetVelocity(pUnit, 0, 10, 0);
		AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, AI_GetParamValue(pGame, pAiTickParam, SKELETONMAGE_AI_PARAM_APPROACH_DISTANCE));
	}
}

//D2Game.0x6FCDC170
void __fastcall D2GAME_AI_SpecialState04_6FCDC170(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, pAiTickParam->pAiControl->nAiSpecialState);
	AITACTICS_IdleInNeutralMode(pGame, pUnit, 1);
}

enum D2C_FetishShamanAIParams
{
	FETISHSHAMAN_AI_PARAM_HEAL_CHANCE_PCT = 0,
	FETISHSHAMAN_AI_PARAM_HEAL_CAPABILITY = 1,
	FETISHSHAMAN_AI_PARAM_HEAL_RANGE = 2,
	FETISHSHAMAN_AI_PARAM_CIRCLE_CHANCE_PCT = 3,
	FETISHSHAMAN_AI_PARAM_HEAL_SEARCH_RANGE = 4,
};

//D2Game.0x6FCDC1C0
void __fastcall AITHINK_Fn065_FetishShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nSkillLevel = 1;

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], -1);
		if (pSkill)
		{
			nSkillLevel = std::max(SKILLS_GetSkillLevel(pUnit, pSkill, 1), 1);
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || pAiTickParam->nTargetDistance >= nSkillLevel || STATES_CheckState(pUnit, STATE_INFERNO))
	{
		if (STATES_CheckState(pUnit, STATE_INFERNO))
		{
			STATES_ToggleState(pUnit, STATE_INFERNO, 0);
		}

		D2FetishShamanAiCallbackArgStrc arg = {};
		arg.nSquaredDistanceToClosestDeadTarget = INT_MAX;
		arg.nMaxSearchRangeSquared = AI_GetParamValue(pGame, pAiTickParam, FETISHSHAMAN_AI_PARAM_HEAL_SEARCH_RANGE) * AI_GetParamValue(pGame, pAiTickParam, FETISHSHAMAN_AI_PARAM_HEAL_SEARCH_RANGE);
		arg.nSquaredDistanceToClosestAliveTarget = INT_MAX;
		arg.nHealCapability = AI_GetParamValue(pGame, pAiTickParam, FETISHSHAMAN_AI_PARAM_HEAL_CAPABILITY);
		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_FetishShaman, 1);

		if (arg.pClosestDeadTarget && AIRollChanceParam(pGame, pUnit, pAiTickParam, FETISHSHAMAN_AI_PARAM_HEAL_CHANCE_PCT) && sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[2], arg.pClosestDeadTarget, 0, 0))
		{
			D2AiCmdStrc aiCmd = {};
			aiCmd.nCmdParam[0] = 14;
			aiCmd.nCmdParam[1] = arg.pClosestDeadTarget->dwUnitId;
			aiCmd.nCmdParam[2] = 1;
			AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);

			if (arg.nSquaredDistanceToClosestDeadTarget <= AI_GetParamValue(pGame, pAiTickParam, FETISHSHAMAN_AI_PARAM_HEAL_RANGE))
			{
				AITACTICS_UseSequenceSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[2], arg.pClosestDeadTarget, 0, 0);
			}
			else
			{
				D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, arg.pClosestDeadTarget, 10);
			}
		}
		else
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, FETISHSHAMAN_AI_PARAM_CIRCLE_CHANCE_PCT))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
			else
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
			}
		}
	}
	else
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 1;

		if (pAiTickParam->pTarget)
		{
			aiCmd.nCmdParam[1] = pAiTickParam->pTarget->dwUnitId;
			aiCmd.nCmdParam[2] = pAiTickParam->pTarget->dwUnitType;
		}
		else
		{
			aiCmd.nCmdParam[1] = -1;
			aiCmd.nCmdParam[2] = 6;
		}

		AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
	}
}

//D2Game.0x6FCDC420
D2UnitStrc* __fastcall AITHINK_TargetCallback_FetishShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2FetishShamanAiCallbackArgStrc* pArg = (D2FetishShamanAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || !(pTarget->dwFlags & UNITFLAG_TARGETABLE) || STATES_CheckStateMaskUdeadOnUnit(pTarget) || (!pArg->nHealCapability && AIGENERAL_GetMinionOwner(pTarget) != pUnit))
	{
		return nullptr;
	}

	int32_t nClassId = pTarget->dwClassId;
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
	if (pMonStatsTxtRecord)
	{
		nClassId = pMonStatsTxtRecord->nBaseId;
	}

	if ((nClassId == MONSTER_FETISH1 || nClassId == MONSTER_FETISHBLOW1 && pArg->nHealCapability != 1) 
		&& STATLIST_GetUnitAlignment(pTarget) == UNIT_ALIGNMENT_EVIL 
		&& (pArg->nHealCapability >= 3 || !MONSTERUNIQUE_CheckMonTypeFlag(pTarget, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_CHAMPION)))
	{
		const int32_t nDistanceSquared = AITHINK_GetSquaredDistance(pUnit, pTarget);
		if (nDistanceSquared <= pArg->nMaxSearchRangeSquared)
		{
			if (pTarget->dwAnimMode == MONMODE_DEAD)
			{
				if (nDistanceSquared < pArg->nSquaredDistanceToClosestDeadTarget)
				{
					pArg->pClosestDeadTarget = pTarget;
					pArg->nSquaredDistanceToClosestDeadTarget = nDistanceSquared;
				}
			}
			else
			{
				++pArg->nAliveTargets;

				if (nDistanceSquared < pArg->nSquaredDistanceToClosestAliveTarget)
				{
					pArg->pClosestAliveTarget = pTarget;
					pArg->nSquaredDistanceToClosestAliveTarget = nDistanceSquared;
				}
			}
		}
	}

	return nullptr;
}

enum D2C_SandMaggotQueenAIParams
{
	SANDMAGGOTQUEEN_AI_PARAM_MAX_SPAWN = 0,
	SANDMAGGOTQUEEN_AI_PARAM_DELAY = 1,
};

//D2Game.0x6FCDC600
void __fastcall AITHINK_Fn066_SandMaggotQueen(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[2])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, DEFAULT_FRAMES_PER_SECOND * AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOTQUEEN_AI_PARAM_DELAY));
		pAiTickParam->pAiControl->dwAiParam[2] = 0;
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1])
	{
		int32_t nSpawnMode = 0;
		int32_t nMonsterId = 0;
		int32_t nX = 0;
		int32_t nY = 0;
		MONSTERS_GetMinionSpawnInfo(pUnit, &nMonsterId, &nX, &nY, &nSpawnMode, 0, AIGENERAL_GetMinionSpawnClassId);

		D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
		if (pRoom)
		{
			D2UnitStrc* pSandMaggot = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nSpawnMode, 2, 66);
			if (pSandMaggot)
			{
				pSandMaggot->dwFlags |= UNITFLAG_NOXP;
				++pAiTickParam->pAiControl->dwAiParam[0];
			}
		}

		AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOTQUEEN_AI_PARAM_DELAY));
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		pAiTickParam->pAiControl->dwAiParam[2] = 1;
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] < AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOTQUEEN_AI_PARAM_MAX_SPAWN))
	{
		AITACTICS_ChangeModeAndTargetCoordinatesOneStep(pGame, pUnit, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 8);
		AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SANDMAGGOTQUEEN_AI_PARAM_DELAY));
		pAiTickParam->pAiControl->dwAiParam[1] = 1;
	}
}

enum D2C_VileMotherAIParams
{
	VILEMOTHER_AI_PARAM_MAX_SPAWN = 0,
	VILEMOTHER_AI_PARAM_MAX_AT_ONCE = 1,
	VILEMOTHER_AI_PARAM_SPAWN_CHANCE_PCT = 2,
	VILEMOTHER_AI_PARAM_ATTACK_CHANCE_PCT = 3,
	VILEMOTHER_AI_PARAM_APPROACH_CHANCE_PCT = 4,
	VILEMOTHER_AI_PARAM_CIRCLE_CHANCE_PCT = 5,
	VILEMOTHER_AI_PARAM_STALL_DURATION = 6,
};

//D2Game.0x6FCDC840
void __fastcall AITHINK_Fn068_VileMother(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nClassId = pUnit ? pUnit->dwClassId : -1;
	const int32_t nMonstersInChain = DATATBLS_GetMonsterChainInfo(nClassId, 0, 0);

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_VILECHILD1);
	int32_t nLastInClass = -1;
	if (pMonStatsTxtRecord)
	{
		nLastInClass = pMonStatsTxtRecord->nBaseId;
		for (int32_t i = 0; i < nMonstersInChain; ++i)
		{
			nLastInClass = pMonStatsTxtRecord->nNextInClass;
			pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nLastInClass);
			if (!pMonStatsTxtRecord)
			{
				break;
			}
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] < AI_GetParamValue(pGame, pAiTickParam, VILEMOTHER_AI_PARAM_MAX_SPAWN) && STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL && AIRollChanceParam(pGame, pUnit, pAiTickParam, VILEMOTHER_AI_PARAM_SPAWN_CHANCE_PCT))
	{
		D2VileMotherAiCallbackArgStrc arg = {};
		arg.nLastInClass = nLastInClass;
		arg.nMaxDistance = 25;
		arg.nCounter = 0;
		sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 12);

		if (arg.nCounter < AI_GetParamValue(pGame, pAiTickParam, VILEMOTHER_AI_PARAM_MAX_AT_ONCE))
		{
			uint8_t nIndex = D2Common_11053(UNITS_GetDirectionToCoords(pUnit, CLIENTS_GetUnitX(pAiTickParam->pTarget), CLIENTS_GetUnitY(pAiTickParam->pTarget)));

			const uint8_t nIndices[8] = { 6, 4, 2, 2, 2, 0, 6, 6 };

			for (int32_t i = 0; i < 8; ++i)
			{
				nIndex = nIndex % 8;

				int32_t nX = 0;
				int32_t nY = 0;
				D2Common_11055(nIndices[nIndex], &nX, &nY);

				const int32_t nFinalX = 3 * nX + CLIENTS_GetUnitX(pUnit);
				const int32_t nFinalY = 3 * nY + CLIENTS_GetUnitY(pUnit);

				if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && sub_6FC68350(MONSTER_VILEMOTHER1, UNITS_GetRoom(pUnit), nFinalX, nFinalY, 0))
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, nFinalX, nFinalY);
					++pAiTickParam->pAiControl->dwAiParam[0];
					return;
				}

				++nIndex;
			}
		}
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VILEMOTHER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, VILEMOTHER_AI_PARAM_STALL_DURATION));
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] >= AI_GetParamValue(pGame, pAiTickParam, VILEMOTHER_AI_PARAM_MAX_SPAWN) || AIRollChanceParam(pGame, pUnit, pAiTickParam, VILEMOTHER_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VILEMOTHER_AI_PARAM_CIRCLE_CHANCE_PCT))
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

enum D2C_VileDogAIParams
{
	VILEDOG_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	VILEDOG_AI_PARAM_STALL_DURATION = 1,
	VILEDOG_AI_PARAM_APPROACH_CHANCE_PCT = 2,
};

//D2Game.0x6FCDCBF0
void __fastcall AITHINK_Fn069_VileDog(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VILEDOG_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, VILEDOG_AI_PARAM_STALL_DURATION));
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, VILEDOG_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

enum D2C_FingerMageAIParams
{
	FINGERMAGE_AI_PARAM_ATTACK_CIRCLE_CHANCE_PCT = 0,
	FINGERMAGE_AI_PARAM_CAST_CHANCE_PCT = 1,
	FINGERMAGE_AI_PARAM_HEALTHY_PCT = 2,
	FINGERMAGE_AI_PARAM_HURT_PCT = 3,
	FINGERMAGE_AI_PARAM_CAST_RANGE = 4,
	FINGERMAGE_AI_PARAM_MAX_RUN_TRIES = 5,
	FINGERMAGE_AI_PARAM_OUT_OF_RANGE = 6,
	FINGERMAGE_AI_PARAM_MELEE_STALL_DURATION = 7,
};

//D2Game.0x6FCDCCD0
void __fastcall AITHINK_Fn070_FingerMage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nVel = 0;

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);

	if (pAiTickParam->pMonstatsTxt->nVelocity > 0)
	{
		const int32_t nRatio = 100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity;
		if (nRatio > 100)
		{
			nVel = std::min(nRatio - 100, 120);
		}
	}

	if (sub_6FCF2E70(pUnit))
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;

		if (pAiTickParam->bCombat)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_CAST_RANGE))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
	{
		++pAiTickParam->pAiControl->dwAiParam[1];

		if (nLifePercentage > AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_HEALTHY_PCT) || (AI_RollPercentage(pUnit) < 25) || pAiTickParam->pAiControl->dwAiParam[1] > AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_MAX_RUN_TRIES))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
		}
		else
		{
			if (pAiTickParam->nTargetDistance < 14)
			{
				AITACTICS_SetVelocity(pUnit, 0, nVel, 0);
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 14, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		return;
	}

	if (!AIGENERAL_GetMinionOwner(pUnit) && nLifePercentage < AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_HURT_PCT))
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 9u, 0);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FINGERMAGE_AI_PARAM_ATTACK_CIRCLE_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || !AIRollChanceParam(pGame, pUnit, pAiTickParam, FINGERMAGE_AI_PARAM_CAST_CHANCE_PCT))
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 5u, 0);
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_CAST_RANGE))
	{
		if (pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_OUT_OF_RANGE))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			return;
		}

		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
	}
	else
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, FINGERMAGE_AI_PARAM_CAST_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (STATES_CheckState(pAiTickParam->pTarget, STATE_FINGERMAGECURSE))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, FINGERMAGE_AI_PARAM_MELEE_STALL_DURATION));
		}
	}
}

enum D2C_RegurgitatorAIParams
{
	REGURGIGATOR_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	REGURGIGATOR_AI_PARAM_EAT_IN_MELEE_CHANCE_PCT = 1,
	REGURGIGATOR_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	REGURGIGATOR_AI_PARAM_LOOK_FOR_FOOD_CHANCE_PCT = 3,
	REGURGIGATOR_AI_PARAM_LOOK_FOR_FOOD_IN_MELEE_PCT = 4,
	REGURGIGATOR_AI_PARAM_SMELL_DISTANCE = 5,
};

//D2Game.0x6FCDD060
void __fastcall AITHINK_Fn071_Regurgitator(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	switch (pAiTickParam->pAiControl->dwAiParam[0])
	{
	case 2:
	{
		D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pAiTickParam->pAiControl->dwAiParam[1]);
		if (!pTarget || pTarget->dwAnimMode != MONMODE_DEAD)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 8u);
			return;
		}

		if (AITHINK_GetSquaredDistance(pUnit, pTarget) > 4)
		{
			if (pAiTickParam->pAiControl->dwAiParam[2] >= 6)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				pAiTickParam->pAiControl->dwAiParam[2] = 0;
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 8u);
				return;
			}

			AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pTarget, 1u);
			++pAiTickParam->pAiControl->dwAiParam[2];
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 3;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
		return;
	}
	case 3:
	{
		D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pAiTickParam->pAiControl->dwAiParam[1]);
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pTarget && pTarget->dwAnimMode == MONMODE_DEAD)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[0] = 4;
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		pAiTickParam->pAiControl->dwAiParam[2] = 0;
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 8u);
		return;
	}
	case 4:
	{
		if (pAiTickParam->bCombat)
		{
			D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 8u, 1);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			pAiTickParam->pAiControl->dwAiParam[0] = 5;
		}
		return;
	}
	case 5:
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 16, 1);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	default:
		break;
	}

	D2RegurgitatorAiCallbackArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nDistance = AI_GetParamValue(pGame, pAiTickParam, REGURGIGATOR_AI_PARAM_SMELL_DISTANCE) * AI_GetParamValue(pGame, pAiTickParam, REGURGIGATOR_AI_PARAM_SMELL_DISTANCE);
	sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Regurgitator, 1);

	if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
	{
		if (!arg.pTarget)
		{
			if (AI_RollPercentage(pUnit) < 20)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				pAiTickParam->pAiControl->dwAiParam[2] = 0;
				return;
			}

			AITACTICS_WalkCloseToUnit(pGame, pUnit, 8u);
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[1] = arg.pTarget->dwUnitId;

		if (AITHINK_GetSquaredDistance(pUnit, arg.pTarget) <= 2)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
			pAiTickParam->pAiControl->dwAiParam[0] = 3;
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
		}
		else
		{
			AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, arg.pTarget, 1u);
			pAiTickParam->pAiControl->dwAiParam[0] = 2;
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
		}
		return;
	}

	if (arg.pTarget)
	{
		if (AITHINK_GetSquaredDistance(pUnit, arg.pTarget) < 9 && AIRollChanceParam(pGame, pUnit, pAiTickParam, REGURGIGATOR_AI_PARAM_EAT_IN_MELEE_CHANCE_PCT)
			|| AIRollChanceParam(pGame, pUnit, pAiTickParam, REGURGIGATOR_AI_PARAM_LOOK_FOR_FOOD_IN_MELEE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, arg.pTarget, 1u);
			pAiTickParam->pAiControl->dwAiParam[0] = 2;
			pAiTickParam->pAiControl->dwAiParam[1] = arg.pTarget->dwUnitId;
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
			return;
		}
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, REGURGIGATOR_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, REGURGIGATOR_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		return;
	}

	if (!arg.pTarget || !AIRollChanceParam(pGame, pUnit, pAiTickParam, REGURGIGATOR_AI_PARAM_LOOK_FOR_FOOD_CHANCE_PCT))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
		return;
	}

	AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, arg.pTarget, 1u);
	pAiTickParam->pAiControl->dwAiParam[0] = 2;
	pAiTickParam->pAiControl->dwAiParam[1] = arg.pTarget->dwUnitId;
	pAiTickParam->pAiControl->dwAiParam[2] = 0;
}

//D2Game.0x6FCDD5C0
D2UnitStrc* __fastcall AITHINK_TargetCallback_Regurgitator(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2RegurgitatorAiCallbackArgStrc* pArg = (D2RegurgitatorAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || pTarget->dwFlags & UNITFLAG_ISMERC || !(pTarget->dwFlags & UNITFLAG_TARGETABLE))
	{
		return nullptr;
	}

	if (!MONSTERS_IsDead(pTarget) || STATES_CheckStateMaskUdeadOnUnit(pTarget) || STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL)
	{
		return nullptr;
	}

	D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pTarget->dwClassId);
	if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SOFT]))
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);
	if (nDistance > pArg->nDistance)
	{
		return nullptr;
	}

	pArg->pTarget = pTarget;
	pArg->nDistance = nDistance;
	return nullptr;
}

enum D2C_DoomKnightAIParams
{
	DOOMKNIGHT_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	DOOMKNIGHT_AI_PARAM_MELEE_STALL_DURATION = 1,
	DOOMKNIGHT_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	DOOMKNIGHT_AI_PARAM_REGULAR_STALL_DURATION = 3,
};

//D2Game.0x6FCDD790
void __fastcall AITHINK_Fn072_DoomKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DOOMKNIGHT_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DOOMKNIGHT_AI_PARAM_MELEE_STALL_DURATION));
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DOOMKNIGHT_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		sub_6FCD0410(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DOOMKNIGHT_AI_PARAM_REGULAR_STALL_DURATION));
}

enum D2C_AbyssKnightAIParams
{
	ABYSSKNIGHT_AI_PARAM_BONE_ARMOR_HP_PCT = 0,
	ABYSSKNIGHT_AI_PARAM_BONE_ARMOR_CHANCE_PCT = 1,
	ABYSSKNIGHT_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	ABYSSKNIGHT_AI_PARAM_MELEE_STALL_DURATION = 3,
	ABYSSKNIGHT_AI_PARAM_NO_FIRE_DISTANCE = 4,
	ABYSSKNIGHT_AI_PARAM_FIRE_RECOVERY = 5,
	ABYSSKNIGHT_AI_PARAM_APPROACH_CHANCE_PCT = 6,
	ABYSSKNIGHT_AI_PARAM_ACTIVE_DISTANCE = 7,
};

//D2Game.0x6FCDD850
void __fastcall AITHINK_Fn073_AbyssKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[1]);

	if (pSkillsTxtRecord && pSkillsTxtRecord->nAuraState >= 0 && !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState)
		&& UNITS_GetCurrentLifePercentage(pUnit) < AI_GetParamValue(pGame, pAiTickParam, ABYSSKNIGHT_AI_PARAM_BONE_ARMOR_HP_PCT) && AIRollChanceParam(pGame, pUnit, pAiTickParam, ABYSSKNIGHT_AI_PARAM_BONE_ARMOR_CHANCE_PCT))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], nullptr, 0, 0);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ABYSSKNIGHT_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, ABYSSKNIGHT_AI_PARAM_MELEE_STALL_DURATION));
		return;
	}

	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, ABYSSKNIGHT_AI_PARAM_NO_FIRE_DISTANCE) && pAiTickParam->pAiControl->dwAiParam[0] <= 0)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = AI_GetParamValue(pGame, pAiTickParam, ABYSSKNIGHT_AI_PARAM_FIRE_RECOVERY);
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && !pAiTickParam->pAiControl->dwAiParam[0] && (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData || pUnit->pMonsterData->nComponent[10] < 4u))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = AI_GetParamValue(pGame, pAiTickParam, ABYSSKNIGHT_AI_PARAM_FIRE_RECOVERY);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] > 0)
	{
		--pAiTickParam->pAiControl->dwAiParam[0];
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ABYSSKNIGHT_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 2, 0, (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1) + 6);
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, ABYSSKNIGHT_AI_PARAM_ACTIVE_DISTANCE))
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

enum D2C_OblivionKnightAIParams
{
	OBLIVIONKNIGHT_AI_PARAM_FLEE_RANGE = 0,
	OBLIVIONKNIGHT_AI_PARAM_ENGAGE_RANGE= 1,
	OBLIVIONKNIGHT_AI_PARAM_CURSE_TIMER = 2,
	OBLIVIONKNIGHT_AI_PARAM_CURSE_CHANCE_PCT = 3,
	OBLIVIONKNIGHT_AI_PARAM_SHOOT_CHANCE_PCT = 4,
	OBLIVIONKNIGHT_AI_PARAM_BONESPIRIT_CHANCE_PCT = 5,
	OBLIVIONKNIGHT_AI_PARAM_APPROACH_CHANCE_PCT = 6,
	OBLIVIONKNIGHT_AI_PARAM_APPROACH_DISTANCE = 7,
};

//D2Game.0x6FCDDB10
void __fastcall AITHINK_Fn074_OblivionKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2OblivionKnightAiCallbackArgStrc arg = {};
	arg.unk0x08 = 2500;
	arg.unk0x0C = INT_MAX;
	arg.unk0x10 = INT_MAX;
	sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_OblivionKnight, 1);

	D2UnitStrc* pTarget = arg.unk0x00;

	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_FLEE_RANGE))
	{
		D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[3]);
		if (pSkillsTxtRecord && pSkillsTxtRecord->wAuraTargetState > 0 && !STATES_CheckState(pAiTickParam->pTarget, pSkillsTxtRecord->wAuraTargetState))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_CURSE_TIMER);
			return;
		}

		if (pTarget)
		{
			if (AITHINK_GetSquaredDistance(pTarget, pAiTickParam->pTarget) > pAiTickParam->nTargetDistance * pAiTickParam->nTargetDistance && AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 4))
			{
				return;
			}
		}

		AITACTICS_SetVelocity(pUnit, 2, 50, 0);

		if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 10, 1))
		{
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	int32_t nDistance = 0;
	pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, nullptr);
	if (pTarget && nDistance < AI_GetParamValue(pGame, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_ENGAGE_RANGE))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[5] > 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[0] && AIRollChanceParam(pGame, pUnit, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_CURSE_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[5], pAiTickParam->pMonstatsTxt->nSkill[5], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_CURSE_TIMER);
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_SHOOT_CHANCE_PCT))
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[2] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_BONESPIRIT_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pTarget, 0, 0);
				return;
			}

			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
			{
				if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData || pUnit->pMonsterData->nComponent[10] < 4u)
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
					return;
				}
			}
		}
	}

	if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_APPROACH_DISTANCE))
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, OBLIVIONKNIGHT_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pAiTickParam->pTarget, 6u);
			return;
		}
	}

	if (AI_RollPercentage(pUnit) >= 70)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
	}
	else
	{
		sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
	}
}

//D2Game.0x6FCDDFA0
D2UnitStrc* __fastcall AITHINK_TargetCallback_OblivionKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2OblivionKnightAiCallbackArgStrc* pArg = (D2OblivionKnightAiCallbackArgStrc*)pCallbackArg;

	if (pTarget == pUnit || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || MONSTERS_IsDead(pTarget) || !(pTarget->dwFlags & UNITFLAG_TARGETABLE))
	{
		return nullptr;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
	if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nBaseId != MONSTER_DOOMKNIGHT1)
	{
		return nullptr;
	}

	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);
	if (nDistance > pArg->unk0x08 || !STATLIST_AreUnitsAligned(pUnit, pTarget) || STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL)
	{
		return nullptr;
	}

	if (nDistance < pArg->unk0x0C)
	{
		pArg->unk0x00 = pTarget;
		pArg->unk0x0C = nDistance;
	}

	if (nDistance < pArg->unk0x10 && UNITS_GetCurrentLifePercentage(pTarget) < 40)
	{
		pArg->unk0x04 = pTarget;
		pArg->unk0x10 = nDistance;
	}

	return nullptr;
}

enum D2C_QuillMotherAIParams
{
	QUILLMOTHER_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	QUILLMOTHER_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	QUILLMOTHER_AI_PARAM_MELEE_STALL_DURATION = 2,
	QUILLMOTHER_AI_PARAM_REGULAR_STALL_DURATION = 3,
};

//D2Game.0x6FCDE150
void __fastcall AITHINK_Fn075_QuillMother(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (sub_6FCF2E70(pUnit))
	{
		D2AiCmdStrc aiCmd = {};

		aiCmd.nCmdParam[0] = 1;

		if (pAiTickParam->pTarget)
		{
			aiCmd.nCmdParam[1] = pAiTickParam->pTarget->dwUnitType;
			aiCmd.nCmdParam[2] = pAiTickParam->pTarget->dwUnitId;
		}
		else
		{
			aiCmd.nCmdParam[1] = 6;
			aiCmd.nCmdParam[2] = -1;
		}

		AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);

		if (pAiTickParam->bCombat)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		}
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, QUILLMOTHER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, QUILLMOTHER_AI_PARAM_MELEE_STALL_DURATION));
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, QUILLMOTHER_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, QUILLMOTHER_AI_PARAM_REGULAR_STALL_DURATION));
}

enum D2C_EvilHoleAIParams
{
	EVILHOLE_AI_PARAM_RUN_TO_SPAWN = 0,
	EVILHOLE_AI_PARAM_SPAWN_DELAY = 1,
};

//D2Game.0x6FCDE2B0
void __fastcall AITHINK_Fn076_EvilHole(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nAnimMode = 0;
	if (pUnit)
	{
		nAnimMode = pUnit->dwAnimMode;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] <= 0)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, EVILHOLE_AI_PARAM_SPAWN_DELAY);
		pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, EVILHOLE_AI_PARAM_RUN_TO_SPAWN);
	}

	if (nAnimMode == MONMODE_NEUTRAL)
	{
		if (pAiTickParam->nTargetDistance > 5)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 10, 0, 0);
		AITACTICS_Idle(pGame, pUnit, 20);
		return;
	}

	if (nAnimMode == MONMODE_SKILL3)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_SKILL4, 0, 0);
		AITACTICS_Idle(pGame, pUnit, 20);
		return;
	}

	if (nAnimMode == MONMODE_SKILL4 && pAiTickParam->pAiControl->dwAiParam[1] > 0)
	{
		if (pGame->dwGameFrame <= pAiTickParam->pAiControl->dwAiParam[0])
		{
			AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, EVILHOLE_AI_PARAM_SPAWN_DELAY));
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, EVILHOLE_AI_PARAM_SPAWN_DELAY);

		int32_t nSpawnMode = 0;
		int32_t nMonsterId = 0;
		int32_t nX = 0;
		int32_t nY = 0;
		MONSTERS_GetMinionSpawnInfo(pUnit, &nMonsterId, &nX, &nY, &nSpawnMode, 0, 0);

		D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
		if (pRoom)
		{
			D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nSpawnMode, 2, 66);
			if (pMonster)
			{
				--pAiTickParam->pAiControl->dwAiParam[1];
				pMonster->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC;
			}
		}

		AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, EVILHOLE_AI_PARAM_SPAWN_DELAY));
		return;
	}

	AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
}

enum D2C_TrapMissileAIParams
{
	TRAPMISSILE_AI_PARAM_DISTANCE = 0,
	TRAPMISSILE_AI_PARAM_NUM_TO_SHOOT = 1,
	TRAPMISSILE_AI_PARAM_DELAY = 2,
};

//D2Game.0x6FCDE4D0
void __fastcall AITHINK_Fn077_TrapMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pTarget && pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, TRAPMISSILE_AI_PARAM_DISTANCE) && pAiTickParam->pAiControl->dwAiParam[0] < AI_GetParamValue(pGame, pAiTickParam, TRAPMISSILE_AI_PARAM_NUM_TO_SHOOT))
	{
		if (pAiTickParam->pAiControl->dwAiParam[1])
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TRAPMISSILE_AI_PARAM_DELAY));
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			pAiTickParam->pAiControl->dwAiParam[1] = 1;
			++pAiTickParam->pAiControl->dwAiParam[0];
		}
	}
	else
	{
		if (pUnit)
		{
			pUnit->dwFlags |= UNITFLAG_NOTC;
		}

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_DEATH, 0, 0);
	}
}

enum D2C_TrapArrowAIParams
{
	TRAPARROW_AI_PARAM_MIN_DISTANCE = 0,
	TRAPARROW_AI_PARAM_MAX_DISTANCE = 1,
	TRAPARROW_AI_PARAM_DELAY = 2,
	TRAPARROW_AI_PARAM_DELAY2 = 2,
};

//D2Game.0x6FCDE570
void __fastcall AITHINK_Fn078_TrapRightArrow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_MIN_DISTANCE)
		|| pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_MAX_DISTANCE))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
		return;
	}

	D2CoordStrc pUnitCoords = {};
	UNITS_GetCoords(pUnit, &pUnitCoords);
	D2CoordStrc pTargetCoords = {};
	UNITS_GetCoords(pAiTickParam->pTarget, &pTargetCoords);

	const int32_t nXDiff = std::abs(pUnitCoords.nX - pTargetCoords.nX);

	if (nXDiff > 2 || pGame->dwGameFrame <= pAiTickParam->pAiControl->dwAiParam[0])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 30);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_DELAY);

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit));
	D2MonsterRegionStrc* pMonsterRegion = pGame->pMonReg[nLevelId];
	if (pMonsterRegion->unk0x2D4 < 0)
	{
		if (nLevelId >= 40)
		{
			pMonsterRegion->unk0x2D4 = ITEMS_RollRandomNumber(&pUnit->pSeed) % 3;
		}
		else
		{
			pMonsterRegion->unk0x2D4 = 0;
		}
	}

	if (pMonsterRegion->unk0x2D4 != 1)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_DELAY2);
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 30);
}

//D2Game.0x6FCDE710
void __fastcall AITHINK_Fn079_TrapLeftArrow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_MIN_DISTANCE) 
		|| pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_MAX_DISTANCE))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
		return;
	}

	D2CoordStrc pUnitCoords = {};
	UNITS_GetCoords(pUnit, &pUnitCoords);
	D2CoordStrc pTargetCoords = {};
	UNITS_GetCoords(pAiTickParam->pTarget, &pTargetCoords);

	const int32_t nYDiff = std::abs(pUnitCoords.nY - pTargetCoords.nY);

	if (nYDiff > 2 || pGame->dwGameFrame <= pAiTickParam->pAiControl->dwAiParam[0])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 30);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_DELAY);

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit));
	D2MonsterRegionStrc* pMonsterRegion = pGame->pMonReg[nLevelId];
	if (pMonsterRegion->unk0x2D4 < 0)
	{
		if (nLevelId >= 40)
		{
			pMonsterRegion->unk0x2D4 = ITEMS_RollRandomNumber(&pUnit->pSeed) % 3;
		}
		else
		{
			pMonsterRegion->unk0x2D4 = 0;
		}
	}

	if (pMonsterRegion->unk0x2D4 != 1)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, TRAPARROW_AI_PARAM_DELAY2);
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 30);
}

enum D2C_TrapNovaAIParams
{
	TRAPNOVA_AI_PARAM_DISTANCE = 0,
	TRAPNOVA_AI_PARAM_NUM_TIMES = 1,
	TRAPNOVA_AI_PARAM_DELAY = 2,
};

//D2Game.0x6FCDE8B0
void __fastcall AITHINK_Fn080_092_TrapPoison_TrapNova(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pTarget && pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, TRAPNOVA_AI_PARAM_DISTANCE) 
		&& pAiTickParam->pAiControl->dwAiParam[0] < AI_GetParamValue(pGame, pAiTickParam, TRAPNOVA_AI_PARAM_NUM_TIMES))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && !pAiTickParam->pAiControl->dwAiParam[1])
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			pAiTickParam->pAiControl->dwAiParam[1] = 1;
			++pAiTickParam->pAiControl->dwAiParam[0];
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TRAPNOVA_AI_PARAM_DELAY));
		}
		return;
	}

	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_NOTC;
	}

	AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_DEATH, 0, 0);
}

enum D2C_TrapMeleeAIParams
{
	TRAPMELEE_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	TRAPMELEE_AI_PARAM_STALL_DURATION = 1,
};

//D2Game.0x6FCDE960
void __fastcall AITHINK_Fn087_TrapMelee(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->bCombat)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, TRAPMELEE_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, TRAPMELEE_AI_PARAM_STALL_DURATION));
}

enum D2C_SpawnerAIParams
{
	SPAWNER_AI_PARAM_NUM_TO_SPAWN = 0,
	SPAWNER_AI_PARAM_ACTIVATION_DISTANCE = 1,
	SPAWNER_AI_PARAM_DELAY = 2,
};

//D2Game.0x6FCDE9E0
void __fastcall AITHINK_Fn082_InvisoSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, SPAWNER_AI_PARAM_NUM_TO_SPAWN);
	}

	if (pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, SPAWNER_AI_PARAM_ACTIVATION_DISTANCE))
	{
		if (pAiTickParam->pAiControl->dwAiParam[1] <= 0)
		{
			AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
			return;
		}

		if (pAiTickParam->pAiControl->dwAiParam[0] <= pGame->dwGameFrame)
		{
			D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
			const int32_t nMonsterId = D2Common_11063(pRoom, MONSTER_MUMMY1);

			int32_t nX = 0;
			int32_t nY = 0;
			if (sub_6FC66260(pGame, pRoom, 0, nMonsterId, &nX, &nY, 0) && D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, 1, 2, 66))
			{
				--pAiTickParam->pAiControl->dwAiParam[1];
				pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SPAWNER_AI_PARAM_DELAY);
			}
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

//D2Game.0x6FCDEAF0
void __fastcall AITHINK_Fn083_MosquitoNest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, SPAWNER_AI_PARAM_ACTIVATION_DISTANCE))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] <= AI_GetParamValue(pGame, pAiTickParam, SPAWNER_AI_PARAM_NUM_TO_SPAWN))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[0] && sub_6FC68350(MONSTER_SUCKERNEST1, UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, SPAWNER_AI_PARAM_DELAY);
			++pAiTickParam->pAiControl->dwAiParam[1];
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_NOTC;
	}

	AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
}

//D2Game.0x6FCDEC70
void __fastcall D2GAME_AI_Unk084_6FCDEC70(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pMonstatsTxt)
	{
		return;
	}

	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);
	if (pSkillsTxtRecord)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = pSkillsTxtRecord->dwParam[1];
	}

	pAiTickParam->pAiControl->dwAiParam[0] += pGame->dwGameFrame;
}

//D2Game.0x6FCDECE0
void __fastcall AITHINK_Fn084_BoneWall(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[0] < pGame->dwGameFrame)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_DEATH, 0, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

enum D2C_HighPriestAIParams
{
	HIGHPRIEST_AI_PARAM_ENGAGE_CHANCE_PCT = 0,
	HIGHPRIEST_AI_PARAM_HEAL_AT_RANGE_CHANCE_PCT = 1,
	HIGHPRIEST_AI_PARAM_HEAL_HYDRA_TIMER = 2,
	HIGHPRIEST_AI_PARAM_HYDRA_AT_RANGE_CHANCE_PCT = 3,
	HIGHPRIEST_AI_PARAM_LIGHTNING_AT_RANGE_CHANCE_PCT = 4,
	HIGHPRIEST_AI_PARAM_DISENGAGE_CHANCE_PCT = 5,
	HIGHPRIEST_AI_PARAM_LIGHTNING_ENGAGED_CHANCE_PCT = 6,
	HIGHPRIEST_AI_PARAM_RANGE = 7,
};

//D2Game.0x6FCDED10
void __fastcall AITHINK_Fn085_HighPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const D2CoordStrc stru_6FD29444[4] =
	{
		{ -5, -5 },
		{ 5, -5 },
		{ 5, 5 },
		{ -5, 5 },
	};

	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		if (pAiTickParam->bCombat)
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_ENGAGE_CHANCE_PCT))
			{
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 6u, 1);
			}
			else
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 1;
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1])
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_HEAL_AT_RANGE_CHANCE_PCT))
			{
				D2HighPriestAiCallbackArgStrc arg = {};
				arg.pTarget = nullptr;
				arg.nLifePercentage = 75;
				arg.nMaxDistance = 2500;
				sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_HighPriest, 1);

				if (arg.pTarget)
				{
					pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, HIGHPRIEST_AI_PARAM_HEAL_HYDRA_TIMER);
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], arg.pTarget, 0, 0);
					return;
				}
			}
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || pGame->dwGameFrame <= pAiTickParam->pAiControl->dwAiParam[1]
			|| pAiTickParam->nTargetDistance >= AI_GetParamValue(pGame, pAiTickParam, HIGHPRIEST_AI_PARAM_RANGE) || (!AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_HYDRA_AT_RANGE_CHANCE_PCT)))
		{
			if (AI_GetParamValue(pGame, pAiTickParam, HIGHPRIEST_AI_PARAM_LIGHTNING_AT_RANGE_CHANCE_PCT) > 0)
			{
				D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pAiTickParam->pMonstatsTxt->wMissS1);
				if (pMissilesTxtRecord && pAiTickParam->nTargetDistance < pMissilesTxtRecord->wRange - 2 && AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_LIGHTNING_AT_RANGE_CHANCE_PCT))
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
					return;
				}
			}

			if (AI_RollPercentage(pUnit) < 80)
			{
				if (pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, HIGHPRIEST_AI_PARAM_RANGE))
				{
					sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
				}
				else
				{
					AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 6u);
				}
				return;
			}
		}
		else
		{
			const uint32_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed) & 3;
			const int32_t nX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + stru_6FD29444[nRand].nX;
			const int32_t nY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + stru_6FD29444[nRand].nY;
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, nX, nY);
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + 100;
			return;
		}
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 1;

	if (pAiTickParam->bCombat)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_LIGHTNING_ENGAGED_CHANCE_PCT))
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_DISENGAGE_CHANCE_PCT))
			{
				if (AI_RollPercentage(pUnit) >= 90)
				{
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
				}
				else
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
				}
			}
			else
			{
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 6u, 1);
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
			}
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
		}
		return;
	}

	if (pAiTickParam->nTargetDistance >= 6 || (!AIRollChanceParam(pGame, pUnit, pAiTickParam, 6)))
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, HIGHPRIEST_AI_PARAM_DISENGAGE_CHANCE_PCT))
		{
			if (AI_RollPercentage(pUnit) >= 70)
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 12);
			}
			else
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			}
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		return;
	}

	AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
}

//D2Game.0x6FCDF2E0
D2UnitStrc* __fastcall AITHINK_TargetCallback_HighPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2HighPriestAiCallbackArgStrc* pArg = (D2HighPriestAiCallbackArgStrc*)pCallbackArg;

	if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER || MONSTERS_IsDead(pTarget) || !STATLIST_AreUnitsAligned(pUnit, pTarget) || STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL)
	{
		return 0;
	}

	if (AITHINK_GetSquaredDistance(pUnit, pTarget) > pArg->nMaxDistance)
	{
		return 0;
	}

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pTarget);
	if (nLifePercentage > 75 || nLifePercentage >= pArg->nLifePercentage)
	{
		return 0;
	}

	pArg->nLifePercentage = nLifePercentage;
	pArg->pTarget = pTarget;
	return 0;
}

enum D2C_DesertTurretAIParams
{
	DESERTTURRET_AI_PARAM_SHORT_DELAY = 0,
	DESERTTURRET_AI_PARAM_NUM_SHOTS = 1,
	DESERTTURRET_AI_PARAM_LONG_DELAY = 2,
	DESERTTURRET_AI_PARAM_RANGE = 3,
	DESERTTURRET_AI_PARAM_SPREAD = 4,
};

//D2Game.0x6FCDF410
void __fastcall AITHINK_Fn094_DesertTurret(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nIndices[] =
	{
		0, 0, 1, 2, 5, 6, 7, 0,
		1, 1, 1, 2, 3, 6, 7, 0,
		1, 2, 2, 2, 3, 4, 7, 0,
		1, 2, 3, 3, 3, 4, 5, 6,
		1, 2, 3, 4, 4, 4, 5, 6,
		7, 2, 3, 4, 5, 5, 5, 6,
		7, 0, 3, 4, 5, 6, 6, 6,
		7, 0, 1, 4, 5, 6, 7, 7,
	};

	D2CoordStrc pOffsets[] =
	{
		{ 1, 1 },
		{ 0, 1 },
		{ -1, 1 },
		{ -1, 0 },
		{ -1, -1 },
		{ 0, -1 },
		{ 1, -1 },
		{ 1, 0 },
	};

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && !pAiTickParam->pAiControl->dwAiParam[0])
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[2] = 0;
		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;
		return;
	}

	if (pGame->dwGameFrame < pAiTickParam->pAiControl->dwAiParam[0])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, DESERTTURRET_AI_PARAM_RANGE))
	{
		if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
		{
			--pAiTickParam->pAiControl->dwAiParam[1];
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		return;
	}

	const int32_t nIndex1 = pAiTickParam->pAiControl->dwAiParam[2] % 8;
	int32_t nIndex2 = D2Common_11053(UNITS_GetDirectionToCoords(pUnit, CLIENTS_GetUnitX(pAiTickParam->pTarget), CLIENTS_GetUnitY(pAiTickParam->pTarget))) & 7;
	if (nIndex2 < 0)
	{
		nIndex2 = (nIndex2 + 8) & 7;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = nIndices[nIndex1 + 8 * nIndex2];

	const int32_t nX = CLIENTS_GetUnitX(pUnit) + AI_GetParamValue(pGame, pAiTickParam, DESERTTURRET_AI_PARAM_SPREAD) * pOffsets[pAiTickParam->pAiControl->dwAiParam[2]].nX;
	const int32_t nY = CLIENTS_GetUnitY(pUnit) + AI_GetParamValue(pGame, pAiTickParam, DESERTTURRET_AI_PARAM_SPREAD) * pOffsets[pAiTickParam->pAiControl->dwAiParam[2]].nY;

	UNITS_SetTargetUnitForDynamicUnit(pUnit, pAiTickParam->pTarget);

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, nX, nY))
	{
		if (sub_6FC68630(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, CLIENTS_GetUnitX(pAiTickParam->pTarget), CLIENTS_GetUnitY(pAiTickParam->pTarget)))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, nX, nY);
			D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX, nY);

			++pAiTickParam->pAiControl->dwAiParam[1];

			if (pAiTickParam->pAiControl->dwAiParam[1] <= AI_GetParamValue(pGame, pAiTickParam, 1))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, DESERTTURRET_AI_PARAM_SHORT_DELAY);
			}
			else
			{
				pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, DESERTTURRET_AI_PARAM_LONG_DELAY);
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
			}
			return;
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
	{
		--pAiTickParam->pAiControl->dwAiParam[1];
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

enum D2C_MegademonAIParams
{
	MEGADEMON_AI_PARAM_INFERNO_RANGED_CHANCE_PCT = 0,
	MEGADEMON_AI_PARAM_INFERNO_MELEE_CHANCE_PCT = 1,
	MEGADEMON_AI_PARAM_SWING_MELEE_CHANCE_PCT = 2,
	MEGADEMON_AI_PARAM_APPROACH_CHANCE_PCT = 3,
	MEGADEMON_AI_PARAM_CIRCLE_MELEE_CHANCE_PCT = 4,
	MEGADEMON_AI_PARAM_INFERNO_TIMER = 5,
};

//D2Game.0x6FCDF780
void __fastcall AITHINK_Fn089_Megademon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nSkillLevel = 1;

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], -1);
		if (pSkill)
		{
			nSkillLevel = std::max(SKILLS_GetSkillLevel(pUnit, pSkill, 1), 1);
		}
	}

	const int32_t bInferno = STATES_CheckState(pUnit, STATE_INFERNO);
	if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || pAiTickParam->bCombat || pAiTickParam->nTargetDistance >= nSkillLevel)
	{
		if (bInferno)
		{
			STATES_ToggleState(pUnit, STATE_INFERNO, 0);
		}
	}
	else
	{
		if (bInferno)
		{
			STATES_ToggleState(pUnit, STATE_INFERNO, 0);
		}
		else
		{
			if (pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[0] && AIRollChanceParam(pGame, pUnit, pAiTickParam, MEGADEMON_AI_PARAM_INFERNO_RANGED_CHANCE_PCT))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, MEGADEMON_AI_PARAM_INFERNO_TIMER);
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				return;
			}

			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, MEGADEMON_AI_PARAM_APPROACH_CHANCE_PCT))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
			else
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			}
			return;
		}
	}

	if (!pAiTickParam->bCombat)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, MEGADEMON_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
		}
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || pGame->dwGameFrame <= pAiTickParam->pAiControl->dwAiParam[0] || !AIRollChanceParam(pGame, pUnit, pAiTickParam, MEGADEMON_AI_PARAM_INFERNO_MELEE_CHANCE_PCT))
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, MEGADEMON_AI_PARAM_SWING_MELEE_CHANCE_PCT))
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, MEGADEMON_AI_PARAM_CIRCLE_MELEE_CHANCE_PCT))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			}
			else
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 3u, 0);
			}
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
	}
	else
	{
		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, MEGADEMON_AI_PARAM_INFERNO_TIMER);
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
	}
}

enum D2C_ArcaneTowerAIParams
{
	ARCANETOWER_AI_PARAM_NUM_SKILL_1 = 0,
	ARCANETOWER_AI_PARAM_SKILL_1_STALL_DURATION = 1,
	ARCANETOWER_AI_PARAM_SKILL_1_LONG_DELAY = 2,
	ARCANETOWER_AI_PARAM_NUM_SKILL_2 = 3,
	ARCANETOWER_AI_PARAM_SKILL_2_STALL_DURATION = 4,
	ARCANETOWER_AI_PARAM_SKILL_2_LONG_DELAY = 5,
};

//D2Game.0x6FCDFA50
void __fastcall AITHINK_Fn093_ArcaneTower(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pAiControl->dwAiParam[1])
	{
		pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_NUM_SKILL_1);
	}

	if (pGame->dwGameFrame < pAiTickParam->pAiControl->dwAiParam[2])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || pAiTickParam->pAiControl->dwAiParam[0])
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);

		--pAiTickParam->pAiControl->dwAiParam[1];
		if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
		{
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_SKILL_2_LONG_DELAY);
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_NUM_SKILL_1);
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_SKILL_2_STALL_DURATION);
		}
	}
	else
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);

		--pAiTickParam->pAiControl->dwAiParam[1];
		if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
		{
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_SKILL_1_STALL_DURATION);
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_NUM_SKILL_2);
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, ARCANETOWER_AI_PARAM_SKILL_1_LONG_DELAY);
		}
	}
}

enum D2C_PantherJavelinAIParams
{
	PANTHERJAVELIN_AI_PARAM_APPROACH_CHANCE_PCT = 0,
	PANTHERJAVELIN_AI_PARAM_THROW_CHANCE_PCT = 1,
	PANTHERJAVELIN_AI_PARAM_GROUP_DISTANCE = 2,
	PANTHERJAVELIN_AI_PARAM_WALK_AWAY_CHANCE_PCT = 3,
	PANTHERJAVELIN_AI_PARAM_STALL_DURATION = 4,
	PANTHERJAVELIN_AI_PARAM_THROW_DISTANCE= 5,
};

//D2Game.0x6FCDFB80
void __fastcall AITHINK_Fn095_PantherJavelin(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;
	D2UnitStrc* pTargetUnit = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
	if (nDistance < 8 && (AIRollChanceParam(pGame, pUnit, pAiTickParam, PANTHERJAVELIN_AI_PARAM_WALK_AWAY_CHANCE_PCT)))
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 16, 1);
		return;
	}

	if (nDistance > AI_GetParamValue(pGame, pAiTickParam, 5) - 6 && AIRollChanceParam(pGame, pUnit, pAiTickParam, PANTHERJAVELIN_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pAiTickParam->pTarget, 4u);
		return;
	}

	if (!pTargetUnit || nDistance >= AI_GetParamValue(pGame, pAiTickParam, PANTHERJAVELIN_AI_PARAM_THROW_DISTANCE))
	{
		D2PantherAiCallbackArgStrc arg = {};
		arg.pTarget = nullptr;
		arg.nDistance = INT_MAX;

		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Panther, 1);

		if (arg.pTarget && arg.nDistance > AI_GetParamValue(pGame, pAiTickParam, PANTHERJAVELIN_AI_PARAM_GROUP_DISTANCE))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, arg.pTarget, 7);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, PANTHERJAVELIN_AI_PARAM_STALL_DURATION));
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, PANTHERJAVELIN_AI_PARAM_THROW_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTargetUnit);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, PANTHERJAVELIN_AI_PARAM_STALL_DURATION));
}

enum D2C_FetishBlowgunAIParams
{
	FETISHBLOWGUN_AI_PARAM_STAY_IN_RANGE = 0,
	FETISHBLOWGUN_AI_PARAM_RUN_AWAY_CHANCE_PCT = 1,
};

//D2Game.0x6FCDFD50
void __fastcall AITHINK_Fn096_FetishBlowgun(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (pCurrentAiCmd)
	{
		if (pCurrentAiCmd->nCmdParam[0] == 1)
		{
			D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, pCurrentAiCmd->nCmdParam[2], pCurrentAiCmd->nCmdParam[1]);
			if (pTarget)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
				AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
				return;
			}
		}
		else if (pCurrentAiCmd->nCmdParam[0] == 14)
		{
			D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, pCurrentAiCmd->nCmdParam[2], pCurrentAiCmd->nCmdParam[1]);
			if (pTarget)
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				AITACTICS_SetVelocity(pUnit, 13, 50, 0);
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 0);
				AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
				return;
			}
		}

		AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FETISHBLOWGUN_AI_PARAM_RUN_AWAY_CHANCE_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 2;
		}
	}
	else
	{
		if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, FETISHBLOWGUN_AI_PARAM_STAY_IN_RANGE))
		{
			AITACTICS_SetVelocity(pUnit, 0, 50, 0);
			D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pAiTickParam->pTarget, 6u);
			return;
		}

		if (pAiTickParam->nTargetDistance < 6 && AIRollChanceParam(pGame, pUnit, pAiTickParam, FETISHBLOWGUN_AI_PARAM_RUN_AWAY_CHANCE_PCT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 2;
		}
	}

	int32_t nDistance = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);

	switch (pAiTickParam->pAiControl->dwAiParam[0])
	{
	case 0:
		++pAiTickParam->pAiControl->dwAiParam[1];

		if (pAiTickParam->pAiControl->dwAiParam[1] > (ITEMS_RollRandomNumber(&pUnit->pSeed) % 3 + 3))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
		}

		if (pTarget)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		}
		else
		{
			sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 0);
		}
		return;

	case 1:
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		AITACTICS_SetVelocity(pUnit, 0, 50, 0);

		if (!sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 6u, 1))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		return;

	case 2:
		if (pAiTickParam->nTargetDistance <= 12)
		{
			AITACTICS_SetVelocity(pUnit, 2, 50, 0);

			if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 14, 1))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
			}
		}
		else
		{
			if (AI_RollPercentage(pUnit) >= 20 || !sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 1))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
		}
		return;

	default:
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}
}

enum D2C_ReanimatedHordeAIParams
{
	REANIMATEDHORDE_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	REANIMATEDHORDE_AI_PARAM_MELEE_STALL_DURATION = 1,
	REANIMATEDHORDE_AI_PARAM_CHARGE_RANGE = 2,
	REANIMATEDHORDE_AI_PARAM_CHARGE_CHANCE_PCT = 3,
	REANIMATEDHORDE_AI_PARAM_FOLLOW_CHANCE_PCT = 4,
	REANIMATEDHORDE_AI_PARAM_WALK_FORWARD_CHANCE_PCT = 5,
	REANIMATEDHORDE_AI_PARAM_RANGED_STALL_DURATION = 6,
	REANIMATEDHORDE_AI_PARAM_REANIMATE = 7,
};

//D2Game.0x6FCE0050
void __fastcall AITHINK_Fn114_ReanimatedHorde(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pUnit)
	{
		pUnit->dwFlags |= (UNITFLAG_ISVALIDTARGET | UNITFLAG_CANBEATTACKED | UNITFLAG_TARGETABLE);
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, REANIMATEDHORDE_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, REANIMATEDHORDE_AI_PARAM_MELEE_STALL_DURATION));
		}
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && sub_6FCF14D0(pUnit, pAiTickParam->pTarget) 
		&& pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, REANIMATEDHORDE_AI_PARAM_CHARGE_RANGE) && pAiTickParam->nTargetDistance > 5 
		&& (AIRollChanceParam(pGame, pUnit, pAiTickParam, REANIMATEDHORDE_AI_PARAM_CHARGE_CHANCE_PCT)))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, REANIMATEDHORDE_AI_PARAM_FOLLOW_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, REANIMATEDHORDE_AI_PARAM_WALK_FORWARD_CHANCE_PCT))
	{
		AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 4, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, REANIMATEDHORDE_AI_PARAM_RANGED_STALL_DURATION));
}

enum D2C_SiegeTowerAIParams
{
	SIEGETOWER_AI_PARAM_STALL_DURATION = 0,
};

//D2Game.0x6FCE0220
void __fastcall AITHINK_Fn113_SiegeTower(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);

	if (!pOwner || SUNIT_IsDead(pOwner))
	{
		if (pAiTickParam->pTarget)
		{
			D2SiegeBeastTowerAiCallbackArgStrc arg = {};
			arg.nMaxDistance = 400;
			D2UnitStrc* pTarget = sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_SiegeBeast, 1);

			if (pTarget)
			{
				D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pTarget);

				if (pUnit)
				{
					D2UnitStrc* pMonster = SUNIT_GetServerUnit(pTarget->pGame, UNIT_MONSTER, pAiControl->dwAiParam[0]);

					if (!pMonster)
					{
						pAiControl->dwAiParam[0] = pUnit->dwUnitId;
					}
					else
					{
						if (AITHINK_GetSquaredDistance(pTarget, pUnit) <= AITHINK_GetSquaredDistance(pTarget, pMonster))
						{
							pAiControl->dwAiParam[0] = pUnit->dwUnitId;
						}
					}
				}
				else
				{
					pAiControl->dwAiParam[0] = -1;
				}

				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SIEGETOWER_AI_PARAM_STALL_DURATION));
			}
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SIEGETOWER_AI_PARAM_STALL_DURATION));
}

//D2Game.0x6FCE0430
D2UnitStrc* __fastcall AITHINK_TargetCallback_SiegeBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2SiegeBeastTowerAiCallbackArgStrc* pArg = (D2SiegeBeastTowerAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || MONSTERS_IsDead(pTarget))
	{
		return 0;
	}

	int32_t nClassId = pTarget->dwClassId;
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
	if (pMonStatsTxtRecord)
	{
		nClassId = pMonStatsTxtRecord->nBaseId;
	}

	if (nClassId != MONSTER_IMP1 || !STATLIST_AreUnitsAligned(pUnit, pTarget) || STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL || SUNIT_GetOwner(pGame, pUnit))
	{
		return 0;
	}

	D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pTarget);
	if (pAiControl->dwAiParam[0] != -1)
	{
		return 0;
	}

	if (AITHINK_GetSquaredDistance(pUnit, pTarget) > pArg->nMaxDistance)
	{
		return 0;
	}

	return pTarget;
}

enum D2C_SiegeBeastAIParams
{
	SIEGEBEAST_AI_PARAM_CALL_IMP_DISTANCE = 0,
	SIEGEBEAST_AI_PARAM_ATTACK_CHANCE_PCT = 1,
	SIEGEBEAST_AI_PARAM_STAMP_MELEE_CHANCE_PCT = 2,
	SIEGEBEAST_AI_PARAM_MELEE_STALL_DURATION = 3,
	SIEGEBEAST_AI_PARAM_STOMP_RANGED_CHANCE_PCT = 4,
	SIEGEBEAST_AI_PARAM_CHARGE_CHANCE_PCT = 5,
	SIEGEBEAST_AI_PARAM_CHARGE_VELOCITY = 6,
};

//D2Game.0x6FCE0610
void __fastcall AITHINK_Fn115_SiegeBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
	if (MONSTERS_IsDead(pUnit))
	{
		return;
	}

	if (!pOwner && STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL)
	{
		D2SiegeBeastTowerAiCallbackArgStrc arg = {};
		arg.nMaxDistance = AI_GetParamValue(pGame, pAiTickParam, SIEGEBEAST_AI_PARAM_CALL_IMP_DISTANCE) * AI_GetParamValue(pGame, pAiTickParam, SIEGEBEAST_AI_PARAM_CALL_IMP_DISTANCE);
		D2UnitStrc* pTarget = sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_SiegeBeast, 1);

		if (pTarget)
		{
			D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pTarget);

			if (pUnit)
			{
				D2UnitStrc* pMonster = SUNIT_GetServerUnit(pTarget->pGame, UNIT_MONSTER, pAiControl->dwAiParam[0]);
				if (pMonster)
				{
					if (AITHINK_GetSquaredDistance(pTarget, pUnit) <= AITHINK_GetSquaredDistance(pTarget, pMonster))
					{
						pAiControl->dwAiParam[0] = pUnit->dwUnitId;
					}
				}
				else
				{
					pAiControl->dwAiParam[0] = pUnit->dwUnitId;
				}
			}
			else
			{
				pAiControl->dwAiParam[0] = -1;
			}
		}
	}

	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0 || (!AIRollChanceParam(pGame, pUnit, pAiTickParam, SIEGEBEAST_AI_PARAM_STAMP_MELEE_CHANCE_PCT)))
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, SIEGEBEAST_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SIEGEBEAST_AI_PARAM_MELEE_STALL_DURATION));
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
		}
		else
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, 0, 0);
		}
	}
	else
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
		{
			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);

			int32_t nMaxDistance = 0;
			if (pSkillsTxtRecord)
			{
				nMaxDistance = pSkillsTxtRecord->dwParam[4];
			}

			if (pAiTickParam->nTargetDistance < nMaxDistance && AIRollChanceParam(pGame, pUnit, pAiTickParam, SIEGEBEAST_AI_PARAM_STOMP_RANGED_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, 0, 0);
				return;
			}
		}

		if (sub_6FCF14D0(pUnit, pAiTickParam->pTarget) && AIRollChanceParam(pGame, pUnit, pAiTickParam, SIEGEBEAST_AI_PARAM_CHARGE_CHANCE_PCT))
		{
			const int16_t nVel = D2Clamp(AI_GetParamValue(pGame, pAiTickParam, SIEGEBEAST_AI_PARAM_CHARGE_VELOCITY), 0i16, 127i16);

			AITACTICS_SetVelocity(pUnit, 0, nVel, 0);
		}

		AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pAiTickParam->pTarget, 12, 0);
	}
}

//D2Game.0x6FCE0960
int32_t __fastcall AITHINK_GetSquaredDistance(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	const int32_t nXDiff = CLIENTS_GetUnitX(pUnit1) - CLIENTS_GetUnitX(pUnit2);
	const int32_t nYDiff = CLIENTS_GetUnitY(pUnit1) - CLIENTS_GetUnitY(pUnit2);

	return nXDiff * nXDiff + nYDiff * nYDiff;
}

enum D2C_MinionAIParams
{
	MINION_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	MINION_AI_PARAM_MELEE_STALL_DURATION = 1,
	MINION_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	MINION_AI_PARAM_RANGED_STALL_DURATION = 3,
	MINION_AI_PARAM_ATTACK_2_OR_1_CHANCE_PCT = MINION_AI_PARAM_MELEE_STALL_DURATION, // = 4 // Note: this is actually a bug in the original game where they use MINION_AI_PARAM_MELEE_STALL_DURATION instead of MINION_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT
};

//D2Game.0x6FCE0A50
void __fastcall AITHINK_Fn116_Minion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t bIdle = 1;

	D2UnitStrc* pTarget = pAiTickParam->pTarget;
	int32_t bCombat = pAiTickParam->bCombat;

	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (pCurrentAiCmd)
	{
		if (pCurrentAiCmd->nCmdParam[0] == 1 && pCurrentAiCmd->nCmdParam[3] > pGame->dwGameFrame)
		{
			D2UnitStrc* pTargetUnit = SUNIT_GetServerUnit(pGame, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]);
			if (pTargetUnit && !SUNIT_IsDead(pTargetUnit))
			{
				bIdle = 0;
				if (pTarget != pTargetUnit)
				{
					pTarget = pTargetUnit;
					UNITS_GetDistanceToOtherUnit(pUnit, pTargetUnit);
					bCombat = UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0);
				}
			}
			else
			{
				AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
			}
		}
		else
		{
			AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
		}
	}

	if (bCombat)
	{
		if (!bIdle || AIRollChanceParam(pGame, pUnit, pAiTickParam, MINION_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			// Bug in original game, see MINION_AI_PARAM_ATTACK_1_OR_2_CHANCE_PCT
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, MINION_AI_PARAM_ATTACK_2_OR_1_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pTarget);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			}
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MINION_AI_PARAM_MELEE_STALL_DURATION));
		}
	}
	else
	{
		if (!bIdle || AIRollChanceParam(pGame, pUnit, pAiTickParam, MINION_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 0);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MINION_AI_PARAM_RANGED_STALL_DURATION));
		}
	}
}

enum D2C_SuicideMinionAIParams
{
	SUICIDEMINION_AI_PARAM_STALL_DURATION = 1,
	SUICIDEMINION_AI_PARAM_APPROACH_CHANCE_PCT = 2,
	SUICIDEMINION_AI_PARAM_BLOW_UP_DELAY = 4, // Actually referenced as = 0 in MonAi.txt
};

//D2Game.0x6FCE0C10
void __fastcall AITHINK_Fn117_SuicideMinion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[0])
	{
		if (pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[0])
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_DEATH, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUICIDEMINION_AI_PARAM_STALL_DURATION));
		return;
	}

	if (pAiTickParam->bCombat)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, 4);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUICIDEMINION_AI_PARAM_STALL_DURATION));
		return;
	}

	if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, SUICIDEMINION_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUICIDEMINION_AI_PARAM_STALL_DURATION));
		return;
	}

	AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
}

enum D2C_SuccubusAIParams
{
	SUCCUBUS_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	SUCCUBUS_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	SUCCUBUS_AI_PARAM_CURSE_CHANCE_PCT = 2,
	SUCCUBUS_AI_PARAM_CURSE_RANGE = 3,
	SUCCUBUS_AI_PARAM_MELEE_STALL_DURATION = 4,
	SUCCUBUS_AI_PARAM_RANGED_STALL_DURATION = 5,
	SUCCUBUS_AI_PARAM_CURSE_LEVEL = 6,
	SUCCUBUS_AI_PARAM_CURSE2_LEVEL = 7,
};

//D2Game.0x6FCE0CD0
void __fastcall AITHINK_Fn118_Succubus(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;

	if (!STATLIST_GetStatListFromUnitAndFlag(pAiTickParam->pTarget, 0x20u) 
		&& nDistance < AI_GetParamValue(pGame, pAiTickParam, SUCCUBUS_AI_PARAM_CURSE_RANGE) 
		&& (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUS_AI_PARAM_CURSE_CHANCE_PCT)))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] > 0 && UNITS_GetCurrentLifePercentage(pAiTickParam->pTarget) >= AI_GetParamValue(pGame, pAiTickParam, SUCCUBUS_AI_PARAM_CURSE_LEVEL))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[1] > 0 && UNITS_GetCurrentLifePercentage(pUnit) <= AI_GetParamValue(pGame, pAiTickParam, SUCCUBUS_AI_PARAM_CURSE2_LEVEL))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[2] > 0)
		{
			if (STATLIST_GetMaxLifeFromUnit(pAiTickParam->pTarget) > STATLIST_GetMaxManaFromUnit(pAiTickParam->pTarget) || !pAiTickParam->pTarget || pAiTickParam->pTarget->dwUnitType != UNIT_PLAYER)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
				return;
			}
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[3] > 0 && pAiTickParam->pTarget && pAiTickParam->pTarget->dwUnitType == UNIT_PLAYER)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUS_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUCCUBUS_AI_PARAM_MELEE_STALL_DURATION));
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[4] > 0 && AI_GetParamValue(pGame, pAiTickParam, SUCCUBUS_AI_PARAM_CURSE2_LEVEL) > 0)
	{
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
		if (pTarget && AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUS_AI_PARAM_CURSE2_LEVEL))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pTarget, 0, 0);
			return;
		}
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUS_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUCCUBUS_AI_PARAM_RANGED_STALL_DURATION));
}

enum D2C_SuccubusWitchAIParams
{
	SUCCUBUSWITCH_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	SUCCUBUSWITCH_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	SUCCUBUSWITCH_AI_PARAM_WALK_AWAY_CHANCE_PCT = 2,
	SUCCUBUSWITCH_AI_PARAM_COMFORT_DISTANCE = 3,
	SUCCUBUSWITCH_AI_PARAM_SHOOT_CHANCE_PCT = 4,
	SUCCUBUSWITCH_AI_PARAM_STALL_DURATION = 5,
	SUCCUBUSWITCH_AI_PARAM_AMPLIFY_DAMAGE_TARGET_HP_PCT = 6,
	SUCCUBUSWITCH_AI_PARAM_WEAKEN_MY_HP_PCT = 7,
};

//D2Game.0x6FCE0FE0
void __fastcall AITHINK_Fn119_SuccubusWitch(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;

	if (pAiTickParam->pTarget && !STATLIST_GetStatListFromUnitAndFlag(pAiTickParam->pTarget, 0x20u) 
		&& nDistance < AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_COMFORT_DISTANCE)
		&& (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_WALK_AWAY_CHANCE_PCT)))
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] > 0 && UNITS_GetCurrentLifePercentage(pAiTickParam->pTarget) >= AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_AMPLIFY_DAMAGE_TARGET_HP_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[1] > 0 && UNITS_GetCurrentLifePercentage(pUnit) <= AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_WEAKEN_MY_HP_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[2] > 0)
		{
			if (STATLIST_GetMaxLifeFromUnit(pAiTickParam->pTarget) > STATLIST_GetMaxManaFromUnit(pAiTickParam->pTarget) || pAiTickParam->pTarget->dwUnitType != UNIT_PLAYER)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pAiTickParam->pTarget, 0, 0);
				return;
			}
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[3] > 0 && pAiTickParam->pTarget->dwUnitType == UNIT_PLAYER)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	if (pAiTickParam->bCombat)
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_WALK_AWAY_CHANCE_PCT) || !D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_COMFORT_DISTANCE), 1))
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_STALL_DURATION));
			}
		}
	}
	else
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[4] >= 0 && AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_WEAKEN_MY_HP_PCT) > 0 && (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_SHOOT_CHANCE_PCT)))
		{
			D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
			if (pTarget && (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_WEAKEN_MY_HP_PCT)))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pTarget, 0, 0);
				return;
			}
		}

		if (nDistance >= AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_COMFORT_DISTANCE)
			|| !AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_WALK_AWAY_CHANCE_PCT) 
			|| !D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_COMFORT_DISTANCE), 1))
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[4] < 0)
			{
				D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
				if (pTarget && (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_SHOOT_CHANCE_PCT)))
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL2, pTarget);
					return;
				}
			}

			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_APPROACH_CHANCE_PCT))
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
			}
			else
			{
				if (AI_RollPercentage(pUnit) >= 50 || !sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 6u, 1))
				{
					AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SUCCUBUSWITCH_AI_PARAM_STALL_DURATION));
				}
			}
		}
	}
}

//D2Game.0x6FCE1480
void __fastcall D2GAME_AI_SpecialState14_6FCE1480(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AI_RollPercentage(pUnit) < 95)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (AI_RollPercentage(pUnit) < 89)
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

enum D2C_OverseerAIParams
{
	OVERSEER_AI_PARAM_RALLY_SPELL_TIMER = 0,
	OVERSEER_AI_PARAM_HEAL_CHANCE_PCT = 1,
	OVERSEER_AI_PARAM_WHIP_CHANCE_PCT = 2,
	OVERSEER_AI_PARAM_COMFORT_DISTANCE = 3,
	OVERSEER_AI_PARAM_COMFORT_ZONE = 4,
	OVERSEER_AI_PARAM_ATTACK_CHANCE_PCT = 5,
	OVERSEER_AI_PARAM_ATTACK_2_OR_1_CHANCE_PCT = 6,
};

//D2Game.0x6FCE1550
void __fastcall AITHINK_Fn120_Overseer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	ACT5Q1_OnSiegeBossActivated(pGame, pUnit);

	D2UnitStrc* pTarget = AITACTICS_GetTargetMinion(pGame, pUnit);
	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 
		&& sub_6FCF2E70(pUnit) && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[0] 
		&& pTarget && AIUTIL_CheckIfMonsterUsesSkill(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, OVERSEER_AI_PARAM_RALLY_SPELL_TIMER);
		return;
	}

	if (pAiTickParam->bCombat && (AIRollChanceParam(pGame, pUnit, pAiTickParam, OVERSEER_AI_PARAM_ATTACK_CHANCE_PCT)))
	{
		if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, OVERSEER_AI_PARAM_ATTACK_2_OR_1_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
		}
	}
	else
	{
		D2NihlathakOverseerAiCallbackArgStrc arg = {};
		arg.nMaxDistance = 576;
		arg.nDistantMaxDistance = 400;
		arg.nMaxLifePercentage = 50;

		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Overseer_Nihlathak, 1);

		if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && arg.pTarget && AIRollChanceParam(pGame, pUnit, pAiTickParam, OVERSEER_AI_PARAM_HEAL_CHANCE_PCT) && AIUTIL_CheckIfMonsterUsesSkill(pUnit, pAiTickParam->pMonstatsTxt->nSkill[1]))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], arg.pTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[2]
			&& arg.pDistantTarget
			&& AIRollChanceParam(pGame, pUnit, pAiTickParam, OVERSEER_AI_PARAM_WHIP_CHANCE_PCT)
			&& STATLIST_GetUnitAlignment(pUnit) != UNIT_ALIGNMENT_GOOD
			&& AIUTIL_CheckIfMonsterUsesSkill(pUnit, pAiTickParam->pMonstatsTxt->nSkill[2]))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], arg.pDistantTarget, 0, 0);
			return;
		}

		if (arg.nCounter)
		{
			if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, OVERSEER_AI_PARAM_COMFORT_DISTANCE) - AI_GetParamValue(pGame, pAiTickParam, OVERSEER_AI_PARAM_COMFORT_ZONE))
			{
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, (uint8_t)AI_GetParamValue(pGame, pAiTickParam, 3) - pAiTickParam->nTargetDistance, 0);
				return;
			}

			if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, OVERSEER_AI_PARAM_COMFORT_ZONE) + AI_GetParamValue(pGame, pAiTickParam, OVERSEER_AI_PARAM_COMFORT_DISTANCE))
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
				return;
			}

			if (AI_RollPercentage(pUnit) < 50)
			{
				sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 5u, 0);
				return;
			}
		}
		else
		{
			if (AI_RollPercentage(pUnit) < 60)
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
	}
}

//D2Game.0x6FCE1910
D2UnitStrc* __fastcall AITHINK_TargetCallback_Overseer_Nihlathak(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2NihlathakOverseerAiCallbackArgStrc* pArg = (D2NihlathakOverseerAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER)
	{
		return nullptr;
	}

	int32_t nClassId = pTarget->dwClassId;
	D2MonStatsTxt* pMonstatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
	if (pMonstatsTxtRecord)
	{
		nClassId = pMonstatsTxtRecord->nBaseId;
	}

	if (nClassId != MONSTER_MINION1 || MONSTERS_IsDead(pTarget) || sub_6FCBD900(pGame, pUnit, pTarget) || STATES_CheckState(pTarget, STATE_BLOODLUST))
	{
		return nullptr;
	}

	++pArg->nCounter;

	if (!pArg->pTarget && AITHINK_GetSquaredDistance(pUnit, pTarget) <= pArg->nMaxDistance && UNITS_GetCurrentLifePercentage(pTarget) < pArg->nMaxLifePercentage)
	{
		pArg->pTarget = pTarget;
	}

	if (!pArg->pDistantTarget && AITHINK_GetSquaredDistance(pUnit, pTarget) <= pArg->nDistantMaxDistance && !MONSTERUNIQUE_CheckMonTypeFlag(pTarget, MONTYPEFLAG_UNIQUE))
	{
		pArg->pDistantTarget = pTarget;
	}

	return nullptr;
}

enum D2C_MinionSpawnerAIParams
{
	MINIONSPAWNER_AI_PARAM_NUM_TO_SPAWN = 0,
	MINIONSPAWNER_AI_PARAM_STALL_DURATION = 1,
	MINIONSPAWNER_AI_PARAM_SPAWN_DELAY = 2,
	MINIONSPAWNER_AI_PARAM_ACTIVATION_RANGE = 3,
	MINIONSPAWNER_AI_PARAM_MAX_IN_AREA = 4, // Unused
};

//D2Game.0x6FCE1B90
void __fastcall AITHINK_Fn121_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2MinionSpawnerAiCallbackArgStrc arg = {};

	if (pAiTickParam->pAiControl->dwAiParam[1] < AI_GetParamValue(pGame, pAiTickParam, MINIONSPAWNER_AI_PARAM_NUM_TO_SPAWN))
	{
		if (pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, MINIONSPAWNER_AI_PARAM_ACTIVATION_RANGE) && pGame->dwGameFrame >= pAiTickParam->pAiControl->dwAiParam[0])
		{
			arg.nMinions = 0;
			sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_MinionSpawner, 1);

			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && arg.nMinions < AI_GetParamValue(pGame, pAiTickParam, MINIONSPAWNER_AI_PARAM_STALL_DURATION))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, MINIONSPAWNER_AI_PARAM_SPAWN_DELAY);
				++pAiTickParam->pAiControl->dwAiParam[1];
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, MINIONSPAWNER_AI_PARAM_STALL_DURATION));
	}
}

//D2Game.0x6FCE1CA0
D2UnitStrc* __fastcall AITHINK_TargetCallback_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2MinionSpawnerAiCallbackArgStrc* pArg = (D2MinionSpawnerAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || !pTarget || pTarget->dwUnitType != UNIT_MONSTER || MONSTERS_IsDead(pTarget))
	{
		return 0;
	}

	int32_t nTargetBaseClass = pTarget->dwClassId;
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nTargetBaseClass);
	if (pMonStatsTxtRecord)
	{
		nTargetBaseClass = pMonStatsTxtRecord->nBaseId;
	}

	if ((nTargetBaseClass == MONSTER_MINION1 || nTargetBaseClass == MONSTER_SUICIDEMINION1) && STATLIST_GetUnitAlignment(pTarget) == UNIT_ALIGNMENT_EVIL)
	{
		++pArg->nMinions;
	}

	return 0;
}

//D2Game.0x6FCE1D30
void __fastcall D2GAME_AI_SpecialState16_6FCE1D30(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nSkillIndices[2] = { 2, 4 };

	int32_t nIndex = ITEMS_RollRandomNumber(&pUnit->pSeed) & 1;

	for (int32_t i = 0; i < 2; ++i)
	{
		nIndex = (nIndex + 1) % 2;

		if (SKILLS_GetSkillById(pUnit, pAiTickParam->pMonstatsTxt->nSkill[nSkillIndices[nIndex]], -1))
		{
			pAiTickParam->pAiControl->dwAiParam[1] = nSkillIndices[nIndex];
			pAiTickParam->pAiControl->dwAiParam[2] = 0;
			return;
		}
	}

	pAiTickParam->pAiControl->dwAiParam[1] = 0;
	pAiTickParam->pAiControl->dwAiParam[2] = 0;
}

//D2Game.0x6FCE1DC0
void __fastcall D2GAME_AI_SpecialState16_6FCE1DC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
	if (!STATES_CheckState(pUnit, STATE_ATTACHED) || !pOwner)
	{
		D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);

		AITHINK_ExecuteAiFn(pGame, pUnit, pAiControl, AISPECIALSTATE_NONE);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 1);
		return;
	}

	D2MonStatsTxt* pImp1TxtRecord = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_IMP1);
	D2MonStatsTxt* pImp2TxtRecord = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_IMP2);

	if (pAiTickParam->nTargetDistance > 24 || SUNIT_IsDead(pOwner) || STATLIST_GetUnitAlignment(pOwner) != UNIT_ALIGNMENT_EVIL || !pAiTickParam->pAiControl->dwAiParam[1])
	{
		AITACTICS_UseSkillInRange(pUnit, pImp1TxtRecord->wAiParam[1][pGame->nDifficulty], pImp1TxtRecord->nSkill[0], pImp1TxtRecord->nSkillMode[0]);

		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pUnit->pMonsterData->pAiControl->dwAiParam[0] = -1;
		}
		return;
	}

	if (pAiTickParam->nTargetDistance >= pImp2TxtRecord->wAiParam[1][pGame->nDifficulty] || (AI_RollPercentage(pUnit) >= pImp2TxtRecord->wAiParam[2][pGame->nDifficulty]))
	{
		if (AI_RollPercentage(pUnit) < 50)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		const int32_t nParam = pAiTickParam->pAiControl->dwAiParam[1];

		if (pAiTickParam->pAiControl->dwAiParam[2])
		{
			if (2 * pImp2TxtRecord->wAiParam[3][pGame->nDifficulty] > 0)
			{
				ITEMS_RollRandomNumber(&pUnit->pSeed);
			}

			if (2 * pImp2TxtRecord->wAiParam[3][pGame->nDifficulty] > 0)
			{
				ITEMS_RollRandomNumber(&pUnit->pSeed);
			}
		}

		if (pImp1TxtRecord->nSkill[nParam] >= 0)
		{
			AITACTICS_UseSkill(pGame, pUnit, pImp1TxtRecord->nSkillMode[nParam], pImp1TxtRecord->nSkill[nParam], pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
}

//D2Game.0x6FCE2080
void __fastcall D2GAME_AI_Unk122_6FCE2080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = -1;
}

enum D2C_ImpAIParams
{
	IMP1_AI_PARAM_CAST_SKILL_HP_PCT = 0,
	IMP1_AI_PARAM_CAST_SKILL_RANGE = 1,

	IMP2_AI_PARAM_APPROACH_DISTANCE = 0,

	IMP3_AI_PARAM_WALK_AWAY_DISTANCE = 0,
	IMP3_AI_PARAM_WALK_AWAY_CHANCE_PCT = 1,
	IMP3_AI_PARAM_CAST_SKILL_DISTANCE = 2,
	IMP3_AI_PARAM_CAST_SKILL_CHANCE_PCT = 3,

	IMP4_AI_PARAM_CAST_SKILL_DISTANCE = 2,
	IMP4_AI_PARAM_CAST_SKILL_CHANCE_PCT = 3,
};

//D2Game.0x6FCE2090
void __fastcall AITHINK_Fn122_Imp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nDistance = pAiTickParam->nTargetDistance;

	D2MonStatsTxt* pImp1Record = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_IMP1);
	D2MonStatsTxt* pImp2Record = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_IMP2);
	D2MonStatsTxt* pImp3Record = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_IMP3);
	D2MonStatsTxt* pImp4Record = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_IMP4);

	if (pAiTickParam->pAiControl->dwAiParam[0] != -1 && STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL)
	{
		D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pAiTickParam->pAiControl->dwAiParam[0]);
		if (!pTarget || SUNIT_IsDead(pTarget) || SUNIT_GetOwner(pGame, pTarget) || STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_EVIL)
		{
			AIGENERAL_GetAiControlFromUnit(pUnit)->dwAiParam[0] = -1;
		}
		else
		{
			if (AITHINK_GetSquaredDistance(pUnit, pTarget) > pImp2Record->wAiParam[IMP2_AI_PARAM_APPROACH_DISTANCE][pGame->nDifficulty] * pImp2Record->wAiParam[IMP2_AI_PARAM_APPROACH_DISTANCE][pGame->nDifficulty])
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 0);
				return;
			}

			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
				return;
			}
		}
	}

	if (pAiTickParam->bCombat)
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && UNITS_GetCurrentLifePercentage(pUnit) < pImp1Record->wAiParam[IMP1_AI_PARAM_CAST_SKILL_HP_PCT][pGame->nDifficulty])
		{
			AITACTICS_UseSkillInRange(pUnit, pImp1Record->wAiParam[IMP1_AI_PARAM_CAST_SKILL_RANGE][pGame->nDifficulty], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pMonstatsTxt->nSkillMode[0]);
			return;
		}

		if (AI_RollPercentage(pUnit) < pImp3Record->wAiParam[IMP3_AI_PARAM_WALK_AWAY_CHANCE_PCT][pGame->nDifficulty] && D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 5u, 1))
		{
			return;
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AI_RollPercentage(pUnit) < pImp1Record->wAiParam[2][pGame->nDifficulty])
	{
		AITACTICS_UseSkillInRange(pUnit, pImp1Record->wAiParam[1][pGame->nDifficulty], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pMonstatsTxt->nSkillMode[0]);
		return;
	}

	if (nDistance < pImp3Record->wAiParam[IMP3_AI_PARAM_WALK_AWAY_DISTANCE][pGame->nDifficulty] && AI_RollPercentage(pUnit) < pImp3Record->wAiParam[IMP3_AI_PARAM_WALK_AWAY_CHANCE_PCT][pGame->nDifficulty])
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 5u, 0);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[3] >= 0)
	{
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, 0);
		if (pTarget)
		{
			if (nDistance < pImp3Record->wAiParam[IMP3_AI_PARAM_CAST_SKILL_DISTANCE][pGame->nDifficulty])
			{
				if (AI_RollPercentage(pUnit) < pImp3Record->wAiParam[IMP3_AI_PARAM_CAST_SKILL_CHANCE_PCT][pGame->nDifficulty])
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
					return;
				}
			}
			else if (nDistance < pImp4Record->wAiParam[IMP4_AI_PARAM_CAST_SKILL_DISTANCE][pGame->nDifficulty])
			{
				if (AI_RollPercentage(pUnit) < pImp4Record->wAiParam[IMP4_AI_PARAM_CAST_SKILL_CHANCE_PCT][pGame->nDifficulty])
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
					return;
				}
			}
		}
	}

	if (AI_RollPercentage(pUnit) < 33)
	{
		AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pAiTickParam->pTarget, 4u);
		return;
	}

	if (AI_RollPercentage(pUnit) < 20)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 8u);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

enum D2C_CatapultAIParams
{
	CATAPULT_AI_PARAM_ATTACK_CHANCE_PCT = 0,
};

//D2Game.0x6FCE2570
void __fastcall AITHINK_Fn123_Catapult(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, CATAPULT_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

enum D2C_FrozenHorrorAIParams
{
	FROZENHORROR_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	FROZENHORROR_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	FROZENHORROR_AI_PARAM_ARCTIC_BLAST_CHANCE_PCT = 2,
	FROZENHORROR_AI_PARAM_STALL_DURATION = 3,
};

//D2Game.0x6FCE25D0
void __fastcall AITHINK_Fn124_FrozenHorror(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nSkillLevel = 0;

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
	{
		D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]);
		if (pSkill)
		{
			nSkillLevel = std::max(SKILLS_GetSkillLevel(pUnit, pSkill, 1), 0);
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && pAiTickParam->nTargetDistance < nSkillLevel && AIRollChanceParam(pGame, pUnit, pAiTickParam, FROZENHORROR_AI_PARAM_ARCTIC_BLAST_CHANCE_PCT) && !STATES_CheckState(pUnit, STATE_INFERNO))
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FROZENHORROR_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else if (AIRollChanceParam(pGame, pUnit, pAiTickParam, FROZENHORROR_AI_PARAM_APPROACH_CHANCE_PCT))
	{
		AITACTICS_WalkToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, FROZENHORROR_AI_PARAM_STALL_DURATION));
}

enum D2C_BloodLordAIParams
{
	BLOODLORD_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	BLOODLORD_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	BLOODLORD_AI_PARAM_FRENZY_ATTACK1_CHANCE_PCT = 2,
	BLOODLORD_AI_PARAM_STALL_DURATION = 3,
};

//D2Game.0x6FCE2760
void __fastcall AITHINK_Fn125_BloodLord(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BLOODLORD_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BLOODLORD_AI_PARAM_FRENZY_ATTACK1_CHANCE_PCT))
			{
				AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				return;
			}

			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BLOODLORD_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, BLOODLORD_AI_PARAM_STALL_DURATION));
}

//D2Game.0x6FCE28A0
void __fastcall AITHINK_Fn097_Spirit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[0])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
		return;
	}

	if (pAiTickParam->bCombat)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE28F0
void __fastcall AITHINK_Fn098_Smith(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	const int32_t nLifePercentage = D2Clamp(UNITS_GetCurrentLifePercentage(pUnit), 0, 100);
	AITACTICS_SetVelocity(pUnit, 0, (100 - nLifePercentage) >> 1, 0);
	AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
}

//D2Game.0x6FCE2960
void __fastcall AITHINK_Fn067_NecroPet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	//TODO: Names

	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		sub_6FCE3740(pGame, pUnit, pAiTickParam);
		return;
	}

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		if (pAiTickParam->pAiControl->dwAiParam[2])
		{
			pOwner = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pAiTickParam->pAiControl->dwAiParam[2]);
			if (pOwner)
			{
				D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = pOwner->dwUnitId;
	AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);

	int32_t nDistance = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pPotentialTarget = sub_6FCF2110(pGame, pOwner, pAiTickParam->pAiControl, &nDistance, &bCombat);
	if (pPotentialTarget && pPotentialTarget->dwFlags & UNITFLAG_PETIGNORE)
	{
		pPotentialTarget = nullptr;
	}

	int32_t bInMeleeRange = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, &bInMeleeRange);
	if (!pTarget || nDistance > 15)
	{
		pTarget = nullptr;
		if (pPotentialTarget && !UNITS_TestCollisionWithUnit(pUnit, pPotentialTarget, COLLIDE_MISSILE_BARRIER))
		{
			nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPotentialTarget);
			if (nDistance < 20)
			{
				pTarget = pPotentialTarget;
			}
		}
	}

	int32_t v17 = 7;
	int32_t v16 = 0;
	if (AI_RollPercentage(pUnit) < 15)
	{
		v17 = 8;
		v16 = 1;
	}

	if (sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, bInMeleeRange, pAiTickParam, v16, v17))
	{
		return;
	}

	if (!pTarget || DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
	{
		sub_6FCD09D0(pGame, pUnit, pOwner, 4u);
		return;
	}

	if (AI_RollPercentage(pUnit) < 80)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		return;
	}

	if (AI_RollPercentage(pUnit) >= 75)
	{
		sub_6FCD0E80(pGame, pUnit, pTarget, 3u, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE2BA0
int32_t __fastcall D2GAME_AI_PetMove_6FCE2BA0(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t a4, int32_t bRun, int32_t nVelocity, int32_t a7)
{
	// TODO: Names
	constexpr uint8_t byte_6FD295C4[] =
	{
		4, 3, 2, 1, 0, 7, 6, 5
	};

	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pOwner);

	const int32_t v65 = D2Common_10224(pOwner->pDynamicPath);
	const int32_t v64 = D2Common_10225(pOwner->pDynamicPath);

	const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
	const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);

	const int32_t nOwnerX = CLIENTS_GetUnitX(pOwner);
	const int32_t nOwnerY = CLIENTS_GetUnitY(pOwner);

	switch (a4)
	{
	case 0:
	{
		uint8_t nIndex = D2Common_11053(UNITS_GetDirectionToCoords(pOwner, v65, v64));

		const int32_t a4a = D2Common_10095(UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));

		for (int32_t i = 0; i < 7; ++i)
		{
			int32_t nXOffset = 0;
			int32_t nYOffset = 0;
			D2Common_11055(byte_6FD295C4[nIndex], &nXOffset, &nYOffset);

			const int32_t nX = v65 + 8 * nXOffset;
			const int32_t nY = v64 + 8 * nYOffset;

			if (D2Common_10095(UNITS_GetRoom(pOwner), nX, nY) == a4a)
			{
				AITACTICS_SetVelocity(pUnit, 0, nVelocity, 40);

				if (bRun)
				{
					if (AITACTICS_RunToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
					{
						return 1;
					}

					if (AITACTICS_RunToTargetCoordinatesDeleteAiEvent(pGame, pUnit, (nX + nUnitX) / 2, (nUnitY + nY) / 2))
					{
						return 1;
					}
				}
				else
				{
					if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
					{
						return 1;
					}

					if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, (nX + nUnitX) / 2, (nUnitY + nY) / 2))
					{
						return 1;
					}
				}
			}

			nIndex = (nIndex + 1) & 7;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		if (sub_6FCD09D0(pGame, pUnit, pOwner, 4u) || bRun && AITACTICS_RunToTargetCoordinatesDeleteAiEvent(pGame, pUnit, (nUnitX + nOwnerX) / 2, (nUnitY + nOwnerY) / 2) || AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, (nOwnerX + nUnitX) / 2, (nUnitY + nOwnerY) / 2))
		{
			return 1;
		}

		return AITACTICS_WalkToTargetCoordinates(pGame, pUnit, nOwnerX, nOwnerY);
	}
	case 1:
	{
		if (pOwner->dwAnimMode == PLRMODE_WALK)
		{
			AITACTICS_SetVelocity(pUnit, 0, 0, 100);

			if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, v65, v64) || AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, (nUnitX + v65) / 2, (nUnitY + v64) / 2))
			{
				return 1;
			}
		}

		if (!nVelocity)
		{
			nVelocity = ITEMS_RollRandomNumber(&pUnit->pSeed) % 40 + 40;
		}

		int32_t b5 = pPlayerData->unk0xA0;

		D2AiParamStrc* pAiParam = nullptr;
		if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pAiParam = pUnit->pMonsterData->pAiParam;
		}

		int32_t b4 = 0;
		for (int32_t i = 0; i < 20; ++i)
		{
			if (b5)
			{
				b5 = b5 - 1;
			}
			else
			{
				b5 = 19;
			}

			const int32_t nX = pPlayerData->unk0xA8[2 * b5];
			const int32_t nY = pPlayerData->unk0xA8[2 * b5 + 1];

			if (nX && nY && AIUTIL_GetDistanceToCoordinates(pUnit, nX, nY) > 5)
			{
				AITACTICS_SetVelocity(pUnit, 0, nVelocity, 100);

				if (bRun)
				{
					if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, nX, nY))
					{
						return 1;
					}
				}
				else
				{
					if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
					{
						return 1;
					}
				}

				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
				AITACTICS_SetVelocity(pUnit, 15, nVelocity, 100);

				if (bRun)
				{
					if (AITACTICS_RunToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
					{
						return 1;
					}
				}
				else
				{
					if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
					{
						return 1;
					}
				}

				if (!b4)
				{
					b4 = 1;

					D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 1, nVelocity, 100);

					if (bRun)
					{
						if (AITACTICS_RunToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
						{
							return 1;
						}
					}
					else
					{
						if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
						{
							return 1;
						}
					}
				}
			}
		}

		int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner) / 4;
		if (nDistance < 4)
		{
			nDistance = 4;
		}

		AITACTICS_SetVelocity(pUnit, 0, 15, 0);

		if (sub_6FCD09D0(pGame, pUnit, pOwner, nDistance))
		{
			return 1;
		}

		return 0;
	}
	case 2:
	{
		if (AI_RollPercentage(pUnit) >= 10)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			return 1;
		}

		if (sub_6FCD09D0(pGame, pUnit, pOwner, ITEMS_RollRandomNumber(&pUnit->pSeed) % 3 + 3))
		{
			return 1;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		AITACTICS_SetVelocity(pUnit, 0, 0, 40);

		if (!AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, v65, v64))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}

		return 1;
	}
	case 3:
	{
		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pOwner);

		int32_t nX = 0;
		int32_t nY = 0;
		if (!sub_6FC66260(pGame, pRoom, D2Common_10096(pRoom, CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner)), MONSTER_NECROSKELETON, &nX, &nY, 0))
		{
			return 0;
		}

		pRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
		if (pRoom && sub_6FCBDFE0(pGame, pUnit, pRoom, nX, nY, 0, 0))
		{
			UNITROOM_RefreshUnit(pUnit);
			pUnit->dwFlagEx |= UNITFLAGEX_TELEPORTED;
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return 1;
		}

		return 0;
	}
	case 4:
	{
		D2PetMoveStrc petMoveArgs = {};
		petMoveArgs.pUnit = pUnit;

		PLAYERPETS_IteratePets(pGame, pOwner, sub_6FCE34B0, &petMoveArgs);

		if (petMoveArgs.nCounter < 1)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			return 1;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pOwner, a7, 1))
		{
			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

			if (!sub_6FCD09D0(pGame, pUnit, pOwner, a7))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
		}

		return 1;
	}
	case 5:
	{
		AITACTICS_SetVelocity(pUnit, 7, 0, 0);

		if (D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pOwner, a7))
		{
			return 1;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pOwner, a7, 1))
		{
			return 1;
		}

		AITACTICS_SetVelocity(pUnit, 0, 0, 40);

		if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, v65, v64))
		{
			return 1;
		}

		return 0;
	}
	default:
		return 0;
	}
}

//D2Game.0x6FCE34B0
void __fastcall sub_6FCE34B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2PetMoveStrc* pArg = (D2PetMoveStrc*)pCallbackArg;
	if (pArg->pUnit == pTarget)
	{
		return;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pArg->pTarget, pTarget);
	if (nDistance > 1)
	{
		return;
	}

	pArg->pTarget = pTarget;
	++pArg->nCounter;
}

//D2Game.0x6FCE34E0
int32_t __fastcall sub_6FCE34E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2UnitStrc* pOwner, int32_t a5, D2AiTickParamStrc* pAiTickParam, int32_t a7, int32_t a8)
{
	//TODO: Names

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);
	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pOwner);

	const int32_t a8a = std::min((PLAYERPETS_GetTotalPetCount(pOwner) >> 1) + a8, 36);

	if (nDistance > 1 || pOwner->dwAnimMode != 1 || a5)
	{
		if (pTarget && !DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
		{
			if (nDistance > 80)
			{
				return D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, a8a);
			}

			return 0;
		}

		const int32_t v37 = D2Common_10095(UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
		const int32_t v36 = D2Common_10095(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));

		const int32_t v26 = D2COMMON_10175_PathGetFirstPointX(pOwner->pDynamicPath);
		const int32_t pTargeta = D2COMMON_10176_PathGetFirstPointY(pOwner->pDynamicPath);
		const int32_t a5a = D2Common_10224(pOwner->pDynamicPath);
		const int32_t v27 = D2Common_10225(pOwner->pDynamicPath);

		int32_t v28 = 2;

		if (pOwner->dwAnimMode == 2 || pOwner->dwAnimMode == 6 || pOwner->dwAnimMode == 3)
		{
			v28 = 0;
		}

		if (v26 != a5a && pTargeta != v27)
		{
			v28 = 0;
		}

		if (v37 != v36 || nDistance > a8a)
		{
			v28 = 1;
		}

		if (nDistance > 50)
		{
			v28 = 3;
		}

		if (AIUTIL_GetDistanceToCoordinates(pUnit, pPlayerData->unk0xA8[40], pPlayerData->unk0xA8[41]) < 28)
		{
			if (v28 == 1)
			{
				if (nDistance < 30)
				{
					v28 = 4;
				}
			}
			else if (v28 == 2)
			{
				v28 = 4;
			}
		}

		if (!a7 || v28 != 2)
		{
			return D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, v28, 0, 0, a8a);
		}

		return 0;
	}

	return D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 5, 0, 0, a8a);
}

//D2Game.0x6FCE3740
void __fastcall sub_6FCE3740(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		if (pAiTickParam->pAiControl->dwAiParam[2])
		{
			D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pAiTickParam->pAiControl->dwAiParam[2]);
			if (pPlayer)
			{
				D2GAME_AI_PetMove_6FCE2BA0(pGame, pPlayer, pUnit, 3, 0, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = pOwner->dwUnitId;

	const int32_t nDistanceToOwner = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);
	if (nDistanceToOwner > 50)
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, 0);
		return;
	}

	int32_t nVel = 0;
	if (pAiTickParam->pMonstatsTxt->nVelocity <= 0)
	{
		nVel = 100;
	}
	else
	{
		nVel = std::min(100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity - 100, 100);
	}

	if (nDistanceToOwner > 28)
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, nVel, 0);
		return;
	}

	int32_t nUnused1 = 0;
	int32_t nUnused2 = 0;
	D2UnitStrc* pPotentialTarget = sub_6FCF2110(pGame, pOwner, pAiTickParam->pAiControl, &nUnused1, &nUnused2);
	if (pPotentialTarget && pPotentialTarget->dwFlags & UNITFLAG_PETIGNORE)
	{
		pPotentialTarget = nullptr;
	}

	int32_t nTargetDistance = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget = sub_6FCCFD40(pGame, pUnit, pAiTickParam->pAiControl, &nTargetDistance, &bCombat, 24);
	if (!pTarget || nTargetDistance > 6)
	{
		pTarget = nullptr;
		if (pPotentialTarget)
		{
			const int32_t nPotentialTargetDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPotentialTarget);
			if (nPotentialTargetDistance < 36)
			{
				pTarget = pPotentialTarget;
			}
		}
	}

	if (!sub_6FCF14D0(pUnit, pTarget))
	{
		pTarget = nullptr;
	}

	// TODO: Names
	int32_t v23 = 0;
	int32_t v24 = 0;
	if (AI_RollPercentage(pUnit) >= 15)
	{
		v24 = 7;
		v23 = 0;
	}
	else
	{
		v24 = 8;
		v23 = 1;
	}

	if (sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, bCombat, pAiTickParam, v23, v24))
	{
		return;
	}

	if (!pTarget || DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
	{
		sub_6FCD09D0(pGame, pUnit, pOwner, 4u);
		return;
	}

	if (bCombat)
	{
		if (AI_RollPercentage(pUnit) < 80)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
	}
	else
	{
		AITACTICS_SetVelocity(pUnit, 0, 0, 12);
		sub_6FCD0410(pGame, pUnit, pTarget, 7);
	}
}

//D2Game.0x6FCE39E0
void __fastcall AITHINK_Fn061_Hireable(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nClassId = -1;
	if (pUnit)
	{
		nClassId = pUnit->dwClassId;
	}

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
	{
		if (nClassId == MONSTER_ROGUEHIRE)
		{
			AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_ROGUE_HIREABLE_NON_PLAYER_OWNER);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_HIREABLE_NON_PLAYER_OWNER);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		return;
	}

	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	int32_t nAnimMode = 0;
	if (pUnit)
	{
		nAnimMode = pUnit->dwAnimMode;
		if (nAnimMode == MONMODE_WALK || nAnimMode == MONMODE_RUN)
		{
			return;
		}
	}

	AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);

	const int32_t nParam = pAiTickParam->pAiControl->dwAiParam[0];
	int32_t nMinDistance = 0;
	int32_t nMaxDistance = 0;
	int32_t nSteps = 0;
	if (nParam <= 16 || nParam >= 20)
	{
		nMaxDistance = 24;
		nMinDistance = 16;
		nSteps = 5;
	}
	else
	{
		nMaxDistance = 2 * nParam;
		nMinDistance = nParam;
		nSteps = nParam >> 1;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);
	if (nDistance > 100)
	{
		D2GAME_PETAI_PetMove_6FCE3EE0(pGame, pOwner, pUnit, 3, 0, 0, 0);
		return;
	}

	if (nDistance > nMaxDistance)
	{
		D2GAME_PETAI_PetMove_6FCE3EE0(pGame, pOwner, pUnit, 1, 1, 60, 0);
		return;
	}

	if (nDistance > nMinDistance)
	{
		if (pOwner->dwAnimMode == PLRMODE_WALK || pOwner->dwAnimMode == PLRMODE_TOWNWALK)
		{
			D2GAME_PETAI_PetMove_6FCE3EE0(pGame, pOwner, pUnit, 0, 0, 0, 0);
			return;
		}

		if (pOwner->dwAnimMode == PLRMODE_RUN)
		{
			D2GAME_PETAI_PetMove_6FCE3EE0(pGame, pOwner, pUnit, 0, 1, 60, 0);
			return;
		}
	}

	if (nAnimMode == MONMODE_NEUTRAL)
	{
		int32_t bWalk = 0;
		if (COLLISION_CheckAnyCollisionWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), PATH_GetUnitCollisionPattern(pUnit), COLLIDE_MISSILE))
		{
			bWalk = 1;

			if (nClassId != MONSTER_ACT2HIRE && (nClassId <= MONSTER_BAALCRABSTAIRS || nClassId > MONSTER_ACT5HIRE2))
			{
				if ((ITEMS_RollRandomNumber(&pUnit->pSeed) & 127) < 12)
				{
					AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
					return;
				}
			}
			else
			{
				if ((ITEMS_RollRandomNumber(&pUnit->pSeed) & 127) < 6)
				{
					AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
					return;
				}
			}
		}

		if (!DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
		{
			int32_t nDist = 0;
			int32_t nTemp = 0;
			D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDist, &nTemp);
			if (pTarget && nDist < 25)
			{
				sub_6FCE4610(pGame, pUnit, nClassId, pOwner, pTarget, &pUnit->pSeed, pAiTickParam);
				return;
			}
		}

		if (bWalk)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
			return;
		}

		if (D2Common_10095(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit)) != D2Common_10095(UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner)))
		{
			D2GAME_PETAI_PetMove_6FCE3EE0(pGame, pOwner, pUnit, 0, 1, 60, 0);
			return;
		}

		if (AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner) <= 1)
		{
			D2GAME_PETAI_PetMove_6FCE3EE0(pGame, pOwner, pUnit, 5, 0, 0, nSteps - 1);
			return;
		}

		if (AI_RollPercentage(pUnit) < 5)
		{
			D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pOwner, nMinDistance);
			return;
		}
	}
	else
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
}

//D2Game.0x6FCE3EE0
int32_t __fastcall D2GAME_PETAI_PetMove_6FCE3EE0(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t eMotionType, int32_t bRun, int32_t nSpeed, BYTE bSteps)
{
	// TODO: Names
	constexpr char byte_6FD295BC[] =
	{
		4, 3, 2, 1, 0, 7, 6, 5
	};

	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pOwner);
	const int32_t v11 = D2Common_10224(pOwner->pDynamicPath);
	const int32_t v12 = D2Common_10225(pOwner->pDynamicPath);

	switch (eMotionType)
	{
	case 0:
	{
		uint8_t nIndex = D2Common_11053(UNITS_GetDirectionToCoords(pOwner, v11, v12));

		const int32_t a4 = D2Common_10095(UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));

		for (int32_t i = 0; i < 7; ++i)
		{
			int32_t nXOffset = 0;
			int32_t nYOffset = 0;
			D2Common_11055(byte_6FD295BC[nIndex], &nXOffset, &nYOffset);

			const int32_t nX = v11 + 8 * nXOffset;
			const int32_t nY = v12 + 8 * nYOffset;

			if (D2Common_10095(UNITS_GetRoom(pOwner), nX, nY) == a4)
			{
				AITACTICS_SetVelocity(pUnit, 0, nSpeed, 40);
				if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
				{
					return 1;
				}
			}

			nIndex = (nIndex + 1) & 7;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
		sub_6FCD09D0(pGame, pUnit, pOwner, 4u);
		return 1;
	}
	case 1:
	{
		if (pOwner->dwAnimMode == PLRMODE_WALK)
		{
			AITACTICS_SetVelocity(pUnit, 0, 0, 100);
			AITACTICS_WalkToTargetCoordinates(pGame, pUnit, v11, v12);
			return 1;
		}

		if (!nSpeed)
		{
			nSpeed = ITEMS_RollRandomNumber(&pUnit->pSeed) % 15 + 50;
		}

		int32_t v29 = pPlayerData->unk0xA0;
		D2AiParamStrc* pAiParam = nullptr;
		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pAiParam = pUnit->pMonsterData->pAiParam;
		}

		int32_t bVelocityParamsSet = 0;

		for (int32_t i = 0; i < 20; ++i)
		{
			if (v29)
			{
				v29 = v29 - 1;
			}
			else
			{
				v29 = 19;
			}

			const int32_t nX = pPlayerData->unk0xA8[2 * v29];
			const int32_t nY = pPlayerData->unk0xA8[2 * v29 + 1];
			if (AIUTIL_GetDistanceToCoordinates(pUnit, nX, nY) > 5)
			{
				AITACTICS_SetVelocity(pUnit, 0, nSpeed, 100);
				if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
				{
					return 1;
				}

				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
				AITACTICS_SetVelocity(pUnit, 15, nSpeed, 100);

				if (bRun)
				{
					if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, nX, nY))
					{
						return 1;
					}
				}
				else
				{
					if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
					{
						return 1;
					}
				}

				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

				if (!bVelocityParamsSet)
				{
					bVelocityParamsSet = 1;

					D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 1, nSpeed, 100);

					if (bRun)
					{
						if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, nX, nY))
						{
							return 1;
						}
					}
					else
					{
						if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
						{
							return 1;
						}
					}

					D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
				}
			}
		}

		int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner) / 4;
		if (nDistance < 4)
		{
			nDistance = 4;
		}

		AITACTICS_SetVelocity(pUnit, 0, 15, 0);
		sub_6FCD09D0(pGame, pUnit, pOwner, nDistance);
		return 1;
	}
	case 2:
	{
		if (AI_RollPercentage(pUnit) >= 10)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		else
		{
			if (!sub_6FCD09D0(pGame, pUnit, pOwner, ITEMS_RollRandomNumber(&pUnit->pSeed) % 3 + 3))
			{
				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
				AITACTICS_SetVelocity(pUnit, 0, 0, 40);
				AITACTICS_WalkToTargetCoordinates(pGame, pUnit, v11, v12);
			}
		}
		return 1;
	}
	case 3:
	{
		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pOwner);
		int32_t nX = 0;
		int32_t nY = 0;
		if (!sub_6FC66260(pGame, pRoom, D2Common_10096(pRoom, CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner)), MONSTER_NECROSKELETON, &nX, &nY, 0))
		{
			return 0;
		}

		pRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
		if (!pRoom || !sub_6FCBDFE0(pGame, pUnit, pRoom, nX, nY, 0, 0))
		{
			return 0;
		}

		UNITROOM_RefreshUnit(pUnit);
		if (pUnit)
		{
			pUnit->dwFlagEx |= UNITFLAGEX_TELEPORTED;
		}
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return 1;
	}
	case 4:
	{
		D2PetMoveStrc petMoveArgs = {};
		petMoveArgs.pUnit = pUnit;

		PLAYERPETS_IteratePets(pGame, pOwner, sub_6FCE34B0, &petMoveArgs);
		if (petMoveArgs.nCounter < 1)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		else
		{
			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

			if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pOwner, bSteps, 1))
			{
				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
				sub_6FCD09D0(pGame, pUnit, pOwner, bSteps);
			}
		}
		return 1;
	}
	case 5:
	{
		AITACTICS_SetVelocity(pUnit, 7, 0, 0);
		if (!D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pOwner, bSteps))
		{
			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
			if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pOwner, bSteps, 1))
			{
				AITACTICS_SetVelocity(pUnit, 0, 0, 40);
				AITACTICS_WalkToTargetCoordinates(pGame, pUnit, v11, v12);
			}
		}
		return 1;
	}
	default:
		return 0;
	}
}


enum D2C_HireableAIParams
{
	HIREABLE_AI_PARAM_IS_MELEE = 0,
};

//D2Game.0x6FCE4610
void __fastcall sub_6FCE4610(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nClassId, D2UnitStrc* pOwner, D2UnitStrc* pTarget, D2SeedStrc* pSeed, D2AiTickParamStrc* pAiTickParam)
{
	uint32_t nChance = 0;
	if (nClassId == MONSTER_ACT2HIRE || nClassId == MONSTER_ACT5HIRE1 || nClassId == MONSTER_ACT5HIRE2)
	{
		nChance = 98;
	}
	else
	{
		nChance = std::min(pAiTickParam->pAiControl->dwAiParam[0] + 40 + 2 * STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 95);
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget);
	int32_t nUnitGUID = -1;
	if (pUnit)
	{
		nUnitGUID = pUnit->dwUnitId;
	}

	D2PetInfoStrc* pPetInfo = PLAYERPETS_GetPetInfoFromPetGUID(pOwner, nUnitGUID);
	if (!pPetInfo)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	const int32_t nHirelingId = pPetInfo->nHirelingId;

	int32_t bUseSkill = 0;
	if (AI_RollPercentage(pUnit) >= nChance)
	{
		bUseSkill = 0;
		pAiTickParam->pAiControl->dwAiParam[0] += 10;
	}
	else
	{
		bUseSkill = 1;
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
	}

	if (AI_GetParamValue(pGame, pAiTickParam, HIREABLE_AI_PARAM_IS_MELEE))
	{
		if (nDistance >= 3 || !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
		{
			PATH_SetStepNum(pUnit->pDynamicPath, pAiTickParam->pMonstats2Txt->nMeleeRng);
			AITACTICS_RunToTargetUnitWithFlags(pGame, pUnit, pTarget, 1);
			return;
		}

		if (bUseSkill)
		{
			sub_6FCE4830(pGame, pUnit, nClassId, nHirelingId, pTarget, pSeed);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		return;
	}

	if (nDistance >= 4 || (AI_RollPercentage(pUnit) >= 50))
	{
		if (bUseSkill)
		{
			sub_6FCE4830(pGame, pUnit, nClassId, nHirelingId, pTarget, pSeed);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		return;
	}

	if (!D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pOwner, 4u))
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pTarget, 4u, 1))
		{
			sub_6FCE4830(pGame, pUnit, nClassId, nHirelingId, pTarget, pSeed);
		}
	}
}

//D2Game.0x6FCE4830
void __fastcall sub_6FCE4830(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nHirelingId, D2UnitStrc* pTarget, D2SeedStrc* pSeed)
{
	D2MonStatsTxt* pMonstatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
	if (!pMonstatsTxtRecord)
	{
		return;
	}

	const int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

	D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, nHirelingId, nLevel);
	if (!pHirelingTxtRecord)
	{
		return;
	}

	uint32_t nChance = pHirelingTxtRecord->dwDefaultChance;
	const int32_t nLevelDiff = std::max(nLevel - pHirelingTxtRecord->nHirelingLevel, 0);

	uint32_t nSkillChances[6] = {};
	for (int32_t i = 0; i < std::size(nSkillChances); ++i)
	{
		const int32_t nSkillId = pHirelingTxtRecord->dwSkill[i];

		D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
		if (!pSkillsTxtRecord)
		{
			break;
		}

		nSkillChances[i] = 0;

		D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1);
		if (pSkill && SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 0)
		{
			if (pSkillsTxtRecord->nAiType != 1 || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
			{
				if (nSkillId != SKILL_INFERNO)
				{
					nChance += pHirelingTxtRecord->dwChance[i] + nLevelDiff * pHirelingTxtRecord->dwChancePerLvl[i] / 4;
					nSkillChances[i] = nChance;
				}
				else
				{
					if (!pTarget || UNITS_GetDistanceToOtherUnit(pUnit, pTarget) <= SKILLS_GetSkillLevel(pUnit, pSkill, 1) / 2 + 4)
					{
						nChance += pHirelingTxtRecord->dwChance[i] + nLevelDiff * pHirelingTxtRecord->dwChancePerLvl[i] / 4;
						nSkillChances[i] = nChance;
					}
				}
			}
		}
	}

	const uint32_t nRoll = ITEMS_RollLimitedRandomNumber(pSeed, nChance + 1);

	if (nRoll >= pHirelingTxtRecord->dwDefaultChance)
	{
		for (int32_t i = 0; i < std::size(nSkillChances); ++i)
		{
			if (nRoll <= nSkillChances[i])
			{
				const int32_t nSkillId = pHirelingTxtRecord->dwSkill[i];
				if (nSkillId > 0)
				{
					D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

					if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA]))
					{
						AITACTICS_UseSkill(pGame, pUnit, pHirelingTxtRecord->nMode[i], nSkillId, pTarget, 0, 0);
						return;
					}

					D2GAME_AssignSkill_6FD13800(pUnit, 0, nSkillId, -1);
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
					return;
				}
				break;
			}
		}
	}

	switch (nMonsterId)
	{
	case MONSTER_ROGUEHIRE:
		AITACTICS_UseSkill(pGame, pUnit, pMonstatsTxtRecord->nSkillMode[0], pMonstatsTxtRecord->nSkill[0], pTarget, 0, 0);
		return;

	case MONSTER_ACT2HIRE:
	case MONSTER_ACT3HIRE:
	case MONSTER_ACT5HIRE1:
	case MONSTER_ACT5HIRE2:
		if (UNITS_IsInMeleeRange(pUnit, pTarget, 0))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}

		break;
	default:
		break;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE4B90
void __fastcall D2GAME_AI_SpecialState03_6FCE4B90(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	D2_ASSERT(pOwner);

	pAiTickParam->pAiControl->dwAiParam[0] = CLIENTS_GetUnitX(pUnit) - CLIENTS_GetUnitX(pOwner);
	pAiTickParam->pAiControl->dwAiParam[1] = CLIENTS_GetUnitY(pUnit) - CLIENTS_GetUnitY(pOwner);

	if (pOwner == pUnit)
	{
		pAiTickParam->pAiControl->dwAiParam[2] = AI_RollPercentage(pUnit);
	}
}

//D2Game.0x6FCE4CC0
void __fastcall D2GAME_AI_SpecialState03_6FCE4CC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);

	if (pCurrentAiCmd && pCurrentAiCmd->nCmdParam[0] == 8)
	{
		AIGENERAL_FreeMinionList(pUnit);
		AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_NONE);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	int32_t bMinionDead = 0;
	AIGENERAL_ExecuteCallbackOnMinions(pUnit, &bMinionDead, 0, sub_6FCE4FD0);

	D2AiCmdStrc aiCmd = {};
	if (sub_6FCF2E70(pUnit) || bMinionDead)
	{
		aiCmd.nCmdParam[0] = 8;
		AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
		AIGENERAL_FreeMinionList(pUnit);
		AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_NONE);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (AIGENERAL_GetMinionOwner(pUnit) != pUnit)
	{
		if (pCurrentAiCmd)
		{
			const int32_t nX = pAiTickParam->pAiControl->dwAiParam[0] + pCurrentAiCmd->nCmdParam[1];
			const int32_t nY = pAiTickParam->pAiControl->dwAiParam[1] + pCurrentAiCmd->nCmdParam[2];
			AIGENERAL_FreeCurrentAiCommand(pGame, pUnit);
			AITACTICS_WalkToTargetCoordinates(pGame, pUnit, nX, nY);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		return;
	}

	if (AI_RollPercentage(pUnit) < 30)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	int32_t nParam = pAiTickParam->pAiControl->dwAiParam[2] + 1;
	if (nParam >= 100)
	{
		nParam = 0;
	}

	int32_t nX = CLIENTS_GetUnitX(pUnit);
	int32_t nY = CLIENTS_GetUnitY(pUnit);

	int32_t nOffset = ITEMS_RollRandomNumber(&pUnit->pSeed) % 10;

	if (nParam < 25)
	{
		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nOffset = -nOffset;
		}

		nX += nOffset;
		nY -= 5;
	}
	else if (nParam < 50)
	{
		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nOffset = -nOffset;
		}

		nX += 5;
		nY += nOffset;
	}
	else if (nParam < 75)
	{
		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nOffset = -nOffset;
		}

		nX += nOffset;
		nY += 5;
	}
	else
	{
		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nOffset = -nOffset;
		}

		nX -= 5;
		nY += nOffset;
	}

	if (!AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, nX, nY))
	{
		pAiTickParam->pAiControl->dwAiParam[2] = nParam + 25;
		return;
	}

	aiCmd.nCmdParam[0] = 2;
	aiCmd.nCmdParam[1] = nX;
	aiCmd.nCmdParam[2] = nY;
	AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
	pAiTickParam->pAiControl->dwAiParam[2] = nParam;
}

//D2Game.0x6FCE4FD0
void __fastcall sub_6FCE4FD0(D2UnitStrc* pUnit, void* a2, void* a3)
{
	if (!pUnit || MONSTERS_IsDead(pUnit))
	{
		*(int32_t*)a2 = 1;
	}
}

//D2Game.0x6FCE4FF0
void __fastcall AITHINK_Fn090_Griswold(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AI_RollPercentage(pUnit) < 80)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (AI_RollPercentage(pUnit) < 50)
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 7);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//TODO: find what AIs are using this state.
enum D2C_SpecialState13AIParams
{
	SPECIALSTATE13_AI_PARAM_RUN_CHANCE_PCT = 0,
	SPECIALSTATE13_AI_PARAM_STALL_DURATION = 1,
	SPECIALSTATE13_AI_PARAM_ATTACK_CHANCE_PCT = 2,
};

//D2Game.0x6FCE5080
void __fastcall D2GAME_AI_SpecialState13_6FCE5080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pMonstatsTxt)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (!pAiCmd)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		aiCmd.nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
		pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	}

	if (!pAiCmd->nCmdParam[1] || !pAiCmd->nCmdParam[2])
	{
		pAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		pAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
	}

	D2AiParamStrc* pAiParam = nullptr;
	if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		pAiParam = pUnit->pMonsterData->pAiParam;
	}

	const int32_t v15 = D2Common_10095(pRoom, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]);

	const int32_t nX = CLIENTS_GetUnitY(pUnit);
	const int32_t nY = CLIENTS_GetUnitX(pUnit);

	if (v15 != D2Common_10095(pRoom, nY, nX))
	{
		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 1, 0, 0);

		if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]))
		{
			return;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	}

	const int32_t nTargetX = CLIENTS_GetUnitX(pAiTickParam->pTarget);
	const int32_t nTargetY = CLIENTS_GetUnitY(pAiTickParam->pTarget);

	if (v15 != D2Common_10095(pRoom, nTargetX, nTargetY))
	{
		if (CLIENTS_GetUnitX(pUnit) == pAiCmd->nCmdParam[1] && CLIENTS_GetUnitY(pUnit) == pAiCmd->nCmdParam[2])
		{
			if (pAiTickParam->nTargetDistance >= 25 || !sub_6FCE5520(pGame, pUnit, pAiTickParam))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			}
			return;
		}

		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 1, 0, 0);

		if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]))
		{
			return;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	}

	if (AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) > 40)
	{
		D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(pAiParam, 1, 0, 0);

		if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]))
		{
			return;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	}

	if (sub_6FCE5520(pGame, pUnit, pAiTickParam))
	{
		return;
	}

	if (pAiTickParam->bCombat)
	{
		if (AI_RollPercentage(pUnit) < AI_GetParamValue(pGame, pAiTickParam, SPECIALSTATE13_AI_PARAM_ATTACK_CHANCE_PCT) + 10)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, SPECIALSTATE13_AI_PARAM_RUN_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 0, 100, 0);
			AITACTICS_RunToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, SPECIALSTATE13_AI_PARAM_STALL_DURATION));
}

//D2Game.0x6FCE5520
int32_t __fastcall sub_6FCE5520(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2MapAIStrc* pMapAi = pAiTickParam->pAiControl->pMapAi;

	if (pMapAi && pAiTickParam->pAiControl->dwAiParam[0] < pMapAi->nPathNodes)
	{
		if (pUnit && pUnit->dwClassId >= 0 && pUnit->dwClassId < sgptDataTables->nMonStatsTxtRecordCount)
		{
			D2MonStatsTxt* pMonStatsTxtRecord = &sgptDataTables->pMonStatsTxt[pUnit->dwClassId];
			if (pMonStatsTxtRecord)
			{
				const int32_t nSkillId = pMonStatsTxtRecord->nSkill[0];
				if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
				{
					AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK1, nSkillId, 0, pMapAi->pPosition[pAiTickParam->pAiControl->dwAiParam[0]].nX, pMapAi->pPosition[pAiTickParam->pAiControl->dwAiParam[0]].nY);
					++pAiTickParam->pAiControl->dwAiParam[0];
					pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame;
					return 1;
				}
			}
		}
	}
	else
	{
		const int32_t nFrameDiff = std::abs(pGame->dwGameFrame - pAiTickParam->pAiControl->dwAiParam[1]);
		if (nFrameDiff > 700)
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
		}
	}

	return 0;
}

//D2Game.0x6FCE5610
void __fastcall D2GAME_AI_Unk129_6FCE5610(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;
	pAiTickParam->pAiControl->dwAiParam[1] = 0;
	pAiTickParam->pAiControl->nMinionSpawnClassId = -1;
}

//D2Game.0x6FCE5640
void __fastcall AITHINK_Fn129_GenericSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	if (pAiTickParam->pAiControl->nMinionSpawnClassId == -1)
	{
		pAiTickParam->pAiControl->nMinionSpawnClassId = MONSTER_MINION1;

		D2MonsterRegionStrc* pMonsterRegion = pGame->pMonReg[MONSTER_GetLevelId(pUnit)];

		const int32_t nMonsterId = pMonsterRegion->pMonData[ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pMonsterRegion->nMonCount)].nMonHcIdx;
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);

		if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_GENERICSPAWN])
		{
			pAiTickParam->pAiControl->nMinionSpawnClassId = nMonsterId;
		}
		else
		{
			pAiTickParam->pAiControl->nMinionSpawnClassId = MONSTER_IMP5;

			for (int32_t i = 0; i < pMonsterRegion->nMonCount; ++i)
			{
				D2MonRegDataStrc* pMonRegData = &pMonsterRegion->pMonData[i];
				pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonRegData->nMonHcIdx);

				if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_GENERICSPAWN])
				{
					pAiTickParam->pAiControl->nMinionSpawnClassId = pMonRegData->nMonHcIdx;
					break;
				}
			}
		}
	}

	if (pAiTickParam->nTargetDistance > 20)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] < AI_GetParamValue(pGame, pAiTickParam, 2))
	{
		const int32_t nFrameDiff = std::abs(pGame->dwGameFrame - pAiTickParam->pAiControl->dwAiParam[0]);
		if (nFrameDiff >= AI_GetParamValue(pGame, pAiTickParam, 0))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;

			if (sub_6FC68350(MONSTER_EVILHUT, UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0))
			{
				++pAiTickParam->pAiControl->dwAiParam[1];
				AITACTICS_UseSequenceSkill(pGame, pUnit, SKILL_NEST, pAiTickParam->pTarget, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_NOTC;
	}

	AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
}

//D2Game.0x6FCE58D0
void __fastcall D2GAME_AI_Unk59_6FCE58D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[2] = 0;
}

//D2Game.0x6FCE58E0
void __fastcall AITHINK_Fn059_BloodRaven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nTargetDistance = pAiTickParam->nTargetDistance;

	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (!pCurrentAiCmd)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		aiCmd.nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);

		pCurrentAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	}

	int32_t nDistance = 0;
	if (pCurrentAiCmd)
	{
		nDistance = AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pAiTickParam->pTarget, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]);
		if (nTargetDistance > 45)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		if (nDistance >= 50 || AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pUnit, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]) > 50)
		{
			pAiTickParam->pAiControl->dwAiParam[2] = 1;
			AITACTICS_SetVelocity(pUnit, 7, 100, 0);

			if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]))
			{
				return;
			}

			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
		}

		if (pAiTickParam->pAiControl->dwAiParam[2] && (uint32_t)AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pUnit, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]) > 5)
		{
			AITACTICS_SetVelocity(pUnit, 7, 100, 0);

			if (AITACTICS_RunToTargetCoordinates(pGame, pUnit, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]))
			{
				return;
			}

			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
		}

		pAiTickParam->pAiControl->dwAiParam[2] = 0;
	}

	if (nTargetDistance > 20 && nDistance < 50)
	{
		nTargetDistance = std::max(nTargetDistance / 2, 12);

		AITACTICS_SetVelocity(pUnit, 7, 100, 0);

		if (AITACTICS_RunCloseToTargetUnit(pGame, pUnit, pAiTickParam->pTarget, nTargetDistance))
		{
			return;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	}

	pAiTickParam->pAiControl->dwAiParam[0] += 3;

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && !pAiTickParam->bCombat && pAiTickParam->pAiControl->dwAiParam[1] < 2 * pGame->nDifficulty + 8 && AI_RollPercentage(pUnit) < pAiTickParam->pAiControl->dwAiParam[0])
	{
		const int32_t nMaxLength = ITEMS_RollRandomNumber(&pUnit->pSeed) % 15 + 5;

		int32_t nX = 0;
		int32_t nY = 0;
		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nX = nMaxLength;
			nY = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxLength);
		}
		else
		{
			nX = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxLength);
			nY = nMaxLength;
		}

		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nX = -nX;
		}

		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			nY = -nY;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, nX + CLIENTS_GetUnitX(pAiTickParam->pTarget), nY + CLIENTS_GetUnitY(pAiTickParam->pTarget));

		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		++pAiTickParam->pAiControl->dwAiParam[1];
		return;
	}

	if (nTargetDistance > 5)
	{
		if (AI_RollPercentage(pUnit) < 5 && nDistance < 50)
		{
			AITACTICS_SetVelocity(pUnit, 7, 100, 0);
			AITACTICS_RunCloseToTargetUnit(pGame, pUnit, pAiTickParam->pTarget, 12);
			return;
		}

		int32_t nUnused1 = 0;
		int32_t nUnused2 = 0;
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nUnused1, &nUnused2);
		if (pTarget && !sub_6FCF2E70(pUnit) && AI_RollPercentage(pUnit) < 80)
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && AI_RollPercentage(pUnit) < 10 * (pGame->nDifficulty + 4))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pTarget, 0, 0);
				return;
			}

			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}

		AITACTICS_SetVelocity(pUnit, 0, 50, 0);

		if (sub_6FCD0E80(pGame, pUnit, pAiTickParam->pTarget, 4u, 1))
		{
			return;
		}
	}

	if (AI_RollPercentage(pUnit) < 30 && nTargetDistance < 12)
	{
		AITACTICS_SetVelocity(pUnit, 7, 100, 0);

		if (sub_6FCD06D0(pGame, pUnit, pAiTickParam->pTarget, 12 - nTargetDistance, 0))
		{
			return;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	}

	AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
}

//D2Game.0x6FCE5EE0
uint8_t __fastcall sub_6FCE5EE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	//TODO: v8, v9

	int32_t bCloseToTarget = 0;
	D2UnitStrc* pTarget = sub_6FCCFD70(pGame, pUnit, &bCloseToTarget);
	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget);

	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData || pUnit->pMonsterData->pMonInteract)
	{
		return 0;
	}

	int32_t v8 = MONSTERAI_IsInteractingWith(pUnit->pMonsterData->pMonInteract, pTarget);
	if (pTarget && pTarget->dwUnitType == UNIT_PLAYER && PLAYER_IsBusy(pTarget) == 1)
	{
		v8 = 1;
	}

	const int32_t v9 = MONSTERAI_HasInteractUnit(pUnit->pMonsterData->pMonInteract);

	if (v9 || v8 || pAiTickParam->pAiControl->dwAiParam[0] > 0)
	{
		if (AIUTIL_GetDistanceToCoordinates(pUnit, pAiTickParam->pAiControl->dwAiParam[1], pAiTickParam->pAiControl->dwAiParam[2]) <= 2 || pAiTickParam->pAiControl->dwAiParam[0] <= 36)
		{
			if (pAiTickParam->pAiControl->dwAiParam[0] > 0)
			{
				if (pUnit->pDynamicPath)
				{
					D2Common_10153(pUnit->pDynamicPath);
				}

				AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
			}
		}
		else
		{
			AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pAiTickParam->pAiControl->dwAiParam[1], pAiTickParam->pAiControl->dwAiParam[2]);
		}

		if (pAiTickParam->pAiControl->dwAiParam[0] >= 0)
		{
			--pAiTickParam->pAiControl->dwAiParam[0];
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
		}
	}
	else
	{
		if (!pTarget || pTarget == pUnit)
		{
			return 0;
		}

		if (nDistance <= 2 || nDistance >= 24)
		{
			D2Common_10153(pUnit->pDynamicPath);

			if (pAiTickParam->pAiControl->dwAiParam[1])
			{
				if (pAiTickParam->pAiControl->dwAiParam[1] < 0)
				{
					pAiTickParam->pAiControl->dwAiParam[1] = 0;
				}

				if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
				{
					--pAiTickParam->pAiControl->dwAiParam[1];
				}
			}
			else
			{
				pAiTickParam->pAiControl->dwAiParam[1] = 60;

				if (pTarget->dwUnitType == UNIT_PLAYER)
				{
					SUNIT_AttachSound(pUnit, 18, pTarget);
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
					return 1;
				}
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		}
		else
		{
			D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
			if (!pAiCmd)
			{
				D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
				if (pCurrentAiCmd && !pCurrentAiCmd->nCmdParam[1] && !pCurrentAiCmd->nCmdParam[2])
				{
					pCurrentAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
					pCurrentAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
					return 1;
				}

				AITACTICS_SetVelocity(pUnit, 1, 0, 0);

				if (nDistance < 5)
				{
					AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pTarget, nDistance - 2, 2);
				}
				else
				{
					AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pTarget, 3, 2);
				}
				return 1;
			}

			if (AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) <= 16)
			{
				AITACTICS_SetVelocity(pUnit, 1, 0, 0);

				if (nDistance < 5)
				{
					AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pTarget, nDistance - 2, 2);
				}
				else
				{
					AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pTarget, 3, 2);
				}
				return 1;
			}

			D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
			if (pCurrentAiCmd)
			{
				pCurrentAiCmd->nCmdParam[1] = pAiCmd->nCmdParam[1];
				pCurrentAiCmd->nCmdParam[2] = pAiCmd->nCmdParam[2];
				pCurrentAiCmd->nCmdParam[3] = 12;
				pCurrentAiCmd->nCmdParam[4] = 10;
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
	}

	return 1;
}

//D2Game.0x6FCE61E0
int32_t __fastcall sub_6FCE61E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition)
{
	if (!pUnit || !pPosition || pPosition->nX == 0 || pPosition->nY == 0 || !AIUTIL_GetDistanceToCoordinates(pUnit, pPosition->nX, pPosition->nY))
	{
		return 0;
	}

	AITACTICS_SetVelocity(pUnit, 7, 0, 0);
	AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pPosition->nX, pPosition->nY);

	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
	if (pAiCmd)
	{
		pAiCmd->nCmdParam[1] = pPosition->nX;
		pAiCmd->nCmdParam[2] = pPosition->nY;
		pAiCmd->nCmdParam[3] = 12;
		pAiCmd->nCmdParam[4] = 10;
	}

	return 1;
}

//D2Game.0x6FCE6270
int32_t __fastcall sub_6FCE6270(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition)
{
	if (!pUnit || !pPosition || pPosition->nX == 0 || pPosition->nY == 0)
	{
		return 0;
	}

	int32_t bResult = 0;
	if (AIUTIL_GetDistanceToCoordinates(pUnit, pPosition->nX, pPosition->nY))
	{
		AITACTICS_SetVelocity(pUnit, 7, 0, 0);
		AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pPosition->nX, pPosition->nY);

		D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
		if (pAiCmd)
		{
			pAiCmd->nCmdParam[1] = pPosition->nX;
			pAiCmd->nCmdParam[2] = pPosition->nY;
			pAiCmd->nCmdParam[3] = 12;
			pAiCmd->nCmdParam[4] = 10;
		}

		bResult = 1;
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
	if (pAiCmd)
	{
		pAiCmd->nCmdParam[1] = pPosition->nX;
		pAiCmd->nCmdParam[2] = pPosition->nY;
		pAiCmd->nCmdParam[3] = 20;
		pAiCmd->nCmdParam[4] = 10;
	}

	return bResult;
}

//D2Game.0x6FCE6340
int32_t __fastcall sub_6FCE6340(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition)
{
	if (!pUnit || !pPosition || !pPosition->nX || !pPosition->nY)
	{
		return 0;
	}

	int32_t bResult = 0;
	if (AIUTIL_GetDistanceToCoordinates(pUnit, pPosition->nX, pPosition->nY))
	{
		AITACTICS_SetVelocity(pUnit, 7, 0, 0);
		AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pPosition->nX, pPosition->nY);

		D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
		if (pAiCmd)
		{
			pAiCmd->nCmdParam[1] = pPosition->nX;
			pAiCmd->nCmdParam[2] = pPosition->nY;
			pAiCmd->nCmdParam[3] = 12;
			pAiCmd->nCmdParam[4] = 10;
		}

		bResult = 1;
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 7, 0);
	if (pAiCmd)
	{
		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_SKILL1])
		{
			pAiCmd->nCmdParam[1] = 8;
		}
		else
		{
			FOG_Trace("NPC: %d tried to skill1", pUnit->dwClassId);
			pAiCmd->nCmdParam[1] = 1;
		}

		pAiCmd->nCmdParam[2] = pPosition->nX;
		pAiCmd->nCmdParam[3] = pPosition->nY;
		pAiCmd->nCmdParam[4] = 4;
	}

	pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
	if (pAiCmd)
	{
		pAiCmd->nCmdParam[1] = pPosition->nX;
		pAiCmd->nCmdParam[2] = pPosition->nY;
		pAiCmd->nCmdParam[3] = 12;
		pAiCmd->nCmdParam[4] = 10;
	}

	return bResult;
}

//D2Game.0x6FCE64D0
int32_t __fastcall sub_6FCE64D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition)
{
	if (!pUnit || !pPosition || !pPosition->nX || !pPosition->nY)
	{
		return 0;
	}

	int32_t bResult = 0;
	if (AIUTIL_GetDistanceToCoordinates(pUnit, pPosition->nX, pPosition->nY))
	{
		AITACTICS_SetVelocity(pUnit, 7, 0, 0);
		AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pPosition->nX, pPosition->nY);

		D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
		if (pAiCmd)
		{
			pAiCmd->nCmdParam[1] = pPosition->nX;
			pAiCmd->nCmdParam[2] = pPosition->nY;
			pAiCmd->nCmdParam[3] = 12;
			pAiCmd->nCmdParam[4] = 10;
		}

		bResult = 1;
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 7, 0);
	if (pAiCmd)
	{
		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_SKILL2])
		{
			pAiCmd->nCmdParam[1] = 9;
		}
		else
		{
			FOG_Trace("NPC: %d tried to skill1", pUnit->dwClassId);
			pAiCmd->nCmdParam[1] = 1;
		}

		pAiCmd->nCmdParam[2] = pPosition->nX;
		pAiCmd->nCmdParam[3] = pPosition->nY;
		pAiCmd->nCmdParam[4] = 4;
	}

	pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 4, 0);
	if (pAiCmd)
	{
		pAiCmd->nCmdParam[1] = pPosition->nX;
		pAiCmd->nCmdParam[2] = pPosition->nY;
		pAiCmd->nCmdParam[3] = 12;
		pAiCmd->nCmdParam[4] = 10;
	}

	return bResult;
}

//D2Game.0x6FCE6660
void __fastcall AITHINK_Fn032_Npc(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
	if (pAiCmd && !pAiCmd->nCmdParam[1] && !pAiCmd->nCmdParam[2])
	{
		pAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		pAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	int32_t nClassId = -1;
	if (pUnit)
	{
		nClassId = pUnit->dwClassId;
	}

	switch (nClassId)
	{
	case MONSTER_ORMUS:
	{
		D2CoordStrc coord = {};
		if (ACT3Q3_GetAltarCoordinates(pGame, &coord))
		{
			if (AIUTIL_GetDistanceToCoordinates(pUnit, coord.nX, coord.nY) <= 3)
			{
				AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 8, 0, 0);
				ACT3Q3_SetAltarMode(pGame);
			}
			else
			{
				AITACTICS_WalkToTargetCoordinates(pGame, pUnit, coord.nX, coord.nY);
			}
			return;
		}
		break;
	}
	case MONSTER_JERHYN:
	{
		if (!ACT2Q4_IsJerhynPalaceActivated(pGame))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
			return;
		}

		// TODO: Names
		int32_t a3 = 0;
		int32_t a4 = 0;
		ACT2Q4_GetAndUpdatePalaceNpcState(pGame, pUnit, &a3, &a4);

		if (a4)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		}

		if (!a3)
		{
			return;
		}

		if (ACT2Q4_IsGuardMoving(pGame, pUnit))
		{
			pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
			if (pAiCmd)
			{
				pAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit) + 9;
				pAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
				return;
			}
		}
		break;
	}
	case MONSTER_ALKOR:
	{
		if (ACT3Q4_GoldenBirdBroughtToAlkor(pGame))
		{
			AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_SKILL1, 0, 0);
			ACT3Q4_ResetAlkor(pGame);
			return;
		}
		break;
	}
	case MONSTER_CAIN5:
	{
		D2CoordStrc coord = {};
		if (ACT1Q4_GetCainPortalInTownCoordinates(pGame, pUnit, &coord))
		{
			if (AIUTIL_GetDistanceToCoordinates(pUnit, coord.nX, coord.nY) > 2)
			{
				AITACTICS_WalkToTargetCoordinates(pGame, pUnit, coord.nX, coord.nY);
				return;
			}

			ACT1Q4_OnCainInTownActivated(pGame);
		}
		break;
	}
	case MONSTER_DREHYA:
	{
		ACT5Q4_AnyaOpenPortal(pGame, pUnit);
		break;
	}
	default:
		break;
	}

	if (!sub_6FCE5EE0(pGame, pUnit, pAiTickParam) && !sub_6FCE69A0(pGame, pUnit, pAiTickParam) && !AITHINK_ExecuteMapAiAction(pGame, pUnit, pAiTickParam))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
	}
}

//D2Game.0x6FCE69A0
uint8_t __fastcall sub_6FCE69A0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	//TODO: v18, dword_6FD4DD88
	int32_t dword_6FD4DD88 = 0;

	int32_t nAnimMode = 0;
	int32_t v18 = 0;

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 4, 0);
	if (pAiCmd && pAiCmd->nCmdParam[3] > 0)
	{
		if (pAiCmd->nCmdParam[1] && pAiCmd->nCmdParam[2] && (uint32_t)AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) > 3)
		{
			if ((dword_6FD4DD88 % 4) == 0)
			{
				AITACTICS_SetVelocity(pUnit, 5, 0, 0);
				AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]);
			}
			else
			{
				AITACTICS_SetVelocity(pUnit, 1, 0, 0);
				AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]);
			}

			++dword_6FD4DD88;
			--pAiCmd->nCmdParam[3];
			return 1;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiCmd->nCmdParam[4]);
		--pAiCmd->nCmdParam[3];
		return 1;
	}

	pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 5, 0);
	if (pAiCmd)
	{
		if (pAiCmd->nCmdParam[1] <= 0)
		{
			if (pAiCmd->nCmdParam[3] > 0)
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiCmd->nCmdParam[4]);
				--pAiCmd->nCmdParam[3];
				return 1;
			}
		}
		else
		{
			if ((pAiCmd->nCmdParam[1] % 2) != 0)
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, pAiCmd->nCmdParam[2]);
				--pAiCmd->nCmdParam[1];
				return 1;
			}

			if (pAiCmd->nCmdParam[3] > 0)
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, pAiCmd->nCmdParam[4]);
				--pAiCmd->nCmdParam[1];
				--pAiCmd->nCmdParam[3];
				return 1;
			}

			--pAiCmd->nCmdParam[1];
		}
	}

	pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 7, 0);
	if (!pAiCmd || !pAiCmd->nCmdParam[1])
	{
		return 0;
	}

	if (pAiCmd->nCmdParam[1] != 8 && pAiCmd->nCmdParam[1] != 9 && pAiCmd->nCmdParam[1] != 10 && pAiCmd->nCmdParam[1] != 11)
	{
		pAiCmd->nCmdParam[1] = 0;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
		return 1;
	}

	v18 = AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[2], pAiCmd->nCmdParam[3]);

	if (v18 <= 0 || pAiCmd->nCmdParam[4] <= 0)
	{
		if (v18 > 1)
		{
			pAiCmd->nCmdParam[1] = 0;
		}

		if (pUnit)
		{
			if (pUnit->dwClassId == MONSTER_CHARSI)
			{
				D2COMMON_10160_PathUpdateDirection(pUnit->pDynamicPath, 0x38u);
			}
			else if (pUnit->dwClassId == MONSTER_WARRIV1)
			{
				D2COMMON_10160_PathUpdateDirection(pUnit->pDynamicPath, 0x34u);
			}
			else if (pUnit->dwClassId == MONSTER_FARA)
			{
				D2COMMON_10160_PathUpdateDirection(pUnit->pDynamicPath, 4u);
			}
			else if (pUnit->dwClassId == MONSTER_JAMELLA)
			{
				if (pAiCmd->nCmdParam[1] == 8)
				{
					D2COMMON_10160_PathUpdateDirection(pUnit->pDynamicPath, 0x34u);
				}
				else if (pAiCmd->nCmdParam[1] == 9)
				{
					D2COMMON_10160_PathUpdateDirection(pUnit->pDynamicPath, 0x30u);
				}
			}
			else if (pUnit->dwClassId == MONSTER_LARZUK)
			{
				if (AI_RollPercentage(pUnit) > 4)
				{
					pAiCmd->nCmdParam[1] = 0;
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
					return 1;
				}
			}
		}

		if (pAiCmd->nCmdParam[1])
		{
			if (!pUnit)
			{
				nAnimMode = 0;
			}
			else
			{
				nAnimMode = pUnit->dwAnimMode;
			}

			if (nAnimMode == pAiCmd->nCmdParam[1])
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, pAiCmd->nCmdParam[1], pUnit);
				pAiCmd->nCmdParam[1] = 0;
			}
			return 1;
		}

		if (pUnit && pUnit->dwClassId == MONSTER_FARA && AI_RollPercentage(pUnit) < 66)
		{
			pAiCmd->nCmdParam[1] = 8;
		}

		return 0;
	}

	AITACTICS_SetVelocity(pUnit, 7, 0, 0);

	if (!AITACTICS_WalkToTargetCoordinatesNoSteps(pGame, pUnit, pAiCmd->nCmdParam[2], pAiCmd->nCmdParam[3]))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
	}

	--pAiCmd->nCmdParam[4];
	return 1;
}

//D2Game.0x6FCE6CD0
int32_t __fastcall AITHINK_ExecuteMapAiAction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	typedef int32_t(__fastcall* ACTIONFN)(D2GameStrc*, D2UnitStrc*, D2AiTickParamStrc*, D2MapAIPathPositionStrc*);

	//TODO: Name
	ACTIONFN dword_6FD3F970[] =
	{
		nullptr,
		sub_6FCE61E0,
		sub_6FCE6270,
		sub_6FCE61E0,
		sub_6FCE6340,
		sub_6FCE64D0,
		nullptr
	};

	D2MapAIStrc* pMapAi = pAiTickParam->pAiControl->pMapAi;
	if (!pMapAi || AI_RollPercentage(pUnit) >= 66 || !pMapAi->nPathNodes)
	{
		return 0;
	}

	D2MapAIPathPositionStrc* pPosition = &pMapAi->pPosition[ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pMapAi->nPathNodes)];
	if (pPosition->nMapAIAction < 0 || pPosition->nMapAIAction >= 7)
	{
		return 0;
	}

	ACTIONFN pfAction = dword_6FD3F970[pPosition->nMapAIAction];
	if (pfAction)
	{
		return pfAction(pGame, pUnit, pAiTickParam, pPosition);
	}

	return 0;
}

//D2Game.0x6FCE6DC0
void __fastcall AITHINK_Fn054_NpcStationary(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t bCloseToTarget = 0;
	D2UnitStrc* pPlayer = sub_6FCCFD70(pGame, pUnit, &bCloseToTarget);

	D2MonsterInteractStrc* pMonInteract = nullptr;
	if (pPlayer && pPlayer != pUnit)
	{
		const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPlayer);
		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pMonInteract = pUnit->pMonsterData->pMonInteract;
		}

		if (pPlayer->dwUnitType == UNIT_PLAYER && PLAYER_IsBusy(pPlayer) == 1 || MONSTERAI_HasInteractUnit(pMonInteract))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
			return;
		}

		int32_t nParam = pAiTickParam->pAiControl->dwAiParam[1];
		if (nDistance >= 24)
		{
			if (nParam <= 0)
			{
				pAiTickParam->pAiControl->dwAiParam[1] = 60;
			}

			nParam = pAiTickParam->pAiControl->dwAiParam[1];
		}
		else
		{
			if (!nParam)
			{
				pAiTickParam->pAiControl->dwAiParam[1] = 60;

				if (pPlayer->dwUnitType == UNIT_PLAYER)
				{
					SUNIT_AttachSound(pUnit, 18, pPlayer);
				}

				AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
				return;
			}
		}

		pAiTickParam->pAiControl->dwAiParam[1] = nParam - 1;
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	if (!pUnit)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	if (pUnit->dwClassId == MONSTER_TYRAEL1)
	{
		if (!ACT2Q6_IsInRangeOfTyrael(pGame, pUnit) || MONSTERAI_HasInteractUnit(pMonInteract))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
			return;
		}

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		ACT2Q6_AttachCompletionSoundToPlayers(pGame);
	}
	else if (pUnit->dwClassId == MONSTER_IZUALGHOST)
	{
		if (!ACT4Q1_IsAnyPlayerInRangeOfIzualGhost(pGame, pUnit) || MONSTERAI_HasInteractUnit(pMonInteract))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
			return;
		}

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		ACT4Q1_AttachCompletionSoundToPlayers(pGame);
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
	}
}

//D2Game.0x6FCE6F80
void __fastcall AITHINK_Fn041_Towner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);

	if (pAiCmd && !pAiCmd->nCmdParam[1] && !pAiCmd->nCmdParam[2])
	{
		pAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		pAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	if (!sub_6FCE69A0(pGame, pUnit, pAiTickParam) && !AITHINK_ExecuteMapAiAction(pGame, pUnit, pAiTickParam))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 12);
	}
}

//D2Game.0x6FCE7070
void __fastcall AITHINK_Fn081_JarJar(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
	if (pAiCmd && !pAiCmd->nCmdParam[1] && !pAiCmd->nCmdParam[2])
	{
		pAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		pAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (!ACT2Q4_HasGuardMovedToEndPosition(pGame))
	{
		D2CoordStrc coords = {}; 
		coords.nX = pAiCmd->nCmdParam[1];
		coords.nY = pAiCmd->nCmdParam[2];

		int32_t bIdle = 0;
		if (ACT2Q4_GetGuardCoordinates(pGame, &coords))
		{
			bIdle = 1;
		}

		if (pAiCmd->nCmdParam[4] >= pGame->dwGameFrame || (pGame->dwGameFrame - pAiCmd->nCmdParam[4]) >= 200)
		{
			if (AIUTIL_GetDistanceToCoordinates(pUnit, coords.nX, coords.nY) <= 1)
			{
				if (bIdle)
				{
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
				}
				else
				{
					if (sub_6FCE5EE0(pGame, pUnit, pAiTickParam))
					{
						pAiCmd->nCmdParam[4] = pGame->dwGameFrame;
					}
					else
					{
						AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
					}
				}
			}
			else
			{
				AITACTICS_WalkToTargetCoordinates(pGame, pUnit, coords.nX, coords.nY);
				pAiCmd->nCmdParam[4] = pGame->dwGameFrame;
			}
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		}
		return;
	}

	if (!pAiCmd)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 120);
		return;
	}

	int32_t bCloseToTarget = 0;
	D2UnitStrc* pOwner = sub_6FCCFD70(pGame, pUnit, &bCloseToTarget);
	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);
	if (ACT2Q4_IsHaremBlockerObjectOpened(pGame) && AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2] - 3) > 2)
	{
		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2] - 3);
		return;
	}

	if (AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2] - 3) > 7)
	{
		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2] - 3);
		return;
	}

	if (sub_6FCE5EE0(pGame, pUnit, pAiTickParam) || !pOwner || pOwner == pUnit)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 120);
		return;
	}

	if (pOwner->dwUnitType != UNIT_PLAYER)
	{
		if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 1000) < 50)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 120);
		return;
	}

	if (nDistance >= 12)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);

		if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 1000) < 50)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 120);
		return;
	}

	if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 1000) < 100)
	{
		D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, pOwner, 2u);
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 200);
}

//D2Game.0x6FCE73A0
void __fastcall AITHINK_Fn031_NpcOutOfTown(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t bDrehyaIced = 0;
	if (pUnit && pUnit->dwClassId == MONSTER_DREHYAICED)
	{
		bDrehyaIced = 1;
	}

	if (AITHINK_SpawnNpcPortal(pGame, pUnit, pAiTickParam))
	{
		return;
	}

	if (bDrehyaIced)
	{
		sub_6FCB5430(pGame);
	}

	D2MonsterInteractStrc* pMonInteract = nullptr;
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		pMonInteract = pUnit->pMonsterData->pMonInteract;
	}

	if (MONSTERAI_HasInteractUnit(pMonInteract))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
		return;
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 3, 0);
	if (pUnit && pUnit->dwAnimMode == MONMODE_DEAD)
	{
		return;
	}

	sub_6FCE5EE0(pGame, pUnit, pAiTickParam);

	if (pAiCmd)
	{
		if (pAiCmd->nCmdParam[3] < 2)
		{
			if (AIUTIL_GetDistanceToCoordinates(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) > 1)
			{
				if (pAiCmd->nCmdParam[4] <= 5)
				{
					++pAiCmd->nCmdParam[4];

					if (bDrehyaIced && sub_6FCB5400(pGame))
					{
						AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
						pAiCmd->nCmdParam[4] = 0;
					}
					else
					{
						AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]);
					}
					return;
				}
			}

			if (pAiCmd->nCmdParam[3] == 1)
			{
				int32_t(__fastcall* pfSpawnPortalOutsideTown)(D2GameStrc*, D2UnitStrc*) = nullptr;
				if (bDrehyaIced)
				{
					pfSpawnPortalOutsideTown = ACT5Q3_SpawnDrehyaPortalOutsideTown;
				}
				else
				{
					pfSpawnPortalOutsideTown = ACT1Q4_SpawnCainPortalOutsideTown;
				}

				if (!pfSpawnPortalOutsideTown(pGame, pUnit))
				{
					pAiCmd->nCmdParam[3] = 1;
					pAiCmd->nCmdParam[4] = 1;
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
					return;
				}

				pAiCmd->nCmdParam[3] = 2;
			}
		}
		else
		{
			++pAiCmd->nCmdParam[3];

			int32_t(__fastcall* pfGetCoordinates)(D2GameStrc*, D2UnitStrc*, D2CoordStrc*) = nullptr;
			void(__fastcall* pfSpawnPortalInTown)(D2GameStrc*) = nullptr;
			if (bDrehyaIced)
			{
				pfGetCoordinates = ACT5Q3_GetDrehyaPortalCoordinates;
				pfSpawnPortalInTown = sub_6FCB53D0;
			}
			else
			{
				pfGetCoordinates = ACT1Q4_GetCainPortalOutsideTownCoordinates;
				pfSpawnPortalInTown = ACT1Q4_SpawnCainPortalInTown;
			}

			D2CoordStrc coord = {};
			if (pfGetCoordinates(pGame, pUnit, &coord))
			{
				if (pAiCmd->nCmdParam[3] < 8 && AIUTIL_GetDistanceToCoordinates(pUnit, coord.nX, coord.nY))
				{
					AITACTICS_WalkToTargetCoordinates(pGame, pUnit, coord.nX, coord.nY);
				}
				else
				{
					pfSpawnPortalInTown(pGame);
					STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, 0, 0);
					AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_DEAD, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
				}
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	AITHINK_SpawnNpcPortal(pGame, pUnit, pAiTickParam);
	AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
}

//D2Game.0x6FCE7640
int32_t __fastcall AITHINK_SpawnNpcPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 3, 0);

	if (!pCurrentAiCmd || pCurrentAiCmd->nCmdParam[1] || pCurrentAiCmd->nCmdParam[2])
	{
		return 0;
	}

	pCurrentAiCmd->nCmdParam[1] = CLIENTS_GetUnitX(pUnit) + 3;
	pCurrentAiCmd->nCmdParam[2] = CLIENTS_GetUnitY(pUnit) + 3;

	int32_t(__fastcall* pfUpdatePortalCoordinates)(D2GameStrc*, D2UnitStrc*) = nullptr;
	void(__fastcall* pfSpawnPortal)(D2GameStrc*) = nullptr;
	if (pUnit->dwClassId == MONSTER_DREHYAICED)
	{
		pfUpdatePortalCoordinates = ACT5Q3_InitializeDrehyaPortalCoordinates;
		pfSpawnPortal = sub_6FCB53D0;
	}
	else
	{
		pfUpdatePortalCoordinates = ACT1Q4_UpdateCainPortalCoordinates;
		pfSpawnPortal = ACT1Q4_SpawnCainPortalInTown;
	}

	if (!pfUpdatePortalCoordinates(pGame, pUnit))
	{
		pfSpawnPortal(pGame);

		STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, 0, 0);

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_DEAD, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
	}

	pCurrentAiCmd->nCmdParam[3] = 1;
	pCurrentAiCmd->nCmdParam[4] = 0;
	AITACTICS_IdleInNeutralMode(pGame, pUnit, 1);

	return 1;
}

//D2Game.0x6FCE77A0
void __fastcall AITHINK_Fn060_GoodNpcRanged(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pUnit || pUnit->dwAnimMode != MONMODE_NEUTRAL)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return;
	}

	// TODO: Names
	int32_t a3 = 0;
	int32_t a4 = 0;


	if (!DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
	{
		D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &a3, &a4);
		if (pTarget && a3 < 20)
		{
			if (AI_RollPercentage(pUnit) < 30)
			{
				if (pUnit->dwClassId == MONSTER_ROGUEHIRE)
				{
					AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
				}
				else
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
				}
			}
			else
			{
				if (AI_RollPercentage(pUnit) < 30)
				{
					sub_6FCD0E80(pGame, pUnit, pTarget, 4u, 0);
				}
				else
				{
					AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
				}
			}
			return;
		}
	}

	if (AI_RollPercentage(pUnit) < 20)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE7930
void __fastcall D2GAME_AI_SpecialState06_6FCE7930(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pUnit || pUnit->dwAnimMode != MONMODE_NEUTRAL)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (DUNGEON_IsRoomInTown(pRoom))
	{
		if (AI_RollPercentage(pUnit) >= 20)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
		}
		return;
	}

	int32_t nDistance = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget = sub_6FCF2110(pGame, pUnit, pAiTickParam->pAiControl, &nDistance, &bCombat);
	if (!pTarget)
	{
		if (AI_RollPercentage(pUnit) >= 20)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
		}
		return;
	}

	if (bCombat)
	{
		if (AI_RollPercentage(pUnit) < 80)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			return;
		}
	}
	else
	{
		if (AI_RollPercentage(pUnit) < 30)
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 7);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE7A60
void __fastcall AITHINK_Fn062_TownRogue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t bCombat = pAiTickParam->bCombat;
	int32_t nDistance = pAiTickParam->nTargetDistance;

	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, &bCombat);
	if (pTarget && nDistance < 25)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
	}
}

//D2Game.0x6FCE7AC0
void __fastcall AITHINK_Fn058_Navi(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2MonsterInteractStrc* pMonInteract = nullptr;
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		pMonInteract = pUnit->pMonsterData->pMonInteract;
	}

	if (MONSTERAI_HasInteractUnit(pMonInteract))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	int32_t bCloseToTarget = 0;
	D2UnitStrc* pPlayer = sub_6FCCFD70(pGame, pUnit, &bCloseToTarget);
	if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && !PLAYER_IsBusy(pPlayer) && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 3) && bCloseToTarget)
	{
		if (pAiTickParam->pAiControl->dwAiParam[1])
		{
			if (pAiTickParam->pAiControl->dwAiParam[1] < 0)
			{
				pAiTickParam->pAiControl->dwAiParam[1] = 0;
			}

			if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
			{
				--pAiTickParam->pAiControl->dwAiParam[1];
			}
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[1] = 60;

			if (pPlayer->dwUnitType == UNIT_PLAYER)
			{
				SUNIT_AttachSound(pUnit, 18, pPlayer);
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
		return;
	}

	int32_t bInMeleeRange = 0;
	int32_t nDistance = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, &bInMeleeRange);
	if (pTarget && nDistance < 25)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 50);
}

//D2Game.0x6FCE7C10
void __fastcall AITHINK_Fn001_100_Idle_Buffy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (UNITS_GetRoom(pUnit))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 200);
	}
}

//D2Game.0x6FCE7C40
void __fastcall D2GAME_AI_SpecialState09_6FCE7C40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (pCurrentAiCmd && pCurrentAiCmd->nCmdParam[0] == 8)
	{
		AIGENERAL_FreeMinionList(pUnit);
		AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_NONE);
	}
	else
	{
		int32_t nUnused = 0;
		AIGENERAL_ExecuteCallbackOnMinions(pUnit, &nUnused, nullptr, sub_6FCE4FD0);

		if (sub_6FCF2E70(pUnit) || pAiTickParam || pAiTickParam->nTargetDistance < 20)
		{
			D2AiCmdStrc aiCmd = {};
			aiCmd.nCmdParam[0] = 8;
			AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);
			AIGENERAL_FreeMinionList(pUnit);
			AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_NONE);
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE7CF0
void __fastcall D2GAME_AI_SpecialState10_17_6FCE7CF0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);

	if (pAiTickParam->bCombat && pUnit->dwClassId >= 0 && (!pMonStatsTxtRecord || !(gdwBitMasks[MONSTATSFLAGINDEX_INTERACT] & pMonStatsTxtRecord->dwMonStatsFlags)) && pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_ATTACK1])
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	if (AI_RollPercentage(pUnit) < 20)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 3u);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE7E20
void __fastcall D2GAME_AI_SpecialState11_6FCE7E20(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
	if (!pTarget)
	{
		return;
	}

	if (pTarget->dwUnitType == UNIT_PLAYER)
	{
		sub_6FC61E30(pUnit, 1, pTarget->dwUnitId);
	}
	else if (pTarget->dwUnitType == UNIT_MONSTER)
	{
		sub_6FC61E30(pUnit, 2, pTarget->dwUnitId);
	}
	else if (pTarget->dwUnitType == UNIT_MISSILE)
	{
		sub_6FC61E30(pUnit, 4, pTarget->dwUnitId);
	}
}

//D2Game.0x6FCE7E80
void __fastcall D2GAME_AI_SpecialState11_6FCE7E80(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!STATES_CheckState(pUnit, STATE_TERROR))
	{
		AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, AISPECIALSTATE_NONE);
		AITACTICS_Idle(pGame, pUnit, 1);
		return;
	}

	int32_t nParam = pAiTickParam->pAiControl->dwAiParam[0];
	if (!nParam)
	{
		nParam = 30;
	}

	if (pAiTickParam->nTargetDistance > nParam)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	int32_t nVel = 0;
	if (pAiTickParam->pMonstatsTxt->nVelocity > 0)
	{
		const int32_t nRatio = 100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity;
		if (nRatio >= 100)
		{
			nVel = std::min(nRatio - 100, 120);
		}
	}

	if (pAiTickParam->pAiControl->dwAiParam[2])
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);

		if (pAiTickParam->bCombat && pUnit->dwClassId >= 0 && (!pMonStatsTxtRecord || !(gdwBitMasks[MONSTATSFLAGINDEX_INTERACT] & pMonStatsTxtRecord->dwMonStatsFlags)) && pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_ATTACK1])
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_SetVelocity(pUnit, 2, nVel, 0);

			pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
			int32_t bResult = 0;
			if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_RUN])
			{
				bResult = sub_6FCD06D0(pGame, pUnit, pAiTickParam->pTarget, 30, 1);
			}
			else
			{
				bResult = D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 30, 1);
			}

			if (!bResult)
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
			}
		}
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = 1;
	sub_6FC61F00(pUnit);
	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	AITACTICS_SetVelocity(pUnit, 2, nVel, 0);

	D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
	if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_RUN])
	{
		sub_6FCD06D0(pGame, pUnit, pAiTickParam->pTarget, 30, 0);
		return;
	}

	D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 30, 0);
}

//D2Game.0x6FCE81B0
void __fastcall D2GAME_AI_SpecialState12_6FCE81B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
	if (!pTarget || DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)))
	{
		AITHINK_ExecuteAiFn(pGame, pUnit, AIGENERAL_GetAiControlFromUnit(pUnit), AISPECIALSTATE_NONE);
		AITACTICS_Idle(pGame, pUnit, 1);
		return;
	}

	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		sub_6FCD0410(pGame, pUnit, pTarget, 7);
		return;
	}

	int32_t bCombat = UNITS_IsInMeleeRange(pUnit, pTarget, 0);
	if (bCombat)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		return;
	}

	int32_t nDistance = 0;
	D2UnitStrc* pTargetUnit = sub_6FCCF9D0(pGame, pUnit, pAiTickParam->pAiControl, &nDistance, &bCombat);
	if (pTargetUnit && bCombat)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTargetUnit);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[0] = 1;
	UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
	sub_6FCD0410(pGame, pUnit, pTarget, 7);
}

//D2Game.0x6FCE82F0
void __fastcall D2GAME_AI_Unk051_6FCE82F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	int32_t nParam1 = 0;
	int32_t nParam2 = 0;
	if (pCurrentAiCmd)
	{
		nParam1 = pCurrentAiCmd->nCmdParam[1];
		nParam2 = pCurrentAiCmd->nCmdParam[2];
	}

	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, pAiTickParam->pAiControl->nAiSpecialState);

	if (!AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0) && nParam1 && nParam2)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = nParam1;
		aiCmd.nCmdParam[2] = nParam2;
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 1);
}

//D2Game.0x6FCE83A0
int32_t __fastcall AITHINK_GetTargetScore(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2AiCmdStrc* pAiCmd)
{
	const int32_t bInRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);

	int32_t nBaseValue = 0;
	if (pAiCmd && AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) > 85 && AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pTarget, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) < 85)
	{
		nBaseValue = 100;
	}

	int32_t nColdResist = 0;
	if (bInRange)
	{
		nColdResist = STATLIST_UnitGetStatValue(pTarget, STAT_COLDRESIST, 0);
	}

	const int32_t nResistValue = (STATLIST_UnitGetStatValue(pTarget, STAT_FIRERESIST, 0) + STATLIST_UnitGetStatValue(pTarget, STAT_LIGHTRESIST, 0)
					+ nColdResist + 4 * (STATLIST_UnitGetStatValue(pTarget, STAT_DAMAGERESIST, 0) + 2 * STATLIST_UnitGetStatValue(pTarget, STAT_MAGICRESIST, 0))) / 15;

	const int32_t nDamageValue = (STATLIST_UnitGetStatValue(pTarget, STAT_MAXDAMAGE, 0) + STATLIST_UnitGetStatValue(pTarget, STAT_FIREMAXDAM, 0)
					+ STATLIST_UnitGetStatValue(pTarget, STAT_LIGHTMAXDAM, 0) + STATLIST_UnitGetStatValue(pTarget, STAT_MAGICMAXDAM, 0)
					+ STATLIST_UnitGetStatValue(pTarget, STAT_COLDMAXDAM, 0) + (STATLIST_UnitGetStatValue(pTarget, STAT_POISONMAXDAM, 0) >> 8)) / 2;

	const int32_t nColdStateValue = STATES_CheckState(pTarget, STATE_COLD) != 0 ? 100 : 0;

	const int32_t nLifePercentageValue = (UNITS_GetCurrentLifePercentage(pTarget) < 20) ? 100 : 0;

	D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pTarget);
	int32_t nLeftSkillLevel = 0;
	uint8_t nLeftAttackRank = 0;
	if (pLeftSkill)
	{
		nLeftSkillLevel = SKILLS_GetSkillLevel(pTarget, pLeftSkill, 1);
		nLeftAttackRank = SKILLS_GetSkillsTxtRecord(SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__))->nAttackRank;
	}

	D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pTarget);
	int32_t nRightSkillLevel = 0;
	uint8_t nRightAttackRank = 0;
	if (pRightSkill)
	{
		nRightSkillLevel = SKILLS_GetSkillLevel(pTarget, pRightSkill, 1);
		nRightAttackRank = SKILLS_GetSkillsTxtRecord(SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__))->nAttackRank;
	}

	const int32_t nSkillValue = (nLeftSkillLevel * nLeftAttackRank + nRightSkillLevel * nRightAttackRank) / 4;
	int32_t nRangeValue = 0;
	if (bInRange)
	{
		nRangeValue = 100;
	}
	else if (!UNITS_TestCollisionWithUnit(pUnit, pTarget, COLLIDE_MISSILE_BARRIER))
	{
		nRangeValue = 75;
	}

	if (pTarget && pTarget->dwUnitType == UNIT_MONSTER)
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
		if (!pMonStatsTxtRecord)
		{
			return 0;
		}

		if (pMonStatsTxtRecord->nThreat <= 1u)
		{
			return 0;
		}
	}

	const int32_t nValue = (nResistValue + 4 * (nRangeValue + nBaseValue) + nRangeValue + nBaseValue + 2 * (nDamageValue + nSkillValue + 2 * nColdStateValue) + 3 * nLifePercentageValue) / 22;
	if (!nValue)
	{
		return 1;
	}

	return nValue;
}

//D2Game.0x6FCE86C0
D2UnitStrc* __fastcall AITHINK_GetTargetForBoss(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* a3, int32_t* pCounter, D2AiCmdStrc* pAiCmd, int32_t(__fastcall* pfCull)(D2UnitStrc*, D2UnitStrc*))
{
	D2_ASSERT(pfCull);

	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	D2TargetNodeStrc* pTargetNode = nullptr;
	D2UnitStrc* pTarget = nullptr;
	int32_t nCurrent = 0;
	int32_t nMax = 0;
	int32_t nCount = 0;
	for (int32_t i = 0; i < 8; ++i)
	{
		pTargetNode = pGame->pTargetNodes[i];
		if (pTargetNode)
		{
			D2_ASSERT(pTargetNode->pUnit && pTargetNode->pUnit->dwUnitType == UNIT_PLAYER);

			if (pfCull(pUnit, pTargetNode->pUnit))
			{
				if (pTargetNode->pUnit->dwAnimMode == PLRMODE_DEAD || pTargetNode->pUnit->dwAnimMode == PLRMODE_DEATH)
				{
					nCurrent = 0;
				}
				else
				{
					nCurrent = AITHINK_GetTargetScore(pGame, pUnit, pTargetNode->pUnit, pAiCmd);
				}

				while (1)
				{
					if (nMax < nCurrent)
					{
						nMax = nCurrent;
						pTarget = pTargetNode->pUnit;
					}

					pTargetNode = pTargetNode->pNext;
					++nCount;

					if (!pTargetNode)
					{
						break;
					}

					nCurrent = AITHINK_GetTargetScore(pGame, pUnit, pTargetNode->pUnit, pAiCmd);
				}
			}
		}
	}

	while (pTargetNode)
	{
		if (pUnit->nAct == pTargetNode->pUnit->nAct)
		{
			nCurrent = AITHINK_GetTargetScore(pGame, pUnit, pTargetNode->pUnit, pAiCmd);
			if (nMax < nCurrent)
			{
				nMax = nCurrent;
				pTarget = pTargetNode->pUnit;
			}
			++nCount;
		}

		pTargetNode = pTargetNode->pNext;
	}

	pTargetNode = pGame->pTargetNodes[9];
	if (pTargetNode)
	{
		D2UnitStrc* pTemp = nullptr;
		int32_t nTemp = 0;
		do
		{
			if (pUnit->nAct == pTargetNode->pUnit->nAct)
			{
				nCurrent = AITHINK_GetTargetScore(pGame, pUnit, pTargetNode->pUnit, pAiCmd);
				if (nTemp < nCurrent)
				{
					nTemp = nCurrent;
					pTemp = pTargetNode->pUnit;
				}
			}
			pTargetNode = pTargetNode->pNext;
		}
		while (pTargetNode);

		if (pTemp && AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTemp, nX, nY) < 5 && !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
		{
			PATH_SetTargetUnit(pUnit->pDynamicPath, pTarget);
			PATH_SetType(pUnit->pDynamicPath, PATHTYPE_TOWARD);
			D2Common_10142(pUnit->pDynamicPath, pUnit, 0);

			if (!PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
			{
				nMax = nTemp;
				pTarget = pTemp;
			}
		}
	}

	*pCounter = nCount;
	*a3 = nMax;

	return pTarget;
}

//D2Game.0x6FCE8950
void __fastcall AITHINK_Fn051_Diablo(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2AiCmdStrc* pCurrentAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (!pCurrentAiCmd)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		aiCmd.nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
		pCurrentAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	}

	int32_t nMax = 0;
	int32_t nCounter = 0;
	D2UnitStrc* pTarget = AITHINK_GetTargetForBoss(pGame, pUnit, &nMax, &nCounter, pCurrentAiCmd, AITHINK_CullPotentialTargetsForDiablo);

	int32_t nParam = 0;
	if (pAiTickParam->pAiControl->dwAiParam[0])
	{
		nParam = pAiTickParam->pAiControl->dwAiParam[0];
		if (nParam != 5)
		{
			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[7]);
			if (pSkillsTxtRecord && pSkillsTxtRecord->nAuraState >= 0 && !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState) && AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[7], pAiTickParam->pMonstatsTxt->nSkill[7], pTarget, 0, 0))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				return;
			}
		}
	}
	else
	{
		if (!pTarget)
		{
			if (COLLISION_CheckAnyCollisionWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 2, COLLIDE_MISSILE))
			{
				nParam = 16;
			}
			else
			{
				if (ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 1000) >= 1)
				{
					nParam = 11;
				}
				else
				{
					nParam = 4;
				}
			}

			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[7]);
			if (pSkillsTxtRecord && pSkillsTxtRecord->nAuraState >= 0 && !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState) && AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[7], pAiTickParam->pMonstatsTxt->nSkill[7], pTarget, 0, 0))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				return;
			}
		}
		else
		{
			D2UnitStrc* pTownPortal = nullptr;
			int32_t bCloseToPortal = 0;

			D2GameStrc* pTargetGame = pTarget->pGame;
			if (!pTargetGame)
			{
				pTargetGame = pUnit->pGame;
			}

			if (pTarget->dwUnitType == UNIT_PLAYER)
			{
				pTownPortal = SUNIT_GetServerUnit(pTargetGame, UNIT_OBJECT, PLAYER_GetUniqueIdFromPlayerData(pTarget));

				if (pTownPortal && DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pTownPortal)) == LEVEL_CHAOSSANCTUM && UNITS_GetDistanceToCoordinates(pTownPortal, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]) < 85)
				{
					bCloseToPortal = 1;
				}
			}

			const int32_t bSpecialSkill = AI_CheckSpecialSkillsOnPrimeEvil(pTarget);

			int32_t bFurtherAway = 0;
			int32_t bFarAway = 0;
			if (pCurrentAiCmd)
			{
				const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pTarget, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]);
				if (nDistance > 85)
				{
					bFarAway = 1;
				}

				if (nDistance > 105)
				{
					bFurtherAway = 1;
				}
			}

			const int32_t bColliding = UNITS_TestCollisionWithUnit(pUnit, pTarget, COLLIDE_MISSILE_BARRIER);
			const int32_t nFireResist = STATLIST_UnitGetStatValue(pTarget, STAT_FIRERESIST, 0);
			const int32_t nLightResist = STATLIST_UnitGetStatValue(pTarget, STAT_LIGHTRESIST, 0);

			int32_t chanceArray[17] = {};
			int32_t bInMeleeRange = 0;
			if (UNITS_IsInMeleeRange(pUnit, pTarget, 0))
			{
				chanceArray[0] = 0;
				chanceArray[1] = 0;
				chanceArray[2] = 40;
				chanceArray[3] = 70;
				chanceArray[4] = 0;
				chanceArray[5] = 40;
				chanceArray[6] = 24;
				chanceArray[7] = 40;
				chanceArray[8] = 15;
				chanceArray[9] = 0;
				chanceArray[10] = 0;
				chanceArray[11] = 0;
				chanceArray[12] = 0;
				chanceArray[13] = 0;
				chanceArray[14] = 0;
				chanceArray[15] = 0;
				chanceArray[16] = 0;

				if (UNITS_GetCurrentLifePercentage(pTarget) < 20)
				{
					chanceArray[2] = 50;
				}

				if (STATES_CheckState(pTarget, STATE_COLD))
				{
					chanceArray[7] = 0;
					chanceArray[8] = 0;
				}

				if (nFireResist > nLightResist)
				{
					chanceArray[6] -= 10;
				}

				if (nFireResist < nLightResist)
				{
					chanceArray[6] += 10;
				}

				if (bColliding)
				{
					chanceArray[5] = 0;
					chanceArray[6] = 0;
				}

				if (bCloseToPortal)
				{
					chanceArray[15] = 10;
				}

				bInMeleeRange = 1;
			}

			if (bColliding)
			{
				if (!bInMeleeRange)
				{
					chanceArray[0] = 0;
					chanceArray[1] = 0;
					chanceArray[2] = 0;
					chanceArray[3] = 0;
					chanceArray[4] = 5;
					chanceArray[5] = 0;
					chanceArray[6] = 25;
					chanceArray[7] = 0;
					chanceArray[8] = 25;
					chanceArray[9] = 40;
					chanceArray[10] = 0;
					chanceArray[11] = 0;
					chanceArray[12] = 25;
					chanceArray[13] = 0;
					chanceArray[14] = 0;
					chanceArray[15] = 0;
					chanceArray[16] = 0;

					if (nCounter < 2)
					{
						chanceArray[6] = 0;
						chanceArray[1] = 25;
						chanceArray[8] -= 5;
						chanceArray[9] = 0;
					}

					if (bSpecialSkill)
					{
						if (bFarAway)
						{
							chanceArray[13] = 25;
							chanceArray[1] = 0;
							chanceArray[12] = 15;

							if (!chanceArray[9])
							{
								chanceArray[9] = 20;
							}

							if (nCounter < 2)
							{
								chanceArray[9] -= 5;
							}
						}
						else
						{
							chanceArray[13] = 15;
						}
					}
					else
					{
						if (bFarAway)
						{
							chanceArray[13] = 25;
							chanceArray[1] = 0;
							chanceArray[12] = 15;
							if (!chanceArray[9])
							{
								chanceArray[9] = 20;
							}

							if (nCounter < 2)
							{
								chanceArray[9] -= 5;
							}
						}
					}

					if (bFurtherAway)
					{
						chanceArray[14] = 60;
					}

					if (bCloseToPortal)
					{
						chanceArray[15] = 20;
					}
				}
			}
			else
			{
				if (!bInMeleeRange)
				{
					chanceArray[0] = 0;
					chanceArray[1] = 0;
					chanceArray[2] = 0;
					chanceArray[3] = 0;
					chanceArray[4] = 0;
					chanceArray[5] = 25;
					chanceArray[6] = 25;
					chanceArray[7] = 0;
					chanceArray[8] = 15;
					chanceArray[9] = 20;
					chanceArray[10] = 10;
					chanceArray[11] = 0;
					chanceArray[12] = 20;
					chanceArray[13] = 0;
					chanceArray[14] = 0;
					chanceArray[15] = 0;
					chanceArray[16] = 0;

					if (AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget) > 25)
					{
						chanceArray[10] = 20;
						chanceArray[5] = 0;
						chanceArray[8] -= 5;
					}

					if (nFireResist > nLightResist)
					{
						chanceArray[6] -= 10;
						chanceArray[8] -= 10;
					}

					if (nFireResist < nLightResist)
					{
						chanceArray[6] += 10;
						chanceArray[8] += 10;
					}

					if (nCounter < 2)
					{
						chanceArray[6] -= 10;
					}

					if (nCounter > 3)
					{
						chanceArray[6] += 5;
					}

					if (nMax > 60)
					{
						chanceArray[9] += 10;
					}

					D2PlayerCountBonusStrc playerCountBonus = {};
					MONSTER_GetPlayerCountBonus(pTargetGame, &playerCountBonus, UNITS_GetRoom(pUnit), pUnit);
					if (nCounter < 2 && playerCountBonus.nDifficulty < 2)
					{
						chanceArray[9] = 0;
					}

					if (bSpecialSkill)
					{
						if (bFarAway)
						{
							chanceArray[1] = 0;
							chanceArray[12] = 10;
							chanceArray[10] = 0;
							chanceArray[13] = 15;

							if (!chanceArray[9])
							{
								chanceArray[9] = 10;
							}
						}
						else
						{
							chanceArray[10] = 30;
							chanceArray[13] = 15;
							chanceArray[6] += 10;
						}
					}
					else
					{
						if (bFarAway)
						{
							chanceArray[1] = 0;
							chanceArray[12] = 10;
							chanceArray[10] = 0;
							chanceArray[13] = 15;

							if (!chanceArray[9])
							{
								chanceArray[9] = 10;
							}
						}
					}

					if (bFurtherAway)
					{
						chanceArray[9] = 20;
						chanceArray[14] = 60;
					}

					if (bCloseToPortal)
					{
						chanceArray[15] = 15;
					}
				}
			}

			if (sub_6FC68630(pTargetGame, pUnit, SKILL_DIABPRISON, pTarget, 0, 0))
			{
				chanceArray[9] = 0;
			}

			if (chanceArray[15] && pTownPortal && !sub_6FC68630(pTargetGame, pUnit, SKILL_DIABPRISON, pTownPortal, 0, 0))
			{
				chanceArray[15] = 0;
			}

			nParam = AI_GetRandomArrayIndex(chanceArray, 17, pUnit, 11);

			if (nParam != 5)
			{
				D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[7]);
				if (pSkillsTxtRecord && pSkillsTxtRecord->nAuraState >= 0 && !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState) && AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[7], pAiTickParam->pMonstatsTxt->nSkill[7], pTarget, 0, 0))
				{
					pAiTickParam->pAiControl->dwAiParam[0] = 0;
					return;
				}
			}
		}
	}

	switch (nParam)
	{
	case 1:
	{
		AITACTICS_SetVelocity(pUnit, 0, 20, 0);
		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget));
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 2:
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 3:
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pTarget);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 4:
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL4, pTarget);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 5:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		if (STATES_CheckState(pUnit, STATE_INFERNO))
		{
			STATES_ToggleState(pUnit, STATE_INFERNO, 0);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = nParam;
		return;
	}
	case 6:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[2] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 7:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[1] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 8:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[3] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 9:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[6] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[6], pAiTickParam->pMonstatsTxt->nSkill[6], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 10:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[4] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 12:
	{
		sub_6FCD0E80(pGame, pUnit, pTarget, 4u, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 13:
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[5] < 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[5], pAiTickParam->pMonstatsTxt->nSkill[5], pTarget, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 14:
	{
		AITACTICS_SetVelocity(pUnit, 0, 50, 100u);

		if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		const int32_t nX = CLIENTS_GetUnitX(pUnit);
		const int32_t nY = CLIENTS_GetUnitY(pUnit);

		const int32_t nOffset = AIUTIL_GetDistanceToCoordinates(pUnit, pCurrentAiCmd->nCmdParam[1], pCurrentAiCmd->nCmdParam[2]);

		int32_t nXSign = 0;
		if (nX < pCurrentAiCmd->nCmdParam[1])
		{
			nXSign = -1;
		}
		else if (nX > pCurrentAiCmd->nCmdParam[1])
		{
			nXSign = 1;
		}

		int32_t nYSign = 0;
		if (nY < pCurrentAiCmd->nCmdParam[2])
		{
			nYSign = -1;
		}
		else if (nY > pCurrentAiCmd->nCmdParam[2])
		{
			nYSign = 1;
		}

		AITACTICS_SetVelocity(pUnit, 0, 50, 100u);

		if (!AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, pCurrentAiCmd->nCmdParam[1] + nXSign * (nOffset >> 1), pCurrentAiCmd->nCmdParam[2] + nYSign * (nOffset >> 1)))
		{
			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 15:
	{
		if (!pTarget || pTarget->dwUnitType != UNIT_PLAYER)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 3);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		const int32_t nPortalGUID = PLAYER_GetUniqueIdFromPlayerData(pTarget);
		if (!SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nPortalGUID))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[6] >= 0)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[6], pAiTickParam->pMonstatsTxt->nSkill[6], 0, nPortalGUID, 2);
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 2);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	case 16:
	{
		AITACTICS_SetVelocity(pUnit, 0, 20, 0);
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 5u);
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	default:
	{
		if (pGame->nDifficulty == 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 12);
		}
		else if (pGame->nDifficulty == 1)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 8);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 4);
		}

		pAiTickParam->pAiControl->dwAiParam[0] = 0;
		return;
	}
	}
}

//D2Game.0x6FCE97C0
int32_t __fastcall AITHINK_CullPotentialTargetsForDiablo(D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pTarget);

	if (pUnit->nAct == pTarget->nAct)
	{
		return (uint32_t)AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTarget, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit)) < 55;
	}

	return 0;
}

//D2Game.0x6FCE9890
void __fastcall AITHINK_Fn042_Vendor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (AI_RollPercentage(pUnit) < 20)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_SKILL1, 0, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 30);
}

//D2Game.0x6FCE98E0
void __fastcall AITHINK_Fn086_Hydra(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[0] < pGame->dwGameFrame)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_DEATH, 0, 0);
		return;
	}

	if (pAiTickParam->pTarget && pAiTickParam->nTargetDistance < 25 && AI_RollPercentage(pUnit) < 60)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

//D2Game.0x6FCE9980
void __fastcall AITHINK_Fn099_TrappedSoul(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pUnit)
	{
		pUnit->dwFlags |= UNITFLAG_NOTC;
	}

	D2CoordStrc unitCoord = {};
	UNITS_GetCoords(pUnit, &unitCoord);

	if (pAiTickParam->pTarget && pAiTickParam->nTargetDistance < 5)
	{
		if (!pAiTickParam->pAiControl->dwAiParam[0])
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 1;
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame;
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL2, pAiTickParam->pTarget);
			return;
		}

		if (pAiTickParam->bCombat && pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[1])
		{
			D2CoordStrc targetCoord = {};
			UNITS_GetCoords(pAiTickParam->pTarget, &targetCoord);

			if (unitCoord.nX >= targetCoord.nX)
			{
				if (unitCoord.nY <= targetCoord.nY)
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
					pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + 35;
					return;
				}

				if (unitCoord.nX > targetCoord.nX)
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
					pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + 5;
					return;
				}
			}

			if (unitCoord.nY < targetCoord.nY)
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + 5;
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pAiTickParam->pTarget);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + 35;
			}
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
		}
	}
	else
	{
		if (pAiTickParam->pAiControl->dwAiParam[0])
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pTarget);
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
	}
}

//D2Game.0x6FCE9AF0
void __fastcall AITHINK_Fn088_7TIllusion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwAiParam[1] == 1)
	{
		if (pUnit)
		{
			pUnit->dwFlags |= UNITFLAG_NOTC;
		}

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
	}
	else
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, MONMODE_ATTACK1, CLIENTS_GetUnitX(pUnit) - 10, CLIENTS_GetUnitY(pUnit));
		pAiTickParam->pAiControl->dwAiParam[1] = 1;
	}
}

//D2Game.0x6FCE9BA0
void __fastcall AITHINK_Fn091_DarkWanderer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2CoordStrc wandererCoords = {};

	if (!ACT3Q7_GetWandererCoordinates(pGame, pUnit, &wandererCoords))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (!pAiTickParam->pAiControl->dwAiParam[0])
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 1;
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
		pAiTickParam->pAiControl->dwAiParam[2] = 0;
	}

	if (!pAiTickParam->pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->nTargetDistance >= 20)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] == 1)
	{
		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, wandererCoords.nX, wandererCoords.nY);
		pAiTickParam->pAiControl->dwAiParam[0] = 2;
		return;
	}
	else if (pAiTickParam->pAiControl->dwAiParam[0] != 2)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 40);
		return;
	}

	if (AIUTIL_GetDistanceToCoordinates(pUnit, wandererCoords.nX, wandererCoords.nY) < 2 || pAiTickParam->pAiControl->dwAiParam[1] >= 3)
	{
		if (pUnit)
		{
			pUnit->dwFlags |= UNITFLAG_NOTC;
		}

		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		ACT3Q7_CreateVileDogSpawnTimer(pGame, pUnit);
	}
	else
	{
		++pAiTickParam->pAiControl->dwAiParam[1];
		AITACTICS_SetVelocity(pUnit, 1, 0, 0);
		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, wandererCoords.nX, wandererCoords.nY);
	}
}

//D2Game.0x6FCE9CE0
void __fastcall D2GAME_AI_Unk101_104_6FCE9CE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame;
	pAiTickParam->pAiControl->dwAiParam[1] = -1;
}

enum D2C_AssassinSentryAIParams
{
	ASSASSINSENTRY_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	ASSASSINSENTRY_AI_PARAM_STALL_DURATION = 1,
	ASSASSINSENTRY_AI_PARAM_INACTIVE_STALL_DURATION = 2,
	ASSASSINSENTRY_AI_PARAM_ACTIVE_DISTANCE = 3,
};

//D2Game.0x6FCE9D00
void __fastcall AITHINK_Fn101_AssassinSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (AITHINK_AssasinSentryHasLostTarget(pGame, pUnit, pAiTickParam, 0))
	{
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]);
	if (!pSkill)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return;
	}

	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	int32_t nDistance = 0;
	int32_t bInMeleeRange = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, &bInMeleeRange);
	if (!pTarget || nDistance >= AI_GetParamValue(pGame, pAiTickParam, ASSASSINSENTRY_AI_PARAM_ACTIVE_DISTANCE))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, ASSASSINSENTRY_AI_PARAM_INACTIVE_STALL_DURATION));
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ASSASSINSENTRY_AI_PARAM_ATTACK_CHANCE_PCT) && !AITHINK_AssasinSentryHasLostTarget(pGame, pUnit, pAiTickParam, 1))
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
		AITACTICS_UseSkill(pGame, pUnit, SKILLS_GetSkillMode(pSkill), pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, ASSASSINSENTRY_AI_PARAM_STALL_DURATION));
}

//D2Game.0x6FCE9E60
int32_t __fastcall AITHINK_AssasinSentryHasLostTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, int32_t bDecreaseParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return 1;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pOwner);
	if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return 1;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] < 0)
	{
		const int32_t nSkillId = pAiTickParam->pMonstatsTxt->nSkill[0];
		D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
		if (!pSkillsTxtRecord)
		{
			AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
			return 1;
		}

		D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
		if (!pSkill)
		{
			AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
			return 1;
		}

		pAiTickParam->pAiControl->dwAiParam[1] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, SKILLS_GetSkillLevel(pUnit, pSkill, 1));
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
	{
		if (bDecreaseParam)
		{
			--pAiTickParam->pAiControl->dwAiParam[1];
		}
		return 0;
	}
	else
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return 1;
	}
}

//D2Game.0x6FCE9FB0
void __fastcall D2GAME_AI_Unk102_6FCE9FB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = -1;
	pAiTickParam->pAiControl->dwAiParam[1] = 1;
	pAiTickParam->pAiControl->dwAiParam[2] = 0;
}

//D2Game.0x6FCE9FD0
void __fastcall AITHINK_Fn102_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int32_t nSkillId = pAiTickParam->pMonstatsTxt->nSkill[0];

	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
	if (!pSkillsTxtRecord)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
	if (!pSkill)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return;
	}

	const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
	if (pAiTickParam->pAiControl->dwAiParam[0] < 0)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] < pGame->dwGameFrame)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pGame, pUnit, 0, 0, 0);
		return;
	}

	if (!pAiTickParam->pAiControl->dwAiParam[2])
	{
		if (pSkillsTxtRecord->wSrvMissileA >= 0 && pSkillsTxtRecord->wSrvMissileA < sgptDataTables->nMissilesTxtRecordCount)
		{
			D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
			if (!pOwner)
			{
				pOwner = pUnit;
			}

			D2UnitStrc* pMissile = D2GAME_CreateMissile_6FD115E0(pGame, pOwner, nSkillId, nSkillLevel, pSkillsTxtRecord->wSrvMissileA, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
			if (pMissile)
			{
				UNITS_StoreOwner(pMissile, pUnit);
				STATLIST_AddUnitStat(pUnit, STAT_TOHIT, UNITS_GetAttackRate(pOwner), 0);

				int32_t nToHitPercent = STATLIST_UnitGetStatValue(pOwner, STAT_ITEM_TOHIT_PERCENT, 0);
				D2UnitStrc* pWeapon = sub_6FC7C7B0(pOwner);
				if (pWeapon)
				{
					nToHitPercent += SKILLS_GetWeaponMasteryBonus(pOwner, pWeapon, 0, 0);
				}

				STATLIST_AddUnitStat(pUnit, STAT_ITEM_TOHIT_PERCENT, nToHitPercent, 0);
			}

			pAiTickParam->pAiControl->dwAiParam[2] = 1;
		}
	}

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetCurrentAiCommandFromUnit(pUnit);
	if (!pAiCmd)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 3);
		return;
	}

	AITACTICS_SetVelocity(pUnit, 15, 0, 20);

	if (pAiTickParam->pAiControl->dwAiParam[1])
	{
		if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, pAiCmd->nCmdParam[3], pAiCmd->nCmdParam[4]))
		{
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[1] = (pAiTickParam->pAiControl->dwAiParam[1] == 0);

		if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]))
		{
			return;
		}
	}
	else
	{
		if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]))
		{
			return;
		}

		pAiTickParam->pAiControl->dwAiParam[1] = (pAiTickParam->pAiControl->dwAiParam[1] == 0);

		if (AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, pAiCmd->nCmdParam[3], pAiCmd->nCmdParam[4]))
		{
			return;
		}
	}

	if (!D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pUnit, AIGENERAL_GetMinionOwner(pUnit), 5u))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
	}
}

//D2Game.0x6FCEA2A0
void __fastcall AITHINK_Fn103_InvisoPet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);

	int32_t nParam = AI_GetParamValue(pGame, pAiTickParam, 0);
	int32_t nAnimMode = 0;
	if (pOwner)
	{
		nAnimMode = pOwner->dwAnimMode;
	}

	if (nAnimMode == 3)
	{
		nParam /= 2;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] >= pGame->dwGameFrame && nAnimMode != 3 && !sub_6FCF2E70(pUnit))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, 1));
		return;
	}

	const int32_t nX = CLIENTS_GetUnitX(pOwner) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nParam) - nParam;
	const int32_t nY = CLIENTS_GetUnitY(pOwner) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nParam) - nParam;

	AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL2, SKILL_TELEPORT2, 0, nX, nY);
	pAiTickParam->pAiControl->dwAiParam[0] = pGame->dwGameFrame + AI_GetParamValue(pGame, pAiTickParam, 1);
}

enum D2C_DeathSentryAIParams
{
	DEATHSENTRY_AI_PARAM_STALL_DURATION = 1,
	DEATHSENTRY_AI_PARAM_SKILL_CHANCE_PCT = 2,
	DEATHSENTRY_AI_PARAM_ACTIVE_DISTANCE = 3,
};

//D2Game.0x6FCEA490
void __fastcall AITHINK_Fn104_DeathSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (AITHINK_AssasinSentryHasLostTarget(pGame, pUnit, pAiTickParam, 0))
	{
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]);
	if (!pSkill)
	{
		return;
	}

	const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
	if (nSkillLevel <= 0)
	{
		return;
	}

	int32_t nDistance = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget1 = sub_6FCF2CC0(pGame, pUnit, &nDistance, &bCombat);
	if (!pTarget1)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DEATHSENTRY_AI_PARAM_STALL_DURATION));
		return;
	}

	D2UnitStrc* pTarget2 = sub_6FD15210(pUnit, pTarget1, pAiTickParam->pMonstatsTxt->nSkill[0], nSkillLevel);

	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);
	int32_t nMaxDistance = 0;
	if (pSkillsTxtRecord)
	{
		nMaxDistance = pSkillsTxtRecord->dwParam[2] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[3];
	}

	if (pTarget2 && pTarget2->dwUnitId != pAiTickParam->pAiControl->dwAiParam[0] && UNITS_GetDistanceToOtherUnit(pTarget1, pTarget2) < nMaxDistance / 2)
	{
		if (!AITHINK_AssasinSentryHasLostTarget(pGame, pUnit, pAiTickParam, 1))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = pTarget2->dwUnitId;
			AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL2, pAiTickParam->pMonstatsTxt->nSkill[0], pTarget2, 0, 0);
		}
		return;
	}

	if (nDistance < AI_GetParamValue(pGame, pAiTickParam, DEATHSENTRY_AI_PARAM_ACTIVE_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, DEATHSENTRY_AI_PARAM_SKILL_CHANCE_PCT))
	{
		if (!AITHINK_AssasinSentryHasLostTarget(pGame, pUnit, pAiTickParam, 1))
		{
			AITACTICS_UseSkill(pGame, pUnit, MONMODE_SEQUENCE, pAiTickParam->pMonstatsTxt->nSkill[1], pTarget1, 0, 0);
		}
		return;
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DEATHSENTRY_AI_PARAM_STALL_DURATION));
	}
}

//D2Game.0x6FCEA680
void __fastcall D2GAME_AI_Unk105_6FCEA680(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[2] = 1;

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pOwner, pAiTickParam->pMonstatsTxt->wAiParam[7][0]);
	if (pSkill)
	{
		pAiTickParam->pAiControl->dwAiParam[2] = SKILLS_GetSkillLevel(pOwner, pSkill, 1);
	}
}

enum D2C_ShadowWarriorAIParams
{
	SHADOWWARRIOR_AI_PARAM_MAX_TARGET_DISTANCE = 0,
	SHADOWWARRIOR_AI_PARAM_MAX_BOSS_DISTANCE = 1,
	SHADOWWARRIOR_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	SHADOWWARRIOR_AI_PARAM_SKILL_DECREMENT = 3,
	SHADOWWARRIOR_AI_PARAM_SUMMONING_SKILL_MIN_MAX_TO_USE_SKILL = 7,
};

//D2Game.0x6FCEA6D0
void __fastcall AITHINK_Fn105_ShadowWarrior(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pTarget = pAiTickParam->pTarget;

	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 100);
		return;
	}

	const int32_t nParam = D2Clamp(pAiTickParam->pMonstatsTxt->wAiParam[7][2], 1i16, 256i16);

	pAiTickParam->pAiControl->dwAiParam[1] -= 1 + AI_GetParamValue(pGame, pAiTickParam, SHADOWWARRIOR_AI_PARAM_SKILL_DECREMENT);

	if (pAiTickParam->pAiControl->dwAiParam[1] < 0 || pAiTickParam->pAiControl->dwAiParam[1] > nParam << 6)
	{
		pAiTickParam->pAiControl->dwAiParam[1] = 0;
	}

	if (pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, SHADOWWARRIOR_AI_PARAM_MAX_TARGET_DISTANCE) 
		|| UNITS_GetDistanceToOtherUnit(pUnit, pOwner) > AI_GetParamValue(pGame, pAiTickParam, SHADOWWARRIOR_AI_PARAM_MAX_BOSS_DISTANCE))
	{
		pTarget = nullptr;
	}

	if (!sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, pAiTickParam->bCombat, pAiTickParam, 0, 6))
	{
		if (!pTarget)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
			return;
		}

		D2SkillStrc* pRightOwnerSkill = UNITS_GetRightSkill(pOwner);
		D2SkillStrc* pLeftOwnerSkill = UNITS_GetLeftSkill(pOwner);

		if (pRightOwnerSkill && pLeftOwnerSkill)
		{
			const int32_t nRightOwnerSkillId = SKILLS_GetSkillIdFromSkill(pRightOwnerSkill, __FILE__, __LINE__);
			const int32_t nLeftOwnerSkillId = SKILLS_GetSkillIdFromSkill(pLeftOwnerSkill, __FILE__, __LINE__);

			const int32_t nRightOwnerSkillLevel = std::max(SKILLS_GetSkillLevel(pOwner, pRightOwnerSkill, 1) / 2 + pAiTickParam->pAiControl->dwAiParam[2] / 3, 1);
			const int32_t nLeftOwnerSkillLevel = std::max(SKILLS_GetSkillLevel(pOwner, pLeftOwnerSkill, 1) / 2 + pAiTickParam->pAiControl->dwAiParam[2] / 3, 1);

			SKILLS_AssignSkill(pUnit, nRightOwnerSkillId, nRightOwnerSkillLevel, 0, __FILE__, __LINE__);
			SKILLS_AssignSkill(pUnit, nLeftOwnerSkillId, nLeftOwnerSkillLevel, 0, __FILE__, __LINE__);

			D2SkillStrc* pRightSkill = SKILLS_GetSkillById(pUnit, nRightOwnerSkillId, -1);
			D2SkillStrc* pLeftSkill = SKILLS_GetSkillById(pUnit, nLeftOwnerSkillId, -1);

			if (pRightSkill && pLeftSkill)
			{
				D2SkillStrc* pSkill = pLeftSkill;

				if (!(ITEMS_RollRandomNumber(&pUnit->pSeed) & 1))
				{
					pSkill = pRightSkill;
				}

				int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
				const int32_t nChanceParam = std::max(pAiTickParam->pAiControl->dwAiParam[2], 1);
				const int32_t nCombatChance = D2Clamp(AI_GetParamValue(pGame, pAiTickParam, SHADOWWARRIOR_AI_PARAM_ATTACK_CHANCE_PCT) - 2 * nChanceParam, 5, 100);

				if (pAiTickParam->bCombat)
				{
					if (AI_RollPercentage(pUnit) < nCombatChance)
					{
						pSkill = SKILLS_GetSkillById(pUnit, 0, -1);
						nSkillId = 0;
					}
				}

				if (!AITHINK_ShadowWarriorCheckUseSkill(pGame, pUnit, pOwner, pSkill, nSkillId, pAiTickParam->bCombat, pAiTickParam))
				{
					if (pSkill != pLeftSkill)
					{
						pSkill = pLeftSkill;
					}
					else
					{
						pSkill = pRightSkill;
					}

					if (!AITHINK_ShadowWarriorCheckUseSkill(pGame, pUnit, pOwner, pSkill, SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__), pAiTickParam->bCombat, pAiTickParam))
					{
						pSkill = SKILLS_GetSkillById(pUnit, 0, -1);
						if (!pSkill)
						{
							SKILLS_AssignSkill(pUnit, 0, 1, 0, __FILE__, __LINE__);
							pSkill = SKILLS_GetSkillById(pUnit, 0, -1);
						}
					}
				}

				if (pSkill)
				{
					nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);

					if (D2Common_11016(pUnit, pSkill) == 1 && !pAiTickParam->bCombat)
					{
						AITACTICS_RunToTargetUnit(pGame, pUnit, pTarget);
						return;
					}

					AITACTICS_UseSkill(pGame, pUnit, SKILLS_GetSkillMode(pSkill), SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__), pTarget, 0, 0);

					D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

					const int32_t nSkillLevel = std::max(SKILLS_GetSkillLevel(pUnit, pSkill, 1), 1);

					pAiTickParam->pAiControl->dwAiParam[0] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwDelay, nSkillId, nSkillLevel) / 3 + pGame->dwGameFrame + 18;
					return;
				}
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
	}
}

//D2Game.0x6FCEAC10
BOOL __fastcall AITHINK_ShadowWarriorCheckUseSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, D2SkillStrc* pSkill, int32_t nSkillId, int32_t bCombat, D2AiTickParamStrc* pAiTickParam)
{
	if (!pSkill || !pOwner || !pAiTickParam || !pAiTickParam->pAiControl || SKILLS_GetClassIdFromSkillId(nSkillId) != pOwner->dwClassId)
	{
		return FALSE;
	}

	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
	if (!pSkillsTxtRecord || !AITHINK_ShadowMasterCheckTargetPetType(pGame, pOwner, pUnit, nSkillId))
	{
		return FALSE;
	}

	if (pAiTickParam->bCombat)
	{
		if (pSkillsTxtRecord->nAiType != 4 && pSkillsTxtRecord->nAiType != 13)
		{
			return FALSE;
		}
	}
	else
	{
		if (pSkillsTxtRecord->nAiType == 4 || pSkillsTxtRecord->nAiType == 13)
		{
			return FALSE;
		}
	}

	if (pSkillsTxtRecord->nAiType == 1 && pSkillsTxtRecord->nAuraState > 0 && STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
	{
		return FALSE;
	}

	if (!pAiTickParam->pTarget)
	{
		switch (pSkillsTxtRecord->nAiType)
		{
		case 2u:
		case 4u:
		case 5u:
		case 11u:
		case 12u:
		case 13u:
			return FALSE;

		default:
			break;
		}
	}

	if (pSkillsTxtRecord->nAiType == 2 && pSkillsTxtRecord->nAuraState > 0 && STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState) || pSkillsTxtRecord->wAuraTargetState > 0 && STATES_CheckState(pAiTickParam->pTarget, pSkillsTxtRecord->wAuraTargetState))
	{
		return FALSE;
	}

	if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PROGRESSIVE] && pSkillsTxtRecord->nAuraState > 0 && STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
	{
		D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
		if (pStatList && STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0) >= 3)
		{
			return FALSE;
		}
	}

	if (nSkillId)
	{
		const int32_t nManaCost = SKILLS_GetShiftedManaCosts(nSkillId, SKILLS_GetSkillLevel(pUnit, pSkill, 1));

		if (AI_RollPercentage(pUnit) > 100 - 160 * nManaCost / 100 || pAiTickParam->pAiControl->dwAiParam[0] > pGame->dwGameFrame)
		{
			return FALSE;
		}

		const int32_t nParam1 = D2Clamp(pAiTickParam->pMonstatsTxt->wAiParam[SHADOWWARRIOR_AI_PARAM_SUMMONING_SKILL_MIN_MAX_TO_USE_SKILL][1], 1i16, 128i16);
		const int32_t nParam2 = D2Clamp(pAiTickParam->pMonstatsTxt->wAiParam[SHADOWWARRIOR_AI_PARAM_SUMMONING_SKILL_MIN_MAX_TO_USE_SKILL][2], 1i16, 256i16);

		if (pAiTickParam->pAiControl->dwAiParam[1] < nParam1 || pAiTickParam->pAiControl->dwAiParam[1] > 32 * nParam2)
		{
			pAiTickParam->pAiControl->dwAiParam[1] = nParam1;
		}

		if (ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pAiTickParam->pAiControl->dwAiParam[1]) > AI_RollPercentage(pUnit))
		{
			return FALSE;
		}

		pAiTickParam->pAiControl->dwAiParam[1] += nManaCost * (320 - pAiTickParam->pAiControl->dwAiParam[2]) / (pAiTickParam->pAiControl->dwAiParam[2] + 100);
	}

	return TRUE;
}

//D2Game.0x6FCEAF20
int32_t __fastcall AITHINK_ShadowMasterCheckTargetPetType(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
	if (!pSkillsTxtRecord)
	{
		return 0;
	}

	const int32_t nSummonId = pSkillsTxtRecord->wSummon;
	if (!nSummonId)
	{
		return 1;
	}

	int32_t nTargetClassId = -1;
	int32_t nTargetGUID = -1;
	if (pTarget)
	{
		nTargetClassId = pTarget->dwClassId;
		nTargetGUID = pTarget->dwUnitId;
	}

	if (nSummonId == nTargetClassId)
	{
		return 0;
	}

	if (pSkillsTxtRecord->nPetType < 0 || pSkillsTxtRecord->nPetType >= sgptDataTables->nPetTypeTxtRecordCount || !pUnit)
	{
		return 1;
	}

	return PLAYERPETS_GetPetTypeFromPetGUID(pUnit, nTargetGUID) != pSkillsTxtRecord->nPetType;
}

enum D2C_ShadowMasterAIParams
{
	SHADOWMASTER_AI_PARAM_APPROACH_DISTANCE_MELEE_BONUS_PROGRESSIVE_BONUS = 0,
	SHADOWMASTER_AI_PARAM_RANDOM_PICK_IGNORE_RANGE_BOSS_LEASH = 1,
	SHADOWMASTER_AI_PARAM_ATTACK_CHANCE_PCT = 2,

	SHADOWMASTER_AI_PARAM_SUMMONING_SKILL = 7,
};

//D2Game.0x6FCEAFE0
void __fastcall D2GAME_AI_Unk106_6FCEAFE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = 0;
	pAiTickParam->pAiControl->dwAiParam[1] = 0;
	pAiTickParam->pAiControl->dwAiParam[2] = 1;

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
	{
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pOwner, pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_SUMMONING_SKILL][0]);
	if (pSkill)
	{
		pAiTickParam->pAiControl->dwAiParam[2] = SKILLS_GetSkillLevel(pOwner, pSkill, 1);
	}

	const int32_t nParam = pAiTickParam->pAiControl->dwAiParam[2];

	if (!SKILLS_GetSkillById(pUnit, 0, -1))
	{
		SKILLS_AssignSkill(pUnit, 0, 1, 0, __FILE__, __LINE__);
	}

	SKILLS_SetLeftActiveSkill(pUnit, 0, -1);
	SKILLS_SetRightActiveSkill(pUnit, 0, -1);

	for (int32_t i = 0; i < SKILLS_GetPlayerSkillCount(pOwner->dwClassId); ++i)
	{
		const int32_t nSkillId = SKILLS_GetClassSkillId(pOwner->dwClassId, i);

		if (AITHINK_ShadowMasterCheckTargetPetType(pGame, pOwner, pUnit, nSkillId))
		{
			//SKILLS_GetSkillById(pUnit, nSkillId, -1);
			pSkill = SKILLS_GetSkillById(pOwner, nSkillId, -1);
			
			const int32_t nSkillLevel = pSkill ? SKILLS_GetSkillLevel(pOwner, pSkill, 0) : 1;
			const int32_t nLevel = D2Clamp(nParam / 2 + nSkillLevel / 2, 1, 24);

			SKILLS_AssignSkill(pUnit, nSkillId, nLevel, 0, __FILE__, __LINE__);
		}
	}
}

//D2Game.0x6FCEB1B0
void __fastcall D2GAME_AI_Unk143_6FCEB1B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[2] = 1;

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
	{
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pOwner, pAiTickParam->pMonstatsTxt->wAiParam[7][0]);
	if (pSkill)
	{
		pAiTickParam->pAiControl->dwAiParam[2] = SKILLS_GetSkillLevel(pOwner, pSkill, 1);
	}

	if (!SKILLS_GetSkillById(pUnit, 0, -1))
	{
		SKILLS_AssignSkill(pUnit, 0, 1, 0, __FILE__, __LINE__);
	}

	SKILLS_SetLeftActiveSkill(pUnit, 0, -1);
	SKILLS_SetRightActiveSkill(pUnit, 0, -1);
}

//D2Game.0x6FCEB240
void __fastcall AITHINK_Fn106_143_ShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	const int16_t nApproachDistance = pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_APPROACH_DISTANCE_MELEE_BONUS_PROGRESSIVE_BONUS][0];
	const int16_t nMeleeBonus = pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_APPROACH_DISTANCE_MELEE_BONUS_PROGRESSIVE_BONUS][1];
	const int16_t nProgressiveBonus = pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_APPROACH_DISTANCE_MELEE_BONUS_PROGRESSIVE_BONUS][2];
	const int16_t nRandomPick = pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_RANDOM_PICK_IGNORE_RANGE_BOSS_LEASH][0];

	D2UnitStrc* pTarget = pAiTickParam->pTarget;

	if (!pUnit->pSkills)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 100);
		return;
	}

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

	const int16_t nBossLeashDistance = pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_RANDOM_PICK_IGNORE_RANGE_BOSS_LEASH][2];
	if (pOwner && AITHINK_GetSquaredDistance(pUnit, pOwner) > nBossLeashDistance * nBossLeashDistance && sub_6FCE34E0(pGame, pUnit, 0, pOwner, pAiTickParam->bCombat, pAiTickParam, 0, 6))
	{
		return;
	}

	D2UnitStrc* pTemp = nullptr;
	if (pAiTickParam->pAiControl->dwAiParam[0] <= 0)
	{
		pTemp = pTarget;
	}
	else
	{
		pTemp = SUNIT_GetTargetUnit(pGame, pUnit);
		if (pTemp)
		{
			pTarget = pTemp;
		}
		else
		{
			pTemp = pTarget;
		}

		if (pTemp)
		{
			--pAiTickParam->pAiControl->dwAiParam[0];
			if (sub_6FCECBA0(pGame, pOwner, pUnit, pAiTickParam->pAiControl->dwAiParam[1], pAiTickParam->bCombat, pTemp, 0, 0, pAiTickParam))
			{
				return;
			}
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			pAiTickParam->pAiControl->dwAiParam[1] = 0;
		}
	}

	const int16_t nIgnoreRange = pAiTickParam->pMonstatsTxt->wAiParam[SHADOWMASTER_AI_PARAM_RANDOM_PICK_IGNORE_RANGE_BOSS_LEASH][1];
	if (pAiTickParam->nTargetDistance > nIgnoreRange)
	{
		pTarget = nullptr;
		pTemp = nullptr;
	}

	if (!pTemp)
	{
		for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
		{
			const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);

			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
			if (pSkillsTxtRecord)
			{
				if (pSkillsTxtRecord->nAiType == 1)
				{
					if (pSkillsTxtRecord->nAuraState > 0 && !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
					{
						D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(pSkillsTxtRecord->nAuraState);

						int32_t bSameStateGroup = 0;
						if (pStatesTxtRecord && pStatesTxtRecord->wGroup)
						{
							for (int32_t nState = 0; nState < sgptDataTables->nStatesTxtRecordCount; ++nState)
							{
								if (nState != pSkillsTxtRecord->nAuraState && SKILLS_GetStatesTxtRecord(nState)->wGroup == pStatesTxtRecord->wGroup && STATES_CheckState(pUnit, nState))
								{
									bSameStateGroup = 1;
								}
							}
						}

						if (!bSameStateGroup || AI_RollPercentage(pUnit) < 4)
						{
							if (AI_RollPercentage(pUnit) < 60 && sub_6FCECBA0(pGame, pOwner, pUnit, nSkillId, pAiTickParam->bCombat, 0, 0, 0, pAiTickParam))
							{
								return;
							}
						}
					}
				}
				else if (pSkillsTxtRecord->nAiType == 6)
				{
					if (!UNITS_GetLeftSkill(pUnit) && AI_RollPercentage(pUnit) < 20)
					{
						SKILLS_SetLeftActiveSkill(pUnit, nSkillId, -1);
					}
				}
			}
		}
	}

	D2UnitStrc* pOwnerTarget = nullptr;
	if (pOwner)
	{
		pOwnerTarget = SUNIT_GetTargetUnit(pGame, pOwner);
		if (pOwnerTarget)
		{
			if (!SUNIT_IsDead(pOwnerTarget) && sub_6FCBD900(pGame, pUnit, pOwnerTarget))
			{
				pTarget = pOwnerTarget;
			}
			else
			{
				pOwnerTarget = nullptr;
			}
		}

		if (AITHINK_GetSquaredDistance(pUnit, pOwner) >= 144 && sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, pAiTickParam->bCombat, pAiTickParam, 0, 6))
		{
			return;
		}
	}

	if (!pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	const int32_t nParam = std::max(pAiTickParam->pAiControl->dwAiParam[2], 1);

	const int32_t nChance = D2Clamp(AI_GetParamValue(pGame, pAiTickParam, SHADOWMASTER_AI_PARAM_ATTACK_CHANCE_PCT) - 2 * nParam, 5, 100);

	if (!pAiTickParam->bCombat || AI_RollPercentage(pUnit) >= nChance || !sub_6FCECBA0(pGame, pOwner, pUnit, 0, pAiTickParam->bCombat, pTarget, 0, 0, pAiTickParam))
	{
		D2ShadowMasterAiCallbackArgStrc arg = {};
		arg.unk0x00 = pOwner;
		arg.unk0x08 = INT_MAX;
		arg.unk0x14 = INT_MAX;

		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_ShadowMaster, 1);

		if (!pAiTickParam->bCombat)
		{
			if (pOwnerTarget)
			{
				pTarget = pOwnerTarget;
			}
			else
			{
				if (arg.unk0x10)
				{
					pTarget = arg.unk0x10;
				}
				else if (arg.unk0x24)
				{
					if (AITHINK_GetSquaredDistance(pUnit, arg.unk0x24) < 1024)
					{
						pTarget = arg.unk0x24;
					}
				}
			}

			if (!AITHINK_IsTargetKillableByShadowMaster(pUnit, pTarget))
			{
				D2UnitStrc* pTargetOwner = AIGENERAL_GetMinionOwner(pTarget);
				if (pTargetOwner && !SUNIT_IsDead(pTargetOwner) && AITHINK_GetSquaredDistance(pUnit, pTargetOwner) < 1024)
				{
					pTarget = pTargetOwner;
				}
			}
		}

		const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);

		const int32_t nDistanceToTarget = AITHINK_GetSquaredDistance(pUnit, pTarget);

		int32_t bProgressiveState = 0;

		if (nProgressiveBonus > 0 && STATES_CheckStateMaskPgsvOnUnit(pUnit))
		{
			bProgressiveState = 1;
		}

		const bool bNotColliding = (UNITS_TestCollisionWithUnit(pUnit, pTarget, COLLIDE_MISSILE_BARRIER) == 0);

		if (arg.unk0x0C > 3 && (ITEMS_RollRandomNumber(&pUnit->pSeed) & 31) < 2 * arg.unk0x0C)
		{
			if (pOwner && AITHINK_GetSquaredDistance(pUnit, pOwner) > 36)
			{
				AITACTICS_RunToTargetUnit(pGame, pUnit, pOwner);
				return;
			}

			if (sub_6FCD06D0(pGame, pUnit, pTarget, 8u, 1))
			{
				return;
			}
		}

		int32_t nAuraStatBonus = 0;

		D2ShadowMasterSkillStrc shadowMasterSkills[48] = {};
		shadowMasterSkills[0].pTarget = pTarget;
		shadowMasterSkills[0].nSkillId = 0;
		shadowMasterSkills[0].nChance = 0;
		shadowMasterSkills[0].nX = 0;
		shadowMasterSkills[0].nY = 0;

		int32_t nCounter = 1;

		for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
		{
			const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
			if (pSkillsTxtRecord)
			{
				D2UnitStrc* pSkillTarget = pTarget;
				int32_t nCurrentValue = 0;
				int32_t nResistance = 0;
				switch (pSkillsTxtRecord->nEType)
				{
				case ELEMTYPE_NONE:
					nResistance = STATLIST_UnitGetStatValue(pTarget, STAT_DAMAGERESIST, 0);
					break;

				case ELEMTYPE_FIRE:
					nResistance = STATLIST_UnitGetStatValue(pTarget, STAT_FIRERESIST, 0);
					break;

				case ELEMTYPE_LTNG:
					nResistance = STATLIST_UnitGetStatValue(pTarget, STAT_LIGHTRESIST, 0);
					break;

				case ELEMTYPE_COLD:
				case ELEMTYPE_FREEZE:
					nResistance = STATLIST_UnitGetStatValue(pTarget, STAT_COLDRESIST, 0);
					break;

				case ELEMTYPE_POIS:
					nResistance = STATLIST_UnitGetStatValue(pTarget, STAT_POISONRESIST, 0);
					break;

				case ELEMTYPE_MAGIC:
					nResistance = STATLIST_UnitGetStatValue(pTarget, STAT_MAGICRESIST, 0);
					break;

				default:
					nResistance = 0;
					break;
				}

				int32_t nTemp = nResistance / -10 + (pSkillsTxtRecord->wReqLevel / 4 + SKILLS_GetSkillLevel(pUnit, pSkill, 1) + pSkillsTxtRecord->wAiBonus);

				switch (pSkillsTxtRecord->nAiType)
				{
				case 1u:
				{
					if (pSkillsTxtRecord->nAuraState <= 0 || STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
					{
						if (arg.unk0x08 <= 25)
						{
							nTemp -= 6;
						}

						D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(pSkillsTxtRecord->nAuraState);
						if (!pStatesTxtRecord || !pStatesTxtRecord->wGroup || sgptDataTables->nStatesTxtRecordCount <= 0)
						{
							nTemp += 10;
						}
						else
						{
							int32_t bSameStateGroup = 0;
							for (int32_t nState = 0; nState < sgptDataTables->nStatesTxtRecordCount; ++nState)
							{
								if (nState != pSkillsTxtRecord->nAuraState)
								{
									if (SKILLS_GetStatesTxtRecord(nState)->wGroup == pStatesTxtRecord->wGroup && STATES_CheckState(pUnit, nState))
									{
										bSameStateGroup = 1;
									}
								}
							}

							if (bSameStateGroup)
							{
								nTemp -= 10;
							}
							else
							{
								nTemp += 10;
							}
						}

						pSkillTarget = pUnit;
						nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
					}
					break;
				}
				case 2u:
				{
					if (pSkillsTxtRecord->nAuraState <= 0 || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
					{
						if (pSkillsTxtRecord->wAuraTargetState <= 0 || !STATES_CheckState(pTarget, pSkillsTxtRecord->wAuraTargetState))
						{
							if (arg.unk0x08 <= 25)
							{
								nTemp -= 10;
							}

							nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
						}
					}
					break;
				}
				case 3u:
				{
					if (arg.unk0x20 > 5)
					{
						nTemp += -2 * arg.unk0x20;
					}

					if (arg.unk0x08 <= 25)
					{
						nTemp -= 7;
					}

					if (arg.unk0x1C < 3)
					{
						nTemp -= 10;
					}

					nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick) + 3 * (arg.unk0x1C - 3);
					break;
				}
				case 4u:
				{
					if (nDistanceToTarget > nApproachDistance * nApproachDistance)
					{
						nTemp -= 10;
					}

					nTemp += nMeleeBonus;

					if (pAiTickParam->bCombat || nDistanceToTarget <= 25)
					{
						nTemp += 10;
					}

					if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[2]))
					{
						if (nProgressiveBonus <= 0 || bProgressiveState)
						{
							nCurrentValue = nTemp + 4 * nAuraStatBonus + 3;
						}
						else
						{
							nCurrentValue = nTemp - 10;
						}

						nCurrentValue += ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
					}
					else
					{
						int32_t nAuraStatValue = 0;
						if (D2StatListStrc* pStatList = GetAuraStateStatList(pUnit, pSkillsTxtRecord))
						{
							nAuraStatValue = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);
							nAuraStatBonus += nAuraStatValue;
						}

						if (nAuraStatValue < 3)
						{
							nCurrentValue = nProgressiveBonus + nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
						}
					}
					break;
				}
				case 5u:
				{
					if (!bNotColliding)
					{
						break;
					}
					if (pSkillsTxtRecord->wSrvMissile < 0)
					{
						if (D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pSkillsTxtRecord->wSrvMissileA))
						{
							if (nDistanceToTarget >= (pMissilesTxtRecord->wRange - 1) * (pMissilesTxtRecord->wRange - 1))
							{
								break;
							}
						}
					}

					if (arg.unk0x08 <= 25)
					{
						nTemp -= 5;
					}

					if (nDistanceToTarget <= 25)
					{
						nTemp -= 5;
					}

					if (bProgressiveState)
					{
						nTemp -= 5;
					}

					nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
					break;
				}
				case 6u:
				{
					if (!UNITS_GetLeftSkill(pUnit))
					{
						if (AI_RollPercentage(pUnit) < 20)
						{
							SKILLS_SetLeftActiveSkill(pUnit, nSkillId, -1);
						}
					}
					else
					{
						if (AI_RollPercentage(pUnit) < 6)
						{
							SKILLS_SetLeftActiveSkill(pUnit, nSkillId, -1);
						}
					}

					break;
				}
				case 7u:
				{
					nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
					if (nLifePercentage <= 66)
					{
						pSkillTarget = 0;
						if (nLifePercentage < 45)
						{
							nCurrentValue += 20;
						}
						else
						{
							nCurrentValue += 10;
						}
					}
					else
					{
						nCurrentValue = 0;
					}
					break;
				}
				case 8u:
				{
					nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
					if (nLifePercentage <= 66)
					{
						pSkillTarget = pUnit;
						if (nLifePercentage < 45)
						{
							nCurrentValue *= 4;
						}
						else
						{
							nCurrentValue *= 2;
						}
					}
					else
					{
						nCurrentValue = 0;
					}
					break;
				}
				case 11u:
				{
					if (!bNotColliding)
					{
						break;
					}
					if (pSkillsTxtRecord->wSrvMissile < 0)
					{
						if (D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pSkillsTxtRecord->wSrvMissileA))
						{
							if (nDistanceToTarget >= (pMissilesTxtRecord->wRange - 1) * (pMissilesTxtRecord->wRange - 1))
							{
								break;
							}
						}
					}

					if (arg.unk0x08 <= 25)
					{
						nTemp -= 5;
					}

					if (nDistanceToTarget <= 25)
					{
						nTemp -= 5;
					}

					if (bProgressiveState)
					{
						nTemp -= 5;
					}

					nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick) + 3 * arg.unk0x1C;
					break;
				}
				case 12u:
				{
					if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER || pTarget->pMonsterData && pTarget->pMonsterData->pMonstatsTxt && pTarget->pMonsterData->pMonstatsTxt->nDrain[pGame->nDifficulty] >= 25u)
					{
						if (nDistanceToTarget > nApproachDistance * nApproachDistance)
						{
							nTemp -= 10;
						}

						nTemp += nMeleeBonus;
						if (pAiTickParam->bCombat || nDistanceToTarget <= 25)
						{
							nTemp += 10;
						}

						int32_t nAuraStatValue = 0;
						if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[2])
						{
							if (D2StatListStrc* pStatList = GetAuraStateStatList(pUnit, pSkillsTxtRecord))
							{
								nAuraStatValue = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);
								nAuraStatBonus += nAuraStatValue;
							}
						}
						if(nAuraStatValue < 3)
						{
							nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);

							if (nLifePercentage < 75)
							{
								nCurrentValue += 8;
							}

							if (nLifePercentage < 50)
							{
								nCurrentValue += 12;
							}
						}
					}
					break;
				}
				case 13u:
				{
					nTemp += nMeleeBonus;

					if (nProgressiveBonus <= 0 || bProgressiveState)
					{
						nTemp += nAuraStatBonus;
					}
					else
					{
						nTemp -= 5;
					}

					if ((nLifePercentage < 50 || arg.unk0x0C > 3) && arg.unk0x10 && arg.unk0x18 < 4 && AITHINK_GetSquaredDistance(pUnit, arg.unk0x10) > 25)
					{
						pSkillTarget = arg.unk0x10;
						nTemp += 20;
						nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
					}
					else
					{
						if (nDistanceToTarget >= 25)
						{
							if (nDistanceToTarget > 324)
							{
								nTemp += 10;
							}

							nCurrentValue = nTemp + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRandomPick);
						}
					}
					break;
				}

				default:
					break;
				}

				if (nCurrentValue > shadowMasterSkills[nCounter].nChance)
				{
					shadowMasterSkills[nCounter].pTarget = pSkillTarget;
					shadowMasterSkills[nCounter].nSkillId = nSkillId;
					shadowMasterSkills[nCounter].nChance = nCurrentValue;
					shadowMasterSkills[nCounter].nX = 0;
					shadowMasterSkills[nCounter].nY = 0;
					++nCounter;
				}
			}
		}

		do
		{
			--nCounter;

			if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 3
				&& sub_6FCECBA0(pGame, pOwner, pUnit, shadowMasterSkills[nCounter].nSkillId, UNITS_IsInMeleeRange(pUnit, shadowMasterSkills[nCounter].pTarget, 0), shadowMasterSkills[nCounter].pTarget, shadowMasterSkills[nCounter].nX, shadowMasterSkills[nCounter].nY, pAiTickParam))
			{
				D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(shadowMasterSkills[nCounter].nSkillId);
				if (pSkillsTxtRecord && pSkillsTxtRecord->wSrvDoFunc == 19)
				{
					pAiTickParam->pAiControl->dwAiParam[1] = shadowMasterSkills[nCounter].nSkillId;
					pAiTickParam->pAiControl->dwAiParam[0] = 25;
				}
				return;
			}
		}
		while (nCounter);

		if (!pTarget || !sub_6FCECBA0(pGame, pOwner, pUnit, 0, UNITS_IsInMeleeRange(pUnit, pTarget, 0), pTarget, 0, 0, pAiTickParam))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
	}
}

//D2Game.0x6FCEC840
int32_t __fastcall AITHINK_IsTargetKillableByShadowMaster(D2UnitStrc* a1, D2UnitStrc* a2)
{
	if (!a2 || a1 == a2 || a2->dwUnitType != UNIT_MONSTER || SUNIT_IsDead(a2))
	{
		return 0;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(a2->dwClassId);
	if (!pMonStatsTxtRecord || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC] || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_KILLABLE]))
	{
		return 0;
	}

	if (a2->dwUnitType != UNIT_PLAYER && !MONSTERS_IsBoss(nullptr, a2) && !MONSTERS_IsPrimeEvil(a2))
	{
		return MONSTERUNIQUE_CheckMonTypeFlag(a2, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_CHAMPION | MONTYPEFLAG_SUPERUNIQUE) != 0;
	}

	return 1;
}

//D2Game.0x6FCEC910
D2UnitStrc* __fastcall AITHINK_TargetCallback_ShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2ShadowMasterAiCallbackArgStrc* pArg = (D2ShadowMasterAiCallbackArgStrc*)pCallbackArg;

	if (pUnit == pTarget || SUNIT_IsDead(pTarget) || !pTarget)
	{
		return nullptr;
	}

	if (pTarget->dwUnitType == UNIT_MONSTER && STATLIST_AreUnitsAligned(pUnit, pTarget))
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
		
		int32_t nClassId = -1;
		if (pMonStatsTxtRecord)
		{
			nClassId = pMonStatsTxtRecord->nBaseId;
		}

		switch (nClassId)
		{
		case MONSTER_WAKEOFDESTRUCTION:
		case MONSTER_CHARGEBOLTSENTRY:
		case MONSTER_LIGHTNINGSENTRY:
		case MONSTER_BLADECREEPER:
		case MONSTER_INFERNOSENTRY:
		case MONSTER_DEATHSENTRY:
			++pArg->unk0x20;
			return nullptr;

		default:
			break;
		}
	}

	if (!(pTarget->dwFlags & UNITFLAG_CANBEATTACKED) || !sub_6FCBD900(pGame, pUnit, pTarget))
	{
		return nullptr;
	}

	if (pArg->unk0x00)
	{
		const int32_t nDistance = AITHINK_GetSquaredDistance(pArg->unk0x00, pTarget);

		if (nDistance <= 100)
		{
			++pArg->unk0x18;
		}

		if (nDistance < pArg->unk0x14)
		{
			pArg->unk0x10 = pTarget;
			pArg->unk0x14 = nDistance;
		}
	}


	const int32_t nDistance = AITHINK_GetSquaredDistance(pUnit, pTarget);
	if (nDistance <= 1024)
	{
		++pArg->unk0x1C;

		if (nDistance <= 100)
		{
			++pArg->unk0x0C;
		}

		if (nDistance < pArg->unk0x08)
		{
			pArg->unk0x04 = pTarget;
			pArg->unk0x08 = nDistance;
		}

		if (AITHINK_IsTargetKillableByShadowMaster(pUnit, pTarget))
		{
			pArg->unk0x24 = pTarget;
		}
	}

	return nullptr;
}

//D2Game.0x6FCECBA0
int32_t __fastcall sub_6FCECBA0(D2GameStrc* pGame, D2UnitStrc* a2, D2UnitStrc* pUnit, WORD wSkillId, int32_t a5, D2UnitStrc* pTarget, int32_t nX, int32_t nY, D2AiTickParamStrc* a9)
{
	if (pTarget && (pTarget == a2 || pTarget == pUnit))
	{
		return 0;
	}

	D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, wSkillId, -1);
	if (!pSkill)
	{
		return 0;
	}

	if (D2Common_11016(pUnit, pSkill) == 1 && !a5)
	{
		return AITACTICS_RunToTargetUnitWithFlags(pGame, pUnit, pTarget, 4);
	}

	return AITACTICS_UseSkill(pGame, pUnit, SKILLS_GetSkillMode(pSkill), wSkillId, pTarget, nX, nY);
}

//D2Game.0x6FCECC40
void __fastcall D2GAME_AI_Unk_110_111_6FCECC40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = 0;
}

enum D2C_VinesAIParams
{
	VINES_AI_PARAM_TARGET_SKILL_DELAY = 0,
	VINES_AI_PARAM_TARGET_SIGHT_DISTANCE = 1,
	VINES_AI_PARAM_STALL_DURATION = 2,
	VINES_AI_PARAM_WALK_AWAY_DISTANCE = 3,
	VINES_AI_PARAM_MOVE_BACK_TO_OWNER_DISTANCE = 4,
};

//D2Game.0x6FCECC50
void __fastcall AITHINK_Fn110_Vines(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (UNITS_GetDistanceToOtherUnit(pUnit, pOwner) >= AI_GetParamValue(pGame, pAiTickParam, VINES_AI_PARAM_MOVE_BACK_TO_OWNER_DISTANCE) && D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, 6))
	{
		return;
	}

	if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
	{
		if (!sub_6FCE34E0(pGame, pUnit, 0, pOwner, 0, pAiTickParam, 0, 6))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, VINES_AI_PARAM_STALL_DURATION));
		}
		return;
	}

	int32_t nDistance = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget = sub_6FCF2CC0(pGame, pUnit, &nDistance, &bCombat);
	if (nDistance >= AI_GetParamValue(pGame, pAiTickParam, VINES_AI_PARAM_TARGET_SIGHT_DISTANCE))
	{
		pTarget = nullptr;
	}

	if (!sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, bCombat, pAiTickParam, 0, 6))
	{
		if (pTarget)
		{
			if (STATES_CheckState(pTarget, STATE_POISON) || STATLIST_UnitGetStatValue(pTarget, STAT_POISONRESIST, 0) == 100)
			{
				D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pTarget, AI_GetParamValue(pGame, pAiTickParam, VINES_AI_PARAM_WALK_AWAY_DISTANCE), 0);
				return;
			}

			if (!bCombat)
			{
				AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTarget, 7);
				return;
			}

			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && (pAiTickParam->pAiControl->dwAiParam[1] + AI_GetParamValue(pGame, pAiTickParam, VINES_AI_PARAM_TARGET_SKILL_DELAY)) < pGame->dwGameFrame)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
				pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame;
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, VINES_AI_PARAM_STALL_DURATION));
	}
}

enum D2C_CycleOfLifeAIParams
{
	CYCLEOFLIFE_AI_PARAM_TARGET_SKILL_DELAY = 0,
	CYCLEOFLIFE_AI_PARAM_TARGET_SIGHT_DISTANCE = 1,
	CYCLEOFLIFE_AI_PARAM_STALL_DURATION = 2,
	CYCLEOFLIFE_AI_PARAM_WALK_AWAY_DISTANCE = 3,
	CYCLEOFLIFE_AI_PARAM_MOVE_BACK_TO_OWNER_DISTANCE = 4,
};

//D2Game.0x6FCECE50
void __fastcall AITHINK_Fn111_CycleOfLife(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);

	if (!pOwner)
	{
		return;
	}

	D2UnitStrc* pTarget = pAiTickParam->pTarget;
	int32_t bCombat = pAiTickParam->bCombat;
	int32_t bUseSkill = 1;

	if (UNITS_GetDistanceToOtherUnit(pUnit, pOwner) >= AI_GetParamValue(pGame, pAiTickParam, CYCLEOFLIFE_AI_PARAM_MOVE_BACK_TO_OWNER_DISTANCE) && D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, 6))
	{
		return;
	}

	if (pTarget && SUNIT_IsDead(pTarget))
	{
		pTarget = nullptr;
		bCombat = 0;
	}

	D2UnitStrc* pTargetUnit = nullptr;
	int32_t nDistance = 0;

	if (pAiTickParam->pMonstatsTxt->nSkill[0] > 0)
	{
		D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);
		D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]);
		if (pSkillsTxtRecord && pSkill)
		{
			const int32_t nAuraRange = D2Clamp(SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, pAiTickParam->pMonstatsTxt->nSkill[0], SKILLS_GetSkillLevel(pUnit, pSkill, 1)), 5, 50);
			pTargetUnit = SKILLS_FindUseableCorpse(pGame, pUnit, pOwner, nAuraRange);
			if (pTargetUnit)
			{
				nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pTargetUnit);
			}
		}
	}

	int32_t bInMeleeRange = 0;
	if (nDistance < AI_GetParamValue(pGame, pAiTickParam, CYCLEOFLIFE_AI_PARAM_TARGET_SIGHT_DISTANCE))
	{
		if (pTargetUnit)
		{
			bInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0);
		}
	}
	else
	{
		pTargetUnit = nullptr;
	}

	if (sub_6FCE34E0(pGame, pUnit, pTargetUnit, pOwner, bInMeleeRange, pAiTickParam, 0, 6))
	{
		return;
	}

	int32_t nClassId = -1;
	if (pUnit)
	{
		nClassId = pUnit->dwClassId;
	}

	if (nClassId == MONSTER_CYCLEOFLIFE)
	{
		if (STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0) >= STATLIST_GetMaxLifeFromUnit(pOwner))
		{
			bUseSkill = 0;
		}
	}
	else if (nClassId == MONSTER_VINECREATURE)
	{
		if (STATLIST_UnitGetStatValue(pOwner, STAT_MANA, 0) >= STATLIST_GetMaxManaFromUnit(pOwner))
		{
			bUseSkill = 0;
		}
	}

	if (pTargetUnit && bInMeleeRange && bUseSkill && (pAiTickParam->pAiControl->dwAiParam[1] + AI_GetParamValue(pGame, pAiTickParam, CYCLEOFLIFE_AI_PARAM_TARGET_SKILL_DELAY)) < pGame->dwGameFrame)
	{
		AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL1, pAiTickParam->pMonstatsTxt->nSkill[0], pTargetUnit, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame;
		return;
	}

	if (bCombat && pTarget && (AI_RollPercentage(pUnit) < 25))
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pTarget, AI_GetParamValue(pGame, pAiTickParam, CYCLEOFLIFE_AI_PARAM_WALK_AWAY_DISTANCE), 0);
		return;
	}

	if (pTargetUnit)
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pTargetUnit, 7);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, CYCLEOFLIFE_AI_PARAM_STALL_DURATION));
}

enum D2C_RavenAIParams
{
	RAVEN_AI_PARAM_CIRCLE_OWNER_MAX_DISTANCE = 0,
	RAVEN_AI_PARAM_CIRCLE_OWNER_MIN_DISTANCE = 1,
	RAVEN_AI_PARAM_ATTACK_DELAY = 2,
	RAVEN_AI_PARAM_ATTACK_CHANCE_PCT = 3,
	RAVEN_AI_PARAM_MAX_TARGET_DISTANCE = 4,
};

//D2Game.0x6FCED140
void __fastcall D2GAME_AI_Unk107_6FCED140(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	pAiTickParam->pAiControl->dwAiParam[0] = -1;
	pAiTickParam->pAiControl->dwAiParam[1] = (uint32_t)(AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_ATTACK_DELAY) + 1);
	pAiTickParam->pAiControl->dwAiParam[2] = ITEMS_RollRandomNumber(&pUnit->pSeed) & 1;
}

//D2Game.0x6FCED190
void __fastcall AITHINK_Fn107_Raven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (!pAiTickParam->pMonstatsTxt)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[0] == -1)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 3;

		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
		{
			D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pOwner, pAiTickParam->pMonstatsTxt->nSkill[0]);
			const int32_t nSkillLevel = SKILLS_GetSkillLevel(pOwner, pSkill, 1);
			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);
			if (pSkillsTxtRecord)
			{
				if (nSkillLevel > 0)
				{
					pAiTickParam->pAiControl->dwAiParam[0] = pSkillsTxtRecord->dwParam[4] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[5];
				}
				else
				{
					pAiTickParam->pAiControl->dwAiParam[0] = 0;
				}
			}
			else
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
			}
		}
	}
	else if (pAiTickParam->pAiControl->dwAiParam[0] == 0)
	{
		SUNITDMG_KillMonster(pGame, pUnit, pUnit, 1);
		return;
	}

	const int32_t nOwnerDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);
	if (nOwnerDistance > 50)
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, 0);
		return;
	}

	int32_t nVel = 0;
	if (pAiTickParam->pMonstatsTxt->nVelocity <= 0)
	{
		nVel = 100;
	}
	else
	{
		nVel = std::min(100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity - 100, 100);
	}

	if (nOwnerDistance > 28)
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, nVel, 0);
		return;
	}

	// TODO: Unused Calculation?
	int32_t nDist = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget = sub_6FCF2110(pGame, pOwner, pAiTickParam->pAiControl, &nDist, &bCombat);
	if (pTarget)
	{
		nDist = UNITS_GetDistanceToOtherUnit(pUnit, pTarget);
	}
	///////////////////////////

	const int32_t nAverageDistance = (AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_CIRCLE_OWNER_MIN_DISTANCE) + AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_CIRCLE_OWNER_MAX_DISTANCE)) / 2;

	if (pAiTickParam->pTarget
		&& pAiTickParam->pAiControl->dwAiParam[1] < pGame->dwGameFrame
		&& AIRollChanceParam(pGame, pUnit, pAiTickParam, RAVEN_AI_PARAM_ATTACK_CHANCE_PCT)
		&& pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_MAX_TARGET_DISTANCE))
	{
		if (pAiTickParam->bCombat)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			pAiTickParam->pAiControl->dwAiParam[1] = pGame->dwGameFrame + 10 * AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_ATTACK_DELAY);
			--pAiTickParam->pAiControl->dwAiParam[0];
		}
		else
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
		}
		return;
	}

	if (nOwnerDistance < AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_CIRCLE_OWNER_MIN_DISTANCE) 
		|| nOwnerDistance > AI_GetParamValue(pGame, pAiTickParam, RAVEN_AI_PARAM_CIRCLE_OWNER_MAX_DISTANCE))
	{
		if (!AITACTICS_WalkAroundTargetWithScaledDistance(pGame, pUnit, pAiTickParam->pAiControl, pOwner, nAverageDistance))
		{
			D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 1, 0, 0, 0);
		}
		return;
	}

	if (!pAiTickParam->pAiControl->dwAiParam[2])
	{
		AITACTICS_SetVelocity(pUnit, 6, 0, 4u);
	}
	else
	{
		AITACTICS_SetVelocity(pUnit, 5, 0, 4u);
	}

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_WALK, &modeChange);
	PATH_SetStepNum(pUnit->pDynamicPath, 1u);
	modeChange.pTargetUnit = pOwner;

	if (!D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1))
	{
		pAiTickParam->pAiControl->dwAiParam[2] = (pAiTickParam->pAiControl->dwAiParam[2] == 0);

		if (!pAiTickParam->pAiControl->dwAiParam[2])
		{
			AITACTICS_SetVelocity(pUnit, 6, 0, 4u);
		}
		else
		{
			AITACTICS_SetVelocity(pUnit, 5, 0, 4u);
		}

		MONSTERMODE_GetModeChangeInfo(pUnit, 2, &modeChange);
		PATH_SetStepNum(pUnit->pDynamicPath, 1u);
		modeChange.pTargetUnit = pOwner;

		if (!D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1) && !AITACTICS_WalkAroundTargetWithScaledDistance(pGame, pUnit, pAiTickParam->pAiControl, pOwner, nAverageDistance))
		{
			D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 1, 0, 0, 0);
		}
	}
}

enum D2C_DruidWolfAIParams
{
	DRUIDWOLF_AI_PARAM_STALL_DURATION = 0,
	DRUIDWOLF_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	DRUIDWOLF_FENRIS_AI_PARAM_EAT_CORPSE_CHANCE_PCT = 2,
	DRUIDWOLF_SPIRITWOLF_AI_PARAM_FOLLOW_OWNER_DISTANCE = 2,
	DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE = 3,
	DRUIDWOLF_AI_PARAM_MAX_OWNER_DISTANCE = 4,
};

//Inlined in D2Game.0x6FCED540
void __fastcall AITHINK_Fn108_Fenris(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		const int32_t nOwnerGUID = pAiTickParam->pAiControl->dwAiParam[2];
		if (nOwnerGUID)
		{
			pOwner = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOwnerGUID);

			if (pOwner && pAiTickParam->pMonstatsTxt->nSkill[1] >= 0)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], 0, CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
				EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + 6, 0, 0);
				AITACTICS_Idle(pGame, pUnit, 8);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = pOwner->dwUnitId;

	if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
	{
		if (!sub_6FCE34E0(pGame, pUnit, 0, pOwner, 0, pAiTickParam, 0, 6))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 33);
		}

		return;
	}

	int32_t nTemp = 0;
	int32_t bCombat = 0;
	int32_t nDistance = 0;
	D2UnitStrc* pTarget = sub_6FCCFD40(pGame, pUnit, pAiTickParam->pAiControl, &nTemp, &bCombat, AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE));
	D2UnitStrc* pPotentialTarget = sub_6FCF2110(pGame, pOwner, pAiTickParam->pAiControl, &nDistance, &nTemp);
	if (pTarget)
	{
		if (!sub_6FCF14D0(pUnit, pTarget))
		{
			pTarget = nullptr;

			if (pPotentialTarget && nDistance < AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE) && sub_6FCF14D0(pUnit, pPotentialTarget))
			{
				pTarget = pPotentialTarget;
			}
		}
	}
	else
	{
		if (pPotentialTarget && nDistance < AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE) && sub_6FCF14D0(pUnit, pPotentialTarget))
		{
			pTarget = pPotentialTarget;
		}
	}

	const int32_t nDistanceToOwner = UNITS_GetDistanceToOtherUnit(pOwner, pUnit);
	if (pTarget && nDistanceToOwner > AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE) && UNITS_GetDistanceToOtherUnit(pOwner, pTarget) > AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE))
	{
		pTarget = nullptr;
	}

	int32_t nVel = 0;
	if (pAiTickParam->pMonstatsTxt->nVelocity <= 0)
	{
		nVel = 100;
	}
	else
	{
		nVel = std::min(100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity - 100, 100);
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[1] >= 0 && nDistanceToOwner > 50)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], 0, CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
		EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + 2, 0, 0);
		AITACTICS_Idle(pGame, pUnit, 10);
		return;
	}

	if (nDistanceToOwner > AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_OWNER_DISTANCE))
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 1, 100, 0);
		return;
	}

	if (nDistanceToOwner > AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE))
	{
		if (pOwner->dwAnimMode == PLRMODE_RUN)
		{
			D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 1, 100, 0);
			return;
		}

		if (pOwner->dwAnimMode == PLRMODE_WALK || pOwner->dwAnimMode == PLRMODE_TOWNWALK)
		{
			D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 0, 0);
			return;
		}
	}

	if (sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, bCombat, pAiTickParam, 1, 6))
	{
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && !STATES_CheckState(pUnit, STATE_FENRIS_RAGE)
		&& (AIRollChanceParam(pGame, pUnit, pAiTickParam, DRUIDWOLF_FENRIS_AI_PARAM_EAT_CORPSE_CHANCE_PCT) || !pTarget || pAiTickParam->pAiControl->dwAiParam[0]))
	{
		D2UnitStrc* pFoundUnit = SKILLS_FindUseableCorpse(pGame, pUnit, pOwner, 10);
		if (pFoundUnit && UNITS_GetDistanceToOtherUnit(pFoundUnit, pUnit) < AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE) / 2)
		{
			if (UNITS_IsInMeleeRange(pUnit, pFoundUnit, 0))
			{
				pAiTickParam->pAiControl->dwAiParam[0] = 0;
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pFoundUnit, 0, 0);
				return;
			}

			if (!bCombat)
			{
				AITACTICS_RunToTargetUnit(pGame, pUnit, pFoundUnit);
				pAiTickParam->pAiControl->dwAiParam[0] = 1;
				pAiTickParam->pAiControl->dwAiParam[1] = pFoundUnit->dwUnitId;
				return;
			}

			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
			AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_STALL_DURATION));
			return;
		}
	}
	else
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
	}

	if (!bCombat)
	{
		if (pTarget)
		{
			AITACTICS_SetVelocity(pUnit, 0, nVel, 0);
			AITACTICS_RunToTargetUnit(pGame, pUnit, pTarget);
		}
		else
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DRUIDWOLF_AI_PARAM_APPROACH_CHANCE_PCT))
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 10u);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
		}
		return;
	}

	AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
	AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_STALL_DURATION));
}

//Inlined in D2Game.0x6FCED540
void __fastcall AITHINK_Fn108_SpiritWolf(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		const int32_t nOwnerGUID = pAiTickParam->pAiControl->dwAiParam[2];
		if (nOwnerGUID)
		{
			pOwner = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOwnerGUID);

			if (pOwner && pAiTickParam->pMonstatsTxt->nSkill[0] >= 0)
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
				EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + 6, 0, 0);
				AITACTICS_Idle(pGame, pUnit, 8);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = pOwner->dwUnitId;

	if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
	{
		if (!sub_6FCE34E0(pGame, pUnit, 0, pOwner, 0, pAiTickParam, 0, 6))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 33);
		}

		return;
	}

	int32_t nTemp = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget = sub_6FCCFD40(pGame, pUnit, pAiTickParam->pAiControl, &nTemp, &bCombat, AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE));
	D2UnitStrc* pPotentialTarget = sub_6FCF2110(pGame, pOwner, pAiTickParam->pAiControl, &nTemp, &nTemp);
	if (pTarget)
	{
		if (!sub_6FCF14D0(pUnit, pTarget))
		{
			pTarget = nullptr;
			if (pPotentialTarget && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPotentialTarget) < AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE) && !sub_6FCF14D0(pUnit, pPotentialTarget))
			{
				pTarget = pPotentialTarget;
			}
		}
	}
	else
	{
		if (pPotentialTarget && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPotentialTarget) < AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE) && !sub_6FCF14D0(pUnit, pPotentialTarget))
		{
			pTarget = pPotentialTarget;
		}
	}

	int32_t nVel = 0;
	if (pAiTickParam->pMonstatsTxt->nVelocity <= 0)
	{
		nVel = 100;
	}
	else
	{
		nVel = std::min(100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity - 100, 100);
	}

	const int32_t nDistanceToOwner = UNITS_GetDistanceToOtherUnit(pOwner, pUnit);

	if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && nDistanceToOwner > 50)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
		EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + 4, 0, 0);
		AITACTICS_Idle(pGame, pUnit, 10);
		return;
	}

	if (nDistanceToOwner > AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_OWNER_DISTANCE))
	{
		if (D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 1, 100, 0))
		{
			return;
		}
	}

	if (nDistanceToOwner > AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_SPIRITWOLF_AI_PARAM_FOLLOW_OWNER_DISTANCE))
	{
		if (pOwner->dwAnimMode == PLRMODE_WALK || pOwner->dwAnimMode == PLRMODE_TOWNWALK)
		{
			if (D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 0, 0))
			{
				return;
			}
		}

		if (pOwner->dwAnimMode == PLRMODE_RUN && D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 1, 100, 0))
		{
			return;
		}
	}

	if (!sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, bCombat, pAiTickParam, 1, 6))
	{
		if (pTarget)
		{
			if (pAiTickParam->pMonstatsTxt)
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
				AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_STALL_DURATION));
			}
			else
			{
				if (UNITS_GetDistanceToOtherUnit(pOwner, pTarget) >= AI_GetParamValue(pGame, pAiTickParam, DRUIDWOLF_AI_PARAM_MAX_TARGET_DISTANCE))
				{
					if (nDistanceToOwner <= 10)
					{
						AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
					}
					else
					{
						AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pOwner, 8, 6);
					}
				}
				else
				{
					AITACTICS_SetVelocity(pUnit, 0, nVel, 0);
					AITACTICS_RunToTargetUnit(pGame, pUnit, pTarget);
				}
			}
		}
		else
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DRUIDWOLF_AI_PARAM_APPROACH_CHANCE_PCT))
			{
				AITACTICS_WalkCloseToUnit(pGame, pUnit, 10u);
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
		}
	}
}

//D2Game.0x6FCED540
void __fastcall AITHINK_Fn108_DruidWolf(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pUnit)
	{
		return;
	}

	if (pUnit->dwClassId == MONSTER_SPIRITWOLF)
	{
		return AITHINK_Fn108_SpiritWolf(pGame, pUnit, pAiTickParam);
	}
	else
	{
		return AITHINK_Fn108_Fenris(pGame, pUnit, pAiTickParam);
	}
}

enum D2C_DruidBearAIParams
{
	DRUIDBEAR_AI_PARAM_STALL_DURATION = 0,
	DRUIDBEAR_AI_PARAM_RUN_CHANCE_PCT = 1,
	DRUIDBEAR_AI_PARAM_SKILL_OR_ATTACK_CHANCE_PCT = 2,
};

//D2Game.0x6FCEDF70
void __fastcall AITHINK_Fn112_DruidBear(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		if (pAiTickParam->pAiControl->dwAiParam[2])
		{
			D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pAiTickParam->pAiControl->dwAiParam[2]);
			if (pPlayer)
			{
				D2GAME_AI_PetMove_6FCE2BA0(pGame, pPlayer, pUnit, 3, 0, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	pAiTickParam->pAiControl->dwAiParam[2] = pOwner->dwUnitId;

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pOwner);

	if (nDistance > 50)
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 3, 0, 0, 0);
		return;
	}

	int32_t nVel = 0;
	if (pAiTickParam->pMonstatsTxt->nVelocity <= 0)
	{
		nVel = 100;
	}
	else
	{
		nVel = std::min(100 * pAiTickParam->pMonstatsTxt->nRun / pAiTickParam->pMonstatsTxt->nVelocity - 100, 100);
	}

	if (nDistance > 28)
	{
		D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 100, 0);
		return;
	}

	if (nDistance > 18)
	{
		if (pOwner->dwAnimMode == 2 || pOwner->dwAnimMode == 6)
		{
			if (D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 0, 0))
			{
				return;
			}
		}

		if (pOwner->dwAnimMode == 3 && D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 100, 0))
		{
			return;
		}
	}

	int32_t nTemp1 = 0;
	int32_t nTemp2 = 0;
	int32_t nTemp3 = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pPotentialTarget = sub_6FCF2110(pGame, pOwner, pAiTickParam->pAiControl, &nTemp1, &nTemp2);
	D2UnitStrc* pTarget = sub_6FCCFD40(pGame, pUnit, pAiTickParam->pAiControl, &nTemp3, &bCombat, 28);

	if (!pTarget)
	{
		if (pPotentialTarget && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPotentialTarget) < 28 && sub_6FCF14D0(pUnit, pPotentialTarget))
		{
			pTarget = pPotentialTarget;
		}
	}
	else if (!sub_6FCF14D0(pUnit, pTarget))
	{
		pTarget = nullptr;
		if (pPotentialTarget && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pPotentialTarget) < 28 && sub_6FCF14D0(pUnit, pPotentialTarget))
		{
			pTarget = pPotentialTarget;
		}
	}

	if (bCombat)
	{
		if (pTarget)
		{
			if (!AIRollChanceParam(pGame, pUnit, pAiTickParam, DRUIDBEAR_AI_PARAM_SKILL_OR_ATTACK_CHANCE_PCT))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
				AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, DRUIDBEAR_AI_PARAM_STALL_DURATION));
			}
			else
			{
				AITACTICS_UseSequenceSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
			}
		}
		else
		{
			if (nDistance <= 16)
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
			else
			{
				D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 0, 0);
			}
		}
		return;
	}

	if (!pTarget)
	{
		if (nDistance <= 16)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		else
		{
			D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 0, 0);
		}
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DRUIDBEAR_AI_PARAM_RUN_CHANCE_PCT))
	{
		AITACTICS_SetVelocity(pUnit, 0, nVel, 40);
	}

	AITACTICS_WalkToTargetUnit(pGame, pUnit, pTarget);
}

enum D2C_TotemAIParams
{
	TOTEM_AI_PARAM_WALK_AWAY_CHANCE_PCT = 0,
	TOTEM_AI_PARAM_IGNORE_TARGET_CHANCE_PCT = 1,
	TOTEM_AI_PARAM_TELEPORT_TO_OWNER_DISTANCE = 2,
	TOTEM_AI_PARAM_FOLLOW_OWNER_DISTANCE = 3,
};

//D2Game.0x6FCEE250
void __fastcall AITHINK_Fn109_Totem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	int32_t nDistance = 0;
	int32_t bCombat = 0;
	D2UnitStrc* pTarget = sub_6FCCFD40(pGame, pUnit, pAiTickParam->pAiControl, &nDistance, &bCombat, 24);
	if (bCombat && pTarget && (AIRollChanceParam(pGame, pUnit, pAiTickParam, TOTEM_AI_PARAM_WALK_AWAY_CHANCE_PCT)) && D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pTarget, 6u, 1))
	{
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, TOTEM_AI_PARAM_IGNORE_TARGET_CHANCE_PCT))
	{
		pTarget = nullptr;
		bCombat = 0;
	}

	nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pOwner);

	if (nDistance > AI_GetParamValue(pGame, pAiTickParam, TOTEM_AI_PARAM_TELEPORT_TO_OWNER_DISTANCE))
	{
		if (sub_6FCBDFE0(pGame, pUnit, UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner), 0, 0))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
			return;
		}
	}

	if (nDistance > AI_GetParamValue(pGame, pAiTickParam, TOTEM_AI_PARAM_FOLLOW_OWNER_DISTANCE))
	{
		const int32_t nOwnerAnimMode = pOwner->dwAnimMode;
		if (((nOwnerAnimMode == PLRMODE_WALK || nOwnerAnimMode == PLRMODE_TOWNWALK) && D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 0, 0))
			|| (nOwnerAnimMode == PLRMODE_RUN && D2GAME_AI_PetMove_6FCE2BA0(pGame, pOwner, pUnit, 0, 0, 60, 0)))
		{
			return;
		}
	}
	if(sub_6FCE34E0(pGame, pUnit, pTarget, pOwner, bCombat, pAiTickParam, 0, 6))
	{
		return;
	}
	AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
}

enum D2C_NpcBarbAIParams
{
	NPCBARB_AI_PARAM_STALL_DURATION = 0,
	NPCBARB_AI_PARAM_RUN_TO_TARGET_CHANCE_PCT = 1,
	NPCBARB_AI_PARAM_RUN_TO_TARGET_MAX_DISTANCE = 2,
};

//D2Game.0x6FCEE450
void __fastcall AITHINK_Fn127_NpcBarb(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pUnit->dwNodeIndex == 11)
	{
		sub_6FC40280(pGame, pUnit, 0, 8);
	}

	if (pAiTickParam->pTarget)
	{
		if (pAiTickParam->bCombat)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, NPCBARB_AI_PARAM_STALL_DURATION));
			return;
		}

		if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, NPCBARB_AI_PARAM_RUN_TO_TARGET_MAX_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, NPCBARB_AI_PARAM_RUN_TO_TARGET_CHANCE_PCT))
		{
			AITACTICS_SetVelocity(pUnit, 0, 100, 0);
			AITACTICS_RunToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
			return;
		}
	}

	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	if (!AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed) % 20 + nX - 40, ITEMS_RollRandomNumber(&pUnit->pSeed) % 20 + nY - 10))
	{
		const int32_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed) & 1;

		if (!AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed) % 20 + nX - 10, ITEMS_RollRandomNumber(&pUnit->pSeed) % 20 - 30 * (2 * (nRand != 0) - 1) + nY - 10))
		{
			if (!AITACTICS_WalkToTargetCoordinatesDeleteAiEvent(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed) % 20 + nX - 10, ITEMS_RollRandomNumber(&pUnit->pSeed) % 20 - 30 * (2 * (nRand == 0) - 1) + nY - 10))
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
			}
		}
	}
}

enum D2C_CatapultSpotterbAIParams
{
	CATAPULTSPOTTER_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	CATAPULTSPOTTER_AI_PARAM_DELAY = 1,
	CATAPULTSPOTTER_AI_PARAM_RANGE = 2, // Unused
	CATAPULTSPOTTER_AI_PARAM_RANDOM_RANGE = 3,
	CATAPULTSPOTTER_AI_PARAM_SHOTS_PER_KILL = 4,
};

//D2Game.0x6FCEE6F0
void __fastcall AITHINK_Fn126_CatapultSpotter(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	constexpr int32_t nCatapultSkillIds[5] = { SKILL_CATAPULTCHARGEDBALL, SKILL_CATAPULTSPIKEBALL, SKILL_CATAPULTBLIZZARD, SKILL_CATAPULTPLAGUE, SKILL_CATAPULTMETEOR };

	if (!pAiTickParam->pAiControl->dwAiParam[2] && AI_RollPercentage(pUnit) < 3)
	{
		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
		D2_ASSERT(pRoom);

		D2DrlgCoordsStrc drlgCoords = {};
		DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

		int32_t nX = drlgCoords.nSubtileX;
		int32_t nY = drlgCoords.nSubtileY;

		int32_t nXOffset = 0;
		int32_t nYOffset = 0;
		D2Common_11066(pUnit, &nXOffset, &nYOffset);

		nXOffset = -nXOffset;
		nYOffset = -nYOffset;

		int32_t nClassId = -20;
		if (pUnit)
		{
			nClassId = pUnit->dwClassId - 19;
		}

		for (int32_t i = 0; i < 3; ++i)
		{
			nX += nXOffset * drlgCoords.nSubtileWidth;
			nY += nYOffset * drlgCoords.nSubtileHeight;

			if (SUNITINACTIVE_IsUnitInsideRoom(pGame, pRoom, nX, nY, nClassId))
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_DEATH, pAiTickParam->pTarget);
				return;
			}
		}
	}

	if (!pAiTickParam->pAiControl->dwAiParam[2])
	{
		pAiTickParam->pAiControl->dwAiParam[2] = 1;
	}

	const int16_t nDelayParam = AI_GetParamValue(pGame, pAiTickParam, CATAPULTSPOTTER_AI_PARAM_DELAY);
	if ((pAiTickParam->pAiControl->dwAiParam[2] + nDelayParam <= pGame->dwGameFrame) 
		&& pAiTickParam->pTarget && AIRollChanceParam(pGame, pUnit, pAiTickParam, CATAPULTSPOTTER_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		if (pAiTickParam->pAiControl->dwAiParam[1] > 0)
		{
			--pAiTickParam->pAiControl->dwAiParam[1];
		}
		else
		{
			pAiTickParam->pAiControl->dwAiParam[0] = ITEMS_RollRandomNumber(&pUnit->pSeed) % 5;
			pAiTickParam->pAiControl->dwAiParam[1] = AI_GetParamValue(pGame, pAiTickParam, CATAPULTSPOTTER_AI_PARAM_SHOTS_PER_KILL);
		}

		const int16_t nRandomRangeParam = AI_GetParamValue(pGame, pAiTickParam, CATAPULTSPOTTER_AI_PARAM_RANDOM_RANGE);
		D2CoordStrc coords = {};
		coords.nX = CLIENTS_GetUnitX(pAiTickParam->pTarget) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRandomRangeParam) - nRandomRangeParam;
		coords.nY = CLIENTS_GetUnitY(pAiTickParam->pTarget) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRandomRangeParam) - nRandomRangeParam;

		if (COLLISION_GetFreeCoordinatesEx(UNITS_GetRoom(pUnit), &coords, 2, 2053, 3))
		{
			AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK1, nCatapultSkillIds[pAiTickParam->pAiControl->dwAiParam[0]], 0, coords.nX, coords.nY);
			pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame;
			return;
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, nDelayParam);
	}
}

enum D2C_DeathMaulerAIParams
{
	DEATHMAULER_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	DEATHMAULER_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	DEATHMAULER_AI_PARAM_SHOOT_RANGE = 2,
	DEATHMAULER_AI_PARAM_SHOOT_CHANCE_PCT = 3,
};

//D2Game.0x6FCEEAD0
void __fastcall AITHINK_Fn130_DeathMauler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DEATHMAULER_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			return;
		}
	}
	else
	{
		if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0
			&& pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, DEATHMAULER_AI_PARAM_SHOOT_RANGE)
			&& AIRollChanceParam(pGame, pUnit, pAiTickParam, DEATHMAULER_AI_PARAM_SHOOT_CHANCE_PCT))
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			return;
		}

		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, DEATHMAULER_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pAiTickParam->pTarget, 0);
			return;
		}
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
}

//D2Game.0x6FCEEC00
void __fastcall AITHINK_Fn131_Wussie(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pPortal = nullptr;

	if (!ACT5Q2_FindPortal(pGame, pUnit, &pPortal))
	{
		int32_t bCloseToTarget = 0;
		D2UnitStrc* pPlayer = sub_6FCCFD70(pGame, pUnit, &bCloseToTarget);
		if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER)
		{
			if (ACT5Q2_IsPrisonDoorDead(pGame, pPlayer, pUnit))
			{
				ACT5Q2_UpdateQuestState(pGame, pPlayer, pUnit);

				if (pUnit->dwNodeIndex == 11)
				{
					sub_6FC40280(pGame, pUnit, 0, 8);
				}
			}
			else
			{
				ACT5Q2_OnWussieActivated(pGame, pPlayer);

				if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 1000) < 100)
				{
					D2GSPacketSrv8A packet8A = {};
					packet8A.nHeader = 0x8Au;
					packet8A.nUnitType = UNIT_MONSTER;
					if (pUnit)
					{
						packet8A.dwUnitGUID = pUnit->dwUnitId;
					}
					else
					{
						packet8A.dwUnitGUID = -1;
					}

					D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), &packet8A, sizeof(packet8A));
				}
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (!pPortal)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (ACT5Q2_CheckWussieCounters(pGame, pUnit) || (uint32_t)AIUTIL_GetDistanceToCoordinates(pUnit, CLIENTS_GetUnitX(pPortal), CLIENTS_GetUnitX(pPortal)) < 4)
	{
		ACT5Q2_UpdateWussieCounters(pGame, 0, pUnit);
		D2Common_10153(pUnit->pDynamicPath);
		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
		DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pUnit), pUnit->dwUnitType, pUnit->dwUnitId);
		UNITS_FreeCollisionPath(pUnit);
		UNITROOM_RemoveUnitFromRoom(pUnit);
		SUNIT_RemoveUnit(pGame, pUnit);
		return;
	}

	if (AI_RollPercentage(pUnit) < 70)
	{
		AITACTICS_WalkToTargetUnitWithFlags(pGame, pUnit, pPortal, 7);
	}
	else if (AI_RollPercentage(pUnit) < 10)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
	}
}

enum D2C_NihlathakAIParams
{
	NIHLATAK_AI_PARAM_SKILL0_CHANCE_PCT = 0,
	NIHLATAK_AI_PARAM_SKILL_RANGE = 1,
	NIHLATAK_AI_PARAM_SKILL2_CHANCE_PCT = 2,
	NIHLATAK_AI_PARAM_SKILL1_CHANCE_PCT = 3,
	NIHLATAK_AI_PARAM_WALK_AWAY_DISTANCE = 4,
};

//D2Game.0x6FCEEE60
void __fastcall AITHINK_Fn128_Nihlathak(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	ACT5Q4_OnNihlathakActivated(pGame);

	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	if (!pAiTickParam->pTarget)
	{
		D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]);
		if (pSkill && sub_6FCF2E70(pUnit))
		{
			AITACTICS_UseSkillInRange(pUnit, AI_GetParamValue(pGame, pAiTickParam, NIHLATAK_AI_PARAM_SKILL_RANGE), SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__), SKILLS_GetSkillMode(pSkill));
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		}
		return;
	}

	D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]);
	if (pSkill && pAiTickParam->bCombat && AIRollChanceParam(pGame, pUnit, pAiTickParam, NIHLATAK_AI_PARAM_SKILL0_CHANCE_PCT))
	{
		AITACTICS_UseSkillInRange(pUnit, AI_GetParamValue(pGame, pAiTickParam, NIHLATAK_AI_PARAM_SKILL_RANGE), SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__), SKILLS_GetSkillMode(pSkill));
		return;
	}
	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, NIHLATAK_AI_PARAM_WALK_AWAY_DISTANCE) && (AI_RollPercentage(pUnit) < 40) && D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, 5u, 1))
	{
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[2] > 0)
	{
		pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[2]);
		if (pSkill && (AIRollChanceParam(pGame, pUnit, pAiTickParam, NIHLATAK_AI_PARAM_SKILL2_CHANCE_PCT)))
		{
			D2UnitStrc* pTarget = sub_6FD15210(pUnit, pAiTickParam->pTarget, pAiTickParam->pMonstatsTxt->nSkill[2], SKILLS_GetSkillLevel(pUnit, pSkill, 1));
			if (pTarget && AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[2], pAiTickParam->pMonstatsTxt->nSkill[2], pTarget, 0, 0))
			{
				return;
			}
		}
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[3] > 0 && SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[3]) && AI_RollPercentage(pUnit) < 60 && pAiTickParam->nTargetDistance < 14)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[1] > 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, NIHLATAK_AI_PARAM_SKILL1_CHANCE_PCT) && AIUTIL_CheckIfMonsterUsesSkill(pUnit, pAiTickParam->pMonstatsTxt->nSkill[1]))
	{
		D2NihlathakOverseerAiCallbackArgStrc arg = {};
		arg.nDistantMaxDistance = 625;
		arg.nMaxLifePercentage = 50;
		sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_Overseer_Nihlathak, 1);

		if (arg.pDistantTarget)
		{
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[1], pAiTickParam->pMonstatsTxt->nSkill[1], arg.pDistantTarget, 0, 0);
			return;
		}

		if (pAiTickParam->pMonstatsTxt->nSkill[4] <= 0)
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
			return;
		}

		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

		if (sub_6FC68350(MONSTER_EVILHUT, pRoom, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0))
		{
			pAiTickParam->pAiControl->nMinionSpawnClassId = D2Common_11063(pRoom, MONSTER_MINION1);
			AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[4], pAiTickParam->pMonstatsTxt->nSkill[4], pAiTickParam->pTarget, 0, 0);
		}
		else
		{
			AITACTICS_WalkCloseToUnit(pGame, pUnit, 6u);
		}
		return;
	}

	if (pAiTickParam->pMonstatsTxt->nSkill[3] > 0 && SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[3]) && pAiTickParam->nTargetDistance < 14)
	{
		AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[3], pAiTickParam->pMonstatsTxt->nSkill[3], pAiTickParam->pTarget, 0, 0);
		return;
	}

	if (AI_RollPercentage(pUnit) < 60)
	{
		AITACTICS_WalkToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
}

//D2Game.0x6FCEF330
void __fastcall AITHINK_Fn132_AncientStatue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!ACT5Q5_IsActivatable(pGame) || ACT5Q5_GetPortalCount(pGame) || STATES_CheckState(pUnit, STATE_INVIS))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pUnit), 0);
	AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK1, SKILL_MINIONSPAWNER, 0, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
}

// Talic
enum D2C_AncientBarb1AIParams
{
	ANCIENTBARB1_AI_PARAM_WHIRLWIND_DISTANCE = 0,
	ANCIENTBARB1_AI_PARAM_WHIRLWIND_CHANCE_PCT = 1,
	ANCIENTBARB1_AI_PARAM_ATTACK_CHANCE_PCT = 2,
	ANCIENTBARB1_AI_PARAM_SKILL_REACH = 3,
};

//D2Game.0x6FCEF3F0
void __fastcall AITHINK_AncientBarb1SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (ACT5Q5_IsNotActivatable(pGame) || STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	D2UnitStrc* pTarget = pAiTickParam->pTarget;
	if (!pTarget)
	{
		int32_t nTemp1 = 0;
		int32_t nTemp2 = 0;
		pTarget = AITHINK_GetTargetForBoss(pGame, pUnit, &nTemp1, &nTemp2, 0, AITHINK_AreUnitsInSameLevel);
		if (!pTarget)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
			return;
		}
	}

	const int32_t bInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);
	const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pTarget);

	if (pAiTickParam->pMonstatsTxt->nSkill[0] > 0 && AIUTIL_CheckIfMonsterUsesSkill(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]) 
		&& nDistance < AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB1_AI_PARAM_WHIRLWIND_DISTANCE) && AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB1_AI_PARAM_WHIRLWIND_CHANCE_PCT))
	{
		const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
		const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);
		const int32_t nXDiff = nTargetX - CLIENTS_GetUnitX(pUnit);
		const int32_t nYDiff = nTargetY - CLIENTS_GetUnitY(pUnit);

		int32_t nScale = UNITS_GetDistanceToCoordinates(pUnit, nTargetX, nTargetY);
		if (!nScale)
		{
			nScale = 1;
		}

		const int32_t nOffsetX = nXDiff * AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB1_AI_PARAM_SKILL_REACH) / nScale;
		const int32_t nOffsetY = nYDiff * AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB1_AI_PARAM_SKILL_REACH) / nScale;

		D2ModeChangeStrc modeChange = {};
		MONSTERMODE_GetModeChangeInfo(pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], &modeChange);
		UNITS_SetUsedSkill(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]));
		PATH_SetStepNum(pUnit->pDynamicPath, 1u);
		modeChange.unk0x14[1] = 100;
		modeChange.nX = nOffsetX + nTargetX;
		modeChange.nY = nOffsetY + nTargetY;
		D2GAME_ModeChange_6FC65220(pGame, &modeChange, 0);
	}
	else
	{
		if (bInMeleeRange)
		{
			if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB1_AI_PARAM_ATTACK_CHANCE_PCT))
			{
				if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
				}
				else
				{
					AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pTarget);
				}
			}
			else
			{
				AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
			}
		}
		else
		{
			AITACTICS_WalkToTargetUnit(pGame, pUnit, pTarget);
			AITACTICS_Idle(pGame, pUnit, 10);
		}
		return;
	}

}

//D2Game.0x6FCEF730
int32_t __fastcall AITHINK_AreUnitsInSameLevel(D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
	D2_ASSERT(pUnit);
	D2_ASSERT(pTarget);

	return DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit)) == DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pTarget));
}

// Madawc
enum D2C_AncientBarb2AIParams
{
	ANCIENTBARB2_AI_PARAM_ATTACK_DISTANCE = 0,
	ANCIENTBARB2_AI_PARAM_ATTACK_CHANCE_PCT = 1,
	ANCIENTBARB2_AI_PARAM_SHOUT_CHANCE_PCT = 2,
	ANCIENTBARB2_AI_PARAM_WALK_AWAY_CHANCE_PCT = 3,
	ANCIENTBARB2_AI_PARAM_WALK_AWAY_MAX_DISTANCE = 4,
};

//Inlined in D2Game.0x6FCEFBC0
void __fastcall AITHINK_AncientBarb2SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (ACT5Q5_IsNotActivatable(pGame))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	int32_t nTemp1 = 0;
	int32_t nTemp2 = 0;
	D2UnitStrc* pTarget = AITHINK_GetTargetForBoss(pGame, pUnit, &nTemp1, &nTemp2, 0, AITHINK_AreUnitsInSameLevel);

	if (!pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	const int32_t bInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);
	const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pTarget);
	if (bInMeleeRange)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB2_AI_PARAM_WALK_AWAY_CHANCE_PCT))
		{
			if (D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pTarget, AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB2_AI_PARAM_WALK_AWAY_MAX_DISTANCE), 1))
			{
				AITACTICS_Idle(pGame, pUnit, 25);
				return;
			}
		}
	}

	D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);
	if (pSkillsTxtRecord)
	{
		if (pSkillsTxtRecord->nAuraState <= 0 || pSkillsTxtRecord->nAuraState > sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
		{
			if (pSkillsTxtRecord->wAuraTargetState <= 0 || pSkillsTxtRecord->wAuraTargetState > sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pTarget, pSkillsTxtRecord->wAuraTargetState))
			{
				if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB2_AI_PARAM_SHOUT_CHANCE_PCT) && AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], 0, 0, 0))
				{
					return;
				}
			}
		}
	}

	if (nDistance < AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB2_AI_PARAM_ATTACK_DISTANCE))
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB2_AI_PARAM_ATTACK_CHANCE_PCT) && AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget))
		{
			return;
		}
	}
	
	if (AITACTICS_WalkToTargetUnitWithSteps(pGame, pUnit, pTarget, nDistance - AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB2_AI_PARAM_ATTACK_DISTANCE)))
	{
		return;
	}
	
	AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
}

// Korlic
enum D2C_AncientBarb3AIParams
{
	ANCIENTBARB3_AI_PARAM_LEAP_DISTANCE = 0,
	ANCIENTBARB3_AI_PARAM_LEAP_CHANCE_PCT = 1,
	ANCIENTBARB3_AI_PARAM_ATTACK_CHANCE_PCT = 2,
};

//D2Game.0x6FCEFA10
void __fastcall AITHINK_AncientBarb3SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (ACT5Q5_IsNotActivatable(pGame))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	int32_t nTemp1 = 0;
	int32_t nTemp2 = 0;
	D2UnitStrc* pTarget = AITHINK_GetTargetForBoss(pGame, pUnit, &nTemp1, &nTemp2, 0, AITHINK_AreUnitsInSameLevel);
	if (!pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	const int32_t bIsInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);
	if (pAiTickParam->pMonstatsTxt->nSkill[0] > 0
		&& UNITS_GetDistanceToOtherUnit(pUnit, pTarget) < AI_GetParamValue(pGame, pAiTickParam, ANCIENTBARB3_AI_PARAM_LEAP_DISTANCE)
		&& AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB3_AI_PARAM_LEAP_CHANCE_PCT)
		&& AIUTIL_CheckIfMonsterUsesSkill(pUnit, pAiTickParam->pMonstatsTxt->nSkill[0]))
	{
		AITACTICS_UseSequenceSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkill[0], pTarget, 0, 0);
		return;
	}

	if (!bIsInMeleeRange)
	{
		AITACTICS_WalkToTargetUnit(pGame, pUnit, pTarget);
		AITACTICS_Idle(pGame, pUnit, 10);
		return;
	}

	if (AIRollChanceParam(pGame, pUnit, pAiTickParam, ANCIENTBARB3_AI_PARAM_ATTACK_CHANCE_PCT))
	{
		if (ITEMS_RollRandomNumber(&pUnit->pSeed) & 1)
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pTarget);
		}
		else
		{
			AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pTarget);
		}
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
	}
}

//D2Game.0x6FCEFBC0
void __fastcall AITHINK_Fn133_Ancient(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pUnit)
	{
		return;
	}

	switch (pUnit->dwClassId)
	{
	case MONSTER_ANCIENTBARB1:
		AITHINK_AncientBarb1SkillHandler(pGame, pUnit, pAiTickParam);
		return;

	case MONSTER_ANCIENTBARB2:
		AITHINK_AncientBarb2SkillHandler(pGame, pUnit, pAiTickParam);
		return;

	case MONSTER_ANCIENTBARB3:
		AITHINK_AncientBarb3SkillHandler(pGame, pUnit, pAiTickParam);
		return;

	default:
		return;
	}
}

enum D2C_BaalThroneAIParams
{
	BAALTHRONE_AI_PARAM_SKILL_CHANCE_PCT = 0,
};

//D2Game.0x6FCEFBF0
void __fastcall AITHINK_Fn134_BaalThrone(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	constexpr int32_t nBaalSuperUniqueMinions[] = { SUPERUNIQUE_BAAL_SUBJECT_1, SUPERUNIQUE_BAAL_SUBJECT_2, SUPERUNIQUE_BAAL_SUBJECT_3, SUPERUNIQUE_BAAL_SUBJECT_4, SUPERUNIQUE_BAAL_SUBJECT_5 };

	if (!pAiTickParam->pMonstatsTxt)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pGame->dwGameFrame < pAiTickParam->pAiControl->dwAiParam[2])
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, pGame->dwGameFrame - pAiTickParam->pAiControl->dwAiParam[2]);
		return;
	}

	const int32_t nParam = pAiTickParam->pAiControl->dwAiParam[0];

	D2BaalThroneAiCallbackArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nDistance = 64;
	arg.unk0x08 = 0;
	arg.nMaxDistance = 64;
	sub_6FCF1E80(pGame, pUnit, &arg, 0, 3);

	if (arg.unk0x08)
	{
		if (pAiTickParam->pTarget)
		{
			D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pAiTickParam->pMonstatsTxt->nSkill[0]);
			if (pSkillsTxtRecord
				&& (pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
				&& (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pAiTickParam->pTarget, pSkillsTxtRecord->wAuraTargetState))
				&& (AIRollChanceParam(pGame, pUnit, pAiTickParam, BAALTHRONE_AI_PARAM_SKILL_CHANCE_PCT)))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
				return;
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 10);
		return;
	}

	if (pAiTickParam->pAiControl->dwAiParam[1] & 1)
	{
		if (nParam < 5)
		{
			int32_t nSuperUnique = nBaalSuperUniqueMinions[nParam];
			if (nSuperUnique >= 0 && nSuperUnique < 66)
			{
				nSuperUnique = sgptDataTables->nSuperUniqueIds[nSuperUnique];
			}
			else
			{
				nSuperUnique = -1;
			}

			SKILLS_AssignSkill(pUnit, SKILL_BAALMONSTERSPAWN, 1, 0, __FILE__, __LINE__);

			D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, SKILL_BAALMONSTERSPAWN);
			D2_ASSERT(pSkill);

			SKILLS_SetParam1(pSkill, DATATBLS_MapSuperUniqueId(2, nSuperUnique));

			AITACTICS_UseSkill(pUnit->pGame, pUnit, MONMODE_SKILL3, SKILL_BAALMONSTERSPAWN, nullptr, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit) + 13);

			++pAiTickParam->pAiControl->dwAiParam[0];
			pAiTickParam->pAiControl->dwAiParam[2] = pUnit->pGame->dwGameFrame + 100;

			AITHINK_BaalThroneSetAiParamFlag(pAiTickParam, 2, 1);
			AITHINK_BaalThroneSetAiParamFlag(pAiTickParam, 1, 0);
		}
		else
		{
			MONSTER_Reinitialize(pGame, pUnit, MONSTER_BAALCRABSTAIRS, MONMODE_NEUTRAL);

			D2AiControlStrc* pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);

			AITHINK_ExecuteAiFn(pGame, pUnit, pAiControl, AISPECIALSTATE_NONE);
			STATES_ToggleState(pUnit, STATE_CHANGECLASS, 1);

			int32_t nUnitGUID = 0;
			int32_t nUnitType = 0;
			if (pUnit)
			{
				nUnitGUID = pUnit->dwUnitId;
				nUnitType = pUnit->dwUnitType;
			}
			else
			{
				nUnitGUID = -1;
				nUnitType = 6;
			}

			D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0, 0, nUnitType, nUnitGUID);
			if (pStatList)
			{
				D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
				STATLIST_SetState(pStatList, STATE_CHANGECLASS);
				if (pStatList)
				{
					STATLIST_SetStatIfListIsValid(pStatList, STAT_SHORTPARAM1, MONSTER_BAALCRABSTAIRS, 0);
				}
			}

			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		}
	}
	else
	{
		if (nParam >= 0 && nParam < 5)
		{
			int32_t nSuperUnique = nBaalSuperUniqueMinions[nParam];
			if (nSuperUnique >= 0 && nSuperUnique < 66)
			{
				nSuperUnique = sgptDataTables->nSuperUniqueIds[nSuperUnique];
			}
			else
			{
				nSuperUnique = -1;
			}

			D2SuperUniquesTxt* pSuperUniquesTxtRecord = DATATBLS_GetSuperUniquesTxtRecord(nSuperUnique);
			if (pSuperUniquesTxtRecord && pSuperUniquesTxtRecord->dwClass >= 0)
			{
				sub_6FCC6470(pUnit, pSuperUniquesTxtRecord->dwClass);

				if (pSuperUniquesTxtRecord->dwClass == MONSTER_FALLENSHAMAN5)
				{
					sub_6FCC6470(pUnit, MONSTER_FALLEN5);
				}
				else if (pSuperUniquesTxtRecord->dwClass == MONSTER_UNRAVELER5)
				{
					sub_6FCC6470(pUnit, MONSTER_SKMAGE_COLD3);
				}
			}
		}

		AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL3, SKILL_BAALCORPSEEXPLODE, pUnit, 0, 0);
		pAiTickParam->pAiControl->dwAiParam[1] |= 1u;
		pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + 250;
		SUNIT_AttachSound(pUnit, 16, 0);
	}
}

//D2Game.0x6FCF0030
void __fastcall AITHINK_BaalThroneSetAiParamFlag(D2AiTickParamStrc* pAiTickParam, int32_t nFlag, int32_t bSet)
{
	if (bSet)
	{
		pAiTickParam->pAiControl->dwAiParam[1] |= nFlag;
	}
	else
	{
		pAiTickParam->pAiControl->dwAiParam[1] &= ~nFlag;
	}
}

//D2Game.0x6FCF0050
D2UnitStrc* __fastcall AITHINK_TargetCallback_BaalToStairs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2BaalToStairsAiCallbackArgStrc* pArg = (D2BaalToStairsAiCallbackArgStrc*)pCallbackArg;

	if (pTarget && pTarget->dwUnitType == UNIT_OBJECT && pTarget->dwClassId == OBJECT_BAALS_PORTAL_WORLDSTONECHAMBER && UNITS_GetDistanceToOtherUnit(pTarget, pUnit) <= pArg->nMaxDistance)
	{
		return pTarget;
	}

	return nullptr;
}

enum D2C_BaalToStairsAIParams
{
	BAALTOSTAIRS_AI_PARAM_ACTIVE_DISTANCE = 0,
};

//D2Game.0x6FCF0090
void __fastcall AITHINK_Fn138_BaalToStairs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2BaalToStairsAiCallbackArgStrc arg = {};
	arg.nMaxDistance = 25;

	D2UnitStrc* pTarget = sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_BaalToStairs, 1);

	if (!pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	if (UNITS_GetDistanceToOtherUnit(pUnit, pTarget) >= AI_GetParamValue(pGame, pAiTickParam, BAALTOSTAIRS_AI_PARAM_ACTIVE_DISTANCE))
	{
		AITACTICS_SetVelocity(pUnit, 1, 0, 0);
		AITACTICS_WalkToTargetUnit(pGame, pUnit, pTarget);
		return;
	}

	ACT5Q6_OpenWorldStoneChamber(pGame);
	STATES_ToggleState(pUnit, STATE_INVIS, 1);
	D2Common_10153(pUnit->pDynamicPath);
	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);
	DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pUnit), pUnit->dwUnitType, pUnit->dwUnitId);
	UNITS_FreeCollisionPath(pUnit);
	UNITROOM_RemoveUnitFromRoom(pUnit);
	SUNIT_RemoveUnit(pGame, pUnit);
}

enum D2C_BaalTauntAIParams
{
	BAALTAUNT_AI_PARAM_IN_RANGE_DISTANCE = 0,
	BAALTAUNT_AI_PARAM_FRAMES_IN_SIGHT_BEFORE_TAUNT = 1,
	BAALTAUNT_AI_PARAM_MAX_TARGET_DISTANCE = 2,
};

//D2Game.0x6FCF0180
void __fastcall AITHINK_Fn136_BaalTaunt(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pTarget)
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
		return;
	}

	int32_t bIsTargetNeutral = 0;
	if (pAiTickParam->pTarget->dwUnitType == UNIT_PLAYER || pAiTickParam->pTarget->dwUnitType == UNIT_MONSTER)
	{
		bIsTargetNeutral = pAiTickParam->pTarget->dwAnimMode == PLRMODE_NEUTRAL;
	}

	if (!bIsTargetNeutral)
	{
		pAiTickParam->pAiControl->dwAiParam[0] = 0;
	}
	else
	{
		++pAiTickParam->pAiControl->dwAiParam[0];

		if (pAiTickParam->pAiControl->dwAiParam[0] > AI_GetParamValue(pGame, pAiTickParam, BAALTAUNT_AI_PARAM_FRAMES_IN_SIGHT_BEFORE_TAUNT))
		{
			pAiTickParam->pAiControl->dwAiParam[0] = 0;
			AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK1, SKILL_BAALTAUNT, pAiTickParam->pTarget, 0, 0);
			return;
		}
	}

	
	if ((pAiTickParam->nTargetDistance > AI_GetParamValue(pGame, pAiTickParam, BAALTAUNT_AI_PARAM_MAX_TARGET_DISTANCE) && sub_6FCBDFE0(pGame, pUnit, UNITS_GetRoom(pAiTickParam->pTarget), CLIENTS_GetUnitX(pAiTickParam->pTarget), CLIENTS_GetUnitY(pAiTickParam->pTarget), 0, 0))
		|| pAiTickParam->nTargetDistance <= AI_GetParamValue(pGame, pAiTickParam, BAALTAUNT_AI_PARAM_IN_RANGE_DISTANCE))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
	}
	else
	{
		AITACTICS_WalkToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
	}
}

enum D2C_BaalTentacleAIParams
{
	BAALTENTACLE_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	BAALTENTACLE_AI_PARAM_STALL_DURATION = 1,
	BAALTENTACLE_AI_PARAM_STAY_ALIVE_RANDOM_RANGE_SECONDS = 2,
};

//D2Game.0x6FCF02D0
void __fastcall AITHINK_Fn139_BaalTentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
	if (!pOwner || SUNIT_IsDead(pOwner))
	{
		SUNITDMG_KillMonster(pGame, pUnit, 0, 1);
		return;
	}

	if (!pAiTickParam->pAiControl->dwAiParam[2])
	{
		pAiTickParam->pAiControl->dwAiParam[2] = pGame->dwGameFrame + 25 * (ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, AI_GetParamValue(pGame, pAiTickParam, BAALTENTACLE_AI_PARAM_STAY_ALIVE_RANDOM_RANGE_SECONDS)) + AI_GetParamValue(pGame, pAiTickParam, BAALTENTACLE_AI_PARAM_STAY_ALIVE_RANDOM_RANGE_SECONDS));
	}

	if (pGame->dwGameFrame > pAiTickParam->pAiControl->dwAiParam[2])
	{
		SUNITDMG_KillMonster(pGame, pUnit, 0, 1);
		return;
	}

	if (pAiTickParam->bCombat && (AIRollChanceParam(pGame, pUnit, pAiTickParam, BAALTENTACLE_AI_PARAM_ATTACK_CHANCE_PCT)))
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, BAALTENTACLE_AI_PARAM_STALL_DURATION));
}

enum D2C_BaalMinionAIParams
{
	BAALMINION_AI_PARAM_ATTACK_CHANCE_PCT = 0,
	BAALMINION_AI_PARAM_APPROACH_CHANCE_PCT = 1,
	BAALMINION_AI_PARAM_SKILL_CHANCE_PCT_MELEE_STALL_DURATION = 2,
	BAALMINION_AI_PARAM_SKILL_STALL_DURATION = 3,
};

//D2Game.0x6FCF0420
void __fastcall AITHINK_Fn141_BaalMinion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pTarget && pAiTickParam->bCombat)
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BAALMINION_AI_PARAM_ATTACK_CHANCE_PCT))
		{
			if (pAiTickParam->pMonstatsTxt->nSkill[0] >= 0 && AIRollChanceParam(pGame, pUnit, pAiTickParam, BAALMINION_AI_PARAM_SKILL_CHANCE_PCT_MELEE_STALL_DURATION))
			{
				AITACTICS_UseSkill(pGame, pUnit, pAiTickParam->pMonstatsTxt->nSkillMode[0], pAiTickParam->pMonstatsTxt->nSkill[0], pAiTickParam->pTarget, 0, 0);
			}
			else
			{
				AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
			}
		}
		else
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, BAALMINION_AI_PARAM_SKILL_CHANCE_PCT_MELEE_STALL_DURATION));
		}
	}
	else
	{
		if (AIRollChanceParam(pGame, pUnit, pAiTickParam, BAALMINION_AI_PARAM_APPROACH_CHANCE_PCT))
		{
			AITACTICS_WalkToTargetUnit(pGame, pUnit, pAiTickParam->pTarget);
		}
	}

	AITACTICS_Idle(pGame, pUnit, AI_GetParamValue(pGame, pAiTickParam, BAALMINION_AI_PARAM_SKILL_STALL_DURATION));
}

//D2Game.0x6FCF0570
D2UnitStrc* __fastcall AITHINK_TargetCallback_PutridDefiler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg)
{
	D2PutridDefilerAiCallbackArgStrc* pArg = (D2PutridDefilerAiCallbackArgStrc*)pCallbackArg;

	if (sub_6FD01910(pUnit, pTarget) && AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget) <= pArg->nMaxDistance)
	{
		return pTarget;
	}

	return nullptr;
}

enum D2C_PutridDefilerAIParams
{
	PUTRIDDEFILER_AI_PARAM_TOO_CLOSE_DISTANCE = 0,
	PUTRIDDEFILER_AI_PARAM_WALK_AWAY_MAX_DISTANCE = 1,
};

//D2Game.0x6FCF05B0
void __fastcall AITHINK_Fn137_PutridDefiler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pTarget && pAiTickParam->bCombat)
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK1, pAiTickParam->pTarget);
		return;
	}

	D2PutridDefilerAiCallbackArgStrc arg = {};
	arg.nMaxDistance = 25;

	D2UnitStrc* pTarget = sub_6FCF1E80(pGame, pUnit, &arg, AITHINK_TargetCallback_PutridDefiler, 1);
	if (pTarget)
	{
		if (UNITS_IsInMeleeRange(pUnit, pTarget, 0))
		{
			AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL1, SKILL_IMPREGNATE, pTarget, 0, 0);
		}
		else
		{
			AITACTICS_WalkToTargetUnit(pGame, pUnit, pTarget);
		}
		return;
	}

	if (pAiTickParam->nTargetDistance < AI_GetParamValue(pGame, pAiTickParam, PUTRIDDEFILER_AI_PARAM_TOO_CLOSE_DISTANCE))
	{
		D2GAME_AICORE_Escape_6FCD0560(pGame, pUnit, pAiTickParam->pTarget, AI_GetParamValue(pGame, pAiTickParam, PUTRIDDEFILER_AI_PARAM_WALK_AWAY_MAX_DISTANCE), 0);
		return;
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 25);
}

//D2Game.0x6FCF06A0
D2C_AiSpecialState __fastcall AITHINK_GetSpecialStateFromAiControl(D2AiControlStrc* pAiControl)
{
	return pAiControl->nAiSpecialState;
}

//D2Game.0x6FCF06B0
void __fastcall AITHINK_SetAiControlParams(D2AiControlStrc* pAiControl, int32_t nParam0, int32_t nParam1, int32_t nParam2)
{
	if (!pAiControl)
	{
		return;
	}

	if (nParam0 != -666)
	{
		pAiControl->dwAiParam[0] = nParam0;
	}

	if (nParam1 != -666)
	{
		pAiControl->dwAiParam[1] = nParam1;
	}

	if (nParam2 != -666)
	{
		pAiControl->dwAiParam[2] = nParam2;
	}
}

//D2Game.0x6FCF06E0
BOOL __fastcall AITHINK_CanUnitSwitchAi(D2UnitStrc* pUnit, D2MonStatsTxt* pMonStatsTxtRecord, D2C_AiSpecialState nAiSpecialState, int32_t bCheckIfSuperUnique)
{
	if (nAiSpecialState == 0)
	{
		return TRUE;
	}

	if (nAiSpecialState != AISPECIALSTATE_DIMVISION && nAiSpecialState != AISPECIALSTATE_TERROR && nAiSpecialState != AISPECIALSTATE_TAUNT)
	{
		return TRUE;
	}

	if (bCheckIfSuperUnique && MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_SUPERUNIQUE))
	{
		return FALSE;
	}

	if (pUnit)
	{
		return UNITS_CanSwitchAI(pUnit->dwClassId);
	}

	return UNITS_CanSwitchAI(-1);
}

//D2Game.0x6FCF0750
const D2AiTableStrc* __fastcall AITHINK_GetAiTableRecord(D2UnitStrc* pUnit, D2C_AiSpecialState nAiSpecialState)
{
	static const D2AiTableStrc gpAiTable_6FD3F990[] =
	{
		{ 0,	nullptr,									AITHINK_Fn000,								nullptr },
		{ 0,	nullptr,									AITHINK_Fn001_100_Idle_Buffy,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn002_Skeleton,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn003_Zombie,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn004_Bighead,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn005_BloodHawk,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn006_Fallen,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn007_Brute,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn008_SandRaider,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn009_Wraith,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn010_CorruptRogue,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn011_Baboon,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn012_019_Goatman_Swarm,			nullptr },
		{ 1,	nullptr,									AITHINK_Fn013_FallenShaman,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn014_QuillRat,						nullptr },
		{ 4,	nullptr,									AITHINK_Fn015_SandMaggot,					D2GAME_AI_Unk015_6FCD34A0 },
		{ 1,	nullptr,									AITHINK_Fn016_ClawViper,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn017_SandLeaper,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn018_PantherWoman,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn012_019_Goatman_Swarm,			nullptr },
		{ 1,	nullptr,									AITHINK_Fn020_Scarab,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn021_Mummy,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn022_GreaterMummy,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn023_Vulture,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn024_Mosquito,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn025_Willowisp,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn026_Arach,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn027_ThornHulk,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn028_Vampire,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn029_BatDemon,						D2GAME_AI_Unk029_6FCD76F0 },
		{ 1,	nullptr,									AITHINK_Fn030_Fetish,						nullptr },
		{ 0,	nullptr,									AITHINK_Fn031_NpcOutOfTown,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn032_Npc,							nullptr },
		{ 1,	nullptr,									AITHINK_Fn033_HellMeteor,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn034_Andariel,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn035_CorruptArcher,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn036_CorruptLancer,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn037_SkeletonBow,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn038_MaggotLarva,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn039_PinHead,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn040_MaggotEgg,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn041_Towner,						nullptr },
		{ 0,	nullptr,									AITHINK_Fn042_Vendor,						nullptr },
		{ 1,	D2GAME_AI_Unk043_045_121_6FCD8E10,			AITHINK_Fn043_FoulCrowNest,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn044_Duriel,						nullptr },
		{ 1,	D2GAME_AI_Unk043_045_121_6FCD8E10,			AITHINK_Fn045_Sarcophagus,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn046_ElementalBeast,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn047_FlyingScimitar,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn048_ZakarumZealot,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn049_ZakarumPriest,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn050_Mephisto,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn051_Diablo,						D2GAME_AI_Unk051_6FCE82F0 },
		{ 5,	nullptr,									AITHINK_Fn052_FrogDemon,					D2GAME_AI_Unk052_6FCDA910 },
		{ 1,	nullptr,									AITHINK_Fn053_Summoner,						nullptr },
		{ 0,	nullptr,									AITHINK_Fn054_NpcStationary,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn055_Izual,						nullptr },
		{ 2,	nullptr,									AITHINK_Fn056_Tentacle,						nullptr },
		{ 2,	nullptr,									AITHINK_Fn057_TentacleHead,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn058_Navi,							nullptr },
		{ 1,	D2GAME_AI_Unk59_6FCE58D0,					AITHINK_Fn059_BloodRaven,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn060_GoodNpcRanged,				nullptr },
		{ 0,	nullptr,									AITHINK_Fn061_Hireable,						D2GAME_AI_SpecialState04_6FCDC170 },
		{ 1,	nullptr,									AITHINK_Fn062_TownRogue,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn063_GargoyleTrap,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn064_SkeletonMage,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn065_FetishShaman,					D2GAME_AI_SpecialState04_6FCDC170 },
		{ 0,	nullptr,									AITHINK_Fn066_SandMaggotQueen,				nullptr },
		{ 0,	nullptr,									AITHINK_Fn067_NecroPet,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn068_VileMother,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn069_VileDog,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn070_FingerMage,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn071_Regurgitator,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn072_DoomKnight,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn073_AbyssKnight,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn074_OblivionKnight,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn075_QuillMother,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn076_EvilHole,						nullptr },
		{ 2,	nullptr,									AITHINK_Fn077_TrapMissile,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn078_TrapRightArrow,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn079_TrapLeftArrow,				nullptr },
		{ 2,	nullptr,									AITHINK_Fn080_092_TrapPoison_TrapNova,		nullptr },
		{ 0,	nullptr,									AITHINK_Fn081_JarJar,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn082_InvisoSpawner,				nullptr },
		{ 0,	nullptr,									AITHINK_Fn083_MosquitoNest,					nullptr },
		{ 0,	D2GAME_AI_Unk084_6FCDEC70,					AITHINK_Fn084_BoneWall,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn085_HighPriest,					nullptr },
		{ 2,	nullptr,									AITHINK_Fn086_Hydra,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn087_TrapMelee,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn088_7TIllusion,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn089_Megademon,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn090_Griswold,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn091_DarkWanderer,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn080_092_TrapPoison_TrapNova,		nullptr },
		{ 1,	nullptr,									AITHINK_Fn093_ArcaneTower,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn094_DesertTurret,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn095_PantherJavelin,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn096_FetishBlowgun,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn097_Spirit,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn098_Smith,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn099_TrappedSoul,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn001_100_Idle_Buffy,				nullptr },
		{ 0,	D2GAME_AI_Unk101_104_6FCE9CE0,				AITHINK_Fn101_AssassinSentry,				nullptr },
		{ 0,	D2GAME_AI_Unk102_6FCE9FB0,					AITHINK_Fn102_BladeCreeper,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn103_InvisoPet,					nullptr },
		{ 0,	D2GAME_AI_Unk101_104_6FCE9CE0,				AITHINK_Fn104_DeathSentry,					nullptr },
		{ 2,	D2GAME_AI_Unk105_6FCEA680,					AITHINK_Fn105_ShadowWarrior,				nullptr },
		{ 2,	D2GAME_AI_Unk106_6FCEAFE0,					AITHINK_Fn106_143_ShadowMaster,				nullptr },
		{ 2,	D2GAME_AI_Unk107_6FCED140,					AITHINK_Fn107_Raven,						nullptr },
		{ 0,	nullptr,									AITHINK_Fn108_DruidWolf,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn109_Totem,						nullptr },
		{ 2,	D2GAME_AI_Unk_110_111_6FCECC40,				AITHINK_Fn110_Vines,						nullptr },
		{ 2,	D2GAME_AI_Unk_110_111_6FCECC40,				AITHINK_Fn111_CycleOfLife,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn112_DruidBear,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn113_SiegeTower,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn114_ReanimatedHorde,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn115_SiegeBeast,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn116_Minion,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn117_SuicideMinion,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn118_Succubus,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn119_SuccubusWitch,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn120_Overseer,						nullptr },
		{ 1,	D2GAME_AI_Unk043_045_121_6FCD8E10,			AITHINK_Fn121_MinionSpawner,				nullptr },
		{ 1,	D2GAME_AI_Unk122_6FCE2080,					AITHINK_Fn122_Imp,							nullptr },
		{ 1,	nullptr,									AITHINK_Fn123_Catapult,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn124_FrozenHorror,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn125_BloodLord,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn126_CatapultSpotter,				nullptr },
		{ 2,	AITHINK_Fn000,								AITHINK_Fn127_NpcBarb,						nullptr },
		{ 2,	AITHINK_Fn000,								AITHINK_Fn128_Nihlathak,					D2GAME_AI_SpecialState04_6FCDC170 },
		{ 1,	D2GAME_AI_Unk129_6FCE5610,					AITHINK_Fn129_GenericSpawner,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn130_DeathMauler,					nullptr },
		{ 2,	nullptr,									AITHINK_Fn131_Wussie,						nullptr },
		{ 0,	nullptr,									AITHINK_Fn132_AncientStatue,				nullptr },
		{ 2,	nullptr,									AITHINK_Fn133_Ancient,						nullptr },
		{ 2,	AITHINK_Fn000,								AITHINK_Fn134_BaalThrone,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn135_BaalCrab,						D2GAME_AI_Unk135_140_6FCCD470 },
		{ 1,	nullptr,									AITHINK_Fn136_BaalTaunt,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn137_PutridDefiler,				nullptr },
		{ 1,	nullptr,									AITHINK_Fn138_BaalToStairs,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn139_BaalTentacle,					nullptr },
		{ 0,	nullptr,									AITHINK_Fn140_BaalCrabClone,				D2GAME_AI_Unk135_140_6FCCD470 },
		{ 1,	nullptr,									AITHINK_Fn141_BaalMinion,					nullptr },
		{ 1,	nullptr,									AITHINK_Fn142_ClawViperEx,					nullptr },
		{ 2,	D2GAME_AI_Unk143_6FCEB1B0,					AITHINK_Fn106_143_ShadowMaster,				nullptr },
	};

	static const D2AiTableStrc gpSpecialAiStateTable_6FD40290[] =
	{
		{ 0,	nullptr,									nullptr,									nullptr },
		{ 0,	nullptr,									AITHINK_Fn001_100_Idle_Buffy,				nullptr },
		{ 1,	nullptr,									D2GAME_AI_SpecialState02_6FCD1660,			nullptr },
		{ 1,	D2GAME_AI_SpecialState03_6FCE4B90,			D2GAME_AI_SpecialState03_6FCE4CC0,			nullptr },
		{ 0,	nullptr,									AITHINK_Fn061_Hireable,						D2GAME_AI_SpecialState04_6FCDC170 },
		{ 0,	nullptr,									AITHINK_Fn060_GoodNpcRanged,				nullptr },
		{ 0,	nullptr,									D2GAME_AI_SpecialState06_6FCE7930,			nullptr },
		{ 0,	nullptr,									AITHINK_Fn067_NecroPet,						nullptr },
		{ 1,	nullptr,									AITHINK_Fn062_TownRogue,					nullptr },
		{ 1,	nullptr,									D2GAME_AI_SpecialState09_6FCE7C40,			nullptr },
		{ 1,	nullptr,									D2GAME_AI_SpecialState10_17_6FCE7CF0,		nullptr },
		{ 1,	D2GAME_AI_SpecialState11_6FCE7E20,			D2GAME_AI_SpecialState11_6FCE7E80,			nullptr },
		{ 0,	nullptr,									D2GAME_AI_SpecialState12_6FCE81B0,			nullptr },
		{ 1,	nullptr,									D2GAME_AI_SpecialState13_6FCE5080,			nullptr },
		{ 1,	nullptr,									D2GAME_AI_SpecialState14_6FCE1480,			nullptr },
		{ 1,	nullptr,									AITHINK_Fn117_SuicideMinion,				nullptr },
		{ 1,	D2GAME_AI_SpecialState16_6FCE1D30,			D2GAME_AI_SpecialState16_6FCE1DC0,			nullptr },
		{ 2,	nullptr,									D2GAME_AI_SpecialState10_17_6FCE7CF0,		nullptr },
	};
	static_assert(ARRAY_SIZE(gpSpecialAiStateTable_6FD40290) == AISPECIALSTATE_TABLE_COUNT, "Enum and table must match");


	if (nAiSpecialState && (nAiSpecialState != AISPECIALSTATE_DIMVISION && nAiSpecialState != AISPECIALSTATE_TERROR && nAiSpecialState != AISPECIALSTATE_TAUNT || UNITS_CanSwitchAI(pUnit ? pUnit->dwClassId : -1)))
	{
		return &gpSpecialAiStateTable_6FD40290[nAiSpecialState];
	}

	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		const int32_t nAi = pUnit->pMonsterData->pMonstatsTxt->wAI;
		if (nAi >= 0 && nAi < 144)
		{
			return &gpAiTable_6FD3F990[nAi];
		}
	}

	return gpAiTable_6FD3F990;
}

//D2Game.0x6FCF07D0
void __fastcall AITHINK_ExecuteAiFn(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2C_AiSpecialState nAiSpecialState)
{
	if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
	{
		FOG_DisplayWarning("! StatsGetState (hUnit, UNIT_STATE_UNINTERRUPTABLE)", __FILE__, __LINE__);
	}

	if (!pGame || !pUnit || !pAiControl || nAiSpecialState >= AISPECIALSTATE_TABLE_COUNT || pUnit->dwUnitType != UNIT_MONSTER)
	{
		return;
	}

	if (pUnit->dwClassId >= sgptDataTables->nMonStatsTxtRecordCount)
	{
		nAiSpecialState = AISPECIALSTATE_NO_MON_STATS;
	}

	const D2AiTableStrc* pAiTableRecord = nullptr;
	if (pAiControl->pAiParamFn)
	{
		pAiTableRecord = AITHINK_GetAiTableRecord(pUnit, pAiControl->nAiSpecialState);
		if (pAiControl->pAiParamFn == pAiTableRecord->pAiParamFn && pAiTableRecord->unk0x0C)
		{
			pAiControl->nAiSpecialState = nAiSpecialState;
			pAiControl->pAiParamFn = pAiTableRecord->unk0x0C;
			return;
		}
	}

	pAiControl->dwAiParam[0] = 0;
	pAiControl->dwAiParam[1] = 0;
	pAiControl->dwAiParam[2] = 0;

	AIGENERAL_FreeAllAiCommands(pGame, pUnit);

	pAiTableRecord = AITHINK_GetAiTableRecord(pUnit, nAiSpecialState);

	if (pAiTableRecord->unk0x04)
	{
		D2AiTickParamStrc aiTickParam = {};
		aiTickParam.pAiControl = pAiControl;
		aiTickParam.pMonstatsTxt = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
		aiTickParam.pMonstats2Txt = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);

		if (!aiTickParam.pMonstatsTxt || !aiTickParam.pMonstats2Txt)
		{
			return;
		}

		pAiTableRecord->unk0x04(pGame, pUnit, &aiTickParam);
	}

	AIPARAMFN pAiParamFn = pAiTableRecord->pAiParamFn;
	if (!pAiParamFn)
	{
		pAiParamFn = AITHINK_Fn001_100_Idle_Buffy;
	}

	pAiControl->nAiSpecialState = nAiSpecialState;
	pAiControl->pAiParamFn = pAiParamFn;

	if (IsBadCodePtr((FARPROC)pAiParamFn))
	{
		FOG_DisplayAssert("pfnFunction", __FILE__, __LINE__);
		exit(-1);
	}
}

//D2Game.0x6FCF0A70
void __fastcall D2GAME_MONSTERS_AiFunction03_6FCF0A70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4)
{
	D2AiTickParamStrc aiTickParam = {};

	aiTickParam.pAiControl = AIGENERAL_GetAiControlFromUnit(pUnit);
	aiTickParam.pMonstatsTxt = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	aiTickParam.pMonstats2Txt = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);

	if (!aiTickParam.pMonstatsTxt || !aiTickParam.pMonstats2Txt)
	{
		return;
	}

	if (STATES_CheckState(pUnit, STATE_STUNNED))
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 3);
		return;
	}

	if (aiTickParam.pMonstatsTxt->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_OPENDOORS] && D2Common_10201(pUnit->pDynamicPath) & 0x800)
	{
		D2DoorObjectAiCallbackArgStrc arg = {};
		arg.pDoor = nullptr;
		arg.nDistance = 9;

		sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 8);

		if (arg.pDoor && DATATBLS_GetObjectsTxtRecord(arg.pDoor->dwClassId)->nMonsterOK)
		{
			sub_6FC78B20(pGame, pUnit, UNIT_OBJECT, arg.pDoor->dwUnitId);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}
	}

	if (D2GAME_AICORE_MinionLeash_6FCF0D10(pGame, pUnit, &aiTickParam))
	{
		return;
	}

	switch (AITHINK_GetAiTableRecord(pUnit, aiTickParam.pAiControl->nAiSpecialState)->unk0x00)
	{
	case 1:
		aiTickParam.pTarget = sub_6FCCF9D0(pGame, pUnit, aiTickParam.pAiControl, &aiTickParam.nTargetDistance, &aiTickParam.bCombat);
		if (!aiTickParam.pTarget)
		{
			return;
		}
		break;

	case 2:
		aiTickParam.pTarget = sub_6FCF2110(pGame, pUnit, aiTickParam.pAiControl, &aiTickParam.nTargetDistance, &aiTickParam.bCombat);
		break;

	case 4:
		aiTickParam.pTarget = sub_6FCCFC00(pGame, pUnit, aiTickParam.pAiControl, &aiTickParam.nTargetDistance, &aiTickParam.bCombat);
		if (!aiTickParam.pTarget)
		{
			return;
		}
		break;

	case 5:
		aiTickParam.pTarget = sub_6FCCFC00(pGame, pUnit, aiTickParam.pAiControl, &aiTickParam.nTargetDistance, &aiTickParam.bCombat);
		break;

	case 0:
		break;

	default:
		return;
	}

	if (sub_6FCF0E40(pGame, pUnit, &aiTickParam))
	{
		return;
	}

	if (IsBadCodePtr((FARPROC)aiTickParam.pAiControl->pAiParamFn))
	{
		FOG_DisplayAssert("tAiParms.ptAi->pfnAi", __FILE__, __LINE__);
		exit(-1);
	}

	aiTickParam.pAiControl->pAiParamFn(pGame, pUnit, &aiTickParam);
}

//D2Game.0x6FCF0D10
int32_t __fastcall D2GAME_AICORE_MinionLeash_6FCF0D10(D2GameStrc* pGame, D2UnitStrc* pMonster, D2AiTickParamStrc* pAiTickParam)
{
	if (pAiTickParam->pAiControl->dwOwnerGUID == -1)
	{
		return 0;
	}

	if (!pMonster)
	{
		pAiTickParam->pAiControl->dwOwnerGUID = -1;
		return 0;
	}

	D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pMonster->dwClassId);
	if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_WALK]))
	{
		pAiTickParam->pAiControl->dwOwnerGUID = -1;
		return 0;
	}

	D2UnitStrc* pOwner = SUNIT_GetServerUnit(pGame, pAiTickParam->pAiControl->dwOwnerType, pAiTickParam->pAiControl->dwOwnerGUID);
	if (!pOwner || SUNIT_IsDead(pOwner))
	{
		pAiTickParam->pAiControl->dwOwnerGUID = -1;
		return 0;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pMonster, pOwner);
	if (nDistance <= 1 && pAiTickParam->pAiControl->dwOwnerType == UNIT_PLAYER)
	{
		if (!D2GAME_AICORE_Escape_6FCD0560(pGame, pMonster, pOwner, 19u, 1))
		{
			D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pMonster, pOwner, 19u);
		}

		return 1;
	}

	if (nDistance > 20)
	{
		AITACTICS_SetVelocity(pMonster, 7, 0, 40u);
		D2GAME_AICORE_WalkToOwner_6FCD0B60(pGame, pMonster, pOwner, 19u);
		return 1;
	}

	return 0;
}

//Inlined in D2Game.0x6FCF0E40
int32_t __fastcall sub_6FCF0E40_inline(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pUnit)
	{
		return 0;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ISMELEE]))
	{
		return 0;
	}

	D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
	if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_WALK]))
	{
		return 0;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	const int32_t nMonSpcWalk = DATATBLS_GetLevelsTxtRecord(DUNGEON_GetLevelIdFromRoom(pRoom))->nMonSpcWalk;
	if (nMonSpcWalk <= 0 || pAiTickParam->nTargetDistance <= nMonSpcWalk || sub_6FCF14D0(pUnit, pAiTickParam->pTarget))
	{
		return 0;
	}

	D2AiCallback11ArgStrc arg = {};
	arg.pTarget = nullptr;
	arg.nDistance = INT_MAX;
	arg.unk0x08 = pAiTickParam->pTarget;
	sub_6FCF1E80(pGame, pUnit, &arg, nullptr, 11);

	if (!arg.pTarget)
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 4u);
		return 1;
	}

	pAiTickParam->pTarget = arg.pTarget;
	pAiTickParam->nTargetDistance = arg.nDistance;
	return 0;
}

//D2Game.0x6FCF0E40
int32_t __fastcall sub_6FCF0E40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	if (!pAiTickParam->pTarget)
	{
		return 0;
	}

	if (pUnit && pUnit->dwClassId == MONSTER_SUMMONER || (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE) || MONSTERS_IsBoss(nullptr, pUnit)))
	{
		if (pAiTickParam->nTargetDistance < 20u && pAiTickParam->pTarget && pAiTickParam->pTarget->dwUnitType == UNIT_PLAYER && !AIUTIL_CheckAiControlFlag(pAiTickParam->pAiControl, 0x10u))
		{
			SUNIT_AttachSound(pUnit, 16u, 0);
			AIUTIL_ToggleAiControlFlag(pAiTickParam->pAiControl, 0x10u, 1);
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 20);
			return 1;
		}
	}

	if (SUNIT_IsDead(pUnit) || !AIUTIL_CheckAiControlFlag(pAiTickParam->pAiControl, 0x20u) || AI_RollPercentage(pUnit) >= 40)
	{
		return sub_6FCF0E40_inline(pGame, pUnit, pAiTickParam);
	}

	int32_t nBaseId = -1;
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
	{
		nBaseId = pUnit->dwClassId;
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
		if (pMonStatsTxtRecord)
		{
			nBaseId = pMonStatsTxtRecord->nBaseId;
		}
	}

	int32_t nClassId = -1;
	if (pUnit)
	{
		nClassId = pUnit->dwClassId;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);

	int32_t bIsMelee = pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ISMELEE];
	if (nBaseId == MONSTER_BIGHEAD1 || nBaseId == MONSTER_COUNCILMEMBER1 || nBaseId == MONSTER_BAALHIGHPRIEST)
	{
		bIsMelee = 1;
	}

	const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

	int32_t nX = 0;
	int32_t nY = 0;
	if (nLifePercentage >= 30 && (bIsMelee || pAiTickParam->nTargetDistance >= 10u)
		|| ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 100) >= 15
		|| !sub_6FC66260(pGame, pRoom, D2Common_10096(pRoom, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit)), nBaseId, &nX, &nY, 0))
	{
		return sub_6FCF0E40_inline(pGame, pUnit, pAiTickParam);
	}

	pRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
	if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
	{
		return sub_6FCF0E40_inline(pGame, pUnit, pAiTickParam);
	}

	if (nLifePercentage < 30 && ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 100) < 25 && !STATES_CheckState(pUnit, STATE_PREVENTHEAL))
	{
		int32_t nAdd = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0) << 8;
		const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
		const int32_t nMaxHitpoints = STATLIST_GetMaxLifeFromUnit(pUnit);
		if (nHitpoints + nAdd > nMaxHitpoints)
		{
			nAdd = nMaxHitpoints - nHitpoints;
		}

		STATLIST_AddUnitStat(pUnit, STAT_HITPOINTS, nAdd, 0);
	}

	AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK1, SKILL_MONTELEPORT, 0, nX, nY);
	return 1;
}
