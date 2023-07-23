#pragma once

#include "AiGeneral.h"
#include <Units/Units.h>
#include <Units/UnitFinds.h>


void __fastcall AITHINK_Fn000(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1660
void __fastcall D2GAME_AI_SpecialState02_6FCD1660(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1750
void __fastcall AITHINK_Fn002_Skeleton(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1880
void __fastcall AITHINK_Fn003_Zombie(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1990
void __fastcall AITHINK_Fn004_Bighead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1BA0
void __fastcall AITHINK_Fn005_BloodHawk(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD1D50
void __fastcall AITHINK_Fn006_Fallen(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2220
void __fastcall AITHINK_Fn007_Brute(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2370
void __fastcall AITHINK_Fn008_SandRaider(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2680
D2UnitStrc* __fastcall AITHINK_TargetCallback_SandRaider(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD27A0
void __fastcall AITHINK_Fn009_Wraith(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2850
void __fastcall AITHINK_Fn010_CorruptRogue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2A00
void __fastcall AITHINK_Fn011_Baboon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2E80
void __fastcall AITHINK_Fn014_QuillRat(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD2FF0
void __fastcall AITHINK_Fn013_FallenShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD32E0
D2UnitStrc* __fastcall AITHINK_TargetCallback_FallenShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD34A0
void __fastcall D2GAME_AI_Unk015_6FCD34A0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3540
void __fastcall AITHINK_Fn015_SandMaggot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3900
void __fastcall AITHINK_Fn016_ClawViper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3B90
void __fastcall AITHINK_Fn142_ClawViperEx(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD3E70
void __fastcall AITHINK_Fn017_SandLeaper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4050
void __fastcall AITHINK_Fn018_PantherWoman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD41F0
D2UnitStrc* __fastcall AITHINK_TargetCallback_Panther(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD4390
void __fastcall AITHINK_Fn012_019_Goatman_Swarm(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4440
void __fastcall AITHINK_Fn020_Scarab(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4720
void __fastcall AITHINK_Fn021_Mummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD48B0
void __fastcall AITHINK_Fn022_GreaterMummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD4C10
D2UnitStrc* __fastcall AITHINK_TargetCallback_GreaterMummy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD4DD0
void __fastcall AITHINK_Fn023_Vulture(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD55D0
int32_t __fastcall sub_6FCD55D0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCD5710
D2UnitStrc* __fastcall AITHINK_TargetCallback_Vulture(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCD5850
void __fastcall AITHINK_Fn024_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD5A60) --------------------------------------------------------
void __fastcall AITHINK_Fn025_Willowisp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD6960
int32_t __fastcall AITHINK_UnitFindCallback_Willowisp(D2UnitStrc* pUnit, D2UnitFindArgStrc* pUnitFindArg);
//D2Game.0x6FCD69E0
void __fastcall AITHINK_Fn026_Arach(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD6D60
void __fastcall AITHINK_Fn027_ThornHulk(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD6FD0
void __fastcall AITHINK_Fn028_Vampire(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD76F0
void __fastcall D2GAME_AI_Unk029_6FCD76F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD7760
void __fastcall AITHINK_Fn029_BatDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD7BA0
void __fastcall AITHINK_Fn030_Fetish(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD7EB0
void __fastcall AITHINK_Fn033_HellMeteor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8090
void __fastcall AITHINK_Fn034_Andariel(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8260
void __fastcall AITHINK_Fn035_CorruptArcher(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD85B0
void __fastcall AITHINK_Fn036_CorruptLancer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD88C0
void __fastcall AITHINK_Fn037_SkeletonBow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8A60
void __fastcall AITHINK_Fn038_MaggotLarva(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8B60
void __fastcall AITHINK_Fn039_PinHead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8D20
void __fastcall AITHINK_Fn040_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8E10
void __fastcall D2GAME_AI_Unk043_045_121_6FCD8E10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8E30
void __fastcall AITHINK_Fn043_FoulCrowNest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD8FE0
void __fastcall AITHINK_Fn044_Duriel(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD91F0
void __fastcall AITHINK_Fn045_Sarcophagus(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD93A0
void __fastcall AITHINK_Fn046_ElementalBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD94D0
void __fastcall AITHINK_Fn047_FlyingScimitar(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD9640
void __fastcall AITHINK_Fn048_ZakarumZealot(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD9A10
void __fastcall AITHINK_Fn049_ZakarumPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCD9F10
D2UnitStrc* __fastcall AITHINK_TargetCallback_ZakarumPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDA0C0
D2UnitStrc* __fastcall AITHINK_FindTargetForMephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MonStatsTxt* pMonStatsTxtRecord, D2UnitStrc* a4);
//D2Game.0x6FCDA190
D2UnitStrc* __fastcall AITHINK_TargetCallback_Mephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDA300
void __fastcall AITHINK_Fn050_Mephisto(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDA910
void __fastcall D2GAME_AI_Unk052_6FCDA910(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDAAA0
void __fastcall AITHINK_Fn052_FrogDemon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDAFC0
void __fastcall AITHINK_Fn053_Summoner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDB3E0
void __fastcall AITHINK_Fn055_Izual(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDB720
void __fastcall AITHINK_Fn056_Tentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDBAA0
void __fastcall AITHINK_Fn057_TentacleHead(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDBCE0
void __fastcall AITHINK_Fn063_GargoyleTrap(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDBF20
void __fastcall AITHINK_Fn064_SkeletonMage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC170
void __fastcall D2GAME_AI_SpecialState04_6FCDC170(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC1C0
void __fastcall AITHINK_Fn065_FetishShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC420
D2UnitStrc* __fastcall AITHINK_TargetCallback_FetishShaman(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDC600
void __fastcall AITHINK_Fn066_SandMaggotQueen(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDC840
void __fastcall AITHINK_Fn068_VileMother(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDCBF0
void __fastcall AITHINK_Fn069_VileDog(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDCCD0
void __fastcall AITHINK_Fn070_FingerMage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDD060
void __fastcall AITHINK_Fn071_Regurgitator(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDD5C0
D2UnitStrc* __fastcall AITHINK_TargetCallback_Regurgitator(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDD790
void __fastcall AITHINK_Fn072_DoomKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDD850
void __fastcall AITHINK_Fn073_AbyssKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDDB10
void __fastcall AITHINK_Fn074_OblivionKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDDFA0
D2UnitStrc* __fastcall AITHINK_TargetCallback_OblivionKnight(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDE150
void __fastcall AITHINK_Fn075_QuillMother(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE2B0
void __fastcall AITHINK_Fn076_EvilHole(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE4D0
void __fastcall AITHINK_Fn077_TrapMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE570
void __fastcall AITHINK_Fn078_TrapRightArrow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE710
void __fastcall AITHINK_Fn079_TrapLeftArrow(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE8B0
void __fastcall AITHINK_Fn080_092_TrapPoison_TrapNova(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE960
void __fastcall AITHINK_Fn087_TrapMelee(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDE9E0
void __fastcall AITHINK_Fn082_InvisoSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDEAF0
void __fastcall AITHINK_Fn083_MosquitoNest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDEC70
void __fastcall D2GAME_AI_Unk084_6FCDEC70(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDECE0
void __fastcall AITHINK_Fn084_BoneWall(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDED10
void __fastcall AITHINK_Fn085_HighPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDF2E0
D2UnitStrc* __fastcall AITHINK_TargetCallback_HighPriest(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCDF410
void __fastcall AITHINK_Fn094_DesertTurret(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDF780
void __fastcall AITHINK_Fn089_Megademon(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDFA50
void __fastcall AITHINK_Fn093_ArcaneTower(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDFB80
void __fastcall AITHINK_Fn095_PantherJavelin(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCDFD50
void __fastcall AITHINK_Fn096_FetishBlowgun(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0050
void __fastcall AITHINK_Fn114_ReanimatedHorde(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0220
void __fastcall AITHINK_Fn113_SiegeTower(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0430
D2UnitStrc* __fastcall AITHINK_TargetCallback_SiegeBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE0610
void __fastcall AITHINK_Fn115_SiegeBeast(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0960
int32_t __fastcall AITHINK_GetSquaredDistance(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//D2Game.0x6FCE0A50
void __fastcall AITHINK_Fn116_Minion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0C10
void __fastcall AITHINK_Fn117_SuicideMinion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0CD0
void __fastcall AITHINK_Fn118_Succubus(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE0FE0
void __fastcall AITHINK_Fn119_SuccubusWitch(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1480
void __fastcall D2GAME_AI_SpecialState14_6FCE1480(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1550
void __fastcall AITHINK_Fn120_Overseer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1910
D2UnitStrc* __fastcall AITHINK_TargetCallback_Overseer_Nihlathak(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE1B90
void __fastcall AITHINK_Fn121_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1CA0
D2UnitStrc* __fastcall AITHINK_TargetCallback_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE1D30
void __fastcall D2GAME_AI_SpecialState16_6FCE1D30(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE1DC0
void __fastcall D2GAME_AI_SpecialState16_6FCE1DC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2080
void __fastcall D2GAME_AI_Unk122_6FCE2080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2090
void __fastcall AITHINK_Fn122_Imp(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2570
void __fastcall AITHINK_Fn123_Catapult(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE25D0
void __fastcall AITHINK_Fn124_FrozenHorror(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2760
void __fastcall AITHINK_Fn125_BloodLord(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE28A0
void __fastcall AITHINK_Fn097_Spirit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE28F0
void __fastcall AITHINK_Fn098_Smith(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2960
void __fastcall AITHINK_Fn067_NecroPet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE2BA0
int32_t __fastcall D2GAME_AI_PetMove_6FCE2BA0(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t a4, int32_t bRun, int32_t nVelocity, int32_t a7);
//D2Game.0x6FCE34B0
void __fastcall sub_6FCE34B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCE34E0
int32_t __fastcall sub_6FCE34E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2UnitStrc* pOwner, int32_t a5, D2AiTickParamStrc* pAiTickParam, int32_t a7, int32_t a8);
//D2Game.0x6FCE3740
void __fastcall sub_6FCE3740(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE39E0
void __fastcall AITHINK_Fn061_Hireable(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE3EE0
int32_t __fastcall D2GAME_PETAI_PetMove_6FCE3EE0(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, int32_t eMotionType, int32_t bRun, int32_t nSpeed, BYTE bSteps);
//D2Game.0x6FCE4610
void __fastcall sub_6FCE4610(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nClassId, D2UnitStrc* pOwner, D2UnitStrc* pTarget, D2SeedStrc* pSeed, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE4830
void __fastcall sub_6FCE4830(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nHirelingId, D2UnitStrc* pTarget, D2SeedStrc* pSeed);
//D2Game.0x6FCE4B90
void __fastcall D2GAME_AI_SpecialState03_6FCE4B90(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE4CC0
void __fastcall D2GAME_AI_SpecialState03_6FCE4CC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE4FD0
void __fastcall sub_6FCE4FD0(D2UnitStrc* pUnit, void* a2, void* a3);
//D2Game.0x6FCE4FF0
void __fastcall AITHINK_Fn090_Griswold(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5080
void __fastcall D2GAME_AI_SpecialState13_6FCE5080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5520
int32_t __fastcall sub_6FCE5520(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5610
void __fastcall D2GAME_AI_Unk129_6FCE5610(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5640
void __fastcall AITHINK_Fn129_GenericSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE58D0
void __fastcall D2GAME_AI_Unk59_6FCE58D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE58E0
void __fastcall AITHINK_Fn059_BloodRaven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE5EE0
uint8_t __fastcall sub_6FCE5EE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE61E0
int32_t __fastcall sub_6FCE61E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE6270
int32_t __fastcall sub_6FCE6270(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE6340
int32_t __fastcall sub_6FCE6340(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE64D0
int32_t __fastcall sub_6FCE64D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, D2MapAIPathPositionStrc* pPosition);
//D2Game.0x6FCE6660
void __fastcall AITHINK_Fn032_Npc(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE69A0
uint8_t __fastcall sub_6FCE69A0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE6CD0
int32_t __fastcall AITHINK_ExecuteMapAiAction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE6DC0
void __fastcall AITHINK_Fn054_NpcStationary(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE6F80
void __fastcall AITHINK_Fn041_Towner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7070
void __fastcall AITHINK_Fn081_JarJar(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE73A0
void __fastcall AITHINK_Fn031_NpcOutOfTown(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7640
int32_t __fastcall AITHINK_SpawnNpcPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE77A0
void __fastcall AITHINK_Fn060_GoodNpcRanged(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7930
void __fastcall D2GAME_AI_SpecialState06_6FCE7930(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7A60
void __fastcall AITHINK_Fn062_TownRogue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7AC0
void __fastcall AITHINK_Fn058_Navi(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7C10
void __fastcall AITHINK_Fn001_100_Idle_Buffy(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7C40
void __fastcall D2GAME_AI_SpecialState09_6FCE7C40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7CF0
void __fastcall D2GAME_AI_SpecialState10_17_6FCE7CF0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7E20
void __fastcall D2GAME_AI_SpecialState11_6FCE7E20(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE7E80
void __fastcall D2GAME_AI_SpecialState11_6FCE7E80(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE81B0
void __fastcall D2GAME_AI_SpecialState12_6FCE81B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE82F0
void __fastcall D2GAME_AI_Unk051_6FCE82F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE83A0
int32_t __fastcall AITHINK_GetTargetScore(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2AiCmdStrc* pAiCmd);
//D2Game.0x6FCE86C0
D2UnitStrc* __fastcall AITHINK_GetTargetForBoss(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* a3, int32_t* pCounter, D2AiCmdStrc* pAiCmd, int32_t(__fastcall* pfCull)(D2UnitStrc*, D2UnitStrc*));
//D2Game.0x6FCE8950
void __fastcall AITHINK_Fn051_Diablo(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE97C0
int32_t __fastcall AITHINK_CullPotentialTargetsForDiablo(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FCE9890
void __fastcall AITHINK_Fn042_Vendor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE98E0
void __fastcall AITHINK_Fn086_Hydra(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9980
void __fastcall AITHINK_Fn099_TrappedSoul(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9AF0
void __fastcall AITHINK_Fn088_7TIllusion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9BA0
void __fastcall AITHINK_Fn091_DarkWanderer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9CE0
void __fastcall D2GAME_AI_Unk101_104_6FCE9CE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9D00
void __fastcall AITHINK_Fn101_AssassinSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9E60
int32_t __fastcall AITHINK_AssasinSentryHasLostTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam, int32_t bDecreaseParam);
//D2Game.0x6FCE9FB0
void __fastcall D2GAME_AI_Unk102_6FCE9FB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCE9FD0
void __fastcall AITHINK_Fn102_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA2A0
void __fastcall AITHINK_Fn103_InvisoPet(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA490
void __fastcall AITHINK_Fn104_DeathSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA680
void __fastcall D2GAME_AI_Unk105_6FCEA680(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEA6D0
void __fastcall AITHINK_Fn105_ShadowWarrior(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEAC10
BOOL __fastcall AITHINK_ShadowWarriorCheckUseSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, D2SkillStrc* pSkill, int32_t nSkillId, int32_t bCombat, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEAF20
int32_t __fastcall AITHINK_ShadowMasterCheckTargetPetType(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId);
//D2Game.0x6FCEAFE0
void __fastcall D2GAME_AI_Unk106_6FCEAFE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEB1B0
void __fastcall D2GAME_AI_Unk143_6FCEB1B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEB240
void __fastcall AITHINK_Fn106_143_ShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEC840
int32_t __fastcall AITHINK_IsTargetKillableByShadowMaster(D2UnitStrc* a1, D2UnitStrc* a2);
//D2Game.0x6FCEC910
D2UnitStrc* __fastcall AITHINK_TargetCallback_ShadowMaster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCECBA0
int32_t __fastcall sub_6FCECBA0(D2GameStrc* pGame, D2UnitStrc* a2, D2UnitStrc* pUnit, WORD wSkillId, int32_t a5, D2UnitStrc* pTarget, int32_t nX, int32_t nY, D2AiTickParamStrc* a9);
//D2Game.0x6FCECC40
void __fastcall D2GAME_AI_Unk_110_111_6FCECC40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCECC50
void __fastcall AITHINK_Fn110_Vines(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCECE50
void __fastcall AITHINK_Fn111_CycleOfLife(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCED140
void __fastcall D2GAME_AI_Unk107_6FCED140(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCED190
void __fastcall AITHINK_Fn107_Raven(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//Inlined in D2Game.0x6FCED540
void __fastcall AITHINK_Fn108_Fenris(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//Inlined in D2Game.0x6FCED540
void __fastcall AITHINK_Fn108_SpiritWolf(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCED540
void __fastcall AITHINK_Fn108_DruidWolf(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEDF70
void __fastcall AITHINK_Fn112_DruidBear(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEE250
void __fastcall AITHINK_Fn109_Totem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEE450
void __fastcall AITHINK_Fn127_NpcBarb(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEE6F0
void __fastcall AITHINK_Fn126_CatapultSpotter(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEEAD0
void __fastcall AITHINK_Fn130_DeathMauler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEEC00
void __fastcall AITHINK_Fn131_Wussie(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEEE60
void __fastcall AITHINK_Fn128_Nihlathak(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEF330
void __fastcall AITHINK_Fn132_AncientStatue(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEF3F0
void __fastcall AITHINK_AncientBarb1SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEF730
int32_t __fastcall AITHINK_AreUnitsInSameLevel(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//Inlined in D2Game.0x6FCEFBC0
void __fastcall AITHINK_AncientBarb2SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEFA10
void __fastcall AITHINK_AncientBarb3SkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEFBC0
void __fastcall AITHINK_Fn133_Ancient(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCEFBF0
void __fastcall AITHINK_Fn134_BaalThrone(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0030
void __fastcall AITHINK_BaalThroneSetAiParamFlag(D2AiTickParamStrc* pAiTickParam, int32_t nFlag, int32_t bSet);
//D2Game.0x6FCF0050
D2UnitStrc* __fastcall AITHINK_TargetCallback_BaalToStairs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF0090
void __fastcall AITHINK_Fn138_BaalToStairs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0180
void __fastcall AITHINK_Fn136_BaalTaunt(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF02D0
void __fastcall AITHINK_Fn139_BaalTentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0420
void __fastcall AITHINK_Fn141_BaalMinion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0570
D2UnitStrc* __fastcall AITHINK_TargetCallback_PutridDefiler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF05B0
void __fastcall AITHINK_Fn137_PutridDefiler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF06A0
int32_t __fastcall AITHINK_GetSpecialStateFromAiControl(D2AiControlStrc* pAiControl);
//D2Game.0x6FCF06B0
void __fastcall AITHINK_SetAiControlParams(D2AiControlStrc* pAiControl, int32_t nParam0, int32_t nParam1, int32_t nParam2);
//D2Game.0x6FCF06E0
int32_t __fastcall AITHINK_CanUnitSwitchAi(D2UnitStrc* pUnit, D2MonStatsTxt* pMonStatsTxtRecord, int32_t nSpecialState, int32_t bCheckIfSuperUnique);
//D2Game.0x6FCF0750
const D2AiTableStrc* __fastcall AITHINK_GetAiTableRecord(D2UnitStrc* pUnit, int32_t nSpecialState);
//D2Game.0x6FCF07D0
void __fastcall AITHINK_ExecuteAiFn(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t nSpecialState);
//D2Game.0x6FCF0A70
void __fastcall D2GAME_MONSTERS_AiFunction03_6FCF0A70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4);
//D2Game.0x6FCF0D10
int32_t __fastcall D2GAME_AICORE_MinionLeash_6FCF0D10(D2GameStrc* pGame, D2UnitStrc* pMonster, D2AiTickParamStrc* pAiTickParam);
//Inlined in D2Game.0x6FCF0E40
int32_t __fastcall sub_6FCF0E40_inline(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCF0E40
int32_t __fastcall sub_6FCF0E40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);