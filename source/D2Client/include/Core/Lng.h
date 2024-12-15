#pragma once  
#include <D2BasicTypes.h>


//1.10f: D2Client.0x6FAA11E0
BOOL __fastcall LNG_RegisterCmdLineArguments();

//1.10f: D2Client.0x6FAA1210
BOOL __fastcall LNG_Initialize();


//1.10f: D2Client.0x6FAA1300
void __fastcall LNG_Shutdown();

//1.10f: D2Client.0x6FAA1330
BOOL __stdcall LNG_MessageSource(DWORD dwMessageId, char* buffer, unsigned int iMaxLength);

//Helper
HACCEL LNG_GetAccelerationTable();
