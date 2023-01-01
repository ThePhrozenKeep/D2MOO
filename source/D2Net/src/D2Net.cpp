#include "D2Net.h"

#include <algorithm>

#include <Fog.h>


#pragma warning (disable: 28159)


int32_t dword_6FC0B264;


//D2Net.0x6FC01A00
int32_t __stdcall sub_6FC01A00()
{
	return dword_6FC0B264 == 1 || dword_6FC0B264 == 2;
}

//D2Net.0x6FC01A20
void __fastcall sub_6FC01A20(int32_t a1)
{
	dword_6FC0B264 = a1;
}

//D2Net.0x6FC01A30
int32_t __fastcall NET_DequeueFirstPacketFromList(CRITICAL_SECTION* pCriticalSection, D2PacketStrc** ppPacketList, uint8_t* pBuffer, uint32_t nBufferSize)
{
	EnterCriticalSection(pCriticalSection);

	D2PacketStrc* pPacket = *ppPacketList;
	if (!pPacket || (dword_6FC0B264 == 2 && GetTickCount() - pPacket->dwTickCount < 500))
	{
		LeaveCriticalSection(pCriticalSection);
		return -1;
	}

	memcpy(pBuffer, pPacket, std::min(nBufferSize, MAX_MSG_SIZE));

	const int32_t nPacketSize = pPacket->nPacketSize;

	*ppPacketList = pPacket->pNext;

	FOG_Free(pPacket, __FILE__, __LINE__, 0);

	LeaveCriticalSection(pCriticalSection);
	return nPacketSize;
}

//D2Net.0x6FC01AE0
int32_t __fastcall NET_FreePacketList(CRITICAL_SECTION* pCriticalSection, D2PacketStrc** ppPacketList)
{
	EnterCriticalSection(pCriticalSection);

	for (D2PacketStrc* i = *ppPacketList; *ppPacketList; i = *ppPacketList)
	{
		*ppPacketList = i->pNext;
		FOG_Free(i, __FILE__, __LINE__, 0);
	}

	LeaveCriticalSection(pCriticalSection);
	return 1;
}
