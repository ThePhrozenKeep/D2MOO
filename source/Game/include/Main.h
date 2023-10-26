#pragma once
#include <Windows.h>
#include <D2BasicTypes.h>

//TODO
#define D2_VERSION_EXPANSION true
#define D2_VERSION_MAJOR 1
#define D2_VERSION_MINOR 10
#define D2_HAS_OPENGL false

#define D2_HAS_MULTILAN defined(D2_VERSION_100) // TODO: figure out when this was removed

/*
 *	Project definitions
 */
#define GAMEAPI				__fastcall
#define KEYHOOKAPI			__stdcall
#define QUERYINTAPI			__fastcall

#define MAX_REG_KEY			1024
#define NUM_GAME_MOD		7

/*
 *	String definitions
 */
#define GAME_FILE_INI		"D2.ini"
#define GAME_OK				"DIABLO_II_OK"

#define SYS_NAME			"Diablo II"
#define SYS_LOG_PREFIX		"D2"

#define SVC_NAME			"DIABLO2SRV"
#define SVC_DISPLAYNAME		"Diablo II Server"

#define INIT_NAME			"d2server"

#define REG_KEY_HOME		"Diablo II"
#define REG_VAL_RENDER		"Render"
#define REG_PATH_HOME		"SOFTWARE\\Blizzard Entertainment\\Diablo II"
#define REG_PATH_BETA		"SOFTWARE\\Blizzard Entertainment\\Diablo II Beta"
#define REG_PATH_VIDEO		"SOFTWARE\\Blizzard Entertainment\\Diablo II\\VideoConfig"

#define HMOD_KEYHOOK		"Keyhook.dll"
#define PROC_QUERYINT		"QueryInterface"
#define PROC_KEYHOOK		"InstallKeyboardHook"
#define PROC_UNKEYHOOK		"UninstallKeyboardHook"

#define ERRMSG_TITLE		"Diablo 2"
#define ERRMSG_LOADMOD		"Cannot load %s: Error %d"
#define ERRMSG_INSTALLSVC	"Unable to install or run service.  Verify that you have administrator access and that the program is located on a local drive."

#define CMDLINE_TOKEN		"-"
#define CMDLINE_SVC			"SvcCmdLine"
#define CMDLINE_USE			"UseCmdLine"
#define CMDLINE_BNET		"-skiptobnet"
#define CMDLINE_ADD_BNET	"%s -skiptobnet"
#define CMDLINE_INSTALL		"-install"
#define CMDLINE_SZ			"CmdLine"

/*
 *	Define module types
 */
enum D2_MODULES
{
	MODULE_NONE,
	MODULE_CLIENT,
	MODULE_SERVER,
	MODULE_MULTIPLAYER,
	MODULE_LAUNCHER,
#if D2_HAS_MULTILAN
	MODULE_MULTILAN,
#endif
	MODULE_EXPAND,
	D2_MODULES_COUNT
};

/*
 *	Define graphic render modes
 */
enum
{
	DISPLAYTYPE_NONE,
	DISPLAYTYPE_GDI,
	DISPLAYTYPE_UNUSED,
	DISPLAYTYPE_RAVE,
	DISPLAYTYPE_GLIDE,
	DISPLAYTYPE_OPENGL,
	DISPLAYTYPE_DIRECT3D
};

/*
 *	Define command line types
 */
enum D2ParseCmdArgType : uint8_t
{
	CMD_BOOLEAN,
	CMD_INTEGER,
	CMD_STRING
};

/*
 *	Create structure for game configuration parameters
 */
#pragma pack(push, 1)
struct D2GameCfgStrc
{
#if D2_VERSION_EXPANSION // Not present in old versions of the game such as 1.00
	BOOL bIsExpansion;
#endif
	BYTE bWindow;
	BYTE b3DFX;
	BYTE bOpenGL;
	BYTE bRave;
	BYTE bD3D;
	BYTE bPerspective;
	BYTE bQuality;
	DWORD dwGamma;
	BYTE bVSync;
	DWORD dwFramerate;
	DWORD dwGameType;
	WORD wJoinID; // Dangerous ! We may be overwriting szGameName since code expects a DWORD
	char szGameName[24];
	char szServerIP[24];
	char szBattleNetIP[24];
	char szMCPIP[24];
	BYTE _0076[4];
	BYTE bNoPK;
	BYTE bOpenC;
	BYTE bAmazon;
	BYTE bPaladin;
	BYTE bSorceress;
	BYTE bNecromancer;
	BYTE bBarbarian;
#if D2_VERSION_EXPANSION
	BYTE bDruid;
	BYTE bAssassin;
#endif
	BYTE bInvincible;
	BYTE _0082[48];
	char szName[24];
	BYTE szRealm[256];
	BYTE _01D0[0x18];
	DWORD dwCTemp;
	BYTE bNoMonsters;
	DWORD dwMonsterClass;
	BYTE bMonsterInfo;
	DWORD dwMonsterDebug;
	BYTE bRare;
	BYTE bUnique;
	BYTE _01DA[2]; // Possibly Set/Magic
	DWORD dwAct;
	BYTE bNoPreload;
	BYTE bDirect;
	BYTE bLowEnd;
	BYTE bNoCompress;
	DWORD dwArena;
	BYTE _01E8[6]; // Related to Arena
#ifndef VERSION_100 // TODO: figure out when this was added. Probably in 1.10
	void* pAllowExpansionCallback;
	BYTE bTxt;
#endif
	BYTE bLog;
	BYTE bMsgLog;
	BYTE bSafeMode;
	BYTE bNoSave;
	DWORD dwSeed;
	BYTE bCheats;
#ifndef VERSION_100 // TODO: figure out when this was added
	BYTE bTeen;
#endif
	BYTE bNoSound;
	BYTE bQuests;
	BYTE _01F9;
	
#ifndef VERSION_100 // TODO: figure out when this was added. Probably in 1.10
	BYTE bBuild;
#endif
	DWORD dwComInt;
	BYTE _01FE[308]; // Related to ComInt
	BYTE bSkipToBNet;
	BYTE _0333[112];
};
#pragma pack(pop)

#pragma warning(push)
#pragma warning(disable:4820)

#pragma pack(push, 4)
/*
 *	Structure for command line arguments
 */
struct D2CmdArgStrc				//sizeof(0x3C)
{
	char szSection[16];			//0x00
	char szKey[16];				//0x10
	char szCommand[16];			//0x20
	D2ParseCmdArgType dwType;	//0x30
	DWORD dwIndex;				//0x34
	DWORD dwDefault;			//0x38
};
#pragma pack(pop)

#pragma warning(pop)

/*
 *	Prototypes
 */
void GAMEAPI ParseCmdLine(D2GameCfgStrc* pCfg, char *argv);
void GAMEAPI stoLower(char *s);
int GAMEAPI GetCmdIndex(const char *s);
void GAMEAPI ParseCmdValue(char *s);
int GAMEAPI GameStart(HINSTANCE hInstance, D2GameCfgStrc* pCfg, D2_MODULES nModType);
void GAMEAPI SaveCmdLine(const char *argv[]);
int GAMEAPI GameInit(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors);
VOID WINAPI D2ServerServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI D2ServerServiceHandlerProc(DWORD dwCtrlCode);
