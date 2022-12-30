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
int32_t __fastcall SKILLITEM_pSpell01_Initializer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD02BF0
int32_t __fastcall SKILLITEM_pSpell01_IdentifyItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD02C80
int32_t __fastcall SKILLITEM_pSpell02_CastPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD02E10
int32_t __fastcall SKILLITEM_pSpell03_Potion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD032D0
int32_t __fastcall SKILLITEM_pSpell04_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03610
int32_t __fastcall SKILLITEM_pSpell05_RejuvPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD039A0
int32_t __fastcall SKILLITEM_pSpell09_StaminaPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03BB0
int32_t __fastcall SKILLITEM_pSpell09_AntidoteThawingPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03C80
int32_t __fastcall SKILLITEM_pSpell08_ExperienceElixir(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03CD0
int32_t __fastcall SKILLITEM_pSpell07_OpenCube(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03D80
int32_t __fastcall SKILLITEM_pSpell10_CastFireBallOnTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03DF0
int32_t __fastcall SKILLITEM_pSpell11_CastFireBallToCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId);
//D2Game.0x6FD03E40
int32_t __fastcall SKILLITEM_pSpell_Handler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY);
//D2Game.0x6FD040B0
int32_t __fastcall SKILLS_SrvDo113_Scroll_Book(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04270
void __fastcall SKILLITEM_ActivateAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a5, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04340
void __fastcall SKILLITEM_DeactivateAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a4, int32_t nSkillId);
//D2Game.0x6FD043F0
int32_t __fastcall SKILLITEM_EventFunc06_AttackerTakesDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD044B0
int32_t __fastcall SKILLITEM_EventFunc10_AttackerTakesLightDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04570
int32_t __fastcall SKILLITEM_EventFunc11_ApplyFireDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04630
int32_t __fastcall SKILLITEM_EventFunc12_ApplyColdDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04720
int32_t __fastcall SKILLITEM_EventFunc07_Knockback(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04820
int32_t __fastcall SKILLITEM_EventFunc08_Howl(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD048B0
int32_t __fastcall SKILLITEM_EventFunc09_Stupidity(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD049D0
int32_t __fastcall SKILLITEM_EventFunc13_DamageToMana(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04B10
int32_t __fastcall SKILLITEM_EventFunc14_Freeze(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04C40
int32_t __fastcall SKILLITEM_CalculateOpenWoundsHpRegen(int32_t nLevel, const int32_t* pValues);
//D2Game.0x6FD04CF0
int32_t __fastcall SKILLITEM_EventFunc15_OpenWounds(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD04E50
int32_t __fastcall SKILLITEM_EventFunc16_CrushingBlow(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD050D0
int32_t __fastcall SKILLITEM_EventFunc17_ManaAfterKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05160
int32_t __fastcall SKILLITEM_EventFunc28_HealAfterKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD051E0
int32_t __fastcall SKILLITEM_EventFunc18_HealAfterDemonKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05270
int32_t __fastcall SKILLITEM_EventFunc19_Slow(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD053D0
int32_t __fastcall SKILLITEM_EventFunc20_SkillOnAttackHitKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05520
int32_t __fastcall SKILLITEM_EventFunc21_SkillOnGetHit(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05640
int32_t __fastcall SKILLITEM_EventFunc30_SkillOnDeathLevelup(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05750
int32_t __fastcall SKILLITEM_EventFunc29_RestInPeace(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05780
int32_t __fastcall SKILLITEM_TimerCallback_ReanimateMonster(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pSource, D2UnitStrc* pTarget, D2DamageStrc* pDamage, int32_t nMonId, int32_t nOwnerId);
//D2Game.0x6FD05B60
int32_t __fastcall SKILLITEM_EventFunc31_Reanimate(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05C20
int32_t __fastcall SKILLITEM_FindTargetPosition(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD05D70
int32_t __fastcall SKILLITEM_HandleItemEffectSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pTargetUnit, int32_t nX, int32_t nY, int32_t* pUnitType, int32_t* pUnitGUID, int32_t* pX, int32_t* pY, int32_t a7);
//D2Game.0x6FD06020
int32_t __fastcall SKILLITEM_CastSkillOnTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pTargetUnit, int32_t a5);
//D2Game.0x6FD060F0
int32_t __fastcall SKILLITEM_CastSkillOnPosition(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t a0);
