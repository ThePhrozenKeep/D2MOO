#pragma once

#include "CommonDefinitions.h"

#define MAX_TEXT_LIST_NODES 8

struct D2MessageStrc			//sizeof 0x04
{
	uint8_t nMenu;				//0x00
	uint8_t pad0x01;			//0x01
	uint16_t nStringId;			//0x02
};

struct D2MessageListStrc		//sizeof 0x22
{
	uint8_t nCount;				//0x00
	uint8_t pad0x01;			//0x01
	D2MessageStrc pMessages[8];	//0x02
};

//D2Common.0x6FDC36E0 (#10901)
D2TextHeaderStrc* __stdcall TEXT_AllocTextHeader(void* pMemPool);
//D2Common.0x6FDC3710 (#10902)
void __stdcall TEXT_FreeTextHeader(D2TextHeaderStrc* pTextHeader);
//D2Common.0x6FDC3760 (#10903)
void __stdcall TEXT_AddNodeToTextList(D2TextHeaderStrc* pTextHeader, short nStringId, int nMenu);
//D2Common.0x6FDC37A0 (#10904)
void __stdcall TEXT_RemoveNodeFromTextList(D2TextHeaderStrc* pTextHeader, short nStringId);
//D2Common.0x6FDC37F0 (#10905)
int __stdcall TEXT_GetMenuType2NodeCount(D2TextHeaderStrc* pTextHeader);
//D2Common.0x6FDC33820 (#10906)
short __stdcall TEXT_GetStringIdOfMenuType2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
//D2Common.0x6FDC3850 (#10907)
short __stdcall TEXT_GetStringIdOfMenuType1Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
//D2Common.0x6FDC3890 (#10908)
short __stdcall TEXT_GetStringIdOfMenuType0Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
//D2Common.0x6FDC38C0 (#10909)
void __stdcall TEXT_CreateMessageListFromTextHeader(D2TextHeaderStrc* pTextHeader, D2MessageListStrc* pMsgList);
//D2Common.0x6FDC3970 (#10910)
void __stdcall TEXT_CreateTextHeaderFromMessageList(D2TextHeaderStrc* pTextHeader, D2MessageListStrc* pMsgList);
//D2Common.0x6FDC3A70 (#10911)
void __stdcall TEXT_SortTextNodeListByStringId(D2TextHeaderStrc* pTextHeader);
