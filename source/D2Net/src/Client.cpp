#include "Client.h"

#include <algorithm>

#include <Fog.h>
#include <Storm.h>

#include "D2Net.h"
#include "Server.h"


#pragma warning (disable: 6387)
#pragma warning (disable: 28159)


CRITICAL_SECTION gCriticalSection;
D2PacketStrc* gpSystemPacketList;
sockaddr_in gHostSockAddr;
D2PacketStrc* gpGamePacketList;
D2PacketBufferStrc* gpPacketBuffer;
HANDLE ghThread;
SOCKET gClientSocket;
HANDLE ghClientThreadHandle;
WSADATA gWSAData;
DWORD gdwThreadId;
int32_t dword_6FC0B25C;
int32_t dword_6FC0B260;


//D2Net.0x6FC01000 (#10017)
int32_t __stdcall D2NET_10017()
{
	if (sub_6FC01A00())
	{
		return 1;
	}

	if (!gpPacketBuffer)
	{
		return 0;
	}

	EnterCriticalSection(&gCriticalSection);
	const int32_t v1 = dword_6FC0B260;
	LeaveCriticalSection(&gCriticalSection);

	return v1;
}

//D2Net.0x6FC01040
DWORD __stdcall CLIENT_ConnectToHost(void* szIpAddress)
{
	dword_6FC0B25C = 0;

	if (WSAStartup(MAKEWORD(1, 1), &gWSAData))
	{
		FOG_DisplayHalt("WSAStartup", __FILE__, __LINE__);
		exit(-1);
	}

	gClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (gClientSocket == SOCKET_ERROR)
	{
		WSAGetLastError();
		FOG_DisplayHalt("Socket", __FILE__, __LINE__);
		exit(-1);
	}

	memset(&gHostSockAddr, 0x00, sizeof(gHostSockAddr));

	gHostSockAddr.sin_family = AF_INET;
	gHostSockAddr.sin_port = htons(GAME_PORT);
	gHostSockAddr.sin_addr.s_addr = inet_addr(FOG_ResolveHostIPAddress((const char*)szIpAddress));

	if (connect(gClientSocket, (sockaddr*)&gHostSockAddr, sizeof(sockaddr_in)) == -1)
	{
		gClientSocket = 0;
	}

	return 1;
}

//D2Net.0x6FC01120 (#10025)
int32_t __stdcall D2NET_10025()
{
	if (sub_6FC01A00())
	{
		return 2;
	}

	if (WaitForSingleObject(ghClientThreadHandle, 100))
	{
		return 1;
	}

	if (!gClientSocket)
	{
		return 0;
	}

	if (!sub_6FC01A00())
	{
		EnterCriticalSection(&gCriticalSection);
		dword_6FC0B260 = 1;
		LeaveCriticalSection(&gCriticalSection);
	}

	ghThread = CreateThread(nullptr, 0, sub_6FC01320, nullptr, 0, &gdwThreadId);
	SetThreadPriority(ghThread, 1);
	return 2;
}

//D2Net.0x6FC011B0 (#10000)
void __stdcall CLIENT_Initialize(int32_t a1, const char* szIpAddress)
{
	gpGamePacketList = nullptr;
	gpSystemPacketList = nullptr;

	InitializeCriticalSection(&gCriticalSection);

	gpPacketBuffer = D2_CALLOC_STRC(D2PacketBufferStrc);

	sub_6FC01A20(a1);

	if (sub_6FC01A00())
	{
		sub_6FC02110();
		return;
	}

	ghClientThreadHandle = CreateThread(nullptr, 0, CLIENT_ConnectToHost, (void*)szIpAddress, 0, &gdwThreadId);
}

//D2Net.0x6FC01240 (#10001)
void __stdcall CLIENT_Release()
{
	EnterCriticalSection(&gCriticalSection);

	if (gpPacketBuffer)
	{
		D2_FREE(gpPacketBuffer);
	}

	gpPacketBuffer = nullptr;

	dword_6FC0B25C = 1;

	LeaveCriticalSection(&gCriticalSection);

	WaitForSingleObject(ghThread, 6000u);

	NET_FreePacketList(&gCriticalSection, &gpSystemPacketList);
	NET_FreePacketList(&gCriticalSection, &gpGamePacketList);

	//EnterCriticalSection(&gCriticalSection);

	DeleteCriticalSection(&gCriticalSection);

	if (sub_6FC01A00())
	{
		return;
	}

	if (FOG_SOCKET_CloseSocket(gClientSocket) == -1)
	{
		WSAGetLastError();
	}

	gClientSocket = 0;
}

//D2Net.0x6FC01300 (#10007)
int32_t __stdcall CLIENT_DequeueGamePacket(uint8_t* pBuffer, uint32_t nBufferSize)
{
	return NET_DequeueFirstPacketFromList(&gCriticalSection, &gpGamePacketList, pBuffer, nBufferSize);
}

//D2Net.0x6FC01310 (#10008)
int32_t __stdcall CLIENT_DequeueSystemPacket(uint8_t* pBuffer, uint32_t nBufferSize)
{
	return NET_DequeueFirstPacketFromList(&gCriticalSection, &gpSystemPacketList, pBuffer, nBufferSize);
}

//D2Net.0x6FC01320
DWORD __stdcall sub_6FC01320(void* a1)
{
	// TODO: Names
	int32_t bPacket0xAEReceived = 0;
	int32_t v2 = 0;

	while (!dword_6FC0B25C)
	{
		fd_set readfds = {};
		readfds.fd_array[0] = gClientSocket;
		readfds.fd_count = 1;

		timeval timeout = {};
		timeout.tv_sec = 0;
		timeout.tv_usec = 100'000;

		int32_t v4 = select(0, &readfds, nullptr, nullptr, &timeout);
		if (v4)
		{
			if (v4 == -1)
			{
				WSAGetLastError();
				if (!sub_6FC01A00())
				{
					EnterCriticalSection(&gCriticalSection);
					dword_6FC0B260 = 0;
					LeaveCriticalSection(&gCriticalSection);
				}
			}
			else if (__WSAFDIsSet(gClientSocket, &readfds))
			{
				if (dword_6FC0B25C)
				{
					OutputDebugStringA("Client thread close #3\n");
					ExitThread(0);
				}

				EnterCriticalSection(&gCriticalSection);
				if (!bPacket0xAEReceived)
				{
					const int32_t v10 = recv(gClientSocket, (char*)&gpPacketBuffer->data[gpPacketBuffer->nUsedBytes], 1460, 0);
					if (v10 > 0)
					{
						gpPacketBuffer->nUsedBytes += v10;
						bPacket0xAEReceived = CLIENT_ReadPacketsFromStream();
					}
				}
				else
				{
					uint8_t buffer[2496] = {};

					const int32_t v5 = recv(gClientSocket, (char*)&buffer[v2], 1460, 0);
					v4 = v5;
					if (v5 > 0)
					{
						v2 += v5;

						int32_t v6 = 0;
						while (v2 >= 2)
						{
							uint8_t v7 = buffer[v6];
							int32_t v8 = 0;
							if (v7 >= 0xF0u)
							{
								v8 = (uint8_t)buffer[v6 + 1] + ((v7 & 0xF) << 8);
							}
							else
							{
								v8 = v7;
							}

							if (v2 < v8)
							{
								break;
							}

							v4 = FOG_10224((char*)&gpPacketBuffer->data[gpPacketBuffer->nUsedBytes], sizeof(gpPacketBuffer->data) - gpPacketBuffer->nUsedBytes, &buffer[v6] + 2 - ((uint32_t)v8 < 0xF0), v8 - (2 - ((uint32_t)v8 < 0xF0)));
							v2 -= v8;
							v6 += v8;
							if (v4 > 0)
							{
								gpPacketBuffer->nUsedBytes += v4;
								CLIENT_ReadPacketsFromStream();
							}
						}

						if (v2 && v6)
						{
							memcpy(buffer, &buffer[v6], v2);
						}
					}
				}

				LeaveCriticalSection(&gCriticalSection);

				if (v4 == -1)
				{
					WSAGetLastError();
					if (!sub_6FC01A00())
					{
						EnterCriticalSection(&gCriticalSection);
						dword_6FC0B260 = 0;
						LeaveCriticalSection(&gCriticalSection);
					}

					OutputDebugStringA("Client thread close #5\n");
					ExitThread(0);
				}

				if (!v4)
				{
					if (!sub_6FC01A00())
					{
						EnterCriticalSection(&gCriticalSection);
						dword_6FC0B260 = 0;
						LeaveCriticalSection(&gCriticalSection);
					}

					OutputDebugStringA("Client thread close #6\n");
					ExitThread(0);
				}
			}
		}
	}

	OutputDebugStringA("Client thread close #2\n");
	ExitThread(0);
}

//D2Net.0x6FC015C0
int32_t __stdcall CLIENT_ReadPacketsFromStream()
{
	D2PacketBufferStrc* pPacketBuffer = gpPacketBuffer;
	int32_t nUsedBytes = gpPacketBuffer->nUsedBytes;
	int32_t bIsPacket0xAE = 0;

	while (nUsedBytes >= 1)
	{
		int32_t nSize = 0;
		if (!SERVER_GetServerPacketSize(pPacketBuffer, nUsedBytes, &nSize))
		{
			break;
		}

		D2_ASSERT(nSize <= MAX_MSG_SIZE);

		if (nSize > nUsedBytes)
		{
			break;
		}

		D2PacketStrc* pPacket = D2_ALLOC_STRC(D2PacketStrc);
		pPacket->nPacketSize = nSize;
		pPacket->dwTickCount = 0;
		pPacket->pNext = nullptr;

		memcpy(&pPacket->data, pPacketBuffer->data, nSize);

		if (pPacketBuffer->data[0] == 0xAE && pPacketBuffer->data[1])
		{
			bIsPacket0xAE = 1;

			if (pPacketBuffer->data[1] == 0x81)
			{
				uint8_t processedData[256] = {};

				for (int32_t i = 0; i < std::size(processedData) / 2; ++i)
				{
					const uint8_t element = pPacketBuffer->data[i + 1];

					processedData[2 * i + 0] = (element & 0xF) + 1;
					processedData[2 * i + 1] = (element >> 4) + 1;
				}

				FOG_10219(processedData);
			}
		}
		else if (pPacketBuffer->data[0] == 0x8F)
		{
			*(uint32_t*)&pPacket->data[13] = GetTickCount();
		}

		D2PacketStrc** ppLast = nullptr;
		if (pPacketBuffer->data[0] >= 0xAEu)
		{
			ppLast = &gpSystemPacketList;
		}
		else
		{
			ppLast = &gpGamePacketList;
		}

		for (D2PacketStrc* i = *ppLast; i; i = i->pNext)
		{
			ppLast = &i->pNext;
		}
		*ppLast = pPacket;

		pPacketBuffer = (D2PacketBufferStrc*)((char*)pPacketBuffer + nSize);
		nUsedBytes -= nSize;
	}

	gpPacketBuffer->nUsedBytes = nUsedBytes;

	if (nUsedBytes && pPacketBuffer != gpPacketBuffer)
	{
		memcpy(gpPacketBuffer, pPacketBuffer, nUsedBytes);
	}

	return bIsPacket0xAE;
}

//D2Net.0x6FC01760 (#10005)
int32_t __stdcall CLIENT_Send(int32_t nUnused, const uint8_t* pBuffer, int32_t nBufferSize)
{
	if (!gpPacketBuffer)
	{
		return 0;
	}

	if (sub_6FC01A00())
	{
		D2_ASSERT(nBufferSize <= MAX_MSG_SIZE);
		return SERVER_EnqueuePacketToMessageList(pBuffer, nBufferSize);
	}

	const int32_t nSentBytes = send(gClientSocket, (const char*)pBuffer, nBufferSize, 0);
	if (nSentBytes != -1)
	{
		return nSentBytes;
	}

	WSAGetLastError();
	if (sub_6FC01A00())
	{
		return 0;
	}

	EnterCriticalSection(&gCriticalSection);
	dword_6FC0B260 = 0;
	LeaveCriticalSection(&gCriticalSection);
	return 0;
}

//D2Net.0x6FC01810
void __fastcall CLIENT_ReadPacketFromBuffer(D2PacketBufferStrc* pBuffer, int32_t nBufferSize)
{
	D2PacketBufferStrc* pPacketBuffer = pBuffer;
	int32_t nRemainingBytes = nBufferSize;

	while (nRemainingBytes > 0)
	{
		D2PacketStrc* pPacket = D2_ALLOC_STRC(D2PacketStrc);

		int32_t nSize = 0;
		if (!SERVER_GetServerPacketSize(pPacketBuffer, nRemainingBytes, &nSize))
		{
			break;
		}

		D2_ASSERT(nSize <= MAX_MSG_SIZE);
		D2_ASSERT(nSize > 0);

		memcpy(&pPacket->data, pPacketBuffer->data, nSize);

		pPacket->nPacketSize = nSize;
		pPacket->dwTickCount = GetTickCount();
		pPacket->pNext = nullptr;

		const uint8_t nHeader = pPacketBuffer->data[0];
		if (nHeader >= 0xB4u)
		{
			FOG_DisplayHalt("Bad message type", __FILE__, __LINE__);
			exit(-1);
		}

		D2PacketStrc* pLast = nullptr;
		if (nHeader >= 0xAEu)
		{
			pLast = gpSystemPacketList;
		}
		else
		{
			pLast = gpGamePacketList;
		}

		if (pLast)
		{
			for (D2PacketStrc* i = pLast->pNext; i; i = i->pNext)
			{
				pLast = i;
			}
			pLast->pNext = pPacket;
		}
		else
		{
			if (nHeader >= 0xB4u)
			{
				FOG_DisplayHalt("Bad message type", __FILE__, __LINE__);
				exit(-1);
			}

			if (nHeader >= 0xAEu)
			{
				gpSystemPacketList = pPacket;
			}
			else
			{
				gpGamePacketList = pPacket;
			}
		}

		pPacketBuffer = (D2PacketBufferStrc*)((char*)pPacketBuffer + nSize);
		nRemainingBytes -= nSize;
	}
}

//D2Net.0x6FC019A0 (#10013)
void __stdcall CLIENT_GetLocalIpAddressString(char* szBuffer)
{
	sockaddr_in sa = {};
	int32_t sa_len = sizeof(sa);

	getsockname(gClientSocket, (sockaddr*)&sa, &sa_len);

	const char* szLocalIpAddress = inet_ntoa(sa.sin_addr);

	SStrCopy(szBuffer, szLocalIpAddress, INT_MAX);
}
