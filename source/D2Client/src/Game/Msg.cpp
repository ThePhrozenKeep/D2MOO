#include <Game/Msg.h> 
#include <D2PacketDef.h>
#include <Storm.h>
#include <Game/Game.h>
#include <D2Client.h>
#include <D2StrTable.h>
#include <D2Net.h>
#include <Client.h>
#include <Core/WINMAIN.h>

D2CLIENTSTUB(sub, 6FAA6A00, uint32_t, __fastcall, ());
D2CLIENTSTUB(CONFIG_GetCTemp, 6FAA69F0, uint32_t, __fastcall, ());
D2VAR(D2CLIENT, peConfigCharacterPlayerClass, uint32_t, 0x6FBA7894 - D2ClientImageBase);
D2VAR(D2CLIENT, pgszPlayerName, char, 0x6FBA7810 - D2ClientImageBase);//16 bytes
D2CLIENTDWORDSTUB(6FBC1AF4);
D2CLIENTDWORDSTUB(6FBC1AFC);

//1.10f:D2Client.0x6FAAD260
void __fastcall SendNewGamePacket(const char* szGameName)
{
	D2GSPacketClt66 tPacketClt66;
	tPacketClt66.nHeader = 0x66;
	SStrCopy(tPacketClt66.szGameName, szGameName, STORM_MAX_STR);
	switch (GAME_GetGameType())
	{
	case GAMETYPE_SINGLEPLAYER:
		tPacketClt66.nGameType = 3;
		break;
	case GAMETYPE_OBNET_HOST:
		tPacketClt66.nGameType = 1;
		break;
	case GAMETYPE_LAN_HOST:
		tPacketClt66.nGameType = 2;
		break;
	default:
		tPacketClt66.nGameType = 0;
		break;

	}
	if (sub_6FAA6A00())
		tPacketClt66.nPlayerClass = CONFIG_GetCTemp_6FAA69F0();
	else
		tPacketClt66.nPlayerClass = *D2CLIENT_peConfigCharacterPlayerClass;
	tPacketClt66.nLocale = STRTABLE_GetLanguage();
	D2ConfigStrc* pCfg = CONFIG_GetConfig();
	if (!pCfg->nArenaFlags)
	{
		pCfg->nArenaFlags = 4;
		pCfg->nArenaFlags |= 0x100000u;
	}
	tPacketClt66.wArena = pCfg->wArena;
	tPacketClt66.nGameFlags = pCfg->nArenaFlags;
	tPacketClt66.nTemplate = pCfg->nArenaTemplate;
	tPacketClt66.unk0x2B = pCfg->_01E9[0];
	tPacketClt66.unk0x2C = pCfg->_01E9[1];
	tPacketClt66.nDifficulty = pCfg->nArenaDifficulty;
	SStrCopy(tPacketClt66.szClientName, D2CLIENT_pgszPlayerName, STORM_MAX_STR);
	CLIENT_Send(0, (const uint8_t*)&tPacketClt66, sizeof(D2GSPacketClt66));
	*D2CLIENT_pdword_6FBC1AF4 += sizeof(D2GSPacketClt66);
	++(*D2CLIENT_pdword_6FBC1AFC);
}
