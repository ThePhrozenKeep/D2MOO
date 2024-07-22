#include <DetoursPatch.h>
#include <chrono>
#include <thread>

#include <D2Debugger.h>
#include <D2Debugger.patch.common.h>
#include <D2Game.h>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {

static ExtraPatchAction D2GameExtraPatchActions[] = {
#ifdef D2_VERSION_110F
	{ 0x6FC386D0 - D2GameImageBase, &GAME_UpdateProgress_WithDebugger, PatchAction::FunctionReplaceOriginalByPatch, &GAME_UpdateProgress_Original},
#endif
    { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

static const int D2ClientImageBase = 0x6FC30000;

static ExtraPatchAction D2ClientExtraPatchActions[] = {
	{ 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
};

ExtraPatchAction* __cdecl D2ClientGetExtraPatchAction(int index)
{
	return &D2ClientExtraPatchActions[index];
}

__declspec(dllexport)
uint32_t __cdecl DllPreLoadHook(HookContext* ctx, const wchar_t* dllName)
{
	if (wcsicmp(dllName, L"D2Game.dll") == 0)
	{
		for (auto& p : D2GameExtraPatchActions)
		{
			ctx->ApplyPatchAction(ctx, p.originalDllOffset, p.patchData, p.action, (void**)p.detouredPatchedFunctionPointerStorageAddress);
		}
	}
	else if (wcsicmp(dllName, L"D2Client.dll") == 0)
	{
		for (auto& p : D2ClientExtraPatchActions)
		{
			ctx->ApplyPatchAction(ctx, p.originalDllOffset, p.patchData, p.action, (void**)p.detouredPatchedFunctionPointerStorageAddress);
		}
	}
	else
	{
		__debugbreak(); // Insert other dlls here if you added them in the .rc.
		return {};
	}
	return 0;
}

} // extern "C"

#include <type_traits>
static_assert(std::is_same<decltype(DllPreLoadHook)*, DllPreLoadHookType>::value, "Ensure calling convention doesn't change");
