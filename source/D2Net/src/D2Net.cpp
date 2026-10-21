#include "D2Net.h"


#include <algorithm>
#include <cstdint>
#include <cstring>

#include <Windows.h>

#include <Fog.h>
#include <Packet.h>


#pragma warning (disable: 28159)


int32_t gnConnectionMode;	// D2NetConnectionMode


//D2Net.0x6FC01A00
BOOL __stdcall NET_IsLocalConnection()
{
	return gnConnectionMode == D2NET_CONNECTION_LOCAL || gnConnectionMode == D2NET_CONNECTION_LOCAL_SIMULATED_LAG;
}

//D2Net.0x6FC01A20
void __fastcall NET_SetConnectionMode(int32_t nConnectionMode)
{
	gnConnectionMode = nConnectionMode;
}

//D2Net.0x6FC01A30
int32_t __fastcall NET_DequeueFirstPacketFromList(CRITICAL_SECTION* pCriticalSection, D2NetQueuedPacketStrc** ppPacketList, uint8_t* pBuffer, uint32_t nBufferSize)
{
	D2_LOCK(pCriticalSection);

	D2NetQueuedPacketStrc* pPacket = *ppPacketList;
	if (!pPacket || (gnConnectionMode == D2NET_CONNECTION_LOCAL_SIMULATED_LAG && GetTickCount() - pPacket->dwReceiveTick < SIMULATED_LAG_MS))
	{
		D2_UNLOCK(pCriticalSection);
		return -1;
	}

	std::memcpy(pBuffer, pPacket, std::min(nBufferSize, MAX_MSG_SIZE));

	const int32_t nPacketSize = pPacket->nPacketSize;

	*ppPacketList = pPacket->pNext;

	D2_FREE(pPacket);

	D2_UNLOCK(pCriticalSection);
	return nPacketSize;
}

//D2Net.0x6FC01AE0
int32_t __fastcall NET_FreePacketList(CRITICAL_SECTION* pCriticalSection, D2NetQueuedPacketStrc** ppPacketList)
{
	D2_LOCK(pCriticalSection);

	for (D2NetQueuedPacketStrc* pPacket = *ppPacketList; *ppPacketList; pPacket = *ppPacketList)
	{
		*ppPacketList = pPacket->pNext;
		D2_FREE(pPacket);
	}

	D2_UNLOCK(pCriticalSection);
	return 1;
}
