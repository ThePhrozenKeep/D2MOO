#pragma once
#include <tracy/Tracy.hpp>
#include <type_traits>

// This will NOT work with variadic functions. See https://devblogs.microsoft.com/oldnewthing/20200715-00/?p=103984 if needed.
// We may want to use his function traits someday, this is good enough for now https://devblogs.microsoft.com/oldnewthing/20200721-00/?p=103995


template<typename FunctionPatchHelper, typename Fn, Fn fn, typename... Args>
static typename std::result_of<Fn(Args...)>::type __fastcall wrapper__fastcall(Args... args) {
	ZoneScopedN(FunctionPatchHelper::Name);
    return FunctionPatchHelper::OriginalFunctionPtr(std::forward<Args>(args)...);
}
template<typename FunctionPatchHelper, typename Fn, Fn fn, typename... Args>
static typename std::result_of<Fn(Args...)>::type __stdcall wrapper__stdcall(Args... args) {
	ZoneScopedN(FunctionPatchHelper::Name);
    return FunctionPatchHelper::OriginalFunctionPtr(std::forward<Args>(args)...);
}
template<typename FunctionPatchHelper, typename Fn, Fn fn, typename... Args>
static typename std::result_of<Fn(Args...)>::type __cdecl wrapper__cdecl(Args... args) {
	ZoneScopedN(FunctionPatchHelper::Name);
    return FunctionPatchHelper::OriginalFunctionPtr(std::forward<Args>(args)...);
}

#define DEFINE_NAME_HELPER(ADDR, FUNC, CCONV) struct PatchHelper_##FUNC { static constexpr auto Name = #FUNC; static decltype(FUNC)* OriginalFunctionPtr; }; decltype(FUNC)* PatchHelper_##FUNC::OriginalFunctionPtr = nullptr;
#define GET_WRAPPER(FUNC, callconv) \
    (decltype(FUNC)*)(wrapper##callconv<PatchHelper_##FUNC, decltype(&FUNC), &FUNC>)

#define DEFINE_PROFILING_EXTRA_PATCH(DLL, ADDR, FUNC, CCONV) \
    {ADDR - DLL##ImageBase, GET_WRAPPER(FUNC,CCONV), PatchAction::FunctionReplaceOriginalByPatch, &PatchHelper_##FUNC::OriginalFunctionPtr },
