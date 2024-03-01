#include <DetoursPatch.h>

//#define DISABLE_ALL_PATCHES

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'001; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'001; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

// We are using the following letters:
// /*C*/ to mean that the function has been "Checked" and works
// /*B*/ to mean wean know it is "Broken" and needs fixing.
//`     ` (spaces) to mean that it is "Unknown" wether it works or not

static PatchAction patchActions[GetOrdinalCount()] = {
		PatchAction::Ignore,                             //   ; Not exported
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

static const int D2GameImageBase = 0x6FC30000;

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
