#pragma once

#pragma pack(1)

//Sub-Structs
struct D2MessageStrc		//sizeof 0x04
{
	BYTE nMenu;				//0x00
	BYTE pad0x01;			//0x01
	WORD nStringId;			//0x02
};

struct D2MessageListStrc	//sizeof 0x22
{
	BYTE nCount;			//0x00
	BYTE pad0x01;			//0x01
	D2MessageStrc pMessages[8];	//0x02
};

#pragma pack()
