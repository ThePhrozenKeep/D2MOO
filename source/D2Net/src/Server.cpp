#include "Server.h"


#include <cstdint>
#include <iterator>

#include <WinSock2.h>

#include <Fog.h>
#include <Storm/String.hpp>

#include <D2PacketDef.h>

#include <Client.h>
#include <D2Net.h>


QServer* gpServer;
int32_t gnLocalClientGameGUID;


constexpr int32_t VARIABLE_PACKET_SIZE = -1;

constexpr uint8_t CLIENT_PACKET_HEADER_ADMIN = 0xFF;

// Return values of SERVER_ValidateClientPacket
constexpr int32_t PACKET_VALIDATION_QUEUED = 1;
constexpr int32_t PACKET_VALIDATION_QUEUED_SYSTEM = 2;
constexpr int32_t PACKET_VALIDATION_INCOMPLETE = 3;
constexpr int32_t PACKET_VALIDATION_INVALID = 4;


//D2Net.0x6FC01B30 (#10024)
int32_t __stdcall SERVER_WSAGetLastError()
{
	return WSAGetLastError();
}

//D2Net.0x6FC01B60 (#10030)
int32_t __fastcall SERVER_GetServerPacketSize(D2NetReceiveBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize)
{
	constexpr int32_t gServerPacketSizeTable[] =
	{
		sizeof(D2GSPacketSrv00),
		sizeof(D2GSPacketSrv01),
		sizeof(D2GSPacketSrv02),
		sizeof(D2GSPacketSrv03),
		sizeof(D2GSPacketSrv04),
		sizeof(D2GSPacketSrv05),
		sizeof(D2GSPacketSrv06),
		sizeof(D2GSPacketSrv07),
		sizeof(D2GSPacketSrv08),
		sizeof(D2GSPacketSrv09),
		sizeof(D2GSPacketSrv0A),
		sizeof(D2GSPacketSrv0B),
		sizeof(D2GSPacketSrv0C),
		sizeof(D2GSPacketSrv0D),
		sizeof(D2GSPacketSrv0E),
		sizeof(D2GSPacketSrv0F),
		sizeof(D2GSPacketSrv10),
		sizeof(D2GSPacketSrv11),
		sizeof(D2GSPacketSrv12),
		sizeof(D2GSPacketSrv13),
		sizeof(D2GSPacketSrv14),
		sizeof(D2GSPacketSrv15),
		VARIABLE_PACKET_SIZE,
		0,
		sizeof(D2GSPacketSrv18),
		sizeof(D2GSPacketSrv19),
		sizeof(D2GSPacketSrv1A),
		sizeof(D2GSPacketSrv1B),
		sizeof(D2GSPacketSrv1C),
		sizeof(D2GSPacketSrv1D),
		sizeof(D2GSPacketSrv1E),
		sizeof(D2GSPacketSrv1F),
		sizeof(D2GSPacketSrv20),
		sizeof(D2GSPacketSrv21),
		sizeof(D2GSPacketSrv22),
		sizeof(D2GSPacketSrv23),
		sizeof(D2GSPacketSrv24),
		sizeof(D2GSPacketSrv25),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrv27),
		sizeof(D2GSPacketSrv28),
		sizeof(D2GSPacketSrv29),
		sizeof(D2GSPacketSrv2A),
		0,
		sizeof(D2GSPacketSrv2C),
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrv3F),
		sizeof(D2GSPacketSrv40),
		0,
		sizeof(D2GSPacketSrv42),
		0,
		0,
		sizeof(D2GSPacketSrv45),
		0,
		sizeof(D2GSPacketSrv47),
		sizeof(D2GSPacketSrv48),
		0,
		0,
		0,
		sizeof(D2GSPacketSrv4C),
		sizeof(D2GSPacketSrv4D),
		sizeof(D2GSPacketSrv4E),
		sizeof(D2GSPacketSrv4F),
		sizeof(D2GSPacketSrv50),
		sizeof(D2GSPacketSrv51),
		sizeof(D2GSPacketSrv52),
		sizeof(D2GSPacketSrv53),
		sizeof(D2GSPacketSrv54),
		0,
		0,
		sizeof(D2GSPacketSrv57),
		sizeof(D2GSPacketSrv58),
		sizeof(D2GSPacketSrv59),
		sizeof(D2GSPacketSrv5A),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrv5C),
		sizeof(D2GSPacketSrv5D),
		sizeof(D2GSPacketSrv5E),
		sizeof(D2GSPacketSrv5F),
		sizeof(D2GSPacketSrv60),
		sizeof(D2GSPacketSrv61),
		sizeof(D2GSPacketSrv62),
		sizeof(D2GSPacketSrv63),
		0,
		sizeof(D2GSPacketSrv65),
		sizeof(D2GSPacketSrv66),
		sizeof(D2GSPacketSrv67),
		sizeof(D2GSPacketSrv68),
		sizeof(D2GSPacketSrv69),
		sizeof(D2GSPacketSrv6A),
		sizeof(D2GSPacketSrv6B),
		sizeof(D2GSPacketSrv6C),
		sizeof(D2GSPacketSrv6D),
		sizeof(D2GSPacketSrv6E),
		sizeof(D2GSPacketSrv6F),
		sizeof(D2GSPacketSrv70),
		sizeof(D2GSPacketSrv71),
		sizeof(D2GSPacketSrv72),
		sizeof(D2GSPacketSrv73),
		sizeof(D2GSPacketSrv74),
		sizeof(D2GSPacketSrv75),
		sizeof(D2GSPacketSrv76),
		sizeof(D2GSPacketSrv77),
		sizeof(D2GSPacketSrv78),
		sizeof(D2GSPacketSrv79),
		sizeof(D2GSPacketSrv7A),
		sizeof(D2GSPacketSrv7B),
		sizeof(D2GSPacketSrv7C),
		sizeof(D2GSPacketSrv7D),
		sizeof(D2GSPacketSrv7E),
		sizeof(D2GSPacketSrv7F),
		0,
		sizeof(D2GSPacketSrv81),
		sizeof(D2GSPacketSrv82),
		0,
		0,
		0,
		0,
		0,
		0,
		sizeof(D2GSPacketSrv89),
		sizeof(D2GSPacketSrv8A),
		sizeof(D2GSPacketSrv8B),
		sizeof(D2GSPacketSrv8C),
		sizeof(D2GSPacketSrv8D),
		sizeof(D2GSPacketSrv8E),
		sizeof(D2GSPacketSrv8F),
		sizeof(D2GSPacketSrv90),
		sizeof(D2GSPacketSrv91),
		sizeof(D2GSPacketSrv92),
		sizeof(D2GSPacketSrv93),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrv95),
		sizeof(D2GSPacketSrv96),
		sizeof(D2GSPacketSrv97),
		sizeof(D2GSPacketSrv98),
		sizeof(D2GSPacketSrv99),
		sizeof(D2GSPacketSrv9A),
		sizeof(D2GSPacketSrv9B),
		VARIABLE_PACKET_SIZE,
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrv9E),
		sizeof(D2GSPacketSrv9F),
		sizeof(D2GSPacketSrvA0),
		sizeof(D2GSPacketSrvA1),
		sizeof(D2GSPacketSrvA2),
		sizeof(D2GSPacketSrvA3),
		sizeof(D2GSPacketSrvA4),
		sizeof(D2GSPacketSrvA5),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvA7),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvA9),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvAB),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvAD),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvAF),
		0,
		sizeof(D2GSPacketSrvB1),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvB3),
	};

	if (nBufferSize == 0)
	{
		return 0;
	}

	const uint8_t nHeader = pBuffer->data[0];
	if (nHeader >= std::size(gServerPacketSizeTable))
	{
		return 0;
	}

	*pSize = gServerPacketSizeTable[nHeader];
	if (*pSize >= 0)
	{
		return *pSize;
	}

	switch (nHeader)
	{
	case 0x16u:
	{
		if (nBufferSize < 0xD)
		{
			return 0;
		}

		*pSize = *(uint16_t*)&pBuffer->data[1];
		return *pSize;
	}
	case 0x26u:
	{
		if (nBufferSize < 10)
		{
			return 0;
		}

		const char* szName = (const char*)&pBuffer->data[10];
		const int32_t nSizeUpToMessage = SStrLen(szName) + 11;
		if (nBufferSize < nSizeUpToMessage)
		{
			return 0;
		}

		const char* szMessage = &szName[SStrLen(szName) + 1];
		const int32_t nPacketSize = nSizeUpToMessage + SStrLen(szMessage) + 1;
		if (nBufferSize < nPacketSize)
		{
			return 0;
		}

		*pSize = nPacketSize;
		return *pSize;
	}
	case 0x3Eu:
	{
		if (nBufferSize < 2)
		{
			return 0;
		}

		*pSize = pBuffer->data[1];
		return *pSize;
	}
	case 0x5Bu:
	{
		if (nBufferSize < 0x22)
		{
			return 0;
		}

		*pSize = *(uint16_t*)&pBuffer->data[1];
		return *pSize;
	}
	case 0x94u:
	{
		if (nBufferSize < 9)
		{
			return 0;
		}

		*pSize = 3 * (pBuffer->data[1] + 2);
		return *pSize;
	}
	case 0x9Cu:
	case 0x9Du:
	{
		if (nBufferSize < 3)
		{
			return 0;
		}

		*pSize = pBuffer->data[2];
		return *pSize;
	}
	case 0xA6u:
	{
		if (nBufferSize < 4)
		{
			return 0;
		}

		*pSize = *(uint16_t*)&pBuffer->data[2];
		return *pSize;
	}
	case 0xA8u: // NOLINT error: switch has 2 consecutive identical branches [bugprone-branch-clone,-warnings-as-errors]
	{
		if (nBufferSize < 7)
		{
			return 0;
		}

		*pSize = pBuffer->data[6];
		return *pSize;
	}
	case 0xAAu:
	{
		if (nBufferSize < 7)
		{
			return 0;
		}

		*pSize = pBuffer->data[6];
		return *pSize;
	}
	case 0xACu:
	{
		if (nBufferSize < 0xD)
		{
			return 0;
		}

		*pSize = pBuffer->data[12];
		return *pSize;
	}
	case 0xAEu:
	{
		if (nBufferSize < 2)
		{
			return 0;
		}

		if (pBuffer->data[1])
		{
			*pSize = pBuffer->data[1] + 1;
		}
		else
		{
			*pSize = 2;
		}
		return *pSize;
	}
	case 0xB2u:
	{
		if (nBufferSize < 8)
		{
			return 0;
		}

		*pSize = pBuffer->data[1] + 7;
		return *pSize;
	}
	default:
	{
		*pSize = 0;
		return 0;
	}
	}
}

//D2Net.0x6FC01E60 (#10031)
int32_t __fastcall SERVER_GetClientPacketSize(D2NetReceiveBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize)
{
	constexpr int32_t gClientPacketSizeTable[] =
	{
		0,
		sizeof(D2GSPacketClt01),
		sizeof(D2GSPacketClt02),
		sizeof(D2GSPacketClt03),
		sizeof(D2GSPacketClt04),
		sizeof(D2GSPacketClt05),
		sizeof(D2GSPacketClt06),
		sizeof(D2GSPacketClt07),
		sizeof(D2GSPacketClt08),
		sizeof(D2GSPacketClt09),
		sizeof(D2GSPacketClt0A),
		sizeof(D2GSPacketClt0B),
		sizeof(D2GSPacketClt0C),
		sizeof(D2GSPacketClt0D),
		sizeof(D2GSPacketClt0E),
		sizeof(D2GSPacketClt0F),
		sizeof(D2GSPacketClt10),
		sizeof(D2GSPacketClt11),
		sizeof(D2GSPacketClt12),
		sizeof(D2GSPacketClt13),
		VARIABLE_PACKET_SIZE,
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketClt16),
		sizeof(D2GSPacketClt17),
		sizeof(D2GSPacketClt18),
		sizeof(D2GSPacketClt19),
		sizeof(D2GSPacketClt1A),
		sizeof(D2GSPacketClt1B),
		sizeof(D2GSPacketClt1C),
		sizeof(D2GSPacketClt1D),
		sizeof(D2GSPacketClt1E),
		sizeof(D2GSPacketClt1F),
		sizeof(D2GSPacketClt20),
		sizeof(D2GSPacketClt21),
		sizeof(D2GSPacketClt22),
		sizeof(D2GSPacketClt23),
		sizeof(D2GSPacketClt24),
		sizeof(D2GSPacketClt25),
		sizeof(D2GSPacketClt26),
		sizeof(D2GSPacketClt27),
		sizeof(D2GSPacketClt28),
		sizeof(D2GSPacketClt29),
		sizeof(D2GSPacketClt2A),
		0,
		0,
		sizeof(D2GSPacketClt2D),
		sizeof(D2GSPacketClt2E),
		sizeof(D2GSPacketClt2F),
		sizeof(D2GSPacketClt30),
		sizeof(D2GSPacketClt31),
		sizeof(D2GSPacketClt32),
		sizeof(D2GSPacketClt33),
		sizeof(D2GSPacketClt34),
		sizeof(D2GSPacketClt35),
		sizeof(D2GSPacketClt36),
		sizeof(D2GSPacketClt37),
		sizeof(D2GSPacketClt38),
		sizeof(D2GSPacketClt39),
		sizeof(D2GSPacketClt3A),
		sizeof(D2GSPacketClt3B),
		sizeof(D2GSPacketClt3C),
		sizeof(D2GSPacketClt3D),
		sizeof(D2GSPacketClt3E),
		sizeof(D2GSPacketClt3F),
		sizeof(D2GSPacketClt40),
		sizeof(D2GSPacketClt41),
		sizeof(D2GSPacketClt42),
		sizeof(D2GSPacketClt43),
		sizeof(D2GSPacketClt44),
		sizeof(D2GSPacketClt45),
		sizeof(D2GSPacketClt46),
		sizeof(D2GSPacketClt47),
		sizeof(D2GSPacketClt48),
		sizeof(D2GSPacketClt49),
		0,
		sizeof(D2GSPacketClt4B),
		sizeof(D2GSPacketClt4C),
		sizeof(D2GSPacketClt4D),
		0,
		sizeof(D2GSPacketClt4F),
		sizeof(D2GSPacketClt50),
		sizeof(D2GSPacketClt51),
		sizeof(D2GSPacketClt52),
		sizeof(D2GSPacketClt53),
		sizeof(D2GSPacketClt54),
		0,
		0,
		0,
		sizeof(D2GSPacketClt58),
		sizeof(D2GSPacketClt59),
		0,
		0,
		0,
		sizeof(D2GSPacketClt5D),
		sizeof(D2GSPacketClt5E),
		sizeof(D2GSPacketClt5F),
		sizeof(D2GSPacketClt60),
		sizeof(D2GSPacketClt61),
		sizeof(D2GSPacketClt62),
		sizeof(D2GSPacketClt63),
		sizeof(D2GSPacketClt64),
		sizeof(D2GSPacketClt65),
		sizeof(D2GSPacketClt66),
		sizeof(D2GSPacketClt67),
		sizeof(D2GSPacketClt68),
		sizeof(D2GSPacketClt69),
		sizeof(D2GSPacketClt6A),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketClt6C),
		sizeof(D2GSPacketClt6D),
		0,
		sizeof(D2GSPacketClt6F),
	};

	if (nBufferSize == 0)
	{
		return 0;
	}

	const uint8_t nHeader = pBuffer->data[0];
	if (nHeader == CLIENT_PACKET_HEADER_ADMIN)
	{
		*pSize = 16;
		return 1;
	}

	if (nHeader >= std::size(gClientPacketSizeTable))
	{
		return 0;
	}

	*pSize = gClientPacketSizeTable[nHeader];
	if (*pSize >= 0)
	{
		return *pSize;
	}

	switch (nHeader)
	{
	case 0x14:
	case 0x15:
	{
		if (nBufferSize < 3)
		{
			return 0;
		}

		const char* szMessage = (const char*)&pBuffer->data[3];
		const int32_t nSizeUpToSecondString = SStrLen(szMessage) + 4;
		if (nBufferSize < nSizeUpToSecondString)
		{
			return 0;
		}

		const char* szSecondString = &szMessage[SStrLen(szMessage) + 1];
		const size_t nSecondStringLength = SStrLen(szSecondString);
		const int32_t nSizeUpToTrailingData = nSizeUpToSecondString + nSecondStringLength + 1;
		if (nBufferSize < nSizeUpToTrailingData)
		{
			return 0;
		}

		// Byte following the second string gives the size of the remaining data
		const int32_t nPacketSize = nSizeUpToTrailingData + szSecondString[nSecondStringLength + 1] + 1;
		if (nBufferSize < nPacketSize)
		{
			return 0;
		}

		*pSize = nPacketSize;
		return *pSize;
	}
	case 0x6B:
	{
		if (nBufferSize < 6)
		{
			return 0;
		}

		*pSize = pBuffer->data[1] + 7;
		return *pSize;
	}
	default:
	{
		*pSize = 0;
		return *pSize;
	}
	}
}

//D2Net.0x6FC01FA0 (#10039)
D2NET_CLIENT_SendFunctionType __stdcall SERVER_GetClientSendFunction()
{
	return CLIENT_Send;
}

//D2Net.0x6FC01FB0 (#10040)
D2NET_SERVER_GetClientGameGUIDFunctionType __stdcall SERVER_GetClientGameGUIDFunction()
{
	return SERVER_GetClientGameGUID;
}

//D2Net.0x6FC01FC0
int32_t __fastcall SERVER_SendToLocalClientCallback(QServer* pUnused, D2NetReceiveBufferStrc* pPacketBuffer, int32_t nBufferSize)
{
	D2_MAYBE_UNUSED(pUnused);
	CLIENT_EnqueuePacketsFromLocalServer(pPacketBuffer, nBufferSize);
	return 1;
}

//D2Net.0x6FC01FE0
int32_t __fastcall SERVER_ValidateClientPacket(D2NetReceiveBufferStrc* pPacketBuffer, uint32_t nBufferSize, int32_t* pPacketSize, int32_t* a4, int32_t* pPacketQueue, int32_t* a6, int32_t nUnused1, int32_t nUnused2)
{
	int32_t nSize = 0;
	if (nBufferSize == 0 || !SERVER_GetClientPacketSize(pPacketBuffer, nBufferSize, &nSize))
	{
		return PACKET_VALIDATION_INCOMPLETE;
	}

	const uint8_t nHeader = pPacketBuffer->data[0];
	if ((nHeader >= 0x70u && nHeader != CLIENT_PACKET_HEADER_ADMIN) || nSize < 0 || nSize > MAX_MSG_SIZE)
	{
		return PACKET_VALIDATION_INVALID;
	}

	if (nSize > nBufferSize)
	{
		return PACKET_VALIDATION_INCOMPLETE;
	}

	*a4 = 0;
	*pPacketSize = nSize;

	if (nHeader < 0x66u)
	{
		*pPacketQueue = SERVER_PACKET_QUEUE_GAME;
	}
	else if (nHeader < 0x70u)
	{
		*pPacketQueue = SERVER_PACKET_QUEUE_SYSTEM;
	}
	else
	{
		*pPacketQueue = SERVER_PACKET_QUEUE_ADMIN;
	}

	*a6 = 100;

	if (*pPacketQueue == SERVER_PACKET_QUEUE_GAME || *pPacketQueue == SERVER_PACKET_QUEUE_ADMIN)
	{
		return PACKET_VALIDATION_QUEUED;
	}

	return PACKET_VALIDATION_QUEUED_SYSTEM;
}

//D2Net.0x6FC020B0
int32_t __fastcall SERVER_OnClientConnectedCallback(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4)
{
	// Tell the new client that the stream will be compressed from now on
	uint8_t compressionInfo[2] = { SERVER_PACKET_HEADER_COMPRESSION_INFO, 1 };

	SERVER_SendPacketToClient(D2NET_SEND_SYSTEM, nClientId, compressionInfo, sizeof(compressionInfo));
	return 1;
}

//D2Net.0x6FC020E0
int32_t __fastcall SERVER_OnClientDisconnectedCallback(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4)
{
	// Enqueue D2CLTSYS_REMOVEFROMGAME on behalf of the disconnected client
	const uint8_t removeFromGame[1] = { 0x6F };

	FOG_10175(gpServer, removeFromGame, sizeof(removeFromGame), nClientId);
	return 1;
}

//D2Net.0x6FC02110
int32_t __fastcall SERVER_DisableLocalClientCompression()
{
	uint8_t compressionInfo[2] = { SERVER_PACKET_HEADER_COMPRESSION_INFO, 0 };

	return SERVER_SendPacketToClient(D2NET_SEND_SYSTEM, 0, compressionInfo, sizeof(compressionInfo));
}

//D2Net.0x6FC02130 (#10002)
int32_t __stdcall SERVER_WaitForSingleObject(uint32_t dwMilliseconds)
{
	return FOG_WaitForSingleObject(gpServer, dwMilliseconds);
}

//D2Net.0x6FC02150 (#10003)
void __stdcall SERVER_Initialize(int32_t a1, int32_t a2)
{
	gpServer = FOG_InitializeServer(a1, 3, GAME_PORT, a2, SERVER_ValidateClientPacket, SERVER_OnClientConnectedCallback, SERVER_OnClientDisconnectedCallback, SERVER_SendToLocalClientCallback);
}

//D2Net.0x6FC02190 (#10035)
int32_t __stdcall D2NET_10035(int32_t nIndex, int32_t nValue)
{
	return FOG_10186(gpServer, nIndex, nValue);
}

//D2Net.0x6FC021B0 (#10036)
void __stdcall D2NET_10036(int32_t a1, int32_t a2)
{
	FOG_10187(gpServer, a2, a1);
}

//D2Net.0x6FC021D0 (#10026)
void __stdcall SERVER_SetMaxClientsPerGame(int32_t nMaxClients)
{
	FOG_SetMaxClientsPerGame(gpServer, nMaxClients);
}

//D2Net.0x6FC021F0 (#10027)
int32_t __stdcall D2NET_10027()
{
	return FOG_10180(gpServer);
}

//D2Net.0x6FC02200 (#10023)
void __stdcall SERVER_SetHackListEnabled(BOOL bEnabled)
{
	FOG_SetHackListEnabled(gpServer, bEnabled);
}

//D2Net.0x6FC02220 (#10004)
void __stdcall SERVER_Release()
{
	// D2GSPacketSrvAF
	const uint8_t disconnectPacket[1] = { 0xAF };
	FOG_10152(gpServer, disconnectPacket, sizeof(disconnectPacket));

	gpServer = nullptr;
}

//D2Net.0x6FC02250 (#10010)
int32_t __stdcall SERVER_ReadGamePacket(uint8_t* pBuffer, int32_t nBufferSize)
{
	return FOG_10156(gpServer, SERVER_PACKET_QUEUE_GAME, pBuffer, nBufferSize);
}

//D2Net.0x6FC02270 (#10011)
int32_t __stdcall SERVER_ReadSystemPacket(uint8_t* pBuffer, int32_t nBufferSize)
{
	return FOG_10156(gpServer, SERVER_PACKET_QUEUE_SYSTEM, pBuffer, nBufferSize);
}

//D2Net.0x6FC02290 (#10012)
int32_t __stdcall SERVER_ReadAdminPacket(uint8_t* pBuffer, int32_t nBufferSize)
{
	return FOG_10156(gpServer, SERVER_PACKET_QUEUE_ADMIN, pBuffer, nBufferSize);
}

//D2Net.0x6FC022B0 (#10006)
uint32_t __stdcall SERVER_SendPacketToClient(int8_t nSendMode, int32_t nClientId, void* pPacketArg, uint32_t nPacketSize)
{
	const uint8_t* pPacket = (const uint8_t*)pPacketArg;

	D2_ASSERT(nPacketSize <= MAX_MSG_SIZE);

	if (NET_IsLocalConnection())
	{
		CLIENT_EnqueuePacketsFromLocalServer((D2NetReceiveBufferStrc*)pPacket, nPacketSize);
		return nPacketSize;
	}

	if (nSendMode == D2NET_SEND_SYSTEM && *pPacket == SERVER_PACKET_HEADER_COMPRESSION_INFO)
	{
		return FOG_10157(gpServer, nClientId, pPacket, nPacketSize) != 0 ? nPacketSize : 0;
	}

	FOG_10222(pPacket, nPacketSize);
	if (nSendMode == D2NET_SEND_UNCOMPRESSED)
	{
		return FOG_10157(gpServer, nClientId, pPacket, nPacketSize) != 0 ? nPacketSize : 0;
	}

	// Compressed frame, prefixed by its total size on 1 byte (< 0xF0) or on 12 bits spread over 2 bytes (0xFX XX)
	uint8_t frame[1036] = {};
	const uint32_t nCompressedSize = FOG_10223(&frame[2], 1032, pPacket, nPacketSize);
	D2_ASSERT(nCompressedSize);

	if (nCompressedSize + 1 < 0xF0)
	{
		frame[1] = nCompressedSize + 1;
		return FOG_10157(gpServer, nClientId, &frame[1], nCompressedSize + 1) != 0 ? nCompressedSize + 1 : 0;
	}

	const uint32_t nFrameSize = nCompressedSize + 2;
	frame[0] = BYTE1(nFrameSize) | 0xF0;
	frame[1] = nCompressedSize + 2;
	return FOG_10157(gpServer, nClientId, frame, nFrameSize) != 0 ? nFrameSize : 0;
}

//D2Net.0x6FC02410 (#10014)
void __stdcall SERVER_GetIpAddressStringFromClientId(int32_t nClientId, char* szBuffer, int32_t nBufferSize)
{
	FOG_10159(gpServer, nClientId, szBuffer, nBufferSize);
}

//D2Net.0x6FC02430 (#10038)
int32_t __stdcall SERVER_GetIpAddressFromClientId(int32_t nClientId)
{
	return FOG_10158(gpServer, nClientId);
}

//D2Net.0x6FC02450 (#10037)
SOCKET __stdcall SERVER_GetSocketFromClientId(int32_t nClientId)
{
	return FOG_10161(gpServer, nClientId);
}

//D2Net.0x6FC02470 (#10015)
void __stdcall D2NET_10015(int32_t nClientId, const char* szFile, int32_t nLine)
{
	FOG_10162(gpServer, nClientId, szFile, nLine);
}

//D2Net.0x6FC02490 (#10032)
void __stdcall D2NET_10032(int32_t nClientId, const char* szFile, int32_t nLine)
{
	FOG_10163(gpServer, nClientId, szFile, nLine);
}

//D2Net.0x6FC024B0 (#10033)
int32_t __stdcall D2NET_10033(int32_t a1, int32_t a2, int32_t a3)
{
	return FOG_10164(gpServer, a1, a2, a3);
}

//D2Net.0x6FC024D0 (#10034)
int32_t __stdcall D2NET_10034(int32_t nClientId, int32_t a2, int32_t a3)
{
	return FOG_10166(gpServer, nClientId, a2, a3);
}

//D2Net.0x6FC024F0 (#10016)
void __stdcall D2NET_10016(int32_t nClientId)
{
	FOG_10165(gpServer, nClientId, __FILE__, __LINE__);
}

//D2Net.0x6FC02510 (#10018)
void __stdcall D2NET_10018(int32_t a1)
{
	if (!gpServer)
	{
		return;
	}

	FOG_10170(gpServer, a1);
}

//D2Net.0x6FC02530 (#10019)
int32_t __stdcall SERVER_SetRemoveClientCallback(D2NET_RemoveClientCallback pfRemoveClient)
{
	return FOG_10171(gpServer, pfRemoveClient);
}

//D2Net.0x6FC02550 (#10020)
int32_t __stdcall SERVER_SetClientGameGUID(int32_t nClientId, int32_t dwGameGuid)
{
	if (nClientId)
	{
		return FOG_10172(gpServer, nClientId, dwGameGuid);
	}

	gnLocalClientGameGUID = dwGameGuid;

	return dwGameGuid;
}

//D2Net.0x6FC02580 (#10021)
int32_t __stdcall SERVER_GetClientGameGUID(int32_t nClientId)
{
	if (nClientId)
	{
		return FOG_10173(gpServer, nClientId);
	}

	return gnLocalClientGameGUID;
}

//D2Net.0x6FC025A0
int32_t __fastcall SERVER_EnqueueLocalClientPacket(const uint8_t* pPacket, uint16_t nPacketSize)
{
	D2_ASSERT(nPacketSize <= MAX_MSG_SIZE);

	// The local client always has client id 0
	return FOG_10175(gpServer, pPacket, nPacketSize, 0) != 0;
}

//D2Net.0x6FC025F0 (#10022)
int32_t __stdcall D2NET_10022(uint32_t dwMilliseconds)
{
	return FOG_10177(gpServer, dwMilliseconds);
}

//D2Net.0x6FC02610 (#10028)
int32_t __stdcall D2NET_10028_Return()
{
	return FOG_10182_Return(gpServer);
}

//D2Net.0x6FC02620 (#10029)
int32_t __stdcall D2NET_10029_Return(int32_t nUnused)
{
	return FOG_10183_Return(gpServer, nUnused);
}
