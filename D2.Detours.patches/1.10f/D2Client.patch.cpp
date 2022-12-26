#include <DetoursPatch.h>
#include <ProfilingPatchHelpers.h>
#include <D2Client.h>
#include <Game/SCmd.h>
#include <Game/Select.h>
#include <Unit/Player.h>
#include <UI/Chat.h>
#include <UI/UI.h>

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'001; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'004; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

// We are using the following letters:
// /*C*/ to mean that the function has been "Checked" and works
// /*B*/ to mean wean know it is "Broken" and needs fixing.
//`     ` (spaces) to mean that it is "Unknown" wether it works or not

static PatchAction patchActions[GetOrdinalCount()] = {
	
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Client_10001	@10001
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Client_10002	@10002
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Client_10003	@10003
    PatchAction::FunctionReplacePatchByOriginal,     //   QueryInterface	@10004
};

signed int __stdcall MainLoop_6FAA9AF0(int a2);

//D2Client.0x6FAA9640
BOOL __fastcall D2CLIENT_DrawGameScene(DWORD);
//D2Client.0x6FADC180
BOOL __fastcall D2CLIENT_DrawPleaseInsertGameDisc(DWORD);
//D2Client.0x6FADC970
BOOL __fastcall D2CLIENT_DrawPleaseInsertCinematicsDisc(DWORD);
//D2Client.0x6FB54C60
void __cdecl SOUND_UpdateEnvironmentSFX();

#define ITERATE_WRAPPERS(x)                                                     \
/*BEGIN LIST*/                                                                  \
    x(0x6FB49920, PLAYER_unk_6FB49920, __fastcall)                              \
    x(0x6FB23800, CLIENT_UpdateUIs_6FB23800, __fastcall)                        \
    x(0x6FAA9640, D2CLIENT_DrawGameScene, __fastcall)                           \
    x(0x6FADC180, D2CLIENT_DrawPleaseInsertGameDisc, __fastcall)                \
    x(0x6FADC970, D2CLIENT_DrawPleaseInsertCinematicsDisc, __fastcall)          \
    x(0x6FB54C60, SOUND_UpdateEnvironmentSFX, __cdecl)                          \
// Following functions are not called only when a frame is active, but in the busy loop.
// This is a bit heavy so don't instrument those.
//    x(0x6FAB5D60, CLIENT_UpdateSelectedUnit, __cdecl)          \
//    x(0x6FAD2FB0, CHAT_Update_6FAD2FB0, __cdecl)                                  \
//    x(0x6FAB1CB0, D2Client_PACKETS_SendPlayerUpdates_6FAB1CB0, __fastcall)      \
//    x(0x6FAB50B0, D2CLIENT_PACKETS_Handler_6FAB50B0, __fastcall)                \
    //x(0x6FAA9AF0, MainLoop_6FAA9AF0, __stdcall)                               \
/*END LIST*/


ITERATE_WRAPPERS(DEFINE_NAME_HELPER)

#define DEFINE_PROFILING_EXTRA_PATCH_D2CLIENT(ADDR, FUNC, CCONV) DEFINE_PROFILING_EXTRA_PATCH(D2Client, ADDR, FUNC, CCONV)
static ExtraPatchAction extraPatchActions[] = {
    ITERATE_WRAPPERS(DEFINE_PROFILING_EXTRA_PATCH_D2CLIENT)
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

extern "C" {

__declspec(dllexport)
PatchAction __cdecl GetPatchAction(int ordinal)
{
    if (ordinal < GetBaseOrdinal() || ordinal > GetLastOrdinal())
        return PatchAction::FunctionReplacePatchByOriginal;
    
    static_assert(GetOrdinalCount() == (sizeof(patchActions) / sizeof(*patchActions)), "Make sure we have the right number of ordinal patch entries");
    return ::patchActions[ordinal - GetBaseOrdinal()];
}

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
