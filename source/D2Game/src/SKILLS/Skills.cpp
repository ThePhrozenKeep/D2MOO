#include "SKILLS/Skills.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2Composit.h>
#include <D2States.h>
#include <D2DataTbls.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/SkillsTbls.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/MonsterIds.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <D2Items.h>
#include <D2Dungeon.h>
#include <D2Collision.h>
#include <Units/UnitFinds.h>
#include <D2Monsters.h>
#include <UselessOrdinals.h>
#include <D2Combat.h>
#include <D2ItemMods.h>
#include <Units/UnitRoom.h>


#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrModes.h"
#include "PLAYER/PlrSave2.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillAma.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillBar.h"
#include "SKILLS/SkillDruid.h"
#include "SKILLS/SkillItem.h"
#include "SKILLS/SkillMonst.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/SkillPal.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillSor.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitInactive.h"


constexpr SkillStartFunc gpSkillSrvStartFnTable_6FD408B0[] =
{
    nullptr,
    SKILLS_SrvSt01_Attack_LeftHandSwing,
    SKILLS_SrvSt02_Kick,
    SKILLS_SrvSt03_Unsummon,
    SKILLS_SrvSt04_Arrow_Bolt,
    SKILLS_SrvSt05_Jab,
    SKILLS_SrvSt06_PowerStrike_ChargedStrike,
    SKILLS_SrvSt07_Impale,
    SKILLS_SrvSt08_Strafe,
    SKILLS_SrvSt09_Fend,
    SKILLS_SrvSt10_LightningStrike,
    SKILLS_SrvSt11_Inferno_ArcticBlast,
    SKILLS_SrvSt12_Telekinesis_DragonFlight,
    SKILLS_SrvSt13_ThunderStorm,
    SKILLS_SrvSt14_Hydra,
    SKILLS_SrvSt15_RaiseSkeleton_Mage,
    SKILLS_SrvSt16_PoisonDagger,
    SKILLS_SrvSt17_Poison_CorpseExplosion,
    SKILLS_SrvSt18_Attract,
    SKILLS_SrvSt19_BonePrison,
    SKILLS_SrvSt20_IronGolem,
    SKILLS_SrvSt21_Revive,
    SKILLS_SrvSt22_PsychicHammer,
    SKILLS_SrvSt23_AssasinChargeStrikes,
    SKILLS_SrvSt24_DragonTalon,
    SKILLS_SrvSt25_64_DragonClaw_MonFrenzy,
    SKILLS_SrvSt26_BladeFury,
    SKILLS_SrvSt27_DragonTail,
    SKILLS_SrvSt28_BladeShield,
    SKILLS_SrvSt29_Sacrifice,
    nullptr,
    SKILLS_SrvSt31_Charge,
    SKILLS_SrvSt32_Conversion_Bash_Stun_Concentrate_BearSmite,
    SKILLS_SrvSt33_FindPotion_GrimWard,
    SKILLS_SrvSt34_FindItem,
    SKILLS_SrvSt35_Vengeance,
    SKILLS_SrvSt36_HolyShield,
    SKILLS_SrvSt37_Zeal_Fury_BloodLordFrenzy,
    SKILLS_SrvSt38_Whirlwind,
    SKILLS_SrvSt39_Berserk,
    SKILLS_SrvSt40_Leap,
    SKILLS_SrvSt41_LeapAttack,
    SKILLS_SrvSt42_FireHit,
    SKILLS_SrvSt43_MaggotEgg,
    SKILLS_SrvSt44_MaggotUp,
    SKILLS_SrvSt45_MaggotDown,
    SKILLS_SrvSt46_AndrialSpray,
    SKILLS_SrvSt47_Jump,
    SKILLS_SrvSt48_SwarmMove,
    SKILLS_SrvSt49_Nest_EvilHutSpawner,
    SKILLS_SrvSt50_QuickStrike,
    SKILLS_SrvSt51_Submerge,
    SKILLS_SrvSt52_Emerge,
    SKILLS_SrvSt53_MonInferno,
    SKILLS_SrvSt54_DiabRun,
    SKILLS_SrvSt55_Mosquito,
    SKILLS_SrvSt56_FeralRage_Maul,
    SKILLS_SrvSt57_Rabies,
    SKILLS_SrvSt58_FireClaws,
    SKILLS_SrvSt59_ImpInferno,
    SKILLS_SrvSt60_SuckBlood,
    SKILLS_SrvSt61_SelfResurrect,
    SKILLS_SrvSt62_MinionSpawner,
    SKILLS_SrvSt63_Corpse_VineCycler,
    SKILLS_SrvSt25_64_DragonClaw_MonFrenzy,
    SKILLS_SrvSt65_Throw_LeftHandThrow,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};


static const SkillDoFunc gpSkillSrvDoFnTable_6FD40A20[] =
{
    nullptr,
    SKILLS_SrvDo001_Attack_LeftHandSwing,
    SKILLS_SrvDo002_Kick_PowerStrike_MonIceSpear_Impale_Bash_Stun_Concentrate_BearSmite_Vengeance_Berserk_FireClaws,
    SKILLS_SrvDo003_Throw,
    SKILLS_SrvDo004_Unsummon,
    SKILLS_SrvDo005_LeftHandThrow,
    SKILLS_SrvDo006_InnerSight_SlowMissiles,
    SKILLS_SrvDo007_Jab,
    SKILLS_SrvDo008_MultipleShot_Teeth_ShockWave,
    SKILLS_SrvDo009_Frenzy,
    SKILLS_SrvDo010_GuidedArrow_BoneSpirit,
    SKILLS_SrvDo011_ChargedStrike,
    SKILLS_SrvDo012_Strafe,
    SKILLS_SrvDo013_Fend_Zeal_Fury,
    SKILLS_SrvDo014_LightningStrike,
    SKILLS_SrvDo015_Dopplezon,
    SKILLS_SrvDo016_Valkyrie,
    SKILLS_SrvDo017_ChargedBolt_BoltSentry,
    SKILLS_SrvDo018_DefensiveBuff,
    SKILLS_SrvDo019_Inferno_ArcticBlast,
    SKILLS_SrvDo020_StaticField,
    SKILLS_SrvDo021_Telekinesis,
    SKILLS_SrvDo022_NovaAttack,
    SKILLS_SrvDo023_Blaze_EnergyShield_SpiderLay,
    SKILLS_SrvDo024_FireWall,
    SKILLS_SrvDo025_Enchant,
    SKILLS_SrvDo026_ChainLightning,
    SKILLS_SrvDo027_Teleport,
    SKILLS_SrvDo028_Meteor_Blizzard_Eruption_BaalTaunt_Catapult,
    SKILLS_SrvDo029_ThunderStorm,
    SKILLS_SrvDo030_Curse,
    SKILLS_SrvDo031_RaiseSkeleton_Mage,
    SKILLS_SrvDo032_PoisonDagger,
    SKILLS_SrvDo033_PsychicHammer,
    SKILLS_SrvDo034_TigerStrike_CobraStrike_RoyalStrike,
    SKILLS_SrvDo035_FistsOfFire_ClawsOfThunder_BladesOfIce,
    SKILLS_SrvDo036_ClawsOfThunder_ProgressiveFn2,
    SKILLS_SrvDo037_ClawsOfThunder_ProgressiveFn3,
    SKILLS_SrvDo038_FistsOfFire_BladesOfIce_ProgressiveFn2,
    SKILLS_SrvDo039_FistsOfFire_BladesOfIce_ProgressiveFn3,
    SKILLS_SrvDo040_RoyalStrike_ProgressiveFn1,
    SKILLS_SrvDo041_RoyalStrike_ProgressiveFn3,
    SKILLS_SrvDo042_DragonTalon,
    SKILLS_SrvDo043_ShockField,
    SKILLS_SrvDo044_BladeSentinel,
    SKILLS_SrvDo045_Sentry,
    SKILLS_SrvDo046_DragonClaw,
    SKILLS_SrvDo047_CloakOfShadows,
    SKILLS_SrvDo048_BladeFury,
    SKILLS_SrvDo049_ShadowWarrior_Master,
    SKILLS_SrvDo050_DragonTail,
    SKILLS_SrvDo051_MindBlast,
    SKILLS_SrvDo052_DragonFlight,
    SKILLS_SrvDo053_Unused,
    SKILLS_SrvDo054_BladeShield,
    SKILLS_SrvDo055_CorpseExplosion,
    SKILLS_SrvDo056_Golem,
    SKILLS_SrvDo057_IronGolem,
    SKILLS_SrvDo058_Revive,
    SKILLS_SrvDo059_Attract,
    SKILLS_SrvDo060_BoneWall,
    SKILLS_SrvDo061_Confuse,
    SKILLS_SrvDo062_BonePrison,
    SKILLS_SrvDo063_PoisonExplosion,
    SKILLS_SrvDo064_Sacrifice,
    SKILLS_SrvDo065_BasicAura,
    SKILLS_SrvDo066_HolyFire_HolyShock_Sanctuary_Conviction,
    SKILLS_SrvDo067_Charge,
    SKILLS_SrvDo068_BasicShout,
    SKILLS_SrvDo069_FindPotion,
    SKILLS_SrvDo070_DoubleSwing,
    SKILLS_SrvDo071_Taunt,
    SKILLS_SrvDo072_FindItem,
    SKILLS_SrvDo073_BlessedHammer,
    SKILLS_SrvDo074_DoubleThrow,
    SKILLS_SrvDo075_GrimWard,
    SKILLS_SrvDo076_Whirlwind,
    SKILLS_SrvDo077_Leap,
    SKILLS_SrvDo078_LeapAttack,
    SKILLS_SrvDo079_Conversion,
    SKILLS_SrvDo080_FistOfTheHeavens,
    SKILLS_SrvDo081_HolyFreeze,
    SKILLS_SrvDo082_Redemption,
    SKILLS_SrvDo083_FireHit,
    SKILLS_SrvDo084_MaggotEgg,
    SKILLS_SrvDo085_UnholyBolt_ShamanFire,
    SKILLS_SrvDo086_MaggotDown,
    SKILLS_SrvDo087_MaggotLay,
    SKILLS_SrvDo088_AndrialSpray,
    SKILLS_SrvDo089_Jump,
    SKILLS_SrvDo090_SwarmMove,
    SKILLS_SrvDo091_Nest_EvilHutSpawner,
    SKILLS_SrvDo092_QuickStrike,
    SKILLS_SrvDo093_GargoyleTrap,
    SKILLS_SrvDo094_Submerge,
    SKILLS_SrvDo095_MonInferno,
    SKILLS_SrvDo096_ZakarumHeal_Bestow,
    SKILLS_SrvDo097_Resurrect,
    SKILLS_SrvDo098_MonTeleport,
    SKILLS_SrvDo099_PrimePoisonNova,
    SKILLS_SrvDo100_DiabCold,
    SKILLS_SrvDo101_FingerMageSpider,
    SKILLS_SrvDo102_DiabWall,
    SKILLS_SrvDo103_DiabRun,
    SKILLS_SrvDo104_DiabPrison,
    SKILLS_SrvDo105_DesertTurret,
    SKILLS_SrvDo106_ArcaneTower,
    SKILLS_SrvDo107_Mosquito,
    SKILLS_SrvDo108_RegurgitatorEat,
    SKILLS_SrvDo109_MonFrenzy,
    SKILLS_SrvDo110_Hireable_RogueMissile,
    SKILLS_SrvDo111_FetishAura,
    SKILLS_SrvDo112_MonCurseCast,
    SKILLS_SrvDo113_Scroll_Book,
    SKILLS_SrvDo114_Raven,
    SKILLS_SrvDo115_Vines,
    SKILLS_SrvDo116_Wearwolf_Wearbear,
    SKILLS_SrvDo117_Firestorm,
    SKILLS_SrvDo118_Twister_Tornado,
    SKILLS_SrvDo119_DruidSummon,
    SKILLS_SrvDo120_FeralRage_Maul,
    SKILLS_SrvDo121_Rabies,
    SKILLS_SrvDo122_Hunger,
    SKILLS_SrvDo123_Volcano,
    SKILLS_SrvDo124_Armageddon_Hurricane,
    SKILLS_SrvDo125_WakeOfDestruction,
    SKILLS_SrvDo126_ImpInferno,
    SKILLS_SrvDo127_SuckBlood,
    SKILLS_SrvDo128_CryHelp,
    SKILLS_SrvDo129_ImpTeleport,
    SKILLS_SrvDo130_VineAttack,
    SKILLS_SrvDo131_OverseerWhip,
    SKILLS_SrvDo132_ImpFireMissile,
    SKILLS_SrvDo133_Impregnate,
    SKILLS_SrvDo134_SiegeBeastStomp,
    SKILLS_SrvDo135_MinionSpawner,
    SKILLS_SrvDo136_DeathMaul,
    SKILLS_SrvDo137_FenrisRage,
    SKILLS_SrvDo138_Unused,
    SKILLS_SrvDo139_BaalColdMissiles,
    SKILLS_SrvDo140_BaalTentacle,
    SKILLS_SrvDo141_BaalCorpseExplode,
    SKILLS_SrvDo142_Unused,
    SKILLS_SrvDo143_FistsOfFire_RoyalStrike_ProgressiveFn,
    SKILLS_SrvDo144_Hydra,
    SKILLS_SrvDo145_Unused,
    SKILLS_SrvDo146_Unused,
    SKILLS_SrvDo147_Unused,
    SKILLS_SrvDo148_DoomKnightMissile,
    SKILLS_SrvDo149_NecromageMissile,
    SKILLS_SrvDo150_Smite,
    SKILLS_SrvDo151_Unused,
    SKILLS_SrvDo152_DiabLight,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
static const int32_t gnSkillSrvDoFns = std::size(gpSkillSrvDoFnTable_6FD40A20);



//D2Game.0x6FD0F8B0
int32_t __fastcall sub_6FD0F8B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    if (!pUnit)
    {
        return nValue;
    }
    
    D2UnitStrc* pBloodGolem = sub_6FC7E8B0(pGame, pUnit, 3, 0);
    if (!pBloodGolem || pBloodGolem->dwUnitType != UNIT_MONSTER || pBloodGolem->dwClassId != MONSTER_BLOODGOLEM)
    {
        return nValue;
    }

    if (!nValue)
    {
        STATLIST_SetUnitStat(pBloodGolem, STAT_HITPOINTS, STATLIST_GetMaxLifeFromUnit(pBloodGolem), 0);
        return nValue;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(MONSTER_BLOODGOLEM);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pMonStatsTxtRecord->nSkill[0]);
    if (!pSkillsTxtRecord)
    {
        return nValue;
    }

    int32_t nLifeAdded = nValue * pSkillsTxtRecord->dwParam[5] / 100;
    if (nLifeAdded > 0)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pBloodGolem);
        int32_t nNewHp = nLifeAdded + STATLIST_UnitGetStatValue(pBloodGolem, STAT_HITPOINTS, 0);
        if (nNewHp > nMaxHp)
        {
            nLifeAdded += nMaxHp - nNewHp;
            nNewHp = nMaxHp;
        }

        STATLIST_SetUnitStat(pBloodGolem, STAT_HITPOINTS, nNewHp, 0);
        return nValue - nLifeAdded;
    }

    return nValue;
}

//D2Game.0x6FD0FA00
int32_t __fastcall sub_6FD0FA00(D2UnitStrc* pUnit, D2UnitStrc* pTarget, uint32_t nAuraFilter)
{
    D2UnitStrc* pAttacker = pUnit;
    if (!pTarget)
    {
        return 0;
    }

    switch (pTarget->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        if (!(nAuraFilter & 1))
        {
            return 0;
        }

        if (nAuraFilter & 0x1000)
        {
            if (pTarget->dwAnimMode != PLRMODE_DEAD)
            {
                return 0;
            }
        }
        else
        {
            if (pTarget->dwAnimMode == PLRMODE_DEAD || pTarget->dwAnimMode == PLRMODE_DEATH)
            {
                return 0;
            }
        }
        break;
    }
    case UNIT_MONSTER:
    {
        if (!(nAuraFilter & 2))
        {
            return 0;
        }

        if (nAuraFilter & 0x1000)
        {
            if (pTarget->dwAnimMode != MONMODE_DEAD)
            {
                return 0;
            }
        }
        else
        {
            if (pTarget->dwAnimMode == MONMODE_DEAD || pTarget->dwAnimMode == MONMODE_DEATH)
            {
                return 0;
            }
        }

        if (nAuraFilter & 4 && !MONSTERS_IsUndead(pTarget) || nAuraFilter & 0x4000 && MONSTERS_IsBoss(nullptr, pTarget) || nAuraFilter & 0x40000 && MONSTERS_IsPrimeEvil(pTarget))
        {
            return 0;
        }
        break;
    }
    case UNIT_OBJECT:
    {
        if (!(nAuraFilter & 0x10))
        {
            return 0;
        }
        break;
    }
    case UNIT_MISSILE:
    {
        if (!(nAuraFilter & 8))
        {
            return 0;
        }

        D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pTarget->dwClassId);
        if (!pMissilesTxtRecord || pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[1])
        {
            return 0;
        }
        break;
    }
    case UNIT_ITEM:
    {
        if (!(nAuraFilter & 0x20))
        {
            return 0;
        }

        break;
    }
    default:
        return 0;
    }

    if (nAuraFilter & 0x80 && !(pTarget->dwFlags & UNITFLAG_CANBEATTACKED))
    {
        return 0;
    }

    if (nAuraFilter & 0x400 && !(pTarget->dwFlags & UNITFLAG_ISVALIDTARGET))
    {
        return 0;
    }

    if (nAuraFilter & 0x100 && DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)))
    {
        return 0;
    }

    if (nAuraFilter & 0x10000)
    {
        if (pAttacker && pAttacker->dwUnitType == UNIT_MISSILE)
        {
            D2UnitStrc* pOwner = SUNIT_GetOwner(SUNIT_GetGameFromUnit(pAttacker), pAttacker);
            if (!pOwner)
            {
                return 0;
            }

            pAttacker = pOwner;
        }

        if (!SUNIT_AreUnitsAligned(SUNIT_GetGameFromUnit(pAttacker), pAttacker, pTarget))
        {
            return 0;
        }

        if (pTarget->dwUnitType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
            if (!pMonStatsTxtRecord || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC])
            {
                return 0;
            }
        }
    }

    if (nAuraFilter & 0x8000)
    {
        if (pAttacker && pAttacker->dwUnitType == UNIT_MISSILE)
        {
            D2UnitStrc* pOwner = SUNIT_GetOwner(SUNIT_GetGameFromUnit(pAttacker), pAttacker);
            if (!pOwner)
            {
                return 0;
            }

            pAttacker = pOwner;
        }

        if (!sub_6FCBD900(SUNIT_GetGameFromUnit(pAttacker), pAttacker, pTarget))
        {
            return 0;
        }
    }

    if (nAuraFilter & 0x20000)
    {
        if (pAttacker && pAttacker->dwUnitType == UNIT_MISSILE)
        {
            D2UnitStrc* pOwner = SUNIT_GetOwner(SUNIT_GetGameFromUnit(pAttacker), pAttacker);
            if (!pOwner)
            {
                return 0;
            }

            pAttacker = pOwner;
        }

        if (!UNITS_IsInMeleeRange(pAttacker, pTarget, 0))
        {
            return 0;
        }
    }

    if (nAuraFilter & 0x80000 && STATES_CheckState(pTarget, STATE_JUSTHIT))
    {
        return 0;
    }

    if (nAuraFilter & 0x200)
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pAttacker);
        if (!pRoom || !UNITS_GetRoom(pTarget))
        {
            return 0;
        }

        D2CoordStrc coords = {};
        coords.nX = CLIENTS_GetUnitX(pAttacker);
        coords.nY = CLIENTS_GetUnitY(pAttacker);

        D2CoordStrc targetCoords = {};
        targetCoords.nX = CLIENTS_GetUnitX(pTarget);
        targetCoords.nY = CLIENTS_GetUnitY(pTarget);
        
        if (COLLISION_RayTrace(pRoom, &coords, &targetCoords, 4u))
        {
            return 0;
        }
    }

    return 1;
}

//D2Game.0x6FD0FDD0
void __fastcall sub_6FD0FDD0(D2UnitStrc* pUnit)
{
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) > nMaxHp)
    {
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxHp, 0);
    }

    const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) > nMaxMana)
    {
        STATLIST_SetUnitStat(pUnit, STAT_MANA, nMaxMana, 0);
    }

    const int32_t nMaxStamina = STATLIST_GetMaxStaminaFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0) > nMaxStamina)
    {
        STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nMaxStamina, 0);
    }

    UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
}

//D2Game.0x6FD0FE50
void __fastcall sub_6FD0FE50(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t (__fastcall* pCallback)(D2AuraCallbackStrc*, D2UnitStrc*), void* pArgs)
{
    D2AuraCallbackStrc auraCallback = {};
    auraCallback.pGame = pGame;
    auraCallback.pOwner = pUnit;
    auraCallback.nCounter = 0;
    auraCallback.pArgs = pArgs;

    pCallback(&auraCallback, pUnit);
}

//D2Game.0x6FD0FE80
void __fastcall sub_6FD0FE80(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, uint32_t nAuraFilter, int32_t(__fastcall* pCallback)(D2AuraCallbackStrc*, D2UnitStrc*), void* pCallbackArgs, int32_t bCheckMonAuraFlag, const char* szFile, int32_t nLine)
{
    if (!pGame || !pUnit || !pCallback || nAuraRange <= 0)
    {
        return;
    }

    D2ActiveRoomStrc* pCurrentRoom = UNITS_GetRoom(pUnit);
    if (!pCurrentRoom)
    {
        return;
    }

    if (!nX || !nY)
    {
        nX = CLIENTS_GetUnitX(pUnit);
        nY = CLIENTS_GetUnitY(pUnit);
    }

    if (!nAuraFilter)
    {
        nAuraFilter = 0x583u;
    }

    D2AuraCallbackStrc auraCallback = {};
    auraCallback.pArgs = pCallbackArgs;
    auraCallback.pOwner = pUnit;
    auraCallback.pGame = pGame;
    auraCallback.nCounter = 0;

    D2ActiveRoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pCurrentRoom, &ppRoomList, &nNumRooms);
    for (int32_t i = 0; i < nNumRooms; ++i)
    {
        D2ActiveRoomStrc* pRoom = ppRoomList[i];
        if (!(nAuraFilter & 0x2000) || !DUNGEON_IsRoomInTown(pRoom))
        {
            D2DrlgCoordsStrc drlgCoords = {};
            DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

            if ((nX + nAuraRange >= drlgCoords.nSubtileX || nX - nAuraRange <= drlgCoords.nSubtileX + drlgCoords.nSubtileWidth)
                && (nY + nAuraRange >= drlgCoords.nSubtileY || nY - nAuraRange <= drlgCoords.nSubtileY + drlgCoords.nSubtileHeight))
            {
                D2UnitStrc* pTarget = pRoom->pUnitFirst;
                while (pTarget)
                {
                    D2UnitStrc* pNextTarget = pTarget->pRoomNext;
                    if (pTarget != pUnit && nAuraRange * nAuraRange >= PATH_ComputeSquaredDistance(nX, nY, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget)) && sub_6FD0FA00(pUnit, pTarget, nAuraFilter))
                    {
                        if (!bCheckMonAuraFlag || pTarget->dwUnitType != UNIT_MONSTER)
                        {
                            if (pCallback(&auraCallback, pTarget))
                            {
                                ++auraCallback.nCounter;
                            }
                        }
                        else
                        {
                            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
                            if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NOAURA]))
                            {
                                if (pCallback(&auraCallback, pTarget))
                                {
                                    ++auraCallback.nCounter;
                                }
                            }
                        }
                    }
                    pTarget = pNextTarget;
                }
            }
        }
    }
}

//D2Game.0x6FD10140
void __fastcall sub_6FD10140(D2GameStrc* pGame, D2UnitStrc* pAttackerArg, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nUnused)
{
    D2UnitStrc* pAttacker = pAttackerArg;

    if (pAttackerArg && pUnit)
    {
        if (pAttackerArg->dwUnitType == UNIT_MISSILE)
        {
            D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pAttackerArg);
            if (pOwner)
            {
                pAttacker = pOwner;
            }
        }

        if (pDamage->wResultFlags)
        {
            const int32_t nBlockFlags = SUNITDMG_ApplyBlockOrDodge(pGame, pAttacker, pUnit, 1, 0);
            if (nBlockFlags & 2)
            {
                pDamage->wResultFlags |= 100u;
            }

            if (nBlockFlags & 4)
            {
                pDamage->wResultFlags |= 0x80u;
            }

            if (nBlockFlags & 0x10)
            {
                pDamage->wResultFlags |= 0x8000u;
            }

            if (nBlockFlags & 1)
            {
                pDamage->wResultFlags |= 0x10u;
            }

            if (nBlockFlags)
            {
                pDamage->wResultFlags &= 0xFEu;
            }

            if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT && !STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
            {
                pDamage->wResultFlags |= 4u;
            }

            D2GAME_MONSTER_ApplyCriticalDamage_6FC62E70(pAttacker, pUnit, pDamage);

            if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
            {
                SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, pDamage);
            }

            SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, pDamage);
        }
    }
}

//D2Game.0x6FD10200
int32_t __fastcall sub_6FD10200(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, D2DamageStrc* pDamage, uint32_t nAuraFilter)
{
    if (!nAuraFilter)
    {
        nAuraFilter = 0x8583u;
    }

    if (pUnit)
    {
        sub_6FD0FE80(pGame, pUnit, nX, nY, nAuraRange, nAuraFilter, sub_6FCF5DE0, pDamage, 0, __FILE__, __LINE__);
    }

    return 1;
}

//D2Game.0x6FD10250
void __fastcall sub_6FD10250(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nState, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        const int32_t nAuraLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);

        int32_t nUnitGUID = -1;
        int32_t nUnitType = 6;
        if (pUnit)
        {
            nUnitGUID = pUnit->dwUnitId;
            nUnitType = pUnit->dwUnitType;
        }

        D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nAuraLength + pGame->dwGameFrame, nUnitType, nUnitGUID);
        if (pStatList)
        {
            STATLIST_SetState(pStatList, nState);
            STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
            D2COMMON_10475_PostStatToStatList(pTarget, pStatList, 1);
            STATES_ToggleState(pTarget, nState, 1);
            EVENT_SetEvent(pGame, pTarget, UNITEVENTCALLBACK_REMOVESTATE, nAuraLength + pGame->dwGameFrame, 0, 0);
            sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
        }
    }
}

//D2Game.0x6FD10360
int32_t __fastcall sub_6FD10360(D2GameStrc* pGame, uint32_t nFlags, D2UnitStrc* pUnit, int32_t nDistance, int32_t(__fastcall* pCallback)(D2UnitStrc*, void*), void* pArgs)
{
    if (!pCallback)
    {
        return 0;
    }

    int32_t nX = 0;
    int32_t nY = 0;

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        nX = CLIENTS_GetUnitX(pTarget);
        nY = CLIENTS_GetUnitY(pTarget);
    }
    else
    {
        nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }

    if (!nX || !nY)
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
    if (!pRoom)
    {
        return 0;
    }

    D2ActiveRoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

    for (int32_t i = 0; i < nNumRooms; ++i)
    {
        if (!(nFlags & 0x2000) || !DUNGEON_IsRoomInTown(ppRoomList[i]))
        {
            D2DrlgCoordsStrc drlgCoords = {};
            DUNGEON_GetRoomCoordinates(ppRoomList[i], &drlgCoords);
            if (nX + nDistance >= drlgCoords.nSubtileX || nX - nDistance <= drlgCoords.nSubtileX + drlgCoords.nSubtileWidth)
            {
                if (nY + nDistance >= drlgCoords.nSubtileY || nY - nDistance <= drlgCoords.nSubtileY + drlgCoords.nSubtileHeight)
                {
                    for (D2UnitStrc* j = ppRoomList[i]->pUnitFirst; j; j = j->pRoomNext)
                    {
                        if (nDistance * nDistance >= PATH_ComputeSquaredDistance(nX, nY, CLIENTS_GetUnitX(j), CLIENTS_GetUnitY(j)) && sub_6FD0FA00(pUnit, j, nFlags))
                        {
                            pCallback(j, pArgs);
                        }
                    }
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FD10630
D2UnitStrc* __fastcall SKILLS_FindAuraTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nAuraRange, int32_t(__fastcall* pCallback)(D2GameStrc*, D2UnitStrc*))
{
    D2_6FD106F0_Strc arg = {};

    arg.pUnit = nullptr;
    arg.nDistance = INT_MAX;
    arg.nX = CLIENTS_GetUnitX(pUnit);
    arg.nY = CLIENTS_GetUnitY(pUnit);
    arg.pCallback = pCallback;

    sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, 0x8783u, sub_6FD106F0, &arg, 0, __FILE__, __LINE__);

    return arg.pUnit;
}

//D2Game.0x6FD106F0
int32_t __fastcall sub_6FD106F0(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2_6FD106F0_Strc* pArgs = (D2_6FD106F0_Strc*)pAuraCallback->pArgs;

    if (!pArgs->pCallback || pArgs->pCallback(pAuraCallback->pGame, pUnit))
    {
        const int32_t nXDiff = CLIENTS_GetUnitX(pUnit) - pArgs->nX;
        const int32_t nYDiff = CLIENTS_GetUnitY(pUnit) - pArgs->nY;

        const int32_t nDistance = nXDiff * nXDiff + nYDiff * nYDiff;
        if (nDistance < pArgs->nDistance)
        {
            pArgs->nDistance = nDistance;
            pArgs->pUnit = pUnit;
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FD10790
int32_t __fastcall sub_6FD10790(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, uint32_t nAuraFilter)
{
    int32_t nSkillId = 0;

    sub_6FD0FE80(pGame, pUnit, nX, nY, nAuraRange, nAuraFilter | 0xA783, sub_6FD107E0, &nSkillId, 0, __FILE__, __LINE__);

    return nSkillId;
}

//D2Game.0x6FD107E0
int32_t __fastcall sub_6FD107E0(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    ++*(int32_t*)pAuraCallback->pArgs;
    return 1;
}

//D2Game.0x6FD107F0
D2UnitStrc* __fastcall sub_6FD107F0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, uint32_t nAuraFilter, int32_t a7, int32_t* a8)
{
    // TODO: Names
    D2_6FD10880_Strc args = {};

    args.unk0x0C = a7;
    args.unk0x10 = a7;
    args.unk0x00 = 0;
    args.unk0x04 = -1;
    args.unk0x08 = 0;
    args.unk0x14 = 0;
    sub_6FD0FE80(pGame, pUnit, nX, nY, nAuraRange, nAuraFilter | 0xA783, sub_6FD10880, &args, 0, __FILE__, __LINE__);

    if (!args.unk0x00)
    {
        return args.unk0x08;
    }
    
    if (a8)
    {
        *a8 = args.unk0x14;
    }

    return args.unk0x00;
}

//D2Game.0x6FD10880
int32_t __fastcall sub_6FD10880(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2_6FD10880_Strc* pArgs = (D2_6FD10880_Strc*)pAuraCallback->pArgs;

    int32_t nUnitGUID = -1;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
    }

    if ((uint32_t)nUnitGUID <= pArgs->unk0x10 || (uint32_t)nUnitGUID >= pArgs->unk0x04)
    {
        if ((uint32_t)nUnitGUID <= pArgs->unk0x0C)
        {
            pArgs->unk0x0C = nUnitGUID;
            pArgs->unk0x08 = pUnit;
        }
        ++pArgs->unk0x14;
    }
    else
    {
        pArgs->unk0x04 = nUnitGUID;
        pArgs->unk0x00 = pUnit;
        ++pArgs->unk0x14;
    }

    return 1;
}

D2PetTypeTxt* __fastcall DATATBLS_GetPetTypeTxtRecord(int32_t nPetType)
{
    if (nPetType >= 0 && nPetType < sgptDataTables->nPetTypeTxtRecordCount)
    {
        return &sgptDataTables->pPetTypeTxt[nPetType];
    }

    return nullptr;
}

//D2Game.0x6FD108D0
void __fastcall sub_6FD108D0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord)
        {
            D2PetTypeTxt* pPetTypeTxtRecord = DATATBLS_GetPetTypeTxtRecord(pSkillsTxtRecord->nPetType);
            if (pPetTypeTxtRecord)
            {
                int32_t nMaxPets = 0;
                for (int32_t i = 0; i < pPetTypeTxtRecord->nSkillCount; ++i)
                {
                    D2SkillsTxt* pTemp = SKILLS_GetSkillsTxtRecord(pPetTypeTxtRecord->wSkillIds[i]);
                    if (pTemp)
                    {
                        D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pPetTypeTxtRecord->wSkillIds[i]);
                        if (pSkill)
                        {
                            const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
                            int32_t nPetMax = 0;
                            if (nSkillLevel > 0)
                            {
                                nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pTemp->dwPetMax, pPetTypeTxtRecord->wSkillIds[i], nSkillLevel);
                                if (nPetMax < 1)
                                {
                                    nPetMax = 1;
                                }
                            }

                            if (nMaxPets < nPetMax)
                            {
                                nMaxPets = nPetMax;
                            }
                        }
                    }

                }

                sub_6FC7D150(pGame, pUnit, pSkillsTxtRecord->nPetType, nMaxPets);
            }
        }
    }
}

//D2Game.0x6FD10A50
int32_t __fastcall D2GAME_SKILLMANA_Consume_6FD10A50(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return 1;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pPlayer);
    if (!pSkill || SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) != nSkillId || SKILLS_GetOwnerGUIDFromSkill(pSkill) == -1)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 0;
        }

        if (!pSkillsTxtRecord->wMana && !pSkillsTxtRecord->wLevelMana)
        {
            return 0;
        }

        int32_t nManaCost = 0;
        if (nSkillLevel > 1)
        {
            nManaCost = (pSkillsTxtRecord->wMana + (nSkillLevel - 1) * pSkillsTxtRecord->wLevelMana) << pSkillsTxtRecord->nManaShift;
        }

        const int32_t nMinManaCost = pSkillsTxtRecord->wMinMana << 8;
        if (nManaCost < nMinManaCost)
        {
            nManaCost = nMinManaCost;
        }

        if (STATES_CheckState(pPlayer, STATE_BLOOD_MANA))
        {
            return D2GAME_SKILLS_BloodMana_6FD025E0(pPlayer, nManaCost);
        }

        if (STATLIST_UnitGetStatValue(pPlayer, STAT_MANA, 0) < nManaCost)
        {
            return 0;
        }

        STATLIST_AddUnitStat(pPlayer, STAT_MANA, -nManaCost, 0);
        return 1;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, SKILLS_GetOwnerGUIDFromSkill(pSkill));
    if (!pItem)
    {
        return 0;
    }
    
    const int32_t nChargeSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    const int32_t nChargeSkillLevel = SKILLS_GetSkillLevel(pPlayer, pSkill, 0);

    int32_t nCharges = 0;
    D2StatListStrc* pStatList = nullptr;
    if (!ITEMMODS_GetItemCharges(pItem, nChargeSkillId, nChargeSkillLevel, &nCharges, &pStatList) || nCharges <= 0)
    {
        return 0;
    }
    
    const int32_t nLayer = (nChargeSkillLevel & sgptDataTables->nShiftedStuff) + (nChargeSkillId << sgptDataTables->nStuff);
    const int32_t nStatValue = STATLIST_GetStatValue(pStatList, STAT_ITEM_CHARGED_SKILL, nLayer);
    if (!nStatValue)
    {
        return 0;
    }
    
    const int32_t nMaxCharges = nStatValue >> 8;
    if (nMaxCharges <= 0 || nMaxCharges > 255)
    {
        return 0;
    }

    --nCharges;

    if (nCharges < 0)
    {
        nCharges = 0;
    }
    else if (nCharges >= nMaxCharges)
    {
        nCharges = nMaxCharges;
    }

    const int32_t nNewValue = (uint8_t)nCharges + (nMaxCharges << 8);
    STATLIST_SetBaseStat2(pStatList, STAT_ITEM_CHARGED_SKILL, nNewValue, nLayer, pItem);
    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pItem, 1, STAT_ITEM_CHARGED_SKILL, nNewValue, nLayer);
    return 1;
}

//D2Game.0x6FD10C90
int32_t __fastcall D2GAME_SKILLMANA_AuraConsume_6FD10C90(D2UnitStrc* pUnit, int32_t nManaCost)
{
    if (pUnit && !pUnit->dwUnitType)
    {
        if (STATES_CheckState(pUnit, STATE_BLOOD_MANA))
        {
            return D2GAME_SKILLS_BloodMana_6FD025E0(pUnit, nManaCost);
        }

        if ((int32_t)STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) < nManaCost)
        {
            return 0;
        }

        STATLIST_AddUnitStat(pUnit, STAT_MANA, -nManaCost, 0);
    }

    return 1;
}

//D2Game.0x6FD10CE0
int32_t __fastcall sub_6FD10CE0(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 1;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
        
    if (SKILLS_GetOwnerGUIDFromSkill(pSkill) == -1)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
        if (!pSkillsTxtRecord)
        {
            return 0;
        }

        int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1) - 1;
        if (nSkillLevel < 0)
        {
            nSkillLevel = 0;
        }

        const int32_t nManaCost = (pSkillsTxtRecord->wMana + nSkillLevel * pSkillsTxtRecord->wLevelMana) << pSkillsTxtRecord->nManaShift;
        if (pSkillsTxtRecord->wSrvDoFunc == 116 && STATES_IsUnitShapeShifted(pUnit))
        {
            return 1;
        }
        
        return STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) >= nManaCost;
    }

    return pSkill->nCharges > 0;
}

//D2Game.0x6FD10D90
D2UnitFindDataStrc* __fastcall sub_6FD10D90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSize, int32_t nX, int32_t nY)
{
    D2UnitFindDataStrc* pUnitFindData = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2UnitFindDataStrc);

    D2UnitFindArgStrc unitFindArg = {};
    unitFindArg.nFlags = 0x583;
    unitFindArg.pUnit = pUnit;
    unitFindArg.nX = nX;
    unitFindArg.nY = nY;
    unitFindArg.nSize = nSize;

    UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, pUnitFindData, UNITS_GetRoom(pUnit), nX, nY, nSize, 0, &unitFindArg);
    UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(pUnitFindData);

    return pUnitFindData;
}

//D2Game.0x6FD10E20
void __fastcall sub_6FD10E20(D2GameStrc* pGame, D2UnitFindDataStrc* pUnitFindData)
{
    UNITFINDS_FreeUnitFindData(pUnitFindData);
    D2_FREE_POOL(pGame->pMemoryPool, pUnitFindData);
}

//D2Game.0x6FD10E50
void __fastcall sub_6FD10E50(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    SUNITEVENT_FreeTimer(SUNIT_GetGameFromUnit(pUnit), pUnit, 1, nState);

    if (!SUNIT_IsDead(pUnit) || !STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, nState))
    {
        STATES_ToggleState(pUnit, nState, 0);
        UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
        SKILLS_RefreshPassiveSkills(pUnit);

        if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
        {
            sub_6FC7DF40(SUNIT_GetGameFromUnit(pUnit), pUnit);
        }
    }
}

//D2Game.0x6FD10EC0
D2StatListStrc* __fastcall sub_6FD10EC0(D2CurseStrc* pCurse)
{
    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pCurse->pTarget);

    D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(pCurse->nState);
    if (!pStatesTxtRecord)
    {
        return 0;
    }

    if (pCurse->pTarget && pCurse->pTarget->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pCurse->pTarget->dwClassId);
        if (!pMonStatsTxtRecord || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC])
        {
            return 0;
        }

        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pCurse->pTarget->dwClassId);
        if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_ISATT]))
        {
            return 0;
        }
    }

    const int32_t nStateMask = STATES_CheckStateMaskCurseByStateId(pCurse->nState);
    if (nStateMask)
    {
        const int32_t nCurseResistance = STATLIST_UnitGetStatValue(pCurse->pTarget, STAT_CURSE_RESISTANCE, 0);
        if (nCurseResistance >= 100)
        {
            return 0;
        }

        if (nCurseResistance)
        {
            pCurse->nDuration -= MONSTERUNIQUE_CalculatePercentage(pCurse->nDuration, nCurseResistance, 100);
        }

        if (STATES_CheckState(pCurse->pTarget, STATE_ATTRACT))
        {
            return 0;
        }
    }

    uint32_t nStatListFlags = 0;
    D2StatListStrc* pStatList = nullptr;
    if (nStateMask)
    {
        nStatListFlags = 0x20u;
        pStatList = STATLIST_GetStatListFromUnitAndFlag(pCurse->pTarget, 0x20u);
    }
    else
    {
        pStatList = STATLIST_GetStatListFromUnitAndState(pCurse->pTarget, pCurse->nState);
    }

    if (pStatList)
    {
        if (STATLIST_GetState(pStatList) == pCurse->nState && pCurse->nSkill == STATLIST_GetSkillId(pStatList))
        {
            const int32_t nSkillLevel = STATLIST_GetSkillLevel(pStatList);
            if (pCurse->nSkillLevel == nSkillLevel)
            {
                if (pCurse->nDuration)
                {
                    STATLIST_SetExpireFrame(pStatList, pGame->dwGameFrame + pCurse->nDuration);
                    EVENT_SetEvent(pGame, pCurse->pTarget, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + pCurse->nDuration, 0, 0);
                }

                return pStatList;
            }

            if (pCurse->nSkillLevel < nSkillLevel)
            {
                return nullptr;
            }
        }

        D2Common_10474(pCurse->pTarget, pStatList);
        STATLIST_FreeStatList(pStatList);
    }

    UNITROOM_RefreshUnit(pCurse->pTarget);

    STATES_ToggleState(pCurse->pTarget, pCurse->nState, 1);

    int32_t nExpireFrame = 0;
    if (pCurse->nDuration)
    {
        nStatListFlags |= 2u;
        nExpireFrame = pGame->dwGameFrame + pCurse->nDuration;
        EVENT_SetEvent(pGame, pCurse->pTarget, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + pCurse->nDuration, 0, 0);
    }

    if (STATES_CheckStateMaskExpByStateId(pCurse->nState))
    {
        nStatListFlags |= 0x800u;
    }

    if (pStatesTxtRecord->nStateFlags[0] & gdwBitMasks[1])
    {
        nStatListFlags |= 8u;
    }

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pCurse->pUnit)
    {
        nUnitGUID = pCurse->pUnit->dwUnitId;
        nUnitType = pCurse->pUnit->dwUnitType;
    }

    pStatList = STATLIST_AllocStatList(pCurse->pTarget ? pCurse->pTarget->pMemoryPool : nullptr, nStatListFlags, nExpireFrame, nUnitType, nUnitGUID);
    STATLIST_SetState(pStatList, pCurse->nState);
    STATLIST_SetSkillId(pStatList, pCurse->nSkill);
    STATLIST_SetSkillLevel(pStatList, pCurse->nSkillLevel);

    if (pCurse->nStat != -1)
    {
        STATLIST_SetStatIfListIsValid(pStatList, pCurse->nStat, pCurse->nStatValue, 0);
    }

    D2COMMON_10475_PostStatToStatList(pCurse->pTarget, pStatList, 1);

    if (pCurse->pStateRemoveCallback)
    {
        STATLIST_SetStatRemoveCallback(pStatList, pCurse->pStateRemoveCallback);
    }
    else
    {
        STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
    }

    return pStatList;
}

//D2Game.0x6FD11260
D2StatListStrc* __fastcall sub_6FD11260(D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel, int32_t nStatId, int32_t nState, StatListRemoveCallback pfRemoveStatCallback)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

    D2CurseStrc curse = {};
    curse.nSkillLevel = nSkillLevel;
    curse.nDuration = nSkillLevel * pSkillsTxtRecord->dwParam[1];
    curse.pTarget = pTarget;
    curse.nStat = nStatId;
    curse.pUnit = pUnit;
    curse.nSkill = nSkillId;
    curse.nStatValue = pSkillsTxtRecord->dwParam[0] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[2];
    curse.nState = nState;
    curse.pStateRemoveCallback = pfRemoveStatCallback;

    if (pfRemoveStatCallback && IsBadCodePtr((FARPROC)pfRemoveStatCallback))
    {
        FOG_DisplayAssert("pfnRemove", __FILE__, __LINE__);
        exit(-1);
    }

    return sub_6FD10EC0(&curse);
}

//D2Game.0x6FD11340
int32_t __fastcall sub_6FD11340(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    if (!pItem)
    {
        return 0;
    }

    int32_t bNoQuantity = 0;
    int32_t nQuantity = STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0) - 1;
    if (nQuantity < 0)
    {
        bNoQuantity = 1;
        nQuantity = 0;
    }

    STATLIST_SetUnitStat(pItem, STAT_QUANTITY, nQuantity, 0);
    sub_6FC51310(pGame, pItem);

    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, nQuantity, 0);

    const int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
    if (STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) != nMaxDurability)
    {
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nMaxDurability, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, nMaxDurability, 0);
    }

    sub_6FC80A30(pGame, pUnit);

    if (bNoQuantity)
    {
        return 0;
    }

    return nQuantity + 1;
}

//D2Game.0x6FD11420
D2UnitStrc* __fastcall sub_6FD11420(D2GameStrc* pGame, int32_t nMissileId, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t bCheckItemQuantity)
{
    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 33;
    missileParams.pOwner = pUnit;
    missileParams.nX = nX + CLIENTS_GetUnitX(pUnit);
    missileParams.nY = nY + CLIENTS_GetUnitY(pUnit);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = nMissileId;

    if (nTargetX && nTargetY)
    {
        missileParams.nTargetX = nTargetX;
        missileParams.nTargetY = nTargetY;
    }
    else
    {
        D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTargetUnit)
        {
            missileParams.nTargetX = CLIENTS_GetUnitX(pTargetUnit);
            missileParams.nTargetY = CLIENTS_GetUnitY(pTargetUnit);
        }
        else
        {
            missileParams.nTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
            missileParams.nTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
        }

        if (!missileParams.nTargetX || !missileParams.nTargetY)
        {
            return 0;
        }
    }

    if (missileParams.nTargetX && missileParams.nTargetY)
    {
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            const int32_t nToHit = STATLIST_UnitGetStatValue(pUnit, STAT_TOHIT, 0);
            if (nToHit)
            {
                missileParams.nAttBonus = nToHit;
                missileParams.dwFlags |= 0x1000;
            }
        }

        if (!bCheckItemQuantity || pUnit->dwUnitType || sub_6FD118C0(pGame, pUnit) > 0)
        {
            return MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }
    return 0;
}

//D2Game.0x6FD115E0
D2UnitStrc* __fastcall D2GAME_CreateMissile_6FD115E0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nMissileId, int32_t nX, int32_t nY)
{
    if (!pUnit)
    {
        return 0;
    }

    int32_t x = nX;
    int32_t y = nY;
    if (!x && !y)
    {
        D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTargetUnit)
        {
            x = CLIENTS_GetUnitX(pTargetUnit);
            y = CLIENTS_GetUnitY(pTargetUnit);
        }
        else
        {
            x = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
            y = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
        }

        if (!x && !y)
        {
            return 0;
        }
    }

    if (UNITS_GetDistanceToCoordinates(pUnit, x, y) > 100)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.nSkill = nSkillId;
    missileParams.nMissile = nMissileId;
    missileParams.dwFlags = 1;
    missileParams.pOwner = pUnit;
    missileParams.nX = x;
    missileParams.nY = y;
    missileParams.nSkillLevel = nSkillLevel;

    return MISSILES_CreateMissileFromParams(pGame, &missileParams);
}

//D2Game.0x6FD11710
D2UnitStrc* __fastcall sub_6FD11710(D2GameStrc* pGame, int32_t nMissileId, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t bSkipCheck)
{
    D2MissileStrc missileParams = {};
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = nSkillId;
    missileParams.dwFlags = 0x420u;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nX = nX + CLIENTS_GetUnitX(pUnit);
    missileParams.nY = nY + CLIENTS_GetUnitY(pUnit);

    if (nTargetX && nTargetY)
    {
        missileParams.nTargetX = nTargetX;
        missileParams.nTargetY = nTargetY;
    }
    else
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget)
        {
            missileParams.nTargetX = CLIENTS_GetUnitX(pTarget);
            missileParams.nTargetY = CLIENTS_GetUnitY(pTarget);
        }
        else
        {
            missileParams.nTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
            missileParams.nTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
        }

        if (!missileParams.nTargetX || !missileParams.nTargetY)
        {
            return 0;
        }
    }

    if (!bSkipCheck || pUnit->dwUnitType != UNIT_PLAYER || sub_6FD118C0(pGame, pUnit) > 0)
    {
        return MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 0;
}

//D2Game.0x6FD118C0
int32_t __fastcall sub_6FD118C0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    D2UnitStrc* pWeapon = sub_6FC7C7B0(pUnit);
    int32_t bNoBow = 1;

    if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_BOW) || ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_CROSSBOW))
    {
        bNoBow = 0;
    }

    D2UnitStrc* pRightArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
    if (pRightArmItem && (!bNoBow || pRightArmItem == pWeapon) && (ITEMS_CheckIfStackable(pRightArmItem) || ITEMS_CheckIfThrowable(pRightArmItem) || STATLIST_UnitGetItemStatOrSkillStatValue(pRightArmItem, STAT_ITEM_THROWABLE, 0)))
    {
        return sub_6FD11340(pGame, pUnit, pRightArmItem);
    }

    D2UnitStrc* pLeftArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
    if (pLeftArmItem && (!bNoBow || pLeftArmItem == pWeapon) && (ITEMS_CheckIfStackable(pLeftArmItem) || ITEMS_CheckIfThrowable(pLeftArmItem) || STATLIST_UnitGetItemStatOrSkillStatValue(pLeftArmItem, STAT_ITEM_THROWABLE, 0)))
    {
        return sub_6FD11340(pGame, pUnit, pLeftArmItem);
    }

    return 0;
}

//D2Game.0x6FD119C0
int32_t __fastcall sub_6FD119C0(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }

    if (pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 1;
    }

    D2UnitStrc* pWeapon = sub_6FC7C7B0(pUnit);
    if (!pWeapon)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t bNoAmmoItem = 1;
    if (ITEMS_GetAmmoType(pWeapon))
    {
        bNoAmmoItem = 0;
        if (!SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) && STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_MAGICARROW, 0))
        {
            return 1;
        }
    }

    if (!pUnit->pInventory)
    {
        return 0;
    }

    D2UnitStrc* pRightArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
    if (pRightArmItem && (!bNoAmmoItem || pRightArmItem == pWeapon) && (ITEMS_CheckIfStackable(pRightArmItem) || ITEMS_CheckIfThrowable(pRightArmItem) || STATLIST_UnitGetItemStatOrSkillStatValue(pRightArmItem, STAT_ITEM_THROWABLE, 0)))
    {
        return ITEMS_CanItemBeUsedForThrowSkill(pRightArmItem);
    }

    D2UnitStrc* pLeftArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
    if (pLeftArmItem && (!bNoAmmoItem || pLeftArmItem == pWeapon) && (ITEMS_CheckIfStackable(pLeftArmItem) || ITEMS_CheckIfThrowable(pLeftArmItem) || STATLIST_UnitGetItemStatOrSkillStatValue(pLeftArmItem, STAT_ITEM_THROWABLE, 0)))
    {
        return ITEMS_CanItemBeUsedForThrowSkill(pLeftArmItem);
    }

    return 0;
}

//D2Game.0x6FD11AE0
void __fastcall sub_6FD11AE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    if (pUnit)
    {
        pUnit->dwFlagEx &= 0xFFFFFFBF;
    }

    if (pItem && D2GAME_Transmogrify_6FC4A660(pGame, pUnit, pItem))
    {
        SUNIT_AttachSound(pUnit, 4u, pUnit);
    }

    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0, -1, 1, 0, -1);
}

//D2Game.0x6FD11B50
int32_t __fastcall D2GAME_CheckSkillIfUnitsClassSkill_6FD11B50(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord)
        {
            return pSkillsTxtRecord->nCharClass == pUnit->dwClassId;
        }
    }

    return 0;
}

//D2Game.0x6FD11BA0
void __fastcall sub_6FD11BA0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        const int32_t nSkillDelay = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwDelay, nSkillId, nSkillLevel);
        if (nSkillDelay > 0)
        {
            D2GAME_SKILLS_SetDelay_6FD11C00(pGame, pUnit, nSkillDelay);
        }
    }
}

//D2Game.0x6FD11C00
void __fastcall D2GAME_SKILLS_SetDelay_6FD11C00(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nDelay)
{
    const int32_t nExpireFrame = nDelay + pGame->dwGameFrame;
    if (pUnit && !pUnit->dwUnitType && nDelay)
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_SKILLDELAY);
        if (!pStatList)
        {
            pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, pUnit->dwUnitType, pUnit->dwUnitId);
            if (!pStatList)
            {
                return;
            }

            STATLIST_SetState(pStatList, STATE_SKILLDELAY);
            STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
            STATES_ToggleState(pUnit, STATE_SKILLDELAY, 1);
        }

        D2COMMON_10476(pStatList, nExpireFrame);
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
    }
}

//D2Game.0x6FD11C90
int32_t __fastcall sub_6FD11C90(D2UnitStrc* pUnit, int32_t nStateId, int32_t a3)
{
    D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(nStateId);
    if (pStatesTxtRecord && pStatesTxtRecord->wGroup)
    {
        int32_t nResult = 0;
        for (int32_t i = 0; i < sgptDataTables->nStatesTxtRecordCount; ++i)
        {
            if (i != nStateId || a3)
            {
                D2StatesTxt* pRecord = SKILLS_GetStatesTxtRecord(i);
                if (pRecord->wGroup == pStatesTxtRecord->wGroup && STATES_CheckState(pUnit, i))
                {
                    nResult = 1;
                    STATES_ToggleState(pUnit, i, 0);

                    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, i);
                    if (pStatList)
                    {
                        D2Common_10474(pUnit, pStatList);
                        STATLIST_FreeStatList(pStatList);
                    }
                }
            }
        }

        return nResult;
    }

    return 0;
}

//D2Game.0x6FD11D90
void __fastcall sub_6FD11D90(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nLength, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

    if (pSkillsTxtRecord)
    {
        switch (pSkillsTxtRecord->nEType)
        {
        case ELEMTYPE_COLD:
            pDamage->dwColdLen = nLength;
            break;
        case ELEMTYPE_POIS:
            pDamage->dwPoisLen = nLength;
            break;
        case ELEMTYPE_STUN:
            pDamage->dwStunLen = nLength;
            break;
        case ELEMTYPE_BURN:
            pDamage->dwBurnLen = nLength;
            break;
        case ELEMTYPE_FREEZE:
            pDamage->dwFrzLen = nLength;
            break;
        default:
            return;
        }
    }
}

//D2Game.0x6FD11E40
void __fastcall sub_6FD11E40(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nElementalType, int32_t nDamage, int32_t nLength, int32_t* pResistStatId, int32_t* pElementalType)
{
    if (nElementalType == ELEMTYPE_RAND)
    {
        constexpr int32_t nElemTypes[] =
        {
            ELEMTYPE_FIRE,
            ELEMTYPE_LTNG,
            ELEMTYPE_COLD,
            ELEMTYPE_POIS,
        };

        nElementalType = nElemTypes[ITEMS_RollRandomNumber(&pUnit->pSeed) & 3];
        if (nLength <= 0)
        {
            nLength = 50;
        }

        if (pElementalType)
        {
            *pElementalType = nElementalType;
        }
    }

    int32_t nResStatId = -1;

    switch (nElementalType)
    {
    case ELEMTYPE_FIRE:
    {
        pDamage->dwHitClass = 0x20u;
        pDamage->dwFireDamage += nDamage;
        nResStatId = STAT_FIRERESIST;
        break;
    }
    case ELEMTYPE_LTNG:
    {
        pDamage->dwHitClass = 0x40u;
        pDamage->dwLtngDamage += nDamage;
        nResStatId = STAT_LIGHTRESIST;
        break;
    }
    case ELEMTYPE_MAGIC:
    {
        nResStatId = STAT_MAGICRESIST;
        pDamage->dwMagDamage += nDamage;
        break;
    }
    case ELEMTYPE_COLD:
    {
        pDamage->dwColdLen = nLength;
        pDamage->dwColdDamage += nDamage;
        pDamage->dwHitClass = 0x30u;
        nResStatId = STAT_COLDRESIST;
        break;
    }
    case ELEMTYPE_POIS:
    {
        pDamage->dwPoisLen = nLength;
        pDamage->dwPoisDamage += nDamage;
        pDamage->dwHitClass = 0x50u;
        nResStatId = STAT_POISONRESIST;
        break;
    }
    case ELEMTYPE_LIFE:
    {
        pDamage->dwLifeLeech += nDamage;
        break;
    }
    case ELEMTYPE_MANA:
    {
        pDamage->dwManaLeech += nDamage;
        break;
    }
    case ELEMTYPE_STAMINA:
    {
        pDamage->dwStamLeech += nDamage;
        break;
    }
    case ELEMTYPE_STUN:
    {
        pDamage->dwHitClass = 0x60u;
        pDamage->dwStunLen += nLength + nDamage;
        break;
    }
    case ELEMTYPE_BURN:
    {
        pDamage->dwBurnLen = nLength;
        pDamage->dwBurnDamage += nDamage;
        break;
    }
    case ELEMTYPE_FREEZE:
    {
        pDamage->dwFrzLen = nLength;
        pDamage->dwColdDamage += nDamage;
        pDamage->dwHitClass = 0x30u;
        nResStatId = STAT_COLDRESIST;
        break;
    }
    default:
    {
        pDamage->dwPhysDamage += nDamage;
        break;
    }
    }

    if (pResistStatId)
    {
        *pResistStatId = nResStatId;
    }
}

//D2Game.0x6FD11FF0
int32_t __fastcall SKILLS_SrvSt01_Attack_LeftHandSwing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    pUnit->nSeqCurrentFramePrecise = UNITS_GetFrameBonus(pUnit) << 8;
    if (STATES_CheckStateMaskOnUnit(pUnit, 38))
    {
        return sub_6FD00370(pGame, pUnit, nSkillLevel);
    }

    if (!COMPOSIT_IsWeaponBowOrXBow(pUnit) || sub_6FD119C0(pUnit))
    {
        return 1;
    }

    sub_6FC80A30(pGame, pUnit);
    sub_6FC80E10(pGame, pUnit);
    return 0;
}

//D2Game.0x6FD12060
int32_t __fastcall SKILLS_SrvSt65_Throw_LeftHandThrow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (sub_6FD119C0(pUnit))
    {
        return 1;
    }

    sub_6FC80A30(pGame, pUnit);
    sub_6FC80E10(pGame, pUnit);
    return 0;
}

//D2Game.0x6FD120A0
int32_t __fastcall SKILLS_SrvSt02_Kick(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

        D2DamageStrc damage = {};
        damage.wResultFlags = 9;
        damage.dwHitFlags = 2;
        damage.dwPhysDamage = pSkillsTxtRecord->dwMinDam << pSkillsTxtRecord->nToHitShift;
        damage.dwHitClass = 1;
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
        return 1;
    }
    return 0;
}

//D2Game.0x6FD12150
int32_t __fastcall SKILLS_SrvSt03_Unsummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget && pTarget->dwUnitType == UNIT_MONSTER && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (AIGENERAL_GetMinionOwner(pTarget) == pUnit)
        {
            const int32_t nPetType = PLAYERPETS_GetPetTypeFromPetGUID(pUnit, pTarget->dwUnitId);
            if (nPetType >= 0 && nPetType < sgptDataTables->nPetTypeTxtRecordCount)
            {
                D2PetTypeTxt* pPetTypeTxtRecord = &sgptDataTables->pPetTypeTxt[nPetType];
                if (pPetTypeTxtRecord && pPetTypeTxtRecord->dwPetFlags & gdwBitMasks[3])
                {
                    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
                    if (pSkill)
                    {
                        SKILLS_SetParam1(pSkill, pTarget->dwUnitId);
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FD12210
int32_t __fastcall SKILLS_SrvDo001_Attack_LeftHandSwing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pWeapon = sub_6FC7C7B0(pUnit);
    if (pWeapon && ITEMS_GetItemType(pWeapon) == ITEMTYPE_MISSILE_POTION)
    {
        return 0;
    }

    if (COMPOSIT_IsWeaponBowOrXBow(pUnit))
    {
        const int32_t nItemType = D2COMMON_11039_CheckWeaponIsMissileBased(pUnit, &nSkillLevel);
        if (nItemType == ITEMTYPE_BOW)
        {
            sub_6FD11420(pGame, ITEMTYPE_BOW, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        }
        else
        {
            sub_6FD11420(pGame, nItemType, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 1);
        }
        return 1;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2DamageStrc damage = {};
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, STATLIST_UnitGetStatValue(pUnit, STAT_PROGRESSIVE_TOHIT, 0), 0);
    damage.dwHitFlags |= 2;
    sub_6FCF5680(pUnit, &damage);
    SUNITDMG_FillDamageValues(pGame, pUnit, pTarget, &damage, 0, 128);
    sub_6FCF5870(pUnit, &damage);
    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);

    if (!STATES_CheckStateMaskOnUnit(pUnit, 38))
    {
        D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
        if (pDamage)
        {
            sub_6FCF77E0(pGame, pUnit, pDamage);
        }

        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        return 1;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    return 1;
}

//D2Game.0x6FD123D0
int32_t __fastcall SKILLS_SrvDo002_Kick_PowerStrike_MonIceSpear_Impale_Bash_Stun_Concentrate_BearSmite_Vengeance_Berserk_FireClaws(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wSrvOverlay > 0 && pSkillsTxtRecord->wSrvOverlay <= sgptDataTables->nOverlayTxtRecordCount)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (!pTarget)
        {
            return 0;
        }

        D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
        if (!pDamage)
        {
            return 1;
        }

        if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            UNITS_SetOverlay(pTarget, pSkillsTxtRecord->wSrvOverlay, 0);
        }
    }

    if (pSkillsTxtRecord->wAuraTargetState >= 0 && pSkillsTxtRecord->wAuraTargetState < sgptDataTables->nStatesTxtRecordCount)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (!pTarget)
        {
            return 0;
        }

        D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
        if (!pDamage)
        {
            return 1;
        }

        if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            int32_t nLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
            if (nLength < 1)
            {
                nLength = 1;
            }

            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pTarget, pSkillsTxtRecord->wAuraTargetState);
            if (!pStatList)
            {
                pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, pGame->dwGameFrame + nLength, pTarget->dwUnitType, pTarget->dwUnitId);
                if (!pStatList)
                {
                    return 1;
                }
            }

            EVENT_SetEvent(pGame, pTarget, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + nLength, 0, 0);
            STATES_ToggleState(pTarget, pSkillsTxtRecord->wAuraTargetState, 1);
            STATLIST_SetState(pStatList, pSkillsTxtRecord->wAuraTargetState);
            D2COMMON_10475_PostStatToStatList(pTarget, pStatList, 1);
            STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
            sub_6FCFE0E0(pTarget, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
        }
    }

    if (pSkillsTxtRecord->nAuraState > 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
        if (!pStatList)
        {
            int32_t nUnitGUID = -1;
            int32_t nUnitType = 6;
            if (pUnit)
            {
                nUnitGUID = pUnit->dwUnitId;
                nUnitType = pUnit->dwUnitType;
            }

            pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 4u, 0, nUnitType, nUnitGUID);
            if (!pStatList)
            {
                return 1;
            }

            STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
            STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
        }

        STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 1);
        sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD12670
int32_t __fastcall SKILLS_SrvDo004_Unsummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill)
    {
        sub_6FC7D260(pGame, pUnit, SKILLS_GetParam1(pSkill), 1);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD126B0
int32_t __fastcall SKILLS_SrvDo003_Throw(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit->pInventory)
    {
        return 0;
    }

    D2UnitStrc* pWeapon = nullptr;
    int32_t nBodyLoc = 0;
    int32_t bIsLeftHandItem = 0;
    if (!INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pWeapon, &nBodyLoc, &bIsLeftHandItem))
    {
        return 0;
    }

    if (!bIsLeftHandItem)
    {
        if (nBodyLoc == BODYLOC_LARM)
        {
            pWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
        }
        else
        {
            pWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
        }
    }

    if (ITEMS_CheckIfThrowable(pWeapon) || STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_THROWABLE, 0))
    {
        if (pWeapon)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
            const int32_t nMissileType = ITEMS_GetMissileType(pWeapon);

            D2UnitStrc* pMissile = nullptr;
            if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_MISSILE_POTION))
            {
                pMissile = sub_6FD11710(pGame, nMissileType, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 1);
            }
            else
            {
                pMissile = sub_6FD11420(pGame, nMissileType, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 1);
            }

            if (pMissile && pWeapon)
            {
                STATLIST_SetUnitStat(pMissile, STAT_TOHIT, STATLIST_UnitGetStatValue(pMissile, STAT_TOHIT, 0) + D2Common_11024(pUnit, pWeapon, 0, 0, 0), 0);
                STATLIST_AddUnitStat(pMissile, STAT_DAMAGEPERCENT, D2Common_11024(pUnit, pWeapon, 0, 1, 0), 0);
            }

            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD12800
int32_t __fastcall SKILLS_SrvDo005_LeftHandThrow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit->pInventory)
    {
        return 0;
    }

    D2UnitStrc* pWeapon = nullptr;
    int32_t nBodyLoc = 0;
    int32_t bIsLeftHandItem = 0;
    if (!INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pWeapon, &nBodyLoc, &bIsLeftHandItem))
    {
        return 0;
    }

    if (bIsLeftHandItem)
    {
        if (nBodyLoc == BODYLOC_LARM)
        {
            pWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
        }
        else
        {
            pWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
        }
    }

    if (ITEMS_CheckIfThrowable(pWeapon) || STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_THROWABLE, 0))
    {
        if (pWeapon)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
            const int32_t nMissileType = ITEMS_GetMissileType(pWeapon);

            D2UnitStrc* pMissile = nullptr;
            if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_MISSILE_POTION))
            {
                pMissile = sub_6FD11710(pGame, nMissileType, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 1);
            }
            else
            {
                pMissile = sub_6FD11420(pGame, nMissileType, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 1);
            }

            if (pMissile && pWeapon)
            {
                STATLIST_SetUnitStat(pMissile, STAT_TOHIT, STATLIST_UnitGetStatValue(pMissile, STAT_TOHIT, 0) + D2Common_11024(pUnit, pWeapon, 0, 0, 0), 0);
                STATLIST_AddUnitStat(pMissile, STAT_DAMAGEPERCENT, D2Common_11024(pUnit, pWeapon, 0, 1, 0), 0);
            }

            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD12950
int32_t __fastcall sub_6FD12950(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t a5)
{
    if (!pGame || !pUnit || !UNITS_GetRoom(pUnit))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !a5 && !sub_6FD10CE0(pUnit))
    {
        return 0;
    }

    if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]))
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
        if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
        {
            UNITS_SetUsedSkill(pUnit, nullptr);

            if (pUnit->dwUnitType == UNIT_PLAYER)
            {
                sub_6FC817D0(pGame, pUnit, 0, PLRMODE_TNEUTRAL, 0, 0, 1);
            }
            return 0;
        }
    }

    if (pSkillsTxtRecord->nLineOfSight != SKILLSLOS_NONE)
    {
        int32_t nX = 0;
        int32_t nY = 0;

        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget)
        {
            nX = CLIENTS_GetUnitX(pTarget);
            nY = CLIENTS_GetUnitY(pTarget);
        }
        else
        {
            nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
            nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
        }

        if (nX && nY)
        {
            uint32_t nCollisionMask = 0;
            switch (pSkillsTxtRecord->nLineOfSight)
            {
            case SKILLSLOS_MISSILE_BARRIER:
                nCollisionMask = COLLIDE_BARRIER;
                break;
            case SKILLSLOS_PLAYER_PATH:
                nCollisionMask = COLLIDE_MASK_WALKING_UNIT;
                break;
            case SKILLSLOS_PLAYER_MONSTER:
                nCollisionMask = COLLIDE_PLAYER | COLLIDE_MONSTER;
                break;
            case SKILLSLOS_PLAYER_FLYING:
                nCollisionMask = COLLIDE_DOOR | COLLIDE_BARRIER;
                break;
            case SKILLSLOS_RADIAL_BARRIER:
                nCollisionMask = COLLIDE_DOOR | COLLIDE_BARRIER | COLLIDE_BLOCK_PLAYER;
                break;
            default:
                return 0;
            }

            if (!D2Common_11026(nX, nY, pUnit, nCollisionMask))
            {
                return 0;
            }
        }
    }

    if (pSkillsTxtRecord->wSrvStartFunc < 0 || pSkillsTxtRecord->wSrvStartFunc >= std::size(gpSkillSrvStartFnTable_6FD408B0))
    {
        return 0;
    }

    SkillStartFunc pfSkillStart = gpSkillSrvStartFnTable_6FD408B0[pSkillsTxtRecord->wSrvStartFunc];
    if (!pfSkillStart)
    {
        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PERIODIC])
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, 0);
        }
        return 1;
    }

    const int32_t nResult = pfSkillStart(pGame, pUnit, nSkillId, nSkillLevel);
    if (nResult)
    {
        if (!(pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_USEMANAONDO]) && !a5)
        {
            D2GAME_SKILLMANA_Consume_6FD10A50(pGame, pUnit, nSkillId, nSkillLevel);
        }

        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PERIODIC])
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, 0);
        }
    }

    return nResult;
}

//D2Game.0x6FD12BA0
int32_t __fastcall D2GAME_SKILLS_Handler_6FD12BA0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t a5, int32_t a6, int32_t a7)
{
    if (!pGame || !pUnit)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t bLivingUnit = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER)
    {
        bLivingUnit = 1;
        D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pUnit);
        if (pUsedSkill && !(SKILLS_GetFlags(pUsedSkill) & 1) && !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]))
        {
            D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
            if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
            {
                UNITS_SetUsedSkill(pUnit, nullptr);

                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    sub_6FC817D0(pGame, pUnit, 0, PLRMODE_TNEUTRAL, 0, 0, 1);
                }

                return 0;
            }
        }

        if (!a6 && (!pUsedSkill || SKILLS_GetSkillIdFromSkill(pUsedSkill, __FILE__, __LINE__) != nSkillId || SKILLS_GetSkillLevel(pUnit, pUsedSkill, 1) <= 0))
        {
            D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
            if (!pSkill || SKILLS_GetSkillLevel(pUnit, pSkill, 1) <= 0)
            {
                return 0;
            }
        }
    }

    int32_t nDoFunc = pSkillsTxtRecord->wSrvDoFunc;
    if (nDoFunc < 0 || nDoFunc >= std::size(gpSkillSrvDoFnTable_6FD40A20) || bLivingUnit && pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_USEMANAONDO] && !sub_6FD10CE0(pUnit))
    {
        return 0;
    }

    if (a7 && a6 && pSkillsTxtRecord->wItemEffect > 1)
    {
        nDoFunc = pSkillsTxtRecord->wItemEffect;
    }
    
    SkillDoFunc pDoFunc = gpSkillSrvDoFnTable_6FD40A20[nDoFunc];
    int32_t nResult = 0;
    if (pDoFunc)
    {
        nResult = pDoFunc(pGame, pUnit, nSkillId, nSkillLevel);
    }

    const int32_t nSrvMissile = pSkillsTxtRecord->wSrvMissile;
    if (nSrvMissile >= 0 && nSrvMissile < sgptDataTables->nMissilesTxtRecordCount && &sgptDataTables->pMissilesTxt[nSrvMissile])
    {

        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;

        int32_t nX = 0;
        int32_t nY = 0;
        int32_t nTargetX = 0;
        int32_t nTargetY = 0;
        if (a6 && a7)
        {
            D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
            if (pTarget)
            {
                nX = CLIENTS_GetUnitX(pTarget);
                nY = CLIENTS_GetUnitY(pTarget);
            }
            else
            {
                nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
                nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
            }

            if (nX && nY)
            {
                nTargetX = nX;
                nTargetY = nY;
                nX -= CLIENTS_GetUnitX(pUnit);
                nY -= CLIENTS_GetUnitY(pUnit);
                nTargetX += nX;
                nTargetY += nY;
            }
        }

        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_LOB])
        {
            sub_6FD11710(pGame, pSkillsTxtRecord->wSrvMissile, pUnit, nSkillId, nSkillLevel, nX, nY, nTargetX, nTargetY, 0);
        }
        else
        {
            sub_6FD11420(pGame, pSkillsTxtRecord->wSrvMissile, pUnit, nSkillId, nSkillLevel, nX, nY, nTargetX, nTargetY, 0);
        }

        nResult = 1;
    }

    if (!nResult)
    {
        return 0;
    }

    if (a5)
    {
        if (bLivingUnit)
        {
            if (!gpSkillSrvStartFnTable_6FD408B0[pSkillsTxtRecord->wSrvStartFunc] || pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_USEMANAONDO])
            {
                D2GAME_SKILLMANA_Consume_6FD10A50(pGame, pUnit, nSkillId, nSkillLevel);
            }

            if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_DECQUANT])
            {
                sub_6FD118C0(pGame, pUnit);
            }
        }

        const int32_t nDelay = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwDelay, nSkillId, nSkillLevel);
        if (nDelay > 0 && pUnit->dwUnitType == UNIT_PLAYER && (pSkillsTxtRecord->nAnim != PLRMODE_SEQUENCE || !(pUnit->dwSeqFrame & 0xFFFFFF00)))
        {
            D2GAME_SKILLS_SetDelay_6FD11C00(pGame, pUnit, nDelay);
        }
    }

    return nResult;
}

//D2Game.0x6FD12FD0
int32_t __fastcall sub_6FD12FD0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        if (!pSkillsTxtRecord)
        {
            if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
            {
                sub_6FC817D0(pGame, pUnit, 0, PLRMODE_TNEUTRAL, 0, 0, 1);
            }
            return 0;
        }

        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_TARGETABLEONLY])
        {
            if (!sub_6FCBD900(pGame, pUnit, pTarget) && !SUNIT_CanPetBeTargetedBySkill(pGame, pUnit, pTarget, nSkillId) && !SUNIT_CanAllyBeTargetedBySkill(pGame, pUnit, pTarget, nSkillId))
            {
                if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
                {
                    sub_6FC817D0(pGame, pUnit, 0, PLRMODE_TNEUTRAL, 0, 0, 1);
                }
                return 0;
            }
        }

        if (!(pTarget->dwFlags & UNITFLAG_TARGETABLE))
        {
            UNITS_SetTargetUnitForDynamicUnit(pUnit, nullptr);
        }
        else
        {
            if (pTarget->dwUnitType == UNIT_MONSTER && pTarget->dwAnimMode == MONMODE_DEAD)
            {
                if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_TARGETCORPSE]))
                {
                    UNITS_SetTargetUnitForDynamicUnit(pUnit, nullptr);
                }
            }
            else
            {
                if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_TARGETCORPSE])
                {
                    UNITS_SetTargetUnitForDynamicUnit(pUnit, nullptr);
                }
            }
        }
    }

    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    int32_t nCharges = 0;
    if (SKILLS_GetSkillInfo(pSkill, nullptr, nullptr, &nSkillLevel, &nCharges) && !nCharges)
    {
        return 0;
    }

    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTargetUnit && !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_TARGETALLY]) && SUNIT_CanAllyBeTargetedBySkill(pGame, pUnit, pTargetUnit, nSkillId))
    {
        return 0;
    }

    if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]))
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
        if (DUNGEON_IsRoomInTown(pRoom))
        {
            UNITS_SetUsedSkill(pUnit, 0);

            if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
            {
                sub_6FC817D0(pGame, pUnit, 0, PLRMODE_TNEUTRAL, 0, 0, 1);
            }
            return 0;
        }
    }

    const int32_t nStartFuncResult = sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 0);
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && !nStartFuncResult)
    {
        sub_6FC817D0(pGame, pUnit, 0, 5, 0, 0, 1);
    }

    return nStartFuncResult;
}

//D2Game.0x6FD13220
int32_t __fastcall SKILL_ComputePeriodicRate(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nDelay = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPerDelay, nSkillId, nSkillLevel);
    if (nDelay <= 5)
    {
        nDelay = 5;
    }

    return nDelay * (pGame->dwGameFrame + nDelay - 1) / nDelay + 1;
}

//D2Game.0x6FD13260
int32_t __fastcall sub_6FD13260(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t a5)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[6] || pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[5]))
    {
		const int32_t nExpireFrame = SKILL_ComputePeriodicRate(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel);
        if (a5)
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, -1);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, nExpireFrame, -1, 0);
        }
        else
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, nSkillId);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, nExpireFrame, nSkillId, nSkillLevel);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FD13330
int32_t __fastcall sub_6FD13330(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a5, int32_t nSkillId, int32_t nSkillLevel, int32_t a6)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[6] || pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[5]))
    {
        const int32_t nExpireFrame = SKILL_ComputePeriodicRate(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel);
        if (a6)
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, -1);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, nExpireFrame, -1, 0);
        }
        else
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSTATS, a5);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSTATS, nExpireFrame, a5, nSkillId);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FD13410
void __fastcall sub_6FD13410(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill)
    {
        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
        D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 1, 0, 0);
        sub_6FD13260(pGame, pUnit, nSkillId, nSkillLevel, 0);
    }
}

//D2Game.0x6FD13470
void __fastcall D2GAME_MONSTERS_AiFunction09_6FD13470(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillIdArg, int32_t a4)
{
    if (nSkillIdArg == -1)
    {
        D2SkillStrc* pSkill = UNITS_GetRightSkill(pUnit);
        if (!pSkill)
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 8, -1);
            return;
        }

        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord && pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[5] && !SUNIT_IsDead(pUnit))
        {
            D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 1, 0, 0);
            sub_6FD13260(pGame, pUnit, nSkillId, nSkillLevel, 1);
        }
        else
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 8, -1);
        }
    }
    else if (nSkillIdArg > 0)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillIdArg);
        if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 8, nSkillIdArg);
            return;
        }
        
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
        if (!pStatList)
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 8, nSkillIdArg);
            return;
        }

        const int32_t nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
        if (SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillIdArg) && nSkillIdArg == STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0))
        {
            D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillIdArg, nSkillLevel, 1, 0, 0);
            sub_6FD13260(pGame, pUnit, nSkillIdArg, nSkillLevel, 0);
        }
        else
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 8, nSkillIdArg);
        }
    }
}

//D2Game.0x6FD13610
void __fastcall D2GAME_MONSTERS_AiFunction10_6FD13610(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || SUNIT_IsDead(pUnit))
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSTATS, a3);
    }
    else
    {
        const int32_t nAura = STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_AURA, nSkillId);
        if (nAura > 0)
        {
            D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nAura, 1, 1, 0);
            sub_6FD13330(pGame, pUnit, a3, nSkillId, nAura, 0);
        }
        else
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_PERIODICSTATS, a3);
        }
    }
}

//D2Game.0x6FD136E0
int32_t __fastcall sub_6FD136E0(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        int32_t nRequiredSkillPoints = 1;
        if (pSkillsTxtRecord->dwSkPoints != -1)
        {
            int32_t nSkillLevel = 0;

            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1);
            if (pSkill)
            {
                nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
            }

            nRequiredSkillPoints = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwSkPoints, nSkillId, nSkillLevel);
        }

        if (STATLIST_GetUnitBaseStat(pUnit, STAT_SKILLPTS, 0) >= nRequiredSkillPoints)
        {
            STATLIST_AddUnitStat(pUnit, STAT_SKILLPTS, -nRequiredSkillPoints, 0);

            D2SkillStrc* pSkill = SKILLS_AddSkill(pUnit, nSkillId);
            SKILLS_RefreshPassiveSkills(pUnit);

            if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
            {
                sub_6FC7DF40(SUNIT_GetGameFromUnit(pUnit), pUnit);
            }

            if (pSkill)
            {
                D2GAME_PACKETS_SendPacket0x21_UpdateSkills_6FC3DB50(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, nSkillId, SKILLS_GetSkillLevel(pUnit, pSkill, 0), 0);

                const int16_t nPassiveState = SKILLS_GetPassiveState(nSkillId);
                if (nPassiveState)
                {
                    STATES_ToggleState(pUnit, nPassiveState, 1);
                }

                SKILLS_RefreshSkill(pUnit, nSkillId);
                return 1;
            }
            else
            {
                STATLIST_AddUnitStat(pUnit, STAT_SKILLPTS, nRequiredSkillPoints, 0);
            }
        }
    }

    return 0;
}

//D2Game.0x6FD13800
void __fastcall D2GAME_AssignSkill_6FD13800(D2UnitStrc* pUnit, int32_t a2, int32_t nSkillId, int32_t nFlags)
{
    // TODO: a2
    if (nSkillId == SKILL_LEFTHANDSWING)
    {
        return;
    }

    if (!a2 && STATES_CheckState(pUnit, STATE_NOMANAREGEN))
    {
        STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 0);
    }

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, nFlags);
    if (!pSkill)
    {
        return;
    }

    D2SkillStrc* pRightSkill = nullptr;
    if (a2)
    {
        SKILLS_SetLeftActiveSkill(pUnit, nSkillId, nFlags);
    }
    else
    {
        pRightSkill = UNITS_GetRightSkill(pUnit);
        SKILLS_SetRightActiveSkill(pUnit, nSkillId, nFlags);
    }

    D2UnitGUID nOwnerGUID = D2UnitInvalidGUID;
    SKILLS_GetSkillInfo(pSkill, &nOwnerGUID, nullptr, nullptr, nullptr);
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2GAME_PACKETS_SendPacket0x23_6FC3DC60(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0, pUnit->dwUnitId, a2, nSkillId, nOwnerGUID);
    }

    if (!a2)
    {
        if (pRightSkill)
        {
            D2SkillsTxt* pRightSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__));
            if (pRightSkillsTxtRecord)
            {
                if (pRightSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA])
                {
                    if (pRightSkillsTxtRecord->nAuraState >= 0 && pRightSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount) // TODO: Was || before which doesn't make any sense
                    {
                        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pRightSkillsTxtRecord->nAuraState);
                        if (pStatList)
                        {
                            D2Common_10474(pUnit, pStatList);
                            STATLIST_FreeStatList(pStatList);
                        }

                        STATES_ToggleState(pUnit, pRightSkillsTxtRecord->nAuraState, 0);
                    }

                    D2GAME_EVENTS_Delete_6FC34840(SUNIT_GetGameFromUnit(pUnit), pUnit, UNITEVENTCALLBACK_PERIODICSKILLS, -1);
                }
            }
        }

        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord && pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA])
        {
            D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
            if (pSkillsTxtRecord->nAuraState >= 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
            {
                D2SkillStrc* pHighestLevelSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
                if (pHighestLevelSkill)
                {
                    const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pHighestLevelSkill, 1);
                    if (nSkillLevel > 0)
                    {
                        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_IMMEDIATE])
                        {
                            D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 1, 0, 0);
                        }
                        else
                        {
                            STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 1);

                            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
                            if (!pStatList)
                            {
                                if (pUnit)
                                {
                                    pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0, 0, pUnit->dwUnitType, pUnit->dwUnitId);
                                }
                                else
                                {
                                    pStatList = STATLIST_AllocStatList(nullptr, 0, 0, 6, -1);
                                }
                            }

                            if (pStatList)
                            {
                                STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
                                STATLIST_SetSkillId(pStatList, nSkillId);
                                STATLIST_SetSkillLevel(pStatList, nSkillLevel);
                                D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                                STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
                                STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
                            }
                        }

                        sub_6FD13260(pGame, pUnit, nSkillId, nSkillLevel, 1);
                    }
                }
            }
        }
    }
}

//D2Game.0x6FD13B20
D2UnitStrc* __fastcall D2GAME_CreateLinkPortal_6FD13B20(D2GameStrc* pGame, D2UnitStrc* pPortal, int32_t nDestLevel, int32_t nSourceLevel)
{
    const uint8_t nAct = DRLG_GetActNoFromLevelId(nDestLevel);
    int32_t nX = 0;
    int32_t nY = 0;
    D2ActiveRoomStrc* pRoom = DUNGEON_FindActSpawnLocationEx(pGame->pAct[nAct], nDestLevel, 11, &nX, &nY, 3);
    D2ActiveRoomStrc* pDestinationRoom = pRoom;

    sub_6FC385A0(pGame, pRoom);

    if (!pRoom)
    {
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pPortal), UNIT_OBJECT, pPortal ? pPortal->dwUnitId : -1);
        SUNIT_RemoveUnit(pGame, pPortal);
        return nullptr;
    }

    D2CoordStrc coords = {};
    coords.nX = nX;
    coords.nY = nY;

    if (QUESTS_PortalCheck(pGame, &coords, nSourceLevel, &pRoom))
    {
        pRoom = D2GAME_GetRoom_6FC52070(pRoom, coords.nX, coords.nY);
    }

    if (!pRoom)
    {
        pRoom = pDestinationRoom;
        coords.nX = nX;
        coords.nY = nY;
    }

    pRoom = COLLISION_GetFreeCoordinatesEx(pRoom, &coords, 3, 0xBE11u, 5);
    if (!pRoom)
    {
        pRoom = DUNGEON_FindActSpawnLocationEx(pGame->pAct[nAct], nDestLevel, 11, &nX, &nY, 3);
        if (!pRoom)
        {
            pRoom = pDestinationRoom;
            D2_ASSERT(pDestinationRoom);
        }
    }

    nX = coords.nX;
    nY = coords.nY;

    D2UnitStrc* pLinkPortal = SUNIT_AllocUnitData(UNIT_OBJECT, pPortal ? pPortal->dwClassId : -1, coords.nX, coords.nY, pGame, pRoom, 1, 2, 0);
    if (!pLinkPortal)
    {
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pPortal), UNIT_OBJECT, pPortal ? pPortal->dwUnitId : -1);
        SUNIT_RemoveUnit(pGame, pPortal);
        return nullptr;
    }

    const int32_t nLocalLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPortal));
    D2_ASSERT(nLocalLevelId <= 255);

    pLinkPortal->pObjectData->InteractType = nLocalLevelId;
    UNITS_SetObjectPortalFlags(pLinkPortal, UNITS_GetObjectPortalFlags(pLinkPortal) | 3);
    UNITS_ChangeAnimMode(pLinkPortal, OBJMODE_OPENED);
    sub_6FCBC9C0(pPortal, pLinkPortal);
    UNITS_StoreOwner(pPortal, pLinkPortal);
    UNITS_StoreOwner(pLinkPortal, pPortal);

    return pLinkPortal;
}

//D2Game.0x6FD13DF0
int32_t __fastcall D2GAME_CreatePortalObject_6FD13DF0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nDestLevel, D2UnitStrc** ppSourceUnit, int32_t nObjectId, int32_t bPerm)
{
    D2_ASSERT(pRoom);

    if (ppSourceUnit)
    {
        *ppSourceUnit = nullptr;
    }

    if (pUnit && DUNGEON_IsRoomInTown(pRoom) && (nDestLevel != LEVEL_MOOMOOFARM || nObjectId != OBJECT_PERMANENT_TOWN_PORTAL))
    {
        SUNIT_AttachSound(pUnit, 0x18u, pUnit);
        return 0;
    }

    const int32_t nAct = DRLG_GetActNoFromLevelId(nDestLevel);
    const int32_t nSourceLevel = DUNGEON_GetLevelIdFromRoom(pRoom);

    D2_ASSERT(nAct == DRLG_GetActNoFromLevelId(nSourceLevel));

    int32_t nTargetX = nX;
    int32_t nTargetY = nY;

    D2CoordStrc coords = {};
    coords.nX = nX;
    coords.nY = nY;
    if (!bPerm)
    {
        if (!COLLISION_GetFreeCoordinatesWithField(pRoom, &coords, &coords, 3, 0x3E01u, 0xC01u, 0))
        {
            return 0;
        }

        nTargetX = coords.nX;
        nTargetY = coords.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = D2GAME_GetRoom_6FC52070(pRoom, nTargetX, nTargetY);
    if (!pTargetRoom)
    {
        return 0;
    }

    D2UnitStrc* pPortal = SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nTargetX, nTargetY, pGame, pTargetRoom, 1, 1, 0);
    if (ppSourceUnit)
    {
        *ppSourceUnit = pPortal;
    }

    UNITS_ChangeAnimMode(pPortal, 1);

    D2UnitStrc* pLinkPortal = D2GAME_CreateLinkPortal_6FD13B20(pGame, pPortal, nDestLevel, nSourceLevel);
    if (pLinkPortal)
    {
        UNITS_SetObjectPortalFlags(pLinkPortal, UNITS_GetObjectPortalFlags(pLinkPortal) | 3);
        DUNGEON_ToggleHasPortalFlag(pTargetRoom, 0);
        DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pLinkPortal), 0);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD14020
int32_t __fastcall D2GAME_GetXAndYFromTargetUnit_6FD14020(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pX, int32_t* pY)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        *pX = CLIENTS_GetUnitX(pTarget);
        *pY = CLIENTS_GetUnitY(pTarget);
    }
    else
    {
        *pX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        *pY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }

    return *pX && *pY;
}

//D2Game.0x6FD140D0
void __fastcall sub_6FD140D0(int32_t* pX, int32_t* pY)
{
    int32_t nDistance = *pX * *pX + *pY * *pY;
    if (nDistance < 4)
    {
        *pX *= 4;
        *pY *= 4;
        nDistance = *pX * *pX + *pY * *pY;
    }

    if (nDistance < 16)
    {
        *pX *= 2;
        *pY *= 2;
    }
}

//D2Game.0x6FD14120
void __fastcall sub_6FD14120(int32_t* pX, int32_t* pY)
{
    int32_t nX = *pY;
    int32_t nY = -*pX;
    while (nX * nX + nY * nY > 3)
    {
        nX /= 2;
        nY /= 2;
    }

    *pX = nX;
    *pY = nY;
}

//D2Game.0x6FD14170
void __fastcall sub_6FD14170(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* a3, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nVelocity)
{
    constexpr int32_t xOffsets[] =
    {
        30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2, 0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29
    };
     
    constexpr int32_t yOffsets[] =
    {
        0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29, 30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2
    };

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 3;
    missileParams.nX = CLIENTS_GetUnitX(a3);
    missileParams.nY = CLIENTS_GetUnitY(a3);
    missileParams.nSkill = nSkillId;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nSkillLevel = nSkillLevel;

    if (nVelocity)
    {
        missileParams.nVelocity = nVelocity;
        missileParams.dwFlags |= 4;
    }

    for (int32_t i = 0; i < 64; ++i)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }
}

//D2Game.0x6FD14260
void __fastcall sub_6FD14260(D2GameStrc* pGame, int32_t nMissiles, D2MissileStrc* missileParams)
{
    constexpr int32_t signX[] =
    {
        -1, 1, 1, -1
    };
    
    constexpr int32_t signY[] =
    {
        -1, -1, 1, 1
    };
    
    constexpr int32_t offset1[] =
    {
        18, 20, 17, 20, 15, 19, 18
    };
    
    constexpr int32_t offset2[] =
    {
        8, 2, 11, 4, 13, 6, 9
    };
    
    constexpr int32_t xOffsets[] =
    {
        20, -20, 0, 0, 14, -14, -14, 14
    };
    
    constexpr int32_t yOffsets[] =
    {
        0, 0, 20, -20, 14, 14, -14, -14
    };

    if ((nMissiles - 1) / 5 > 7)
    {
        FOG_DisplayAssert("(nMissiles - 1) / 5 <= MISSILEARRAY_TABLE_LENGTH", __FILE__, __LINE__);
        exit(-1);
    }

    missileParams->nTargetX = 14;
    missileParams->nTargetY = -14;
    MISSILES_CreateMissileFromParams(pGame, missileParams);

    int32_t nSubMissiles = nMissiles - 1;
    for (int32_t j = 0; j < 7 && nSubMissiles > 0; ++j)
    {
        for (int32_t i = 0; i < 4; ++i)
        {
            if (nSubMissiles <= 0)
            {
                break;
            }

            missileParams->nTargetX = offset1[j] * signX[i];
            missileParams->nTargetY = offset2[j] * signY[i];
            MISSILES_CreateMissileFromParams(pGame, missileParams);

            missileParams->nTargetX = offset2[j] * signX[i];
            missileParams->nTargetY = offset1[j] * signY[i];
            MISSILES_CreateMissileFromParams(pGame, missileParams);
            nSubMissiles -= 2;
        }

        missileParams->nTargetX = xOffsets[j];
        missileParams->nTargetY = yOffsets[j];
        MISSILES_CreateMissileFromParams(pGame, missileParams);
        --nSubMissiles;
    }
}

//D2Game.0x6FD14370
void __fastcall D2GAME_MONSTERS_AiFunction06_6FD14370(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(pSkillsTxtRecord->nAuraState);
        if (pStatesTxtRecord)
        {
            sub_6FD14400(pGame, pUnit, pStatesTxtRecord->wSrvActiveFunc, nSkillId, nSkillLevel);
        }
    }
}

//D2Game.0x6FD14400
void __fastcall sub_6FD14400(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nDoFunc, int32_t nSkillId, int32_t nSkillLevel)
{
    using SkillDoFunc = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t);
    SkillDoFunc gpSkillSrvDoFnTable_6FD40A20[] =
    {
        nullptr,
        SKILLS_SrvDo001_Attack_LeftHandSwing,
        SKILLS_SrvDo002_Kick_PowerStrike_MonIceSpear_Impale_Bash_Stun_Concentrate_BearSmite_Vengeance_Berserk_FireClaws,
        SKILLS_SrvDo003_Throw,
        SKILLS_SrvDo004_Unsummon,
        SKILLS_SrvDo005_LeftHandThrow,
        SKILLS_SrvDo006_InnerSight_SlowMissiles,
        SKILLS_SrvDo007_Jab,
        SKILLS_SrvDo008_MultipleShot_Teeth_ShockWave,
        SKILLS_SrvDo009_Frenzy,
        SKILLS_SrvDo010_GuidedArrow_BoneSpirit,
        SKILLS_SrvDo011_ChargedStrike,
        SKILLS_SrvDo012_Strafe,
        SKILLS_SrvDo013_Fend_Zeal_Fury,
        SKILLS_SrvDo014_LightningStrike,
        SKILLS_SrvDo015_Dopplezon,
        SKILLS_SrvDo016_Valkyrie,
        SKILLS_SrvDo017_ChargedBolt_BoltSentry,
        SKILLS_SrvDo018_DefensiveBuff,
        SKILLS_SrvDo019_Inferno_ArcticBlast,
        SKILLS_SrvDo020_StaticField,
        SKILLS_SrvDo021_Telekinesis,
        SKILLS_SrvDo022_NovaAttack,
        SKILLS_SrvDo023_Blaze_EnergyShield_SpiderLay,
        SKILLS_SrvDo024_FireWall,
        SKILLS_SrvDo025_Enchant,
        SKILLS_SrvDo026_ChainLightning,
        SKILLS_SrvDo027_Teleport,
        SKILLS_SrvDo028_Meteor_Blizzard_Eruption_BaalTaunt_Catapult,
        SKILLS_SrvDo029_ThunderStorm,
        SKILLS_SrvDo030_Curse,
        SKILLS_SrvDo031_RaiseSkeleton_Mage,
        SKILLS_SrvDo032_PoisonDagger,
        SKILLS_SrvDo033_PsychicHammer,
        SKILLS_SrvDo034_TigerStrike_CobraStrike_RoyalStrike,
        SKILLS_SrvDo035_FistsOfFire_ClawsOfThunder_BladesOfIce,
        SKILLS_SrvDo036_ClawsOfThunder_ProgressiveFn2,
        SKILLS_SrvDo037_ClawsOfThunder_ProgressiveFn3,
        SKILLS_SrvDo038_FistsOfFire_BladesOfIce_ProgressiveFn2,
        SKILLS_SrvDo039_FistsOfFire_BladesOfIce_ProgressiveFn3,
        SKILLS_SrvDo040_RoyalStrike_ProgressiveFn1,
        SKILLS_SrvDo041_RoyalStrike_ProgressiveFn3,
        SKILLS_SrvDo042_DragonTalon,
        SKILLS_SrvDo043_ShockField,
        SKILLS_SrvDo044_BladeSentinel,
        SKILLS_SrvDo045_Sentry,
        SKILLS_SrvDo046_DragonClaw,
        SKILLS_SrvDo047_CloakOfShadows,
        SKILLS_SrvDo048_BladeFury,
        SKILLS_SrvDo049_ShadowWarrior_Master,
        SKILLS_SrvDo050_DragonTail,
        SKILLS_SrvDo051_MindBlast,
        SKILLS_SrvDo052_DragonFlight,
        SKILLS_SrvDo053_Unused,
        SKILLS_SrvDo054_BladeShield,
        SKILLS_SrvDo055_CorpseExplosion,
        SKILLS_SrvDo056_Golem,
        SKILLS_SrvDo057_IronGolem,
        SKILLS_SrvDo058_Revive,
        SKILLS_SrvDo059_Attract,
        SKILLS_SrvDo060_BoneWall,
        SKILLS_SrvDo061_Confuse,
        SKILLS_SrvDo062_BonePrison,
        SKILLS_SrvDo063_PoisonExplosion,
        SKILLS_SrvDo064_Sacrifice,
        SKILLS_SrvDo065_BasicAura,
        SKILLS_SrvDo066_HolyFire_HolyShock_Sanctuary_Conviction,
        SKILLS_SrvDo067_Charge,
        SKILLS_SrvDo068_BasicShout,
        SKILLS_SrvDo069_FindPotion,
        SKILLS_SrvDo070_DoubleSwing,
        SKILLS_SrvDo071_Taunt,
        SKILLS_SrvDo072_FindItem,
        SKILLS_SrvDo073_BlessedHammer,
        SKILLS_SrvDo074_DoubleThrow,
        SKILLS_SrvDo075_GrimWard,
        SKILLS_SrvDo076_Whirlwind,
        SKILLS_SrvDo077_Leap,
        SKILLS_SrvDo078_LeapAttack,
        SKILLS_SrvDo079_Conversion,
        SKILLS_SrvDo080_FistOfTheHeavens,
        SKILLS_SrvDo081_HolyFreeze,
        SKILLS_SrvDo082_Redemption,
        SKILLS_SrvDo083_FireHit,
        SKILLS_SrvDo084_MaggotEgg,
        SKILLS_SrvDo085_UnholyBolt_ShamanFire,
        SKILLS_SrvDo086_MaggotDown,
        SKILLS_SrvDo087_MaggotLay,
        SKILLS_SrvDo088_AndrialSpray,
        SKILLS_SrvDo089_Jump,
        SKILLS_SrvDo090_SwarmMove,
        SKILLS_SrvDo091_Nest_EvilHutSpawner,
        SKILLS_SrvDo092_QuickStrike,
        SKILLS_SrvDo093_GargoyleTrap,
        SKILLS_SrvDo094_Submerge,
        SKILLS_SrvDo095_MonInferno,
        SKILLS_SrvDo096_ZakarumHeal_Bestow,
        SKILLS_SrvDo097_Resurrect,
        SKILLS_SrvDo098_MonTeleport,
        SKILLS_SrvDo099_PrimePoisonNova,
        SKILLS_SrvDo100_DiabCold,
        SKILLS_SrvDo101_FingerMageSpider,
        SKILLS_SrvDo102_DiabWall,
        SKILLS_SrvDo103_DiabRun,
        SKILLS_SrvDo104_DiabPrison,
        SKILLS_SrvDo105_DesertTurret,
        SKILLS_SrvDo106_ArcaneTower,
        SKILLS_SrvDo107_Mosquito,
        SKILLS_SrvDo108_RegurgitatorEat,
        SKILLS_SrvDo109_MonFrenzy,
        SKILLS_SrvDo110_Hireable_RogueMissile,
        SKILLS_SrvDo111_FetishAura,
        SKILLS_SrvDo112_MonCurseCast,
        SKILLS_SrvDo113_Scroll_Book,
        SKILLS_SrvDo114_Raven,
        SKILLS_SrvDo115_Vines,
        SKILLS_SrvDo116_Wearwolf_Wearbear,
        SKILLS_SrvDo117_Firestorm,
        SKILLS_SrvDo118_Twister_Tornado,
        SKILLS_SrvDo119_DruidSummon,
        SKILLS_SrvDo120_FeralRage_Maul,
        SKILLS_SrvDo121_Rabies,
        SKILLS_SrvDo122_Hunger,
        SKILLS_SrvDo123_Volcano,
        SKILLS_SrvDo124_Armageddon_Hurricane,
        SKILLS_SrvDo125_WakeOfDestruction,
        SKILLS_SrvDo126_ImpInferno,
        SKILLS_SrvDo127_SuckBlood,
        SKILLS_SrvDo128_CryHelp,
        SKILLS_SrvDo129_ImpTeleport,
        SKILLS_SrvDo130_VineAttack,
        SKILLS_SrvDo131_OverseerWhip,
        SKILLS_SrvDo132_ImpFireMissile,
        SKILLS_SrvDo133_Impregnate,
        SKILLS_SrvDo134_SiegeBeastStomp,
        SKILLS_SrvDo135_MinionSpawner,
        SKILLS_SrvDo136_DeathMaul,
        SKILLS_SrvDo137_FenrisRage,
        SKILLS_SrvDo138_Unused,
        SKILLS_SrvDo139_BaalColdMissiles,
        SKILLS_SrvDo140_BaalTentacle,
        SKILLS_SrvDo141_BaalCorpseExplode,
        SKILLS_SrvDo142_Unused,
        SKILLS_SrvDo143_FistsOfFire_RoyalStrike_ProgressiveFn,
        SKILLS_SrvDo144_Hydra,
        SKILLS_SrvDo145_Unused,
        SKILLS_SrvDo146_Unused,
        SKILLS_SrvDo147_Unused,
        SKILLS_SrvDo148_DoomKnightMissile,
        SKILLS_SrvDo149_NecromageMissile,
        SKILLS_SrvDo150_Smite,
        SKILLS_SrvDo151_Unused,
        SKILLS_SrvDo152_DiabLight,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    if (nDoFunc >= 0 && nDoFunc < std::size(gpSkillSrvDoFnTable_6FD40A20) && gpSkillSrvDoFnTable_6FD40A20[nDoFunc])
    {
        gpSkillSrvDoFnTable_6FD40A20[nDoFunc](pGame, pUnit, nSkillId, nSkillLevel);
    }
}

//D2Game.0x6FD14430
D2UnitStrc* __fastcall D2GAME_SummonPet_6FD14430(D2GameStrc* pGame, D2SummonArgStrc* pSummonArg)
{
    int32_t nX = 0;
    int32_t nY = 0;

    if (!(pSummonArg->dwFlags & 1))
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pSummonArg->pOwner);
        if (pTarget)
        {
            nX = CLIENTS_GetUnitX(pTarget);
            nY = CLIENTS_GetUnitY(pTarget);
        }
        else
        {
            nX = D2COMMON_10175_PathGetFirstPointX(pSummonArg->pOwner->pDynamicPath);
            nY = D2COMMON_10176_PathGetFirstPointY(pSummonArg->pOwner->pDynamicPath);
        }
    }
    else
    {
        nX = pSummonArg->pPosition.nX;
        nY = pSummonArg->pPosition.nY;
    }

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pSummonArg->pOwner), nX, nY);
    if (!pRoom)
    {
        return nullptr;
    }

    D2UnitStrc* pPet = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, pSummonArg->nHcIdx, pSummonArg->nMonMode, -1, 66);
    if (!pPet)
    {
        if (pSummonArg->dwFlags & 4)
        {
            return nullptr;
        }

        pPet = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, pSummonArg->nHcIdx, pSummonArg->nMonMode, 4, 66);
        if (!pPet)
        {
            return nullptr;
        }
    }

    if (!(pSummonArg->dwFlags & 8) && pPet)
    {
        pPet->dwFlags |= UNITFLAG_ISREVIVE;
    }

    if (pSummonArg->dwFlags & 2)
    {
        sub_6FD146D0(pGame, pSummonArg->pOwner);
        if (pPet)
        {
            D2GAME_SetNecropetFlag_6FCBD760(pSummonArg->pOwner, pPet->dwUnitId);
        }
        else
        {
            D2GAME_SetNecropetFlag_6FCBD760(pSummonArg->pOwner, -1);
        }
    }
    
    int32_t nPetMax = pSummonArg->nPetMax;
    if (nPetMax < 1)
    {
        nPetMax = 1;
    }
    sub_6FC7D7A0(pGame, pSummonArg->pOwner, pPet, pSummonArg->nPetType, nPetMax);

    if (pPet)
    {
        pPet->dwFlags |= UNITFLAG_NOTC;
    }

    if (pSummonArg->pOwner)
    {
        AIGENERAL_SetOwnerData(pGame, pPet, pSummonArg->pOwner->dwUnitId, pSummonArg->pOwner->dwUnitType, 0, 0);
    }
    else
    {
        AIGENERAL_SetOwnerData(pGame, pPet, -1, 6, 0, 0);
    }

    D2_ASSERT(!STATES_CheckState(pPet, STATE_UNINTERRUPTABLE));

    AITHINK_ExecuteAiFn(pGame, pPet, AIGENERAL_GetAiControlFromUnit(pPet), pSummonArg->nAiSpecialState);
    MONSTER_UpdateAiCallbackEvent(pGame, pPet);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPet, UNITEVENTCALLBACK_AITHINK, 0);
    EVENT_SetEvent(pGame, pPet, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 25, 0, 0);

    return pPet;
}

//D2Game.0x6FD146D0
void __fastcall sub_6FD146D0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, D2GAME_GetNecropetFlag_6FCBD790(pUnit));
    if (pPet)
    {
        if ((pPet->dwFlagEx >> 8) & 1)
        {
            SUNIT_RemoveUnit(pGame, pPet);
            D2GAME_SetNecropetFlag_6FCBD760(pUnit, -1);
            return;
        }

        pPet->dwFlags |= 0x4000000u;

        SUNITDMG_KillMonster(pGame, pPet, pUnit, 1);
    }

    D2GAME_SetNecropetFlag_6FCBD760(pUnit, -1);
}

//D2Game.0x6FD14770
void __fastcall sub_6FD14770(D2GameStrc* pGame, int32_t nFlags, D2UnitStrc* pUnit, int32_t nSkillLevel, int32_t nRange, CurseCallbackFunc pfCurse)
{
    D2_ASSERT(pfCurse);

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    int32_t nX = 0;
    int32_t nY = 0;
    if (pTarget)
    {
        nX = CLIENTS_GetUnitX(pTarget);
        nY = CLIENTS_GetUnitY(pTarget);
    }
    else
    {
        nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }

    if (nX && nY)
    {
        D2UnitFindArgStrc unitFindArg = {};
        unitFindArg.nFlags = nFlags;
        unitFindArg.pUnit = pUnit;
        unitFindArg.nX = nX;
        unitFindArg.nY = nY;
        unitFindArg.nSize = nRange;

        D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
        if (pRoom)
        {
            D2UnitFindDataStrc unitFindData = {};

            UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, pRoom, nX, nY, nRange, 0, &unitFindArg);
            UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);
            if (IsBadCodePtr((FARPROC)pfCurse))
            {
                FOG_DisplayAssert("pfnCurse", __FILE__, __LINE__);
                exit(-1);
            }

            for (int32_t i = 0; i < unitFindData.nIndex; ++i)
            {
                pfCurse(pGame, pUnit, unitFindData.pUnitsArray[i], nSkillLevel);
            }
            UNITFINDS_FreeUnitFindData(&unitFindData);
        }
    }
}

//D2Game.0x6FD149B0
void __fastcall sub_6FD149B0(D2GameStrc* pGame, D2EffectStrc* pEffect)
{
    D2_ASSERT(pEffect);
    D2_ASSERT(pEffect->pfAreaEffect);

    int32_t nParam = pEffect->nParam;
    if (!nParam)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pEffect->nSkillId);
        if (pSkillsTxtRecord && pEffect->nSkillLevel > 0)
        {
            nParam = pSkillsTxtRecord->dwParam[0] + (pEffect->nSkillLevel - 1) * pSkillsTxtRecord->dwParam[1];
        }
    }

    D2UnitFindArgStrc unitFindArg = {};
    unitFindArg.nFlags = pEffect->nFlags;
    unitFindArg.pUnit = pEffect->pUnit;
    unitFindArg.nX = pEffect->nX;
    unitFindArg.nY = pEffect->nY;
    unitFindArg.nSize = nParam;

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pEffect->pUnit), pEffect->nX, pEffect->nY);
    if (pRoom)
    {
        D2UnitFindDataStrc unitFindData = {};
        UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, pRoom, pEffect->nX, pEffect->nY, nParam, 0, &unitFindArg);
        unitFindData.nFlags = pEffect->nFlags;
        UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);

        if (IsBadCodePtr((FARPROC)pEffect->pfAreaEffect))
        {
            FOG_DisplayAssert("ptEffect->pfnAreaAffect", __FILE__, __LINE__);
            exit(-1);
        }

        for (int32_t i = 0; i < unitFindData.nIndex; ++i)
        {
            ((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int32_t, int32_t))pEffect->pfAreaEffect)(pGame, pEffect->pUnit, unitFindData.pUnitsArray[i], pEffect->nSkillLevel, pEffect->unk0x1C);
        }

        UNITFINDS_FreeUnitFindData(&unitFindData);
    }

}

//D2Game.0x6FD14BD0
void __fastcall sub_6FD14BD0(D2GameStrc* pGame, int32_t nFlags, int32_t nX, int32_t nY, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, void* pfAreaEffect)
{
    D2EffectStrc effect = {};

    effect.nFlags = nFlags;
    effect.nY = nY;
    effect.pUnit = pUnit;
    effect.nX = nX;
    effect.nSkillLevel = nSkillLevel;
    effect.pfAreaEffect = pfAreaEffect;
    effect.nSkillId = nSkillId;
    sub_6FD149B0(pGame, &effect);
}

//D2Game.0x6FD14C30
void __fastcall sub_6FD14C30(D2UnitStrc* pUnit)
{
    SKILLS_RefreshPassiveSkills(pUnit);
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC7DF40(SUNIT_GetGameFromUnit(pUnit), pUnit);
    }
}

//D2Game.0x6FD14C60
void __fastcall D2GAME_SetSkills_6FD14C60(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bRemove)
{
    if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        SKILLS_AssignSkill(pUnit, nSkillId, nSkillLevel, bRemove, __FILE__, __LINE__);
        sub_6FD14C30(pUnit);
    }
}

//D2Game.0x6FD14CC0
void __fastcall D2GAME_RefreshPassiveSkills_6FD14CC0(D2UnitStrc* pUnit)
{
    if (pUnit->pSkills)
    {
        for (D2SkillStrc* i = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills); i; i = SKILLS_GetNextSkill(i))
        {
            const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(i, __FILE__, __LINE__);
            const int16_t nPassiveState = SKILLS_GetPassiveState(nSkillId);
            if (nPassiveState > 0)
            {
                STATES_ToggleState(pUnit, nPassiveState, 1);
                SKILLS_RefreshSkill(pUnit, nSkillId);
            }
        }
    }
}

//D2Game.0x6FD14D20
void __fastcall sub_6FD14D20(D2UnitStrc* pUnit, int32_t nUnused, D2MonSkillInfoStrc* pMonSkillInfo)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
        if (pOwner)
        {
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
            if (pMonStatsTxtRecord && pMonStatsTxtRecord->wSkillDamage > 0 && pMonStatsTxtRecord->wSkillDamage <= sgptDataTables->nSkillsTxtRecordCount)
            {
                const int32_t nSkillLevel = SKILLS_GetHighestSkillLevelById(pOwner, pMonStatsTxtRecord->wSkillDamage);
                pMonSkillInfo->nMinDamage = SKILLS_GetMinPhysDamage(pOwner, pMonStatsTxtRecord->wSkillDamage, nSkillLevel, 1) >> 8;
                pMonSkillInfo->nMaxDamage = SKILLS_GetMaxPhysDamage(pOwner, pMonStatsTxtRecord->wSkillDamage, nSkillLevel, 1) >> 8;
                pMonSkillInfo->nToHit = SKILLS_GetToHitFactor(pOwner, pMonStatsTxtRecord->wSkillDamage, nSkillLevel);
            }
        }
    }
}

//D2Game.0x6FD14DD0
int32_t __fastcall D2GAME_RollElementalDamage_6FD14DD0(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        const int32_t nMinDamage = SKILLS_GetMinElemDamage(pUnit, nSkillId, nSkillLevel, 1);
        const int32_t nMaxDamage = SKILLS_GetMaxElemDamage(pUnit, nSkillId, nSkillLevel, 1);
        const int32_t nDamage = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage) + nMinDamage;
        sub_6FD11E40(pUnit, pDamage, pSkillsTxtRecord->nEType, nDamage, SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1), 0, 0);
        return nDamage;
    }

    return 0;
}

//D2Game.0x6FD14EC0
void __fastcall D2GAME_RollPhysicalDamage_6FD14EC0(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        const int32_t nMinDamage = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
        const int32_t nMaxDamage = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
        const int32_t nDamage = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage) + nMinDamage;
        pDamage->dwPhysDamage += nDamage;
    }
}

//D2Game.0x6FD14F70
void __fastcall sub_6FD14F70(D2GameStrc* pGame, D2UnitStrc* pUnused, D2UnitStrc* pUnit, int32_t nDelay)
{
    if (!STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE) && pUnit && pUnit->dwUnitType == UNIT_MONSTER && !MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE) && !MONSTERS_IsBoss(nullptr, pUnit))
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nDelay);
        if (pStatList)
        {
            D2COMMON_10476(pStatList, pGame->dwGameFrame + nDelay);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + nDelay, 0, 0);
        }
        else
        {
            pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, pGame->dwGameFrame + 100, pUnit->dwUnitType, pUnit->dwUnitId);
            if (pStatList)
            {
                EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + 100, 0, 0);
                STATLIST_SetState(pStatList, STATE_STUNNED);
                STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
                D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                STATES_ToggleState(pUnit, STATE_STUNNED, 1);
            }
        }

        D2ModeChangeStrc modeChange = {};
        MONSTERMODE_GetModeChangeInfo(pUnit, 1, &modeChange);
        D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
        pUnit->dwFlags &= 0xFFFFFFFE;
    }
}

//D2Game.0x6FD15080
void __fastcall sub_6FD15080(D2UnitStrc* pUnit, int32_t a2)
{
    sub_6FCBCFD0(SUNIT_GetGameFromUnit(pUnit), pUnit, a2);
}

//D2Game.0x6FD150A0
int32_t __fastcall sub_6FD150A0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pTarget);
    D2_ASSERT(pGame);

    SUNIT_SetCombatMode(pGame, pUnit, 1);
    UNITS_SetUsedSkill(pUnit, 0);

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, 0, -1);
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, pSkill, PLRMODE_ATTACK1, pTarget->dwUnitType, pTarget->dwUnitId, 1);
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2ModeChangeStrc pModeChange = {};
        MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_ATTACK1, &pModeChange);
        pModeChange.pTargetUnit = pTarget;
        D2GAME_ModeChange_6FC65220(pGame, &pModeChange, 1);
    }

    return 1;
}

//D2Game.0x6FD15190
int32_t __fastcall sub_6FD15190(D2UnitStrc* pUnit, D2C_AiSpecialState nAiSpecialState)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || pUnit->dwClassId < 0)
    {
        return AIUTIL_CanUnitSwitchAi(pUnit, nAiSpecialState) != 0;
    }

    int32_t nClassId = pUnit->dwClassId;
    if (nClassId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = &sgptDataTables->pMonStatsTxt[nClassId];
        if (pMonStatsTxtRecord)
        {
            nClassId = pMonStatsTxtRecord->nBaseId;
        }
    }

    if (nClassId == MONSTER_IMP1 && STATES_CheckState(pUnit, STATE_ATTACHED))
    {
        return 0;
    }
    else
    {
        return AIUTIL_CanUnitSwitchAi(pUnit, nAiSpecialState) != 0;
    }
}

//D2Game.0x6FD15210
D2UnitStrc* __fastcall sub_6FD15210(D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit && nSkillLevel > 0 && pTarget)
    {
        const int32_t nX = CLIENTS_GetUnitX(pTarget);
        const int32_t nY = CLIENTS_GetUnitY(pTarget);

        D2UnitFindArgStrc unitFindArg = {};
        unitFindArg.nFlags = 4098;
        unitFindArg.nX = nX;
        unitFindArg.nY = nY;
        unitFindArg.nSize = 10;
        
        D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);

        D2UnitFindDataStrc unitFindData = {};
        UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, UNITS_GetRoom(pTarget), nX, nY, 10, sub_6FD15320, &unitFindArg);
        UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);
        
        D2UnitStrc* pResult = nullptr;
        if (unitFindData.nIndex)
        {
            pResult = *unitFindData.pUnitsArray;
        }

        UNITFINDS_FreeUnitFindData(&unitFindData);
        return pResult;
    }

    return nullptr;
}

//D2Game.0x6FD15320
int32_t __fastcall sub_6FD15320(D2UnitStrc* pUnit, D2UnitFindArgStrc* pArg)
{
    if (D2COMMON_11021(pUnit))
    {
        return UNITFINDS_TestUnit(pUnit, pArg);
    }

    return 0;
}

//D2Game.0x6FD15340
int32_t __fastcall sub_6FD15340(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMissileId)
{
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(nMissileId);
    D2_ASSERT(pMissilesTxtRecord);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        int32_t nX = 0;
        int32_t nY = 0;
        if (pTarget)
        {
            nX = CLIENTS_GetUnitX(pTarget);
            nY = CLIENTS_GetUnitY(pTarget);
        }
        else
        {
            nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
            nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
        }

        if (nX && nY)
        {
            return !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pMissilesTxtRecord->nSize, pMissilesTxtRecord->nSize, 5u);
        }
    }

    return 0;
}

//D2Game.0x6FD15460
int32_t __fastcall sub_6FD15460(D2UnitStrc* pUnit)
{
    return UNITS_GetMeleeRange(pUnit) + 4;
}

//D2Game.0x6FD15470
void __fastcall sub_6FD15470(D2UnitStrc* pUnit, int32_t nAttackRate)
{
    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    D2StatListStrc* pStatList = STATLIST_AllocStatList(0, 4u, 0, nUnitType, nUnitGUID);
    if (pStatList)
    {
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_ATTACKRATE, nAttackRate, 0);
        UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    }
}

//D2Game.0x6FD154D0
void __fastcall sub_6FD154D0(D2UnitStrc* pUnit)
{
    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
    if (pOwner && pOwner->dwUnitType == UNIT_MONSTER)
    {
        if (pOwner == pUnit)
        {
            sub_6FCCF4B0(pUnit);
        }
        else
        {
            AIGENERAL_FreeMinionList(pUnit);
        }
    }
}

//D2Game.0x6FD15500
int32_t __fastcall sub_6FD15500(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
            if (pCharStatsTxtRecord)
            {
                return pCharStatsTxtRecord->nWalkSpeed << 8;
            }
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
            if (pMonStatsTxtRecord)
            {
                return pMonStatsTxtRecord->nVelocity << 8;
            }
        }
    }

    return 0x600;
}

//D2Game.0x6FD15580
int32_t __fastcall D2GAME_GetSummonIdFromSkill_6FD15580(D2UnitStrc* pUnit, int32_t bFromMonster, int32_t nSkillId, int32_t nSkillLevel, int32_t* pSpawnMode, int32_t* pX, int32_t* pY)
{
    int32_t nMonsterId = MONSTERS_GetSpawnMode_XY(pUnit, bFromMonster, nSkillId, nSkillLevel, pSpawnMode, pX, pY);

    if (nMonsterId < 0 || nMonsterId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        nMonsterId = AIGENERAL_GetMinionSpawnClassId(pUnit);
        if (nMonsterId <= 0 || nMonsterId >= sgptDataTables->nMonStatsTxtRecordCount)
        {
            nMonsterId = -1;
        }
    }

    return nMonsterId;
}

//D2Game.0x6FD155E0
void __fastcall sub_6FD155E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel, D2DamageStrc* pDamage, int32_t a8)
{
    if (pSkillsTxtRecord->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        pDamage->wResultFlags = pSkillsTxtRecord->wResultFlags;
    }
    else
    {
        pDamage->wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), a8);
        if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            pDamage->wResultFlags |= pSkillsTxtRecord->wResultFlags;
        }
    }
}

//D2Game.0x6FD15650
void __fastcall sub_6FD15650(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, uint16_t wResultFlags, int32_t a5, D2DamageStrc* pDamage, int32_t a7)
{
    if (wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        pDamage->wResultFlags = wResultFlags;
    }
    else
    {
        pDamage->wResultFlags = SUNITDMG_GetResultFlags(pGame, pOwner, pUnit, a5, a7);
        if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            pDamage->wResultFlags |= wResultFlags;
        }
    }
}

//D2Game.0x6FD156A0
D2UnitEventStrc* __fastcall sub_6FD156A0(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nTimerType, int32_t a4, int32_t a5, int32_t nEventFunc, int32_t a7, int32_t a8)
{
    constexpr D2UnitEventCallbackFunction gpEventCallbackTable_6FD40D20[] =
    {
        nullptr,
        SKILLS_EventFunc01_ChillingArmor,
        SKILLS_EventFunc02_FrozenArmor,
        SKILLS_EventFunc03_ShiverArmor,
        D2GAME_EventFunc04_6FD0E840,
        D2GAME_EventFunc05_6FD0EDE0,
        SKILLITEM_EventFunc06_AttackerTakesDamage,
        SKILLITEM_EventFunc07_Knockback,
        SKILLITEM_EventFunc08_Howl,
        SKILLITEM_EventFunc09_Stupidity,
        SKILLITEM_EventFunc10_AttackerTakesLightDamage,
        SKILLITEM_EventFunc11_ApplyFireDamage,
        SKILLITEM_EventFunc12_ApplyColdDamage,
        SKILLITEM_EventFunc13_DamageToMana,
        SKILLITEM_EventFunc14_Freeze,
        SKILLITEM_EventFunc15_OpenWounds,
        SKILLITEM_EventFunc16_CrushingBlow,
        SKILLITEM_EventFunc17_ManaAfterKill,
        SKILLITEM_EventFunc18_HealAfterDemonKill,
        SKILLITEM_EventFunc19_Slow,
        SKILLITEM_EventFunc20_SkillOnAttackHitKill,
        SKILLITEM_EventFunc21_SkillOnGetHit,
        D2GAME_EventFunc22_6FD0F000,
        D2GAME_EventFunc23_6FD0F1F0,
        SKILLS_EventFunc24_EnergyShield,
        D2GAME_EventFunc25_6FD00140,
        D2GAME_EventFunc26_6FD0F5E0,
        D2GAME_EventFunc27_6FD0F7A0,
        SKILLITEM_EventFunc28_HealAfterKill,
        SKILLITEM_EventFunc29_RestInPeace,
        SKILLITEM_EventFunc30_SkillOnDeathLevelup,
        SKILLITEM_EventFunc31_Reanimate,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    if (nEventFunc >= 0 && nEventFunc < std::size(gpEventCallbackTable_6FD40D20) && gpEventCallbackTable_6FD40D20[nEventFunc])
    {
        return SUNITEVENT_AllocTimer(pGame, pUnit, nTimerType, a4, a5, gpEventCallbackTable_6FD40D20[nEventFunc], a7, a8);
    }

    return nullptr;
}




//D2Game.0x6FC808D0
int32_t __fastcall SKILLS_SrvSt18_Attract(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    return 1;
}


D2StatesTxt* __fastcall SKILLS_GetStatesTxtRecord(int32_t nStateId)
{
    if (nStateId >= 0 && nStateId < sgptDataTables->nStatesTxtRecordCount)
    {
        return &sgptDataTables->pStatesTxt[nStateId];
    }

    return nullptr;
}

D2ItemStatCostTxt* __fastcall SKILLS_GetItemStatCostTxtRecord(int32_t nStatId)
{
    if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return &sgptDataTables->pItemStatCostTxt[nStatId];
    }

    return nullptr;
}

D2MissilesTxt* __fastcall SKILLS_GetMissilesTxtRecord(int32_t nMissileId)
{
    if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        return &sgptDataTables->pMissilesTxt[nMissileId];
    }

    return nullptr;
}