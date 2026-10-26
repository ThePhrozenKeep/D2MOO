#pragma once


#include <cstdint>

#include <Windows.h>

#include "D2MCPClient.h"


struct FClientConnection;

// Max size of a message, both sent and received
constexpr uint32_t MCP_MESSAGE_BUFFER_SIZE = 0x400;

//D2MCPClient.0x6FA02E58
extern FClientConnection* gpMCPConnection;
//D2MCPClient.0x6FA02CD0
extern uint16_t gnMCPPendingRequestId;
//D2MCPClient.0x6FA02CCC
extern uint32_t gdwMCPGameToken;


//D2MCPClient.0x6F9F16F0
BOOL __fastcall MCP_DispatchMessage(const uint8_t* pMessage, uint32_t nMessageSize);
//D2MCPClient.0x6F9F1730
BOOL __fastcall MCP_DispatchConnectionMessage(const uint8_t* pMessage, uint32_t nMessageSize, void* pUserData);

//D2MCPClient.0x6F9F1F20
BOOL __fastcall TOMCP_Startup(const D2MCPConnectInfoStrc* pConnectInfo);
