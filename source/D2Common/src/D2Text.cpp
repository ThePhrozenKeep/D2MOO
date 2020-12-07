#include "D2Text.h"


//D2Common.0x6FDC36E0 (#10901)
D2TextHeaderStrc* __stdcall TEXT_AllocTextHeader(void* pMemPool)
{
	D2TextHeaderStrc* pTextHeader = (D2TextHeaderStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2TextHeaderStrc), __FILE__, __LINE__, 0);
	pTextHeader->pMemPool = pMemPool;
	pTextHeader->nCount = 0;
	pTextHeader->pNode = NULL;
	
	return pTextHeader;
}

//D2Common.0x6FDC3710 (#10902)
void __stdcall TEXT_FreeTextHeader(D2TextHeaderStrc* pTextHeader)
{
	D2TextNodeStrc* pNode = NULL;
	D2TextNodeStrc* pNext = NULL;

	pNode = pTextHeader->pNode;
	while (pNode)
	{
		pNext = pNode->pNext;
		FOG_FreeServerMemory(pTextHeader->pMemPool, pNode, __FILE__, __LINE__, 0);
		pNode = pNext;
	}
	pTextHeader->nCount = 0;
	FOG_FreeServerMemory(pTextHeader->pMemPool, pTextHeader, __FILE__, __LINE__, 0);
}

//D2Common.0x6FDC3760 (#10903)
void __stdcall TEXT_AddNodeToTextList(D2TextHeaderStrc* pTextHeader, short nStringId, int nMenu)
{
	D2TextNodeStrc* pNode = NULL;

	pNode = (D2TextNodeStrc*)FOG_AllocServerMemory(pTextHeader->pMemPool, sizeof(D2TextNodeStrc), __FILE__, __LINE__, 0);
	pNode->pNext = pTextHeader->pNode;
	pNode->nStringId = nStringId;
	pNode->nMenu = nMenu;

	++pTextHeader->nCount;
	pTextHeader->pNode = pNode;
}

//D2Common.0x6FDC37A0 (#10904)
void __stdcall TEXT_RemoveNodeFromTextList(D2TextHeaderStrc* pTextHeader, short nStringId)
{
	D2TextNodeStrc* pPrevious = NULL;
	D2TextNodeStrc* pNode = NULL;

	pNode = pTextHeader->pNode;
	if (pNode)
	{
		while (pNode->nStringId != nStringId)
		{
			pPrevious = pNode;

			pNode = pNode->pNext;

			if (!pNode)
			{
				return;
			}
		}

		if (pPrevious)
		{
			pPrevious->pNext = pNode->pNext;
		}
		else
		{
			pTextHeader->pNode = pNode->pNext;
		}

		FOG_FreeServerMemory(pTextHeader->pMemPool, pNode, __FILE__, __LINE__, 0);
		--pTextHeader->nCount;
	}
}

//D2Common.0x6FDC37F0 (#10905)
int __stdcall TEXT_GetMenuType2NodeCount(D2TextHeaderStrc* pTextHeader)
{
	int nNodes = 0;

	for (D2TextNodeStrc* pNode = pTextHeader->pNode; pNode; pNode = pNode->pNext)
	{
		if (pNode->nMenu == 2)
		{
			++nNodes;
		}
	}

	return nNodes;
}

//D2Common.0x6FDC33820 (#10906)
short __stdcall TEXT_GetStringIdOfMenuType2Node(D2TextHeaderStrc* pTextHeader, int nNodeId)
{
	D2TextNodeStrc* pNode = NULL;
	int nCounter = 0;

	pNode = pTextHeader->pNode;

	while (pNode)
	{
		if (pNode->nMenu == 2)
		{
			if (nCounter == nNodeId)
			{
				return pNode->nStringId;
			}

			++nCounter;
		}

		pNode = pNode->pNext;
	}

	return 3724;
}

//D2Common.0x6FDC3850 (#10907)
short __stdcall TEXT_GetStringIdOfMenuType1Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId)
{
	D2TextNodeStrc* pNode = NULL;
	int nCounter = 0;

	pNode = pTextHeader->pNode;

	while (pNode)
	{
		if (pNode->nMenu != 0)
		{
			if (nCounter == nNodeId)
			{
				return pNode->nStringId;
			}

			++nCounter;
		}

		pNode = pNode->pNext;
	}

	return -1;
}

//D2Common.0x6FDC3890 (#10908)
short __stdcall TEXT_GetStringIdOfMenuType0Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId)
{
	D2TextNodeStrc* pNode = NULL;
	int nCounter = 0;

	pNode = pTextHeader->pNode;

	while (pNode)
	{
		if (pNode->nMenu != 1)
		{
			if (nCounter == nNodeId)
			{
				return pNode->nStringId;
			}

			++nCounter;
		}

		pNode = pNode->pNext;
	}

	return -1;
}

//D2Common.0x6FDC38C0 (#10909)
void __stdcall TEXT_CreateMessageListFromTextHeader(D2TextHeaderStrc* pTextHeader, D2MessageListStrc* pMsgList)
{
	D2TextNodeStrc* pNode = NULL;
	int nCounter = 0;

	D2_ASSERT(pTextHeader);
	D2_ASSERT(pMsgList);

	memset(pMsgList, 0x00, sizeof(D2MessageListStrc));

	pNode = pTextHeader->pNode;
	pMsgList->nCount = LOBYTE(pTextHeader->nCount);

	if (pNode)
	{
		do
		{
			D2_ASSERT(nCounter < MAX_TEXT_LIST_NODES);

			pMsgList->pMessages[nCounter].nMenu = LOBYTE(pNode->nMenu);
			pMsgList->pMessages[nCounter].nStringId = pNode->nStringId;
			++nCounter;

			pNode = pNode->pNext;
		}
		while (pNode);
	}
}

//D2Common.0x6FDC3970 (#10910)
void __stdcall TEXT_CreateTextHeaderFromMessageList(D2TextHeaderStrc* pTextHeader, D2MessageListStrc* pMsgList)
{
	D2TextNodeStrc* pNext = NULL;
	D2TextNodeStrc* pNode = NULL;

	D2_ASSERT(pTextHeader);
	D2_ASSERT(pMsgList);

	pNode = pTextHeader->pNode;
	if (pNode)
	{
		do
		{
			pNext = pNode->pNext;
			FOG_FreeServerMemory(pTextHeader->pMemPool, pNode, __FILE__, __LINE__, 0);
			pNode = pNext;
		}
		while (pNode);
	}

	D2_ASSERT(pMsgList->nCount < MAX_TEXT_LIST_NODES);

	pTextHeader->nCount = 0;
	while (pTextHeader->nCount < pMsgList->nCount)
	{
		pNode = (D2TextNodeStrc*)FOG_AllocServerMemory(pTextHeader->pMemPool, sizeof(D2TextNodeStrc), __FILE__, __LINE__, 0);
		pNode->nMenu = pMsgList->pMessages[pTextHeader->nCount].nMenu;
		pNode->nStringId = pMsgList->pMessages[pTextHeader->nCount].nStringId;
		pNode->pNext = pTextHeader->pNode;

		++pTextHeader->nCount;
		pTextHeader->pNode = pNode;
	}
}

//D2Common.0x6FDC3A70 (#10911)
void __stdcall TEXT_SortTextNodeListByStringId(D2TextHeaderStrc* pTextHeader)
{
	D2TextNodeStrc* pNodeList = NULL;
	D2TextNodeStrc* pNode = NULL;
	int nPreviousMenu = 0;
	int nMenu = 0;
	int nCounter = 0;
	int nId = 0;
	short nPreviousStringId = 0;
	short nStringId = 0;

	D2_ASSERT(pTextHeader);

	if (pTextHeader->nCount > 1)
	{
		pNodeList = (D2TextNodeStrc*)FOG_AllocServerMemory(pTextHeader->pMemPool, sizeof(D2TextNodeStrc) * pTextHeader->nCount, __FILE__, __LINE__, 0);

		pNode = pTextHeader->pNode;
		nCounter = 0;
		while (pNode)
		{
			pNodeList[nCounter].nStringId = pNode->nStringId;
			pNodeList[nCounter].nMenu = pNode->nMenu;

			pNode = pNode->pNext;
			++nCounter;
		}

		nPreviousStringId = pNodeList[0].nStringId;
		nPreviousMenu = pNodeList[0].nMenu;

		nCounter = 0;
		do
		{
			nStringId = pNodeList[nCounter + 1].nStringId;
			nMenu = pNodeList[nCounter + 1].nMenu;

			if (nPreviousStringId <= nStringId)
			{
				nPreviousMenu = nMenu;
				nPreviousStringId = nStringId;
			}
			else
			{
				pNodeList[nCounter + 1].nStringId = nPreviousStringId;
				pNodeList[nCounter + 1].nMenu = nPreviousMenu;

				nId = nCounter;
				while (nId)
				{
					if (pNodeList[nId - 1].nStringId <= nStringId)
					{
						break;
					}

					pNodeList[nId].nMenu = pNodeList[nId - 1].nMenu;
					pNodeList[nId].nStringId = pNodeList[nId - 1].nStringId;

					--nId;
				}

				pNodeList[nId].nMenu = nMenu;
				pNodeList[nId].nStringId = nStringId;
			}

			++nCounter;
		}
		while (nCounter + 1 < pTextHeader->nCount);

		pNode = pTextHeader->pNode;
		nCounter = 0;
		while (nCounter < pTextHeader->nCount)
		{
			pNode->nStringId = pNodeList[nCounter].nStringId;
			pNode->nMenu = pNodeList[nCounter].nMenu;

			pNode = pNode->pNext;

			++nCounter;
		}

		FOG_FreeServerMemory(pTextHeader->pMemPool, pNodeList, __FILE__, __LINE__, 0);
	}
}
