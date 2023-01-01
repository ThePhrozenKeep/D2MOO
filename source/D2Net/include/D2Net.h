#pragma once

#include <Windows.h>

#include "Packet.h"


constexpr int32_t GAME_PORT = 4000;


//D2Net.0x6FC01A00
int32_t __stdcall sub_6FC01A00();
//D2Net.0x6FC01A20
void __fastcall sub_6FC01A20(int32_t a1);
//D2Net.0x6FC01A30
int32_t __fastcall NET_DequeueFirstPacketFromList(CRITICAL_SECTION* pCriticalSection, D2PacketStrc** ppPacketList, uint8_t* pBuffer, uint32_t nBufferSize);
//D2Net.0x6FC01AE0
int32_t __fastcall NET_FreePacketList(CRITICAL_SECTION* pCriticalSection, D2PacketStrc** ppPacketList);
