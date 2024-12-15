#include <Core/WINMAIN.h>
#include <Core/lng.h> 
#include <D2CommonDefinitions.h>
#include <Fog.h>
#include <Storm.h>
#include <D2Client.h>
#include <D2WinMain.h>
#include <D2WinArchive.h>
#include <GAME/Game.h>
#include <CGAME/Game.h>
#include <CGAME/Record.h>
#include <Core/WNDPROC.h>
#include <D2StrTable.h>
#include <D2DataTbls.h>
#include <D2Log.h>

#ifdef D2_VERSION_110F

D2PTR(D2CLIENT, pgpConfig_6FB9A948, D2ConfigStrc*, 0x6FB9A948 - D2ClientImageBase); //1.13c:0x6FBCB980
D2ConfigStrc* CONFIG_GetConfig() { return *D2CLIENT_pgpConfig_6FB9A948; }

//1.10f: D2Client.0x6FBA7C08
BOOL sgbGameInitialized = FALSE;
D2CLIENTDWORDSTUB(6FB74538);
D2CLIENTDWORDSTUB(6FB9A950);

D2VAR(D2CLIENT, pgnAppMode, D2C_AppMode, 0x6FB744D8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgszPlrName, char, 0x6FBA7810 - D2ClientImageBase); //16 bytes
D2VAR(D2CLIENT, dword_6FB9A954, HACCEL, 0x6FB9A954 - D2ClientImageBase);

D2VAR(D2CLIENT, pgtGameDataTbl_6FBA7828, struct D2GameDataTableStrc, 0x6FBA7828 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtGameList_6FBA7980, void*, 0x6FBA7980 - D2ClientImageBase);

using AppModeEntryPoint = D2C_AppMode(__fastcall*)();

//1.00 : D2Client.0x100F53C0
//1.10f: D2Client.0x6FB744EC
AppModeEntryPoint afnAppModeEntryPoints[]
{
	nullptr,
	nullptr,
	APPMODE_Game,
	APPMODE_Startup,
	APPMODE_CloseWindow,
	APPMODE_JoinGame,
	APPMODE_LeaveGame,
};

//1.00 : D2Client.0x1011ED3C
//1.10f: D2Client.0x6FB9A934
D2C_AppMode sgaAppModeStack[APPMODE_STACK_SIZE];
//1.00 : D2Client.0x1011ED64
//1.10f: D2Client.0x6FB9A95C
int sgnLastAppmode = 0;

//1.00 : D2Client.0x10006A20
//1.10f: D2Client.0x6FAA1D00
void __fastcall APPMODE_Enqueue(D2C_AppMode nAppMode)
{
	D2_ASSERT(sgnLastAppmode < APPMODE_STACK_SIZE);
	sgaAppModeStack[sgnLastAppmode] = nAppMode;
	sgnLastAppmode++;
}

//1.10f: Inlined
D2C_AppMode __fastcall APPMODE_Dequeue()
{
	D2_ASSERT(sgnLastAppmode);
	const D2C_AppMode nNextAppMode = sgaAppModeStack[0];
	for (int i = 1; i < APPMODE_STACK_SIZE; i++)
	{
		sgaAppModeStack[i - 1] = sgaAppModeStack[i];
	}
	sgnLastAppmode--;
	return nNextAppMode;
}



//1.10f:0x6FB9A958 
BOOL gbHasShownHelp = FALSE;
//1.10f:Inlined
void __stdcall DisplayHelp()
{
	CHAR szBuffer[16] = {};
	DWORD pcbBuffer;
	if (!gbHasShownHelp)
	{
		gbHasShownHelp = TRUE;
		pcbBuffer = ARRAY_SIZE(szBuffer) - 1;
		GetUserNameA(szBuffer, &pcbBuffer);
		if (0 == SStrCmp(szBuffer, "rseis", STORM_MAX_STR)) // Richard Seis was getting tired of the popup
		{
			return;
		}
		MessageBoxA(
			NULL,
			"Available command line options:\n"
			"  -set[default command line]\t\tSet new default command line string\n"
			"  -lng[lngfile]\t\t\tUse \"lngfile\" for language resources\n"
			"  -s[name]\t\t\t\tAttach to server \"name\"\n"
			"  -mode[mode]\t\t\tStart in [mode], where mode is\n"
			"  -c\t\t\t\tPrecache\n"
			"  \t\t\t\tgame\n"
			"  \t\t\t\tTyler\n"
			"  \t\t\t\tPath\n"
			"  \t\t\t\tComposite\n",
			"Diablo II",
			0);

	}
}
//1.10f: D2Client.0x6FAA2340 D2Client.0x6FAA22D0 
BOOL __stdcall SCmd_DisplayHelp(int _unused)
{
	D2_MAYBE_UNUSED(_unused);
	DisplayHelp();
	return FALSE;
}

//1.00 : D2Client.0x10006AE0
//1.10f: D2Client.0x6FAA1DA0
BOOL __stdcall CmdArg_DisplayHelp(int _unused, int _unused2)
{
	D2_MAYBE_UNUSED(_unused);
	D2_MAYBE_UNUSED(_unused2);
	DisplayHelp();
	return FALSE;
}

//1.00 : D2Client.0x10006C20
//1.10f: D2Client.0x6FAA1E80
int __stdcall CmdArg_Set(int a1, int _unused)
{
	D2_MAYBE_UNUSED(_unused);
	return SRegSaveString("Diablo II", "Default Command Line", 0, *(const char**)(a1 + 24));
}

//1.10f: D2Client.0x6FB74508
const char* gszModeNames[] = {
	nullptr,
	nullptr,
	"Game", // This starts directly in the game loop instead of going through 
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	// The following modes have been removed
	"Tyler",
	"Path",
	"Test",
	"Composite",
};
#ifdef D2_VERSION_110F
static_assert(ARRAY_SIZE(gszModeNames) == 11, "gszModeNames has 11 values in original game.");
#endif
//1.00 : D2Client.0x10006C50
//1.10f: D2Client.0x6FAA1EA0
BOOL __stdcall CmdArg_Mode(int a1, int _unused)
{
	D2_MAYBE_UNUSED(_unused);
	for (int i = 0; i < ARRAY_SIZE(gszModeNames); i++)
	{
		if (gszModeNames[i] && 0 == _strcmpi(gszModeNames[i], *(const char**)(a1 + 24)))
		{
			*D2CLIENT_pgnAppMode = D2C_AppMode(i);
			return TRUE;
		}
	}
	DisplayHelp();
	return FALSE;
}

//1.00 : D2Client.0x10006B70
//1.10f: D2Client.0x6FAA1E10
// Allocates and touches 0x6000 contiguous pages (4kB) by setting first byte to 0
// Presumably this is used to reduce page faults ?
signed int __stdcall CmdArg_PreCache(int _unused, int _unused2)
{
	D2_MAYBE_UNUSED(_unused);
	D2_MAYBE_UNUSED(_unused2);
	const size_t nPageSize = 0x1000;
    char* pages = (char*)D2_ALLOC(0x6000 * nPageSize);
	for (int i = 0; i < 0x6000; i++)
	{
		pages[i * nPageSize] = 0;
	}
	D2_FREE(pages);
    return TRUE;
}

//1.00 : D2Client.0x10006BE0
//1.10f: D2Client.0x6FAA1E60
signed int __stdcall CmdArg_Record(int _unused, int _unused2)
{
	D2_MAYBE_UNUSED(_unused);
	D2_MAYBE_UNUSED(_unused2);
	RECORD_ToggleRecording();
	return TRUE;
}

//1.00 : D2Client.0x10006C00
//1.10f: D2Client.0x6FAA1E70
signed int __stdcall CmdArg_Play(int _unused, int _unused2)
{
	D2_MAYBE_UNUSED(_unused);
	D2_MAYBE_UNUSED(_unused2);
	RECORD_Play();
	return TRUE;
}

//1.00 : D2Client.0x100EE028
//1.10f: D2Client.0x6FB6E0D0
SCmdArg aCommandLineOptions[] = {
	{0			, 0xFFFFFFFF, "?", CmdArg_DisplayHelp},
	{0			, 0xFFFFFFFF, "h", CmdArg_DisplayHelp},
	{0			, 0xFFFFFFFF, "help", CmdArg_DisplayHelp},
	{0x20000	, 0xFFFFFFFF, "set", CmdArg_Set},
	{0x20000	, 0xFFFFFFFF, "mode",CmdArg_Mode},
	{0			, 0xFFFFFFFF, "c", CmdArg_PreCache},
	{0			, 0xFFFFFFFF, "r", CmdArg_Record},
	{0			, 0xFFFFFFFF, "p", CmdArg_Play},
};

//1.10f: D2Client.0x6FB6E0C8
BOOL(__fastcall* gafnCommandsToRegister[])() =
{
	LNG_RegisterCmdLineArguments
};


//1.10f: Inlined in D2Client.0x6FAADD00 APPMODE_JoinGame
void CLIENT_InitGame()
{
	GAME_InitGameDataTable(D2CLIENT_pgtGameDataTbl_6FBA7828, D2CLIENT_pgtGameList_6FBA7980);
	sgbGameInitialized = 1;
	D2Game_Initialize();
}

//1.10f: D2Client.0x6FAADC70
BOOL __fastcall CLIENT_FreeGame()
{
	if (!sgbGameInitialized)
		return FALSE;
	sgbGameInitialized = FALSE;
	return D2Game_Shutdown();
}

//1.10f: D2Client.0x6FB9A94C
static HMODULE ghModule;
//1.10f: D2Client.0x6FB9A830 
static char gszModuleFilename[280];

//1.10f: D2Client.0x6FAA2050
int __stdcall D2ClientEntrypoint(HMODULE hModule, D2ConfigStrc* pConfig)
{
	*D2CLIENT_pgpConfig_6FB9A948 = pConfig;
	FOG_FlushMemoryPoolSystem(nullptr);
	FOG_SetMemoryPoolSystemMode(nullptr, TRUE);
	
	ghModule = hModule;
	GetModuleFileNameA(hModule, gszModuleFilename, 260u);

	SCmdRegisterArgList(aCommandLineOptions, ARRAY_SIZE(aCommandLineOptions));
	for (auto* commandRegisterFunction : gafnCommandsToRegister)
	{
		if (!commandRegisterFunction())
			return 0;
	}
	if (!ApplyConfiguration(pConfig))
		return 0;
	
	char tDefaultCommandLineString[D2_MAX_PATH];
	if (SRegLoadString("Diablo II", "Default Command Line", 0, tDefaultCommandLineString, ARRAY_SIZE(tDefaultCommandLineString)))
		SCmdProcess(tDefaultCommandLineString, 0, &SCmd_DisplayHelp, &SCmd_DisplayHelp);

	DWORD nPlrNameLength = 16 - 1;
	if (!D2CLIENT_pgszPlrName[0] && !GetUserNameA(D2CLIENT_pgszPlrName, &nPlrNameLength))
		wsprintfA(D2CLIENT_pgszPlrName, "%s", "noname"); // Why not a simple strncpy ? :shrug:
	
	if (!LNG_Initialize())
		return FALSE;
	D2Win_10015();
	D2Win_10014_SetWndProc(WNDPROC_WindowProc);
	*D2CLIENT_dword_6FB9A954 = LoadAcceleratorsA(ghModule, MAKEINTRESOURCEA(101));
	BOOL bIsExpansion = FOG_IsExpansion();
	if (!STRTABLE_LoadStringTables(D2Win_GetArchive(), nullptr, bIsExpansion))
		return 0;
	char locale[4];
	STRTABLE_GetLanguageString(locale, 0);
	FOG_Trace("%s is locale\n", locale);

	DATATBLS_LoadAllTxts(D2Win_GetArchive(), 0, pConfig->dwGameType == GAMETYPE_OBNET_HOST);
	*D2CLIENT_pdword_6FB74538 = 1;
	*D2CLIENT_pgnAppMode = APPMODE_STARTUP;
	do
	{
		*D2CLIENT_pgnAppMode = afnAppModeEntryPoints[*D2CLIENT_pgnAppMode]();
		D2_ASSERT(*D2CLIENT_pgnAppMode < APPMODE_LAST);
		D2_ASSERT(*D2CLIENT_pgnAppMode > APPMODE_CONTINUE);
	} while (*D2CLIENT_pgnAppMode != APPMODE_SHOULDQUIT);
	D2Win_10016();
	DATATBLS_UnloadAllBins();
	STRTABLE_FreeStringTables();
	LNG_Shutdown();
	LOG_11105();
	CLIENT_FreeGame();
	if (*D2CLIENT_pdword_6FB74538)
	{
		*D2CLIENT_pdword_6FB9A950 = 0;
		return 0;
	}
	else
	{
		switch (pConfig->dwGameType)
		{
		case GAMETYPE_BNET_BETA:
		case GAMETYPE_BNET:
		case GAMETYPE_OBNET_HOST:
		case GAMETYPE_OBNET_JOIN:
			*D2CLIENT_pdword_6FB9A950 = 3;
			return 3;
		default:
			*D2CLIENT_pdword_6FB9A950 = 4;
			return 4;
		}
	}
}

#endif //D2_VERSION_110F
