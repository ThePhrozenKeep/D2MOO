#pragma once

#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>

#pragma pack(1)

typedef BOOL(__fastcall* SPELLPREPARE)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, D2UnitStrc*, int32_t, int32_t, int32_t);
typedef BOOL(__fastcall* SPELLDO)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, D2UnitStrc*, int32_t, int32_t, int32_t);

struct D2pSpellTblStrc
{
	SPELLPREPARE pfSpellPrepare;			//0x00
	SPELLDO pfSpellDo;						//0x04
};

#pragma pack()

//D2Game.0x6FD02BA0
//1.14d: 0x005BE130
int32_t __fastcall SKILLITEM_pSpell01_Initializer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD02BF0
//1.14d: 0x005BE230
int32_t __fastcall SKILLITEM_pSpell01_IdentifyItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD02C80
//1.14d: 0x005BE290
int32_t __fastcall SKILLITEM_pSpell02_CastPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD02E10
//1.14d: 0x005BE3F0
int32_t __fastcall SKILLITEM_pSpell03_Potion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD032D0
//1.14d: 0x005BE7B0
int32_t __fastcall SKILLITEM_pSpell04_ApplyPotionStats(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03610
//1.14d: 0x005BEAC0
int32_t __fastcall SKILLITEM_pSpell05_RejuvPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD039A0
//1.14d: 0x005BEDA0
int32_t __fastcall SKILLITEM_pSpell09_StaminaPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03BB0
//1.14d: 0x005BEF90
int32_t __fastcall SKILLITEM_pSpell06_AntidoteThawingPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03C80
//1.14d: 0x005BF060
int32_t __fastcall SKILLITEM_pSpell08_AddItemValueToStat(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03CD0
//1.14d: 0x005BF0C0
int32_t __fastcall SKILLITEM_pSpell07_OpenCube(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03D80
//1.14d: 0x005BF170
int32_t __fastcall SKILLITEM_pSpell10_CastFireBallOnTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03DF0
//1.14d: 0x005BF1F0
int32_t __fastcall SKILLITEM_pSpell11_CastFireBallToCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03E40
//1.14d: 0x005BF240
int32_t __fastcall SKILLITEM_pSpell_Handler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY);
//D2Game.0x6FD040B0
//1.14d: 0x005BF3D0
int32_t __fastcall SKILLS_SrvDo113_Scroll_Book(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04270
//1.14d: 0x005BF510
void __fastcall SKILLITEM_ActivateAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventKey, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04340
//1.14d: 0x005BF5D0
void __fastcall SKILLITEM_DeactivateAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventKey, int32_t nSkillId);
//D2Game.0x6FD043F0
//1.14d: 0x005BF670
int32_t __fastcall SKILLITEM_EventFunc06_AttackerTakesDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD044B0
//1.14d: 0x005BF720
int32_t __fastcall SKILLITEM_EventFunc10_AttackerTakesLightDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04570
//1.14d: 0x005BF7D0
int32_t __fastcall SKILLITEM_EventFunc11_ApplyFireDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04630
//1.14d: 0x005BF880
int32_t __fastcall SKILLITEM_EventFunc12_ApplyColdDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04720
//1.14d: 0x005BF960
int32_t __fastcall SKILLITEM_EventFunc07_Knockback(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04820
//1.14d: 0x005BFA10
int32_t __fastcall SKILLITEM_EventFunc08_Howl(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD048B0
//1.14d: 0x005BFAA0
int32_t __fastcall SKILLITEM_EventFunc09_Stupidity(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD049D0
//1.14d: 0x005BFBB0
int32_t __fastcall SKILLITEM_EventFunc13_DamageToMana(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04B10
//1.14d: 0x005BFC60
int32_t __fastcall SKILLITEM_EventFunc14_Freeze(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04C40
//1.14d: 0x005BFDA0
int32_t __fastcall SKILLITEM_CalculateOpenWoundsHpRegen(int32_t nLevel, const int32_t* pValues);
//D2Game.0x6FD04CF0
//1.14d: 0x005BFE60
int32_t __fastcall SKILLITEM_EventFunc15_OpenWounds(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04E50
//1.14d: 0x005BFFC0
int32_t __fastcall SKILLITEM_EventFunc16_CrushingBlow(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD050D0
//1.14d: 0x005C0130
int32_t __fastcall SKILLITEM_EventFunc17_ManaAfterKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05160
//1.14d: 0x005C01D0
int32_t __fastcall SKILLITEM_EventFunc28_HealAfterKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD051E0
//1.14d: 0x005C0260
int32_t __fastcall SKILLITEM_EventFunc18_HealAfterDemonKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05270
//1.14d: 0x005C0310
int32_t __fastcall SKILLITEM_EventFunc19_Slow(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD053D0
//1.14d: 0x005C0470
int32_t __fastcall SKILLITEM_EventFunc20_SkillOnAttackHitKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05520
//1.14d: 0x005C05A0
int32_t __fastcall SKILLITEM_EventFunc21_SkillOnGetHit(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05640
//1.14d: 0x005C0670
int32_t __fastcall SKILLITEM_EventFunc30_SkillOnDeathLevelup(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05750
//1.14d: 0x005C0770
int32_t __fastcall SKILLITEM_EventFunc29_RestInPeace(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05780
//1.14d: 0x005C07A0
int32_t __fastcall SKILLITEM_TimerCallback_ReanimateMonster(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pSource, D2UnitStrc* pTarget, D2DamageStrc* pDamage, int32_t nMonId, int32_t nOwnerId);
//D2Game.0x6FD05B60
//1.14d: 0x005C09E0
int32_t __fastcall SKILLITEM_EventFunc31_Reanimate(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05C20
//1.14d: 0x005FD850
int32_t __fastcall SKILLITEM_FindTargetPosition(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05D70
//1.14d: 0x005FDA80
int32_t __fastcall SKILLITEM_HandleItemEffectSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pTargetUnit, int32_t nX, int32_t nY, int32_t* pUnitType, int32_t* pUnitGUID, int32_t* pX, int32_t* pY, int32_t nItemEffectParam);
//D2Game.0x6FD06020
//1.14d: 0x005FDCA0
int32_t __fastcall SKILLITEM_CastSkillOnTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pTargetUnit, int32_t nItemEffectParam);
//D2Game.0x6FD060F0
//1.14d: 0x005FDD60
int32_t __fastcall SKILLITEM_CastSkillOnPosition(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nItemEffectParam);
