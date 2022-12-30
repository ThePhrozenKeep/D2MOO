#pragma once

#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>


#pragma pack(push, 1)
struct D2AuraCallbackStrc;
struct D2MissilesTxt;

struct D2MissileUnitFindArgStrc
{
    D2GameStrc* pGame;
    D2UnitStrc* pMissile;
    D2UnitStrc* pOwner;
    D2MissilesTxt* pMissilesTxtRecord;
};

using MissileUnitFindFunc = int32_t(__fastcall*)(D2UnitStrc*, void*);
struct D2MissileUnitFindTableStrc
{
    MissileUnitFindFunc pfUnitFindCallback;
    int32_t nCollisionMask;
};
#pragma pack(pop)


extern D2MissileUnitFindTableStrc stru_6FD2E5F8[9];


//D2Game.0x6FC55CE0
int32_t __fastcall MISSMODE_UnitFindCallback_CanCollideWithMonster(D2UnitStrc* pUnit, void* pArgument);
//D2Game.0x6FC55D90
int32_t __fastcall MISSMODE_UnitFindCallback_CanCollideWithAlignUnit(D2UnitStrc* pUnit, void* pArgument);
//D2Game.0x6FC55E60
int32_t __fastcall MISSMODE_UnitFindCallback_CanCollideWithPlayerOrMonster(D2UnitStrc* pUnit, void* pArgument);
//D2Game.0x6FC55F20
int32_t __fastcall MISSMODE_UnitFindCallback_CanMissileDestroy(D2UnitStrc* pUnit, void* pArgument);
//D2Game.0x6FC55F80
void __stdcall MISSMODE_FillDamageParams(D2UnitStrc* pMissile, D2UnitStrc* pTarget, D2DamageStrc* pDamage);
//D2Game.0x6FC56290
int32_t __fastcall MISSMODE_RollDamageValue(D2UnitStrc* pUnit, int32_t nMinDamStat, int32_t nMaxDamStat, int32_t nMasteryStat);
//D2Game.0x6FC56480
int32_t __fastcall MISSMODE_GetDamageValue(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage);
//D2Game.0x6FC56730
void __fastcall MISSMODE_ResetDamageParams(D2GameStrc* pGame, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC567E0
void __fastcall MISSMODE_AddDamageValue(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nDamage);
//D2Game.0x6FC568F0
int32_t __fastcall MISSMODE_CreatePoisonCloudHitSubmissiles(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nSubStep, int32_t nMainStep, int32_t nLoops);
//D2Game.0x6FC56AB0
void __fastcall MISSMODE_CreateImmolationArrowHitSubmissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nMissileId, int32_t nRange);
//D2Game.0x6FC56D50
int32_t __fastcall MISSMODE_HandleMissileCollision(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC56FA0
D2UnitStrc* __fastcall MISSMODE_CreatePlagueJavelin_PoisonJavelin_PoisonTrapHitSubmissiles(D2GameStrc* pGame, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nRange, int32_t nLoops);
//D2Game.0x6FC570B0
int32_t __fastcall MISSMODE_SrvDo02_PlagueJavelin_PoisonJavelin_PoisonTrap(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57140
int32_t __fastcall MISSMODE_SrvDo03_PoisonCloud_Blizzard_ThunderStorm_HandOfGod(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC571D0
int32_t __fastcall MISSMODE_SrvDo05_FireWall_ImmolationFire_MeteorFire_MoltenBoulderFirePath(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC573B0
int32_t __fastcall MISSMODE_SrvDo06_MoltenBoulder_FireWallMaker(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57510
int32_t __fastcall MISSMODE_SrvDo07_GuidedArrow_BoneSpirit(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57640
int32_t __fastcall MISSMODE_SrvDo08_MonBlizzCenter(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC576F0
D2UnitStrc* __fastcall MISSMODE_CreateMissileWithCollisionCheck(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nRange, int32_t nFrames, int32_t nMissileId, uint16_t nCollisionMask);
//D2Game.0x6FC57910
int32_t __fastcall MISSMODE_SrvDo09_BatLightningBolt(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57A40
int32_t __fastcall MISSMODE_SrvDo10_BlizzardCenter(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57B60
int32_t __fastcall MISSMODE_SrvDo11_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57DA0
int32_t __fastcall MISSMODE_SrvDo12_DiabWallMaker(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC57F00
int32_t __fastcall MISSMODE_SrvDo13_BoneWallMaker(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58140
int32_t __fastcall MISSMODE_SrvDo14_GrimWard(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC581F0
int32_t __fastcall MISSMODE_SrvDo15_FrozenOrb(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58340
int32_t __fastcall MISSMODE_SrvDo16_FrozenOrbNova(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58480
int32_t __fastcall MISSMODE_SrvDo17_CairnStones(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC585E0
int32_t __fastcall MISSMODE_SrvDo18_TowerChestSpawner(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58860
int32_t __fastcall MISSMODE_SrvDo20_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58940
int32_t __fastcall MISSMODE_SrvDo21_Distraction(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58B00
int32_t __fastcall MISSMODE_SrvDo22_LightningTrailingJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58CC0
int32_t __fastcall MISSMODE_SrvDo23_24_SuccFireBall_FirestormMaker(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58E10
int32_t __fastcall MISSMODE_SrvDo25_EruptionCenter(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC58F30
int32_t __fastcall MISSMODE_SrvDo26_Vines_PlagueVines(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC59040
int32_t __fastcall MISSMODE_SrvDo27_Tornado(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC591C0
int32_t __fastcall MISSMODE_SrvDo28_Volcano(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC594B0
int32_t __fastcall MISSMODE_SrvDo29_RecyclerDelay(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC596C0
int32_t __fastcall MISSMODE_SrvDo33_VineRecyclerDelay(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC598D0
int32_t __fastcall MISSMODE_SrvDo30_RabiesPlague(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC59B90
int32_t __fastcall MISSMODE_SrvDo31_WakeOfDestructionMaker_BaalColdMaker(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC59CB0
int32_t __fastcall MISSMODE_SrvDo32_TigerFury(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC59D80
int32_t __fastcall MISSMODE_SrvDo34_BaalTauntControl(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC59F
int32_t __fastcall MISSMODE_SrvDo35_RoyalStrikeChaosIce(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC5A180
int32_t __fastcall MISSMODE_SrvHit01_Fireball_ExplodingArrow_FreezingArrowExplosion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5A330
int32_t __fastcall MISSMODE_SrvHit24_PantherPotOrange(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5A4F0
int32_t __fastcall MISSMODE_SrvHit02_PlagueJavelin_PoisonPotion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5A580
int32_t __fastcall MISSMODE_SrvHit44_ExplodingJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5A740
int32_t __fastcall MISSMODE_SrvHit03_ExplosivePotion_BombOnGround(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5A760
int32_t __fastcall MISSMODE_SrvHit07_HolyBolt_FistOfTheHeavenBolt(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5A9B0
int32_t __fastcall MISSMODE_SrvHit08_Blaze(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5AA00
int32_t __fastcall MISSMODE_SrvHit06_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5AB00
int32_t __fastcall MISSMODE_SrvHit09_ImmolationArrow(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5AD40
int32_t __fastcall MISSMODE_SrvHit10_GuidedArrow_BoneSpirit(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5B020
int32_t __fastcall MISSMODE_SrvHit12_ChainLightning_LightningStrike(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5B280
int32_t __fastcall MISSMODE_SrvHit13_GlacialSpike_HellMeteorDown(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5B4E0
int32_t __fastcall MISSMODE_SrvHit04_ExplodingArrow_FreezingArrow_RoyalStrikeMeteorCenter(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5B5C0
int32_t __fastcall MISSMODE_SrvHit11_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5B6A0
int32_t __fastcall MISSMODE_SrvHit14_MeteorCenter_CatapultMeteor_RoyalStrikeMeteor(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5B910
void __fastcall MISSMODE_CreateMeteor_MoltenBoulderSubmissiles(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nMissileId, int32_t nRange, int32_t nStep);
//D2Game.0x6FC5BA50
int32_t __fastcall MISSMODE_SrvHit15_SpiderGooLay(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5BB80
int32_t __fastcall MISSMODE_SrvHit16_SpiderGoo_VinesTrail_VinesWither(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5BD30
int32_t __fastcall MISSMODE_SrvHit17_Howl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5BED0
int32_t __fastcall MISSMODE_SrvHit18_Shout_BattleCommand_BattleOrders(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5BF30
int32_t __fastcall MISSMODE_SrvHit19_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5C0D0
int32_t __fastcall MISSMODE_SrvHit20_LightningFury(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5C2D0
int32_t __fastcall MISSMODE_LightningFury_AuraCallback(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pTarget);
//D2Game.0x6FC5C3A0
int32_t __fastcall MISSMODE_SrvHit21_BattleCry(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5C4D0
int32_t __fastcall MISSMODE_SrvHit22_FistOfTheHeavensDelay(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5C790
int32_t __fastcall MISSMODE_FistOfTheHeavensDelay_AuraCallback(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pTarget);
//D2Game.0x6FC5C860
int32_t __fastcall MISSMODE_CreatePantherPotGreenSubmissiles(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep);
//D2Game.0x6FC5C990
int32_t __fastcall MISSMODE_SrvHit25_PantherPotGreen(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5CA50
int32_t __fastcall MISSMODE_SrvHit26_GrimWardStart(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5CBE0
int32_t __fastcall MISSMODE_SrvHit27_GrimWard(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5CBF0
int32_t __fastcall MISSMODE_SrvHit28_GrimWardScare(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5CE10
int32_t __fastcall MISSMODE_SrvHit29_FrozenOrb(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5CF50
int32_t __fastcall MISSMODE_SrvHit05_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D070
int32_t __fastcall MISSMODE_SrvHit31_FireHead(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D160
int32_t __fastcall MISSMODE_SrvHit32_CairnStones(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D270
int32_t __fastcall MISSMODE_SrvHit33_TowerChestSpawner(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D290
int32_t __fastcall MISSMODE_SrvHit35_OrbMist(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D320
int32_t __fastcall MISSMODE_SrvHit36_MissileInAir(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D430
int32_t __fastcall MISSMODE_SrvHit37_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D440
void __fastcall MISSMODE_CatapultChargedBall_LightningTrailingJavelin_SubmissileInitFunc(D2UnitStrc* pMissile, int32_t nUnused);
//D2Game.0x6FC5D490
int32_t __fastcall MISSMODE_SrvHit38_CatapultChargedBall(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D620
int32_t __fastcall MISSMODE_SrvHit39_ImpSpawnMonsters(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D6A0
int32_t __fastcall MISSMODE_SrvHit40_CatapultSpikeBall(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D7C0
int32_t __fastcall MISSMODE_SrvHit43_HealingVortex(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5D950
int32_t __fastcall MISSMODE_SrvHit45_LightningTrailingJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5DA80
int32_t __fastcall MISSMODE_SrvHit47_MoltenBoulder(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5DD10
int32_t __fastcall MISSMODE_SrvHit48_MoltenBoulderEmerge(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5DE50
int32_t __fastcall MISSMODE_SrvHit50_PlagueVinesTrail(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5DED0
int32_t __fastcall MISSMODE_SrvHit51_VolcanoDebris(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5DFC0
int32_t __fastcall MISSMODE_SrvHit52_BladeFury(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E0D0
int32_t __fastcall MISSMODE_SrvHit53_RabiesContagion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E160
int32_t __fastcall MISSMODE_SrvHit54_BaalSpawnMonsters(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E220
int32_t __fastcall MISSMODE_SrvHit55_Baalnferno(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E370
int32_t __fastcall MISSMODE_SrvHit56_ArmageddonControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E530
int32_t __fastcall MISSMODE_SrvHit58_BaalTauntLightningControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E760
int32_t __fastcall MISSMODE_SrvHit59_BaalTauntPoisonControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E7F0
int32_t __fastcall MISSMODE_SrvHit23_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5E890
void __fastcall MISSMODE_SrvDmg01_FireArrow_MagicArrow_ColdArrow(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5E9D0
void __fastcall MISSMODE_SrvDmg12_LightningJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5EB20
void __fastcall MISSMODE_SrvDmg02_IceArrow_RoyalStrikeChaos(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5EC70
void __fastcall MISSMODE_SrvDmg03_Blaze_FireWall_ImmolationFire_MeteorFire(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5ECE0
void __fastcall MISSMODE_SrvDmg04_IceBlast(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5ED00
void __fastcall MISSMODE_SrvDmg10_BladesOfIceCubes(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5ED50
void __fastcall MISSMODE_SrvDmg05_BlessedHammer(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5EF40
void __fastcall MISSMODE_SrvDmg06_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5EF80
void __fastcall MISSMODE_SrvDmg07_Warcry_ShockWave(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5F060
void __fastcall MISSMODE_SrvDmg08_EruptionCrack(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5F0C0
void __fastcall MISSMODE_SrvDmg09_Twister(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5F170
void __fastcall MISSMODE_SrvDmg11_RabiesContagion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5F1F0
void __fastcall MISSMODE_SrvDmg13_BlessedHammerEx(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5F250
void __fastcall MISSMODE_SrvDmg14_MoltenBoulder(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC5F4B0
D2MonStats2Txt* __fastcall D2GAME_GetMonStats2TxtRecord_6FC5F4B0(int32_t nRecordId);
//D2Game.0x6FC5F4E0
void __fastcall MISSMODE_SetDamageFlags(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pDefender, D2DamageStrc* pDamage);
//D2Game.0x6FC5F6C0
int32_t __fastcall MISSMODE_SrvDo19_RadamentDeath(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC5F7C0
int32_t __fastcall MISSMODE_RadamentDeathAreaEffectCallback(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nSkillLevel, int32_t nUnused);
//D2Game.0x6FC5F7E0
int32_t __fastcall MISSMODE_RadamentDeathAreaEffectCallbackFirstFrame(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nSkillLevel, int32_t nUnused);
//D2Game.0x6FC5F800
int32_t __fastcall MISSMODE_SrvDo36_BaalFxControl(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC5F860
int32_t __fastcall MISSMODE_SrvHit57_BaalFxControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Game.0x6FC5F8B0
int32_t __fastcall MISSMODE_SrvDo37_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC5F8C0
int32_t __fastcall MISSMODE_SrvDo01_BasicMissile(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC5FAD0
int32_t __fastcall MISSMODE_SrvDmgHitHandler(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, int32_t a4);
//D2Game.0x6FC60080
void __fastcall MISSMODE_ToggleStateOff(D2UnitStrc* pUnit, int32_t nState, int32_t nUnused);
//D2Game.0x6FC60090
void __fastcall MISSMODE_SrvDoHandler(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nUnused);