#include "McpConnect.h"


#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <Windows.h>

#include <D2MCPClient.h>
#include <Fog.h>
#include <Safesock.h>
#include <Storm/String.hpp>


//D2MCPClient.0x6FA02D08
static D2MCPConnectInfoStrc gtMCPConnectInfo;
//D2MCPClient.0x6FA02E54
static HANDLE ghMCPConnectionThread;
//D2MCPClient.0x6FA03014
static HANDLE ghMCPStartupEvent;
//D2MCPClient.0x6FA02D00
static uint32_t gnMCPLastError;

FClientConnection* gpMCPConnection;
uint16_t gnMCPPendingRequestId;
uint32_t gdwMCPGameToken;

//D2MCPClient.0x6FA02CB0
static char gszMCPGameServerIp[18];
//D2MCPClient.0x6FA02CC2
static uint16_t gnMCPGameRequestId;
//D2MCPClient.0x6FA02CD4
static uint32_t gdwMCPGameHash;
//D2MCPClient.0x6FA02CC4
static uint32_t gnMCPCreateQueuePosition;
//D2MCPClient.0x6FA02E60
static D2MCPGameInfoStrc gtMCPGameInfo;
//D2MCPClient.0x6FA0301C
static D2MCPCharListStrc* gpMCPCharList;

//D2MCPClient.0x6FA02CDC
static char gszMCPMsg0EString1[36];
//D2MCPClient.0x6FA02FEC
static char gszMCPMsg0EString2[28];
//D2MCPClient.0x6FA02C88
static BOOL gbMCPMsg0EReceived;

//D2MCPClient.0x6FA02CC8
static BOOL gbMCPCharCreateReceived;
//D2MCPClient.0x6FA0300C
static BOOL gbMCPCharLogonReceived;
//D2MCPClient.0x6FA03008
static BOOL gbMCPCharDeleteReceived;
//D2MCPClient.0x6FA02C8C
static BOOL gbMCPMsg0BReceived;
//D2MCPClient.0x6FA02C84
static BOOL gbMCPMsg0CReceived;
//D2MCPClient.0x6FA02CD8
static BOOL gbMCPMsg0DReceived;
//D2MCPClient.0x6FA02E5C
static BOOL gbMCPMsg0FReceived;
//D2MCPClient.0x6FA02E50
static BOOL gbMCPMsg10Received;
//D2MCPClient.0x6FA02C80
static BOOL gbMCPCreateQueueReceived;
//D2MCPClient.0x6FA03018
static BOOL gbMCPCharUpgradeReceived;

//D2MCPClient.0x6FA02C58
static MCPRequestCallback gpMCPMsg08Callback;
//D2MCPClient.0x6FA02C5C
static MCPRequestCallback gpMCPMsg09Callback;
//D2MCPClient.0x6FA02C60
static MCPLadderDataCallback gpMCPLadderDataCallback;
//D2MCPClient.0x6FA02C64
static MCPLadderDataBeginCallback gpMCPLadderDataBeginCallback;
//D2MCPClient.0x6FA02C68
static MCPMotdCallback gpMCPMotdCallback;

//D2MCPClient.0x6F9FAC58
// The original buffer is 0x8000 bytes, but the server may announce up to 0xFFFF bytes of data.
static uint8_t gMCPLadderDataBuffer[UINT16_MAX + 1];
//D2MCPClient.0x6FA02C70
static int32_t gnMCPLadderDataTotalSize;
//D2MCPClient.0x6FA02C6C
static int32_t gnMCPLadderDataReceivedSize;


#pragma pack(push, 1)

struct D2MCPResultMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint32_t nResult;		// 0x01
};

struct D2MCPRequestResultMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint16_t nRequestId;	// 0x01
	uint32_t nResult;		// 0x03
};

struct D2MCPCreateGameResponseMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint16_t nRequestId;	// 0x01
	uint32_t dwGameToken;	// 0x03
	uint32_t nResult;		// 0x07
};

struct D2MCPJoinGameResponseMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint16_t nRequestId;	// 0x01
	uint32_t dwGameToken;	// 0x03
	uint32_t dwIpAddress;	// 0x07
	uint32_t dwGameHash;	// 0x0B
	uint32_t nResult;		// 0x0F
};

struct D2MCPGameListResponseMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint16_t nRequestId;	// 0x01
	uint32_t dwIndex;		// 0x03
	uint8_t nPlayers;		// 0x07
	int32_t nStatus;		// 0x08
	char szStrings[1];		// 0x0C Game name, then description
};

struct D2MCPGameInfoResponseMsgStrc
{
	uint8_t nMessageId;			// 0x00
	uint16_t nRequestId;		// 0x01
	int32_t nStatus;			// 0x03
	uint32_t dwUnk0x07;			// 0x07
	uint8_t nUnk0x0B;			// 0x0B
	uint8_t nUnk0x0C;			// 0x0C
	uint8_t nUnk0x0D;			// 0x0D
	uint8_t nPlayers;			// 0x0E
	uint8_t nPlayerClass[16];	// 0x0F
	uint8_t nPlayerLevel[16];	// 0x1F
	char szStrings[1];			// 0x2F Game description, then the players names
};

struct D2MCPRequestDataMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint16_t nRequestId;	// 0x01
	uint8_t pData[1];		// 0x03
};

struct D2MCPMsg10ResponseMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint32_t dwGameToken;	// 0x01
	uint32_t dwGameHash;	// 0x05
	uint32_t dwIpAddress;	// 0x09
	uint32_t nResult;		// 0x0D
};

struct D2MCPLadderDataMsgStrc
{
	uint8_t nMessageId;			// 0x00
	uint8_t nLadderType;		// 0x01
	uint16_t nTotalSize;		// 0x02
	uint16_t nCurrentSize;		// 0x04
	uint16_t nReceivedSize;		// 0x06
	uint8_t pData[1];			// 0x08
};

struct D2MCPLadderDataHeaderStrc
{
	int32_t nUnk0;			// 0x00
	int32_t nEntries;		// 0x04
	int32_t nEntrySize;		// 0x08
};

struct D2MCPMotdMsgStrc
{
	uint8_t nMessageId;		// 0x00
	uint8_t nUnk;			// 0x01
	uint8_t pData[1];		// 0x02
};

#pragma pack(pop)


using MCPMessageHandler = BOOL(__fastcall*)(const uint8_t* pMessage, uint32_t nMessageSize);
using MCPConnectionMessageHandler = BOOL(__fastcall*)(const uint8_t* pMessage, uint32_t nMessageSize);

static DWORD __stdcall MCP_ConnectionThread(void* pParameter);


//D2MCPClient.0x6F9F17D0 (#10000)
HANDLE __stdcall D2MCPClientConnect(const D2MCPConnectInfoStrc* pConnectInfo)
{
	if (!ghMCPConnectionThread)
	{
		DWORD nThreadId = 0;
		gtMCPConnectInfo = *pConnectInfo;
		ghMCPConnectionThread = CreateThread(nullptr, 0, MCP_ConnectionThread, nullptr, 0, &nThreadId);
	}

	return ghMCPConnectionThread;
}

//D2MCPClient.0x6F9F1810
static DWORD __stdcall MCP_ConnectionThread(void* pParameter)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	FClientConnection* pConnection = FOG_10068_ClientCreate(FOG_inet_ntoa(gtMCPConnectInfo.dwMCPIp), gtMCPConnectInfo.dwMCPPort, "clt->mcp");
	gpMCPConnection = pConnection;
	FOG_10077_ClientSetMessageCallback(pConnection, MCP_DispatchConnectionMessage, nullptr);
	FOG_10073_ClientStart(pConnection);

	DWORD nFogError = 0;
	int nWSALastError = 0;
	FSafeSock* pSafeSock = FOG_10079_ClientGetSafeSock(pConnection);
	if (!pSafeSock)
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_CONNECTION_FAILED);
		return FALSE;
	}

	FOG_SOCKET_GetError(pSafeSock, &nFogError, &nWSALastError);
	if (nFogError)
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_CONNECTION_FAILED);
		return FALSE;
	}

	// The connection was closed while we were connecting
	if (pConnection != gpMCPConnection)
	{
		FOG_10075_ClientDestroy(pConnection);
		return FALSE;
	}

	// Select the MCP protocol
	const uint8_t nProtocol = 1;
	FOG_10071_ClientSendRaw(pConnection, &nProtocol, sizeof(nProtocol));

	if (!ghMCPStartupEvent)
	{
		ghMCPStartupEvent = CreateEventA(nullptr, FALSE, FALSE, nullptr);
	}

	if (!TOMCP_Startup(&gtMCPConnectInfo))
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_STARTUP_SEND_FAILED);
		return TRUE;
	}

	// Wait for the server to answer to MCP_STARTUP, at most 60 seconds
	const DWORD nTimeout = GetTickCount() + 60000;
	while (true)
	{
		pSafeSock = FOG_10079_ClientGetSafeSock(gpMCPConnection);
		if (!pSafeSock)
		{
			break;
		}

		sockaddr tPeerAddress = {};
		int nPeerAddressLength = sizeof(tPeerAddress);
		if (!FOG_SOCKET_GetPeerName(pSafeSock, &tPeerAddress, &nPeerAddressLength) || GetTickCount() >= nTimeout)
		{
			break;
		}

		const DWORD nWaitResult = WaitForSingleObject(ghMCPStartupEvent, 0);
		if (nWaitResult == WAIT_OBJECT_0)
		{
			return TRUE;
		}

		if (nWaitResult != WAIT_TIMEOUT)
		{
			break;
		}

		Sleep(10);
	}

	D2MCPClientSetLastError(MCPCLIENT_ERROR_CONNECTION_FAILED);
	return TRUE;
}

//D2MCPClient.0x6F9F1980 (#10001)
void __cdecl D2MCPClientCloseMCP()
{
	if (gpMCPConnection)
	{
		FOG_10069_ClientClose(gpMCPConnection);
		gpMCPConnection = nullptr;
	}

	if (ghMCPConnectionThread)
	{
		WaitForSingleObject(ghMCPConnectionThread, INFINITE);
		CloseHandle(ghMCPConnectionThread);
		ghMCPConnectionThread = nullptr;
	}

	if (ghMCPStartupEvent)
	{
		CloseHandle(ghMCPStartupEvent);
		ghMCPStartupEvent = nullptr;
	}
}

//D2MCPClient.0x6F9F19E0 (#10002)
BOOL __cdecl D2MCPClientIsConnected()
{
	FSafeSock* pSafeSock = FOG_10079_ClientGetSafeSock(gpMCPConnection);
	if (!pSafeSock)
	{
		return FALSE;
	}

	sockaddr tPeerAddress = {};
	int nPeerAddressLength = sizeof(tPeerAddress);
	return FOG_SOCKET_GetPeerName(pSafeSock, &tPeerAddress, &nPeerAddressLength);
}

//D2MCPClient.0x6F9F1A20 (#10003)
BOOL __cdecl D2MCPClientProcessMessages()
{
	if (!gpMCPConnection)
	{
		return FALSE;
	}

	uint8_t pBuffer[MCP_MESSAGE_BUFFER_SIZE];
	const uint16_t nMessageSize = FOG_10072_ClientReceive(gpMCPConnection, pBuffer, sizeof(pBuffer));
	if (nMessageSize)
	{
		MCP_DispatchMessage(pBuffer, nMessageSize);
	}

	return TRUE;
}

//D2MCPClient.0x6F9F17C0 (#10055)
HANDLE __cdecl D2MCPClientGetConnectionThread()
{
	return ghMCPConnectionThread;
}


//D2MCPClient.0x6F9F1780 (#10011)
void __fastcall D2MCPClientSetMsg08Callback(MCPRequestCallback pCallback)
{
	gpMCPMsg08Callback = pCallback;
}

//D2MCPClient.0x6F9F1790 (#10010)
void __fastcall D2MCPClientSetMsg09Callback(MCPRequestCallback pCallback)
{
	gpMCPMsg09Callback = pCallback;
}

//D2MCPClient.0x6F9F17A0 (#10012)
void __fastcall D2MCPClientSetLadderDataCallbacks(MCPLadderDataCallback pCallback, MCPLadderDataBeginCallback pBeginCallback)
{
	gpMCPLadderDataCallback = pCallback;
	gpMCPLadderDataBeginCallback = pBeginCallback;
}

//D2MCPClient.0x6F9F17B0 (#10048)
void __fastcall D2MCPClientSetMotdCallback(MCPMotdCallback pCallback)
{
	gpMCPMotdCallback = pCallback;
}


//D2MCPClient.0x6F9F1AB0 (#10046)
uint32_t __cdecl D2MCPClientGetLastError()
{
	return gnMCPLastError;
}

//D2MCPClient.0x6F9F1AA0 (#10047)
void __stdcall D2MCPClientSetLastError(uint32_t nError)
{
	gnMCPLastError = nError;
}

//D2MCPClient.0x6F9F1A70 (#10015)
const char* __cdecl D2MCPClientGetGameServerIp()
{
	return gszMCPGameServerIp;
}

//D2MCPClient.0x6F9F1A80 (#10016)
uint16_t __cdecl D2MCPClientGetGameRequestId()
{
	return gnMCPGameRequestId;
}

//D2MCPClient.0x6F9F1A90 (#10017)
uint32_t __cdecl D2MCPClientGetGameToken()
{
	return gdwMCPGameToken;
}

//D2MCPClient.0x6F9F1C90 (#10018)
uint32_t __cdecl D2MCPClientGetGameHash()
{
	return gdwMCPGameHash;
}

//D2MCPClient.0x6F9F1C00 (#10019)
D2MCPGameInfoStrc* __cdecl D2MCPClientGetGameInfo()
{
	return &gtMCPGameInfo;
}

//D2MCPClient.0x6F9F1C10 (#10020)
void __cdecl D2MCPClientClearGameInfo()
{
	std::memset(&gtMCPGameInfo, 0x00, sizeof(gtMCPGameInfo));
}

//D2MCPClient.0x6F9F1D10 (#10057)
D2MCPCharListStrc* __cdecl D2MCPClientGetCharacterList()
{
	return gpMCPCharList;
}

//D2MCPClient.0x6F9F1D20 (#10058)
void __stdcall D2MCPClientSetCharacterList(const uint8_t* pMessage, uint32_t nMessageSize)
{
	if (gpMCPCharList)
	{
		D2_FREE(gpMCPCharList);
		gpMCPCharList = nullptr;
	}

	if (!nMessageSize)
	{
		return;
	}

	gpMCPCharList = (D2MCPCharListStrc*)D2_ALLOC(nMessageSize);
	std::memcpy(gpMCPCharList, pMessage, nMessageSize);

	D2MCPCharListStrc* pMcp = gpMCPCharList;
	if (pMessage[0] == MCP_CHARLIST)
	{
		D2_ASSERT(pMcp->count < 256);
		D2_ASSERT(pMcp->firstChar < 256);
		D2_ASSERT(pMcp->totalChars < 256);
		D2_ASSERT(pMcp->maxCharsAllowed < 256);
	}
	else if (pMessage[0] == MCP_CHARLIST2)
	{
		D2_ASSERT(pMcp->count < 256);
		D2_ASSERT(pMcp->firstChar < 256);
		D2_ASSERT(pMcp->totalChars < 256);
		D2_ASSERT(pMcp->maxCharsAllowed < 256);

		// Entries are a DWORD followed by the character name and its stats string.
		// The original game walks them without using the result.
		const char* pEntry = (const char*)pMcp + sizeof(D2MCPCharListStrc);
		for (int i = 0; i < pMcp->maxCharsAllowed; ++i)
		{
			pEntry += sizeof(uint32_t);
			pEntry += strlen(pEntry) + 1;
			pEntry += strlen(pEntry) + 1;
		}
	}
	else
	{
		D2_ASSERT(!"Bad verb in McpClientSetCharacterList");
	}
}

//D2MCPClient.0x6F9F1B80 (#10038)
BOOL __stdcall D2MCPClientGetMsg0EStrings(char* szString1, char* szString2)
{
	strcpy(szString1, gszMCPMsg0EString1);
	strcpy(szString2, gszMCPMsg0EString2);
	return gbMCPMsg0EReceived;
}

//D2MCPClient.0x6F9F1BC0 (#10039)
void __stdcall D2MCPClientSetMsg0EStrings(BOOL bReceived, const char* szString1, const char* szString2)
{
	SStrCopy(gszMCPMsg0EString1, szString1, sizeof(gszMCPMsg0EString1));
	SStrCopy(gszMCPMsg0EString2, szString2, sizeof(gszMCPMsg0EString2));
	gbMCPMsg0EReceived = bReceived;
}

//D2MCPClient.0x6F9F1CD0 (#10050)
uint32_t __cdecl D2MCPClientGetCreateQueuePosition()
{
	return gnMCPCreateQueuePosition;
}

//D2MCPClient.0x6F9F1CE0 (#10051)
void __stdcall D2MCPClientSetCreateQueuePosition(uint32_t nPosition)
{
	gnMCPCreateQueuePosition = nPosition;
}

//D2MCPClient.0x6F9F1AC0 (#10029)
BOOL __cdecl D2MCPClientGetCharCreateReceived()
{
	return gbMCPCharCreateReceived;
}

//D2MCPClient.0x6F9F1AD0 (#10028)
void __stdcall D2MCPClientSetCharCreateReceived(BOOL bReceived)
{
	gbMCPCharCreateReceived = bReceived;
}

//D2MCPClient.0x6F9F1AE0 (#10031)
BOOL __cdecl D2MCPClientGetCharLogonReceived()
{
	return gbMCPCharLogonReceived;
}

//D2MCPClient.0x6F9F1AF0 (#10030)
void __stdcall D2MCPClientSetCharLogonReceived(BOOL bReceived)
{
	gbMCPCharLogonReceived = bReceived;
}

//D2MCPClient.0x6F9F1B00 (#10033)
BOOL __cdecl D2MCPClientGetCharDeleteReceived()
{
	return gbMCPCharDeleteReceived;
}

//D2MCPClient.0x6F9F1B10 (#10032)
void __stdcall D2MCPClientSetCharDeleteReceived(BOOL bReceived)
{
	gbMCPCharDeleteReceived = bReceived;
}

//D2MCPClient.0x6F9F1B50 (#10035)
BOOL __cdecl D2MCPClientGetMsg0BReceived()
{
	return gbMCPMsg0BReceived;
}

//D2MCPClient.0x6F9F1B40 (#10034)
void __stdcall D2MCPClientSetMsg0BReceived(BOOL bReceived)
{
	gbMCPMsg0BReceived = bReceived;
}

//D2MCPClient.0x6F9F1B60 (#10037)
BOOL __cdecl D2MCPClientGetMsg0CReceived()
{
	return gbMCPMsg0CReceived;
}

//D2MCPClient.0x6F9F1B70 (#10036)
void __stdcall D2MCPClientSetMsg0CReceived(BOOL bReceived)
{
	gbMCPMsg0CReceived = bReceived;
}

//D2MCPClient.0x6F9F1C30 (#10040)
BOOL __cdecl D2MCPClientGetMsg0DReceived()
{
	return gbMCPMsg0DReceived;
}

//D2MCPClient.0x6F9F1C40 (#10041)
void __stdcall D2MCPClientSetMsg0DReceived(BOOL bReceived)
{
	gbMCPMsg0DReceived = bReceived;
}

//D2MCPClient.0x6F9F1C50 (#10042)
BOOL __cdecl D2MCPClientGetMsg0FReceived()
{
	return gbMCPMsg0FReceived;
}

//D2MCPClient.0x6F9F1C60 (#10043)
void __stdcall D2MCPClientSetMsg0FReceived(BOOL bReceived)
{
	gbMCPMsg0FReceived = bReceived;
}

//D2MCPClient.0x6F9F1C70 (#10044)
BOOL __cdecl D2MCPClientGetMsg10Received()
{
	return gbMCPMsg10Received;
}

//D2MCPClient.0x6F9F1C80 (#10045)
void __stdcall D2MCPClientSetMsg10Received(BOOL bReceived)
{
	gbMCPMsg10Received = bReceived;
}

//D2MCPClient.0x6F9F1CF0 (#10052)
BOOL __cdecl D2MCPClientGetCreateQueueReceived()
{
	return gbMCPCreateQueueReceived;
}

//D2MCPClient.0x6F9F1D00 (#10053)
void __stdcall D2MCPClientSetCreateQueueReceived(BOOL bReceived)
{
	gbMCPCreateQueueReceived = bReceived;
}

//D2MCPClient.0x6F9F1B20 (#10061)
BOOL __cdecl D2MCPClientGetCharUpgradeReceived()
{
	return gbMCPCharUpgradeReceived;
}

//D2MCPClient.0x6F9F1B30 (#10060)
void __stdcall D2MCPClientSetCharUpgradeReceived(BOOL bReceived)
{
	gbMCPCharUpgradeReceived = bReceived;
}


// Message handlers

//D2MCPClient.0x6F9F1010
static BOOL __fastcall FROMMCP_Ignore(const uint8_t* pMessage, uint32_t nMessageSize)
{
	return TRUE;
}

//D2MCPClient.0x6F9F1020
static BOOL __fastcall FROMMCP_Startup(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	SetEvent(ghMCPStartupEvent);
	return TRUE;
}

//D2MCPClient.0x6F9F1040
static BOOL __fastcall FROMMCP_CharCreate(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetCharCreateReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F1080
static BOOL __fastcall FROMMCP_CreateGame(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const D2MCPCreateGameResponseMsgStrc* pMsg = (const D2MCPCreateGameResponseMsgStrc*)pMessage;
	if (pMsg->nRequestId == gnMCPPendingRequestId)
	{
		D2MCPClientSetLastError(pMsg->nResult);
		gdwMCPGameToken = pMsg->dwGameToken;
		gnMCPGameRequestId = gnMCPPendingRequestId;
	}

	return TRUE;
}

//D2MCPClient.0x6F9F10C0
static BOOL __fastcall FROMMCP_JoinGame(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const D2MCPJoinGameResponseMsgStrc* pMsg = (const D2MCPJoinGameResponseMsgStrc*)pMessage;
	if (pMsg->nRequestId != gnMCPPendingRequestId)
	{
		return TRUE;
	}

	gnMCPGameRequestId = gnMCPPendingRequestId;
	gdwMCPGameToken = pMsg->dwGameToken;
	gdwMCPGameHash = pMsg->dwGameHash;

	if (pMsg->dwIpAddress)
	{
		SStrCopy(gszMCPGameServerIp, FOG_inet_ntoa(pMsg->dwIpAddress), sizeof(gszMCPGameServerIp));
	}
	else if (pMsg->nResult)
	{
		D2MCPClientSetLastError(pMsg->nResult);
	}
	else
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_GAME_NOT_FOUND);
	}

	return TRUE;
}

//D2MCPClient.0x6F9F1140
static BOOL __fastcall FROMMCP_GameList(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const D2MCPGameListResponseMsgStrc* pMsg = (const D2MCPGameListResponseMsgStrc*)pMessage;
	if (pMsg->nRequestId != gnMCPPendingRequestId)
	{
		return TRUE;
	}

	gtMCPGameInfo.dwIndex = pMsg->dwIndex;
	gtMCPGameInfo.nRequestId = gnMCPPendingRequestId;
	gtMCPGameInfo.nPlayers = pMsg->nPlayers;
	gtMCPGameInfo.nStatus = pMsg->nStatus;

	if (pMsg->nStatus == -2)
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_GAME_LIST_UNAVAILABLE);
		return TRUE;
	}

	if (pMsg->nStatus == -1)
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_GAME_LIST_FAILED);
		return TRUE;
	}

	const char* szGameName = pMsg->szStrings;
	const char* szGameDescription = szGameName + strlen(szGameName) + 1;
	const char* pEnd = szGameDescription + strlen(szGameDescription) + 1;
	SStrCopy(gtMCPGameInfo.szGameName, szGameName, sizeof(gtMCPGameInfo.szGameName));
	SStrCopy(gtMCPGameInfo.szGameDescription, szGameDescription, sizeof(gtMCPGameInfo.szGameDescription));
	gtMCPGameInfo.bMoreData = (uint32_t)(pEnd - (const char*)pMessage) < nMessageSize;
	return TRUE;
}

//D2MCPClient.0x6F9F1210
static BOOL __fastcall FROMMCP_GameInfo(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const D2MCPGameInfoResponseMsgStrc* pMsg = (const D2MCPGameInfoResponseMsgStrc*)pMessage;
	if (pMsg->nRequestId != gnMCPPendingRequestId)
	{
		return TRUE;
	}

	gtMCPGameInfo.nRequestId = gnMCPPendingRequestId;
	gtMCPGameInfo.nStatus = pMsg->nStatus;
	gtMCPGameInfo.nPlayers = pMsg->nPlayers;
	gtMCPGameInfo.dwUnk0x0C = pMsg->dwUnk0x07;
	gtMCPGameInfo.nUnk0x10 = pMsg->nUnk0x0B;
	gtMCPGameInfo.nUnk0x11 = pMsg->nUnk0x0C;
	gtMCPGameInfo.nUnk0x12 = pMsg->nUnk0x0D;

	if (pMsg->nStatus == -1)
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_GAME_LIST_FAILED);
		return TRUE;
	}

	if (pMsg->nStatus == -2)
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_GAME_LIST_UNAVAILABLE);
		return TRUE;
	}

	const char* szString = pMsg->szStrings;
	SStrCopy(gtMCPGameInfo.szGameDescription, szString, sizeof(gtMCPGameInfo.szGameDescription));
	szString += strlen(szString) + 1;

	// The original game does not clamp the number of players to the size of the arrays
	const int nPlayers = std::min<int>(pMsg->nPlayers, sizeof(gtMCPGameInfo.szPlayerName) / sizeof(gtMCPGameInfo.szPlayerName[0]));
	for (int i = 0; i < nPlayers; ++i)
	{
		gtMCPGameInfo.nPlayerClass[i] = pMsg->nPlayerClass[i];
		gtMCPGameInfo.nPlayerLevel[i] = pMsg->nPlayerLevel[i];
		SStrCopy(gtMCPGameInfo.szPlayerName[i], szString, sizeof(gtMCPGameInfo.szPlayerName[i]));
		szString += strlen(szString) + 1;
	}

	return TRUE;
}

//D2MCPClient.0x6F9F1060
static BOOL __fastcall FROMMCP_CharLogon(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetCharLogonReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F1360
static BOOL __fastcall FROMMCP_Msg08(const uint8_t* pMessage, uint32_t nMessageSize)
{
	if (gpMCPMsg08Callback)
	{
		const D2MCPRequestDataMsgStrc* pMsg = (const D2MCPRequestDataMsgStrc*)pMessage;
		gpMCPMsg08Callback(pMsg->nRequestId, pMsg->pData);
	}

	return TRUE;
}

//D2MCPClient.0x6F9F1380
static BOOL __fastcall FROMMCP_Msg09(const uint8_t* pMessage, uint32_t nMessageSize)
{
	if (gpMCPMsg09Callback)
	{
		const D2MCPRequestDataMsgStrc* pMsg = (const D2MCPRequestDataMsgStrc*)pMessage;
		// Note: data starts one byte later than for MCP_MSG_08
		gpMCPMsg09Callback(pMsg->nRequestId, pMsg->pData + 1);
	}

	return TRUE;
}

//D2MCPClient.0x6F9F13A0
static BOOL __fastcall FROMMCP_CharDelete(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const D2MCPRequestResultMsgStrc* pMsg = (const D2MCPRequestResultMsgStrc*)pMessage;
	if (pMsg->nRequestId == gnMCPPendingRequestId)
	{
		D2MCPClientSetLastError(pMsg->nResult);
		D2MCPClientSetCharDeleteReceived(TRUE);
	}

	return TRUE;
}

//D2MCPClient.0x6F9F13F0
static BOOL __fastcall FROMMCP_Msg0B(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetMsg0BReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F1410
static BOOL __fastcall FROMMCP_Msg0C(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetMsg0CReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F1460
static BOOL __fastcall FROMMCP_Msg0D(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetMsg0DReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F1430
static BOOL __fastcall FROMMCP_Msg0E(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const char* szString1 = (const char*)pMessage + 1;
	const char* szString2 = szString1 + strlen(szString1) + 1;
	D2MCPClientSetMsg0EStrings(TRUE, szString1, szString2);
	return TRUE;
}

//D2MCPClient.0x6F9F1480
static BOOL __fastcall FROMMCP_Msg0F(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetMsg0FReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F14A0
static BOOL __fastcall FROMMCP_Msg10(const uint8_t* pMessage, uint32_t nMessageSize)
{
	const D2MCPMsg10ResponseMsgStrc* pMsg = (const D2MCPMsg10ResponseMsgStrc*)pMessage;
	D2MCPClientSetLastError(pMsg->nResult);
	gdwMCPGameToken = pMsg->dwGameToken;
	gdwMCPGameHash = pMsg->dwGameHash;

	if (pMsg->dwIpAddress)
	{
		SStrCopy(gszMCPGameServerIp, FOG_inet_ntoa(pMsg->dwIpAddress), sizeof(gszMCPGameServerIp));
	}
	else if (pMsg->nResult)
	{
		D2MCPClientSetLastError(pMsg->nResult);
	}
	else
	{
		D2MCPClientSetLastError(MCPCLIENT_ERROR_GAME_NOT_FOUND);
	}

	D2MCPClientSetMsg10Received(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F1520
static BOOL __fastcall FROMMCP_LadderData(const uint8_t* pMessage, uint32_t nMessageSize)
{
	if (nMessageSize < offsetof(D2MCPLadderDataMsgStrc, pData))
	{
		return FALSE;
	}

	const D2MCPLadderDataMsgStrc* pMsg = (const D2MCPLadderDataMsgStrc*)pMessage;
	if (!pMsg->nTotalSize && !pMsg->nCurrentSize && !pMsg->nReceivedSize)
	{
		if (gpMCPLadderDataCallback)
		{
			gpMCPLadderDataCallback(0, 0, 0, 0, nullptr);
		}

		gnMCPLadderDataTotalSize = 0;
		gnMCPLadderDataReceivedSize = 0;
		return TRUE;
	}

	if (pMsg->nTotalSize < sizeof(D2MCPLadderDataHeaderStrc))
	{
		return FALSE;
	}

	gnMCPLadderDataTotalSize = pMsg->nTotalSize;
	gnMCPLadderDataReceivedSize = pMsg->nReceivedSize;

	const int32_t nReceivedSize = pMsg->nCurrentSize + pMsg->nReceivedSize;
	if (nReceivedSize > gnMCPLadderDataTotalSize)
	{
		return FALSE;
	}

	std::memcpy(&gMCPLadderDataBuffer[pMsg->nReceivedSize], pMsg->pData, pMsg->nCurrentSize);
	gnMCPLadderDataReceivedSize = nReceivedSize;
	if (nReceivedSize < gnMCPLadderDataTotalSize)
	{
		// Wait for the other parts
		return TRUE;
	}

	if (gpMCPLadderDataBeginCallback)
	{
		gpMCPLadderDataBeginCallback();
	}

	const uint8_t* pData = gMCPLadderDataBuffer;
	do
	{
		const D2MCPLadderDataHeaderStrc* pHeader = (const D2MCPLadderDataHeaderStrc*)pData;
		pData += sizeof(D2MCPLadderDataHeaderStrc);
		gnMCPLadderDataTotalSize -= sizeof(D2MCPLadderDataHeaderStrc);

		if (pHeader->nEntries < 0 || pHeader->nEntrySize < 0
			|| pHeader->nEntries > 256 || pHeader->nEntrySize > 16
			|| pHeader->nUnk0 > 2000)
		{
			break;
		}

		const int32_t nEntriesSize = (pHeader->nEntrySize + 12) * pHeader->nEntries;
		if (nEntriesSize > gnMCPLadderDataTotalSize)
		{
			break;
		}

		if (gpMCPLadderDataCallback)
		{
			gpMCPLadderDataCallback(pHeader->nUnk0, pHeader->nEntries, pHeader->nEntrySize, pMsg->nLadderType, pData);
		}

		pData += nEntriesSize;
		gnMCPLadderDataTotalSize -= nEntriesSize;
	}
	while (gnMCPLadderDataTotalSize > 0);

	gnMCPLadderDataTotalSize = 0;
	return TRUE;
}

//D2MCPClient.0x6F9F1690
static BOOL __fastcall FROMMCP_Motd(const uint8_t* pMessage, uint32_t nMessageSize)
{
	if (nMessageSize < offsetof(D2MCPMotdMsgStrc, pData))
	{
		return FALSE;
	}

	if (gpMCPMotdCallback)
	{
		const D2MCPMotdMsgStrc* pMsg = (const D2MCPMotdMsgStrc*)pMessage;
		gpMCPMotdCallback(pMsg->nUnk, nMessageSize - offsetof(D2MCPMotdMsgStrc, pData), pMsg->pData);
	}

	return TRUE;
}

//D2MCPClient.0x6F9F16C0
static BOOL __fastcall FROMMCP_CreateQueue(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetCreateQueuePosition(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetCreateQueueReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F16E0
static BOOL __fastcall FROMMCP_CharList(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetCharacterList(pMessage, nMessageSize);
	return TRUE;
}

//D2MCPClient.0x6F9F13D0
static BOOL __fastcall FROMMCP_CharUpgrade(const uint8_t* pMessage, uint32_t nMessageSize)
{
	D2MCPClientSetLastError(((const D2MCPResultMsgStrc*)pMessage)->nResult);
	D2MCPClientSetCharUpgradeReceived(TRUE);
	return TRUE;
}

//D2MCPClient.0x6F9F8040
static const MCPMessageHandler gpMCPMessageHandlers[NUM_MCP_MESSAGES] = {
	FROMMCP_Ignore,			// 0x00
	nullptr,				// 0x01 MCP_STARTUP, handled by gpMCPConnectionMessageHandlers
	FROMMCP_CharCreate,		// 0x02 MCP_CHARCREATE
	FROMMCP_CreateGame,		// 0x03 MCP_CREATEGAME
	FROMMCP_JoinGame,		// 0x04 MCP_JOINGAME
	FROMMCP_GameList,		// 0x05 MCP_GAMELIST
	FROMMCP_GameInfo,		// 0x06 MCP_GAMEINFO
	FROMMCP_CharLogon,		// 0x07 MCP_CHARLOGON
	FROMMCP_Msg08,			// 0x08
	FROMMCP_Msg09,			// 0x09
	FROMMCP_CharDelete,		// 0x0A MCP_CHARDELETE
	FROMMCP_Msg0B,			// 0x0B
	FROMMCP_Msg0C,			// 0x0C
	FROMMCP_Msg0D,			// 0x0D
	FROMMCP_Msg0E,			// 0x0E
	FROMMCP_Msg0F,			// 0x0F
	FROMMCP_Msg10,			// 0x10
	FROMMCP_LadderData,		// 0x11 MCP_REQUESTLADDERDATA
	FROMMCP_Motd,			// 0x12 MCP_MOTD
	nullptr,				// 0x13 MCP_CANCELGAMECREATE
	FROMMCP_CreateQueue,	// 0x14 MCP_CREATEQUEUE
	nullptr,				// 0x15
	nullptr,				// 0x16 MCP_CHARRANK
	FROMMCP_CharList,		// 0x17 MCP_CHARLIST
	FROMMCP_CharUpgrade,	// 0x18 MCP_CHARUPGRADE
	FROMMCP_CharList,		// 0x19 MCP_CHARLIST2
};

//D2MCPClient.0x6F9F80A8
static const MCPConnectionMessageHandler gpMCPConnectionMessageHandlers[NUM_MCP_MESSAGES] = {
	nullptr,				// 0x00
	FROMMCP_Startup,		// 0x01 MCP_STARTUP
};

//D2MCPClient.0x6F9F16F0
BOOL __fastcall MCP_DispatchMessage(const uint8_t* pMessage, uint32_t nMessageSize)
{
	if (nMessageSize == 0 || pMessage[0] >= NUM_MCP_MESSAGES)
	{
		return FALSE;
	}

	if (const MCPMessageHandler pHandler = gpMCPMessageHandlers[pMessage[0]])
	{
		return pHandler(pMessage, nMessageSize);
	}

	return FALSE;
}

//D2MCPClient.0x6F9F1730
BOOL __fastcall MCP_DispatchConnectionMessage(const uint8_t* pMessage, uint32_t nMessageSize, void* pUserData)
{
	if (nMessageSize == 0 || pMessage[0] >= NUM_MCP_MESSAGES)
	{
		return FALSE;
	}

	if (const MCPConnectionMessageHandler pHandler = gpMCPConnectionMessageHandlers[pMessage[0]])
	{
		return pHandler(pMessage, nMessageSize);
	}

	return FALSE;
}
