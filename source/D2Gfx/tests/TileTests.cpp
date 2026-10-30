#include <D2GfxTestDefines.h>

#ifdef TILE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <filesystem>

#include <D2GfxTestDefinitions.h>
#include <TestUtilities.h>

#include <D2Gfx.h>
#include <DrawMode.h>
#include <Tile.h>

#include <Fixtures/D2Gfx/D2GfxGlobalsFixture.h>


using GfxFixture = D2GfxGlobalsFixture<NoOpFixture>;


TEST_SUITE("TileTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Gfx.dll").string().c_str()));


	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA743C0 (#10080)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TILE_TileDrawLit, dll_base + 0x000043C0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same tile and light
			uint8_t pTile[0x60] = {};
			D2GfxLightStrc pLight[4] = {};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const int32_t nScreenPanels = random_signed_integer(0, 3);
			const auto nReturnValue = random_unsigned_integer();

			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut(reinterpret_cast<D2TileLibraryEntryStrc*>(pTile), nXPos, nYPos, pLight, nScreenPanels);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original(reinterpret_cast<D2TileLibraryEntryStrc*>(pTile), nXPos, nYPos, pLight, nScreenPanels);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA743F0 (#10081)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TILE_TileDrawTrans, dll_base + 0x000043F0);

		REPEAT_10();

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same tile and light
			uint8_t pTile[0x60] = {};
			D2GfxLightStrc pLight[4] = {};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const int32_t nScreenPanels = random_signed_integer(0, 3);
			const auto nAlpha = static_cast<uint8_t>(random_unsigned_integer(0, 255));
			const auto nReturnValue = random_unsigned_integer();

			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut(reinterpret_cast<D2TileLibraryEntryStrc*>(pTile), nXPos, nYPos, pLight, nScreenPanels, nAlpha);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original(reinterpret_cast<D2TileLibraryEntryStrc*>(pTile), nXPos, nYPos, pLight, nScreenPanels, nAlpha);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74420 (#10082)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(TILE_ShadowTileDraw, dll_base + 0x00004420);

		const auto eDrawMode = GENERATE(DRAWMODE_TRANS25, DRAWMODE_TRANS50, DRAWMODE_TRANS75, DRAWMODE_MODULATE, DRAWMODE_BURN, DRAWMODE_NORMAL, DRAWMODE_TRANSHIGHLIGHT, DRAWMODE_HIGHLIGHT);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same tile
			uint8_t pTile[0x60] = {};
			const int32_t nXPos = random_signed_integer(-200, 1000);
			const int32_t nYPos = random_signed_integer(-200, 800);
			const int32_t nScreenPanels = random_signed_integer(0, 3);
			const auto nReturnValue = random_unsigned_integer();

			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut(reinterpret_cast<D2TileLibraryEntryStrc*>(pTile), nXPos, nYPos, eDrawMode, nScreenPanels);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original(reinterpret_cast<D2TileLibraryEntryStrc*>(pTile), nXPos, nYPos, eDrawMode, nScreenPanels);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}
}

#endif
