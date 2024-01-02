#include <chrono>
#include <thread>
#include <type_traits>

#include <D2Debugger.h>
#include <GAME/Game.h>


decltype(&GAME_UpdateProgress) GAME_UpdateProgress_Original = nullptr;

bool CheckEnvVarTrue(const char* envVarName)
{
	char* envBuffer = nullptr;
	size_t bufferSize = 0;

	if (0 == _dupenv_s(&envBuffer, &bufferSize, envVarName))
	{
		const bool isSet = envBuffer && *envBuffer == '1';
		free(envBuffer);
		return isSet;
	}
	return false;
}

bool IsDebuggerEnabled()
{
	static bool bEnabledFromCommandLine = strstr(GetCommandLineA(), "-debug") || CheckEnvVarTrue("D2_DEBUGGER");
	if (bEnabledFromCommandLine)
	{
		return true;
	}
	// TODO: support toggling window based on chat command ?
	return false;
}

void __fastcall GAME_UpdateProgress_WithDebugger(D2GameStrc* pGame)
{
	if (IsDebuggerEnabled())
	{
		static bool bDebuggerAvailable = D2DebuggerInit() == 0;
		if (bDebuggerAvailable)
		{
			static bool bFreezeGame = false;
			do {
				D2DebuggerNewFrame();
				if (!bFreezeGame)
				{
					GAME_UpdateProgress_Original(pGame);
				}
				bFreezeGame = D2DebugGame(pGame);
				D2DebuggerEndFrame(bFreezeGame/*vsync ON if frozen*/);
			} while (bFreezeGame);
			return;
		}
	}
	GAME_UpdateProgress_Original(pGame);
}
static_assert(std::is_same_v<decltype(&GAME_UpdateProgress), decltype(&GAME_UpdateProgress_WithDebugger)>, "GAME_UpdateProgress_WithDebugger has a different type than previously known.");
