#include <D2CommonTestDefines.h>

#ifdef MONSTER_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/MonsterTbls.h>
#include <Units/Units.h>


TEST_SUITE("MonsterTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD64C40 (#10592)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonsterChainInfo, dll_base + 0x00024C40);
		
		SUBCASE("")
		{
			// Input data
			int moo_pMaxChainId{};
			int moo_pChainId{};
			int original_pMaxChainId{};
			int original_pChainId{};
			int nMonsterId{};

			const auto setup_data = [](
				int& pMaxChainId,
				int& pChainId
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMaxChainId, moo_pChainId);
			setup_data(original_pMaxChainId, original_pChainId);

			// Call both implementations
			const auto moo_result = sut(nMonsterId, &moo_pMaxChainId, &moo_pChainId);
			const auto original_result = original(nMonsterId, &original_pMaxChainId, &original_pChainId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMaxChainId, original_pMaxChainId, "Comparing pMaxChainId");
			SKIP_MOO_CHECK_EQ(moo_pChainId, original_pChainId, "Comparing pChainId");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD64CD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MonStatsSkillModeLinker, dll_base + 0x00024CD0);
		
		SUBCASE("")
		{
			// Input data
			char moo_pSrc{};
			char original_pSrc{};
			void* moo_pRecord = nullptr;
			void* original_pRecord = nullptr;
			int nOffset{};
			int nPosition{};
			int nTxtRow{};
			int nTxtColumn{};

			const auto setup_data = [](
				char& pSrc
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSrc);
			setup_data(original_pSrc);

			// Call both implementations
			sut(&moo_pSrc, moo_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);
			original(&original_pSrc, original_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSrc, original_pSrc, "Comparing pSrc");
			SKIP_MOO_CHECK_EQ(moo_pRecord, original_pRecord, "Comparing pRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD64DF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonStatsTxt, dll_base + 0x00024DF0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD67BD0 (#11261)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CalculateMonsterStatsByLevel, dll_base + 0x00027BD0);
		
		SUBCASE("")
		{
			// Input data
			D2MonStatsInitStrc moo_pMonStatsInit{};
			D2MonStatsInitStrc original_pMonStatsInit{};
			int nMonsterId{};
			int nGameType{};
			int nDifficulty{};
			int nLevel{};
			short nFlags{};

			const auto setup_data = [](
				D2MonStatsInitStrc& pMonStatsInit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonStatsInit);
			setup_data(original_pMonStatsInit);

			// Call both implementations
			const auto moo_result = sut(nMonsterId, nGameType, nDifficulty, nLevel, nFlags, &moo_pMonStatsInit);
			const auto original_result = original(nMonsterId, nGameType, nDifficulty, nLevel, nFlags, &original_pMonStatsInit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonStatsInit, original_pMonStatsInit, "Comparing pMonStatsInit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD689B0 (#10642)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_SetVelocityInMonStatsTxtRecord, dll_base + 0x000289B0);
		
		SUBCASE("")
		{
			int nMonsterId{};
			short nVelocity{};

			// Call both implementations
			sut(nMonsterId, nVelocity);
			original(nMonsterId, nVelocity);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68A00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_ResetGlobalDefaultUnicodeString, dll_base + 0x00028A00);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68A10 (#10651)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_RollRandomUniqueTitleString, dll_base + 0x00028A10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68A80 (#10652)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_RollRandomUniquePrefixString, dll_base + 0x00028A80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68AF0 (#10653)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_RollRandomUniqueSuffixString, dll_base + 0x00028AF0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68B60 (#10654)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_RollRandomUniqueAppellationString, dll_base + 0x00028B60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68BD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_ReallocTCExInfo, dll_base + 0x00028BD0);
		
		SUBCASE("")
		{
			// Input data
			D2TCExShortStrc moo_pTCExShort{};
			D2TCExShortStrc original_pTCExShort{};
			int nNewRecordCount{};

			const auto setup_data = [](
				D2TCExShortStrc& pTCExShort
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTCExShort);
			setup_data(original_pTCExShort);

			// Call both implementations
			sut(&moo_pTCExShort, nNewRecordCount);
			original(&original_pTCExShort, nNewRecordCount);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTCExShort, original_pTCExShort, "Comparing pTCExShort");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68C40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UpdateTreasureClassProbabilities, dll_base + 0x00028C40);
		
		SUBCASE("")
		{
			// Input data
			D2TCExShortStrc moo_pTCExShort{};
			D2TCExInfoStrc moo_pTCExInfo{};
			D2TCExShortStrc original_pTCExShort{};
			D2TCExInfoStrc original_pTCExInfo{};
			int nProbability{};
			BOOL bExpansion{};

			const auto setup_data = [](
				D2TCExShortStrc& pTCExShort,
				D2TCExInfoStrc& pTCExInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pTCExShort, moo_pTCExInfo);
			setup_data(original_pTCExShort, original_pTCExInfo);

			// Call both implementations
			sut(&moo_pTCExShort, &moo_pTCExInfo, nProbability, bExpansion);
			original(&original_pTCExShort, &original_pTCExInfo, nProbability, bExpansion);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pTCExShort, original_pTCExShort, "Comparing pTCExShort");
			SKIP_MOO_CHECK_EQ(moo_pTCExInfo, original_pTCExInfo, "Comparing pTCExInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68D40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_ApplyRatio, dll_base + 0x00028D40);
		
		SUBCASE("")
		{
			signed int nValue{};
			signed int nMultiplier{};
			signed int nDivisor{};

			// Call both implementations
			const auto moo_result = sut(nValue, nMultiplier, nDivisor);
			const auto original_result = original(nValue, nMultiplier, nDivisor);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68DC0 (#10658)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetTreasureClassExRecordFromName, dll_base + 0x00028DC0);
		
		SUBCASE("")
		{
			// Input data
			char moo_szText{};
			char original_szText{};

			const auto setup_data = [](
				char& szText
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_szText);
			setup_data(original_szText);

			// Call both implementations
			const auto moo_result = sut(&moo_szText);
			const auto original_result = original(&original_szText);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_szText, original_szText, "Comparing szText");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68DF0 (#10659)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetTreasureClassExRecordFromIdAndLevel, dll_base + 0x00028DF0);
		
		SUBCASE("")
		{
			uint16_t wTCId{};
			int nLvl{};

			// Call both implementations
			const auto moo_result = sut(wTCId, nLvl);
			const auto original_result = original(wTCId, nLvl);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68E50 (#10660)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetTreasureClassExRecordFromActAndDifficulty, dll_base + 0x00028E50);
		
		SUBCASE("")
		{
			int nDifficulty{};
			int nAct{};
			int nIndex{};

			// Call both implementations
			const auto moo_result = sut(nDifficulty, nAct, nIndex);
			const auto original_result = original(nDifficulty, nAct, nIndex);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD68EC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadTreasureClassExTxt, dll_base + 0x00028EC0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69B70 (#10656)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_ShouldNeverCallInExpansion, dll_base + 0x00029B70);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69B90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonItemPercentTxt, dll_base + 0x00029B90);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69C40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadMonItemPercentTxt, dll_base + 0x00029C40);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69C50 (#10662)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonItemPercentDataTables, dll_base + 0x00029C50);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69C60 (#10663)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonItemPercentTxtRecord, dll_base + 0x00029C60);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69CD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonUModTxt, dll_base + 0x00029CD0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD69FA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadSuperUniquesTxt, dll_base + 0x00029FA0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6A440 (#10668)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetSuperUniquesTxtRecord, dll_base + 0x0002A440);
		
		SUBCASE("")
		{
			int nSuperUniqueId{};

			// Call both implementations
			const auto moo_result = sut(nSuperUniqueId);
			const auto original_result = original(nSuperUniqueId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6A470 (#11257)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetSuperUniquesTxtRecordCount, dll_base + 0x0002A470);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6A480" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadHirelingTxt, dll_base + 0x0002A480);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B1A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetItemTypesTxtRecord, dll_base + 0x0002B1A0);
		
		SUBCASE("")
		{
			int nItemType{};

			// Call both implementations
			const auto moo_result = sut(nItemType);
			const auto original_result = original(nItemType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B1D0 (#10583)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetHirelingTxtRecordFromIdAndLevel, dll_base + 0x0002B1D0);
		
		SUBCASE("")
		{
			BOOL bExpansion{};
			int nId{};
			int nLevel{};

			// Call both implementations
			const auto moo_result = sut(bExpansion, nId, nLevel);
			const auto original_result = original(bExpansion, nId, nLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B270 (#10585)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNextHirelingTxtRecordFromNameId, dll_base + 0x0002B270);
		
		SUBCASE("")
		{
			// Input data
			D2HirelingTxt moo_pOldRecord{};
			D2HirelingTxt original_pOldRecord{};
			BOOL bExpansion{};
			uint16_t nNameId{};

			const auto setup_data = [](
				D2HirelingTxt& pOldRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOldRecord);
			setup_data(original_pOldRecord);

			// Call both implementations
			const auto moo_result = sut(bExpansion, nNameId, &moo_pOldRecord);
			const auto original_result = original(bExpansion, nNameId, &original_pOldRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOldRecord, original_pOldRecord, "Comparing pOldRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B310 (#10586)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNextHirelingTxtRecordFromClassId, dll_base + 0x0002B310);
		
		SUBCASE("")
		{
			// Input data
			D2HirelingTxt moo_pOldRecord{};
			D2HirelingTxt original_pOldRecord{};
			BOOL bExpansion{};
			int nClass{};

			const auto setup_data = [](
				D2HirelingTxt& pOldRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOldRecord);
			setup_data(original_pOldRecord);

			// Call both implementations
			const auto moo_result = sut(bExpansion, nClass, &moo_pOldRecord);
			const auto original_result = original(bExpansion, nClass, &original_pOldRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOldRecord, original_pOldRecord, "Comparing pOldRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B3A0 (#10587)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty, dll_base + 0x0002B3A0);
		
		SUBCASE("")
		{
			// Input data
			D2HirelingTxt moo_pOldRecord{};
			D2HirelingTxt original_pOldRecord{};
			BOOL bExpansion{};
			int nVendorId{};
			int nDifficulty{};

			const auto setup_data = [](
				D2HirelingTxt& pOldRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOldRecord);
			setup_data(original_pOldRecord);

			// Call both implementations
			const auto moo_result = sut(bExpansion, nVendorId, nDifficulty, &moo_pOldRecord);
			const auto original_result = original(bExpansion, nVendorId, nDifficulty, &original_pOldRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOldRecord, original_pOldRecord, "Comparing pOldRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B440 (#10584)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty, dll_base + 0x0002B440);
		
		SUBCASE("")
		{
			// Input data
			D2HirelingTxt moo_pOldRecord{};
			D2HirelingTxt original_pOldRecord{};
			BOOL bExpansion{};
			int nAct{};
			int nDifficulty{};

			const auto setup_data = [](
				D2HirelingTxt& pOldRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pOldRecord);
			setup_data(original_pOldRecord);

			// Call both implementations
			const auto moo_result = sut(bExpansion, nAct, nDifficulty, &moo_pOldRecord);
			const auto original_result = original(bExpansion, nAct, nDifficulty, &original_pOldRecord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pOldRecord, original_pOldRecord, "Comparing pOldRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B4F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadNpcTxt, dll_base + 0x0002B4F0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B820 (#10588)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNpcTxtRecord, dll_base + 0x0002B820);
		
		SUBCASE("")
		{
			uint32_t dwNpcId{};

			// Call both implementations
			const auto moo_result = sut(dwNpcId);
			const auto original_result = original(dwNpcId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6B850" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonSoundsTxt, dll_base + 0x0002B850);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6BF50 (#11252)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonSoundsTxtRecordFromSoundId, dll_base + 0x0002BF50);
		
		SUBCASE("")
		{
			int nSoundId{};

			// Call both implementations
			const auto moo_result = sut(nSoundId);
			const auto original_result = original(nSoundId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6BF80 (#11250)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonSoundsTxtRecordFromMonsterId, dll_base + 0x0002BF80);
		
		SUBCASE("")
		{
			int nMonsterId{};

			// Call both implementations
			const auto moo_result = sut(nMonsterId);
			const auto original_result = original(nMonsterId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6BFE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MonStats2CompositLinker, dll_base + 0x0002BFE0);
		
		SUBCASE("")
		{
			// Input data
			char moo_pSrc{};
			char original_pSrc{};
			void* moo_pRecord = nullptr;
			void* original_pRecord = nullptr;
			int nOffset{};
			int nPosition{};
			int nTxtRow{};
			int nTxtColumn{};

			const auto setup_data = [](
				char& pSrc
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSrc);
			setup_data(original_pSrc);

			// Call both implementations
			sut(&moo_pSrc, moo_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);
			original(&original_pSrc, original_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSrc, original_pSrc, "Comparing pSrc");
			SKIP_MOO_CHECK_EQ(moo_pRecord, original_pRecord, "Comparing pRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6C150" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonStats2Txt, dll_base + 0x0002C150);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6D660" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CheckNestedMonsterTypes, dll_base + 0x0002D660);
		
		SUBCASE("")
		{
			int nMonType1{};
			int nMonType2{};

			// Call both implementations
			const auto moo_result = sut(nMonType1, nMonType2);
			const auto original_result = original(nMonType1, nMonType2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6D730" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonTypeTxt, dll_base + 0x0002D730);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6D910" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadMonTypeTxt, dll_base + 0x0002D910);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6D960" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonPropTxt, dll_base + 0x0002D960);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6E8E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonLvlTxt, dll_base + 0x0002E8E0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6EDE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MonPresetPlaceLinker, dll_base + 0x0002EDE0);
		
		SUBCASE("")
		{
			// Input data
			char moo_pSrc{};
			char original_pSrc{};
			void* moo_pRecord = nullptr;
			void* original_pRecord = nullptr;
			int nOffset{};
			int nPosition{};
			int nTxtRow{};
			int nTxtColumn{};

			const auto setup_data = [](
				char& pSrc
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSrc);
			setup_data(original_pSrc);

			// Call both implementations
			sut(&moo_pSrc, moo_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);
			original(&original_pSrc, original_pRecord, nOffset, nPosition, nTxtRow, nTxtColumn);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSrc, original_pSrc, "Comparing pSrc");
			SKIP_MOO_CHECK_EQ(moo_pRecord, original_pRecord, "Comparing pRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6EE60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonPresetTxt, dll_base + 0x0002EE60);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6EF30 (#11256)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonPresetTxtActSection, dll_base + 0x0002EF30);
		
		SUBCASE("")
		{
			// Input data
			int moo_pRecordCount{};
			int original_pRecordCount{};
			int nAct{};

			const auto setup_data = [](
				int& pRecordCount
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRecordCount);
			setup_data(original_pRecordCount);

			// Call both implementations
			const auto moo_result = sut(nAct, &moo_pRecordCount);
			const auto original_result = original(nAct, &original_pRecordCount);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRecordCount, original_pRecordCount, "Comparing pRecordCount");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6EF70 (#11258)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_MapSuperUniqueId, dll_base + 0x0002EF70);
		
		SUBCASE("")
		{
			int nType{};
			int nSuperUnique{};

			// Call both implementations
			const auto moo_result = sut(nType, nSuperUnique);
			const auto original_result = original(nType, nSuperUnique);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6EFA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadSoundsTxt, dll_base + 0x0002EFA0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6F020" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadSoundsTxt, dll_base + 0x0002F020);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6F230" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadMonEquipTxt, dll_base + 0x0002F230);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6F580" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadSomeMonsterTxts, dll_base + 0x0002F580);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6F810" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CreateItemTypeTreasureClasses, dll_base + 0x0002F810);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD6FBB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadSomeMonsterTxts, dll_base + 0x0002FBB0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
}
#endif
