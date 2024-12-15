#include <CGAME/View.h>
#include <D2Client.h>

#ifdef D2_VERSION_110F
D2CLIENTSTUB(D2CLIENT_GAMEVIEW_AllocateGameView, 6FAB6FF0, D2GameViewStrc*, __fastcall, ());

//1.10f: D2Client.0x6FAB6FF0
D2GameViewStrc* __fastcall GAMEVIEW_AllocateGameView()
{
	return D2CLIENT_GAMEVIEW_AllocateGameView_6FAB6FF0();
}
#endif
