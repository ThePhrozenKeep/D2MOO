#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include <DataTbls/MissilesTbls.h>
#include <DataTbls/MonsterTbls.h>

#include <Drlg/D2DrlgDrlg.h>

//D2Game.0x6FC6AC00
void __fastcall MONSTERUNIQUE_ToggleUnitFlag(D2UnitStrc* pUnit, int32_t nFlag, int32_t bSet);
//D2Game.0x6FC6AC30
uint8_t* __fastcall MONSTERUNIQUE_GetUMods(D2UnitStrc* pUnit);
//D2Game.0x6FC6AC50
uint16_t __fastcall MONSTERUNIQUE_GetNameSeed(D2UnitStrc* pUnit);
//D2Game.0x6FC6AC70
int32_t __fastcall MONSTERUNIQUE_CheckMonTypeFlag(D2UnitStrc* pUnit, uint16_t nFlag);
//D2Game.0x6FC6ACA0
void __fastcall MONSTERUNIQUE_ToggleMonTypeFlag(D2UnitStrc* pUnit, uint16_t nFlag, int32_t bSet);
//D2Game.0x6FC6ACD0
int16_t __fastcall MONSTERUNIQUE_GetBossHcIdx(D2UnitStrc* pUnit);
//D2Game.0x6FC6ACF0
int32_t __fastcall MONSTERUNIQUE_HasUMods(D2UnitStrc* pUnit);
//D2Game.0x6FC6AD10
int32_t __fastcall MONSTERUNIQUE_GetSuperUniqueBossHcIdx(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6AD50
void __fastcall MONSTERUNIQUE_UMod1_RandomName(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6AD90
void __fastcall MONSTERUNIQUE_UMod2_HealthBonus(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6AF70
int32_t __fastcall MONSTERUNIQUE_CalculatePercentage(int32_t a1, int32_t a2, int32_t a3);
//D2Game.0x6FC6AFF0
void __fastcall MONSTERUNIQUE_UMod4_LevelBonus(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B030
void __fastcall MONSTERUNIQUE_UMod16_Champion(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B210
void __fastcall MONSTERUNIQUE_UMod36_Ghostly(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B3A0
void __fastcall MONSTERUNIQUE_UMod37_Fanatic(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B3E0
void __fastcall MONSTERUNIQUE_UMod38_Possessed(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B4B0
void __fastcall MONSTERUNIQUE_UMod39_Berserk(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B5D0
void __fastcall MONSTERUNIQUE_UMod41_AlwaysRun(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B610
void __fastcall MONSTERUNIQUE_UMod8_Resistant(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B8C0
void __fastcall MONSTERUNIQUE_UMod26_Teleport(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B910
void __fastcall MONSTERUNIQUE_UMod30_AuraEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BA70
void __fastcall MONSTERUNIQUE_UMod5_Strong(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BB80
void __fastcall MONSTERUNIQUE_UMod6_Fast(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BC10
void __fastcall MONSTERUNIQUE_UMod9_FireEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BDD0
void __fastcall MONSTERUNIQUE_UMod17_LightningEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BF90
void __fastcall MONSTERUNIQUE_UMod18_ColdEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C160
void __fastcall MONSTERUNIQUE_UMod23_PoisonEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C340
void __fastcall MONSTERUNIQUE_UMod25_ManaSteal(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C4F0
void __fastcall MONSTERUNIQUE_CastAmplifyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4);
//D2Game.0x6FC6C5B0
void __fastcall MONSTERUNIQUE_CurseCallback_ApplyAmplifyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel);
//D2Game.0x6FC6C710
void __fastcall MONSTERUNIQUE_FireEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C740
void __fastcall MONSTERUNIQUE_CastCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C9E0
void __fastcall MONSTERUNIQUE_CastCorpseExplode2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CAB0
void __fastcall MONSTERUNIQUE_CastNova(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CB40
void __fastcall MONSTERUNIQUE_CastLightUniqueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CD30
void __fastcall sub_6FC6CD30(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CD60
void __fastcall MONSTERUNIQUE_CastColdUniqueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CDB0
void __fastcall MONSTERUNIQUE_CastCorpsePoisonCloud(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CE50
void __fastcall MONSTERUNIQUE_KillMinions(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CEC0
void __fastcall MONSTERUNIQUE_MinionCallback_KillMinion(D2UnitStrc* pUnit, void* pGameArg, void* pUnused);
//D2Game.0x6FC6CF10
void __fastcall sub_6FC6CF10(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CF90
void __fastcall sub_6FC6CF90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D030
void __fastcall MONSTERUNIQUE_ScarabModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D060
void __fastcall MONSTERUNIQUE_CastBugLightningMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D1C0
void __fastcall MONSTERUNIQUE_ApplyElementalDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D410
D2MonUModTxt* __fastcall MONSTERUNIQUE_GetMonUModTxtRecord(uint32_t nUMod);
//D2Game.0x6FC6D440
void __fastcall sub_6FC6D440(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D690
void __fastcall MONSTERUNIQUE_StealBeltItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D800
void __fastcall MONSTERUNIQUE_QuestCompleteModeChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D8B0
void __fastcall MONSTERUNIQUE_CastQueenPoisonCloudMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DA40
void __fastcall sub_6FC6DA40(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DCB0
void __fastcall MONSTERUNIQUE_LightningEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DCE0
void __fastcall MONSTERUNIQUE_ColdEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DD20
void __fastcall sub_6FC6DD20(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DDE0
void __fastcall sub_6FC6DDE0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DFA0
void __fastcall MONSTERUNIQUE_ApplyShatterState(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DFC0
void __fastcall sub_6FC6DFC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E070
void __fastcall MONSTERUNIQUE_SetTrapDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E240
void __fastcall sub_6FC6E240(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E390
void __fastcall MONSTERUNIQUE_SuicideModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E410
void __fastcall MONSTERUNIQUE_CastSuicideExplodeMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E700
void __fastcall MONSTERUNIQUE_CreatePainWorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E730
void __fastcall sub_6FC6E730(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E770
void __fastcall sub_6FC6E770(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E780
void __fastcall sub_6FC6E780(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMissile, int32_t nIndex);
//D2Game.0x6FC6E860
void __fastcall sub_6FC6E860(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E870
void __fastcall D2GAME_MONSTERS_AiFunction08_6FC6E870(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E890
void __fastcall sub_6FC6E890(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E8A0
void __fastcall sub_6FC6E8A0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E8B0
void __fastcall sub_6FC6E8B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMissile);
//D2Game.0x6FC6E8D0
D2UnitStrc* __fastcall sub_6FC6E8D0(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nSuperUniqueId, int32_t a5, uint16_t nX, uint16_t nY, int32_t a8);
//D2Game.0x6FC6E940
void __fastcall sub_6FC6E940(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3);
//D2Game.0x6FC6EBE0
D2MonsterDataStrc* __fastcall MONSTERUNIQUE_GetMonsterData(D2UnitStrc* pUnit);
//D2Game.0x6FC6EBF0
int32_t __fastcall MONSTERUNIQUE_GetUModCount(uint8_t* pUMods);
//D2Game.0x6FC6EC10
int32_t __fastcall sub_6FC6EC10(D2UnitStrc* pUnit, D2MonUModTxt* pMonUModTxtRecord, int32_t bExpansion);
//D2Game.0x6FC6EE90
int32_t __fastcall sub_6FC6EE90(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* a3);
//D2Game.0x6FC6F160
uint32_t __fastcall MONSTERUNIQUE_CheckMonModeFlag(int32_t nMonsterId, int32_t nFlag);
//D2Game.0x6FC6F1D0
uint32_t __fastcall MONSTERUNIQUE_CheckMonStatsFlag(int32_t nMonsterId, int32_t nFlag);
//D2Game.0x6FC6F220
D2UnitStrc* __fastcall D2GAME_SpawnMonster_6FC6F220(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nX, int32_t nY, int32_t nUnitGUID, int32_t nClassId, int32_t a8);
//D2Game.0x6FC6F440
void __fastcall D2GAME_SpawnMinions_6FC6F440(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, D2UnitStrc* pUnit, int32_t bSpawnMinions, int32_t nMinGroup, int32_t nMaxGroup);
//D2Game.0x6FC6F670
void __fastcall sub_6FC6F670(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6F690
D2UnitStrc* __fastcall D2GAME_SpawnSuperUnique_6FC6F690(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nSuperUnique);
//D2Game.0x6FC6FBA0
D2UnitStrc* __fastcall sub_6FC6FBA0(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nUnitGUID, uint16_t nNameSeed, int32_t bChampion, int32_t bSuperUnique, int16_t nBossHcIdx, uint8_t* pUMods);
//D2Game.0x6FC6FDC0
D2UnitStrc* __fastcall sub_6FC6FDC0(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nUnitGUID, uint8_t* pUMods);
//D2Game.0x6FC6FF10
void __fastcall D2GAME_BOSSES_AssignUMod_6FC6FF10(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6FFD0
void __fastcall D2GAME_MONSTERS_Unk_6FC6FFD0(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nUMod);
