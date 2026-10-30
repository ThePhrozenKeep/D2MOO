#pragma once


#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <doctest.h>

#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>

#include <DynamicArray.h>
#include <EqualityMacros.h>

#include <D2Gfx.h>


// A call of a function of D2GraphicsInterfaceStrc, all arguments are stored as 32 bits values
struct D2GfxInterfaceCallStrc
{
	uint32_t nFunction;						// Index of the function pointer in D2GraphicsInterfaceStrc
	uint32_t nArgCount;
	uint32_t nArgs[9];
};

BEGIN_VISIT(D2GfxInterfaceCallStrc)
	FIELD(nFunction)
	FIELD(nArgCount)
	ARRAY(nArgs)
END_VISIT()

DYNAMIC_ARRAY_TYPE(D2GfxInterfaceCallStrc)


// Replaces the graphics interface of the render dlls (D2DDraw.dll, D2Direct3D.dll...).
// Every call is recorded in `calls`, and functions returning a value return `return_value`.
struct D2GraphicsInterfaceMock
{
	static inline std::vector<D2GfxInterfaceCallStrc> calls;
	static inline uint32_t return_value = 0;
	// Strings formatted by pfOutputString, the va_list itself is not recorded since it points to the stack
	static inline std::vector<std::string> output_strings;

	static std::vector<D2GfxInterfaceCallStrc> take_calls()
	{
		return std::exchange(calls, {});
	}

	static std::vector<std::string> take_output_strings()
	{
		return std::exchange(output_strings, {});
	}

	template<typename T>
	static uint32_t to_argument(T value)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(value));
		}
		else
		{
			return static_cast<uint32_t>(value);
		}
	}

	template<typename T>
	static T from_return_value(uint32_t value)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return reinterpret_cast<T>(static_cast<uintptr_t>(value));
		}
		else
		{
			return static_cast<T>(value);
		}
	}

	template<typename... Args>
	static void record(uint32_t nFunction, Args... args)
	{
		static_assert(sizeof...(Args) <= sizeof(D2GfxInterfaceCallStrc::nArgs) / sizeof(uint32_t), "Too many arguments to record");

		D2GfxInterfaceCallStrc call{};
		call.nFunction = nFunction;
		call.nArgCount = sizeof...(Args);

		[[maybe_unused]] uint32_t nArg = 0;
		((call.nArgs[nArg++] = to_argument(args)), ...);

		calls.push_back(call);
	}
};


template<size_t Index, typename FunctionPointer>
struct D2GraphicsInterfaceStub;

template<size_t Index, typename Return, typename... Args>
struct D2GraphicsInterfaceStub<Index, Return(__fastcall*)(Args...)>
{
	static Return __fastcall Call(Args... args)
	{
		D2GraphicsInterfaceMock::record(Index, args...);

		if constexpr (std::is_void_v<Return>)
		{
			return;
		}
		else
		{
			return D2GraphicsInterfaceMock::from_return_value<Return>(D2GraphicsInterfaceMock::return_value);
		}
	}
};

#define D2GFX_INTERFACE_FUNCTION_INDEX(NAME) (offsetof(D2GraphicsInterfaceStrc, NAME) / sizeof(void*))

#define D2GFX_MOCK_INTERFACE_FUNCTION(INTERFACE, NAME)																					\
	(INTERFACE).NAME = &D2GraphicsInterfaceStub<D2GFX_INTERFACE_FUNCTION_INDEX(NAME), decltype(D2GraphicsInterfaceStrc::NAME)>::Call;


inline void __fastcall D2GraphicsInterfaceMock_OutputString(int32_t nPosX, int32_t nPosY, const char* szFormat, va_list va)
{
	D2GraphicsInterfaceMock::record(D2GFX_INTERFACE_FUNCTION_INDEX(pfOutputString), nPosX, nPosY, szFormat);

	char szBuffer[1024] = {};
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, va);
	D2GraphicsInterfaceMock::output_strings.emplace_back(szBuffer);
}

inline D2GraphicsInterfaceStrc make_graphics_interface_mock()
{
	D2GraphicsInterfaceStrc graphics_interface{};

	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDetect)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfInit)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfClose)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCreateSurface)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCloseSurface)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfPauseSurface)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfStartDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfEndDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfBlit)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfChangeRes)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfGetBackBuffer)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfActivateWindow)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfSetOption)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfPlayCutscene)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfOpenSmackCutscene)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCheckCutScene)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDecodeSmacker)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfPlaySmacker)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCloseSmacker)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfGetCacheResults)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfGetDimensions)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfSetGlobalScale)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfSetGamma)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfGammaCanBeControlled)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfUpdatePerspective)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfPerspectiveTransform)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfPerspectiveTransformScale)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfPerspectiveClearScale)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfSetPalette)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfSetPaletteTables)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfSetAmbientColor)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfFloorTileDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelFlatSpriteDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelDrawColor)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelDrawEx)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelDrawShadow)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelDrawHilight)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfCelDrawClipped)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfTileDrawLit)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfTileDrawTrans)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfShadowTileDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfUtilDiamond)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfUtilRect)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfUtilFilledRect)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfUtilPoint)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDrawBox)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDrawBoxAlpha)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDrawLine)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfClearScreen)
	graphics_interface.pfOutputString = D2GraphicsInterfaceMock_OutputString;
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDebugDraw)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfDebugFillBackBuffer)
	D2GFX_MOCK_INTERFACE_FUNCTION(graphics_interface, pfClearCaches)

	return graphics_interface;
}


// Compares the calls made to the graphics interface by both implementations
inline void check_interface_calls_eq(std::vector<D2GfxInterfaceCallStrc>& moo_calls, std::vector<D2GfxInterfaceCallStrc>& original_calls, const char* context_title)
{
	CHECK_MESSAGE(moo_calls.size() == original_calls.size(), context_title, " - Different number of calls to the graphics interface");
	if (moo_calls.size() == original_calls.size() && !moo_calls.empty())
	{
		DynamicArray<D2GfxInterfaceCallStrc> moo_array{ moo_calls.data(), static_cast<int>(moo_calls.size()) };
		DynamicArray<D2GfxInterfaceCallStrc> original_array{ original_calls.data(), static_cast<int>(original_calls.size()) };
		MOO_CHECK_EQ(moo_array, original_array, context_title);
	}
}
