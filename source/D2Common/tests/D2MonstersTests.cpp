#include <D2CommonTestDefines.h>

#ifdef MONSTERS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Monsters.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Units/Units.h>


TEST_SUITE("D2MonstersTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA4C10 (#11082)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_HirelingInit, dll_base + 0x00064C10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2HirelingInitStrc moo_pHirelingInit{};
			D2UnitStrc original_pMonster{};
			D2HirelingInitStrc original_pHirelingInit{};
			BOOL bExpansion{};
			int nLowSeed{};
			int nAct{};
			int nDifficulty{};

			const auto setup_data = [](
				D2UnitStrc& pMonster,
				D2HirelingInitStrc& pHirelingInit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster, moo_pHirelingInit);
			setup_data(original_pMonster, original_pHirelingInit);

			// Call both implementations
			const auto moo_result = sut(bExpansion, &moo_pMonster, nLowSeed, nAct, nDifficulty, &moo_pHirelingInit);
			const auto original_result = original(bExpansion, &original_pMonster, nLowSeed, nAct, nDifficulty, &original_pHirelingInit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			SKIP_MOO_CHECK_EQ(moo_pHirelingInit, original_pHirelingInit, "Comparing pHirelingInit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA4E20 (#11081)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11081, dll_base + 0x00064E20);
		
		SUBCASE("")
		{
			// Input data
			D2HirelingInitStrc moo_pHirelingInit{};
			D2HirelingInitStrc original_pHirelingInit{};
			int nLowSeed{};
			uint8_t a3{};

			const auto setup_data = [](
				D2HirelingInitStrc& pHirelingInit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHirelingInit);
			setup_data(original_pHirelingInit);

			// Call both implementations
			const auto moo_result = sut(nLowSeed, &moo_pHirelingInit, a3);
			const auto original_result = original(nLowSeed, &original_pHirelingInit, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHirelingInit, original_pHirelingInit, "Comparing pHirelingInit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA51A0 (#11085)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingDescString, dll_base + 0x000651A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA51C0 (#11086)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetActFromHirelingTxt, dll_base + 0x000651C0);
		
		SUBCASE("")
		{
			BOOL bExpansion{};
			int nClassId{};
			uint16_t nNameId{};

			// Call both implementations
			const auto moo_result = sut(bExpansion, nClassId, nNameId);
			const auto original_result = original(bExpansion, nClassId, nNameId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5200 (#11084)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingExpForNextLevel, dll_base + 0x00065200);
		
		SUBCASE("")
		{
			int nLevel{};
			int nExpPerLevel{};

			// Call both implementations
			const auto moo_result = sut(nLevel, nExpPerLevel);
			const auto original_result = original(nLevel, nExpPerLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5220 (#11083)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingResurrectionCost, dll_base + 0x00065220);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pHireling{};
			D2UnitStrc original_pHireling{};

			const auto setup_data = [](
				D2UnitStrc& pHireling
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHireling);
			setup_data(original_pHireling);

			// Call both implementations
			const auto moo_result = sut(&moo_pHireling);
			const auto original_result = original(&original_pHireling);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHireling, original_pHireling, "Comparing pHireling");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5270 (#11068)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11068_GetCompInfo, dll_base + 0x00065270);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};
			int nComponent{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster, nComponent);
			const auto original_result = original(&original_pMonster, nComponent);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA52F0 (#11069)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11069, dll_base + 0x000652F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};
			unsigned int nIndex{};
			unsigned int nComponent{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster, nIndex, nComponent);
			const auto original_result = original(&original_pMonster, nIndex, nComponent);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5450 (#11070)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11070, dll_base + 0x00065450);
		
		SUBCASE("")
		{
			int nMonsterId{};
			unsigned int nComponent{};
			unsigned int a3{};

			// Call both implementations
			const auto moo_result = sut(nMonsterId, nComponent, a3);
			const auto original_result = original(nMonsterId, nComponent, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA54E0 (#11050)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11050, dll_base + 0x000654E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int a2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, a2);
			const auto original_result = original(&original_pUnit, a2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA55E0 (#11052)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11052, dll_base + 0x000655E0);
		
		SUBCASE("")
		{
			uint8_t a1{};

			// Call both implementations
			const auto moo_result = sut(a1);
			const auto original_result = original(a1);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5600 (#11053)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11053, dll_base + 0x00065600);
		
		SUBCASE("")
		{
			uint8_t a1{};

			// Call both implementations
			const auto moo_result = sut(a1);
			const auto original_result = original(a1);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5620 (#11054)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11054, dll_base + 0x00065620);
		
		SUBCASE("")
		{
			uint8_t a1{};

			// Call both implementations
			const auto moo_result = sut(a1);
			const auto original_result = original(a1);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5640 (#11055)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11055, dll_base + 0x00065640);
		
		SUBCASE("")
		{
			// Input data
			int moo_a2{};
			int moo_a3{};
			int original_a2{};
			int original_a3{};
			uint8_t a1{};

			const auto setup_data = [](
				int& a2,
				int& a3
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a2, moo_a3);
			setup_data(original_a2, original_a3);

			// Call both implementations
			sut(a1, &moo_a2, &moo_a3);
			original(a1, &original_a2, &original_a3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5670 (#11297)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_SetMonsterNameInMonsterData, dll_base + 0x00065670);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};
			Unicode wszName{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			sut(&moo_pMonster, &wszName);
			original(&original_pMonster, &wszName);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA56C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_CanBeInTown, dll_base + 0x000656C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster);
			const auto original_result = original(&original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5750 (#11057)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsSandLeaper, dll_base + 0x00065750);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};
			BOOL bAlwaysReturnFalse{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster, bAlwaysReturnFalse);
			const auto original_result = original(&original_pMonster, bAlwaysReturnFalse);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA57D0 (#11058)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsDemon, dll_base + 0x000657D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster);
			const auto original_result = original(&original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5830 (#11059)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsUndead, dll_base + 0x00065830);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster);
			const auto original_result = original(&original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA58A0 (#11060)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsBoss, dll_base + 0x000658A0);
		
		SUBCASE("")
		{
			// Input data
			D2MonStatsTxt moo_pMonStatsTxtRecord{};
			D2UnitStrc moo_pMonster{};
			D2MonStatsTxt original_pMonStatsTxtRecord{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2MonStatsTxt& pMonStatsTxtRecord,
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonStatsTxtRecord, moo_pMonster);
			setup_data(original_pMonStatsTxtRecord, original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonStatsTxtRecord, &moo_pMonster);
			const auto original_result = original(&original_pMonStatsTxtRecord, &original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonStatsTxtRecord, original_pMonStatsTxtRecord, "Comparing pMonStatsTxtRecord");
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5900 (#11064)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsDead, dll_base + 0x00065900);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster);
			const auto original_result = original(&original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5930 (#11280)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetSpawnMode_XY, dll_base + 0x00065930);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			int moo_pSpawnMode{};
			int moo_pX{};
			int moo_pY{};
			D2UnitStrc original_pMonster{};
			int original_pSpawnMode{};
			int original_pX{};
			int original_pY{};
			BOOL bFromMonster{};
			int nSkillId{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2UnitStrc& pMonster,
				int& pSpawnMode,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster, moo_pSpawnMode, moo_pX, moo_pY);
			setup_data(original_pMonster, original_pSpawnMode, original_pX, original_pY);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster, bFromMonster, nSkillId, nSkillLevel, &moo_pSpawnMode, &moo_pX, &moo_pY);
			const auto original_result = original(&original_pMonster, bFromMonster, nSkillId, nSkillLevel, &original_pSpawnMode, &original_pX, &original_pY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			SKIP_MOO_CHECK_EQ(moo_pSpawnMode, original_pSpawnMode, "Comparing pSpawnMode");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA5B30 (#11061)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetMinionSpawnInfo, dll_base + 0x00065B30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			int moo_pId{};
			int moo_pX{};
			int moo_pY{};
			int moo_pSpawnMode{};
			D2UnitStrc original_pMonster{};
			int original_pId{};
			int original_pX{};
			int original_pY{};
			int original_pSpawnMode{};
			int nDifficulty{};

			const auto setup_data = [](
				D2UnitStrc& pMonster,
				int& pId,
				int& pX,
				int& pY,
				int& pSpawnMode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster, moo_pId, moo_pX, moo_pY, moo_pSpawnMode);
			setup_data(original_pMonster, original_pId, original_pX, original_pY, original_pSpawnMode);

			// Call both implementations
			sut(&moo_pMonster, &moo_pId, &moo_pX, &moo_pY, &moo_pSpawnMode, nDifficulty, nullptr);
			original(&original_pMonster, &original_pId, &original_pX, &original_pY, &original_pSpawnMode, nDifficulty, nullptr);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			SKIP_MOO_CHECK_EQ(moo_pId, original_pId, "Comparing pId");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
			SKIP_MOO_CHECK_EQ(moo_pSpawnMode, original_pSpawnMode, "Comparing pSpawnMode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6410 (#11051)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetMaximalLightRadius, dll_base + 0x00066410);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster);
			const auto original_result = original(&original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA64B0 (#11063)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11063, dll_base + 0x000664B0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nMonsterId{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nMonsterId);
			const auto original_result = original(&original_pRoom, nMonsterId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6620 (#11065)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_IsPrimeEvil, dll_base + 0x00066620);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			const auto moo_result = sut(&moo_pMonster);
			const auto original_result = original(&original_pMonster);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6680 (#11066)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11066, dll_base + 0x00066680);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			int moo_pDirectionX{};
			int moo_pDirectionY{};
			D2UnitStrc original_pMonster{};
			int original_pDirectionX{};
			int original_pDirectionY{};

			const auto setup_data = [](
				D2UnitStrc& pMonster,
				int& pDirectionX,
				int& pDirectionY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster, moo_pDirectionX, moo_pDirectionY);
			setup_data(original_pMonster, original_pDirectionX, original_pDirectionY);

			// Call both implementations
			sut(&moo_pMonster, &moo_pDirectionX, &moo_pDirectionY);
			original(&original_pMonster, &original_pDirectionX, &original_pDirectionY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
			SKIP_MOO_CHECK_EQ(moo_pDirectionX, original_pDirectionX, "Comparing pDirectionX");
			SKIP_MOO_CHECK_EQ(moo_pDirectionY, original_pDirectionY, "Comparing pDirectionY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6730 (#11067)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetHirelingTypeId, dll_base + 0x00066730);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pHireling{};
			D2UnitStrc original_pHireling{};

			const auto setup_data = [](
				D2UnitStrc& pHireling
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pHireling);
			setup_data(original_pHireling);

			// Call both implementations
			const auto moo_result = sut(&moo_pHireling);
			const auto original_result = original(&original_pHireling);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pHireling, original_pHireling, "Comparing pHireling");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6790 (#11246)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_ApplyClassicScaling, dll_base + 0x00066790);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMonster{};
			D2UnitStrc original_pMonster{};
			BOOL bExpansion{};
			uint8_t nDifficulty{};

			const auto setup_data = [](
				D2UnitStrc& pMonster
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMonster);
			setup_data(original_pMonster);

			// Call both implementations
			sut(&moo_pMonster, bExpansion, nDifficulty);
			original(&original_pMonster, bExpansion, nDifficulty);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMonster, original_pMonster, "Comparing pMonster");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6920" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetBaseIdFromMonsterId, dll_base + 0x00066920);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6950" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_GetClassIdFromMonsterChain, dll_base + 0x00066950);
		
		SUBCASE("")
		{
			int nMonsterId{};
			int nChainId{};

			// Call both implementations
			const auto moo_result = sut(nMonsterId, nChainId);
			const auto original_result = original(nMonsterId, nChainId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA69C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MONSTERS_ValidateMonsterId, dll_base + 0x000669C0);
		
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
}
#endif
