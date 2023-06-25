#include <DetoursPatch.h>
#include <chrono>
#include <thread>

#include <D2Debugger.h>
#include <GAME/Game.h>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 0; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return -1; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }

__declspec(dllexport)
PatchAction __cdecl GetPatchAction(int ordinal)
{
    return PatchAction::Ignore;
}

static const int D2GameImageBase = 0x6FC30000;

decltype(&GAME_UpdateProgress) GAME_UpdateProgress_Original = nullptr;

bool IsDebuggerEnabled()
{
    static bool bEnabledFromCommandLine = strstr(GetCommandLineA(), "-debug") || (getenv("D2_DEBUGGER") && *getenv("D2_DEBUGGER") == '1');
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


static ExtraPatchAction extraPatchActions[] = {
    { 0x6FC386D0 - D2GameImageBase, &GAME_UpdateProgress_WithDebugger, PatchAction::FunctionReplaceOriginalByPatch, &GAME_UpdateProgress_Original},
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

__declspec(dllexport)
constexpr int __cdecl GetExtraPatchActionsCount() { return sizeof(extraPatchActions) / sizeof(ExtraPatchAction); }

__declspec(dllexport)
ExtraPatchAction* __cdecl GetExtraPatchAction(int index)
{
    return &extraPatchActions[index];
}

}

#include <type_traits>
static_assert(std::is_same<decltype(GetBaseOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetLastOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetPatchAction)*, GetPatchActionType>::value, "Ensure calling convention doesn't change");


static_assert(std::is_same<decltype(GetExtraPatchActionsCount)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetExtraPatchAction)*, GetExtraPatchActionType>::value, "Ensure calling convention doesn't change");
