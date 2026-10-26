#pragma once


#include <Windows.h>

#include <cstdint>


//1.10f Image base: 0x6F9F0000

#ifdef D2_VERSION_110F
constexpr int D2MCPClientImageBase = 0x6F9F0000;
#endif

// Messages exchanged with the Realm server (MCP). The values are the first byte of each message.
enum D2C_MCPMessages : uint8_t
{
	MCP_STARTUP = 0x01,
	MCP_CHARCREATE = 0x02,
	MCP_CREATEGAME = 0x03,
	MCP_JOINGAME = 0x04,
	MCP_GAMELIST = 0x05,
	MCP_GAMEINFO = 0x06,
	MCP_CHARLOGON = 0x07,
	MCP_MSG_08 = 0x08,
	MCP_MSG_09 = 0x09,
	MCP_CHARDELETE = 0x0A,
	MCP_MSG_0B = 0x0B,
	MCP_MSG_0C = 0x0C,
	MCP_MSG_0D = 0x0D,
	MCP_MSG_0E = 0x0E,
	MCP_MSG_0F = 0x0F,
	MCP_MSG_10 = 0x10,
	MCP_REQUESTLADDERDATA = 0x11,
	MCP_MOTD = 0x12,
	MCP_CANCELGAMECREATE = 0x13,
	MCP_CREATEQUEUE = 0x14,
	MCP_CHARRANK = 0x16,
	MCP_CHARLIST = 0x17,
	MCP_CHARUPGRADE = 0x18,
	MCP_CHARLIST2 = 0x19,

	NUM_MCP_MESSAGES = 0x1A,
};

// Error codes reported through D2MCPClientGetLastError, besides the result codes sent by the server.
enum D2C_MCPClientErrors : uint32_t
{
	MCPCLIENT_ERROR_NONE = 0,
	MCPCLIENT_ERROR_CONNECTION_FAILED = 2,
	MCPCLIENT_ERROR_STARTUP_SEND_FAILED = 11,
	MCPCLIENT_ERROR_GAME_NOT_FOUND = 40,
	MCPCLIENT_ERROR_GAME_LIST_FAILED = 50,
	MCPCLIENT_ERROR_GAME_LIST_UNAVAILABLE = 51,
};

#pragma pack(push, 1)

// Realm logon information obtained from Battle.net (SID_LOGONREALMEX), see S2C_Packet_SID_LOGONREALMEX
struct D2MCPConnectInfoStrc				// sizeof 0x148
{
	uint32_t dwMCPCookie;				// 0x00
	uint32_t dwMCPStatus;				// 0x04
	uint32_t dwMCPChunk1[2];			// 0x08
	uint32_t dwMCPIp;					// 0x10
	uint32_t dwMCPPort;					// 0x14
	uint32_t dwMCPChunk2[12];			// 0x18
	char szBattleNetUniqueName[256];	// 0x48
};

struct D2MCPCreateGameInfoStrc			// sizeof 0x47
{
	char szGameName[16];				// 0x00
	char szGamePassword[16];			// 0x10
	char szGameDescription[32];			// 0x20
	uint32_t dwDifficulty;				// 0x40
	uint8_t nUnk0x44;					// 0x44
	uint8_t nLevelDifference;			// 0x45
	uint8_t nMaxPlayers;				// 0x46
};

struct D2MCPGameListRequestStrc			// sizeof 0x18
{
	char szSearchString[20];			// 0x00
	uint32_t dwUnk0x14;					// 0x14
};

// Filled by MCP_GAMELIST and MCP_GAMEINFO responses
struct D2MCPGameInfoStrc				// sizeof 0x18C
{
	BOOL bMoreData;						// 0x00
	uint16_t nRequestId;				// 0x04
	uint16_t pad0x06;					// 0x06
	uint32_t dwIndex;					// 0x08
	uint32_t dwUnk0x0C;					// 0x0C
	uint8_t nUnk0x10;					// 0x10
	uint8_t nUnk0x11;					// 0x11
	uint8_t nUnk0x12;					// 0x12
	uint8_t pad0x13;					// 0x13
	uint32_t nPlayers;					// 0x14
	int32_t nStatus;					// 0x18
	char szGameName[16];				// 0x1C
	char szGameDescription[32];			// 0x2C
	uint16_t nPlayerClass[16];			// 0x4C
	uint16_t nPlayerLevel[16];			// 0x6C
	char szPlayerName[16][16];			// 0x8C
};

// Header of the MCP_CHARLIST and MCP_CHARLIST2 messages. Names come from the original asserts.
struct D2MCPCharListStrc
{
	uint8_t nMessageId;					// 0x00
	uint16_t count;						// 0x01
	uint16_t firstChar;					// 0x03
	uint16_t totalChars;				// 0x05
	uint16_t maxCharsAllowed;			// 0x07
	// Followed by the characters
};

#pragma pack(pop)

using MCPRequestCallback = void(__fastcall*)(uint16_t nRequestId, const uint8_t* pData);
using MCPLadderDataCallback = void(__fastcall*)(int32_t nUnk0, int32_t nEntries, int32_t nEntrySize, uint8_t nLadderType, const uint8_t* pEntries);
using MCPLadderDataBeginCallback = void(__fastcall*)();
using MCPMotdCallback = void(__fastcall*)(uint8_t nUnk, uint32_t nSize, const uint8_t* pData);

// Connection
//D2MCPClient.0x6F9F17D0 (#10000)
D2MCPCLIENT_DLL_DECL HANDLE __stdcall D2MCPClientConnect(const D2MCPConnectInfoStrc* pConnectInfo);
//D2MCPClient.0x6F9F1980 (#10001)
D2MCPCLIENT_DLL_DECL void __cdecl D2MCPClientCloseMCP();
//D2MCPClient.0x6F9F19E0 (#10002)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientIsConnected();
//D2MCPClient.0x6F9F1A20 (#10003)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientProcessMessages();
//D2MCPClient.0x6F9F17C0 (#10055)
D2MCPCLIENT_DLL_DECL HANDLE __cdecl D2MCPClientGetConnectionThread();

// Requests
//D2MCPClient.0x6F9F1CA0 (#10008)
D2MCPCLIENT_DLL_DECL BOOL __stdcall D2MCPClientCharCreate(const char* szCharName, uint32_t nClass, uint16_t nFlags);
//D2MCPClient.0x6F9F1CC0 (#10009)
D2MCPCLIENT_DLL_DECL BOOL __stdcall D2MCPClientCharLogon(const char* szCharName);
//D2MCPClient.0x6F9F2180 (#10004)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientCreateGame(const D2MCPCreateGameInfoStrc* pCreateGameInfo, uint16_t nRequestId);
//D2MCPClient.0x6F9F2300 (#10005)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientJoinGame(const char* szGameName, const char* szGamePassword, uint16_t nRequestId);
//D2MCPClient.0x6F9F2400 (#10006)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientRequestGameList(const D2MCPGameListRequestStrc* pRequest, uint16_t nRequestId);
//D2MCPClient.0x6F9F2490 (#10007)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientRequestGameInfo(const char* szGameName, uint16_t nRequestId);
//D2MCPClient.0x6F9F2520 (#10013)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg08(const char* szString, uint16_t nRequestId);
//D2MCPClient.0x6F9F25D0 (#10014)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg09(const char* szString, uint16_t nRequestId);
//D2MCPClient.0x6F9F2680 (#10021)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientCharDelete(const char* szCharName, uint16_t nRequestId);
//D2MCPClient.0x6F9F2730 (#10022)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg0B(const char* szString1, const char* szString2);
//D2MCPClient.0x6F9F2820 (#10023)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg0C(const char* szString);
//D2MCPClient.0x6F9F28C0 (#10024)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg0D();
//D2MCPClient.0x6F9F28F0 (#10025)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg0F();
//D2MCPClient.0x6F9F2920 (#10026)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientSendMsg10();
//D2MCPClient.0x6F9F2950 (#10027)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientRequestLadderData(uint8_t nLadderType, uint16_t nStartingPosition);
//D2MCPClient.0x6F9F2990 (#10049)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientRequestMotd();
//D2MCPClient.0x6F9F29C0 (#10054)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientCancelGameCreate();
//D2MCPClient.0x6F9F29F0 (#10056)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientRequestCharList();
//D2MCPClient.0x6F9F2A20 (#10059)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientCharRank(const char* szCharName, uint32_t dwUnk1, uint32_t dwUnk2);
//D2MCPClient.0x6F9F26E0 (#10062)
D2MCPCLIENT_DLL_DECL BOOL __fastcall D2MCPClientCharUpgrade(const char* szCharName);

// Callbacks
//D2MCPClient.0x6F9F1780 (#10011)
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClientSetMsg08Callback(MCPRequestCallback pCallback);
//D2MCPClient.0x6F9F1790 (#10010)
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClientSetMsg09Callback(MCPRequestCallback pCallback);
//D2MCPClient.0x6F9F17A0 (#10012)
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClientSetLadderDataCallbacks(MCPLadderDataCallback pCallback, MCPLadderDataBeginCallback pBeginCallback);
//D2MCPClient.0x6F9F17B0 (#10048)
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClientSetMotdCallback(MCPMotdCallback pCallback);

// State
//D2MCPClient.0x6F9F1AB0 (#10046)
D2MCPCLIENT_DLL_DECL uint32_t __cdecl D2MCPClientGetLastError();
//D2MCPClient.0x6F9F1AA0 (#10047)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetLastError(uint32_t nError);
//D2MCPClient.0x6F9F1A70 (#10015)
D2MCPCLIENT_DLL_DECL const char* __cdecl D2MCPClientGetGameServerIp();
//D2MCPClient.0x6F9F1A80 (#10016)
D2MCPCLIENT_DLL_DECL uint16_t __cdecl D2MCPClientGetGameRequestId();
//D2MCPClient.0x6F9F1A90 (#10017)
D2MCPCLIENT_DLL_DECL uint32_t __cdecl D2MCPClientGetGameToken();
//D2MCPClient.0x6F9F1C90 (#10018)
D2MCPCLIENT_DLL_DECL uint32_t __cdecl D2MCPClientGetGameHash();
//D2MCPClient.0x6F9F1C00 (#10019)
D2MCPCLIENT_DLL_DECL D2MCPGameInfoStrc* __cdecl D2MCPClientGetGameInfo();
//D2MCPClient.0x6F9F1C10 (#10020)
D2MCPCLIENT_DLL_DECL void __cdecl D2MCPClientClearGameInfo();
//D2MCPClient.0x6F9F1D10 (#10057)
D2MCPCLIENT_DLL_DECL D2MCPCharListStrc* __cdecl D2MCPClientGetCharacterList();
//D2MCPClient.0x6F9F1D20 (#10058)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCharacterList(const uint8_t* pMessage, uint32_t nMessageSize);
//D2MCPClient.0x6F9F1B80 (#10038)
D2MCPCLIENT_DLL_DECL BOOL __stdcall D2MCPClientGetMsg0EStrings(char* szString1, char* szString2);
//D2MCPClient.0x6F9F1BC0 (#10039)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetMsg0EStrings(BOOL bReceived, const char* szString1, const char* szString2);
//D2MCPClient.0x6F9F1CD0 (#10050)
D2MCPCLIENT_DLL_DECL uint32_t __cdecl D2MCPClientGetCreateQueuePosition();
//D2MCPClient.0x6F9F1CE0 (#10051)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCreateQueuePosition(uint32_t nPosition);

// "Response received" flags, set to TRUE when the server answers the matching request.
//D2MCPClient.0x6F9F1AC0 (#10029)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetCharCreateReceived();
//D2MCPClient.0x6F9F1AD0 (#10028)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCharCreateReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1AE0 (#10031)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetCharLogonReceived();
//D2MCPClient.0x6F9F1AF0 (#10030)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCharLogonReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1B00 (#10033)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetCharDeleteReceived();
//D2MCPClient.0x6F9F1B10 (#10032)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCharDeleteReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1B50 (#10035)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetMsg0BReceived();
//D2MCPClient.0x6F9F1B40 (#10034)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetMsg0BReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1B60 (#10037)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetMsg0CReceived();
//D2MCPClient.0x6F9F1B70 (#10036)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetMsg0CReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1C30 (#10040)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetMsg0DReceived();
//D2MCPClient.0x6F9F1C40 (#10041)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetMsg0DReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1C50 (#10042)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetMsg0FReceived();
//D2MCPClient.0x6F9F1C60 (#10043)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetMsg0FReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1C70 (#10044)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetMsg10Received();
//D2MCPClient.0x6F9F1C80 (#10045)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetMsg10Received(BOOL bReceived);
//D2MCPClient.0x6F9F1CF0 (#10052)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetCreateQueueReceived();
//D2MCPClient.0x6F9F1D00 (#10053)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCreateQueueReceived(BOOL bReceived);
//D2MCPClient.0x6F9F1B20 (#10061)
D2MCPCLIENT_DLL_DECL BOOL __cdecl D2MCPClientGetCharUpgradeReceived();
//D2MCPClient.0x6F9F1B30 (#10060)
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClientSetCharUpgradeReceived(BOOL bReceived);
