#pragma once

#include <Units/Units.h>


enum D2C_ClientSystemMessage {
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
	// 1.14d: every id is one higher, there is no REMOVEFROMGAME (0x6F is ignored) and 0x70 is new
	D2CLTSYS_NEWGAME = 0x67,
	D2CLTSYS_JOINGAME = 0x68,
	D2CLTSYS_ENDGAME = 0x69,
	D2CLTSYS_JOINLIST = 0x6A,
	D2CLTSYS_JOINACT = 0x6B,
	D2CLTSYS_OPENCHAR = 0x6C,
	D2CLTSYS_PING = 0x6D,
	D2CLTSYS_DISCONNECT = 0x6E,
	D2CLTSYS_REQUESTDISCONNECT = 0x70, // flags the client; the client timeout check then saves and disconnects it
#else
	D2CLTSYS_NEWGAME = 0x66,
	D2CLTSYS_JOINGAME = 0x67,
	D2CLTSYS_ENDGAME = 0x68,
	D2CLTSYS_JOINLIST = 0x69,
	D2CLTSYS_JOINACT = 0x6A,
	D2CLTSYS_OPENCHAR = 0x6B,
	D2CLTSYS_PING = 0x6C,
	D2CLTSYS_DISCONNECT = 0x6D,
	D2CLTSYS_REMOVEFROMGAME = 0x6F,
#endif
};

//D2Game.0x6FC317F0
int32_t __fastcall CCMD_IsStringZeroTerminated(const char* pData, int32_t nDataSize);
//D2Game.0x6FC31810
uint32_t __fastcall CCMD_CanClientJoinGame(int32_t a1, int32_t a2, char* szClientName);
//D2Game.0x6FC31910
void __fastcall CCMD_ProcessClientSystemMessage(void* pData, int32_t nSize);
//D2Game.0x6FC31C00
void __fastcall CCMD_ProcessClientMessage(void* pData, int32_t nPacketSize);
