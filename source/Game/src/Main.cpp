#include <windows.h>
#include <stddef.h>
#include <stdio.h>

#include "Main.h"

#include <Fog.h>
#include <Storm.h>
#include <D2Gfx.h>
#include <Window.h>
#include <D2Sound.h>
#include <D2WinMain.h>
#include <D2WinArchive.h>
#include <D2MCPClient.h>

// Thanks to galaxyhaxz for providing the base to work on ! https://github.com/galaxyhaxz/d2src

static_assert(sizeof(D2CmdArgStrc) == 0x3C, "Check D2CmdArgStrc matches original size");

#define cmdidx(m)	offsetof(D2GameCfgStrc, m)
//1.10f: Game.0x
D2CmdArgStrc gaCmdArguments[] = {
	{ "VIDEO",     "WINDOW",       "w",          CMD_BOOLEAN, cmdidx(bWindow),        0 },
	{ "VIDEO",     "3DFX",         "3dfx",       CMD_BOOLEAN, cmdidx(b3DFX),          0 },
	{ "VIDEO",     "OPENGL",       "opengl",     CMD_BOOLEAN, cmdidx(bOpenGL),        0 },
	{ "VIDEO",     "D3D",          "d3d",        CMD_BOOLEAN, cmdidx(bRave),          0 },
	{ "VIDEO",     "RAVE",         "rave",       CMD_BOOLEAN, cmdidx(bD3D),           0 },
	{ "VIDEO",     "PERSPECTIVE",  "per",        CMD_BOOLEAN, cmdidx(bPerspective),   0 },
	{ "VIDEO",     "QUALITY",      "lq",         CMD_BOOLEAN, cmdidx(bQuality),       0 },
	{ "VIDEO",     "GAMMA",        "gamma",      CMD_INTEGER, cmdidx(dwGamma),        0 },
	{ "VIDEO",     "VSYNC",        "vsync",      CMD_BOOLEAN, cmdidx(bVSync),         0 },
	{ "VIDEO",     "FRAMERATE",    "fr",         CMD_INTEGER, cmdidx(dwFramerate),    0 },
	{ "NETWORK",   "SERVERIP",     "s",          CMD_STRING,  cmdidx(szServerIP),     0 },
	{ "NETWORK",   "GAMETYPE",     "gametype",   CMD_INTEGER, cmdidx(dwGameType),     0 },
	{ "NETWORK",   "ARENA",        "arena",      CMD_INTEGER, cmdidx(dwArena),        0 },
	{ "NETWORK",   "JOINID",       "joinid",     CMD_INTEGER, cmdidx(wJoinID),        0 },
	{ "NETWORK",   "GAMENAME",     "gamename",   CMD_STRING,  cmdidx(szGameName),     0 },
	{ "NETWORK",   "BATTLENETIP",  "bn",         CMD_STRING,  cmdidx(szBattleNetIP),  0 },
	{ "NETWORK",   "MCPIP",        "mcpip",      CMD_STRING,  cmdidx(szMCPIP),        0 },
	{ "CHARACTER", "AMAZON",       "ama",        CMD_BOOLEAN, cmdidx(bAmazon),        1 },
	{ "CHARACTER", "PALADIN",      "pal",        CMD_BOOLEAN, cmdidx(bPaladin),       0 },
	{ "CHARACTER", "SORCERESS",    "sor",        CMD_BOOLEAN, cmdidx(bSorceress),     0 },
	{ "CHARACTER", "NECROMANCER",  "nec",        CMD_BOOLEAN, cmdidx(bNecromancer),   0 },
	{ "CHARACTER", "BARBARIAN",    "bar",        CMD_BOOLEAN, cmdidx(bBarbarian),     0 },
#if D2_VERSION_EXPANSION && 0 // Not actually wired in original game, but we could do it!
	{ "CHARACTER", "DRUID",        "dru",        CMD_BOOLEAN, cmdidx(bDruid),         0 },
	{ "CHARACTER", "ASSASSIN",     "ass",        CMD_BOOLEAN, cmdidx(bAssassin),      0 },
#endif
	{ "CHARACTER", "INVINCIBLE",   "i",          CMD_BOOLEAN, cmdidx(bInvincible),    0 },
	{ "CHARACTER", "NAME",         "name",       CMD_STRING,  cmdidx(szName),         0 },
#ifndef D2_VERSION_100 // TODO: figure out when this was added
	{ "CHARACTER", "REALM",        "realm",      CMD_STRING,  cmdidx(szRealm),        0 },
#endif
	{ "CHARACTER", "CTEMP",        "ctemp",      CMD_INTEGER, cmdidx(dwCTemp),        0 },
	{ "MONSTER",   "NOMONSTERS",   "nm",         CMD_BOOLEAN, cmdidx(bNoMonsters),    0 },
	{ "MONSTER",   "MONSTERCLASS", "m",          CMD_INTEGER, cmdidx(dwMonsterClass), 0 },
	{ "MONSTER",   "MONSTERINFO",  "minfo",      CMD_BOOLEAN, cmdidx(bMonsterInfo),   0 },
	{ "MONSTER",   "MONSTERDEBUG", "md",         CMD_INTEGER, cmdidx(dwMonsterDebug), 0 },
	{ "ITEM",      "RARE",         "rare",       CMD_BOOLEAN, cmdidx(bRare),          0 },
	{ "ITEM",      "UNIQUE",       "unique",     CMD_BOOLEAN, cmdidx(bUnique),        0 },
	{ "INTERFACE", "ACT",          "act",        CMD_INTEGER, cmdidx(dwAct),          1 },
	{ "DEBUG",     "LOG",          "log",        CMD_BOOLEAN, cmdidx(bLog),           0 },
	{ "DEBUG",     "MSGLOG",       "msglog",     CMD_BOOLEAN, cmdidx(bMsgLog),        0 },
	{ "DEBUG",     "SAFEMODE",     "safe",       CMD_BOOLEAN, cmdidx(bSafeMode),      0 },
	{ "DEBUG",     "NOSAVE",       "nosave",     CMD_BOOLEAN, cmdidx(bNoSave),        0 },
	{ "DEBUG",     "SEED",         "seed",       CMD_INTEGER, cmdidx(dwSeed),         0 },
	{ "NETWORK",   "NOPK",         "nopk",       CMD_BOOLEAN, cmdidx(bNoPK),          0 },
	{ "DEBUG",     "CHEATS",       "cheats",     CMD_BOOLEAN, cmdidx(bCheats),        0 },
#ifndef D2_VERSION_100 // TODO: figure out when this was added
	{ "DEBUG",     "TEEN",         "teen",       CMD_BOOLEAN, cmdidx(bTeen),          0 },
#endif
	{ "DEBUG",     "NOSOUND",      "ns",         CMD_BOOLEAN, cmdidx(bNoSound),       0 },
	{ "FILEIO",    "NOPREDLOAD",   "npl",        CMD_BOOLEAN, cmdidx(bNoPreload),     0 },
	{ "FILEIO",    "DIRECT",       "direct",     CMD_BOOLEAN, cmdidx(bDirect),        0 },
	{ "FILEIO",    "LOWEND",       "lem",        CMD_BOOLEAN, cmdidx(bLowEnd),        0 },
	{ "DEBUG",     "QuEsTs",       "questall",   CMD_BOOLEAN, cmdidx(bQuests),        0 },
	{ "NETWORK",   "COMINT",       "comint",     CMD_INTEGER, cmdidx(dwComInt),       0 },
	{ "NETWORK",   "SKIPTOBNET",   "skiptobnet", CMD_BOOLEAN, cmdidx(bSkipToBNet),    0 },
	{ "NETWORK",   "OPENC",        "openc",      CMD_BOOLEAN, cmdidx(bOpenC),         0 },
	{ "FILEIO",    "NOCOMPRESS",   "nocompress", CMD_BOOLEAN, cmdidx(bNoCompress),    0 },
#ifndef D2_VERSION_100 // TODO: figure out when this was added. Probably in 1.10
	{ "TXT",       "TXT",          "txt",        CMD_BOOLEAN, cmdidx(bTxt),           0 },
	{ "BUILD",     "BUILD",        "build",      CMD_BOOLEAN, cmdidx(bBuild),         0 },
#endif
};
#undef cmdidx

/*
 *	Module names loaded by main game
 */
char *lpszD2Module[] = {
	"none.dll",
	"D2Client.dll",
	"D2Server.dll",
	"D2Multi.dll",
	"D2Launch.dll",
#if D2_HAS_MULTILAN
	"D2MultiLAN.dll",
#endif
	"D2EClient.dll"
};
static_assert(D2_MODULES_COUNT == ARRAY_SIZE(lpszD2Module), "Size of module types need to match enum.");

/*
 *	Internal name type for each module
 */
char *lpszModuleType[] = {
	"modstate0",
	"client",
	"server",
	"multiplayer",
	"launcher",
#if D2_HAS_MULTILAN
	"multilan",
#endif
	"expand"
};
static_assert(D2_MODULES_COUNT == ARRAY_SIZE(lpszModuleType), "Size of module types need to match enum.");

/*
 *	Structure for service status and handler
 */
 //1.10f: Game.0x40FDA8
SERVICE_STATUS gD2ServerServiceStatus = {
	SERVICE_WIN32_OWN_PROCESS,
	SERVICE_RUNNING,
	SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,
	0,
	0,
	0,
	0
};
//1.10f: Game.0x41345C
SERVICE_STATUS_HANDLE ghD2ServerServiceStatus = NULL;
//1.10f: Game.0x413464
HINSTANCE ghCurrentProcess = NULL;
//1.10f: Game.0x413468
BOOL gbD2ServerStopEvent = FALSE;
int gnCmdShow = 0;
//1.10f: Game.0x413470
char szSRegReadBuf[MAX_REG_KEY];
char lpZero = '\0';

//1.10f: Game.0x413444
D2_MODULES geModState = MODULE_NONE;

typedef D2_MODULES (QUERYINTAPI* ModuleInitPointer)(D2GameCfgStrc*);
//1.10f: Game.0x413448
void* gpCurrentModuleInterface = nullptr; // Could be local, shareware builds use a table instead ?
//1.10f: Game.0x413450
HMODULE ghModKeyhook = NULL;
//1.10f: Game.0x41344C
BOOL gbUseKeyhook = FALSE;

// Helper function
static bool GetD2IniPath(char* pPathBuffer, size_t nBufferSize)
{
	if (GetCurrentDirectoryA(nBufferSize, pPathBuffer))
	{
		size_t nCurrentWorkingDirLen = strlen(pPathBuffer);

		int nDirectoriesLevels = 0;
		for (int i = 0; i < nCurrentWorkingDirLen && i < nBufferSize; i++)
		{
			if (pPathBuffer[i] == '\\')
			{
				nDirectoriesLevels++;
			}
		}

		if (nDirectoriesLevels > 2)
			nDirectoriesLevels = 2;

		int nPathCut = 0;
		for (nPathCut = 0; nPathCut < nCurrentWorkingDirLen && nDirectoriesLevels != 0; nPathCut++)
		{
			if (pPathBuffer[nPathCut] == '\\')
				nDirectoriesLevels--;
		}

		pPathBuffer[nPathCut] = '\0';
		strcat(pPathBuffer, GAME_FILE_INI);
		return true;
	}
	return false;
}

//1.10f: Game.0x401040
void GAMEAPI ParseCmdLine(D2GameCfgStrc* pCfg, char *argv)
{
	memset(pCfg, 0, sizeof(*pCfg));

	char szPath[_MAX_DIR];
	if(GetD2IniPath(szPath, sizeof(szPath)))
	{
		for (const D2CmdArgStrc& rCmdArg : gaCmdArguments)
		{
			void* pCfgMember = (char*)pCfg + rCmdArg.dwIndex;
			switch (rCmdArg.dwType)
			{
			case CMD_INTEGER:
				*(uint32_t*)pCfgMember = GetPrivateProfileIntA(rCmdArg.szSection, rCmdArg.szKey, rCmdArg.dwDefault, szPath);
				break;
			case CMD_STRING:
				GetPrivateProfileStringA(rCmdArg.szSection, rCmdArg.szKey, &lpZero, (char*)pCfgMember, 16, szPath);
				break;
			case CMD_BOOLEAN:
				*(BYTE*)pCfgMember = 0 != GetPrivateProfileIntA(rCmdArg.szSection, rCmdArg.szKey, rCmdArg.dwDefault, szPath);
				break;
			}
		}
	}



	const size_t nArgvLength = strlen(argv);
	char szCommandNameTestBuf[24] = {};
	char szCommandValueBuf[24] = {};
	char szCommand[48] = {};
		
	for (int nCharIndex = 0; nCharIndex < nArgvLength; nCharIndex++)
	{
		if (argv[nCharIndex] != '-')
		{
			continue;
		}
		nCharIndex++;

		if (nCharIndex < nArgvLength)
		{
			int nCommandIndex = -1;
			const char* pszCommand = &argv[nCharIndex];
			const size_t nRemainingLen = strlen(pszCommand);
			int i = 0;
			while (i < nRemainingLen && pszCommand[i] != '\0' && pszCommand[i] != '-')
			{
				szCommand[i] = pszCommand[i];
				i++;
			}

			szCommand[i] = '\0';
			szCommand[i + 1] = '\0';
			strcpy(szCommandNameTestBuf, szCommand);
			stoLower(szCommandNameTestBuf);
			for (size_t i = strlen(szCommandNameTestBuf); i != 0; i--)
			{
				szCommandNameTestBuf[i] = '\0';
				nCommandIndex = GetCmdIndex(szCommandNameTestBuf);
				if (nCommandIndex != -1)
				{
					break;
				}
			}
			// Copy what's left of the command (value)
			size_t nValueIdx = 0;
			for (size_t nCommandValueOffset = strlen(szCommandNameTestBuf); 
				szCommand[nCommandValueOffset + nValueIdx];
				nValueIdx++
				)
			{
				szCommandValueBuf[nValueIdx] = szCommand[nCommandValueOffset + nValueIdx];
			}
			szCommandValueBuf[nValueIdx] = '\0';

			ParseCmdValue(szCommandValueBuf);

			if (nCommandIndex != -1)
			{
				void* pCfgMember = (char*)pCfg + gaCmdArguments[nCommandIndex].dwIndex;

				switch ((char)gaCmdArguments[nCommandIndex].dwType)
				{
				case CMD_INTEGER:
					*(int*)pCfgMember = atoi(szCommandValueBuf);
					break;
				case CMD_STRING:
					strcpy((char*)pCfgMember, szCommandValueBuf);
					break;
				case CMD_BOOLEAN:
					*(BYTE*)pCfgMember = TRUE;
					break;
				}
			}
		}
	}
}

//1.10f: Game.0x401330
void GAMEAPI stoLower(char *s)
{
	if(*s)
	{
		do
		{
			if(*s >= 'A' && *s <= 'Z')
				*s += ('a' - 'A');
		} while((s++)[1]);
	}
}

//1.10f: Game.0x401350
int GAMEAPI GetCmdIndex(const char *s)
{
	for (int nIndex = 0; nIndex < ARRAY_SIZE(gaCmdArguments); nIndex++)
	{
		if (0 == strcmp(gaCmdArguments[nIndex].szCommand, s))
		{
			return nIndex;
		}
	}
	return -1;
}

//1.10f: Game.0x4013B0
void GAMEAPI ParseCmdValue(char *s)
{
	char szBuf[24];
	strcpy(szBuf, s);

	const char separatorChars[] = { ' ', '\t', '\n', ':', 0};
	size_t nFirstChar = strspn(szBuf, separatorChars);
	size_t nChars = strcspn(&szBuf[nFirstChar], separatorChars);
	for (size_t i = 0; i < nChars; i++)
	{
		s[i] = s[i + nFirstChar];
	}
	s[nChars] = 0;
}

//1.10f: 0x4014D0 (Inlined)
D2_MODULES LoadCurrentlySelectedModule(D2GameCfgStrc* pCfg)
{
	if (geModState >= MODULE_NONE && geModState < D2_MODULES_COUNT)
	{
		if (HMODULE hModule = LoadLibraryA(lpszD2Module[geModState]))
		{
			if (FARPROC pQueryInterface = GetProcAddress(hModule, PROC_QUERYINT))
			{
				gpCurrentModuleInterface = (void*)pQueryInterface();
				return (*(ModuleInitPointer*)gpCurrentModuleInterface)(pCfg);
			}

			GetLastError();
		}
		else
		{
			char szErrMsg[100];
			sprintf(szErrMsg, ERRMSG_LOADMOD, lpszD2Module[geModState], GetLastError());
			MessageBoxA(NULL, szErrMsg, ERRMSG_TITLE, MB_ICONERROR);
		}
	}
	return MODULE_NONE;
}

//1.10f: Game.0x401570
int GAMEAPI GameStart(HINSTANCE hInstance, D2GameCfgStrc* pCfg, D2_MODULES nModType)
{
	BOOL bSoundStarted = FALSE;
	BOOL bGfxStarted = FALSE;

	FOG_MPQSetConfig(pCfg->bDirect, FALSE);
	FOG_AsyncDataInitialize(TRUE);
	FOG_10082_Noop();

	if(geModState != MODULE_SERVER)
	{
		if(!ARCHIVE_LoadArchives() || !ARCHIVE_LoadExpansionArchives(ARCHIVE_ShowInsertPlayDiscMessage, ARCHIVE_ShowInsertExpansionDiscMessage, 0, pCfg))
		{
			ARCHIVE_FreeArchives();
			return 0;
		}
#if D2_VERSION_EXPANSION
		pCfg->bIsExpansion = FOG_IsExpansion();
#endif
	}


	int dwRenderMode = DISPLAYTYPE_NONE;
	if (pCfg->b3DFX) dwRenderMode = DISPLAYTYPE_GLIDE;
	else if (pCfg->bWindow) dwRenderMode = DISPLAYTYPE_GDI;
#if D2_HAS_OPENGL
	else if (pCfg->bOpenGL) dwRenderMode = DISPLAYTYPE_OPENGL;
#endif
	else if (pCfg->bD3D) dwRenderMode = DISPLAYTYPE_DIRECT3D;
	else dwRenderMode = DISPLAYTYPE_RAVE;

	if(nModType != MODULE_SERVER)
	{
		if(!D2Win_CreateWindow(hInstance, dwRenderMode, pCfg->bWindow, !pCfg->bNoCompress))
			return 0;

		if(pCfg->bPerspective && dwRenderMode >= DISPLAYTYPE_GLIDE)
			D2GFX_SetPerspective(TRUE);

		if(!D2Win_InitializeSpriteCache(pCfg->bWindow != 0, D2GAMERES_640x480))
		{
			WINDOW_Destroy();
			return 0;
		}

		if(gbUseKeyhook)
		{
			ghModKeyhook = LoadLibraryA(HMOD_KEYHOOK);
		}

		if(ghModKeyhook)
		{
			if (FARPROC pFunc = GetProcAddress(ghModKeyhook, PROC_KEYHOOK))
			{
				((void (KEYHOOKAPI*)(HWND))pFunc)(WINDOW_GetWindow());
			}
		}

		bGfxStarted = TRUE;
	}

	if(pCfg->bQuality)
	{
		D2GFX_ToggleLowQuality();
	}

	if(pCfg->dwGamma)
	{
		D2GFX_SetGamma(pCfg->dwGamma);
	}

	if(pCfg->bVSync)
	{
		D2GFX_EnableVSync();
	}
	
	if (!pCfg->bIsExpansion)
	{
		SRegSaveValue("Diablo II", "Resolution", 0, 0);
	}

	if(!pCfg->bNoSound && nModType != MODULE_SERVER)
	{
		D2SOUND_OpenSoundSystem(pCfg->bIsExpansion);
		bSoundStarted = TRUE;
	}

	while(geModState != MODULE_NONE)
	{
		if(nModType == MODULE_SERVER)
		{
			if(bSoundStarted)
			{
				D2SOUND_CloseSoundSystem();
				bSoundStarted = FALSE;
			}

			if(bGfxStarted)
			{
				D2Win_CloseSpriteCache();
				D2GFX_Release();
				bGfxStarted = FALSE;
			}
		}

		geModState = LoadCurrentlySelectedModule(pCfg);
	}

	if (bSoundStarted)
	{
		D2SOUND_CloseSoundSystem();
	}

	if(bGfxStarted)
	{
		D2Win_CloseSpriteCache();
		D2GFX_Release();
	}

	ARCHIVE_FreeArchives();

	if(ghModKeyhook)
	{
		if (FARPROC pFunc = GetProcAddress(ghModKeyhook, PROC_UNKEYHOOK))
		{
			pFunc();
		}

		FreeLibrary(ghModKeyhook);
	}

	FOG_AsyncDataDestroy();
	D2MCPClientCloseMCP();

	if(pCfg->dwComInt)
		(*(void (**)(void))(pCfg->dwComInt + 12))();

	FOG_DestroyMemoryPoolSystem(nullptr);

	return 0;
}

//1.10f: Game.0x401870
void GAMEAPI SaveCmdLine(const char* argv[])
{
	char szSRegWriteBuf[MAX_REG_KEY * 2];

	DWORD bUseCmdLine = FALSE;

	if(*argv && strlen(*argv))
	{
		sprintf(szSRegWriteBuf, CMDLINE_ADD_BNET, *argv);
		SRegSaveString(REG_KEY_HOME, CMDLINE_SZ, SREG_DEFAULT, szSRegWriteBuf);
	}
	else
	{
		SRegLoadValue(REG_KEY_HOME, CMDLINE_USE, SREG_DEFAULT, &bUseCmdLine);

		if(bUseCmdLine)
		{
			// Note:
			SRegLoadString(REG_KEY_HOME, CMDLINE_SZ, SREG_DEFAULT, szSRegReadBuf, MAX_REG_KEY);
			*argv = szSRegReadBuf;
		}
		else
		{
			strcpy(szSRegWriteBuf, CMDLINE_BNET);
			SRegSaveString(REG_KEY_HOME, CMDLINE_SZ, SREG_DEFAULT, szSRegWriteBuf);
		}
	}

	bUseCmdLine = FALSE;
	SRegSaveValue(REG_KEY_HOME, CMDLINE_USE, SREG_DEFAULT, NULL);
	
	if(gbD2ServerStopEvent)
	{
		SRegLoadString(REG_KEY_HOME, CMDLINE_SVC, SREG_DEFAULT, szSRegReadBuf, MAX_REG_KEY);
		*argv = szSRegReadBuf;
	}
}

void MoveBetaSettingsToRelease()
{
	HKEY phkResult;
	if (ERROR_SUCCESS == RegOpenKeyA(HKEY_LOCAL_MACHINE, REG_PATH_BETA, &phkResult))
	{
		HKEY hKey;
		RegCreateKeyA(HKEY_LOCAL_MACHINE, REG_PATH_HOME, &hKey);

		for (int i = 0; ; i++)
		{
			DWORD cchValue = MAX_PATH;
			DWORD cbData = MAX_REG_KEY;
			char szValue[MAX_PATH];
			BYTE bData[MAX_REG_KEY];
			DWORD dwRegType;
			if (ERROR_SUCCESS != RegEnumValueA(phkResult, i, szValue, &cchValue, NULL, &dwRegType, bData, &cbData))
			{
				break;
			}
			if (ERROR_SUCCESS != RegSetValueExA(hKey, szValue, 0, dwRegType, bData, cbData))
			{
				break;
			}
		}

		RegCloseKey(hKey);
		RegCloseKey(phkResult);
		RegDeleteKeyA(HKEY_LOCAL_MACHINE, REG_PATH_BETA);
	}

}

//1.10f: Game.0x401970
int GAMEAPI GameInit(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors)
{
	char *lpArgvTokens;
	char **lpszModType;
	char *lpArgvCmd;
	char szRegPathVid[sizeof(REG_PATH_VIDEO)];
	D2GameCfgStrc pCfg;

	lpArgvCmd = &lpZero;

	if (dwNumServicesArgs > 1)
	{
		lpArgvCmd = lpServiceArgVectors[dwNumServicesArgs-1];
	}

	char szVersion[MAX_PATH];
	SStrPrintf(szVersion, MAX_PATH, "v%d.%02d", D2_VERSION_MAJOR, D2_VERSION_MINOR);
	FOG_SetLogPrefix(SYS_LOG_PREFIX);
	FOG_InitErrorMgr(SYS_NAME, NULL, szVersion, TRUE);

	if (HANDLE hEvent = OpenEventA(EVENT_MODIFY_STATE, TRUE, GAME_OK))
	{
		SetEvent(hEvent);
	}

	SaveCmdLine((const char**)&lpArgvCmd);

	MoveBetaSettingsToRelease();

	char szPath[MAX_PATH] = {0};
	FOG_GetInstallPath(szPath, MAX_PATH);

	if (gbD2ServerStopEvent)
	{
		SetCurrentDirectoryA(szPath);
	}

	const size_t argvLen = strlen(lpArgvCmd) + 1;
	char* lpArgvDupe = (char*)D2_ALLOC(argvLen);
	SStrCopy(lpArgvDupe, lpArgvCmd, argvLen);
	
	int nChosenModule = MODULE_NONE;

	for (char* pCurrentParam = strtok(lpArgvDupe, "-"); pCurrentParam; pCurrentParam = strtok(0, "-"))
	{
		for (int i = 0; i < D2_MODULES_COUNT && pCurrentParam; i++)
		{
			if (0 == strncmp(pCurrentParam, lpszModuleType[i], strlen(lpszModuleType[i])) && i != MODULE_CLIENT)
			{
				nChosenModule = i;
			}
		}
	}

	D2_FREE(lpArgvDupe);
	ParseCmdLine(&pCfg, lpArgvCmd);

	if(!pCfg.b3DFX && !pCfg.bWindow && !pCfg.bOpenGL && !pCfg.bD3D)
	{
		memcpy(szRegPathVid, REG_PATH_VIDEO, sizeof(REG_PATH_VIDEO));

		HKEY hKey;
		if((ERROR_SUCCESS == RegOpenKeyExA(HKEY_CURRENT_USER, szRegPathVid, 0, KEY_QUERY_VALUE, &hKey)) ||
		   (ERROR_SUCCESS == RegOpenKeyExA(HKEY_LOCAL_MACHINE, szRegPathVid, 0, KEY_QUERY_VALUE, &hKey)) )
		{
			DWORD dwRegType = REG_DWORD_LITTLE_ENDIAN;
			DWORD dwValue;
			DWORD dwCbData = sizeof(dwValue);
			if(ERROR_SUCCESS == RegQueryValueExA(hKey, REG_VAL_RENDER, NULL, &dwRegType, (LPBYTE)&dwValue, &dwCbData))
			{
				switch(dwValue)
				{
					case 1:
						pCfg.bD3D = TRUE;
						break;
					case 2:
						pCfg.bOpenGL = TRUE;
						break;
					case 3:
						pCfg.b3DFX = TRUE;
						break;
					case 4:
						pCfg.bWindow = TRUE;
						break;
					default: // Rave
						break;
				}

				RegCloseKey(hKey);
			}
		}
	}

	return GameStart(ghCurrentProcess, &pCfg, MODULE_LAUNCHER);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
	SC_HANDLE schService;

	ghCurrentProcess = hInstance;
	gnCmdShow = nCmdShow;

	const bool bHasBuildVersion = GetVersion() & 0x80000000;

	if(!bHasBuildVersion)
	{
		if(SC_HANDLE schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS))
		{
			bool bCouldOpenService = false;
			if (SC_HANDLE schService = OpenServiceA(schSCManager, SVC_NAME, SERVICE_ALL_ACCESS))
			{
				bCouldOpenService = true;
				CloseServiceHandle(schService);
			}

			CloseServiceHandle(schSCManager);

			if(bCouldOpenService)
			{
				SERVICE_TABLE_ENTRYA DispatchTable[] = {
					{ SVC_NAME, D2ServerServiceMain },
					{ NULL, NULL }
				};

				if (StartServiceCtrlDispatcherA(DispatchTable) == 0)
				{
					return 0; // Successfully started process
				}
			}
		}
	}

	if (strstr(lpCmdLine, CMDLINE_INSTALL) == nullptr)
	{
		// If we did not ask to install the service, simply run the game.
		char* argv[2];
		argv[0] = INIT_NAME;
		argv[1] = lpCmdLine;

		GameInit(ARRAY_SIZE(argv), argv);
	}
	else
	{
		// Install the service
		SC_HANDLE schSCManager = NULL;
		if (!bHasBuildVersion)
		{
			schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		}
		if (!schSCManager)
		{
			MessageBoxA(NULL, ERRMSG_INSTALLSVC, SVC_DISPLAYNAME, MB_ICONEXCLAMATION | MB_SETFOREGROUND);
			return 1;
		}
		else
		{
			SC_HANDLE schService = OpenServiceA(schSCManager, SVC_NAME, SERVICE_ALL_ACCESS);
			if (schService == NULL && SRegSaveString("Diablo II", "SvcCmdLine", 4, "-service"))
			{
				CHAR szModuleName[MAX_PATH];
				GetModuleFileNameA(GetModuleHandleA(NULL), szModuleName, sizeof(szModuleName));
				schService = CreateServiceA(
					schSCManager,
					SVC_NAME, SVC_DISPLAYNAME,
					SERVICE_ALL_ACCESS,
					SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
					SERVICE_AUTO_START,
					SERVICE_ERROR_NORMAL,
					szModuleName,
					NULL, NULL, NULL, NULL, NULL);
			}
			if (schService != NULL)
			{
				CloseServiceHandle(schService);
			}

			CloseServiceHandle(schSCManager);
		}
	}

	return 0;
}

//1.10f: Game.0x401ED0
VOID WINAPI D2ServerServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	gbD2ServerStopEvent = TRUE;
	ghD2ServerServiceStatus = RegisterServiceCtrlHandlerA(SVC_NAME, D2ServerServiceHandlerProc);
	SetServiceStatus(ghD2ServerServiceStatus, &gD2ServerServiceStatus);
	GameInit(dwArgc, lpszArgv);
	gD2ServerServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(ghD2ServerServiceStatus, &gD2ServerServiceStatus);
	gbD2ServerStopEvent = FALSE;
}

VOID WINAPI D2ServerServiceHandlerProc(DWORD dwCtrlCode)
{
	switch(dwCtrlCode)
	{
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
			gD2ServerServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(ghD2ServerServiceStatus, &gD2ServerServiceStatus);
			gbD2ServerStopEvent = TRUE;
			return;

		case SERVICE_CONTROL_INTERROGATE:
			SetServiceStatus(ghD2ServerServiceStatus, &gD2ServerServiceStatus);
			return;

		default:
			break;
	}
}
