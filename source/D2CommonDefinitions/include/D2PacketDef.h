#pragma once

#pragma pack(1)

//Sub-Structs
struct D2MessageStrc		//sizeof 0x04
{
	uint8_t nMenu;				//0x00
	uint8_t pad0x01;			//0x01
	uint16_t nStringId;			//0x02
};

struct D2MessageListStrc	//sizeof 0x22
{
	uint8_t nCount;			//0x00
	uint8_t pad0x01;			//0x01
	D2MessageStrc pMessages[8];	//0x02
};

#pragma pack()
