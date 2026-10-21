#include "Client.h"


#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>

#include <Windows.h>
#include <WinSock2.h>
#include <ws2def.h>

#include <Fog.h>
#include <Safesock.h>
#include <Storm/String.hpp>

#include <D2Net.h>
#include <Packet.h>
#include <Server.h>


#pragma warning (disable: 6387)
#pragma warning (disable: 28159)


CRITICAL_SECTION gClientCriticalSection;
D2NetQueuedPacketStrc* gpSystemPacketList;
sockaddr_in gHostSockAddr;
D2NetQueuedPacketStrc* gpGamePacketList;
D2NetReceiveBufferStrc* gpReceiveBuffer;
HANDLE ghReceiveThread;
SOCKET gClientSocket;
HANDLE ghConnectThread;
WSADATA gWSAData;
DWORD gdwThreadId;
BOOL gbStopReceiveThread;
BOOL gbConnected;


//D2Net.0x6FC01000 (#10017)
int32_t __stdcall CLIENT_IsConnected()
{
	if (NET_IsLocalConnection())
	{
		return 1;
	}

	if (!gpReceiveBuffer)
	{
		return 0;
	}

	D2_LOCK(&gClientCriticalSection);
	const BOOL bConnected = gbConnected;
	D2_UNLOCK(&gClientCriticalSection);

	return bConnected;
}

//D2Net.0x6FC01040
DWORD __stdcall CLIENT_ConnectToHost(void* szHostAddress)
{
	gbStopReceiveThread = 0;

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

	std::memset(&gHostSockAddr, 0x00, sizeof(gHostSockAddr));

	gHostSockAddr.sin_family = AF_INET;
	gHostSockAddr.sin_port = htons(GAME_PORT);
	gHostSockAddr.sin_addr.s_addr = inet_addr(FOG_ResolveHostIPAddress((const char*)szHostAddress)); // NOLINT

	if (connect(gClientSocket, (sockaddr*)&gHostSockAddr, sizeof(sockaddr_in)) == -1)
	{
		gClientSocket = 0;
	}

	return 1;
}

//D2Net.0x6FC01120 (#10025)
int32_t __stdcall CLIENT_GetConnectionStatus()
{
	if (NET_IsLocalConnection())
	{
		return CLIENT_CONNECTION_ESTABLISHED;
	}

	if (WaitForSingleObject(ghConnectThread, 100))
	{
		return CLIENT_CONNECTION_PENDING;
	}

	if (!gClientSocket)
	{
		return CLIENT_CONNECTION_FAILED;
	}

	if (!NET_IsLocalConnection())
	{
		D2_LOCK(&gClientCriticalSection);
		gbConnected = 1;
		D2_UNLOCK(&gClientCriticalSection);
	}

	ghReceiveThread = CreateThread(nullptr, 0, CLIENT_ReceiveThreadProc, nullptr, 0, &gdwThreadId);
	SetThreadPriority(ghReceiveThread, 1);
	
	SetThreadDescription(ghReceiveThread, L"D2ClientThread"); // D2Moo only
	return CLIENT_CONNECTION_ESTABLISHED;
}

//D2Net.0x6FC011B0 (#10000)
int32_t __stdcall CLIENT_Initialize(int32_t nConnectionMode, const char* szHostAddress)
{
	gpGamePacketList = nullptr;
	gpSystemPacketList = nullptr;

	InitializeCriticalSection(&gClientCriticalSection);

	gpReceiveBuffer = D2_CALLOC_STRC(D2NetReceiveBufferStrc);

	NET_SetConnectionMode(nConnectionMode);

	if (NET_IsLocalConnection())
	{
		SERVER_DisableLocalClientCompression();
		return 1;
	}

	ghConnectThread = CreateThread(nullptr, 0, CLIENT_ConnectToHost, (void*)szHostAddress, 0, &gdwThreadId);
	SetThreadDescription(ghConnectThread, L"D2ClientConnectToHostThread"); // D2Moo only
	return ghConnectThread != nullptr;
}

//D2Net.0x6FC01240 (#10001)
void __stdcall CLIENT_Release()
{
	D2_LOCK(&gClientCriticalSection);

	if (gpReceiveBuffer)
	{
		D2_FREE(gpReceiveBuffer);
	}

	gpReceiveBuffer = nullptr;

	gbStopReceiveThread = 1;

	D2_UNLOCK(&gClientCriticalSection);

	WaitForSingleObject(ghReceiveThread, 6000u);

	NET_FreePacketList(&gClientCriticalSection, &gpSystemPacketList);
	NET_FreePacketList(&gClientCriticalSection, &gpGamePacketList);

#ifdef NO_BUG_FIX
	// Original game would lock the CS here, but it is actually undefined behaviour to then delete it.
	// "If a critical section is deleted while it is still owned, the state of the threads waiting for ownership of the deleted critical section is undefined."
	// This never happens because no thread other than the current one should use this critical section after ghReceiveThread is closed anyway.
	// Just in case, don't do it.
	D2_LOCK(&gClientCriticalSection);
#endif

	DeleteCriticalSection(&gClientCriticalSection);

	if (NET_IsLocalConnection())
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
	return NET_DequeueFirstPacketFromList(&gClientCriticalSection, &gpGamePacketList, pBuffer, nBufferSize);
}

//D2Net.0x6FC01310 (#10008)
int32_t __stdcall CLIENT_DequeueSystemPacket(uint8_t* pBuffer, uint32_t nBufferSize)
{
	return NET_DequeueFirstPacketFromList(&gClientCriticalSection, &gpSystemPacketList, pBuffer, nBufferSize);
}

//D2Net.0x6FC01320
DWORD __stdcall CLIENT_ReceiveThreadProc(void* pUnused)
{
	BOOL bCompressionEnabled = FALSE;
	// Compressed bytes received from the socket, not yet decompressed into gpReceiveBuffer
	int32_t nCompressedBytes = 0;
	uint8_t compressedBuffer[2496] = {};

	while (!gbStopReceiveThread)
	{
		fd_set readfds = {};
		readfds.fd_array[0] = gClientSocket;
		readfds.fd_count = 1;

		timeval timeout = {};
		timeout.tv_sec = 0;
		timeout.tv_usec = 100'000;

		int32_t nResult = select(0, &readfds, nullptr, nullptr, &timeout);
		if (nResult)
		{
			if (nResult == -1)
			{
				WSAGetLastError();
				if (!NET_IsLocalConnection())
				{
					D2_LOCK(&gClientCriticalSection);
					gbConnected = 0;
					D2_UNLOCK(&gClientCriticalSection);
				}
			}
			else if (__WSAFDIsSet(gClientSocket, &readfds))
			{
				if (gbStopReceiveThread)
				{
					OutputDebugStringA("Client thread close #3\n");
					ExitThread(0);
				}

				D2_LOCK(&gClientCriticalSection);
				if (!bCompressionEnabled)
				{
					const int32_t nReceivedBytes = recv(gClientSocket, (char*)&gpReceiveBuffer->data[gpReceiveBuffer->nUsedBytes], 1460, 0);
					if (nReceivedBytes > 0)
					{
						gpReceiveBuffer->nUsedBytes += nReceivedBytes;
						bCompressionEnabled = CLIENT_ParseReceiveBuffer();
					}
				}
				else
				{
					const int32_t nReceivedBytes = recv(gClientSocket, (char*)&compressedBuffer[nCompressedBytes], 1460, 0);
					nResult = nReceivedBytes;
					if (nReceivedBytes > 0)
					{
						nCompressedBytes += nReceivedBytes;

						// Each compressed frame starts with its total size, encoded on 1 byte (< 0xF0) or on 12 bits spread over 2 bytes (0xFX XX)
						int32_t nFrameOffset = 0;
						while (nCompressedBytes >= 2)
						{
							const uint8_t nFirstByte = compressedBuffer[nFrameOffset];
							int32_t nFrameSize = 0;
							if (nFirstByte >= 0xF0u)
							{
								nFrameSize = (uint8_t)compressedBuffer[nFrameOffset + 1] + ((nFirstByte & 0xF) << 8);
							}
							else
							{
								nFrameSize = nFirstByte;
							}

							if (nCompressedBytes < nFrameSize)
							{
								break;
							}

							const int32_t nFrameHeaderSize = 2 - ((uint32_t)nFrameSize < 0xF0);
							nResult = FOG_10224((char*)&gpReceiveBuffer->data[gpReceiveBuffer->nUsedBytes], int(sizeof(gpReceiveBuffer->data) - gpReceiveBuffer->nUsedBytes), &compressedBuffer[nFrameOffset] + nFrameHeaderSize, nFrameSize - nFrameHeaderSize);
							nCompressedBytes -= nFrameSize;
							nFrameOffset += nFrameSize;
							if (nResult > 0)
							{
								gpReceiveBuffer->nUsedBytes += nResult;
								CLIENT_ParseReceiveBuffer();
							}
						}

						if (nCompressedBytes && nFrameOffset)
						{
							memcpy(compressedBuffer, &compressedBuffer[nFrameOffset], nCompressedBytes);
						}
					}
				}

				D2_UNLOCK(&gClientCriticalSection);

				if (nResult == -1)
				{
					WSAGetLastError();
					if (!NET_IsLocalConnection())
					{
						D2_LOCK(&gClientCriticalSection);
						gbConnected = 0;
						D2_UNLOCK(&gClientCriticalSection);
					}

					OutputDebugStringA("Client thread close #5\n");
					ExitThread(0);
				}

				if (!nResult)
				{
					if (!NET_IsLocalConnection())
					{
						D2_LOCK(&gClientCriticalSection);
						gbConnected = 0;
						D2_UNLOCK(&gClientCriticalSection);
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
int32_t __stdcall CLIENT_ParseReceiveBuffer()
{
	D2NetReceiveBufferStrc* pPacketBuffer = gpReceiveBuffer;
	int32_t nUsedBytes = gpReceiveBuffer->nUsedBytes;
	BOOL bCompressionEnabled = FALSE;

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

		D2NetQueuedPacketStrc* pPacket = D2_ALLOC_STRC(D2NetQueuedPacketStrc);
		pPacket->nPacketSize = nSize;
		pPacket->dwReceiveTick = 0;
		pPacket->pNext = nullptr;

		memcpy(&pPacket->data, pPacketBuffer->data, nSize);

		if (pPacketBuffer->data[0] == SERVER_PACKET_HEADER_COMPRESSION_INFO && pPacketBuffer->data[1])
		{
			bCompressionEnabled = TRUE;

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

		D2NetQueuedPacketStrc** ppLast = nullptr;
		if (pPacketBuffer->data[0] >= SERVER_SYSTEM_PACKET_HEADER_MIN)
		{
			ppLast = &gpSystemPacketList;
		}
		else
		{
			ppLast = &gpGamePacketList;
		}

		for (D2NetQueuedPacketStrc* pQueuedPacket = *ppLast; pQueuedPacket; pQueuedPacket = pQueuedPacket->pNext)
		{
			ppLast = &pQueuedPacket->pNext;
		}
		*ppLast = pPacket;

		pPacketBuffer = (D2NetReceiveBufferStrc*)((char*)pPacketBuffer + nSize);
		nUsedBytes -= nSize;
	}

	gpReceiveBuffer->nUsedBytes = nUsedBytes;

	if (nUsedBytes && pPacketBuffer != gpReceiveBuffer)
	{
		memcpy(gpReceiveBuffer, pPacketBuffer, nUsedBytes);
	}

	return bCompressionEnabled;
}

//D2Net.0x6FC01760 (#10005)
int32_t __stdcall CLIENT_Send(int32_t nUnused, const uint8_t* pBuffer, int32_t nBufferSize)
{
	if (!gpReceiveBuffer)
	{
		return 0;
	}

	if (NET_IsLocalConnection())
	{
		D2_ASSERT(nBufferSize <= MAX_MSG_SIZE);
		return SERVER_EnqueueLocalClientPacket(pBuffer, static_cast<uint16_t>(nBufferSize));
	}

	const int32_t nSentBytes = send(gClientSocket, (const char*)pBuffer, nBufferSize, 0);
	if (nSentBytes != -1)
	{
		return nSentBytes;
	}

	WSAGetLastError();
	if (NET_IsLocalConnection())
	{
		return 0;
	}

	D2_LOCK(&gClientCriticalSection);
	gbConnected = 0;
	D2_UNLOCK(&gClientCriticalSection);
	return 0;
}

//D2Net.0x6FC01810
void __fastcall CLIENT_EnqueuePacketsFromLocalServer(D2NetReceiveBufferStrc* pBuffer, int32_t nBufferSize)
{
	D2NetReceiveBufferStrc* pPacketBuffer = pBuffer;
	int32_t nRemainingBytes = nBufferSize;

	while (nRemainingBytes > 0)
	{
		D2NetQueuedPacketStrc* pPacket = D2_ALLOC_STRC(D2NetQueuedPacketStrc);

		int32_t nSize = 0;
		if (!SERVER_GetServerPacketSize(pPacketBuffer, nRemainingBytes, &nSize))
		{
			break;
		}

		D2_ASSERT(nSize <= MAX_MSG_SIZE);
		D2_ASSERT(nSize > 0);

		memcpy(&pPacket->data, pPacketBuffer->data, nSize);

		pPacket->nPacketSize = nSize;
		pPacket->dwReceiveTick = GetTickCount();
		pPacket->pNext = nullptr;

		const uint8_t nHeader = pPacketBuffer->data[0];
		if (nHeader >= SERVER_PACKET_HEADER_END)
		{
			FOG_DisplayHalt("Bad message type", __FILE__, __LINE__);
			exit(-1);
		}

		D2NetQueuedPacketStrc* pLast = nullptr;
		if (nHeader >= SERVER_SYSTEM_PACKET_HEADER_MIN)
		{
			pLast = gpSystemPacketList;
		}
		else
		{
			pLast = gpGamePacketList;
		}

		if (pLast)
		{
			for (D2NetQueuedPacketStrc* pQueuedPacket = pLast->pNext; pQueuedPacket; pQueuedPacket = pQueuedPacket->pNext)
			{
				pLast = pQueuedPacket;
			}
			pLast->pNext = pPacket;
		}
		else
		{
			if (nHeader >= SERVER_PACKET_HEADER_END)
			{
				FOG_DisplayHalt("Bad message type", __FILE__, __LINE__);
				exit(-1);
			}

			if (nHeader >= SERVER_SYSTEM_PACKET_HEADER_MIN)
			{
				gpSystemPacketList = pPacket;
			}
			else
			{
				gpGamePacketList = pPacket;
			}
		}

		pPacketBuffer = (D2NetReceiveBufferStrc*)((char*)pPacketBuffer + nSize);
		nRemainingBytes -= nSize;
	}
}

//D2Net.0x6FC019A0 (#10013)
void __stdcall CLIENT_GetLocalIpAddressString(char* szBuffer)
{
	sockaddr_in localSockAddr = {};
	int32_t nLocalSockAddrSize = sizeof(localSockAddr);

	getsockname(gClientSocket, (sockaddr*)&localSockAddr, &nLocalSockAddrSize);

	const char* szLocalIpAddress = inet_ntoa(localSockAddr.sin_addr); // NOLINT

	SStrCopy(szBuffer, szLocalIpAddress, INT_MAX);
}
