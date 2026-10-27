#pragma once

#include <GAME/Game.h>
#include <Units/Units.h>

#include <DataTbls/MissilesTbls.h>
#include <DataTbls/MonsterTbls.h>

#include <Drlg/D2DrlgDrlg.h>

//D2Game.0x6FC6AC00
void __fastcall MONSTERUNIQUE_ToggleUnitFlag(D2UnitStrc* pUnit, int32_t nFlag, int32_t bSet);
//D2Game.0x6FC6AC30
//1.14d: 0x005A0120
uint8_t* __fastcall MONSTERUNIQUE_GetUMods(D2UnitStrc* pUnit);
//D2Game.0x6FC6AC50
//1.14d: 0x005A0140
uint16_t __fastcall MONSTERUNIQUE_GetNameSeed(D2UnitStrc* pUnit);
//D2Game.0x6FC6AC70
//1.14d: 0x005A0180
int32_t __fastcall MONSTERUNIQUE_CheckMonTypeFlag(D2UnitStrc* pUnit, uint16_t nFlag);
//D2Game.0x6FC6ACA0
void __fastcall MONSTERUNIQUE_ToggleMonTypeFlag(D2UnitStrc* pUnit, uint16_t nFlag, int32_t bSet);
//D2Game.0x6FC6ACD0
//1.14d: 0x005A01E0
int16_t __fastcall MONSTERUNIQUE_GetBossHcIdx(D2UnitStrc* pUnit);
//D2Game.0x6FC6ACF0
//1.14d: 0x005A0380
int32_t __fastcall MONSTERUNIQUE_HasUMods(D2UnitStrc* pUnit);
//D2Game.0x6FC6AD10
//1.14d: 0x005A03A0
int32_t __fastcall MONSTERUNIQUE_GetSuperUniqueBossHcIdx(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6AD50
//1.14d: 0x005A0CE0
void __fastcall MONSTERUNIQUE_UMod1_RandomName(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6AD90
//1.14d: 0x005A0DC0
void __fastcall MONSTERUNIQUE_UMod2_HealthBonus(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6AF70
int32_t __fastcall MONSTERUNIQUE_CalculatePercentage(int32_t nValue, int32_t nMultiplier, int32_t nDivisor);
//D2Game.0x6FC6AFF0
//1.14d: 0x005A0E40
void __fastcall MONSTERUNIQUE_UMod4_LevelBonus(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B030
//1.14d: 0x005A0E80
void __fastcall MONSTERUNIQUE_UMod16_Champion(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B210
//1.14d: 0x005A1080
void __fastcall MONSTERUNIQUE_UMod36_Ghostly(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B3A0
//1.14d: 0x005A11F0
void __fastcall MONSTERUNIQUE_UMod37_Fanatic(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B3E0
//1.14d: 0x005A1230
void __fastcall MONSTERUNIQUE_UMod38_Possessed(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B4B0
//1.14d: 0x005A1280
void __fastcall MONSTERUNIQUE_UMod39_Berserk(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B5D0
//1.14d: 0x005A1330
void __fastcall MONSTERUNIQUE_UMod41_AlwaysRun(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B610
//1.14d: 0x005A1370
void __fastcall MONSTERUNIQUE_UMod8_Resistant(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B8C0
//1.14d: 0x005A1600
void __fastcall MONSTERUNIQUE_UMod26_Teleport(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6B910
//1.14d: 0x005A1650
void __fastcall MONSTERUNIQUE_UMod30_AuraEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BA70
//1.14d: 0x005A17E0
void __fastcall MONSTERUNIQUE_UMod5_Strong(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BB80
//1.14d: 0x005A1910
void __fastcall MONSTERUNIQUE_UMod6_Fast(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BC10
//1.14d: 0x005A1990
void __fastcall MONSTERUNIQUE_UMod9_FireEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BDD0
//1.14d: 0x005A1B00
void __fastcall MONSTERUNIQUE_UMod17_LightningEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6BF90
//1.14d: 0x005A1C70
void __fastcall MONSTERUNIQUE_UMod18_ColdEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C160
//1.14d: 0x005A1E00
void __fastcall MONSTERUNIQUE_UMod23_PoisonEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C340
//1.14d: 0x005A1F90
void __fastcall MONSTERUNIQUE_UMod25_ManaSteal(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C4F0
//1.14d: 0x005A2530
void __fastcall MONSTERUNIQUE_CastAmplifyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnusedUMod, int32_t bUnique);
//D2Game.0x6FC6C5B0
//1.14d: 0x005A23D0
void __fastcall MONSTERUNIQUE_CurseCallback_ApplyAmplifyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel);
//D2Game.0x6FC6C710
//1.14d: 0x005A25F0
void __fastcall MONSTERUNIQUE_FireEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C740
//1.14d: 0x005A2620
void __fastcall MONSTERUNIQUE_CastCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6C9E0
//1.14d: 0x005A2840
void __fastcall MONSTERUNIQUE_CastCorpseExplode2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CAB0
//1.14d: 0x005A2910
void __fastcall MONSTERUNIQUE_CastNova(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CB40
//1.14d: 0x005A29A0
void __fastcall MONSTERUNIQUE_CastLightUniqueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CD30
//1.14d: 0x005A2BA0
void __fastcall MONSTERUNIQUE_CastLightUniqueMissileIfNotGetHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CD60
//1.14d: 0x005A2BD0
void __fastcall MONSTERUNIQUE_CastColdUniqueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CDB0
//1.14d: 0x005A2C20
void __fastcall MONSTERUNIQUE_CastCorpsePoisonCloud(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CE50
//1.14d: 0x005A2D10
void __fastcall MONSTERUNIQUE_KillMinions(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CEC0
//1.14d: 0x005A2CC0
void __fastcall MONSTERUNIQUE_MinionCallback_KillMinion(D2UnitStrc* pUnit, void* pGameArg, void* pUnused);
//D2Game.0x6FC6CF10
//1.14d: 0x005A2D80
void __fastcall MONSTERUNIQUE_AddSecondaryDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6CF90
//1.14d: 0x005A2E00
void __fastcall MONSTERUNIQUE_AddRogueHireMissileSecondaryDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D030
//1.14d: 0x005A2EB0
void __fastcall MONSTERUNIQUE_ScarabModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D060
//1.14d: 0x005A2EE0
void __fastcall MONSTERUNIQUE_CastBugLightningMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D1C0
//1.14d: 0x005A3040
void __fastcall MONSTERUNIQUE_ApplyElementalDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D410
D2MonUModTxt* __fastcall MONSTERUNIQUE_GetMonUModTxtRecord(uint32_t nUMod);
//D2Game.0x6FC6D440
//1.14d: 0x005A30B0
void __fastcall MONSTERUNIQUE_ApplyMissileElementalDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D690
//1.14d: 0x005A30E0
void __fastcall MONSTERUNIQUE_StealBeltItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D800
//1.14d: 0x005A3250
void __fastcall MONSTERUNIQUE_QuestCompleteModeChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6D8B0
//1.14d: 0x005A3490
void __fastcall MONSTERUNIQUE_CastQueenPoisonCloudMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DA40
//1.14d: 0x005A3610
void __fastcall MONSTERUNIQUE_CreateMultishotMissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DCB0
//1.14d: 0x005A37D0
void __fastcall MONSTERUNIQUE_LightningEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DCE0
//1.14d: 0x005A3800
void __fastcall MONSTERUNIQUE_ColdEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DD20
//1.14d: 0x005A3840
void __fastcall MONSTERUNIQUE_ScheduleSelfResurrection(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DDE0
//1.14d: 0x005A3910
void __fastcall MONSTERUNIQUE_TrySelfResurrection(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DFA0
//1.14d: 0x005A3A80
void __fastcall MONSTERUNIQUE_ApplyShatterState(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6DFC0
//1.14d: 0x005A3AA0
void __fastcall MONSTERUNIQUE_KillUnitOrRemovePet(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E070
//1.14d: 0x005A3B50
void __fastcall MONSTERUNIQUE_SetTrapDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E240
//1.14d: 0x005A3D20
void __fastcall MONSTERUNIQUE_ApplyCorpseFireDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E390
//1.14d: 0x005A3E70
void __fastcall MONSTERUNIQUE_SuicideModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E410
//1.14d: 0x005A3EF0
void __fastcall MONSTERUNIQUE_CastSuicideExplodeMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E700
//1.14d: 0x005A4200
void __fastcall MONSTERUNIQUE_CreatePainWorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E730
//1.14d: 0x005A4230
void __fastcall MONSTERUNIQUE_UpdateAiAndScheduleUModEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E770
//1.14d: 0x005A4350
void __fastcall MONSTERUNIQUE_ExecuteUModModeStartCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E780
//1.14d: 0x005A4270
void __fastcall MONSTERUNIQUE_ExecuteUModCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMissile, int32_t nIndex);
//D2Game.0x6FC6E860
//1.14d: 0x005A4360
void __fastcall MONSTERUNIQUE_ExecuteUModModeChangeCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E870
//1.14d: 0x005A4370
void __fastcall MONSTERUNIQUE_EventExecuteUModCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6E890
//1.14d: 0x005A4390
void __fastcall MONSTERUNIQUE_ExecuteUModSuccessfulHitCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E8A0
//1.14d: 0x005A43A0
void __fastcall MONSTERUNIQUE_ExecuteUModDamagedCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC6E8B0
//1.14d: 0x005A43B0
void __fastcall MONSTERUNIQUE_ExecuteUModMissileCallbacks(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMissile);
//D2Game.0x6FC6E8D0
//1.14d: 0x005A43E0
D2UnitStrc* __fastcall MONSTERUNIQUE_SpawnUniqueGroup(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nSuperUniqueId, int32_t bAllowChampion, uint16_t nX, uint16_t nY, int32_t bAvoidWarps);
//D2Game.0x6FC6E940
//1.14d: 0x005A0760
void __fastcall MONSTERUNIQUE_AssignRandomUMods(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bAllowChampion);
//D2Game.0x6FC6EBE0
D2MonsterDataStrc* __fastcall MONSTERUNIQUE_GetMonsterData(D2UnitStrc* pUnit);
//D2Game.0x6FC6EBF0
//1.14d: 0x005A0260
int32_t __fastcall MONSTERUNIQUE_GetUModCount(uint8_t* pUMods);
//D2Game.0x6FC6EC10
//1.14d: 0x005A03E0
BOOL __fastcall MONSTERUNIQUE_CanAssignUMod(D2UnitStrc* pUnit, D2MonUModTxt* pMonUModTxtRecord, int32_t bExpansion);
//D2Game.0x6FC6EE90
//1.14d: 0x005A0600
int32_t __fastcall MONSTERUNIQUE_RollNonChampionUMod(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pUsedUMods);
//D2Game.0x6FC6F160
//1.14d: 0x0046C140
uint32_t __fastcall MONSTERUNIQUE_CheckMonModeFlag(int32_t nMonsterId, int32_t nFlag);
//D2Game.0x6FC6F1D0
//1.14d: 0x00457490
uint32_t __fastcall MONSTERUNIQUE_CheckMonStatsFlag(int32_t nMonsterId, int32_t nFlag);
//D2Game.0x6FC6F220
//1.14d: 0x005A09E0
D2UnitStrc* __fastcall MONSTERUNIQUE_SpawnUniqueMonster(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nX, int32_t nY, int32_t nUnitGUID, int32_t nClassId, int32_t bAvoidWarps);
//D2Game.0x6FC6F440
//1.14d: 0x005A2120
void __fastcall MONSTERUNIQUE_SpawnMinions(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, D2UnitStrc* pUnit, int32_t bSpawnMinions, int32_t nMinGroup, int32_t nMaxGroup);
//D2Game.0x6FC6F670
//1.14d: 0x005A2100
void __fastcall MONSTERUNIQUE_InitializeUMod(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6F690
//1.14d: 0x005A49B0
D2UnitStrc* __fastcall D2GAME_SpawnSuperUnique_6FC6F690(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nSuperUnique);
//D2Game.0x6FC6FBA0
//1.14d: 0x005A4440
D2UnitStrc* __fastcall MONSTERUNIQUE_RestoreUniqueMonster(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nUnitGUID, uint16_t nNameSeed, int32_t bChampion, int32_t bSuperUnique, int16_t nBossHcIdx, uint8_t* pUMods);
//D2Game.0x6FC6FDC0
//1.14d: 0x005A46E0
D2UnitStrc* __fastcall MONSTERUNIQUE_RestoreMinion(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nUnitGUID, uint8_t* pUMods);
//D2Game.0x6FC6FF10
//1.14d: 0x005A4850
void __fastcall D2GAME_BOSSES_AssignUMod_6FC6FF10(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique);
//D2Game.0x6FC6FFD0
//1.14d: 0x005A48C0
void __fastcall MONSTERUNIQUE_PromoteToChampion(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nUMod);
