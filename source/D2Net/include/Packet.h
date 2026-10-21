#pragma once


#include <cstdint>


constexpr uint32_t MAX_MSG_SIZE = 516;

// Server packets with a header >= this value are system packets, the others are game packets
constexpr uint8_t SERVER_SYSTEM_PACKET_HEADER_MIN = 0xAE;
// One past the highest valid server packet header
constexpr uint8_t SERVER_PACKET_HEADER_END = 0xB4;
// Sent by the server to tell the client whether the stream is compressed
constexpr uint8_t SERVER_PACKET_HEADER_COMPRESSION_INFO = 0xAE;


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
