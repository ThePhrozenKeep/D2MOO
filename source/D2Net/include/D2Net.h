#pragma once


#include <cstdint>

#include <Windows.h>

#include <Packet.h>


//1.10f Image Base: 0x6FC00000

#ifdef D2_VERSION_110F
constexpr int D2NetImageBase = 0x6FC00000;
#endif


constexpr int32_t GAME_PORT = 4000;

enum D2NetConnectionMode : int32_t
{
	D2NET_CONNECTION_REMOTE = 0,				// Client and server talk over TCP
	D2NET_CONNECTION_LOCAL = 1,					// Client and server live in the same process (single player)
	D2NET_CONNECTION_LOCAL_SIMULATED_LAG = 2,	// Same as local, but packets are held back to simulate latency
};

constexpr uint32_t SIMULATED_LAG_MS = 500;


//D2Net.0x6FC01A00
BOOL __stdcall NET_IsLocalConnection();
//D2Net.0x6FC01A20
void __fastcall NET_SetConnectionMode(int32_t nConnectionMode);
//D2Net.0x6FC01A30
int32_t __fastcall NET_DequeueFirstPacketFromList(CRITICAL_SECTION* pCriticalSection, D2NetQueuedPacketStrc** ppPacketList, uint8_t* pBuffer, uint32_t nBufferSize);
//D2Net.0x6FC01AE0
int32_t __fastcall NET_FreePacketList(CRITICAL_SECTION* pCriticalSection, D2NetQueuedPacketStrc** ppPacketList);
