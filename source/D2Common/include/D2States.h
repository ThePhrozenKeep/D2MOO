#pragma once

#include "CommonDefinitions.h"


//D2Common.0x6FDB4560 (#10486)
void __stdcall STATES_ToggleState(D2UnitStrc* pUnit, int nState, BOOL bSet);
//D2Common.0x6FDB45A0 (#10487)
BOOL __stdcall STATES_CheckState(D2UnitStrc* pUnit, int nState);
//D2Common.0x6FDB45F0 (#10488)
void __stdcall STATES_ToggleGfxStateFlag(D2UnitStrc* pUnit, int nState, BOOL bSet);
//D2Common.0x6FDB4670 (#10489)
BOOL __stdcall STATES_CheckGfxStateFlag(D2UnitStrc* pUnit, int nState);
//D2Common.0x6FDB46C0 (#10491)
void __stdcall STATES_ClearGfxStateFlags(D2UnitStrc* pUnit);
//D2Common.0x6FDB4710 (#10490)
BOOL __stdcall STATES_IsAnyGfxStateFlagSet(D2UnitStrc* pUnit);
//D2Common.0x6FDB4760 (#10492)
DWORD* __stdcall STATES_GetGfxStateFlags(D2UnitStrc* pUnit);
//D2Common.0x6FDB4790 (#10493)
void __stdcall STATES_UpdateStayDeathFlags(D2UnitStrc* pUnit, BOOL bIsBoss);
//D2Common.0x6FDB4900 (#10495)
int __fastcall STATES_GetNumberOfStateFlags();
//D2Common.0x6FDB4920 (#10496)
BOOL __stdcall STATES_CheckStateMaskAuraByStateId(int nState);
//D2Common.0x6FDB4960 (#10503)
BOOL __stdcall STATES_CheckStateMaskAuraOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB49D0 (#10500)
BOOL __stdcall STATES_CheckStateMaskPgsvByStateId(int nState);
//D2Common.0x6FDB4A10 (#11234)
BOOL __stdcall STATES_CheckStateMaskPgsvOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4A80 (#10501)
void __stdcall STATES_UpdatePgsvFlags(D2UnitStrc* pUnit);
//D2Common.0x6FDB4B20 (#10558)
BOOL __stdcall STATES_CheckStateMaskHideByStateId(int nState);
//D2Common.0x6FDB4B50 (#10557)
BOOL __stdcall STATES_CheckStateMaskHideOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4BC0 (#10502)
BOOL __stdcall STATES_CheckStateMaskActiveOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4C30 (#11272)
BOOL __stdcall STATES_CheckStateMaskRemHitOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4CA0 (#10538)
BOOL __stdcall STATES_CheckStateMaskDamBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4D10 (#10545)
BOOL __stdcall STATES_CheckStateMaskDamRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4D80 (#10537)
BOOL __stdcall STATES_CheckStateMaskAttBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4DF0 (#10544)
BOOL __stdcall STATES_CheckStateMaskAttRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4E60 (#10551)
BOOL __stdcall STATES_IsUnitShapeShifted(D2UnitStrc* pUnit);
//D2Common.0x6FDB4E80 (#10497)
BOOL __stdcall STATES_CheckStateMaskCurseByStateId(int nState);
//D2Common.0x6FDB4EC0 (#10498)
BOOL __stdcall STATES_CheckStateMaskCurableByStateId(int nState);
//D2Common.0x6FDB4F00 (#10554)
BOOL __stdcall STATES_CheckStateMaskStayDeathOnUnitByStateId(D2UnitStrc* pUnit, int nState);
//D2Common.0x6FDB4F60
BOOL __fastcall STATES_CheckStateMaskRestrictOnUnit(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB4FC0 (#10536)
BOOL __stdcall STATES_CheckStateMaskBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5030 (#10539)
BOOL __stdcall STATES_CheckStateMaskArmBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB50A0 (#10540)
BOOL __stdcall STATES_CheckStateMaskRFBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5110 (#10541)
BOOL __stdcall STATES_CheckStateMaskRCBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5180 (#10542)
BOOL __stdcall STATES_CheckStateMaskRLBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB51F0 (#10543)
BOOL __stdcall STATES_CheckStateMaskRPBlueOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5260 (#10546)
BOOL __stdcall STATES_CheckStateMaskArmRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB52D0 (#10547)
BOOL __stdcall STATES_CheckStateMaskRFRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5340 (#10548)
BOOL __stdcall STATES_CheckStateMaskRCRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB53B0 (#10549)
BOOL __stdcall STATES_CheckStateMaskRLRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5420 (#10550)
BOOL __stdcall STATES_CheckStateMaskRPRedOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5490 (#10552)
BOOL __stdcall STATES_CheckStateMaskExpOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5500 (#10553)
BOOL __stdcall STATES_CheckStateMaskExpByStateId(int nState);
//D2Common.0x6FDB5540 (#10555)
BOOL __stdcall STATES_CheckStateMaskShatterOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB55B0 (#10556)
BOOL __stdcall STATES_CheckStateMaskShatterByStateId(int nState);
//D2Common.0x6FDB55F0 (#10559)
BOOL __stdcall STATES_CheckStateMaskLifeOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5660 (#10560)
BOOL __stdcall STATES_CheckStateMaskUdeadOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB56D0 (#10561)
BOOL __stdcall STATES_CheckStateMaskGreenOnUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB5740 (#11303)
BOOL __stdcall STATES_CheckStateMaskOnUnit(D2UnitStrc* pUnit, int nStateMask);
