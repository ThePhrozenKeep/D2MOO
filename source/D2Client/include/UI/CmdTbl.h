#pragma once  

#pragma pack(push, 1)

enum D2HotkeyIndices
{
	HOTKEY_STATSCREEN = 0x0,
	HOTKEY_INVENTORY = 0x1,
	HOTKEY_PARTY = 0x2,
	HOTKEY_MSGLOG = 0x3,
	HOTKEY_QUESTS = 0x4,
	HOTKEY_CHAT = 0x5,
	HOTKEY_HELP = 0x6,
	HOTKEY_AUTOMAP = 0x7,
	HOTKEY_MINIMAP_POSITION = 0x8,
	HOTKEY_FADEAUTOMAP = 0x9,
	HOTKEY_PARTYONMAP = 0xA,
	HOTKEY_NAMESONMAP = 0xB,
	HOTKEY_SKILLTREE = 0xC,
	HOTKEY_SKILLSELECT = 0xD,
	HOTKEY_SKILL1 = 0xE,
	HOTKEY_SKILL2 = 0xF,
	HOTKEY_SKILL3 = 0x10,
	HOTKEY_SKILL4 = 0x11,
	HOTKEY_SKILL5 = 0x12,
	HOTKEY_SKILL6 = 0x13,
	HOTKEY_SKILL7 = 0x14,
	HOTKEY_SKILL8 = 0x15,
	HOTKEY_SHOWBELT = 0x16,
	HOTKEY_POTION1 = 0x17,
	HOTKEY_POTION2 = 0x18,
	HOTKEY_POTION3 = 0x19,
	HOTKEY_POTION4 = 0x1A,
	HOTKEY_SPEECH1 = 0x1B,
	HOTKEY_SPEECH2 = 0x1C,
	HOTKEY_SPEECH3 = 0x1D,
	HOTKEY_SPEECH4 = 0x1E,
	HOTKEY_SPEECH5 = 0x1F,
	HOTKEY_SPEECH6 = 0x20,
	HOTKEY_SPEECH7 = 0x21,
	HOTKEY_RUN = 0x22,
	HOTKEY_RUNWALK = 0x23,
	HOTKEY_STANDSTILL = 0x24,
	HOTKEY_SHOWITEMS = 0x25,
	HOTKEY_CLEARSCREEN = 0x26,
	HOTKEY_PREVSKILL = 0x27,
	HOTKEY_NEXTSKILL = 0x28,
	HOTKEY_CLEARMESSAGES = 0x29,
	HOTKEY_SCREENSHOT = 0x2A,
	HOTKEY_PORTRAITS = 0x2B,
	HOTKEY_SWITCHWEAP = 0x2C,
	HOTKEY_MINIMAP = 0x2D,
	HOTKEY_SKILL9 = 0x2E,
	HOTKEY_SKILL10 = 0x2F,
	HOTKEY_SKILL11 = 0x30,
	HOTKEY_SKILL12 = 0x31,
	HOTKEY_SKILL13 = 0x32,
	HOTKEY_SKILL14 = 0x33,
	HOTKEY_SKILL15 = 0x34,
	HOTKEY_SKILL16 = 0x35,
	HOTKEY_MERCINV = 0x36,
	HOTKEY_SPEECH8 = 0x37,
	HOTKEY_ESCMENU = 0x38,
	HOTKEY_MAX_COUNT = 0x39,
};

// This is globally Winuser.h values + custom ones for mouse but not all are authorized ?
enum D2VirtualKeys : int16_t
{
	D2VK_BACK = 0x8,
	D2VK_TAB = 0x9,
	D2VK_CLEAR = 0xC,
	D2VK_RETURN = 0xD,
	D2VK_SHIFT = 0x10,
	D2VK_CONTROL = 0x11,
	D2VK_MENU = 0x12,
	D2VK_PAUSE = 0x13,
	D2VK_CAPITAL = 0x14,
	D2VK_KANA = 0x15,
	D2VK_HANGUL = 0x15,
	D2VK_IME_ON = 0x16,
	D2VK_JUNJA = 0x17,
	D2VK_FINAL = 0x18,
	D2VK_HANJA = 0x19,
	D2VK_KANJI = 0x19,
	D2VK_IME_OFF = 0x1A,
	D2VK_ESCAPE = 0x1B,
	D2VK_CONVERT = 0x1C,
	D2VK_NONCONVERT = 0x1D,
	D2VK_ACCEPT = 0x1E,
	D2VK_MODECHANGE = 0x1F,
	D2VK_SPACE = 0x20,
	D2VK_PRIOR = 0x21,
	D2VK_NEXT = 0x22,
	D2VK_END = 0x23,
	D2VK_HOME = 0x24,
	D2VK_LEFT = 0x25,
	D2VK_UP = 0x26,
	D2VK_RIGHT = 0x27,
	D2VK_DOWN = 0x28,
	D2VK_SELECT = 0x29,
	D2VK_PRINT = 0x2A,
	D2VK_EXECUTE = 0x2B,
	D2VK_SNAPSHOT = 0x2C,
	D2VK_INSERT = 0x2D,
	D2VK_DELETE = 0x2E,
	D2VK_HELP = 0x2F,

	D2VK_0 = 0x30,
	D2VK_1 = 0x31,
	D2VK_2 = 0x32,
	D2VK_3 = 0x33,
	D2VK_4 = 0x34,
	D2VK_5 = 0x35,
	D2VK_6 = 0x36,
	D2VK_7 = 0x37,
	D2VK_8 = 0x38,
	D2VK_9 = 0x39,
	D2VK_A = 0x41,
	D2VK_B = 0x42,
	D2VK_C = 0x43,
	D2VK_D = 0x44,
	D2VK_E = 0x45,
	D2VK_F = 0x46,
	D2VK_G = 0x47,
	D2VK_H = 0x48,
	D2VK_I = 0x49,
	D2VK_J = 0x4A,
	D2VK_K = 0x4B,
	D2VK_L = 0x4C,
	D2VK_M = 0x4D,
	D2VK_N = 0x4E,
	D2VK_O = 0x4F,
	D2VK_P = 0x50,
	D2VK_Q = 0x51,
	D2VK_R = 0x52,
	D2VK_S = 0x53,
	D2VK_T = 0x54,
	D2VK_U = 0x55,
	D2VK_V = 0x56,
	D2VK_W = 0x57,
	D2VK_X = 0x58,
	D2VK_Y = 0x59,
	D2VK_Z = 0x5A,

	D2VK_LWIN = 0x5B,
	D2VK_RWIN = 0x5C,
	D2VK_APPS = 0x5D,
	D2VK_SLEEP = 0x5F,

	D2VK_NUMPAD0 = 0x60,
	D2VK_NUMPAD1 = 0x61,
	D2VK_NUMPAD2 = 0x62,
	D2VK_NUMPAD3 = 0x63,
	D2VK_NUMPAD4 = 0x64,
	D2VK_NUMPAD5 = 0x65,
	D2VK_NUMPAD6 = 0x66,
	D2VK_NUMPAD7 = 0x67,
	D2VK_NUMPAD8 = 0x68,
	D2VK_NUMPAD9 = 0x69,
	D2VK_MULTIPLY = 0x6A,
	D2VK_ADD = 0x6B,
	D2VK_SEPARATOR = 0x6C,
	D2VK_SUBTRACT = 0x6D,
	D2VK_DECIMAL = 0x6E,
	D2VK_DIVIDE = 0x6F,

	D2VK_F1 = 0x70,
	D2VK_F2 = 0x71,
	D2VK_F3 = 0x72,
	D2VK_F4 = 0x73,
	D2VK_F5 = 0x74,
	D2VK_F6 = 0x75,
	D2VK_F7 = 0x76,
	D2VK_F8 = 0x77,
	D2VK_F9 = 0x78,
	D2VK_F10 = 0x79,
	D2VK_F11 = 0x7A,
	D2VK_F12 = 0x7B,
	D2VK_F13 = 0x7C,
	D2VK_F14 = 0x7D,
	D2VK_F15 = 0x7E,
	D2VK_F16 = 0x7F,
	D2VK_F17 = 0x80,
	D2VK_F18 = 0x81,
	D2VK_F19 = 0x82,
	D2VK_F20 = 0x83,
	D2VK_F21 = 0x84,
	D2VK_F22 = 0x85,
	D2VK_F23 = 0x86,
	D2VK_F24 = 0x87,
	/* UI navigation keys are reserved */
	D2VK_NAVIGATION_VIEW = 0x88,
	D2VK_NAVIGATION_MENU = 0x89,
	D2VK_NAVIGATION_UP = 0x8A,
	D2VK_NAVIGATION_DOWN = 0x8B,
	D2VK_NAVIGATION_LEFT = 0x8C,
	D2VK_NAVIGATION_RIGHT = 0x8D,
	D2VK_NAVIGATION_ACCEPT = 0x8E,
	D2VK_NAVIGATION_CANCEL = 0x8F,

	/* Those are most of the time not recognized by the game since it only works for GetAsyncKeyState and GetKeyState */
	D2VK_LSHIFT = 0xA0,
	D2VK_RSHIFT = 0xA1,
	D2VK_LCONTROL = 0xA2,
	D2VK_RCONTROL = 0xA3,
	D2VK_LMENU = 0xA4,
	D2VK_RMENU = 0xA5,

	D2VK_OEM_1 = 0xBA,
	D2VK_OEM_PLUS = 0xBB,
	D2VK_OEM_COMMA = 0xBC,
	D2VK_OEM_MINUS = 0xBD,
	D2VK_OEM_PERIOD = 0xBE,
	D2VK_OEM_2 = 0xBF,
	D2VK_OEM_3 = 0xC0,
	
	/* D2 Custom buttons */
	D2VK_MBUTTON = 0x100,
	D2VK_MBUTTON4 = 0x101,
	D2VK_MBUTTON5 = 0x102,
	D2VK_WHEEL_UP = 0x103,
	D2VK_WHEEL_DOWN = 0x104,
	D2VK_INVALID = -1,
};


struct HotkeyMappingEntry	// Size: 0x0A
{
	D2HotkeyIndices eCmd;	// 0x00
	D2VirtualKeys VKCode;	// 0x04
	BOOL bPrimaryHotkey;	// 0x06
}; 

struct KeyFunctions
{
	FARPROC FKeyDown;
	FARPROC FKeyLift;
	BOOL bUnk0x8;
};

#pragma pack(pop)

//1.10f:D2Client.0x6FAD4F00
BOOL __fastcall COMMAND_HasKeyMapping(int nHotkeyIdx, BOOL bPrimaryHotkey);

//Helper function
bool COMMAND_KeyHasBeenPressed(int nHotkey, bool bPrimaryHotkey);
