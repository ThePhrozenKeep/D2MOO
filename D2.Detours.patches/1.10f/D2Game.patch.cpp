#include <DetoursPatch.h>
#include <ProfilingPatchHelpers.h>
#include <GAME/Game.h>
#include <D2Game.h>


//#define DISABLE_ALL_PATCHES

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'001; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'062; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

// We are using the following letters:
// /*C*/ to mean that the function has been "Checked" and works
// /*B*/ to mean wean know it is "Broken" and needs fixing.
//`     ` (spaces) to mean that it is "Unknown" wether it works or not

static PatchAction patchActions[GetOrdinalCount()] = {
	
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10001_Return0												@10001
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_InitGameDataTable											@10002
    PatchAction::Ignore,                             //   GAME_ProcessNetworkMessages										@10003
    PatchAction::Ignore,                             //   GAME_UpdateGamesProgress											@10004
    PatchAction::Ignore,                             //   GAME_UpdateClients												@10005
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_CloseAllGames												@10006
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ReceiveDatabaseCharacter										@10007
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10008														@10008
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetGlobalAct													@10009
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetInitSeed													@10010
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10011														@10011
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGamesCount												@10012
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10013														@10013
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGameInformation											@10014
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGameServerGameIds											@10015
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetPlayerUnitsCount											@10016
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetPlayerUnitsInfo											@10017
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10018														@10018
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetUnitsDescriptions											@10019
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetStatistics												@10020
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SendMessageToAllClients										@10021
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SendMessageToGameClients										@10022
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetServerCallbackFunctions									@10023
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10024_RemoveClientFromGame									@10024
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetUnitFromTypeAndGUID										@10025
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetAct														@10026
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetFreeMonsterIndex											@10027
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetRoomBySubtileCoordinates									@10028
    PatchAction::FunctionReplacePatchByOriginal,     //   D2GAME_10029_SAVE_WriteFileInterface								@10029
    PatchAction::Ignore,                             //   ; Not exported
    PatchAction::Ignore,                             //   ; Not exported
    PatchAction::Ignore,                             //   ; Not exported
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10033														@10033
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10034_Return												@10034
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_DisableSequences											@10035
    PatchAction::FunctionReplacePatchByOriginal,     //   D2GAME_10036_PLRSAVE_EnableSaveFileWriting						@10036
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_CheckFirstPickedSet										@10037
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_CheckNotIntroQuest											@10038
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10039														@10039
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10040														@10040
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10041														@10041
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10042														@10042
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10043														@10043
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10044														@10044
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_ProcessGame													@10045
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_Initialize														@10046
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_CreateNewEmptyGame											@10047
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10048_Return1												@10048
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10049_Return1												@10049
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_Shutdown														@10050
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10051														@10051
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetMemoryUsage												@10052
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10053														@10053
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10054														@10054
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_DisconnectClientByName										@10055
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10056														@10056
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10057														@10057
    PatchAction::FunctionReplacePatchByOriginal,     //   CLIENTS_GetExpansionClientCount									@10058
    PatchAction::FunctionReplacePatchByOriginal,     //   PLAYER_SetVirtualPlayerCount										@10059
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10060_Return												@10060
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10061_Return												@10061
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ReturnArgument												@10062
};

#define ITERATE_WRAPPERS(x)                                     \
/*BEGIN LIST*/                                                  \
    x(0x6FC386D0, GAME_UpdateProgress, __fastcall)              \
    x(0x6FC38610, GAME_UpdateEnvironment, __fastcall)           \
    x(0x6FC392A0, GAME_UpdateClients, __stdcall)          \
//    x(0x6FC38E20, GAME_UpdateGamesProgress, __stdcall)          \
    //x(0x6FC38530, GAME_ProcessNetworkMessages, __fastcall)      \
/*END LIST*/


ITERATE_WRAPPERS(DEFINE_NAME_HELPER)

#define DEFINE_PROFILING_EXTRA_PATCH_D2GAME(ADDR, FUNC, CCONV) DEFINE_PROFILING_EXTRA_PATCH(D2Game, ADDR, FUNC, CCONV)
static ExtraPatchAction extraPatchActions[] = {
    ITERATE_WRAPPERS(DEFINE_PROFILING_EXTRA_PATCH_D2GAME)
    //{ 0x6FC386D0 - D2GameImageBase, &GAME_UpdateProgress, PatchAction::FunctionReplaceOriginalByPatch, &GAME_UpdateProgress_OriginalFunctionPtr },
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

extern "C" {

__declspec(dllexport)
PatchAction __cdecl GetPatchAction(int ordinal)
{
#ifdef DISABLE_ALL_PATCHES
    return PatchAction::Ignore;
#else
    if (ordinal < GetBaseOrdinal() || ordinal > GetLastOrdinal())
        return PatchAction::FunctionReplacePatchByOriginal;
    
    static_assert(GetOrdinalCount() == (sizeof(patchActions) / sizeof(*patchActions)), "Make sure we have the right number of ordinal patch entries");
    return ::patchActions[ordinal - GetBaseOrdinal()];
#endif
}

__declspec(dllexport)
constexpr int __cdecl GetExtraPatchActionsCount() { 

#ifdef DISABLE_ALL_PATCHES
    return 0;
#else
    return sizeof(extraPatchActions) / sizeof(ExtraPatchAction); 
#endif
}

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
