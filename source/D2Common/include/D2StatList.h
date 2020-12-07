#pragma once

#include "CommonDefinitions.h"


//D2Common.0x6FDB57C0 (#10563)
BOOL __stdcall STATLIST_AreUnitsAligned(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//----- (6FDB5830) --------------------------------------------------------
int __fastcall sub_6FDB5830(D2StatListExStrc* a1, int a2);
//D2Common.0x6FDB6300
int __fastcall sub_6FDB6300(D2StatExStrc* pStatEx, int nLayer_StatId);
//D2Common.0x6FDB6340
int __fastcall sub_6FDB6340(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord);
//D2Common.0x6FDB63E0
int __fastcall sub_6FDB63E0(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord);
//D2Common.0x6FDB64A0
int __fastcall sub_6FDB64A0(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit);
//D2Common.0x6FDB6920
D2StatStrc* __fastcall sub_6FDB6920(D2StatExStrc* pStatEx, int nLayer_StatId);
//D2Common.0x6FDB6970
D2StatStrc* __fastcall sub_6FDB6970(void* pMemPool, D2StatExStrc* pStatEx, int nLayer_StatId);
//D2Common.0x6FDB6A30
void __fastcall sub_6FDB6A30(void* pMemPool, D2StatExStrc* pStatEx, D2StatStrc* pStat);
//D2Common.0x6FDB6AB0
void __fastcall sub_6FDB6AB0(D2StatListExStrc* pStatList, int nLayer_StatId, int nValue, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit);
//D2Common.0x6FDB6C10
void __fastcall sub_6FDB6C10(D2StatListExStrc* pStatListEx, int nLayer_StatId, int nValue, D2UnitStrc* pUnit);
//D2Common.0x6FDB6E30
void __stdcall D2Common_ExpireStatListEx_6FDB6E30(D2StatListExStrc* pStatList);
//D2Common.0x6FDB7030 (#10485)
void __stdcall STATLIST_FreeStatList(D2StatListStrc* pStatList);
//D2Common.0x6FDB7050
void __fastcall D2Common_STATLIST_FreeStatListEx_6FDB7050(D2StatListExStrc* pStatListEx);
//D2Common.0x6FDB7110 (#10527)
void __stdcall STATLIST_FreeStatListEx(D2UnitStrc* pUnit);
//D2Common.0x6FDB7140 (#10470)
D2StatListStrc* __stdcall STATLIST_AllocStatList(void* pMemPool, DWORD fFilter, DWORD dwTimeout, int nUnitType, int nUnitGUID);
//D2Common.0x6FDB7190 (#10526)
void __stdcall STATLIST_AllocStatListEx(D2UnitStrc* pUnit, char nFlags, void* pCallbackFunc, D2GameStrc* pGame);
//D2Common.0x6FDB7260 (#10471)
int __stdcall STATLIST_GetOwnerType(D2StatListStrc* pStatList);
//D2Common.0x6FD912D0 (#10472)
int __stdcall STATLIST_GetOwnerGUID(D2StatListStrc* pStatList);
//D2Common.0x6FDB7280 (#11304)
int __stdcall STATLIST_GetBaseStatsCount(D2StatListStrc* pStatList);
//D2Common.0x6FDB72A0 (#11305)
int __stdcall STATLIST_GetFullStatsCountFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB72C0 (#10478)
void __stdcall STATLIST_SetState(D2StatListStrc* pStatList, int nState);
//D2Common.0x6FDB72E0 (#10479)
int __stdcall STATLIST_GetState(D2StatListStrc* pStatList);
//D2Common.0x6FDB7300 (#10528)
void __stdcall STATLIST_SetExpireFrame(D2StatListStrc* pStatList, int nExpireFrame);
//D2Common.0x6FDB7320 (#10529)
int __stdcall STATLIST_GetExpireFrame(D2StatListStrc* pStatList);
//D2Common.0x6FDB7340 (#10475)
void __stdcall D2COMMON_10475_PostStatToStatList(D2UnitStrc* pUnit, D2StatListStrc* pStatList, BOOL bResetFlag);
//D2Common.0x6FDB7560 (#10464)
void __stdcall STATLIST_AddStat(D2StatListStrc* pStatList, int nStatId, int nValue, WORD nLayer);
//D2Common.0x6FDB7690
void __fastcall sub_6FDB7690(D2StatListExStrc* pStatListEx, int nLayer_StatId);
//D2Common.0x6FDB77B0 (#10463)
BOOL __stdcall STATLIST_SetStat(D2StatListStrc* pStatList, int nStatId, int nValue, WORD nLayer);
//D2Common.0x6FDB7910 (#10465)
void __stdcall STATLIST_SetStatIfListIsValid(D2StatListStrc* pStatList, int nStatId, int nValue, WORD nLayer);
//D2Common.0x6FDB7930 (#11294)
BOOL __stdcall D2Common_11294(D2StatListStrc* pStatList, int nStatId, int nValue, WORD nLayer, D2UnitStrc* pUnit);
//D2Common.0x6FDB7A90 (#11295)
void __stdcall D2Common_11295(D2StatListStrc* pStatList, int nStatId, int nValue, WORD nLayer, D2UnitStrc* pUnit);
//D2Common.0x6FDB7AB0 (#10517)
void __stdcall STATLIST_SetUnitStat(D2UnitStrc* pUnit, int nStatId, int nValue, WORD nLayer);
//D2Common.0x6FDB7B00 (#10518)
void __stdcall STATLIST_AddUnitStat(D2UnitStrc* pUnit, int nStatId, int nValue, WORD nLayer);
//D2Common.0x6FDB7B30 (#10521)
int __stdcall STATLIST_GetUnitBaseStat(D2UnitStrc* pUnit, int nStatId, WORD nLayer);
//D2Common.0x6FDB7C30 (#10519)
int __stdcall STATLIST_GetUnitStat(D2UnitStrc* pUnit, int nStatId, WORD nLayer);
//D2Common.0x(6FDB7D40 (#10466)
int __stdcall STATLIST_GetStatValue(D2StatListStrc* pStatList, int nStatId, WORD nLayer);
//D2Common.0x6FDB7E30 (#10520)
int __stdcall STATLIST_GetUnitStatSigned(D2UnitStrc* pUnit, int nStatId, WORD nLayer);
//D2Common.0x6FDB7F40 (#10522)
int __stdcall STATLIST_GetUnitStatBonus(D2UnitStrc* pUnit, int nStatId, WORD nLayer);
//D2Common.0x6FDB80C0 (#10515)
void __stdcall D2Common_10515(D2UnitStrc* pUnit);
//D2Common.0x6FDB8120 (#10467)
int __stdcall D2Common_10467(D2StatListStrc* pStatList, int nStat);
//D2Common.0x6FDB8150 (#10468)
void __stdcall STATLIST_RemoveAllStats(D2StatListStrc* pStatList);
//D2Common.0x6FDB8190
D2StatListStrc* __stdcall D2Common_GetStateFromStatListEx_6FDB8190(D2StatListExStrc* pStatListEx, int nStateId);
//D2Common.0x6FDB81E0 (#10480)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitAndState(D2UnitStrc* pUnit, int nState);
//D2Common.0x6FDB8200 (#10482)
D2StatListStrc* __stdcall STATLIST_GetStatListFromFlag(D2StatListStrc* pStatList, int nFlag);
//D2Common.0x6FDB8230 (#10481)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitAndFlag(D2UnitStrc* pUnit, int nFlag);
//D2Common.0x6FDB8270 (#10483)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitStateOrFlag(D2UnitStrc* pUnit, int nState, int nFlag);
//D2Common.0x6FDB82C0 (#10484)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitStateAndFlag(D2UnitStrc* pUnit, int nState, int nFlag);
//D2Common.0x6FDB8310 (#10523)
void __stdcall STATLIST_MergeStatLists(D2UnitStrc* pTarget, D2UnitStrc* pUnit, BOOL bType);
//D2Common.0x6FDB83A0 (#10535)
D2UnitStrc* __stdcall STATLIST_GetOwner(D2UnitStrc* pUnit, BOOL* pDynamic);
//D2Common.0x6FDB8420 (#10512)
void __stdcall D2Common_10512(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int nStatId, void(__fastcall* pfCallback)(D2UnitStrc*, int, int, D2UnitStrc*));
//D2Common.0x6FDB84E0 (#10513)
void __stdcall D2Common_10513(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, void(__fastcall* pfCallback)(D2UnitStrc*, int, int, D2UnitStrc*));
//D2Common.0x6FDB85D0 (#10511)
void __stdcall STATLIST_FreeModStats(D2UnitStrc* pUnit);
//D2Common.0x6FDB8620 (#10562)
int __stdcall STATLIST_GetUnitAlignment(D2UnitStrc* pUnit);
//D2Common.0x6FDB8750 (#10534)
void __stdcall D2Common_10534(D2UnitStrc* pUnit);
//D2Common.0x6FDB8770 (#10530)
BOOL __stdcall D2COMMON_10530_D2CheckStatlistFlagDMGRed(D2UnitStrc* pUnit);
//D2Common.0x6FDB87A0 (#10532)
int __stdcall D2Common_10532(D2StatListExStrc* pStatListEx, int nStatId);
//D2Common.0x6FDB8890 (#10533)
void __stdcall D2Common_10533(D2UnitStrc* pUnit);
//D2Common.0x6FDB8900
void __stdcall D2Common_STATES_ToggleState_6FDB8900(D2UnitStrc* pUnit, int nState, BOOL bSet);
//D2Common.0x6FDB8A90
DWORD* __stdcall D2COMMON_STATES_GetStatFlags_6FDB8A90(D2UnitStrc* pUnit);
//D2Common.0x6FDB8AC0
DWORD* __stdcall D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(D2UnitStrc* pUnit);
//D2Common.0x6FDB8B10 (#10516)
void __stdcall D2Common_10516(D2UnitStrc* pUnit, int nFrame);
//D2Common.0x6FDB8BA0 (#11268)
int __stdcall STATLIST_GetFullStatsDataFromUnit(D2UnitStrc* pUnit, D2StatStrc* pStatBuffer, int nBufferSize);
//D2Common.0x6FDB8C00 (#11243)
int __stdcall STATLIST_GetBaseStatsData(D2StatListExStrc* pStatListEx, D2StatStrc* pStatBuffer, int nBufferSize);
//D2Common.0x6FDB8C50 (#10573)
void __stdcall STATLIST_MergeBaseStats(D2StatListStrc* pTargetStatList, D2StatListStrc* pSourceStatlist);
//D2Common.0x6FDB8CA0 (#10477)
void __stdcall STATLIST_SetStatRemoveCallback(D2StatListStrc* pStatList, void* pfStatRemove);
//D2Common.0x6FDB8CC0 (#10469)
void __stdcall D2Common_10469(D2UnitStrc* pUnit);
//D2Common.0x6FDB8D30 (#10514)
void __stdcall D2Common_10514(D2UnitStrc* pUnit);
//D2Common.0x6FDB8EB0 (#10574)
BOOL __stdcall D2Common_10574(D2UnitStrc* pUnit, int nStateId, BOOL bSet);
//D2Common.0x6FDB8F30 (#10525)
void __stdcall D2Common_10525(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//D2Common.0x6FDB91C0 (#10474)
void __stdcall D2Common_10474(D2UnitStrc* pUnused, D2StatListStrc* pStatList);
//D2Common.0x6FDB91D0 (#10564)
int __stdcall STATLIST_GetMaxLifeFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB92C0 (#10565)
int __stdcall STATLIST_GetMaxManaFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB93B0 (#10566)
int __stdcall STATLIST_GetMaxStaminaFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB94A0 (#10567)
int __stdcall STATLIST_GetMaxDurabilityFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB95D0 (#10568)
int __stdcall STATLIST_GetMaxDamageFromUnit(D2UnitStrc* pUnit, BOOL b2Handed);
//D2Common.0x6FDB96F0 (#10569)
int __stdcall STATLIST_GetMinDamageFromUnit(D2UnitStrc* pUnit, BOOL b2Handed);
//D2Common.0x6FDB9810 (#10570)
int __stdcall STATLIST_GetMaxThrowDamageFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB9900 (#10571)
int __stdcall STATLIST_GetMinThrowDamageFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB99F0 (#10572)
int __stdcall STATLIST_GetDefenseFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB9AE0 (#10524)
void __stdcall STATLIST_ExpireUnitStatlist(D2UnitStrc* pUnused, D2UnitStrc* pUnit);
//D2Common.0x6FDB9B00 (#10531)
int __stdcall D2Common_10531_SetStatInStatListLayer0(D2StatListStrc* pStatList, int nStatId, int nValue, int nUnused);
//D2Common.0x6FDB9B10 (#11248)
int __stdcall D2Common_11248(D2UnitStrc* pUnused, D2UnitStrc* pUnit, int nStatId);
//D2Common.0x6FDA9E60 (#11264)
void __stdcall STATLIST_SetSkillId(D2StatListStrc* pStatList, int nSkillId);
//D2Common.0x6FDB9C10 (#11265)
int __stdcall STATLIST_GetSkillId(D2StatListStrc* pStatList);
//D2Common.0x6FDB9C20 (#11266)
void __stdcall STATLIST_SetSkillLevel(D2StatListStrc* pStatList, int nSkillLevel);
//D2Common.0x6FDA9E70 (#11267)
int __stdcall STATLIST_GetSkillLevel(D2StatListStrc* pStatList);
//D2Common.0x6FDB9C30 (#11269)
int __stdcall D2COMMON_11269_CopyStats(D2StatListExStrc* pStatListEx, int nStatId, D2StatStrc* pBuffer, int nBufferSize);
//D2Common.0x6FDB9C50
int __fastcall D2Common_CopyStats_6FDB9C50(D2StatListExStrc* pStatListEx, int nStatId, D2StatStrc* pBuffer, int nBufferSize);
//D2Common.0x6FDB9D20 (#11270)
int __stdcall D2Common_11270(D2UnitStrc* pUnit, int nStatId, D2StatStrc* pBuffer, int nBufferSize);
//D2Common.0x6FDB9D60 (#11273)
int __stdcall D2Common_11273(D2UnitStrc* pUnit, int a2);
//D2Common.0x6FDB9D90 (#11274)
BOOL __stdcall D2Common_11274(D2UnitStrc* pTarget, D2UnitStrc* pUnit);
//D2Common.0x6FDB9E60 (#11275)
BOOL __stdcall D2Common_11275(D2UnitStrc* pTarget, D2UnitStrc* pUnit);
