#include "Server.h"

#include <algorithm>

#include <Fog.h>
#include <Storm.h>

#include <D2PacketDef.h>

#include "Client.h"
#include "D2Net.h"


D2ServerStrc* gpServer;
int32_t dword_6FC0B26C;


constexpr int32_t VARIABLE_PACKET_SIZE = -1;


//D2Net.0x6FC01B30 (#10024)
int32_t __stdcall SERVER_WSAGetLastError()
{
	return WSAGetLastError();
}

//D2Net.0x6FC01B60 (#10030)
int32_t __fastcall SERVER_GetServerPacketSize(D2PacketBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize)
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
		//sizeof(D2GSPacketSrv1D),
		3, // 0x1D
		//sizeof(D2GSPacketSrv1E),
		4, // 0x1E
		//sizeof(D2GSPacketSrv1F),
		6, // 0x1F
		//sizeof(D2GSPacketSrv20),
		10, // 0x20
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
		//sizeof(D2GSPacketSrv4E),
		7, // 0x4E
		sizeof(D2GSPacketSrv4F),
		sizeof(D2GSPacketSrv50),
		sizeof(D2GSPacketSrv51),
		sizeof(D2GSPacketSrv52),
		sizeof(D2GSPacketSrv53),
		//sizeof(D2GSPacketSrv54),
		3, // 0x54
		0,
		0,
		//sizeof(D2GSPacketSrv57),
		14, // 0x57
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
		//sizeof(D2GSPacketSrv95),
		13,
		sizeof(D2GSPacketSrv96),
		sizeof(D2GSPacketSrv97),
		sizeof(D2GSPacketSrv98),
		sizeof(D2GSPacketSrv99),
		sizeof(D2GSPacketSrv9A),
		sizeof(D2GSPacketSrv9B),
		VARIABLE_PACKET_SIZE,
		VARIABLE_PACKET_SIZE,
		//sizeof(D2GSPacketSrv9E),
		7, // 0x9E
		//sizeof(D2GSPacketSrv9F),
		8, // 0x9F
		//sizeof(D2GSPacketSrvA0),
		10, // 0xA0
		//sizeof(D2GSPacketSrvA1),
		7, // 0xA1
		//sizeof(D2GSPacketSrvA2),
		8, // 0xA2
		sizeof(D2GSPacketSrvA3),
		sizeof(D2GSPacketSrvA4),
		sizeof(D2GSPacketSrvA5),
		VARIABLE_PACKET_SIZE,
		//sizeof(D2GSPacketSrvA7),
		7, // 0xA7
		VARIABLE_PACKET_SIZE,
		//sizeof(D2GSPacketSrvA9),
		7, // 0xA9
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvAB),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvAD),
		VARIABLE_PACKET_SIZE,
		//sizeof(D2GSPacketSrvAF),
		1, // 0xAF
		0,
		sizeof(D2GSPacketSrvB1),
		VARIABLE_PACKET_SIZE,
		sizeof(D2GSPacketSrvB3),
	};

	if (nBufferSize < 1)
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

		const char* v6 = (const char*)&pBuffer->data[10];
		const int32_t v7 = SStrLen(v6) + 11;
		if (nBufferSize < v7)
		{
			return 0;
		}

		const int32_t v8 = SStrLen(v6);
		const int32_t result = v7 + SStrLen(&v6[v8 + 1]) + 1;
		if (nBufferSize < result)
		{
			return 0;
		}

		*pSize = result;
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
	case 0x3Eu:
	{
		if (nBufferSize < 2)
		{
			return 0;
		}

		*pSize = pBuffer->data[1];
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
	case 0xA8u:
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
int32_t __fastcall SERVER_GetClientPacketSize(D2PacketBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize)
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
		//sizeof(D2GSPacketClt1A),
		9, // 0x1A
		//sizeof(D2GSPacketClt1B),
		9, // 0x1B
		//sizeof(D2GSPacketClt1C),
		3, // 0x1C
		//sizeof(D2GSPacketClt1D),
		9, // 0x1D
		//sizeof(D2GSPacketClt1E),
		9, // 0x1E
		//sizeof(D2GSPacketClt1F),
		17, // 0x1F
		sizeof(D2GSPacketClt20),
		//sizeof(D2GSPacketClt21),
		9, // 0x21
		//sizeof(D2GSPacketClt22),
		5, // 0x22
		sizeof(D2GSPacketClt23),
		//sizeof(D2GSPacketClt24),
		5, // 0x24
		//sizeof(D2GSPacketClt25),
		9, // 0x25
		sizeof(D2GSPacketClt26),
		sizeof(D2GSPacketClt27),
		//sizeof(D2GSPacketClt28),
		9, // 0x28
		sizeof(D2GSPacketClt29),
		sizeof(D2GSPacketClt2A),
		0,
		0,
		//sizeof(D2GSPacketClt2D),
		1, // 0x2D
		//sizeof(D2GSPacketClt2E),
		3, // 0x2E
		//sizeof(D2GSPacketClt2F),
		9, // 0x2F
		//sizeof(D2GSPacketClt30),
		9, // 0x30
		sizeof(D2GSPacketClt31),
		sizeof(D2GSPacketClt32),
		sizeof(D2GSPacketClt33),
		//sizeof(D2GSPacketClt34),
		5, // 0x34
		//sizeof(D2GSPacketClt35),
		17, // 0x35
		//sizeof(D2GSPacketClt36),
		9, // 0x36
		//sizeof(D2GSPacketClt37),
		5, // 0x37
		sizeof(D2GSPacketClt38),
		//sizeof(D2GSPacketClt39),
		5, // 0x39
		sizeof(D2GSPacketClt3A),
		sizeof(D2GSPacketClt3B),
		sizeof(D2GSPacketClt3C),
		sizeof(D2GSPacketClt3D),
		//sizeof(D2GSPacketClt3E),
		5, // 0x3E
		//sizeof(D2GSPacketClt3F),
		3, // 0x3F
		//sizeof(D2GSPacketClt40),
		1, // 0x40
		sizeof(D2GSPacketClt41),
		sizeof(D2GSPacketClt42),
		//sizeof(D2GSPacketClt43),
		1, // 0x43
		sizeof(D2GSPacketClt44),
		sizeof(D2GSPacketClt45),
		//sizeof(D2GSPacketClt46),
		13, // 0x46
		//sizeof(D2GSPacketClt47),
		13, // 0x47
		//sizeof(D2GSPacketClt48),
		1, // 0x48
		sizeof(D2GSPacketClt49),
		0,
		//sizeof(D2GSPacketClt4B),
		9, // 0x4B
		//sizeof(D2GSPacketClt4C),
		5, // 0x4C
		//sizeof(D2GSPacketClt4D),
		3, // 0x4D
		0,
		sizeof(D2GSPacketClt4F),
		//sizeof(D2GSPacketClt50),
		9, // 0x50
		sizeof(D2GSPacketClt51),
		//sizeof(D2GSPacketClt52),
		5, // 0x52
		//sizeof(D2GSPacketClt53),
		1, // 0x53
		//sizeof(D2GSPacketClt54),
		1, // 0x54
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
		//sizeof(D2GSPacketClt5F),
		5, // 0x5F
		//sizeof(D2GSPacketClt60),
		1, // 0x60
		//sizeof(D2GSPacketClt61),
		3, // 0x61
		//sizeof(D2GSPacketClt62),
		5, // 0x62
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

	if (nBufferSize < 1)
	{
		return 0;
	}

	const uint8_t nHeader = pBuffer->data[0];
	if (nHeader == 0xFF)
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
			*pSize = 0;
			return *pSize;
		}

		const char* v7 = (const char*)&pBuffer->data[3];
		const int32_t v8 = SStrLen(v7) + 4;
		if (nBufferSize < v8)
		{
			*pSize = 0;
			return *pSize;
		}

		const char* v9 = &v7[SStrLen(v7) + 1];
		const int32_t v10 = v8 + SStrLen(v9) + 1;
		if (nBufferSize < v10)
		{
			*pSize = 0;
			return *pSize;
		}

		const int32_t result = v10 + v9[SStrLen(v9) + 1] + 1;
		if (nBufferSize < result)
		{
			return 0;
		}

		*pSize = result;
		return *pSize;
	}
	case 0x6B:
	{
		if (nBufferSize < 6)
		{
			return 0;
		}

		*pSize = *(uint8_t*)(pBuffer + 1) + 7;
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
int32_t* __stdcall D2NET_10039()
{
	return (int32_t*)CLIENT_Send;
}

//D2Net.0x6FC01FB0 (#10040)
int32_t* __stdcall D2NET_10040()
{
	return (int32_t*)SERVER_GetClientGameGUID;
}

//D2Net.0x6FC01FC0
int32_t __fastcall SERVER_ReadPacketFromBufferCallback(int32_t nUnused, D2PacketBufferStrc* pPacketBuffer, int32_t nBufferSize)
{
	CLIENT_ReadPacketFromBuffer(pPacketBuffer, nBufferSize);
	return 1;
}

//D2Net.0x6FC01FE0
int32_t __fastcall SERVER_ValidateClientPacket(D2PacketBufferStrc* a1, uint32_t a2, int32_t* a3, int32_t* a4, int32_t* a5, int32_t* a6, int32_t nUnused1, int32_t nUnused2)
{
	int32_t nSize = 0;
	if (a2 < 1 || !SERVER_GetClientPacketSize(a1, a2, &nSize))
	{
		return 3;
	}

	const uint8_t nHeader = a1->data[0];
	if ((nHeader >= 0x70u && nHeader != 0xFF) || nSize < 0 || nSize > MAX_MSG_SIZE)
	{
		return 4;
	}

	if (nSize > a2)
	{
		return 3;
	}

	*a4 = 0;
	*a3 = nSize;

	if (nHeader < 0x66u)
	{
		*a5 = 1;
	}
	else if (nHeader < 0x70u)
	{
		*a5 = 0;
	}
	else
	{
		*a5 = 2;
	}

	*a6 = 100;

	if (*a5 == 1 || *a5 == 2)
	{
		return 1;
	}

	return 2;
}

//D2Net.0x6FC020B0
int32_t __fastcall sub_6FC020B0(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4)
{
	uint8_t data[2] = { 0xAE, 1 };

	D2NET_10006(0, nClientId, data, sizeof(data));
	return 1;
}

//D2Net.0x6FC020E0
int32_t __fastcall sub_6FC020E0(int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
	const uint8_t data[1] = { 0x6F };

	FOG_10175(gpServer, data, sizeof(data), a2);
	return 1;
}

//D2Net.0x6FC02110
int32_t __fastcall sub_6FC02110()
{
	uint8_t data[2] = { 0xAE, 0 };

	return D2NET_10006(0, 0, data, sizeof(data));
}

//D2Net.0x6FC02130 (#10002)
int32_t __stdcall SERVER_WaitForSingleObject(uint32_t dwMilliseconds)
{
	return FOG_WaitForSingleObject(gpServer, dwMilliseconds);
}

//D2Net.0x6FC02150 (#10003)
void __stdcall SERVER_Initialize(int32_t a1, int32_t a2)
{
	gpServer = (D2ServerStrc*)FOG_InitializeServer(a1, 3, GAME_PORT, a2, SERVER_ValidateClientPacket, sub_6FC020B0, sub_6FC020E0, SERVER_ReadPacketFromBufferCallback);
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
void __stdcall D2NET_10026(int32_t a1)
{
	FOG_10151(gpServer, a1);
}

//D2Net.0x6FC021F0 (#10027)
int32_t __stdcall D2NET_10027()
{
	return FOG_10180(gpServer);
}

//D2Net.0x6FC02200 (#10023)
void __stdcall D2NET_10023(int32_t a1)
{
	FOG_10178(gpServer, a1);
}

//D2Net.0x6FC02220 (#10004)
void __stdcall SERVER_Release()
{
	const uint8_t data[1] = { 0xAF };
	FOG_10152(gpServer, data, sizeof(data));

	gpServer = nullptr;
}

// TODO: Better name
//D2Net.0x6FC02250 (#10010)
int32_t __stdcall SERVER_ReadFromMessageList1(uint8_t* pBuffer, int32_t nBufferSize)
{
	return FOG_10156(gpServer, 1, pBuffer, nBufferSize);
}

// TODO: Better name
//D2Net.0x6FC02270 (#10011)
int32_t __stdcall SERVER_ReadFromMessageList0(uint8_t* pBuffer, int32_t nBufferSize)
{
	return FOG_10156(gpServer, 0, pBuffer, nBufferSize);
}

// TODO: Better name
//D2Net.0x6FC02290 (#10012)
int32_t __stdcall SERVER_ReadFromMessageList2(uint8_t* pBuffer, int32_t nBufferSize)
{
	return FOG_10156(gpServer, 2, pBuffer, nBufferSize);
}

//D2Net.0x6FC022B0 (#10006)
uint32_t __stdcall D2NET_10006(int8_t a1, int32_t nClientId, void* pBufferArg, uint32_t nBufferSize)
{
	const uint8_t* pBuffer = (const uint8_t*)pBufferArg;

	D2_ASSERT(nBufferSize <= MAX_MSG_SIZE);

	if (sub_6FC01A00())
	{
		CLIENT_ReadPacketFromBuffer((D2PacketBufferStrc*)pBuffer, nBufferSize);
		return nBufferSize;
	}

	if (!a1 && *pBuffer == 0xAE)
	{
		return FOG_10157(gpServer, nClientId, pBuffer, nBufferSize) != 0 ? nBufferSize : 0;
	}

	FOG_10222(pBuffer, nBufferSize);
	if (a1 == 2)
	{
		return FOG_10157(gpServer, nClientId, pBuffer, nBufferSize) != 0 ? nBufferSize : 0;
	}

	uint8_t data[1036] = {};
	const uint32_t nSize = FOG_10223(&data[2], 1032, pBuffer, nBufferSize);
	D2_ASSERT(nSize);

	if (nSize + 1 < 0xF0)
	{
		data[1] = nSize + 1;
		return FOG_10157(gpServer, nClientId, &data[1], nSize + 1) != 0 ? nSize + 1 : 0;
	}

	const uint32_t v6 = nSize + 2;
	data[0] = BYTE1(v6) | 0xF0;
	data[1] = nSize + 2;
	return FOG_10157(gpServer, nClientId, data, v6) != 0 ? v6 : 0;
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
int32_t __stdcall D2NET_10019(void* pfCallback)
{
	return FOG_10171(gpServer, pfCallback);
}

//D2Net.0x6FC02550 (#10020)
int32_t __stdcall SERVER_SetClientGameGUID(int32_t nClientId, int32_t a2)
{
	if (nClientId)
	{
		return FOG_10172(gpServer, nClientId, a2);
	}

	dword_6FC0B26C = a2;

	return a2;
}

//D2Net.0x6FC02580 (#10021)
int32_t __stdcall SERVER_GetClientGameGUID(int32_t nClientId)
{
	if (nClientId)
	{
		return FOG_10173(gpServer, nClientId);
	}

	return dword_6FC0B26C;
}

//D2Net.0x6FC025A0
int32_t __fastcall SERVER_EnqueuePacketToMessageList(const uint8_t* pBuffer, int32_t nBufferSize)
{
	D2_ASSERT(nBufferSize <= MAX_MSG_SIZE);

	return FOG_10175(gpServer, pBuffer, nBufferSize, 0) != 0;
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
