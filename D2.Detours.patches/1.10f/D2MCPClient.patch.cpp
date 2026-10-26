#include <DetoursPatch.h>

//#define DISABLE_ALL_PATCHES

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'000; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'062; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

// We are using the following letters:
// /*C*/ to mean that the function has been "Checked" and works
// /*B*/ to mean wean know it is "Broken" and needs fixing.
//`     ` (spaces) to mean that it is "Unknown" wether it works or not

static PatchAction patchActions[GetOrdinalCount()] = {
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientConnect @10000
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCloseMCP @10001
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientIsConnected @10002
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientProcessMessages @10003
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCreateGame @10004
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientJoinGame @10005
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientRequestGameList @10006
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientRequestGameInfo @10007
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCharCreate @10008
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCharLogon @10009
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg09Callback @10010
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg08Callback @10011
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetLadderDataCallbacks @10012
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg08 @10013
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg09 @10014
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetGameServerIp @10015
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetGameRequestId @10016
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetGameToken @10017
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetGameHash @10018
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetGameInfo @10019
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientClearGameInfo @10020
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCharDelete @10021
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg0B @10022
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg0C @10023
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg0D @10024
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg0F @10025
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSendMsg10 @10026
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientRequestLadderData @10027
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCharCreateReceived @10028
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCharCreateReceived @10029
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCharLogonReceived @10030
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCharLogonReceived @10031
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCharDeleteReceived @10032
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCharDeleteReceived @10033
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg0BReceived @10034
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetMsg0BReceived @10035
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg0CReceived @10036
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetMsg0CReceived @10037
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetMsg0EStrings @10038
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg0EStrings @10039
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetMsg0DReceived @10040
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg0DReceived @10041
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetMsg0FReceived @10042
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg0FReceived @10043
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetMsg10Received @10044
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMsg10Received @10045
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetLastError @10046
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetLastError @10047
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetMotdCallback @10048
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientRequestMotd @10049
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCreateQueuePosition @10050
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCreateQueuePosition @10051
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCreateQueueReceived @10052
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCreateQueueReceived @10053
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCancelGameCreate @10054
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetConnectionThread @10055
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientRequestCharList @10056
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCharacterList @10057
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCharacterList @10058
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCharRank @10059
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientSetCharUpgradeReceived @10060
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientGetCharUpgradeReceived @10061
    PatchAction::FunctionReplacePatchByOriginal,       //   D2MCPClientCharUpgrade @10062
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

static const int D2MCPClientImageBase = 0x6F9F0000;

static ExtraPatchAction extraPatchActions[] = {    
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

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
