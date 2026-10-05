#pragma once

#include <D2Dll.h>
#include <D2StdInt.h>

//1.10f Image Base: 0x6F980000

#ifdef D2_VERSION_110F
D2_CONSTEXPR int D2SoundImageBase = 0x6F980000;
#endif

#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
D2FUNC_DLL_VOID(D2SOUND, OpenSoundSystem, __fastcall, (BOOL bExpansion, BOOL bSoundBackground), 0x0000/*TODO*/)	//D2Sound.#10002
#else
D2FUNC_DLL_VOID(D2SOUND, OpenSoundSystem, __fastcall, (BOOL bExpansion), 0x10E0)					//D2Sound.#10000
#endif
D2FUNC_DLL_VOID(D2SOUND, CloseSoundSystem, __fastcall, (), 0x21C0)									//D2Sound.#10001
D2FUNC_DLL_VOID(D2SOUND, 10034, __fastcall, (int), 0x4D60)											//D2Sound.#10034
D2FUNC_DLL_VOID(D2SOUND, 10039, __fastcall, (const char*), 0x2800)									//D2Sound.#10039
D2FUNC_DLL(D2SOUND, 10048, void*, __fastcall, (), 0x2B30)											//D2Sound.#10048
D2FUNC_DLL_VOID(D2SOUND, 10049, __fastcall, (void*), 0x2B40)										//D2Sound.#10049
D2FUNC_DLL_VOID(D2SOUND, 10065, __fastcall, (), 0x49D0)											//D2Sound.#10065
D2FUNC_DLL(D2SOUND, 10066, int, __fastcall, (), 0x2990)												//D2Sound.#10066
D2FUNC_DLL(D2SOUND, 10069, int, __fastcall, (), 0x2BC0)												//D2Sound.#10069
D2FUNC_DLL_VOID(D2SOUND, 10070, __fastcall, (int), 0x2BD0)											//D2Sound.#10070
