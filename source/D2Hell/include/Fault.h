#pragma once

#include <Windows.h>

typedef void(__stdcall* FaultAssertHandler)(const char*, unsigned __int32, const char*);
typedef BOOL(__stdcall* MessageSource)(DWORD dwMessageId, char* buffer, unsigned int iMaxLength);

//D2OpenGL.0x1000948F
void __fastcall FaultGetString(DWORD dwMessageId, char* buffer, unsigned int iMaxLength);

//D2OpenGL.0x1000934F
FaultAssertHandler __fastcall FaultSetAssertHandler(FaultAssertHandler newAssertHandler);

//D2OpenGL.0x10009370
//D2Client.0x6FB6BB94
void __fastcall FaultRegisterMessageSource(MessageSource newMessageSource);

//D2OpenGL.0x10009782
BOOL __fastcall FaultDoAssert(const char* a1, unsigned int a2, const char* a3);

//D2OpenGL.0x100095EC
void __cdecl FaultExit();



// See comment near #ifdef XFAULT_IMPL if you get linker errors using any of the XFault functions
BOOL __fastcall XFaultMessage(const char* msg);
BOOL XFaultMessage(DWORD errorCode, ...);

BOOL __fastcall XFaultFatal(const char* msg);
BOOL XFaultFatal(DWORD errorCode, ...);

BOOL XFaultContinuableError(DWORD errorCode, ...);



// To use any of the XFault functions, you need to include this header with XFAULT_IMPL and XFAULT_MODULE_NAME defined in ONE (and only one) of the .cpp files of your .dll.
#ifdef XFAULT_IMPL

const char* gpszModuleName = XFAULT_MODULE_NAME;


BOOL __fastcall XFaultMessage(const char* msg)
{
	MessageBoxA(0, msg, gpszModuleName, MB_ICONHAND | MB_TASKMODAL);
	return TRUE;
}

BOOL XFaultMessage(DWORD errorCode, ...)
{
	va_list va;
	CHAR faultStringBuffer[512];
	CHAR msgBuffer[512];

	va_start(va, errorCode);
	FaultGetString(errorCode, faultStringBuffer, sizeof(faultStringBuffer));
	wvsprintfA(msgBuffer, faultStringBuffer, va);
	va_end(va);
	return XFaultMessage(msgBuffer);
}

BOOL __fastcall XFaultFatal(const char* msg)
{
	MessageBoxA(0, msg, gpszModuleName, MB_ICONHAND | MB_TASKMODAL);
	FaultExit();
	return FALSE;
}

BOOL XFaultFatal(DWORD errorCode, ...)
{
	va_list va;
	CHAR faultStringBuffer[512];
	CHAR msgBuffer[512];

	va_start(va, errorCode);
	FaultGetString(errorCode, faultStringBuffer, sizeof(faultStringBuffer));
	wvsprintfA(msgBuffer, faultStringBuffer, va);
	va_end(va);
	return XFaultFatal(msgBuffer);
}


BOOL XFaultContinuableError(DWORD errorCode, ...)
{
	va_list va;
	CHAR faultStringBuffer[512];
	CHAR msgBuffer[512];

	va_start(va, errorCode);
	FaultGetString(errorCode, faultStringBuffer, sizeof(faultStringBuffer));
	wvsprintfA(msgBuffer, faultStringBuffer, va);
	va_end(va);
	const int buttonID = MessageBoxA(0, msgBuffer, gpszModuleName, MB_RETRYCANCEL | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL);
	if (buttonID == IDRETRY)
		return 1;
	FaultExit();
	return 0;
}


#endif
