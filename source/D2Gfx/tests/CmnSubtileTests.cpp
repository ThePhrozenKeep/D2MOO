#include <D2GfxTestDefines.h>

#ifdef CMNSUBTILE_TESTS

#include <doctest.h>

#include <Windows.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <vector>

#include <D2GfxTestDefinitions.h>
#include <TestUtilities.h>

#include <CmnSubtile.h>
#include <D2Gfx.h>
#include <DisplayType.h>

#include <Fixtures/D2Gfx/D2GfxGlobalsFixture.h>
#include <Fixtures/D2Gfx/D2GfxSoftwareRendererFixture.h>


DYNAMIC_ARRAY_TYPE(int32_t)
DYNAMIC_ARRAY_TYPE(D2GfxLightExStrc)


using SoftwareRendererFixture = D2GfxSoftwareRendererFixture<D2GfxGlobalsFixture<NoOpFixture>>;


// Intensities of the 4 corners of a tile or block (left, right, bottom right, bottom left), as computed by the callers of the gouraud functions
static void generate_intensities(int32_t(&pIntensities)[4], int32_t nMode)
{
	const int32_t nBase = random_signed_integer(0, 255);
	for (auto& nIntensity : pIntensities)
	{
		switch (nMode)
		{
		case 0: // Random
			nIntensity = random_signed_integer(0, 255);
			break;

		case 1: // Almost flat, drawn without interpolation
			nIntensity = std::clamp(nBase + random_signed_integer(-1, 1), 0, 255);
			break;

		case 2: // Around the difference of intensities above which the intensity is interpolated
			nIntensity = std::clamp(nBase + random_signed_integer(-3, 3), 0, 255);
			break;

		default: // Full intensity
			nIntensity = random_signed_integer(0xF8, 0xFF);
			break;
		}
	}
}


TEST_SUITE("CmnSubtileTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Gfx.dll").string().c_str()));


	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71000")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_InitGouraudCache, dll_base + 0x00001000);

		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");

			// Check specific values
			CHECK_EQ(moo_renderer.pBuffer, nullptr);
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71010")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_FillYBufferTable, dll_base + 0x00001010);

		const auto nPitch = GENERATE(buffer_pitch, buffer_pitch / 2, 640, 800);
		const auto nViewMatrixAdjust = GENERATE(0, 1, 2);
		const auto bViewportUpToDate = GENERATE(0, 1);

		SUBCASE("")
		{
			// Input data
			const int32_t nHeight = random_signed_integer(100, buffer_height);
			const int32_t nUnknown = random_signed_integer(0, 1000);

			const auto setup_data = [nViewMatrixAdjust, bViewportUpToDate](
				D2GfxGlobals& globals,
				D2GfxSoftwareRendererGlobals& renderer
			) {
				globals.nViewMatrixAdjust = nViewMatrixAdjust;
				renderer.nViewportViewMatrixAdjust = bViewportUpToDate ? nViewMatrixAdjust : -1;
			};

			setup_data(moo_globals, moo_renderer);
			setup_data(original_globals, original_renderer);

			// Call both implementations, each with its own back buffer
			sut(moo_back_buffer(), nPitch, nHeight, nUnknown);
			original(original_back_buffer(), nPitch, nHeight, nUnknown);

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71070")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_SetPaletteTables, dll_base + 0x00001070);

		SUBCASE("")
		{
			// Input data
			D2PaletteTableStrc moo_pPaletteTable = palette_table;
			D2PaletteTableStrc original_pPaletteTable = palette_table;

			const auto setup_data = [](
				D2GfxSoftwareRendererGlobals& renderer
			) {
				renderer.pTransPalettes = nullptr;
				std::memset(renderer.pPaletteTable, 0, sizeof(renderer.pPaletteTable));
				std::memset(renderer.ShadowLightPalettes, 0, sizeof(renderer.ShadowLightPalettes));
			};

			setup_data(moo_renderer);
			setup_data(original_renderer);

			// Call both implementations
			sut(&moo_pPaletteTable);
			original(&original_pPaletteTable);

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_pPaletteTable, original_pPaletteTable, "Comparing pPaletteTable");

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA710C0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_SetContrastAndGamma, dll_base + 0x000010C0);

		// The default contrast depends on the display type. The values stored in the registry (if any) are read by both implementations.
		const auto nDisplayType = GENERATE(DISPLAYTYPE_GDI, DISPLAYTYPE_DDRAW, DISPLAYTYPE_GLIDE, DISPLAYTYPE_DIRECT3D);

		SUBCASE("")
		{
			const auto nReturnValue = random_unsigned_integer();
			const int32_t nGamma = random_signed_integer(0, 255);

			const auto setup_data = [nDisplayType, nGamma](
				D2GfxGlobals& globals
			) {
				globals.nDisplayType = nDisplayType;
				globals.GfxSettings.nGamma = nGamma;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut();
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original();
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71130")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawIsoBlockFlat, dll_base + 0x00001130);

		REPEAT_5();

		SUBCASE("")
		{
			for (auto i = 0; i < 20; ++i)
			{
				// Input data
				auto moo_pSrc = generate_random_bytes(256);
				auto original_pSrc = moo_pSrc;
				auto moo_pPalette = generate_random_bytes(sizeof(D2PaletteStrc));
				auto original_pPalette = moo_pPalette;

				const int32_t nX = random_signed_integer(0, buffer_width - 32);
				const int32_t nY = random_signed_integer(0, buffer_height - 15);

				// Call both implementations
				sut(moo_pSrc.data(), moo_pixel(nX, nY), moo_pPalette.data());
				original(original_pSrc.data(), original_pixel(nX, nY), original_pPalette.data());

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_bytes_eq(moo_pPalette, original_pPalette, "Comparing pPalette");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71340")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawIsoBlockGouraud, dll_base + 0x00001340);

		REPEAT_5();

		SUBCASE("")
		{
			for (auto i = 0; i < 20; ++i)
			{
				// Input data
				auto moo_pSrc = generate_random_bytes(256);
				auto original_pSrc = moo_pSrc;

				const int32_t nX = random_signed_integer(0, buffer_width - 32);
				const int32_t nY = random_signed_integer(0, buffer_height - 15);

				// Same parameters as CMNSUBTILE_FloorTileDraw
				int32_t pIntensities[4] = {};
				generate_intensities(pIntensities, 0);
				const int32_t nLeftIntensity = 16 * pIntensities[0];
				const int32_t nLeftIntensityStep = pIntensities[3] - pIntensities[0];
				const int32_t nRightIntensity = 16 * pIntensities[1];
				const int32_t nRightIntensityStep = pIntensities[2] - pIntensities[1];

				// Call both implementations
				sut(moo_pSrc.data(), moo_pixel(nX, nY), nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep);
				original(original_pSrc.data(), original_pixel(nX, nY), nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71720")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawRleFlat, dll_base + 0x00001720);

		// Floor blocks have 15 lines, vis tiles 32
		const auto nLines = GENERATE(15, 32);

		SUBCASE("")
		{
			for (auto i = 0; i < 20; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(nLines, 32);
				auto original_pSrc = moo_pSrc;
				auto moo_pPalette = generate_random_bytes(sizeof(D2PaletteStrc));
				auto original_pPalette = moo_pPalette;

				const int32_t nX = random_signed_integer(0, buffer_width - 32);
				const int32_t nY = random_signed_integer(0, buffer_height - nLines);

				// Call both implementations
				sut(moo_pSrc.data(), moo_pixel(nX, nY), moo_pPalette.data(), nLines);
				original(original_pSrc.data(), original_pixel(nX, nY), original_pPalette.data(), nLines);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_bytes_eq(moo_pPalette, original_pPalette, "Comparing pPalette");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71970")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawRleGouraud, dll_base + 0x00001970);

		// Parameters used for floor blocks (15 lines, intensities * 16, shift of 7) and vis tiles (32 lines, intensities * 32, shift of 8)
		const auto bFloorBlock = GENERATE(0, 1);

		SUBCASE("")
		{
			const int32_t nLines = bFloorBlock ? 15 : 32;
			const int32_t nIntensityMultiplier = bFloorBlock ? 16 : 32;
			const uint8_t nIntensityShift = bFloorBlock ? 7 : 8;

			for (auto i = 0; i < 20; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(nLines, 32);
				auto original_pSrc = moo_pSrc;

				const int32_t nX = random_signed_integer(0, buffer_width - 32);
				const int32_t nY = random_signed_integer(0, buffer_height - nLines);

				int32_t pIntensities[4] = {};
				generate_intensities(pIntensities, 0);
				const int32_t nLeftIntensity = nIntensityMultiplier * pIntensities[0];
				const int32_t nLeftIntensityStep = pIntensities[3] - pIntensities[0];
				const int32_t nRightIntensity = nIntensityMultiplier * pIntensities[1];
				const int32_t nRightIntensityStep = pIntensities[2] - pIntensities[1];

				// Call both implementations
				sut(moo_pSrc.data(), moo_pixel(nX, nY), nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, nLines, nIntensityShift);
				original(original_pSrc.data(), original_pixel(nX, nY), nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, nLines, nIntensityShift);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA71D90")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawRleFlatClipped, dll_base + 0x00001D90);

		const auto nViewport = GENERATE(0, 1, 2);
		const auto nLines = GENERATE(15, 32);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(nLines, 32);
				auto original_pSrc = moo_pSrc;
				auto moo_pPalette = generate_random_bytes(sizeof(D2PaletteStrc));
				auto original_pPalette = moo_pPalette;

				// Positions of the vis tiles that are drawn at all
				const int32_t nX = random_signed_integer(moo_renderer.nVisTileMinX, moo_renderer.nVisTileMaxX - 1);
				const int32_t nY = random_signed_integer(moo_renderer.nVisTileMinY, moo_renderer.nVisTileMaxY - 1);

				// Call both implementations
				sut(nX, nY, moo_pSrc.data(), moo_pixel(nX, nY), moo_pPalette.data(), nLines);
				original(nX, nY, original_pSrc.data(), original_pixel(nX, nY), original_pPalette.data(), nLines);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_bytes_eq(moo_pPalette, original_pPalette, "Comparing pPalette");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72090")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawRleGouraudClipped, dll_base + 0x00002090);

		const auto nViewport = GENERATE(0, 1, 2);
		// Parameters used for floor blocks (15 lines, intensities * 16, shift of 7) and vis tiles (32 lines, intensities * 32, shift of 8)
		const auto bFloorBlock = GENERATE(0, 1);

		SUBCASE("")
		{
			set_viewport(nViewport);

			const int32_t nLines = bFloorBlock ? 15 : 32;
			const int32_t nIntensityMultiplier = bFloorBlock ? 16 : 32;
			const uint8_t nIntensityShift = bFloorBlock ? 7 : 8;

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(nLines, 32);
				auto original_pSrc = moo_pSrc;

				// Positions of the vis tiles that are drawn at all
				const int32_t nX = random_signed_integer(moo_renderer.nVisTileMinX, moo_renderer.nVisTileMaxX - 1);
				const int32_t nY = random_signed_integer(moo_renderer.nVisTileMinY, moo_renderer.nVisTileMaxY - 1);

				int32_t pIntensities[4] = {};
				generate_intensities(pIntensities, 0);
				const int32_t nLeftIntensity = nIntensityMultiplier * pIntensities[0];
				const int32_t nLeftIntensityStep = pIntensities[3] - pIntensities[0];
				const int32_t nRightIntensity = nIntensityMultiplier * pIntensities[1];
				const int32_t nRightIntensityStep = pIntensities[2] - pIntensities[1];

				// Call both implementations
				sut(nX, nY, moo_pSrc.data(), moo_pixel(nX, nY), nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, nLines, nIntensityShift);
				original(nX, nY, original_pSrc.data(), original_pixel(nX, nY), nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, nLines, nIntensityShift);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72570")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_BuildGouraudCache, dll_base + 0x00002570);

		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");

			// Check specific values
			for (int32_t nIntensity = 0; nIntensity < 32; ++nIntensity)
			{
				CHECK_EQ(moo_renderer.GouraudCache[nIntensity][nIntensity][0], nIntensity);
				CHECK_EQ(moo_renderer.GouraudCache[nIntensity][nIntensity][31], nIntensity);
			}
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72630")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_SetViewport, dll_base + 0x00002630);

		// Only 1 and 2 have a meaning (left or right panel open)
		const auto nViewMatrixAdjust = GENERATE(0, 1, 2, 3);

		SUBCASE("")
		{
			for (auto i = 0; i < 20; ++i)
			{
				// Input data
				const int32_t nWidth = random_signed_integer(0, 2000);
				const int32_t nHeight = random_signed_integer(0, 2000);

				const auto setup_data = [nViewMatrixAdjust](
					D2GfxGlobals& globals
				) {
					globals.nViewMatrixAdjust = nViewMatrixAdjust;
				};

				setup_data(moo_globals);
				setup_data(original_globals);

				// Call both implementations
				sut(nWidth, nHeight);
				original(nWidth, nHeight);

				// Compare potentially modified global data
				check_renderer_eq("Comparing renderer globals");
				check_globals_eq("Comparing globals");
			}
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72700")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_BuildLineOffsets, dll_base + 0x00002700);

		SUBCASE("")
		{
			// Input data
			const int32_t nPitch = random_signed_integer(0, 2000);

			const auto setup_data = [nPitch](
				D2GfxSoftwareRendererGlobals& renderer
			) {
				renderer.nPitch = nPitch;
				std::memset(renderer.nLineOffsets, 0, sizeof(renderer.nLineOffsets));
			};

			setup_data(moo_renderer);
			setup_data(original_renderer);

			// Call both implementations
			sut();
			original();

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");

			// Check specific values
			CHECK_EQ(moo_renderer.nLineOffsets[32], 0);
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72730")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawVisTile, dll_base + 0x00002730);

		const auto nViewport = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(32, 32);
				auto original_pSrc = moo_pSrc;

				// Also covers tiles that are not drawn at all
				const int32_t nX = random_signed_integer(-64, buffer_width + 32);
				const int32_t nY = random_signed_integer(-64, buffer_height);

				// Call both implementations
				sut(moo_pSrc.data(), nX, nY);
				original(original_pSrc.data(), nX, nY);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72900")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawFlatLitVisTile, dll_base + 0x00002900);

		const auto nViewport = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(32, 32);
				auto original_pSrc = moo_pSrc;

				// Also covers tiles that are not drawn at all
				const int32_t nX = random_signed_integer(-64, buffer_width + 32);
				const int32_t nY = random_signed_integer(-64, buffer_height);
				const uint8_t nIntensity = static_cast<uint8_t>(random_unsigned_integer(0, 255));

				// Call both implementations
				sut(moo_pSrc.data(), nX, nY, nIntensity);
				original(original_pSrc.data(), nX, nY, nIntensity);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA729D0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_TileDrawLit, dll_base + 0x000029D0);

		const auto nViewport = GENERATE(0, 1, 2);
		const auto bForceFlatLighting = GENERATE(0, 1);
		const auto bLowQuality = GENERATE(0, 1);

		SUBCASE("")
		{
			set_viewport(nViewport);

			const auto setup_data = [bForceFlatLighting, bLowQuality](
				D2GfxGlobals& globals,
				D2GfxSoftwareRendererGlobals& renderer
			) {
				globals.GfxSettings.bLowQuality = bLowQuality;
				renderer.bForceFlatLighting = bForceFlatLighting;
			};

			setup_data(moo_globals, moo_renderer);
			setup_data(original_globals, original_renderer);

			for (auto i = 0; i < 60; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(32, 32);
				auto original_pSrc = moo_pSrc;

				int32_t moo_pIntensities[4] = {};
				generate_intensities(moo_pIntensities, i % 4);
				int32_t original_pIntensities[4] = {};
				std::memcpy(original_pIntensities, moo_pIntensities, sizeof(original_pIntensities));

				// Also covers tiles that are not drawn at all
				const int32_t nX = random_signed_integer(-64, buffer_width + 32);
				const int32_t nY = random_signed_integer(-64, buffer_height);

				// Call both implementations
				sut(moo_pSrc.data(), nX, nY, moo_pIntensities);
				original(original_pSrc.data(), nX, nY, original_pIntensities);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				DynamicArray<int32_t> moo_pIntensities_array{ moo_pIntensities, 4 };
				DynamicArray<int32_t> original_pIntensities_array{ original_pIntensities, 4 };
				MOO_CHECK_EQ(moo_pIntensities_array, original_pIntensities_array, "Comparing pIntensities");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72B80")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawBlendedVisTile, dll_base + 0x00002B80);

		const auto nViewport = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(32, 32);
				auto original_pSrc = moo_pSrc;

				// Also covers tiles that are not drawn at all
				const int32_t nX = random_signed_integer(-64, buffer_width + 32);
				const int32_t nY = random_signed_integer(-64, buffer_height);
				const uint8_t nAlpha = static_cast<uint8_t>(random_unsigned_integer(0, 255));

				// Call both implementations
				sut(moo_pSrc.data(), nX, nY, nAlpha);
				original(original_pSrc.data(), nX, nY, nAlpha);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA72DC0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawRoofTile, dll_base + 0x00002DC0);

		const auto nViewport = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_pSrc = generate_rle_data(32, 32);
				auto original_pSrc = moo_pSrc;

				int32_t moo_pIntensities[4] = {};
				generate_intensities(moo_pIntensities, 0);
				int32_t original_pIntensities[4] = {};
				std::memcpy(original_pIntensities, moo_pIntensities, sizeof(original_pIntensities));

				// Also covers tiles that are not drawn at all
				const int32_t nX = random_signed_integer(-64, buffer_width + 32);
				const int32_t nY = random_signed_integer(-64, buffer_height);
				const uint8_t nAlpha = static_cast<uint8_t>(random_unsigned_integer(0, 255));

				// Call both implementations
				sut(moo_pSrc.data(), nX, nY, moo_pIntensities, nAlpha);
				original(original_pSrc.data(), nX, nY, original_pIntensities, nAlpha);

				// Compare potentially modified input data
				check_bytes_eq(moo_pSrc, original_pSrc, "Comparing pSrc");
				DynamicArray<int32_t> moo_pIntensities_array{ moo_pIntensities, 4 };
				DynamicArray<int32_t> original_pIntensities_array{ original_pIntensities, 4 };
				MOO_CHECK_EQ(moo_pIntensities_array, original_pIntensities_array, "Comparing pIntensities");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA73130")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawFloorBlockFlatClipped, dll_base + 0x00003130);

		const auto nViewport = GENERATE(0, 1, 2);
		const auto nFormat = GENERATE(TILEBLOCKFORMAT_VISIBLE, TILEBLOCKFORMAT_VISIBLE | TILEBLOCKFORMAT_RLE);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_data = (nFormat & TILEBLOCKFORMAT_RLE) ? generate_rle_data(15, 32) : generate_random_bytes(256);
				auto original_data = moo_data;

				D2TileLibraryBlockStrc moo_pBlock{};
				moo_pBlock.nFormat = static_cast<uint16_t>(nFormat);
				moo_pBlock.nLength = static_cast<uint32_t>(moo_data.size());
				D2TileLibraryBlockStrc original_pBlock = moo_pBlock;
				moo_pBlock.dwOffset_pData = moo_data.data();
				original_pBlock.dwOffset_pData = original_data.data();

				// Positions of the floor blocks that are drawn at all
				const int32_t nX = random_signed_integer(moo_renderer.nFloorBlockMinX, moo_renderer.nFloorBlockMaxX - 1);
				const int32_t nY = random_signed_integer(moo_renderer.nFloorBlockMinY, moo_renderer.nFloorBlockMaxY - 1);
				const uint8_t nIntensity = static_cast<uint8_t>(random_unsigned_integer(0, 255));

				// Call both implementations
				sut(&moo_pBlock, nX, nY, moo_pixel(nX, nY), nIntensity);
				original(&original_pBlock, nX, nY, original_pixel(nX, nY), nIntensity);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pBlock, original_pBlock, "Comparing pBlock");
				check_bytes_eq(moo_data, original_data, "Comparing pBlock->dwOffset_pData");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA73270")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_DrawFloorBlockGouraudClipped, dll_base + 0x00003270);

		const auto nViewport = GENERATE(0, 1, 2);
		const auto nFormat = GENERATE(TILEBLOCKFORMAT_VISIBLE, TILEBLOCKFORMAT_VISIBLE | TILEBLOCKFORMAT_RLE);

		SUBCASE("")
		{
			set_viewport(nViewport);

			for (auto i = 0; i < 50; ++i)
			{
				// Input data
				auto moo_data = (nFormat & TILEBLOCKFORMAT_RLE) ? generate_rle_data(15, 32) : generate_random_bytes(256);
				auto original_data = moo_data;

				D2TileLibraryBlockStrc moo_pBlock{};
				moo_pBlock.nFormat = static_cast<uint16_t>(nFormat);
				moo_pBlock.nLength = static_cast<uint32_t>(moo_data.size());
				D2TileLibraryBlockStrc original_pBlock = moo_pBlock;
				moo_pBlock.dwOffset_pData = moo_data.data();
				original_pBlock.dwOffset_pData = original_data.data();

				int32_t moo_pIntensities[4] = {};
				generate_intensities(moo_pIntensities, 0);
				int32_t original_pIntensities[4] = {};
				std::memcpy(original_pIntensities, moo_pIntensities, sizeof(original_pIntensities));

				// Positions of the floor blocks that are drawn at all
				const int32_t nX = random_signed_integer(moo_renderer.nFloorBlockMinX, moo_renderer.nFloorBlockMaxX - 1);
				const int32_t nY = random_signed_integer(moo_renderer.nFloorBlockMinY, moo_renderer.nFloorBlockMaxY - 1);

				// Call both implementations
				sut(&moo_pBlock, nX, nY, moo_pixel(nX, nY), moo_pIntensities);
				original(&original_pBlock, nX, nY, original_pixel(nX, nY), original_pIntensities);

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pBlock, original_pBlock, "Comparing pBlock");
				check_bytes_eq(moo_data, original_data, "Comparing pBlock->dwOffset_pData");
				DynamicArray<int32_t> moo_pIntensities_array{ moo_pIntensities, 4 };
				DynamicArray<int32_t> original_pIntensities_array{ original_pIntensities, 4 };
				MOO_CHECK_EQ(moo_pIntensities_array, original_pIntensities_array, "Comparing pIntensities");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
		}
	}

	TEST_CASE_FIXTURE(SoftwareRendererFixture, "D2Gfx.0x6FA73410")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(CMNSUBTILE_FloorTileDraw, dll_base + 0x00003410);

		const auto nViewport = GENERATE(0, 1, 2);
		const auto bForceFlatLighting = GENERATE(0, 1);
		const auto bLowQuality = GENERATE(0, 1);

		SUBCASE("")
		{
			set_viewport(nViewport);

			const auto setup_data = [bForceFlatLighting, bLowQuality](
				D2GfxGlobals& globals,
				D2GfxSoftwareRendererGlobals& renderer
			) {
				globals.GfxSettings.bLowQuality = bLowQuality;
				renderer.bForceFlatLighting = bForceFlatLighting;
			};

			setup_data(moo_globals, moo_renderer);
			setup_data(original_globals, original_renderer);

			for (auto i = 0; i < 20; ++i)
			{
				// Input data: a floor tile of 160x80 pixels made of 32x15 blocks
				const int32_t nBlocks = random_signed_integer(1, 16);

				std::vector<std::vector<uint8_t>> moo_data(nBlocks);
				std::vector<std::vector<uint8_t>> original_data(nBlocks);
				std::vector<D2TileLibraryBlockStrc> moo_blocks(nBlocks);
				std::vector<D2TileLibraryBlockStrc> original_blocks(nBlocks);
				for (int32_t nBlock = 0; nBlock < nBlocks; ++nBlock)
				{
					D2TileLibraryBlockStrc& block = moo_blocks[nBlock];
					block.nPosX = static_cast<int16_t>(random_signed_integer(0, 4) * 32);
					block.nPosY = static_cast<int16_t>(random_signed_integer(0, 9) * 8 - 8);
					block.nGridX = static_cast<uint8_t>(random_signed_integer(0, 4));
					block.nGridY = static_cast<uint8_t>(random_signed_integer(0, 4));
					block.nFormat = static_cast<uint16_t>(random_unsigned_integer(0, 1) * TILEBLOCKFORMAT_VISIBLE | random_unsigned_integer(0, 1) * TILEBLOCKFORMAT_RLE);

					moo_data[nBlock] = (block.nFormat & TILEBLOCKFORMAT_RLE) ? generate_rle_data(15, 32) : generate_random_bytes(256);
					original_data[nBlock] = moo_data[nBlock];
					block.nLength = static_cast<uint32_t>(moo_data[nBlock].size());

					original_blocks[nBlock] = block;
					moo_blocks[nBlock].dwOffset_pData = moo_data[nBlock].data();
					original_blocks[nBlock].dwOffset_pData = original_data[nBlock].data();
				}

				D2GfxTileLibraryEntryStrc moo_pTile{};
				moo_pTile.nBlocks = nBlocks;
				D2GfxTileLibraryEntryStrc original_pTile = moo_pTile;
				moo_pTile.pBlocks = moo_blocks.data();
				original_pTile.pBlocks = original_blocks.data();

				// Light grid with 8 entries per row, the blocks use the entries up to (nGridX + 8 * nGridY) + 26.
				// The intensities are random, or close enough to each other for the blocks to be drawn without interpolation or around that limit.
				const int32_t nBaseIntensity = random_signed_integer(0, 255);
				const int32_t nIntensityVariation = (i % 3 == 0) ? 255 : (i % 3 == 1) ? 1 : 4;
				std::vector<D2GfxLightExStrc> moo_pLightEx(8 * 9);
				for (auto& light : moo_pLightEx)
				{
					light.nIntensity = static_cast<uint8_t>(std::clamp(nBaseIntensity + random_signed_integer(-nIntensityVariation, nIntensityVariation), 0, 255));
					light.nRed = static_cast<uint8_t>(random_unsigned_integer(0, 255));
					light.nGreen = static_cast<uint8_t>(random_unsigned_integer(0, 255));
					light.nBlue = static_cast<uint8_t>(random_unsigned_integer(0, 255));
				}
				std::vector<D2GfxLightExStrc> original_pLightEx = moo_pLightEx;

				// Also covers tiles that are not drawn at all
				const int32_t nX = random_signed_integer(-192, buffer_width + 32);
				const int32_t nY = random_signed_integer(-96, buffer_height);

				// Call both implementations
				sut(reinterpret_cast<D2TileLibraryEntryStrc*>(&moo_pTile), nX, nY, moo_pLightEx.data());
				original(reinterpret_cast<D2TileLibraryEntryStrc*>(&original_pTile), nX, nY, original_pLightEx.data());

				// Compare potentially modified input data
				MOO_CHECK_EQ(moo_pTile, original_pTile, "Comparing pTile");
				for (int32_t nBlock = 0; nBlock < nBlocks; ++nBlock)
				{
					check_bytes_eq(moo_data[nBlock], original_data[nBlock], "Comparing pTile->pBlocks[]->dwOffset_pData");
				}
				DynamicArray<D2GfxLightExStrc> moo_pLightEx_array{ moo_pLightEx.data(), static_cast<int>(moo_pLightEx.size()) };
				DynamicArray<D2GfxLightExStrc> original_pLightEx_array{ original_pLightEx.data(), static_cast<int>(original_pLightEx.size()) };
				MOO_CHECK_EQ(moo_pLightEx_array, original_pLightEx_array, "Comparing pLightEx");
				check_buffers_eq("Comparing back buffers");
			}

			// Compare potentially modified global data
			check_renderer_eq("Comparing renderer globals");
			check_globals_eq("Comparing globals");
		}
	}
}

#endif
