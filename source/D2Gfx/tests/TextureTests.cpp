#include <D2GfxTestDefines.h>

#ifdef TEXTURE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <filesystem>

#include <D2GfxTestDefinitions.h>
#include <TestUtilities.h>

#include <D2Gfx.h>
#include <DrawMode.h>
#include <Texture.h>

#include <Fixtures/D2Gfx/D2GfxGlobalsFixture.h>


using GfxFixture = D2GfxGlobalsFixture<NoOpFixture>;


TEST_SUITE("TextureTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Gfx.dll").string().c_str()));


	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74270 (#10071)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelFlatSpriteDraw, dll_base + 0x00004270);

		const auto eDrawMode = GENERATE(DRAWMODE_TRANS25, DRAWMODE_TRANS50, DRAWMODE_TRANS75, DRAWMODE_MODULATE, DRAWMODE_BURN, DRAWMODE_NORMAL, DRAWMODE_TRANSHIGHLIGHT, DRAWMODE_HIGHLIGHT);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data and palette
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const uint32_t dwGamma = random_unsigned_integer(0, 255);
			const int32_t nScreenMode = random_signed_integer(0, 3);
			uint8_t pPalette[256] = {};

			// Call both implementations
			sut(&pData, nXPos, nYPos, dwGamma, eDrawMode, nScreenMode, pPalette);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos, dwGamma, eDrawMode, nScreenMode, pPalette);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA742A0 (#10072)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelDraw, dll_base + 0x000042A0);

		const auto eDrawMode = GENERATE(DRAWMODE_TRANS25, DRAWMODE_TRANS50, DRAWMODE_TRANS75, DRAWMODE_MODULATE, DRAWMODE_BURN, DRAWMODE_NORMAL, DRAWMODE_TRANSHIGHLIGHT, DRAWMODE_HIGHLIGHT);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data and palette
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const uint32_t dwGamma = random_unsigned_integer(0, 255);
			uint8_t pPalette[256] = {};

			// Call both implementations
			sut(&pData, nXPos, nYPos, dwGamma, eDrawMode, pPalette);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos, dwGamma, eDrawMode, pPalette);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA742D0 (#10073)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelDrawColor, dll_base + 0x000042D0);

		const auto eDrawMode = GENERATE(DRAWMODE_TRANS25, DRAWMODE_TRANS50, DRAWMODE_TRANS75, DRAWMODE_MODULATE, DRAWMODE_BURN, DRAWMODE_NORMAL, DRAWMODE_TRANSHIGHLIGHT, DRAWMODE_HIGHLIGHT);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const uint32_t dwGamma = random_unsigned_integer(0, 255);
			const int32_t nGlobalPaletteShift = random_signed_integer(0, 255);

			// Call both implementations
			sut(&pData, nXPos, nYPos, dwGamma, eDrawMode, nGlobalPaletteShift);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos, dwGamma, eDrawMode, nGlobalPaletteShift);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74300 (#10074)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelDrawEx, dll_base + 0x00004300);

		const auto eDrawMode = GENERATE(DRAWMODE_TRANS25, DRAWMODE_TRANS50, DRAWMODE_TRANS75, DRAWMODE_MODULATE, DRAWMODE_BURN, DRAWMODE_NORMAL, DRAWMODE_TRANSHIGHLIGHT, DRAWMODE_HIGHLIGHT);
		// Nothing is drawn without lines
		const auto bDrawLines = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const int32_t nSkipLines = random_signed_integer(0, 100);
			const int32_t nDrawLines = bDrawLines ? random_signed_integer(1, 100) : 0;

			// Call both implementations
			sut(&pData, nXPos, nYPos, nSkipLines, nDrawLines, eDrawMode);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos, nSkipLines, nDrawLines, eDrawMode);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74330 (#10077)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelDrawClipped, dll_base + 0x00004330);

		const auto eDrawMode = GENERATE(DRAWMODE_TRANS25, DRAWMODE_TRANS50, DRAWMODE_TRANS75, DRAWMODE_MODULATE, DRAWMODE_BURN, DRAWMODE_NORMAL, DRAWMODE_TRANSHIGHLIGHT, DRAWMODE_HIGHLIGHT);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data and crop rect
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			RECT pCropRect = { random_signed_integer(0, 800), random_signed_integer(0, 600), random_signed_integer(0, 800), random_signed_integer(0, 600) };

			// Call both implementations
			sut(&pData, nXPos, nYPos, &pCropRect, eDrawMode);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos, &pCropRect, eDrawMode);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74360 (#10075)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelDrawShadow, dll_base + 0x00004360);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);

			// Call both implementations
			sut(&pData, nXPos, nYPos);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74380 (#10076)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_CelDrawHilight, dll_base + 0x00004380);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same gfx data
			D2GfxDataStrc pData{};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const auto nPaletteIndex = static_cast<uint8_t>(random_unsigned_integer(0, 255));

			// Call both implementations
			sut(&pData, nXPos, nYPos, nPaletteIndex);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(&pData, nXPos, nYPos, nPaletteIndex);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA743A0 (#10078)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TEXTURE_DebugFillBackBuffer, dll_base + 0x000043A0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);

			// Call both implementations
			sut(nXPos, nYPos);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(nXPos, nYPos);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}
}

#endif
