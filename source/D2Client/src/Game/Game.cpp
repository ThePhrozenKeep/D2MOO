#include <Game/Game.h> 

#include <D2Client.h>
#include <D2Dll.h>

HMODULE delayedD2CLIENTDllBaseGet()
{
	static HMODULE DLLBASE_D2LANG = LoadLibraryA("D2Client.dll");
	return DLLBASE_D2LANG;
}

//1.10f:D2Client.0x6FBBC200
//1.13c:D2Client.0x6FBCBBFC
//D2UnitStrc* gpPlayerUnit_6FBBC200;
D2VAR(D2CLIENT, pgpPlayerUnit_6FBBC200, D2UnitStrc*, 0x6FBBC200 - D2ClientImageBase);
//1.00 :D2Client.0x10001CA3
//1.10f:D2Client.0x6FB283D0
D2UnitStrc* __fastcall D2CLIENT_GetPlayerUnit_6FB283D0()
{
    return *D2CLIENT_pgpPlayerUnit_6FBBC200;
}
