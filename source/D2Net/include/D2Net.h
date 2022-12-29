#pragma once

#include <D2Dll.h>
#include <D2BasicTypes.h>

#ifdef D2NET_IMPL
#define D2NET_DLL_DECL 
#else
#define D2NET_DLL_DECL __declspec( dllimport )
#endif

// 1.10f Image Base: 0x6FC00000

D2FUNC_DLL(D2NET, 10006, BOOL, __stdcall, (char a1, int nClientId, void* pBuffer, unsigned int nBufferLen), 0x22B0)			//D2Net.#10006
D2FUNC_DLL(D2NET, 10010, size_t, __stdcall, (void* pBuffer, unsigned int nBufferLen), 0x2250)								//D2Net.#10010
D2FUNC_DLL(D2NET, 10011, size_t, __stdcall, (void* pBuffer, unsigned int nBufferLen), 0x2270)								//D2Net.#10011
D2FUNC_DLL(D2NET, 10012, size_t, __stdcall, (void* pBuffer, unsigned int nBufferLen), 0x2290)								//D2Net.#10012
D2FUNC_DLL(D2NET, GetClientIPAddress, size_t, __stdcall, (int nClientId, char* szIPAddress, int nBufferSize), 0x2410)		//D2Net.#10014
D2FUNC_DLL(D2NET, 10015, void, __stdcall, (int nClientId, const char* szFile, int32_t nLine), 0x2470)											//D2Net.#10015
D2FUNC_DLL(D2NET, 10016, void, __stdcall, (int nClientId), 0x24F0)															//D2Net.#10016
using D2NET_Unk_Callback = void __stdcall (int32_t nClientId, const char* szFile, int32_t nLine);
D2FUNC_DLL(D2NET, 10019, void, __stdcall, (D2NET_Unk_Callback pUnkCallback), 0x2530)															//D2Net.#10019
D2FUNC_DLL(D2NET, SetClientGameGUID, void, __stdcall, (int nClientId, int dwGameGuid), 0x2550)								//D2Net.#10020
D2FUNC_DLL(D2NET, GetClientGameGUID, int, __stdcall, (int nClientId), 0x2580)												//D2Net.#10021
D2FUNC_DLL(D2NET, WSAGetLastError, int, __stdcall, (), 0x1B30)																//D2Net.#10024

