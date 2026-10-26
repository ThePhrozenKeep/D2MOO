#include "McpConnect.h"


#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <Windows.h>

#include <D2MCPClient.h>
#include <Fog.h>
#include <Safesock.h>
#include <Storm/String.hpp>


#pragma pack(push, 1)

struct D2MCPStartupMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint32_t dwMCPCookie;						// 0x01
	uint32_t dwMCPStatus;						// 0x05
	uint32_t dwMCPChunk1[2];					// 0x09
	uint32_t dwMCPChunk2[12];					// 0x11
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x41
};

struct D2MCPCharCreateMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint32_t nClass;							// 0x01
	uint16_t nFlags;							// 0x05
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x07
};

struct D2MCPCreateGameMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint16_t nRequestId;						// 0x01
	uint32_t dwDifficulty;						// 0x03
	uint8_t nUnk0x07;							// 0x07
	uint8_t nLevelDifference;					// 0x08
	uint8_t nMaxPlayers;						// 0x09
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x0A
};

struct D2MCPGameListMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint16_t nRequestId;						// 0x01
	uint32_t dwUnk0x03;							// 0x03
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x07
};

struct D2MCPRequestStringsMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint16_t nRequestId;						// 0x01
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x03
};

struct D2MCPStringsMsgStrc
{
	uint8_t nMessageId;							// 0x00
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x01
};

struct D2MCPLadderDataRequestMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint8_t nLadderType;						// 0x01
	uint16_t nStartingPosition;					// 0x02
};

struct D2MCPCharListRequestMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint32_t nCharacters;						// 0x01
};

struct D2MCPCharRankMsgStrc
{
	uint8_t nMessageId;							// 0x00
	uint32_t dwUnk1;							// 0x01
	uint32_t dwUnk2;							// 0x05
	char szStrings[MCP_MESSAGE_BUFFER_SIZE];	// 0x09
};

#pragma pack(pop)


// Appends a string to the strings part of a message, *lpdwSize being the number of bytes already used by previous strings.
static void TOMCP_PackString(char* pStrings, const char* szString, uint32_t* lpdwSize, uint32_t dwMaxSize)
{
	const uint32_t nLength = (uint32_t)strlen(szString) + 1;
	*lpdwSize += nLength;
	D2_ASSERT(*lpdwSize < dwMaxSize);
	std::memcpy(&pStrings[*lpdwSize - nLength], szString, nLength);
}

static BOOL TOMCP_Send(const void* pMessage, uint32_t dwSize)
{
	return FOG_10070_ClientSendMessage(gpMCPConnection, pMessage, dwSize);
}

static BOOL TOMCP_SendMessageId(uint8_t nMessageId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);
	return TOMCP_Send(&nMessageId, sizeof(nMessageId));
}

//D2MCPClient.0x6F9F1F20
BOOL __fastcall TOMCP_Startup(const D2MCPConnectInfoStrc* pConnectInfo)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPStartupMsgStrc tMsg{};
	tMsg.nMessageId = MCP_STARTUP;
	tMsg.dwMCPCookie = pConnectInfo->dwMCPCookie;
	tMsg.dwMCPStatus = pConnectInfo->dwMCPStatus;
	std::memcpy(tMsg.dwMCPChunk1, pConnectInfo->dwMCPChunk1, sizeof(tMsg.dwMCPChunk1));
	std::memcpy(tMsg.dwMCPChunk2, pConnectInfo->dwMCPChunk2, sizeof(tMsg.dwMCPChunk2));

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, pConnectInfo->szBattleNetUniqueName, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPStartupMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F2020
static BOOL __fastcall TOMCP_CharCreate(const char* szCharName, uint32_t nClass, uint16_t nFlags)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);
	D2MCPClientSetCharCreateReceived(FALSE);

	D2MCPCharCreateMsgStrc tMsg{};
	tMsg.nMessageId = MCP_CHARCREATE;
	tMsg.nClass = nClass;
	tMsg.nFlags = nFlags;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szCharName, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPCharCreateMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F1CA0 (#10008)
BOOL __stdcall D2MCPClientCharCreate(const char* szCharName, uint32_t nClass, uint16_t nFlags)
{
	return TOMCP_CharCreate(szCharName, nClass, nFlags);
}

//D2MCPClient.0x6F9F20E0
static BOOL __fastcall TOMCP_CharLogon(const char* szCharName)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPStringsMsgStrc tMsg{};
	tMsg.nMessageId = MCP_CHARLOGON;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szCharName, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPStringsMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F1CC0 (#10009)
BOOL __stdcall D2MCPClientCharLogon(const char* szCharName)
{
	return TOMCP_CharLogon(szCharName);
}

//D2MCPClient.0x6F9F2180 (#10004)
BOOL __fastcall D2MCPClientCreateGame(const D2MCPCreateGameInfoStrc* pCreateGameInfo, uint16_t nRequestId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPCreateGameMsgStrc tMsg{};
	tMsg.nMessageId = MCP_CREATEGAME;
	tMsg.nRequestId = nRequestId;
	tMsg.dwDifficulty = pCreateGameInfo->dwDifficulty;
	tMsg.nUnk0x07 = pCreateGameInfo->nUnk0x44;
	tMsg.nLevelDifference = pCreateGameInfo->nLevelDifference;
	tMsg.nMaxPlayers = pCreateGameInfo->nMaxPlayers;
	gnMCPPendingRequestId = nRequestId;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, pCreateGameInfo->szGameName, &dwSize, sizeof(tMsg.szStrings));
	TOMCP_PackString(tMsg.szStrings, pCreateGameInfo->szGamePassword, &dwSize, sizeof(tMsg.szStrings));
	TOMCP_PackString(tMsg.szStrings, pCreateGameInfo->szGameDescription, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPCreateGameMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);

	gdwMCPGameToken = 0;
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F2300 (#10005)
BOOL __fastcall D2MCPClientJoinGame(const char* szGameName, const char* szGamePassword, uint16_t nRequestId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPRequestStringsMsgStrc tMsg{};
	tMsg.nMessageId = MCP_JOINGAME;
	tMsg.nRequestId = nRequestId;
	gnMCPPendingRequestId = nRequestId;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szGameName, &dwSize, sizeof(tMsg.szStrings));
	TOMCP_PackString(tMsg.szStrings, szGamePassword, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPRequestStringsMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F2400 (#10006)
BOOL __fastcall D2MCPClientRequestGameList(const D2MCPGameListRequestStrc* pRequest, uint16_t nRequestId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPGameListMsgStrc tMsg{};
	tMsg.nMessageId = MCP_GAMELIST;
	tMsg.nRequestId = nRequestId;
	tMsg.dwUnk0x03 = pRequest->dwUnk0x14;
	gnMCPPendingRequestId = nRequestId;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, pRequest->szSearchString, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPGameListMsgStrc, szStrings);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F2490 (#10007)
BOOL __fastcall D2MCPClientRequestGameInfo(const char* szGameName, uint16_t nRequestId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPRequestStringsMsgStrc tMsg{};
	tMsg.nMessageId = MCP_GAMEINFO;
	tMsg.nRequestId = nRequestId;
	gnMCPPendingRequestId = nRequestId;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szGameName, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPRequestStringsMsgStrc, szStrings);
	return TOMCP_Send(&tMsg, dwSize);
}

static BOOL TOMCP_SendRequestString(uint8_t nMessageId, const char* szString, uint16_t nRequestId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPRequestStringsMsgStrc tMsg{};
	tMsg.nMessageId = nMessageId;
	tMsg.nRequestId = nRequestId;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szString, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPRequestStringsMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);

	gnMCPPendingRequestId = nRequestId;
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F2520 (#10013)
BOOL __fastcall D2MCPClientSendMsg08(const char* szString, uint16_t nRequestId)
{
	return TOMCP_SendRequestString(MCP_MSG_08, szString, nRequestId);
}

//D2MCPClient.0x6F9F25D0 (#10014)
BOOL __fastcall D2MCPClientSendMsg09(const char* szString, uint16_t nRequestId)
{
	return TOMCP_SendRequestString(MCP_MSG_09, szString, nRequestId);
}

//D2MCPClient.0x6F9F2680 (#10021)
BOOL __fastcall D2MCPClientCharDelete(const char* szCharName, uint16_t nRequestId)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPRequestStringsMsgStrc tMsg{};
	gnMCPPendingRequestId = nRequestId;
	tMsg.nMessageId = MCP_CHARDELETE;
	tMsg.nRequestId = nRequestId;
	SStrCopy(tMsg.szStrings, szCharName, sizeof(tMsg.szStrings));
	return TOMCP_Send(&tMsg, offsetof(D2MCPRequestStringsMsgStrc, szStrings) + SStrLen(tMsg.szStrings) + 1);
}

//D2MCPClient.0x6F9F2730 (#10022)
BOOL __fastcall D2MCPClientSendMsg0B(const char* szString1, const char* szString2)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPStringsMsgStrc tMsg{};
	tMsg.nMessageId = MCP_MSG_0B;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szString1, &dwSize, sizeof(tMsg.szStrings));
	TOMCP_PackString(tMsg.szStrings, szString2, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPStringsMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F2820 (#10023)
BOOL __fastcall D2MCPClientSendMsg0C(const char* szString)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPStringsMsgStrc tMsg{};
	tMsg.nMessageId = MCP_MSG_0C;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szString, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPStringsMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F28C0 (#10024)
BOOL __fastcall D2MCPClientSendMsg0D()
{
	return TOMCP_SendMessageId(MCP_MSG_0D);
}

//D2MCPClient.0x6F9F28F0 (#10025)
BOOL __fastcall D2MCPClientSendMsg0F()
{
	return TOMCP_SendMessageId(MCP_MSG_0F);
}

//D2MCPClient.0x6F9F2920 (#10026)
BOOL __fastcall D2MCPClientSendMsg10()
{
	return TOMCP_SendMessageId(MCP_MSG_10);
}

//D2MCPClient.0x6F9F2950 (#10027)
BOOL __fastcall D2MCPClientRequestLadderData(uint8_t nLadderType, uint16_t nStartingPosition)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPLadderDataRequestMsgStrc tMsg{};
	tMsg.nMessageId = MCP_REQUESTLADDERDATA;
	tMsg.nLadderType = nLadderType;
	tMsg.nStartingPosition = nStartingPosition;
	return TOMCP_Send(&tMsg, sizeof(tMsg));
}

//D2MCPClient.0x6F9F2990 (#10049)
BOOL __fastcall D2MCPClientRequestMotd()
{
	return TOMCP_SendMessageId(MCP_MOTD);
}

//D2MCPClient.0x6F9F29C0 (#10054)
BOOL __fastcall D2MCPClientCancelGameCreate()
{
	return TOMCP_SendMessageId(MCP_CANCELGAMECREATE);
}

//D2MCPClient.0x6F9F29F0 (#10056)
BOOL __fastcall D2MCPClientRequestCharList()
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPCharListRequestMsgStrc tMsg{};
	tMsg.nMessageId = MCP_CHARLIST2;
	tMsg.nCharacters = 8;
	return TOMCP_Send(&tMsg, sizeof(tMsg));
}

//D2MCPClient.0x6F9F2A20 (#10059)
BOOL __fastcall D2MCPClientCharRank(const char* szCharName, uint32_t dwUnk1, uint32_t dwUnk2)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPCharRankMsgStrc tMsg{};
	tMsg.nMessageId = MCP_CHARRANK;
	tMsg.dwUnk1 = dwUnk1;
	tMsg.dwUnk2 = dwUnk2;

	uint32_t dwSize = 0;
	TOMCP_PackString(tMsg.szStrings, szCharName, &dwSize, sizeof(tMsg.szStrings));
	dwSize += offsetof(D2MCPCharRankMsgStrc, szStrings);
	D2_ASSERT(dwSize < USHRT_MAX);
	return TOMCP_Send(&tMsg, dwSize);
}

//D2MCPClient.0x6F9F26E0 (#10062)
BOOL __fastcall D2MCPClientCharUpgrade(const char* szCharName)
{
	D2MCPClientSetLastError(MCPCLIENT_ERROR_NONE);

	D2MCPStringsMsgStrc tMsg{};
	tMsg.nMessageId = MCP_CHARUPGRADE;
	SStrCopy(tMsg.szStrings, szCharName, sizeof(tMsg.szStrings));
	return TOMCP_Send(&tMsg, offsetof(D2MCPStringsMsgStrc, szStrings) + SStrLen(tMsg.szStrings) + 1);
}
