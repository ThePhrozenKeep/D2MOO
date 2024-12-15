#pragma once  
#include <D2BasicTypes.h>

//1.10f: D2Client.0x6FAAD260
void __fastcall SendNewGamePacket(const char* szGameName);

//1.10f: D2Client.0x6FAAD560
void __fastcall MSG_Send_D2CLTSYS_OPENCHAR(BOOL bUnk);