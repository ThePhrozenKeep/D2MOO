#pragma once

#pragma warning(push)
#pragma warning(disable:4820 4121)
#include <Winsock2.h>
#pragma warning(pop)

#include <D2BasicTypes.h>
#include <Fog.h>

#pragma pack(1)

struct FSafeSock
{
	BOOL bConnected;
	BOOL bConnecting;
	SOCKET nSocket;
	CRITICAL_SECTION tCriticalSection;
	HANDLE hThread;
	DWORD nThreadId;
	BOOL bStopping;
	DWORD nFogErrorCode;
	DWORD nWSALastError;
	char szName[64];
};

#pragma pack()

D2FUNC_DLL(FOG, SOCKET_CloseSocket, int, __fastcall, (SOCKET), 0x13280)														//Fog.#10000
D2FUNC_DLL(FOG, SOCKET_WSAStartup, BOOL, __fastcall, (), 0x13290)															//Fog.#10001
D2FUNC_DLL(FOG, SOCKET_WSACleanup, int, __fastcall, (), 0x132C0)															//Fog.#10002
D2FUNC_DLL(FOG, SOCKET_Connect, FSafeSock*, __fastcall, (char* szHostAddress, uint16_t nPortNumber, LPTHREAD_START_ROUTINE pThreadProc, LPVOID lpParameter, const char* szName), 0x132D0)						//Fog.#10003
D2FUNC_DLL(FOG, SOCKET_ConnectEx, FSafeSock*, __fastcall, (char* szHostAddress, uint16_t nPortNumber, LPTHREAD_START_ROUTINE pThreadProc, LPVOID lpParameter, const char* szName, BOOL bNonBlocking), 0x132F0)	//Fog.#10004
D2FUNC_DLL(FOG, SOCKET_Destroy, void,  __fastcall, (FSafeSock* pSafeSock), 0x13590)											//Fog.#10005
D2FUNC_DLL(FOG, SOCKET_Receive, int, __fastcall, (FSafeSock* pSafeSock, char* pBuffer, int nBufferLength), 0x13630)			//Fog.#10006
D2FUNC_DLL(FOG, SOCKET_Send, int, __fastcall, (FSafeSock* pSafeSock, char* pBuffer, int nBufferLength), 0x13690)			//Fog.#10007
D2FUNC_DLL(FOG, SOCKET_Select, int,  __fastcall, (FSafeSock* pSafeSock), 0x13770)											//Fog.#10008
D2FUNC_DLL(FOG, SOCKET_SendNoLock, int, __fastcall, (FSafeSock* pSafeSock, char* pBuffer, int nBufferLength), 0x13700)		//Fog.#10009
D2FUNC_DLL(FOG, SOCKET_Lock, void, __fastcall, (FSafeSock* pSafeSock), 0x13750)												//Fog.#10010
D2FUNC_DLL(FOG, SOCKET_Unlock, void, __fastcall, (FSafeSock* pSafeSock), 0x13760)											//Fog.#10011
D2FUNC_DLL(FOG, SOCKET_GetPeerName, BOOL, __fastcall, (FSafeSock* pSafeSock, struct sockaddr* name, int* namelen), 0x13860)	//Fog.#10012
D2FUNC_DLL(FOG, GetHostIPAddress, const char*, __fastcall, (), 0x138E0)														//Fog.#10013
D2FUNC_DLL(FOG, inet_ntoa, const char*, __fastcall, (/*IN_ADDR, but cant due to callconv*/ULONG inaddr_S_addr), 0x13A00)	//Fog.#10014
D2FUNC_DLL(FOG, ResolveHostIPAddress, const char*, __fastcall, (const char* szHostName), 0x13A40)							//Fog.#10015
D2FUNC_DLL(FOG, SOCKET_GetError, BOOL, __fastcall, (FSafeSock* pSafeSock, DWORD* pFogError, int* pWSALastError), 0x13A20)	//Fog.#10016
D2FUNC_DLL(FOG, SOCKET_SetBlockingMode, BOOL, __fastcall, (FSafeSock* pSafeSock, BOOL bNonBlocking), 0x13B20)				//Fog.#10017

// Message based client connection built on top of FSafeSock, used by D2MCPClient.
// Names and signatures are deduced from their usage in D2MCPClient.dll, offsets in Fog.dll are not known yet.
struct FClientConnection;
// Called by the connection for each received message. Returns TRUE if the message was handled, FALSE to queue it for FOG_10072_ClientReceive.
typedef BOOL(__fastcall* FClientConnectionMessageCallback)(const uint8_t* pMessage, uint32_t nMessageSize, void* pUserData);

D2FUNC_DLL(FOG, 10068_ClientCreate, FClientConnection*, __stdcall, (const char* szIpAddress, int nPort, const char* szName), 0)			//Fog.#10068
D2FUNC_DLL(FOG, 10069_ClientClose, void, __stdcall, (FClientConnection* pConnection), 0)													//Fog.#10069
D2FUNC_DLL(FOG, 10070_ClientSendMessage, BOOL, __stdcall, (FClientConnection* pConnection, const void* pMessage, uint32_t nMessageSize), 0)	//Fog.#10070
D2FUNC_DLL(FOG, 10071_ClientSendRaw, BOOL, __stdcall, (FClientConnection* pConnection, const void* pData, uint32_t nDataSize), 0)			//Fog.#10071
D2FUNC_DLL(FOG, 10072_ClientReceive, uint16_t, __stdcall, (FClientConnection* pConnection, uint8_t* pBuffer, uint32_t nBufferSize), 0)		//Fog.#10072
D2FUNC_DLL(FOG, 10073_ClientStart, void, __stdcall, (FClientConnection* pConnection), 0)													//Fog.#10073
D2FUNC_DLL(FOG, 10075_ClientDestroy, void, __stdcall, (FClientConnection* pConnection), 0)													//Fog.#10075
D2FUNC_DLL(FOG, 10077_ClientSetMessageCallback, void, __stdcall, (FClientConnection* pConnection, FClientConnectionMessageCallback pCallback, void* pUserData), 0) //Fog.#10077
D2FUNC_DLL(FOG, 10079_ClientGetSafeSock, FSafeSock*, __stdcall, (FClientConnection* pConnection), 0)										//Fog.#10079
