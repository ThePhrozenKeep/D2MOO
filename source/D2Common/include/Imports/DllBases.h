#pragma once

#include <Windows.h>
extern HMODULE DLLBASE_D2CMP;
extern HMODULE DLLBASE_D2LANG;
extern HMODULE DLLBASE_FOG;

HMODULE delayedD2CMPDllBaseGet();
HMODULE delayedD2LANGDllBaseGet();
HMODULE delayedFOGDllBaseGet();
