#include <UI/npcmenu.h> 
#include <Unit/CUnit.h>
#include <D2Client.h>

#ifdef D2_VERSION_110F

//1.10f: D2Client.0x6FBB5CF9
//1.13c: D2Client.0x6FBC9721
D2VAR(D2CLIENT, pgbNpcActive, BOOL, 0x6FBB5CF9 - D2ClientImageBase);
//1.10f: D2Client.0x6FBB5CF5
//1.13c: D2Client.0x6FBC971D
D2VAR(D2CLIENT, pgnActiveNpcGUID, D2UnitGUID, 0x6FBB5CF5 - D2ClientImageBase);

//1.10f: D2Client.0x6FAF3930
//1.13c: D2Client.0x6FAF6150
D2UnitStrc* __fastcall UI_NPC_GetActiveNpcUnit()
{
	if (*D2CLIENT_pgbNpcActive)
		return CUNIT_GetClientUnit(*D2CLIENT_pgnActiveNpcGUID, UNIT_MONSTER);
	else
		return nullptr;
}

#endif // D2_VERSION_110F
