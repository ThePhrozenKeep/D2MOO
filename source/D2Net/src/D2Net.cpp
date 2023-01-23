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
	D2_LOCK(pCriticalSection);

	D2PacketStrc* pPacket = *ppPacketList;
	if (!pPacket || (dword_6FC0B264 == 2 && GetTickCount() - pPacket->dwTickCount < 500))
	{
		D2_UNLOCK(pCriticalSection);
		return -1;
	}

	memcpy(pBuffer, pPacket, std::min(nBufferSize, MAX_MSG_SIZE));

	const int32_t nPacketSize = pPacket->nPacketSize;

	*ppPacketList = pPacket->pNext;

	D2_FREE(pPacket);

	D2_UNLOCK(pCriticalSection);
	return nPacketSize;
}

//D2Net.0x6FC01AE0
int32_t __fastcall NET_FreePacketList(CRITICAL_SECTION* pCriticalSection, D2PacketStrc** ppPacketList)
{
	D2_LOCK(pCriticalSection);

	for (D2PacketStrc* i = *ppPacketList; *ppPacketList; i = *ppPacketList)
	{
		*ppPacketList = i->pNext;
		D2_FREE(i);
	}

	D2_UNLOCK(pCriticalSection);
	return 1;
}
