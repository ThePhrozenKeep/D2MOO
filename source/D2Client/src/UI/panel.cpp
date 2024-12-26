#include <UI/panel.h>
#include <D2Client.h>
#include <Core/ARCHIVE.h>
#include <UI/UI.h>
#include <Fog.h>
#include <D2StrTable.h>
#include <stdio.h>

#ifdef D2_VERSION_110F

D2CLIENTSTUB(GetExpansionFlag, 6FAAC080, BOOL, __fastcall, ());
D2VAR(D2CLIENT, pgpCellfile_Panel_Button, D2CellFileStrc*, 0x6FBB5E2C - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_InvChar, D2CellFileStrc*, 0x6FBB5E24 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_Arrow, D2CellFileStrc*, 0x6FBB5E20 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_InvWarn, D2CellFileStrc*, 0x6FBB5E58 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_DuroIcon, D2CellFileStrc*, 0x6FBB5E28 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_GemSocket, D2CellFileStrc*, 0x6FBB5E34 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_PopBelt, D2CellFileStrc*, 0x6FBB5E1C - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_800ControlPanel7, D2CellFileStrc*, 0x6FBB5E54 -D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_800BorderFrame, D2CellFileStrc*, 0x6FBB5E3C -D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_ControlPanel7, D2CellFileStrc*, 0x6FBB5E44 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_HealthMana, D2CellFileStrc*, 0x6FBB5E30 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_Overlap, D2CellFileStrc*, 0x6FBB5E8C - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_MenuButton, D2CellFileStrc*, 0x6FBB5E18 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_MenuPanel, D2CellFileStrc*, 0x6FBB5E4C - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_Skillpoints, D2CellFileStrc*, 0x6FBB5E50 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_RunButton, D2CellFileStrc*, 0x6FBB5E48 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_Panel_HostilePic, D2CellFileStrc*, 0x6FBB5E40 -D2ClientImageBase);

D2VAR(D2CLIENT, pgnStatsValues, DWORD, 0x6FBB5E5C -D2ClientImageBase); // 12 stats

//1.10f: D2Client.0x6FAFE370
void __fastcall UI_PANEL_Init()
{
	*D2CLIENT_pgpCellfile_Panel_Button = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\btn");
	bool bIsExpansion = FOG_IsExpansion() && D2CLIENT_GetExpansionFlag_6FAAC080();

	const D2C_Language nLocaleId = STRTABLE_GetLanguage();
	if (nLocaleId >= LANGUAGE_JAPANESE && nLocaleId <= LANGUAGE_CHINESETWN)
	{
		char Dest[D2_MAX_PATH];
		sprintf(Dest, bIsExpansion ? "%s\\UI\\InvChar6" : "%s\\UI\\InvChar", "DATA\\LOCAL");
		*D2CLIENT_pgpCellfile_Panel_InvChar = D2CLIENT_CELLFILE_GetCellFile(Dest, 0);
	}
	else
	{
		*D2CLIENT_pgpCellfile_Panel_InvChar = D2CLIENT_CELLFILE_GetCellFileFromUIFolder(bIsExpansion ? "Panel\\InvChar6" : "Panel\\InvChar");
	}

	*D2CLIENT_pgpCellfile_Panel_Arrow = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\Arrow");
	*D2CLIENT_pgpCellfile_Panel_InvWarn = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\Invwarn");
	*D2CLIENT_pgpCellfile_Panel_DuroIcon = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\DuroIcon");
	*D2CLIENT_pgpCellfile_Panel_GemSocket = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\GemSocket");
	*D2CLIENT_pgpCellfile_Panel_PopBelt = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\ctrlpnl_popbelt");
	*D2CLIENT_pgpCellfile_Panel_HealthMana = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\Hlthmana");
	*D2CLIENT_pgpCellfile_Panel_Overlap = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\overlap");
	*D2CLIENT_pgpCellfile_Panel_MenuButton = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\menubutton");
	*D2CLIENT_pgpCellfile_Panel_MenuPanel = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\menupanel");
	*D2CLIENT_pgpCellfile_Panel_Skillpoints = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\skillpoints");
	*D2CLIENT_pgpCellfile_Panel_RunButton = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\runbutton");
	*D2CLIENT_pgpCellfile_Panel_HostilePic = D2CLIENT_CELLFILE_GetCellFileFromUIFolder("Panel\\hostilepic");
	memset(D2CLIENT_pgnStatsValues, 0, 12 * sizeof(DWORD));
}

//1.10f: D2Client.0x6FAFE4D0
void __fastcall UI_PANEL_Shutdown()
{
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_Button);
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_InvChar);
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_Arrow );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_InvWarn );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_DuroIcon );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_GemSocket );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_PopBelt );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_800ControlPanel7);
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_800BorderFrame);
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_ControlPanel7);
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_HealthMana );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_Overlap );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_MenuButton );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_MenuPanel );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_Skillpoints );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_RunButton );
	SafeFreeCellFile(D2CLIENT_pgpCellfile_Panel_HostilePic);
	memset(D2CLIENT_pgnStatsValues, 0, 12 * sizeof(DWORD));
}

#endif //D2_VERSION_110F
