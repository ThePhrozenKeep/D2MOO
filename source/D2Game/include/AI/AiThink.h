#pragma once

#include "AiGeneral.h"
#include <Units/Units.h>
#include <Units/UnitFinds.h>


//1.10f: D2Game.0x6FC35710
//1.14d: 0x005B0CC0
void __fastcall AITHINK_Fn000(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1660
//1.14d: 0x005B14E0
void __fastcall D2GAME_AI_SpecialState02_6FCD1660(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1750
//1.14d: 0x005EFCF0
void __fastcall AITHINK_Fn002_Skeleton(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1880
//1.14d: 0x005EFE20
void __fastcall AITHINK_Fn003_Zombie(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1990
//1.14d: 0x005EFF50
void __fastcall AITHINK_Fn004_Bighead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1BA0
//1.14d: 0x005F00E0
void __fastcall AITHINK_Fn005_BloodHawk(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1D50
//1.14d: 0x005F02C0
void __fastcall AITHINK_Fn006_Fallen(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2220
//1.14d: 0x005EFB80
void __fastcall AITHINK_Fn007_Brute(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2370
//1.14d: 0x005F0700
void __fastcall AITHINK_Fn008_SandRaider(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2680
//1.14d: 0x005F06B0
D2UnitStrc* __fastcall AITHINK_TargetCallback_SandRaider(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD27A0
//1.14d: 0x005F0A20
void __fastcall AITHINK_Fn009_Wraith(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2850
//1.14d: 0x005F0B00
void __fastcall AITHINK_Fn010_CorruptRogue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2A00
//1.14d: 0x005F0CD0
void __fastcall AITHINK_Fn011_Baboon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2E80
//1.14d: 0x005F1140
void __fastcall AITHINK_Fn014_QuillRat(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2FF0
//1.14d: 0x005F1440
void __fastcall AITHINK_Fn013_FallenShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD32E0
//1.14d: 0x005F1380
D2UnitStrc* __fastcall AITHINK_TargetCallback_FallenShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD34A0
//1.14d: 0x005F1750
void __fastcall AITHINK_SpecialFn015_SandMaggot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3540
//1.14d: 0x005F1800
void __fastcall AITHINK_Fn015_SandMaggot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3900
//1.14d: 0x005F1B60
void __fastcall AITHINK_Fn016_ClawViper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3B90
//1.14d: 0x005F1DE0
void __fastcall AITHINK_Fn142_ClawViperEx(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3E70
//1.14d: 0x005F20A0
void __fastcall AITHINK_Fn017_SandLeaper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4050
//1.14d: 0x005F22B0
void __fastcall AITHINK_Fn018_PantherWoman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD41F0
//1.14d: 0x005B0D00
D2UnitStrc* __fastcall AITHINK_TargetCallback_Panther(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD4390
//1.14d: 0x005F12A0
void __fastcall AITHINK_Fn012_019_Goatman_Swarm(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4440
//1.14d: 0x005F2540
void __fastcall AITHINK_Fn020_Scarab(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4720
//1.14d: 0x005F2850
void __fastcall AITHINK_Fn021_Mummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD48B0
//1.14d: 0x005F2B10
void __fastcall AITHINK_Fn022_GreaterMummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4C10
//1.14d: 0x005F2A00
D2UnitStrc* __fastcall AITHINK_TargetCallback_GreaterMummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD4DD0
//1.14d: 0x005F3170
void __fastcall AITHINK_Fn023_Vulture(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD55D0
//1.14d: 0x005F2FC0
int32_t __fastcall AITHINK_TryRestoreGroundCollision(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCD5710
//1.14d: 0x005F30F0
D2UnitStrc* __fastcall AITHINK_TargetCallback_Vulture(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD5850
//1.14d: 0x005F3730
void __fastcall AITHINK_Fn024_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD5A60) --------------------------------------------------------
//1.14d: 0x005F39B0
void __fastcall AITHINK_Fn025_Willowisp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD6960
//1.14d: 0x005F3960
int32_t __fastcall AITHINK_UnitFindCallback_Willowisp(D2UnitStrc* pUnit, D2UnitFindArgStrc* pUnitFindArg);
//D2Game.0x6FCD69E0
//1.14d: 0x005F4510
void __fastcall AITHINK_Fn026_Arach(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD6D60
//1.14d: 0x005F4850
void __fastcall AITHINK_Fn027_ThornHulk(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD6FD0
//1.14d: 0x005F4A70
void __fastcall AITHINK_Fn028_Vampire(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD76F0
//1.14d: 0x005F4FD0
void __fastcall AITHINK_BatDemon_HandleSpecialAiState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD7760
//1.14d: 0x005F5040
void __fastcall AITHINK_Fn029_BatDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD7BA0
//1.14d: 0x005F53E0
void __fastcall AITHINK_Fn030_Fetish(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD7EB0
//1.14d: 0x005F56D0
void __fastcall AITHINK_Fn033_HellMeteor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8090
//1.14d: 0x005F5830
void __fastcall AITHINK_Fn034_Andariel(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8260
//1.14d: 0x005F5A20
void __fastcall AITHINK_Fn035_CorruptArcher(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD85B0
//1.14d: 0x005F5D50
void __fastcall AITHINK_Fn036_CorruptLancer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD88C0
//1.14d: 0x005F6070
void __fastcall AITHINK_Fn037_SkeletonBow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8A60
//1.14d: 0x005F6220
void __fastcall AITHINK_Fn038_MaggotLarva(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8B60
//1.14d: 0x005F6340
void __fastcall AITHINK_Fn039_PinHead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8D20
//1.14d: 0x005F6530
void __fastcall AITHINK_Fn040_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8E10
//1.14d: 0x005F6630
void __fastcall AITHINK_InitializeSpawnTimerAndCount(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8E30
//1.14d: 0x005F6650
void __fastcall AITHINK_Fn043_FoulCrowNest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8FE0
//1.14d: 0x005F67B0
void __fastcall AITHINK_Fn044_Duriel(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD91F0
//1.14d: 0x005F6A10
void __fastcall AITHINK_Fn045_Sarcophagus(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD93A0
//1.14d: 0x005F6B70
void __fastcall AITHINK_Fn046_ElementalBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD94D0
//1.14d: 0x005F6CA0
void __fastcall AITHINK_Fn047_FlyingScimitar(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD9640
//1.14d: 0x005F6E60
void __fastcall AITHINK_Fn048_ZakarumZealot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD9A10
//1.14d: 0x005F72D0
void __fastcall AITHINK_Fn049_ZakarumPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD9F10
//1.14d: 0x005F7240
D2UnitStrc* __fastcall AITHINK_TargetCallback_ZakarumPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDA0C0
//1.14d: 0x005F77C0
D2UnitStrc* __fastcall AITHINK_FindTargetForMephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MonStatsTxt* pMonStatsTxtRecord, D2UnitStrc* pFallbackTarget);
//D2Game.0x6FCDA190
//1.14d: 0x005F7700
D2UnitStrc* __fastcall AITHINK_TargetCallback_Mephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDA300
//1.14d: 0x005F78B0
void __fastcall AITHINK_Fn050_Mephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDA910
//1.14d: 0x005F81D0
void __fastcall AITHINK_SpecialFn052_FrogDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDAAA0
//1.14d: 0x005F8260
void __fastcall AITHINK_Fn052_FrogDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDAFC0
//1.14d: 0x005F85C0
void __fastcall AITHINK_Fn053_Summoner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDB3E0
//1.14d: 0x005F89B0
void __fastcall AITHINK_Fn055_Izual(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDB720
//1.14d: 0x005F8F80
void __fastcall AITHINK_Fn056_Tentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDBAA0
//1.14d: 0x005F9270
void __fastcall AITHINK_Fn057_TentacleHead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDBCE0
//1.14d: 0x005F9490
void __fastcall AITHINK_Fn063_GargoyleTrap(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDBF20
//1.14d: 0x005F96C0
void __fastcall AITHINK_Fn064_SkeletonMage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC170
//1.14d: 0x005E5280
void __fastcall AITHINK_ClearInfernoAndResetAi(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC1C0
//1.14d: 0x005F9A80
void __fastcall AITHINK_Fn065_FetishShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC420
//1.14d: 0x005F99A0
D2UnitStrc* __fastcall AITHINK_TargetCallback_FetishShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDC600
//1.14d: 0x005F9CF0
void __fastcall AITHINK_Fn066_SandMaggotQueen(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC840
//1.14d: 0x005FA010
void __fastcall AITHINK_Fn068_VileMother(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDCBF0
//1.14d: 0x005FA280
void __fastcall AITHINK_Fn069_VileDog(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDCCD0
//1.14d: 0x005FA380
void __fastcall AITHINK_Fn070_FingerMage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDD060
//1.14d: 0x005FA710
void __fastcall AITHINK_Fn071_Regurgitator(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDD5C0
//1.14d: 0x005FA690
D2UnitStrc* __fastcall AITHINK_TargetCallback_Regurgitator(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDD790
//1.14d: 0x005FAA90
void __fastcall AITHINK_Fn072_DoomKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDD850
//1.14d: 0x005FAB80
void __fastcall AITHINK_Fn073_AbyssKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDDB10
//1.14d: 0x005FAF00
void __fastcall AITHINK_Fn074_OblivionKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDDFA0
//1.14d: 0x005FAE40
D2UnitStrc* __fastcall AITHINK_TargetCallback_OblivionKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDE150
//1.14d: 0x005FB2A0
void __fastcall AITHINK_Fn075_QuillMother(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE2B0
//1.14d: 0x005FB410
void __fastcall AITHINK_Fn076_EvilHole(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE4D0
//1.14d: 0x005FB5B0
void __fastcall AITHINK_Fn077_TrapMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE570
//1.14d: 0x005FB6C0
void __fastcall AITHINK_Fn078_TrapRightArrow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE710
//1.14d: 0x005FB7E0
void __fastcall AITHINK_Fn079_TrapLeftArrow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE8B0
//1.14d: 0x005FB900
void __fastcall AITHINK_Fn080_092_TrapPoison_TrapNova(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE960
//1.14d: 0x005FBA60
void __fastcall AITHINK_Fn087_TrapMelee(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE9E0
//1.14d: 0x005E0160
void __fastcall AITHINK_Fn082_InvisoSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDEAF0
//1.14d: 0x005E0260
void __fastcall AITHINK_Fn083_MosquitoNest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDEC70
//1.14d: 0x005E0390
void __fastcall AITHINK_InitializeBoneWallLifetime(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDECE0
//1.14d: 0x005E0400
void __fastcall AITHINK_Fn084_BoneWall(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDED10
//1.14d: 0x005E0490
void __fastcall AITHINK_Fn085_HighPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDF2E0
//1.14d: 0x005E0430
D2UnitStrc* __fastcall AITHINK_TargetCallback_HighPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDF410
//1.14d: 0x005E0980
void __fastcall AITHINK_Fn094_DesertTurret(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDF780
//1.14d: 0x005E0C80
void __fastcall AITHINK_Fn089_Megademon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDFA50
//1.14d: 0x005E0F60
void __fastcall AITHINK_Fn093_ArcaneTower(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDFB80
//1.14d: 0x005E1080
void __fastcall AITHINK_Fn095_PantherJavelin(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDFD50
//1.14d: 0x005E1250
void __fastcall AITHINK_Fn096_FetishBlowgun(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0050
//1.14d: 0x005E1540
void __fastcall AITHINK_Fn114_ReanimatedHorde(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0220
//1.14d: 0x005E1860
void __fastcall AITHINK_Fn113_SiegeTower(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0430
//1.14d: 0x005E1720
D2UnitStrc* __fastcall AITHINK_TargetCallback_SiegeBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE0610
//1.14d: 0x005E1900
void __fastcall AITHINK_Fn115_SiegeBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0960
//1.14d: 0x005B0BD0
int32_t __fastcall AITHINK_GetSquaredDistance(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//D2Game.0x6FCE0A50
//1.14d: 0x005E1B60
void __fastcall AITHINK_Fn116_Minion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0C10
//1.14d: 0x005E1D30
void __fastcall AITHINK_Fn117_SuicideMinion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0CD0
//1.14d: 0x005E1E00
void __fastcall AITHINK_Fn118_Succubus(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0FE0
//1.14d: 0x005E2120
void __fastcall AITHINK_Fn119_SuccubusWitch(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1480
//1.14d: 0x005E2610
void __fastcall AITHINK_SpecialState14_AttackOrApproach(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1550
//1.14d: 0x005E27A0
void __fastcall AITHINK_Fn120_Overseer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1910
//1.14d: 0x005E26D0
D2UnitStrc* __fastcall AITHINK_TargetCallback_Overseer_Nihlathak(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE1B90
//1.14d: 0x005E2BD0
void __fastcall AITHINK_Fn121_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1CA0
//1.14d: 0x005E2B70
D2UnitStrc* __fastcall AITHINK_TargetCallback_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE1D30
//1.14d: 0x005E2CD0
void __fastcall AITHINK_InitializeMountedImpSkills(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1DC0
//1.14d: 0x005E2D80
void __fastcall AITHINK_SpecialState16_MountedImp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2080
//1.14d: 0x005E2FD0
void __fastcall AITHINK_InitializeImpMountTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2090
//1.14d: 0x005E2FF0
void __fastcall AITHINK_Fn122_Imp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2570
//1.14d: 0x005E34C0
void __fastcall AITHINK_Fn123_Catapult(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE25D0
//1.14d: 0x005E3530
void __fastcall AITHINK_Fn124_FrozenHorror(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2760
//1.14d: 0x005E36F0
void __fastcall AITHINK_Fn125_BloodLord(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE28A0
//1.14d: 0x005E3840
void __fastcall AITHINK_Fn097_Spirit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE28F0
//1.14d: 0x005E3890
void __fastcall AITHINK_Fn098_Smith(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2960
//1.14d: 0x005E4CF0
void __fastcall AITHINK_Fn067_NecroPet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2BA0
//1.14d: 0x005E3EA0
int32_t __fastcall D2GAME_AI_PetMove_6FCE2BA0(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t nMovementMode, int32_t bRun, int32_t nVelocity, int32_t nFollowDistance);
//D2Game.0x6FCE34B0
//1.14d: 0x005E3900
void __fastcall AITHINK_CountNearbyPets(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE34E0
//1.14d: 0x005E45D0
int32_t __fastcall AITHINK_TryPetFollowOwner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2UnitStrc* pOwner, int32_t bInMeleeRange, D2AiTickParamStrc* pAiTickParam, int32_t bSuppressIdleMovement, int32_t nFollowDistanceBonus);
//D2Game.0x6FCE3740
//1.14d: 0x005E4830
void __fastcall AITHINK_MeleePet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE39E0
//1.14d: 0x005E52D0
void __fastcall AITHINK_Fn061_Hireable(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE3EE0
//1.14d: 0x005E3930
int32_t __fastcall D2GAME_PETAI_PetMove_6FCE3EE0(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t eMotionType, int32_t bRun, int32_t nSpeed, BYTE bSteps);
//D2Game.0x6FCE4610
//1.14d: 0x005E5050
void __fastcall AITHINK_HirelingCombat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nClassId, D2UnitStrc* pOwner, D2UnitStrc* pTarget, D2SeedStrc* pSeed, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE4830
//1.14d: 0x005E4D30
void __fastcall AITHINK_SelectHirelingAction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nHirelingId, D2UnitStrc* pTarget, D2SeedStrc* pSeed);
//D2Game.0x6FCE4B90
//1.14d: 0x005E5730
void __fastcall AITHINK_InitializeFormationOffsets(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE4CC0
//1.14d: 0x005E5870
void __fastcall AITHINK_Formation(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE4FD0
//1.14d: 0x005E5710
void __fastcall AITHINK_MinionCallback_CheckDead(D2UnitStrc* pUnit, void* pDeadMinionFound, void* pUnused);
//D2Game.0x6FCE4FF0
//1.14d: 0x005E5AC0
void __fastcall AITHINK_Fn090_Griswold(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5080
//1.14d: 0x005E5C50
void __fastcall AITHINK_SpecialState13_Countess(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5520
//1.14d: 0x005E5B70
int32_t __fastcall AITHINK_UseSkillAtMapAiPosition(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5610
//1.14d: 0x005E6190
void __fastcall AITHINK_InitializeGenericSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5640
//1.14d: 0x005E61B0
void __fastcall AITHINK_Fn129_GenericSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE58D0
//1.14d: 0x005E6300
void __fastcall AITHINK_ResetBloodRavenReturnState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE58E0
//1.14d: 0x005E6320
void __fastcall AITHINK_Fn059_BloodRaven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5EE0
//1.14d: 0x005E68F0
uint8_t __fastcall AITHINK_HandleNpcProximityAndInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE61E0
//1.14d: 0x005E6DE0
int32_t __fastcall AITHINK_MapAiAction_WalkToPosition(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE6270
//1.14d: 0x005E6E80
int32_t __fastcall AITHINK_MapAiAction_WalkToPositionOrWait(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE6340
//1.14d: 0x005E6F00
int32_t __fastcall AITHINK_MapAiAction_QueueSkill1(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE64D0
//1.14d: 0x005E6FC0
int32_t __fastcall AITHINK_MapAiAction_QueueSkill2(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE6660
//1.14d: 0x005E7130
void __fastcall AITHINK_Fn032_Npc(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE69A0
//1.14d: 0x005E6AE0
uint8_t __fastcall AITHINK_ExecuteNpcAiCommands(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE6CD0
//1.14d: 0x005E7080
int32_t __fastcall AITHINK_ExecuteMapAiAction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE6DC0
//1.14d: 0x005E73A0
void __fastcall AITHINK_Fn054_NpcStationary(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE6F80
//1.14d: 0x005E7540
void __fastcall AITHINK_Fn041_Towner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7070
//1.14d: 0x005E7590
void __fastcall AITHINK_Fn081_JarJar(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE73A0
//1.14d: 0x005E7880
void __fastcall AITHINK_Fn031_NpcOutOfTown(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7640
//1.14d: 0x005E77A0
int32_t __fastcall AITHINK_SpawnNpcPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE77A0
//1.14d: 0x005E7AC0
void __fastcall AITHINK_Fn060_GoodNpcRanged(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7930
//1.14d: 0x005E7C10
void __fastcall D2GAME_AI_SpecialState06_6FCE7930(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7A60
//1.14d: 0x005E7D60
void __fastcall AITHINK_Fn062_TownRogue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7AC0
//1.14d: 0x005E7E20
void __fastcall AITHINK_Fn058_Navi(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7C10
//1.14d: 0x005B0CD0
void __fastcall AITHINK_Fn001_100_Idle_Buffy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7C40
//1.14d: 0x005E7F80
void __fastcall D2GAME_AI_SpecialState09_6FCE7C40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7CF0
//1.14d: 0x005E8020
void __fastcall D2GAME_AI_SpecialState10_17_6FCE7CF0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7E20
//1.14d: 0x005E80E0
void __fastcall D2GAME_AI_SpecialState11_6FCE7E20(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7E80
//1.14d: 0x005E8140
void __fastcall D2GAME_AI_SpecialState11_6FCE7E80(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE81B0
//1.14d: 0x005E8340
void __fastcall D2GAME_AI_SpecialState12_6FCE81B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE82F0
//1.14d: 0x005E8480
void __fastcall AITHINK_ResetAiAndPreserveHomeCommand(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE83A0
//1.14d: 0x005E8530
int32_t __fastcall AITHINK_GetTargetScore(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2AiCmdStrc* pAiCmd);
//D2Game.0x6FCE86C0
//1.14d: 0x005E8F20
D2UnitStrc* __fastcall AITHINK_GetTargetForBoss(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pTargetScore, int32_t* pCounter, D2AiCmdStrc* pAiCmd, int32_t(__fastcall* pfCull)(D2UnitStrc*, D2UnitStrc*));
//D2Game.0x6FCE8950
//1.14d: 0x005E9170
void __fastcall AITHINK_Fn051_Diablo(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE97C0
//1.14d: 0x005E8EB0
int32_t __fastcall AITHINK_CullPotentialTargetsForDiablo(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FCE9890
//1.14d: 0x005E9E00
void __fastcall AITHINK_Fn042_Vendor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE98E0
//1.14d: 0x005E9E60
void __fastcall AITHINK_Fn086_Hydra(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9980
//1.14d: 0x005E9F10
void __fastcall AITHINK_Fn099_TrappedSoul(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9AF0
//1.14d: 0x005EA080
void __fastcall AITHINK_Fn088_7TIllusion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9BA0
//1.14d: 0x005EA130
void __fastcall AITHINK_Fn091_DarkWanderer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9CE0
//1.14d: 0x005EA290
void __fastcall AITHINK_InitializeAssassinSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9D00
//1.14d: 0x005EA3D0
void __fastcall AITHINK_Fn101_AssassinSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9E60
//1.14d: 0x005EA2B0
int32_t __fastcall AITHINK_AssasinSentryHasLostTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, int32_t bDecreaseParam);
//D2Game.0x6FCE9FB0
//1.14d: 0x005EA510
void __fastcall AITHINK_InitializeBladeCreeper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9FD0
//1.14d: 0x005EA540
void __fastcall AITHINK_Fn102_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA2A0
//1.14d: 0x005EA7A0
void __fastcall AITHINK_Fn103_InvisoPet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA490
//1.14d: 0x005EA980
void __fastcall AITHINK_Fn104_DeathSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA680
//1.14d: 0x005EAF50
void __fastcall AITHINK_InitializeShadowWarrior(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA6D0
//1.14d: 0x005EAFA0
void __fastcall AITHINK_Fn105_ShadowWarrior(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEAC10
//1.14d: 0x005EAD50
BOOL __fastcall AITHINK_ShadowWarriorCheckUseSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, D2SkillStrc* pSkill, int32_t nSkillId, int32_t bCombat, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEAF20
//1.14d: 0x005EAB20
int32_t __fastcall AITHINK_ShadowMasterCheckTargetPetType(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId);
//D2Game.0x6FCEAFE0
//1.14d: 0x005EB490
void __fastcall AITHINK_InitializeShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEB1B0
//1.14d: 0x005EB5C0
void __fastcall AITHINK_InitializeShadowMasterAttackSkills(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEB240
//1.14d: 0x005EB970
void __fastcall AITHINK_Fn106_143_ShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEC840
//1.14d: 0x005EB650
int32_t __fastcall AITHINK_IsTargetKillableByShadowMaster(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FCEC910
//1.14d: 0x005EB6D0
D2UnitStrc* __fastcall AITHINK_TargetCallback_ShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCECBA0
//1.14d: 0x005EB8B0
int32_t __fastcall AITHINK_UseShadowMasterSkill(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, WORD wSkillId, int32_t bCombat, D2UnitStrc* pTarget, int32_t nX, int32_t nY, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCECC40
//1.14d: 0x005EC6A0
void __fastcall AITHINK_InitializeVineState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCECC50
//1.14d: 0x005EC6C0
void __fastcall AITHINK_Fn110_Vines(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCECE50
//1.14d: 0x005EC8C0
void __fastcall AITHINK_Fn111_CycleOfLife(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCED140
//1.14d: 0x005ECB70
void __fastcall AITHINK_InitializeRaven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCED190
//1.14d: 0x005ECC10
void __fastcall AITHINK_Fn107_Raven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//Inlined in D2Game.0x6FCED540
//1.14d: 0x005ED2A0
void __fastcall AITHINK_Fn108_Fenris(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//Inlined in D2Game.0x6FCED540
//1.14d: 0x005ECEE0
void __fastcall AITHINK_Fn108_SpiritWolf(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCED540
//1.14d: 0x005ED710
void __fastcall AITHINK_Fn108_DruidWolf(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEDF70
//1.14d: 0x005ED730
void __fastcall AITHINK_Fn112_DruidBear(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEE250
//1.14d: 0x005ED9E0
void __fastcall AITHINK_Fn109_Totem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEE450
//1.14d: 0x005EDC50
void __fastcall AITHINK_Fn127_NpcBarb(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEE6F0
//1.14d: 0x005EE040
void __fastcall AITHINK_Fn126_CatapultSpotter(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEEAD0
//1.14d: 0x005EE260
void __fastcall AITHINK_Fn130_DeathMauler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEEC00
//1.14d: 0x005EE3C0
void __fastcall AITHINK_Fn131_Wussie(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEEE60
//1.14d: 0x005EE5D0
void __fastcall AITHINK_Fn128_Nihlathak(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEF330
//1.14d: 0x005EEAA0
void __fastcall AITHINK_Fn132_AncientStatue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEF3F0
//1.14d: 0x005EEB70
void __fastcall AITHINK_AncientBarb1SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEF730
//1.14d: 0x005EEB10
int32_t __fastcall AITHINK_AreUnitsInSameLevel(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//Inlined in D2Game.0x6FCEFBC0
//1.14d: 0x005EEDA0
void __fastcall AITHINK_AncientBarb2SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEFA10
//1.14d: 0x005EF010
void __fastcall AITHINK_AncientBarb3SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEFBC0
//1.14d: 0x005EF1A0
void __fastcall AITHINK_Fn133_Ancient(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEFBF0
//1.14d: 0x005EF320
void __fastcall AITHINK_Fn134_BaalThrone(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0030
void __fastcall AITHINK_BaalThroneSetAiParamFlag(D2AiTickParamStrc* pAiTickParam, int32_t nFlag, int32_t bSet);
//D2Game.0x6FCF0050
//1.14d: 0x005EF5E0
D2UnitStrc* __fastcall AITHINK_TargetCallback_BaalToStairs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF0090
//1.14d: 0x005EF620
void __fastcall AITHINK_Fn138_BaalToStairs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0180
//1.14d: 0x005EF710
void __fastcall AITHINK_Fn136_BaalTaunt(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF02D0
//1.14d: 0x005EF820
void __fastcall AITHINK_Fn139_BaalTentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0420
//1.14d: 0x005EF910
void __fastcall AITHINK_Fn141_BaalMinion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0570
//1.14d: 0x005EFA50
D2UnitStrc* __fastcall AITHINK_TargetCallback_PutridDefiler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF05B0
//1.14d: 0x005EFA90
void __fastcall AITHINK_Fn137_PutridDefiler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF06A0
//1.14d: 0x005B0D60
D2C_AiSpecialState __fastcall AITHINK_GetSpecialStateFromAiControl(D2AiControlStrc* pAiControl);
//D2Game.0x6FCF06B0
//1.14d: 0x005B0D70
void __fastcall AITHINK_SetAiControlParams(D2AiControlStrc* pAiControl, int32_t nParam0, int32_t nParam1, int32_t nParam2);
//D2Game.0x6FCF06E0
//1.14d: 0x005B0DA0
BOOL __fastcall AITHINK_CanUnitSwitchAi(D2UnitStrc* pUnit, D2MonStatsTxt* pMonStatsTxtRecord, D2C_AiSpecialState nAiSpecialState, int32_t bCheckIfSuperUnique);
//D2Game.0x6FCF0750
//1.14d: 0x005B15D0
const D2AiTableStrc* __fastcall AITHINK_GetAiTableRecord(D2UnitStrc* pUnit, D2C_AiSpecialState nAiSpecialState);
//D2Game.0x6FCF07D0
//1.14d: 0x005B0E00
void __fastcall AITHINK_ExecuteAiFn(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2C_AiSpecialState nAiSpecialState);
//D2Game.0x6FCF0A70
//1.14d: 0x005B1740
void __fastcall AITHINK_ExecuteMonsterAiTick(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnused1, int32_t nUnused2);
//D2Game.0x6FCF0D10
//1.14d: 0x005B0FF0
int32_t __fastcall D2GAME_AICORE_MinionLeash_6FCF0D10(D2GameStrc* pGame, D2UnitStrc* pMonster, D2AiTickParamStrc* pAiTickParam);
//Inlined in D2Game.0x6FCF0E40
int32_t __fastcall AITHINK_RetargetBlockedApproach(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0E40
//1.14d: 0x005B13E0
int32_t __fastcall AITHINK_HandleMonsterAiPreActions(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);