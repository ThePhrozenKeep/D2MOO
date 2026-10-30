#include <D2CommonTestDefines.h>

#ifdef OVERLAY_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/OverlayTbls.h>


TEST_SUITE("OverlayTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD720A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadOverlayTxt, dll_base + 0x000320A0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72500" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadOverlayTxt, dll_base + 0x00032500);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72530 (#10674)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetFramesFromOverlayTxt, dll_base + 0x00032530);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72570 (#10675)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATABLS_GetPreDrawFromOverlayTxt, dll_base + 0x00032570);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD725B0 (#10676)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetOffsetXFromOverlayTxt, dll_base + 0x000325B0);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD725F0 (#10677)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetOffsetYFromOverlayTxt, dll_base + 0x000325F0);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72630 (#10678)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetTransFromOverlayTxt, dll_base + 0x00032630);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72670 (#10679)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetRadiusAndColorFromOverlayTxt, dll_base + 0x00032670);
		
		SUBCASE("")
		{
			// Input data
			int moo_pInitRadius{};
			int moo_pRadius{};
			uint8_t moo_pRed{};
			uint8_t moo_pGreen{};
			uint8_t moo_pBlue{};
			int original_pInitRadius{};
			int original_pRadius{};
			uint8_t original_pRed{};
			uint8_t original_pGreen{};
			uint8_t original_pBlue{};
			int nOverlayId{};

			const auto setup_data = [](
				int& pInitRadius,
				int& pRadius,
				uint8_t& pRed,
				uint8_t& pGreen,
				uint8_t& pBlue
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInitRadius, moo_pRadius, moo_pRed, moo_pGreen, moo_pBlue);
			setup_data(original_pInitRadius, original_pRadius, original_pRed, original_pGreen, original_pBlue);

			// Call both implementations
			const auto moo_result = sut(nOverlayId, &moo_pInitRadius, &moo_pRadius, &moo_pRed, &moo_pGreen, &moo_pBlue);
			const auto original_result = original(nOverlayId, &original_pInitRadius, &original_pRadius, &original_pRed, &original_pGreen, &original_pBlue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInitRadius, original_pInitRadius, "Comparing pInitRadius");
			SKIP_MOO_CHECK_EQ(moo_pRadius, original_pRadius, "Comparing pRadius");
			SKIP_MOO_CHECK_EQ(moo_pRed, original_pRed, "Comparing pRed");
			SKIP_MOO_CHECK_EQ(moo_pGreen, original_pGreen, "Comparing pGreen");
			SKIP_MOO_CHECK_EQ(moo_pBlue, original_pBlue, "Comparing pBlue");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72720 (#10680)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_Get1OfNFromOverlayTxt, dll_base + 0x00032720);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD72760 (#10681)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetDirFromOverlayTxt, dll_base + 0x00032760);
		
		SUBCASE("")
		{
			int nOverlayId{};

			// Call both implementations
			const auto moo_result = sut(nOverlayId);
			const auto original_result = original(nOverlayId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
}
#endif
