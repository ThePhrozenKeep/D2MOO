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
