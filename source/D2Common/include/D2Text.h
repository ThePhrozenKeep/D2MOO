#pragma once

#include "D2CommonDefinitions.h"
#include "D2PacketDef.h"

#pragma pack(1)

#define MAX_TEXT_LIST_NODES 8

struct D2TextNodeStrc
{
	int16_t nStringId;			//0x00
	uint16_t pad0x02;			//0x02
	int32_t nMenu;				//0x04
	D2TextNodeStrc* pNext;		//0x08
};

struct D2TextHeaderStrc
{
	void* pMemPool;				//0x00
	uint16_t nCount;			//0x04
	uint16_t pad0x06;			//0x06
	D2TextNodeStrc* pNode;		//0x08
};

struct D2TextMessageStrc
{
	wchar_t* pStringLines[6];	//0x00
	uint16_t nLines;			//0x18
	int32_t nColor;				//0x1A
	uint32_t nEndTick;			//0x1E
	D2TextMessageStrc* pNext;	//0x22
};
#pragma pack()

//D2Common.0x6FDC36E0 (#10901)
D2COMMON_DLL_DECL D2TextHeaderStrc* __stdcall TEXT_AllocTextHeader(void* pMemPool);
//D2Common.0x6FDC3710 (#10902)
D2COMMON_DLL_DECL void __stdcall TEXT_FreeTextHeader(D2TextHeaderStrc* pTextHeader);
//D2Common.0x6FDC3760 (#10903)
D2COMMON_DLL_DECL void __stdcall TEXT_AddNodeToTextList(D2TextHeaderStrc* pTextHeader, short nStringId, int nMenu);
//D2Common.0x6FDC37A0 (#10904)
D2COMMON_DLL_DECL void __stdcall TEXT_RemoveNodeFromTextList(D2TextHeaderStrc* pTextHeader, short nStringId);
//D2Common.0x6FDC37F0 (#10905)
D2COMMON_DLL_DECL int __stdcall TEXT_GetMenuType2NodeCount(D2TextHeaderStrc* pTextHeader);
//D2Common.0x6FDC33820 (#10906)
D2COMMON_DLL_DECL short __stdcall TEXT_GetStringIdOfMenuType2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
//D2Common.0x6FDC3850 (#10907)
D2COMMON_DLL_DECL short __stdcall TEXT_GetStringIdOfMenuType1Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
//D2Common.0x6FDC3890 (#10908)
D2COMMON_DLL_DECL short __stdcall TEXT_GetStringIdOfMenuType0Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
//D2Common.0x6FDC38C0 (#10909)
D2COMMON_DLL_DECL void __stdcall TEXT_CreateMessageListFromTextHeader(D2TextHeaderStrc* pTextHeader, D2MessageListStrc* pMsgList);
//D2Common.0x6FDC3970 (#10910)
D2COMMON_DLL_DECL void __stdcall TEXT_CreateTextHeaderFromMessageList(D2TextHeaderStrc* pTextHeader, D2MessageListStrc* pMsgList);
//D2Common.0x6FDC3A70 (#10911)
D2COMMON_DLL_DECL void __stdcall TEXT_SortTextNodeListByStringId(D2TextHeaderStrc* pTextHeader);
