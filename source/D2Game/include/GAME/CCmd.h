#pragma once

#include <Units/Units.h>


enum D2C_ClientSystemMessage {
	D2CLTSYS_NEWGAME = 0x66,
	D2CLTSYS_JOINGAME = 0x67,
	D2CLTSYS_ENDGAME = 0x68,
	D2CLTSYS_JOINLIST = 0x69,
	D2CLTSYS_JOINACT = 0x6A,
	D2CLTSYS_OPENCHAR = 0x6B,
	D2CLTSYS_0x6C = 0x6C,
	D2CLTSYS_DISCONNECT = 0x6D,
	D2CLTSYS_REMOVEFROMGAME = 0x6F,
};

//D2Game.0x6FC317F0
int32_t __fastcall CCMD_IsStringZeroTerminated(const char* pData, int32_t nDataSize);
//D2Game.0x6FC31810
uint32_t __fastcall CCMD_CanClientJoinGame(int32_t a1, int32_t a2, char* szClientName);
//D2Game.0x6FC31910
void __fastcall CCMD_ProcessClientSystemMessage(void* pData, int32_t nSize);
//D2Game.0x6FC31C00
void __fastcall CCMD_ProcessClientMessage(void* pData, int32_t nPacketSize);
