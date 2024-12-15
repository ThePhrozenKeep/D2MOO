#include <D2BasicTypes.h>
#include <UI/CmdTbl.h> 
#include <D2Client.h>
#include <Fog.h>
#include <Core/WNDPROC.h>
#include <CGAME/Game.h>

#ifdef D2_VERSION_110F

HMODULE delayedD2CLIENTDllBaseGet();
//1.10f: D2Client.0x6FB78500
HotkeyMappingEntry gDefaultCommandTable[] = {
	{ HOTKEY_STATSCREEN, D2VK_A, 1 },
	{ HOTKEY_STATSCREEN, D2VK_C, 0 },
	{ HOTKEY_INVENTORY, D2VK_B, 0 },
	{ HOTKEY_INVENTORY, D2VK_I, 1 },
	{ HOTKEY_PARTY, D2VK_P, 1 },
	{ HOTKEY_PARTY, D2VK_INVALID, 0 },
	{ HOTKEY_MSGLOG, D2VK_M, 1 },
	{ HOTKEY_MSGLOG, D2VK_INVALID, 0 },
	{ HOTKEY_QUESTS, D2VK_Q, 1 },
	{ HOTKEY_QUESTS, D2VK_INVALID, 0 },
	{ HOTKEY_CHAT, D2VK_RETURN, 1 },
	{ HOTKEY_CHAT, D2VK_INVALID, 0 },
	{ HOTKEY_HELP, D2VK_H, 1 },
	{ HOTKEY_HELP, D2VK_INVALID, 0 },
	{ HOTKEY_AUTOMAP, D2VK_TAB, 1 },
	{ HOTKEY_AUTOMAP, D2VK_MBUTTON, 0 },
	{ HOTKEY_MINIMAP_POSITION, D2VK_F9, 1 },
	{ HOTKEY_MINIMAP_POSITION, D2VK_INVALID, 0 },
	{ HOTKEY_FADEAUTOMAP, D2VK_F10, 1 },
	{ HOTKEY_FADEAUTOMAP, D2VK_INVALID, 0 },
	{ HOTKEY_PARTYONMAP, D2VK_F11, 1 },
	{ HOTKEY_PARTYONMAP, D2VK_INVALID, 0 },
	{ HOTKEY_NAMESONMAP, D2VK_F12, 1 },
	{ HOTKEY_NAMESONMAP, D2VK_INVALID, 0 },
	{ HOTKEY_SKILLTREE, D2VK_T, 1 },
	{ HOTKEY_SKILLTREE, D2VK_INVALID, 0 },
	{ HOTKEY_SKILLSELECT, D2VK_S, 1 },
	{ HOTKEY_SKILLSELECT, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL1, D2VK_F1, 1 },
	{ HOTKEY_SKILL1, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL2, D2VK_F2, 1 },
	{ HOTKEY_SKILL2, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL3, D2VK_F3, 1 },
	{ HOTKEY_SKILL3, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL4, D2VK_F4, 1 },
	{ HOTKEY_SKILL4, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL5, D2VK_F5, 1 },
	{ HOTKEY_SKILL5, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL6, D2VK_F6, 1 },
	{ HOTKEY_SKILL6, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL7, D2VK_F7, 1 },
	{ HOTKEY_SKILL7, D2VK_INVALID, 0 },
	{ HOTKEY_SHOWBELT, D2VK_OEM_3, 1 },
	{ HOTKEY_SHOWBELT, D2VK_INVALID, 0 },
	{ HOTKEY_POTION1, D2VK_1, 1 },
	{ HOTKEY_POTION1, D2VK_INVALID, 0 },
	{ HOTKEY_POTION2, D2VK_2, 1 },
	{ HOTKEY_POTION2, D2VK_INVALID, 0 },
	{ HOTKEY_POTION3, D2VK_3, 1 },
	{ HOTKEY_POTION3, D2VK_INVALID, 0 },
	{ HOTKEY_POTION4, D2VK_4, 1 },
	{ HOTKEY_POTION4, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH1, D2VK_NUMPAD0, 1 },
	{ HOTKEY_SPEECH1, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH2, D2VK_NUMPAD1, 1 },
	{ HOTKEY_SPEECH2, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH3, D2VK_NUMPAD2, 1 },
	{ HOTKEY_SPEECH3, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH4, D2VK_NUMPAD3, 1 },
	{ HOTKEY_SPEECH4, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH5, D2VK_NUMPAD4, 1 },
	{ HOTKEY_SPEECH5, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH6, D2VK_NUMPAD5, 1 },
	{ HOTKEY_SPEECH6, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH7, D2VK_NUMPAD6, 1 },
	{ HOTKEY_SPEECH7, D2VK_INVALID, 0 },
	{ HOTKEY_RUN, D2VK_CONTROL, 1 },
	{ HOTKEY_RUN, D2VK_INVALID, 0 },
	{ HOTKEY_RUNWALK, D2VK_R, 1 },
	{ HOTKEY_RUNWALK, D2VK_MBUTTON5, 0 },
	{ HOTKEY_STANDSTILL, D2VK_SHIFT, 1 },
	{ HOTKEY_STANDSTILL, D2VK_INVALID, 0 },
	{ HOTKEY_SHOWITEMS, D2VK_MENU, 1 },
	{ HOTKEY_SHOWITEMS, D2VK_MBUTTON4, 0 },
	{ HOTKEY_CLEARSCREEN, D2VK_SPACE, 1 },
	{ HOTKEY_CLEARSCREEN, D2VK_INVALID, 0 },
	{ HOTKEY_PREVSKILL, D2VK_WHEEL_UP, 1 },
	{ HOTKEY_PREVSKILL, D2VK_INVALID, 0 },
	{ HOTKEY_NEXTSKILL, D2VK_WHEEL_DOWN, 1 },
	{ HOTKEY_NEXTSKILL, D2VK_INVALID, 0 },
	{ HOTKEY_CLEARMESSAGES, D2VK_N, 1 },
	{ HOTKEY_CLEARMESSAGES, D2VK_INVALID, 0 },
	{ HOTKEY_SCREENSHOT, D2VK_SNAPSHOT, 1 },
	{ HOTKEY_SCREENSHOT, D2VK_INVALID, 0 },
	{ HOTKEY_PORTRAITS, D2VK_Z, 1 },
	{ HOTKEY_PORTRAITS, D2VK_INVALID, 0 },
	{ HOTKEY_SWITCHWEAP, D2VK_W, 1 },
	{ HOTKEY_SWITCHWEAP, D2VK_INVALID, 0 },
	{ HOTKEY_MINIMAP, D2VK_V, 1 },
	{ HOTKEY_MINIMAP, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL8, D2VK_F8, 1 },
	{ HOTKEY_SKILL8, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL9, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL9, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL10, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL10, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL11, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL11, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL12, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL12, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL13, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL13, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL14, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL14, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL15, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL15, D2VK_INVALID, 0 },
	{ HOTKEY_SKILL16, D2VK_INVALID, 1 },
	{ HOTKEY_SKILL16, D2VK_INVALID, 0 },
	{ HOTKEY_MERCINV, D2VK_O, 1 },
	{ HOTKEY_MERCINV, D2VK_INVALID, 0 },
	{ HOTKEY_SPEECH8, D2VK_NUMPAD7, 1 },
	{ HOTKEY_SPEECH8, D2VK_INVALID, 0 },
	{ HOTKEY_ESCMENU, D2VK_ESCAPE, 1 },
	{ HOTKEY_ESCMENU, D2VK_INVALID, 0 },
};

#ifdef D2_VERSION_110F
static_assert(ARRAY_SIZE(gDefaultCommandTable) == 114, "gDefaultCommandTable has 114 values in original game.");
#endif

//1.10f: D2Client.0x6FBB2D38
//HotkeyMappingEntry sgCurrCmdMapping[ARRAY_SIZE(gDefaultCommandTable)];
D2VAR(D2CLIENT, psgCurrCmdMapping, decltype(gDefaultCommandTable), 0x6FBB2D38 - D2ClientImageBase);

//1.10f: D2Client.0x6FB78978
D2VAR(D2CLIENT, sgtKeyFunctions, KeyFunctions, 0x6FB78978 - D2ClientImageBase);

D2VAR(D2CLIENT, sgpfMouse3DownFn, FARPROC, 0x6FBB3624 - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouse3LiftFn, FARPROC, 0x6FBB3628 - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouse4DownFn, FARPROC, 0x6FBB31AC - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouse4LiftFn, FARPROC, 0x6FBB2D34 - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouse5DownFn, FARPROC, 0x6FBB2D2C - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouse5LiftFn, FARPROC, 0x6FBB2D20 - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouseWheelUpFn, FARPROC, 0x6FBB362C - D2ClientImageBase);
D2VAR(D2CLIENT, sgpfMouseWheelDownFn, FARPROC, 0x6FBB2D30 - D2ClientImageBase);


//1.10f: D2Client.0x6FAD4160
void __fastcall COMMAND_HandleMouseWheel_6FAD4160(int a1)
{
	if (a1 < 0 && *D2CLIENT_sgpfMouseWheelDownFn)
	{
		D2_ASSERT(*D2CLIENT_sgpfMouseWheelDownFn);
		(*D2CLIENT_sgpfMouseWheelDownFn)();
	}
	else if (a1 > 0 && *D2CLIENT_sgpfMouseWheelUpFn)
	{
		D2_ASSERT(*D2CLIENT_sgpfMouseWheelUpFn);
		(*D2CLIENT_sgpfMouseWheelUpFn)();
	}
}

//Helper function
//1.10f: Inlined
void UpdateMouseFunction(HotkeyMappingEntry& mappingEntry,  FARPROC* KeyDownFn, FARPROC* KeyLiftFn)
{
	switch (mappingEntry.VKCode)
	{
	case D2VK_MBUTTON:
		if (!sub_6FAAB2E0())
		{
			mappingEntry.VKCode = D2VK_INVALID;
			return;
		}
		break;
	case D2VK_MBUTTON4:
	case D2VK_MBUTTON5:
		if (!sub_6FAAB320())
		{
			mappingEntry.VKCode = D2VK_INVALID;
			return;
		}
		break;
	case D2VK_WHEEL_UP:
	case D2VK_WHEEL_DOWN:
		if (!sub_6FAAB250() && !sub_6FAAB290() || D2CLIENT_sgtKeyFunctions[mappingEntry.eCmd].FKeyLift)
		{
			mappingEntry.VKCode = D2VK_INVALID;
			return;
		}
		break;
	default:
		return;
	}

	if(KeyDownFn) *KeyDownFn = D2CLIENT_sgtKeyFunctions[mappingEntry.eCmd].FKeyDown;
	if(KeyLiftFn) *KeyLiftFn = D2CLIENT_sgtKeyFunctions[mappingEntry.eCmd].FKeyLift;
}

//1.10f: D2Client.0x6FAD4470
void __fastcall D2Client_COMMAND_PrepareMouseMappings()
{
	*D2CLIENT_sgpfMouseWheelUpFn = nullptr;
	*D2CLIENT_sgpfMouseWheelDownFn = nullptr;
	*D2CLIENT_sgpfMouse3DownFn = nullptr;
	*D2CLIENT_sgpfMouse3LiftFn = nullptr;
	*D2CLIENT_sgpfMouse4DownFn = nullptr;
	*D2CLIENT_sgpfMouse4LiftFn = nullptr;
	*D2CLIENT_sgpfMouse5DownFn = nullptr;
	*D2CLIENT_sgpfMouse5LiftFn = nullptr;

	for (HotkeyMappingEntry& entry : *D2CLIENT_psgCurrCmdMapping)
	{
		if (entry.VKCode > D2VK_HELP && entry.eCmd < HOTKEY_MAX_COUNT)
		{
			switch (entry.VKCode)
			{
			case D2VK_MBUTTON:
				UpdateMouseFunction(entry, D2CLIENT_sgpfMouse3DownFn, D2CLIENT_sgpfMouse3LiftFn);
				break;
			case D2VK_MBUTTON4:
				UpdateMouseFunction(entry, D2CLIENT_sgpfMouse4DownFn, D2CLIENT_sgpfMouse4LiftFn);
				break;
			case D2VK_MBUTTON5:
				UpdateMouseFunction(entry, D2CLIENT_sgpfMouse5DownFn, D2CLIENT_sgpfMouse5LiftFn);
				break;
			case D2VK_WHEEL_UP:
				UpdateMouseFunction(entry, D2CLIENT_sgpfMouseWheelUpFn, nullptr);
				break;
			case D2VK_WHEEL_DOWN:
				UpdateMouseFunction(entry, D2CLIENT_sgpfMouseWheelDownFn, nullptr);
				break;
			}
		}
	}
}

//1.10f: D2Client.0x6FAD4B60
D2VirtualKeys __fastcall COMMAND_FindVirtualKeyCode_6FAD4B60(int nHotkeyIdx, BOOL bPrimaryHotkey)
{
	for (const HotkeyMappingEntry& entry : *D2CLIENT_psgCurrCmdMapping)
	{
		if (entry.eCmd == nHotkeyIdx && entry.bPrimaryHotkey == bPrimaryHotkey)
		{
			return entry.VKCode;
		}
	}
	return D2VK_INVALID;
}

//1.10f: D2Client.0x6FAD4F00
BOOL __fastcall COMMAND_HasKeyMapping(int nHotkeyIdx, BOOL bPrimaryHotkey)
{
	return COMMAND_FindVirtualKeyCode_6FAD4B60(nHotkeyIdx, bPrimaryHotkey) != D2VK_INVALID;
}

//Helper function
bool COMMAND_KeyHasBeenPressed(int nHotkey, bool bPrimaryHotkey)
{
	if (COMMAND_HasKeyMapping(nHotkey, bPrimaryHotkey))
	{
		const SHORT keyCode = COMMAND_FindVirtualKeyCode_6FAD4B60(nHotkey, bPrimaryHotkey);
		return keyCode != D2VK_INVALID && (GetAsyncKeyState(keyCode) & 0x8000);
	}
	return false;
}

#endif //D2_VERSION_110F