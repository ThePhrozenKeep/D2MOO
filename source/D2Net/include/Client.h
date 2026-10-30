#pragma once


#include <cstdint>

#include <Windows.h>

#include <Packet.h>


// Return values of CLIENT_GetConnectionStatus
constexpr int32_t CLIENT_CONNECTION_FAILED = 0;
constexpr int32_t CLIENT_CONNECTION_PENDING = 1;
constexpr int32_t CLIENT_CONNECTION_ESTABLISHED = 2;


//D2Net.0x6FC01000 (#10017)
//1.14d: 0x0052A640
D2NET_DLL_DECL int32_t __stdcall CLIENT_IsConnected();
//D2Net.0x6FC01040
//1.14d: 0x0052A680
DWORD __stdcall CLIENT_ConnectToHost(void* szHostAddress);
//D2Net.0x6FC01120 (#10025)
//1.14d: 0x0052B050
D2NET_DLL_DECL int32_t __stdcall CLIENT_GetConnectionStatus();
//D2Net.0x6FC011B0 (#10000)
//1.14d: 0x0052A750
D2NET_DLL_DECL int32_t __stdcall CLIENT_Initialize(int32_t nConnectionMode, const char* szHostAddress);
//D2Net.0x6FC01240 (#10001)
//1.14d: 0x0052A7E0
D2NET_DLL_DECL void __stdcall CLIENT_Release();
//D2Net.0x6FC01300 (#10007)
//1.14d: 0x0052A890
D2NET_DLL_DECL int32_t __stdcall CLIENT_DequeueGamePacket(uint8_t* pBuffer, uint32_t nBufferSize);
//D2Net.0x6FC01310 (#10008)
//1.14d: 0x0052A8B0
D2NET_DLL_DECL int32_t __stdcall CLIENT_DequeueSystemPacket(uint8_t* pBuffer, uint32_t nBufferSize);
//D2Net.0x6FC01320
//1.14d: 0x0052AB00
DWORD __stdcall CLIENT_ReceiveThreadProc(void* pUnused);
//D2Net.0x6FC015C0
//1.14d: 0x0052A8D0
int32_t __stdcall CLIENT_ParseReceiveBuffer();
//D2Net.0x6FC01760 (#10005)
//1.14d: 0x0052AE50
D2NET_DLL_DECL int32_t __stdcall CLIENT_Send(int32_t nUnused, const uint8_t* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC01810
//1.14d: 0x0052AEB0
void __fastcall CLIENT_EnqueuePacketsFromLocalServer(D2NetReceiveBufferStrc* pBuffer, int32_t nBufferSize);
//D2Net.0x6FC019A0 (#10013)
//1.14d: 0x0052AFE0
D2NET_DLL_DECL void __stdcall CLIENT_GetLocalIpAddressString(char* szBuffer);
