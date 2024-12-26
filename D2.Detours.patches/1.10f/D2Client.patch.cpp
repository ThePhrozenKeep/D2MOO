#include <DetoursPatch.h>
#include <ProfilingPatchHelpers.h>
#include <D2Client.h>
#include <CGAME/Record.h>
#include <CGAME/SCmd.h>
#include <CGAME/Select.h>
#include <Unit/Player.h>
#include <UI/chat.h>
#include <UI/UI.h>
#include <Unit/Item.h>
#include <Sound/Sound.h>
#include <Fog.h>
#include <CGAME/Game.h>
#include <Engine/Cursor.h>
#include <Core/Lng.h>
#include <Core/WINMAIN.h>
#include <Core/WNDPROC.h>

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

//D2Client.0x6FADC180
BOOL __fastcall D2CLIENT_DrawPleaseInsertGameDisc(DWORD);
//D2Client.0x6FADC970
BOOL __fastcall D2CLIENT_DrawPleaseInsertCinematicsDisc(DWORD);

char* __cdecl RECORD_unk_6FAAEB40() { 
    UNIMPLEMENTED();
    return nullptr; 
}


//D2Client.0x6FB23800
BOOL __fastcall CLIENT_UpdateUIs_6FB23800(int a1, int a2) {
    UNIMPLEMENTED();
}
//1.00 : D2Client.0x10001F73
//1.10f: D2CLient.0x6FB23860
int __fastcall D2CLIENT_CheckUIState_6FB23860(D2C_UIIds nUIId) {
    UNIMPLEMENTED();
}

//1.00 : D2Client.0x100013C0
//1.10f: D2CLient.0x6FB54C60
//1.13c: D2CLient.0x6FB0F020
void __cdecl SOUND_UpdateEnvironmentSFX() {
    UNIMPLEMENTED();
}
BOOL __fastcall D2CLIENT_UI_Handler_6FB23260(D2C_UIIds nUIId, D2C_UiStates nUIState, BOOL a3){
	UNIMPLEMENTED();
}


#define ITERATE_WRAPPERS(x)                                                     \
/*BEGIN LIST*/                                                                  \
    x(0x6FB23800, CLIENT_UpdateUIs_6FB23800, __fastcall)                        \
    x(0x6FADC180, D2CLIENT_DrawPleaseInsertGameDisc, __fastcall)                \
    x(0x6FADC970, D2CLIENT_DrawPleaseInsertCinematicsDisc, __fastcall)          \
//    x(0x6FAA9640, D2CLIENT_DrawGameScene, __fastcall)                           \
//    x(0x6FB49920, PLAYER_unk_6FB49920, __fastcall)                              \
//    x(0x6FB54C60, SOUND_UpdateEnvironmentSFX, __cdecl)                          \
// Following functions are not called only when a frame is active, but in the busy loop.
// This is a bit heavy so don't instrument those.
//    x(0x6FAB5D60, CLIENT_UpdateSelectedUnit, __cdecl)          \
//    x(0x6FAD2FB0, CHAT_Update_6FAD2FB0, __cdecl)                                  \
//    x(0x6FAB1CB0, D2Client_PACKETS_SendPlayerUpdates_6FAB1CB0, __fastcall)      \
//    x(0x6FAB50B0, D2CLIENT_PACKETS_Handler_6FAB50B0, __fastcall)                \
/*END LIST*/


ITERATE_WRAPPERS(DEFINE_NAME_HELPER)

#define DEFINE_PROFILING_EXTRA_PATCH_D2CLIENT(ADDR, FUNC, CCONV) DEFINE_PROFILING_EXTRA_PATCH(D2Client, ADDR, FUNC, CCONV)
static ExtraPatchAction extraPatchActions[] = {
#ifdef D2_VERSION_110F
	// Override patch functions first
    { 0x6FAAEB40 - D2ClientImageBase, &RECORD_unk_6FAAEB40, PatchAction::FunctionReplacePatchByOriginal},
    { 0x6FB54C60 - D2ClientImageBase, &SOUND_UpdateEnvironmentSFX, PatchAction::FunctionReplacePatchByOriginal},
    { 0x6FB23800 - D2ClientImageBase, &CLIENT_UpdateUIs_6FB23800, PatchAction::FunctionReplacePatchByOriginal},
	{ 0x6FB23860 - D2ClientImageBase, &D2CLIENT_CheckUIState_6FB23860, PatchAction::FunctionReplacePatchByOriginal},
	{ 0x6FB23260 - D2ClientImageBase, &D2CLIENT_UI_Handler_6FB23260, PatchAction::FunctionReplacePatchByOriginal},
    
	

	// Then stub for profiling
    ITERATE_WRAPPERS(DEFINE_PROFILING_EXTRA_PATCH_D2CLIENT)

	// Then the ones we rewrote
    { 0x6FAA25D0 - D2ClientImageBase, &ExecuteMessageLoop_6FAA25D0, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAA9450 - D2ClientImageBase, &D2Client_InitGame, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAA9AF0 - D2ClientImageBase, &ClientGameLoop, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAA9640 - D2ClientImageBase, &D2CLIENT_DrawGameScene, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAAB370 - D2ClientImageBase, &APPMODE_Game, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAADD00 - D2ClientImageBase, &APPMODE_JoinGame, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAAA4E0 - D2ClientImageBase, &APPMODE_Startup, PatchAction::FunctionReplaceOriginalByPatch},
	
	{ 0x6FAABEE0 - D2ClientImageBase, &ApplyConfiguration, PatchAction::FunctionReplaceOriginalByPatch},

	{ 0x6FAA2050 - D2ClientImageBase, &D2ClientEntrypoint, PatchAction::FunctionReplaceOriginalByPatch},
	
	{ 0x6FAA1210 - D2ClientImageBase, &LNG_Initialize, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FAA1300 - D2ClientImageBase, &LNG_Shutdown, PatchAction::FunctionReplaceOriginalByPatch},
	
	{ 0x6FB57330 - D2ClientImageBase, &CLIENT_OnMouseMove, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57450 - D2ClientImageBase, &CLIENT_OnNonClientMouseMove, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57480 - D2ClientImageBase, &CLIENT_UpdateCursorPosInGame, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57500 - D2ClientImageBase, &CLIENT_UpdateCursorOnLeftButtonUp, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57580 - D2ClientImageBase, &CLIENT_SetCursorBuySell, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB575B0 - D2ClientImageBase, &CLIENT_SetCursorHeldItem, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB575E0 - D2ClientImageBase, &CLIENT_GetCursorItem, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB575F0 - D2ClientImageBase, &CLIENT_LoadCursors, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB576B0 - D2ClientImageBase, &CLIENT_UnloadCursors, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB579A0 - D2ClientImageBase, &CLIENT_GetCursorUnk0x08, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57A00 - D2ClientImageBase, &CLIENT_GetCursorFrameDuration, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57BC0 - D2ClientImageBase, &CLIENT_GetMouseX, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57BD0 - D2ClientImageBase, &CLIENT_GetMouseY, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57BE0 - D2ClientImageBase, &CLIENT_SetCursorPos, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57700 - D2ClientImageBase, &CLIENT_DrawCursorMain, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57A60 - D2ClientImageBase, &CLIENT_DrawCursorBuySell, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57AC0 - D2ClientImageBase, &CLIENT_DrawCursorDefault, PatchAction::FunctionReplaceOriginalByPatch},
	{ 0x6FB57CC0 - D2ClientImageBase, &CLIENT_GetCursorState, PatchAction::FunctionReplaceOriginalByPatch},
	// Needed if patching cursor functions as it's inlined in CLIENT_SetCursorUsingItem and references globals
	{ 0x6FB2E880 - D2ClientImageBase, &UIHandleCursorItemPacket, PatchAction::FunctionReplaceOriginalByPatch},
	
	

#endif
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
