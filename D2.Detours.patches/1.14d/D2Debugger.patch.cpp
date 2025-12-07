#include <DetoursPatch.h>
#include <chrono>
#include <thread>

#include <D2Debugger.h>
#include <D2Debugger.patch.common.h>
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

static const int GameExeImageBase = 0x00400000;


static ExtraPatchAction extraPatchActions[] = {
	{ 0x52D870 - GameExeImageBase, &GAME_UpdateProgress_WithDebugger, PatchAction::FunctionReplaceOriginalByPatch, &GAME_UpdateProgress_Original},
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
