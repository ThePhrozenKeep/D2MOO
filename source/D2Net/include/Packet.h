#pragma once


#include <cstdint>


constexpr uint32_t MAX_MSG_SIZE = 516;

#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
// 1.14d drops the 0xAD memory check request, adds the Warden packet 0xAE and moves the server system packets one id up;
// the client sends its Warden reply as 0x66 and its system requests one id up (Game.exe CLIENT_ReadPacketFromBuffer
// 0x0052AF32 / 0x0052AF3E, SERVER_ValidateClientPacket 0x0052B14A / 0x0052B168)
constexpr uint8_t SERVER_SYSTEM_PACKET_HEADER_MIN = 0xAF;
constexpr uint8_t SERVER_PACKET_HEADER_END = 0xB5;
constexpr uint8_t SERVER_PACKET_HEADER_COMPRESSION_INFO = 0xAF;
constexpr uint8_t SERVER_PACKET_HEADER_SAVE_PART = 0xB3;
constexpr uint8_t CLIENT_SYSTEM_PACKET_HEADER_MIN = 0x67;
constexpr uint8_t CLIENT_PACKET_HEADER_END = 0x71;
constexpr uint8_t CLIENT_PACKET_HEADER_SAVE_UPLOAD = 0x6C;
constexpr uint8_t CLIENT_PACKET_HEADER_REMOVE_FROM_GAME = 0x70;
#else
// Server packets with a header >= this value are system packets, the others are game packets
constexpr uint8_t SERVER_SYSTEM_PACKET_HEADER_MIN = 0xAE;
// One past the highest valid server packet header
constexpr uint8_t SERVER_PACKET_HEADER_END = 0xB4;
// Sent by the server to tell the client whether the stream is compressed
constexpr uint8_t SERVER_PACKET_HEADER_COMPRESSION_INFO = 0xAE;
// A part of the character save sent to the client
constexpr uint8_t SERVER_PACKET_HEADER_SAVE_PART = 0xB2;
// Client packets with a header >= this value (and below CLIENT_PACKET_HEADER_END) are system requests
constexpr uint8_t CLIENT_SYSTEM_PACKET_HEADER_MIN = 0x66;
// One past the highest valid client packet header (apart from the admin header 0xFF)
constexpr uint8_t CLIENT_PACKET_HEADER_END = 0x70;
// A part of the character save uploaded by the client
constexpr uint8_t CLIENT_PACKET_HEADER_SAVE_UPLOAD = 0x6B;
// Queued by the server on behalf of a client that disconnected
constexpr uint8_t CLIENT_PACKET_HEADER_REMOVE_FROM_GAME = 0x6F;
#endif


#pragma pack(push, 1)
// Packet waiting in one of the client's receive queues
struct D2NetQueuedPacketStrc
{
	uint8_t data[MAX_MSG_SIZE];
	uint32_t nPacketSize;
	uint32_t dwReceiveTick;
	D2NetQueuedPacketStrc* pNext;
};

// Accumulates decompressed bytes received from the server until they form complete packets
struct D2NetReceiveBufferStrc
{
	uint8_t data[1976];
	uint32_t nUsedBytes;
};
#pragma pack(pop)

#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
// 1.14d packet sizes by id (-1: variable size), as in Game.exe 0x00730AE8 (server packets) and 0x00730DC0 (client packets).
// Server.cpp keeps its line numbers (its asserts expand __LINE__), so these tables and the Warden cases live here.
constexpr int32_t gServerPacketSizeTable114d[] =
{
	1, 8, 1, 12, 1, 1, 1, 6, 6, 11, 6, 6, 9, 13, 12, 16, // 0x00
	16, 8, 26, 14, 18, 11, -1, 0, 15, 2, 2, 3, 5, 3, 4, 6, // 0x10
	10, 12, 12, 13, 90, 90, -1, 40, 103, 97, 15, 0, 8, 0, 0, 0, // 0x20
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 8, // 0x30
	13, 0, 6, 0, 0, 13, 0, 11, 11, 0, 0, 0, 16, 17, 7, 1, // 0x40
	15, 14, 42, 10, 3, 0, 0, 14, 7, 26, 40, -1, 5, 6, 38, 5, // 0x50
	7, 2, 7, 21, 0, 7, 7, 16, 21, 12, 12, 16, 16, 10, 1, 1, // 0x60
	1, 1, 1, 32, 10, 13, 6, 2, 21, 6, 13, 8, 6, 18, 5, 10, // 0x70
	0, 20, 29, 0, 0, 0, 0, 0, 0, 2, 6, 6, 11, 7, 10, 33, // 0x80
	13, 26, 6, 8, -1, 13, 9, 1, 7, 16, 17, 7, -1, -1, 7, 8, // 0x90
	10, 7, 8, 24, 3, 8, -1, 7, -1, 7, -1, 7, -1, 0, -1, -1, // 0xA0
	1, 0, 53, -1, 5, // 0xB0
};
constexpr int32_t gClientPacketSizeTable114d[] =
{
	0, 5, 9, 5, 9, 5, 9, 9, 5, 9, 9, 1, 5, 9, 9, 5, // 0x00
	9, 9, 1, 9, -1, -1, 13, 5, 17, 5, 9, 9, 3, 9, 9, 17, // 0x10
	13, 9, 5, 9, 5, 9, 13, 9, 9, 9, 9, 0, 0, 1, 3, 9, // 0x20
	9, 9, 17, 17, 5, 17, 9, 5, 13, 5, 3, 3, 9, 5, 5, 3, // 0x30
	1, 1, 1, 1, 17, 9, 13, 13, 1, 9, 0, 9, 5, 3, 0, 7, // 0x40
	9, 9, 5, 1, 1, 0, 0, 0, 3, 17, 0, 0, 0, 7, 6, 5, // 0x50
	1, 3, 5, 5, 0, 0, -1, 46, 37, 1, 1, 1, -1, 13, 1, 0, // 0x60
	1, // 0x70
};
#define D2NET_SERVER_PACKET_SIZES gServerPacketSizeTable114d
#define D2NET_CLIENT_PACKET_SIZES gClientPacketSizeTable114d

// The Warden packet (server 0xAE) and its reply (client 0x66): { id, uint16_t nDataSize, data }; a size above 0x1FD
// counts as no data (Game.exe SERVER_GetServerPacketSize 0x0052BAB4, SERVER_GetClientPacketSize 0x0052BCE0)
inline int32_t D2NET_GetWardenPacketSize(const D2NetReceiveBufferStrc* pBuffer, uint32_t nBufferSize, int32_t* pSize)
{
	if (nBufferSize < 3)
	{
		return 0;
	}

	uint16_t nDataSize = uint16_t(pBuffer->data[1] | (pBuffer->data[2] << 8));
	if (nDataSize > 0x1FD)
	{
		nDataSize = 0;
	}
	*pSize = nDataSize + 3;
	return *pSize;
}
#define D2NET_SERVER_EXTRA_PACKET_CASES case 0xAEu: return D2NET_GetWardenPacketSize(pBuffer, nBufferSize, pSize);
#define D2NET_CLIENT_EXTRA_PACKET_CASES case 0x66u: return D2NET_GetWardenPacketSize(pBuffer, nBufferSize, pSize);
#else
#define D2NET_SERVER_PACKET_SIZES gServerPacketSizeTable
#define D2NET_CLIENT_PACKET_SIZES gClientPacketSizeTable
#define D2NET_SERVER_EXTRA_PACKET_CASES
#define D2NET_CLIENT_EXTRA_PACKET_CASES
#endif
