#pragma once
#include <Windows.h>
#include <D2BasicTypes.h>

#include <D2Config.h>

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
 *	Define command line types
 */
enum D2ParseCmdArgType : uint8_t
{
	CMD_BOOLEAN,
	CMD_INTEGER,
	CMD_STRING
};


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
void GAMEAPI ParseCmdLine(D2ConfigStrc* pCfg, char *argv);
void GAMEAPI stoLower(char *s);
int GAMEAPI GetCmdIndex(const char *s);
void GAMEAPI ParseCmdValue(char *s);
int GAMEAPI GameStart(HINSTANCE hInstance, D2ConfigStrc* pCfg, D2_MODULES nModType);
void GAMEAPI SaveCmdLine(const char *argv[]);
int GAMEAPI GameInit(DWORD dwNumServicesArgs, const char* lpServiceArgVectors[]);
VOID WINAPI D2ServerServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI D2ServerServiceHandlerProc(DWORD dwCtrlCode);
