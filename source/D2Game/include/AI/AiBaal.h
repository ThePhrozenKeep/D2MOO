#pragma once

#include <Units/Units.h>
#include "AiGeneral.h"


//D2Game.0x6FCCD450
//1.14d: 0x005FC830
void __fastcall AIBAAL_CountLivingMinions(D2UnitStrc* pUnit, void* ppUnitArg, void* pCounterArg);
//D2Game.0x6FCCD470
//1.14d: 0x005FCF30
void __fastcall AIBAAL_RunSpecialAiState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
//D2Game.0x6FCCD520
//1.14d: 0x005FCFE0
void __fastcall AITHINK_Fn135_BaalCrab(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
#ifdef D2_VERSION_HAS_UBERS
//1.14d: 0x005FD200
void __fastcall AITHINK_Fn145_UberBaal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
#endif
//D2Game.0x6FCCD630
//1.14d: 0x005FBDD0
D2UnitStrc* __fastcall AIBAAL_GetTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pMax, int32_t* pCount, void* pArgs, int32_t(__fastcall* pfCull)(D2UnitStrc*, D2UnitStrc*));
//D2Game.0x6FCCD8A0
//1.14d: 0x005FBB00
int32_t __fastcall AIBAAL_GetTargetScore(D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pArgs);
//D2Game.0x6FCCDBB0
//1.14d: 0x005FC000
int32_t __fastcall AIBAAL_CullPotentialTargets(D2UnitStrc* pBaal, D2UnitStrc* pTarget);
//D2Game.0x6FCCDC80
//1.14d: 0x005FC630
int32_t __fastcall AIBAAL_RollRandomAiParam(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2UnitStrc* pTarget, int32_t nMax, int32_t nCount, D2AiCmdStrc* pAiCmd);
//D2Game.0x6FCCE040
//1.14d: 0x005FC0C0
int32_t __fastcall AI_CheckSpecialSkillsOnPrimeEvil(D2UnitStrc* pUnit);
//D2Game.0x6FCCE100
int32_t __fastcall AI_GetRandomArrayIndex(int32_t* pArray, int32_t nArraySize, D2UnitStrc* pUnit, int32_t nDefaultValue);
//D2Game.0x6FCCE1A0
//1.14d: 0x005FC450
int32_t __fastcall AIBAAL_RollRandomAiParamForNonCollidingUnit(D2GameStrc* pGame, D2AiControlStrc* pAiControl, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nCount, int32_t bInMediumRange, int32_t bInFarRange, int32_t bInCloseRange, int32_t nMax);
//D2Game.0x6FCCE450
//1.14d: 0x005FCB60
void __fastcall AIBAAL_MainSkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2UnitStrc* pTarget, int32_t nParam, D2AiCmdStrc* pAiCmd);
//D2Game.0x6FCCEB70
//1.14d: 0x005FD210
void __fastcall AITHINK_Fn140_BaalCrabClone(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam);
