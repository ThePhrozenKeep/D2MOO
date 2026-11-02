#include "UselessOrdinals.h"

#include "D2Skills.h"
#include "D2StatList.h"


//1.10f: D2Common.0x6FD412B0
int __stdcall D2COMMON_10016_Return0()
{
	REMOVE_LATER_Trace("D2COMMON_10016_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDA5250
void __stdcall D2COMMON_10019_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_10019_Return: Useless");
}

//1.10f: D2Common.0x6FDA5260
void __stdcall D2COMMON_10139_Return()
{
	REMOVE_LATER_Trace("D2COMMON_10139_Return: Useless");
}

//1.10f: D2Common.0x6FDA5260
void __stdcall D2COMMON_10140_Return()
{
	REMOVE_LATER_Trace("D2COMMON_10140_Return: Useless");
}

//1.10f: D2Common.0x6FDA5260
void __stdcall D2COMMON_10238_Return()
{
	REMOVE_LATER_Trace("D2COMMON_10238_Return: Useless");
}

//1.10f: D2Common.0x6FD9FB40
int __stdcall D2COMMON_10239_Return0(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10239_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDA5250
//1.14d: 0x0063BD00
void __stdcall D2COMMON_10251_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_10251_Return: Useless");
}

//1.10f: D2Common.0x6FDA6400
//1.14d: 0x0063BE30
int __stdcall D2COMMON_10254_Return0(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10254_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDA6400
//1.14d: 0x0063C550
void __stdcall D2COMMON_10267_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10267_Return: Useless");
}

//1.10f: D2Common.0x6FDB2E70
int __stdcall D2COMMON_10301_Return0(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_10301_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDA6400
void __stdcall D2COMMON_10303_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10303_Return: Useless");
}

//1.10f: D2Common.0x6FDA6400
void __stdcall D2COMMON_10308_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10308_Return: Useless");
}

//1.10f: D2Common.0x6FDB2E70
int __stdcall D2COMMON_10309_Return0(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_10309_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDA6400
void __stdcall D2COMMON_10441_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10441_Return: Useless");
}

//1.10f: D2Common.0x6FDB7320
//1.14d: 0x00626090
int __stdcall D2COMMON_10473(D2StatListStrc* pStatList)
{
	return STATLIST_GetExpireFrame(pStatList);
}

//1.10f: D2Common.0x6FDB7300
//1.14d: 0x006260B0
void __stdcall D2COMMON_10476(D2StatListStrc* pStatList, int nExpireFrame)
{
	return STATLIST_SetExpireFrame(pStatList, nExpireFrame);
}

//1.10f: D2Common.0x6FD9FB40
int __stdcall D2COMMON_10661_Return0(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10661_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FD9FB40
int __stdcall D2COMMON_10835_Return0(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_10835_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDB40F0
void __stdcall D2COMMON_10856_Return(int a1, int a2, int a3)
{
	REMOVE_LATER_Trace("D2COMMON_10856_Return: Useless");
}

//1.10f: D2Common.0x6FDAFCA0
int __stdcall D2COMMON_10943_Return1(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_10943_Return1: Useless");
	return 1;
}

//1.10f: D2Common.0x6FDB2E70
int __stdcall D2COMMON_11009_Return0(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11009_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDB3480
//1.14d: 0x00645590
BOOL __stdcall D2COMMON_11018(D2UnitStrc* pUnit)
{
	REMOVE_LATER_Trace("D2COMMON_11018: Duplicate of D2Common_11020");
	return D2Common_11020(pUnit);
}

//1.10f: D2Common.0x6FDB3520
//1.14d: 0x006455E0
BOOL __stdcall D2COMMON_11019(D2UnitStrc* pUnit)
{
	REMOVE_LATER_Trace("D2COMMON_11019: Duplicate of SKILLS_CanUnitCorpseBeSelected");
	return SKILLS_CanUnitCorpseBeSelected(pUnit);
}

//1.10f: D2Common.0x6FDB3520
//1.14d: 0x00645680
BOOL __stdcall D2COMMON_11021(D2UnitStrc* pUnit)
{
	REMOVE_LATER_Trace("D2COMMON_11021: Duplicate of SKILLS_CanUnitCorpseBeSelected");
	return SKILLS_CanUnitCorpseBeSelected(pUnit);
}

//1.10f: D2Common.0x6FDAFCA0
int __stdcall D2COMMON_11048_Return1(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11048_Return1: Useless");
	return 1;
}

//1.10f: D2Common.0x6FDA6400
//1.14d: 0x0063EBC0
void __stdcall D2COMMON_11062_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_11062_Return: Useless");
}

//1.10f: D2Common.0x6FDA6400
void __stdcall D2COMMON_11076_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_11076_Return: Useless");
}

//1.10f: D2Common.0x6FDA6400
//1.14d: 0x0066A220
void __stdcall D2COMMON_11077_Return(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_11077_Return: Useless");
}

//1.10f: D2Common.0x6FD9FB40
int __stdcall D2COMMON_11078_Return0(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_11078_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDA5250
void __stdcall D2COMMON_11080_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11080_Return: Useless");
}

//1.10f: D2Common.0x6FDA5250
void __stdcall D2COMMON_11229_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11229_Return: Useless");
}

//1.10f: D2Common.0x6FDB2E70
int __stdcall D2COMMON_11235_Return0(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11235_Return0: Useless");
	return 0;
}

//1.10f: D2Common.0x6FDB40F0
void __stdcall D2COMMON_11236_Return(int a1, int a2, int a3)
{
	REMOVE_LATER_Trace("D2COMMON_11236_Return: Useless");
}

//1.10f: D2Common.0x6FDB40F0
void __stdcall D2COMMON_11237_Return(int a1, int a2, int a3)
{
	REMOVE_LATER_Trace("D2COMMON_11237_Return: Useless");
}

//1.10f: D2Common.0x6FDB40F0
void __stdcall D2COMMON_11240_Return(int a1, int a2, int a3)
{
	REMOVE_LATER_Trace("D2COMMON_11240_Return: Useless");
}

//1.10f: D2Common.0x6FDA5250
void __stdcall D2COMMON_11241_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11241_Return: Useless");
}

//1.10f: D2Common.0x6FDA5250
void __stdcall D2COMMON_11277_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11277_Return: Useless");
}

//1.10f: D2Common.0x6FDA5250
void __stdcall D2COMMON_11296_Return(int a1)
{
	REMOVE_LATER_Trace("D2COMMON_11296_Return: Useless");
}
