#pragma once


#include <cstdint>

#include <Windows.h>
#include <WinSock2.h>

#include <Fog.h>

#include <Packet.h>


using D2NET_RemoveClientCallback = void (__stdcall*) (int32_t nClientId, const char* szFile, int32_t nLine);
using D2NET_CLIENT_SendFunctionType = int32_t (__stdcall*) (int32_t nUnused, const uint8_t* pBuffer, int32_t nBufferSize);
using D2NET_SERVER_GetClientGameGUIDFunctionType = int32_t(__stdcall*)(int32_t nClientId);


// nSendMode of SERVER_SendPacketToClient
constexpr int8_t D2NET_SEND_SYSTEM = 0;			// Compressed, except for the compression info packet itself
constexpr int8_t D2NET_SEND_GAME = 1;			// Compressed
constexpr int8_t D2NET_SEND_UNCOMPRESSED = 2;	// Sent as is (used for admin packets)

// Queues in which the server sorts received client packets
constexpr int32_t SERVER_PACKET_QUEUE_SYSTEM = 0;	// Packets 0x66-0x6F (1.14d: 0x67-0x70)
constexpr int32_t SERVER_PACKET_QUEUE_GAME = 1;		// Packets < 0x66 (1.14d: < 0x67, the Warden reply included)
constexpr int32_t SERVER_PACKET_QUEUE_ADMIN = 2;	// Packet 0xFF


//D2Net.0x6FC01B30 (#10024)
D2NET_DLL_DECL int32_t __stdcall SERVER_WSAGetLastError();
//D2Net.0x6FC01B60 (#10030)
//1.14d: 0x0052B920
D2NET_DLL_DECL int32_t __fastcall SERVER_GetServerPacketSize(D2NetReceiveBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize);
//D2Net.0x6FC01E60 (#10031)
D2NET_DLL_DECL int32_t __fastcall SERVER_GetClientPacketSize(D2NetReceiveBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize);
//D2Net.0x6FC01FA0 (#10039)
D2NET_DLL_DECL D2NET_CLIENT_SendFunctionType __stdcall SERVER_GetClientSendFunction();
//D2Net.0x6FC01FB0 (#10040)
D2NET_DLL_DECL D2NET_SERVER_GetClientGameGUIDFunctionType __stdcall SERVER_GetClientGameGUIDFunction();
//D2Net.0x6FC01FC0
int32_t __fastcall SERVER_SendToLocalClientCallback(QServer* pUnused, D2NetReceiveBufferStrc* pPacketBuffer, int32_t nBufferSize);
//D2Net.0x6FC01FE0
int32_t __fastcall SERVER_ValidateClientPacket(D2NetReceiveBufferStrc* pPacketBuffer, uint32_t nBufferSize, int32_t* pPacketSize, int32_t* a4, int32_t* pPacketQueue, int32_t* a6, int32_t nUnused1, int32_t nUnused2);
//D2Net.0x6FC020B0
int32_t __fastcall SERVER_OnClientConnectedCallback(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4);
//D2Net.0x6FC020E0
int32_t __fastcall SERVER_OnClientDisconnectedCallback(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4);
//D2Net.0x6FC02110
int32_t __fastcall SERVER_DisableLocalClientCompression();
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
D2NET_DLL_DECL int32_t __stdcall SERVER_ReadGamePacket(uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC02270 (#10011)
D2NET_DLL_DECL int32_t __stdcall SERVER_ReadSystemPacket(uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC02290 (#10012)
D2NET_DLL_DECL int32_t __stdcall SERVER_ReadAdminPacket(uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC022B0 (#10006)
D2NET_DLL_DECL uint32_t __stdcall SERVER_SendPacketToClient(int8_t nSendMode, int32_t nClientId, void* pPacketArg, uint32_t nPacketSize);
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
D2NET_DLL_DECL int32_t __stdcall SERVER_SetRemoveClientCallback(D2NET_RemoveClientCallback pfRemoveClient);
//D2Net.0x6FC02550 (#10020)
D2NET_DLL_DECL int32_t __stdcall SERVER_SetClientGameGUID(int32_t nClientId, int32_t dwGameGuid);
//D2Net.0x6FC02580 (#10021)
D2NET_DLL_DECL int32_t __stdcall SERVER_GetClientGameGUID(int32_t nClientId);
//D2Net.0x6FC025A0
int32_t __fastcall SERVER_EnqueueLocalClientPacket(const uint8_t* pPacket, uint16_t nPacketSize);
//D2Net.0x6FC025F0 (#10022)
D2NET_DLL_DECL int32_t __stdcall D2NET_10022(uint32_t dwMilliseconds);
//D2Net.0x6FC02610 (#10028)
D2NET_DLL_DECL int32_t __stdcall D2NET_10028_Return();
//D2Net.0x6FC02620 (#10029)
D2NET_DLL_DECL int32_t __stdcall D2NET_10029_Return(int32_t nUnused);
