#pragma once

#include <Windows.h>
#include <Safesock.h>

#include "D2Net.h"
#include "Packet.h"

using D2NET_Unk_Callback = void (__stdcall*) (int32_t nClientId, const char* szFile, int32_t nLine);
using D2NET_CLIENT_SendFunctionType = int32_t (__stdcall*) (int32_t nUnused, const uint8_t* pBuffer, int32_t nBufferSize);
using D2NET_SERVER_GetClientGameGUIDFunctionType = int32_t(__stdcall*)(int32_t nClientId);

//D2Net.0x6FC01B30 (#10024)
D2NET_DLL_DECL int32_t __stdcall SERVER_WSAGetLastError();
//D2Net.0x6FC01B60 (#10030)
D2NET_DLL_DECL int32_t __fastcall SERVER_GetServerPacketSize(D2PacketBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize);
//D2Net.0x6FC01E60 (#10031)
D2NET_DLL_DECL int32_t __fastcall SERVER_GetClientPacketSize(D2PacketBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize);
//D2Net.0x6FC01FA0 (#10039)
D2NET_DLL_DECL D2NET_CLIENT_SendFunctionType __stdcall D2NET_10039();
//D2Net.0x6FC01FB0 (#10040)
D2NET_DLL_DECL D2NET_SERVER_GetClientGameGUIDFunctionType __stdcall D2NET_10040();
//D2Net.0x6FC01FC0
int32_t __fastcall SERVER_ReadPacketFromBufferCallback(QServer* nUnused, D2PacketBufferStrc* pPacketBuffer, int32_t nBufferSize);
//D2Net.0x6FC01FE0
int32_t __fastcall SERVER_ValidateClientPacket(D2PacketBufferStrc* pPacketBuffer, uint32_t nBufferSize, int32_t* a3, int32_t* a4, int32_t* a5, int32_t* a6, int32_t nUnused1, int32_t nUnused2);
//D2Net.0x6FC020B0
int32_t __fastcall sub_6FC020B0(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4);
//D2Net.0x6FC020E0
int32_t __fastcall sub_6FC020E0(int32_t a1, int32_t a2, int32_t a3, int32_t a4);
//D2Net.0x6FC02110
int32_t __fastcall sub_6FC02110();
//D2Net.0x6FC02130 (#10002)
D2NET_DLL_DECL int32_t __stdcall SERVER_WaitForSingleObject(uint32_t dwMilliseconds);
//D2Net.0x6FC02150 (#10003)
D2NET_DLL_DECL void __stdcall SERVER_Initialize(int32_t a1, int32_t a2);
//D2Net.0x6FC02190 (#10035)
D2NET_DLL_DECL int32_t __stdcall D2NET_10035(int32_t nIndex, int32_t nValue);
//D2Net.0x6FC021B0 (#10036)
D2NET_DLL_DECL void __stdcall D2NET_10036(int32_t a1, int32_t a2);
//D2Net.0x6FC021D0 (#10026)
D2NET_DLL_DECL void __stdcall SERVER_SetMaxClientsPerGame(int32_t nMaxClients);
//D2Net.0x6FC021F0 (#10027)
D2NET_DLL_DECL int32_t __stdcall D2NET_10027();
//D2Net.0x6FC02200 (#10023)
D2NET_DLL_DECL void __stdcall SERVER_SetHackListEnabled(BOOL bEnabled);
//D2Net.0x6FC02220 (#10004)
D2NET_DLL_DECL void __stdcall SERVER_Release();
//D2Net.0x6FC02250 (#10010)
D2NET_DLL_DECL int32_t __stdcall SERVER_ReadFromMessageList1(uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC02270 (#10011)
D2NET_DLL_DECL int32_t __stdcall SERVER_ReadFromMessageList0(uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC02290 (#10012)
D2NET_DLL_DECL int32_t __stdcall SERVER_ReadFromMessageList2(uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC022B0 (#10006)
D2NET_DLL_DECL uint32_t __stdcall D2NET_10006(int8_t a1, int32_t nClientId, void* pBufferArg, uint32_t nBufferSize);
//D2Net.0x6FC02410 (#10014)
D2NET_DLL_DECL void __stdcall SERVER_GetIpAddressStringFromClientId(int32_t nClientId, char* szBuffer, int32_t nBufferSize);
//D2Net.0x6FC02430 (#10038)
D2NET_DLL_DECL int32_t __stdcall SERVER_GetIpAddressFromClientId(int32_t nClientId);
//D2Net.0x6FC02450 (#10037)
D2NET_DLL_DECL SOCKET __stdcall SERVER_GetSocketFromClientId(int32_t nClientId);
//D2Net.0x6FC02470 (#10015)
D2NET_DLL_DECL void __stdcall D2NET_10015(int32_t nClientId, const char* szFile, int32_t nLine);
//D2Net.0x6FC02490 (#10032)
D2NET_DLL_DECL void __stdcall D2NET_10032(int32_t nClientId, const char* szFile, int32_t nLine);
//D2Net.0x6FC024B0 (#10033)
D2NET_DLL_DECL int32_t __stdcall D2NET_10033(int32_t a1, int32_t a2, int32_t a3);
//D2Net.0x6FC024D0 (#10034)
D2NET_DLL_DECL int32_t __stdcall D2NET_10034(int32_t nClientId, int32_t a2, int32_t a3);
//D2Net.0x6FC024F0 (#10016)
D2NET_DLL_DECL void __stdcall D2NET_10016(int32_t nClientId);
//D2Net.0x6FC02510 (#10018)
D2NET_DLL_DECL void __stdcall D2NET_10018(int32_t a1);
//D2Net.0x6FC02530 (#10019)
D2NET_DLL_DECL int32_t __stdcall D2NET_10019(D2NET_Unk_Callback pfCallback);
//D2Net.0x6FC02550 (#10020)
D2NET_DLL_DECL int32_t __stdcall SERVER_SetClientGameGUID(int32_t nClientId, int32_t dwGameGuid);
//D2Net.0x6FC02580 (#10021)
D2NET_DLL_DECL int32_t __stdcall SERVER_GetClientGameGUID(int32_t nClientId);
//D2Net.0x6FC025A0
int32_t __fastcall SERVER_EnqueuePacketToMessageList(const uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC025F0 (#10022)
D2NET_DLL_DECL int32_t __stdcall D2NET_10022(uint32_t dwMilliseconds);
//D2Net.0x6FC02610 (#10028)
D2NET_DLL_DECL int32_t __stdcall D2NET_10028_Return();
//D2Net.0x6FC02620 (#10029)
D2NET_DLL_DECL int32_t __stdcall D2NET_10029_Return(int32_t nUnused);
