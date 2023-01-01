#pragma once

#include <cstdint>


constexpr uint32_t MAX_MSG_SIZE = 516;


#pragma pack(push, 1)
struct D2PacketStrc
{
	uint8_t data[MAX_MSG_SIZE];
	uint32_t nPacketSize;
	uint32_t dwTickCount;
	D2PacketStrc* pNext;
};

struct D2PacketBufferStrc
{
	uint8_t data[1976];
	uint32_t nUsedBytes;
};
#pragma pack(pop)
