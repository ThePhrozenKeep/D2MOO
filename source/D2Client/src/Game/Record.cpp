#include <CGAME/Record.h> 
#include <D2Client.h>
#include <Fog.h>
#include <D2CommonDefinitions.h>

#ifdef D2_VERSION_110F
D2CLIENTDWORDSTUB(6FBAB98C);
D2CLIENTDWORDSTUB(6FBAB984);
//1.10f:D2Client.0x6FAAEB20
void __fastcall RECORD_ToggleRecording()
{
	if (*D2CLIENT_pdword_6FBAB98C)
		*D2CLIENT_pdword_6FBAB98C = 0;
	else
		*D2CLIENT_pdword_6FBAB984 = 1;
}

//1.10f:D2Client.0x6FAAE940
void __fastcall RECORD_Play()
{
	UNIMPLEMENTED();
}

#endif //D2_VERSION_110F
