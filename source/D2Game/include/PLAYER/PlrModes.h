#pragma once

#include <Units/Units.h>
#include <GAME/Event.h>

#pragma pack(push, 1)
struct D2UnkPlrModeStrc2
{
    int32_t nRightSkillId;
    int32_t nLeftSkillId;
    int32_t nRightSkillOwnerGUID;
    int32_t nLeftSkillOwnerGUID;
};
#pragma pack(pop)


//D2Game.0x6FC7F340
int32_t __fastcall D2GAME_PLRMODES_First_6FC7F340(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode, int32_t a4, D2SkillStrc* pSkill);
//D2Game.0x6FC7F550
void __fastcall PLRMODE_StartXY_Neutral(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY);
//D2Game.0x6FC7F5A0
void __fastcall PLRMODE_StartID_WalkRunKnockback(D2GameStrc* pGame, D2UnitStrc* a2, int32_t a3, D2UnitStrc* pTargetUnit);
//D2Game.0x6FC7F600
void __fastcall sub_6FC7F600(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode);
//D2Game.0x6FC7F730
void __fastcall PLRMODE_StartXY_WalkRunKnockback(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY);
//D2Game.0x6FC7F780
int32_t __fastcall sub_6FC7F780(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t a4);
//D2Game.0x6FC7FB70
void __fastcall sub_6FC7FB70(D2UnitStrc* pPlayer, int32_t a2);
//D2Game.0x6FC7FB90
int32_t __fastcall sub_6FC7FB90(D2UnitStrc* pPlayer);
//D2Game.0x6FC7FBB0
int32_t __fastcall sub_6FC7FBB0(D2UnitStrc* pPlayer);
//1.10f: D2Game.0x6FC7FBD0
//1.13c: D2Game.0x6FC99210
D2UnitStrc* __fastcall D2GAME_CORPSE_Handler_6FC7FBD0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC802F0
void __fastcall sub_6FC802F0(D2GameStrc* pGame, int32_t nBodyloc, D2UnitStrc* pItem, D2UnitStrc* pUnit);
//D2Game.0x6FC803F0
void __fastcall sub_6FC803F0(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Game.0x6FC80440
void __fastcall sub_6FC80440(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pDeadBody);
//D2Game.0x6FC805B0
void __fastcall PLRMODE_StartID_Death(D2GameStrc* pGame, D2UnitStrc* pDefender, int32_t a3, D2UnitStrc* pAttacker);
//D2Game.0x6FC80710
void __fastcall PLRMODE_StartXY_Dead(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4, int32_t a5);
//D2Game.0x6FC80870
void __fastcall PLRMODE_StartXY_Block(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY);
//D2Game.0x6FC808D0
int32_t __fastcall sub_6FC808D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC808E0
void __fastcall PLRMODE_StartXY_GetHit(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY);
//D2Game.0x6FC80940
void __fastcall PLRMODE_StartXY_AttackCastThrowKickSpecialSequence(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY);
//D2Game.0x6FC809B0
void __fastcall PLRMODE_StartID_AttackCastThrowKickSpecialSequence(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, D2UnitStrc* pTarget);
//D2Game.0x6FC80A30
void __fastcall sub_6FC80A30(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC80B90
void __fastcall sub_6FC80B90(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pWeapon);
//D2Game.0x6FC80E10
void __fastcall sub_6FC80E10(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC80EE0
int32_t __fastcall sub_6FC80EE0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC80F80
void __fastcall D2GAME_EVENTS_StatRegen_6FC80F80(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4);
//D2Game.0x6FC81250
void __fastcall sub_6FC81250(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4);
//D2Game.0x6FC814F0
void __fastcall sub_6FC814F0(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC81560
void __fastcall sub_6FC81560(D2UnitStrc* pUnit, D2UnkPlrModeStrc2* a2);
//D2Game.0x6FC81600
void __fastcall sub_6FC81600(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC81650
void __fastcall sub_6FC81650(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2ClientStrc* pClient, int32_t a4);
//D2Game.0x6FC817D0
void __fastcall sub_6FC817D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pUsedSkill, int32_t nMode, int32_t nX, int32_t nY, int32_t a7);
//D2Game.0x6FC81890
int32_t __fastcall sub_6FC81890(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode);
//D2Game.0x6FC81A00
void __fastcall D2GAME_PLAYERMODE_Change_6FC81A00(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2SkillStrc* pSkill, BYTE nMode, int32_t nUnitType, int32_t nTargetGUID, int32_t bAllowReEnter);
//D2Game.0x6FC81B20
void __fastcall sub_6FC81B20(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4);
//D2Game.0x6FC81B90
void __fastcall sub_6FC81B90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4);
//D2Game.0x6FC81BD0
void __fastcall D2GAME_EVENTS_Callback_6FC81BD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEvent, int32_t dwArg, int32_t dwArgEx);
