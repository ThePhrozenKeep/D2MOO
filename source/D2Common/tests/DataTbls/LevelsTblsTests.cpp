#include <D2CommonTestDefines.h>

#ifdef LEVELS_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/LevelsTbls.h>


TEST_SUITE("LevelsTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD5EAE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLevelsTxt, dll_base + 0x0001EAE0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD603C0 (#10631)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLevelsTxtRecord, dll_base + 0x000203C0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD603F0 (#10632)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetRainFromLevelsTxt, dll_base + 0x000203F0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60430 (#10634)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNoPerFromLevelsTxt, dll_base + 0x00020430);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60470 (#10633)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMudFromLevelsTxt, dll_base + 0x00020470);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD604B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLevelsTxt, dll_base + 0x000204B0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD604F0 (#11247)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonsterLevelInArea, dll_base + 0x000204F0);
		
		SUBCASE("")
		{
			int nLevelId{};
			uint8_t nDifficulty{};
			BOOL bExpansion{};

			// Call both implementations
			const auto moo_result = sut(nLevelId, nDifficulty, bExpansion);
			const auto original_result = original(nLevelId, nDifficulty, bExpansion);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60560" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetPortalLevels, dll_base + 0x00020560);
		
		SUBCASE("")
		{
			// Input data
			int moo_pnPortalLevels{};
			int original_pnPortalLevels{};

			const auto setup_data = [](
				int& pnPortalLevels
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pnPortalLevels);
			setup_data(original_pnPortalLevels);

			// Call both implementations
			const auto moo_result = sut(&moo_pnPortalLevels);
			const auto original_result = original(&original_pnPortalLevels);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pnPortalLevels, original_pnPortalLevels, "Comparing pnPortalLevels");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60570" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLevelDefsBin, dll_base + 0x00020570);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60D60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLevelDefsBin, dll_base + 0x00020D60);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60D90 (#10010)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLevelDefRecord, dll_base + 0x00020D90);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60DC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLevelTypesTxt, dll_base + 0x00020DC0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61450" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLevelTypesTxt, dll_base + 0x00021450);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61460 (#10023)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLevelTypesTxtRecord, dll_base + 0x00021460);
		
		SUBCASE("")
		{
			int nLevelType{};

			// Call both implementations
			const auto moo_result = sut(nLevelType);
			const auto original_result = original(nLevelType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD614A0 (#11226)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CheckActInLevelTypesTxt, dll_base + 0x000214A0);
		
		SUBCASE("")
		{
			int nLevelType{};
			uint8_t nAct{};

			// Call both implementations
			const auto moo_result = sut(nLevelType, nAct);
			const auto original_result = original(nLevelType, nAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61500 (#11227)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetFileNameFromLevelTypeAndFileId, dll_base + 0x00021500);
		
		SUBCASE("")
		{
			// Input data
			char moo_szFile{};
			char original_szFile{};
			int nLevelType{};
			int nFile{};

			const auto setup_data = [](
				char& szFile
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_szFile);
			setup_data(original_szFile);

			// Call both implementations
			sut(nLevelType, nFile, &moo_szFile);
			original(nLevelType, nFile, &original_szFile);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_szFile, original_szFile, "Comparing szFile");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61570" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlPrestTxt, dll_base + 0x00021570);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};
			int a2{};

			// Call both implementations
			sut(hArchive, a2);
			original(hArchive, a2);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61AD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlPrestTxt, dll_base + 0x00021AD0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61B50 (#10024)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlPrestTxtRecord, dll_base + 0x00021B50);
		
		SUBCASE("")
		{
			int nId{};

			// Call both implementations
			const auto moo_result = sut(nId);
			const auto original_result = original(nId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61B80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlPrestTxtRecordFromLevelId, dll_base + 0x00021B80);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61BC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlWarpTxt, dll_base + 0x00021BC0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61DD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlWarpTxt, dll_base + 0x00021DD0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61DE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection, dll_base + 0x00021DE0);
		
		SUBCASE("")
		{
			int nLevelId{};
			char szDirection{};

			// Call both implementations
			const auto moo_result = sut(nLevelId, szDirection);
			const auto original_result = original(nLevelId, szDirection);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61E60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlMazeTxt, dll_base + 0x00021E60);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61F90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlMazeTxt, dll_base + 0x00021F90);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61FA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlMazeTxtRecordFromLevelId, dll_base + 0x00021FA0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62020" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlSubTxt, dll_base + 0x00022020);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};
			int a2{};
			int a3{};

			// Call both implementations
			sut(hArchive, a2, a3);
			original(hArchive, a2, a3);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62600" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlSubTxt, dll_base + 0x00022600);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD626F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlSubTxtRecord, dll_base + 0x000226F0);
		
		SUBCASE("")
		{
			int nSubType{};

			// Call both implementations
			const auto moo_result = sut(nSubType);
			const auto original_result = original(nSubType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62780" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_AllocGlobalTileLibraryHash, dll_base + 0x00022780);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD628A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_FreeGlobalTileLibraryHash, dll_base + 0x000228A0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD628C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadAutomapTxt, dll_base + 0x000228C0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62D30 (#10011)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetAutomapCellId, dll_base + 0x00022D30);
		
		SUBCASE("")
		{
			uint32_t dwAutomapLevelType{};
			uint32_t dwAutomapTileType{};
			int nStyle{};
			int nSequence{};

			// Call both implementations
			const auto moo_result = sut(dwAutomapLevelType, dwAutomapTileType, nStyle, nSequence);
			const auto original_result = original(dwAutomapLevelType, dwAutomapTileType, nStyle, nSequence);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62E70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_FreeAutomap, dll_base + 0x00022E70);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
}
#endif
