#include <D2CommonTestDefines.h>

#ifdef SKILLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Skills.h>
#include <DataTbls/SkillsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Units/Units.h>


TEST_SUITE("D2SkillsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEB10 (#10938)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetPassiveState, dll_base + 0x0006EB10);
		
		SUBCASE("")
		{
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nSkillId);
			const auto original_result = original(nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAEB60 (#11271)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSpecialParamValue, dll_base + 0x0006EB60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			uint8_t nParamId{};
			int nSkillId{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nParamId, nSkillId, nSkillLevel);
			const auto original_result = original(&original_pUnit, nParamId, nSkillId, nSkillLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF6A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAF6A0, dll_base + 0x0006F6A0);
		
		SUBCASE("")
		{
			int32_t nParamId{};
			void* moo_pUserData = nullptr;
			void* original_pUserData = nullptr;

			// Call both implementations
			const auto moo_result = sut(nParamId, moo_pUserData);
			const auto original_result = original(nParamId, original_pUserData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUserData, original_pUserData, "Comparing pUserData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF6C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAF6C0, dll_base + 0x0006F6C0);
		
		SUBCASE("")
		{
			int nSkillId{};
			int nParamId{};
			int nUnused{};
			void* moo_pUserData = nullptr;
			void* original_pUserData = nullptr;

			// Call both implementations
			const auto moo_result = sut(nSkillId, nParamId, nUnused, moo_pUserData);
			const auto original_result = original(nSkillId, nParamId, nUnused, original_pUserData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUserData, original_pUserData, "Comparing pUserData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF780" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAF780, dll_base + 0x0006F780);
		
		SUBCASE("")
		{
			int nMissileId{};
			int nParamId{};
			int nUnused{};
			void* moo_pUserData = nullptr;
			void* original_pUserData = nullptr;

			// Call both implementations
			const auto moo_result = sut(nMissileId, nParamId, nUnused, moo_pUserData);
			const auto original_result = original(nMissileId, nParamId, nUnused, original_pUserData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUserData, original_pUserData, "Comparing pUserData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF7A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDAF7A0, dll_base + 0x0006F7A0);
		
		SUBCASE("")
		{
			int nSkillId{};
			int a2{};
			int a3{};
			void* moo_pUserData = nullptr;
			void* original_pUserData = nullptr;

			// Call both implementations
			const auto moo_result = sut(nSkillId, a2, a3, moo_pUserData);
			const auto original_result = original(nSkillId, a2, a3, original_pUserData);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUserData, original_pUserData, "Comparing pUserData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF7E0 (#11276)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_EvaluateSkillFormula, dll_base + 0x0006F7E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			unsigned int nCalc{};
			int nSkillId{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nCalc, nSkillId, nSkillLevel);
			const auto original_result = original(&original_pUnit, nCalc, nSkillId, nSkillLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF850 (#11302)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_EvaluateSkillDescFormula, dll_base + 0x0006F850);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			unsigned int nCalc{};
			int nSkillId{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nCalc, nSkillId, nSkillLevel);
			const auto original_result = original(&original_pUnit, nCalc, nSkillId, nSkillLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAF8C0 (#10940)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_RefreshSkill, dll_base + 0x0006F8C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId);
			original(&original_pUnit, nSkillId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFB40 (#10941)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_RefreshPassiveSkills, dll_base + 0x0006FB40);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFC30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetHighestLevelSkillFromSkillId, dll_base + 0x0006FC30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFC80 (#10942)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillModeFromUnit, dll_base + 0x0006FC80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFCA0 (#11049)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_Return1, dll_base + 0x0006FCA0);
		
		SUBCASE("")
		{
			int a1{};

			// Call both implementations
			const auto moo_result = sut(a1);
			const auto original_result = original(a1);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFCB0 (#10944)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetRange, dll_base + 0x0006FCB0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFCD0 (#10945)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_AllocSkillList, dll_base + 0x0006FCD0);
		
		SUBCASE("")
		{
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool);
			const auto original_result = original(original_pMemPool);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFD10 (#10946)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_InitSkillList, dll_base + 0x0006FD10);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFEA0 (#10947)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetNextSkill, dll_base + 0x0006FEA0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFEA0 (#10948)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetFirstSkillFromSkillList, dll_base + 0x0006FEA0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillListStrc moo_pSkillList{};
			D2SkillListStrc original_pSkillList{};

			const auto setup_data = [](
				D2SkillListStrc& pSkillList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkillList);
			setup_data(original_pSkillList);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkillList);
			const auto original_result = original(&original_pSkillList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkillList, original_pSkillList, "Comparing pSkillList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFEC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetLeftSkillFromSkillList, dll_base + 0x0006FEC0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillListStrc moo_pSkillList{};
			D2SkillListStrc original_pSkillList{};

			const auto setup_data = [](
				D2SkillListStrc& pSkillList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkillList);
			setup_data(original_pSkillList);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkillList);
			const auto original_result = original(&original_pSkillList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkillList, original_pSkillList, "Comparing pSkillList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFEF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetRightSkillFromSkillList, dll_base + 0x0006FEF0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillListStrc moo_pSkillList{};
			D2SkillListStrc original_pSkillList{};

			const auto setup_data = [](
				D2SkillListStrc& pSkillList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkillList);
			setup_data(original_pSkillList);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkillList);
			const auto original_result = original(&original_pSkillList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkillList, original_pSkillList, "Comparing pSkillList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFF20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetUsedSkillInSkillList, dll_base + 0x0006FF20);
		
		SUBCASE("")
		{
			// Input data
			D2SkillListStrc moo_pSkillList{};
			D2SkillStrc moo_pUsedSkill{};
			D2SkillListStrc original_pSkillList{};
			D2SkillStrc original_pUsedSkill{};

			const auto setup_data = [](
				D2SkillListStrc& pSkillList,
				D2SkillStrc& pUsedSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkillList, moo_pUsedSkill);
			setup_data(original_pSkillList, original_pUsedSkill);

			// Call both implementations
			sut(&moo_pSkillList, &moo_pUsedSkill);
			original(&original_pSkillList, &original_pUsedSkill);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkillList, original_pSkillList, "Comparing pSkillList");
			SKIP_MOO_CHECK_EQ(moo_pUsedSkill, original_pUsedSkill, "Comparing pUsedSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFF30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetUsedSkillFromSkillList, dll_base + 0x0006FF30);
		
		SUBCASE("")
		{
			// Input data
			D2SkillListStrc moo_pSkillList{};
			D2SkillListStrc original_pSkillList{};

			const auto setup_data = [](
				D2SkillListStrc& pSkillList
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkillList);
			setup_data(original_pSkillList);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkillList);
			const auto original_result = original(&original_pSkillList);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkillList, original_pSkillList, "Comparing pSkillList");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFF40 (#10949)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillById, dll_base + 0x0006FF40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			D2UnitGUID nOwnerGUID{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nOwnerGUID);
			const auto original_result = original(&original_pUnit, nSkillId, nOwnerGUID);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFF80 (#10950)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetHighestLevelSkillFromUnitAndId, dll_base + 0x0006FF80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFFD0 (#10951)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_RemoveSkill, dll_base + 0x0006FFD0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, &szFile, nLine);
			original(&original_pUnit, nSkillId, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAFFF0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_SKILLS_RemoveSkill_6FDAFFF0, dll_base + 0x0006FFF0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int bDecrementAndCheckSkillLevel{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, bDecrementAndCheckSkillLevel, &szFile, nLine);
			original(&original_pUnit, nSkillId, bDecrementAndCheckSkillLevel, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0270 (#10958)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10958, dll_base + 0x00070270);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			void* moo_a2 = nullptr;
			void* original_a2 = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, moo_a2);
			const auto original_result = original(&original_pUnit, original_a2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_a2, original_a2, "Comparing a2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB02A0 (#10959)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10959, dll_base + 0x000702A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB02C0 (#10960)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_FreeSkillList, dll_base + 0x000702C0);
		
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
			sut(&moo_pUnit);
			original(&original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0320 (#10952)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_AddSkill, dll_base + 0x00070320);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB04D0 (#10953)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_AssignSkill, dll_base + 0x000704D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			BOOL bRemove{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, nSkillLevel, bRemove, &szFile, nLine);
			original(&original_pUnit, nSkillId, nSkillLevel, bRemove, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB05E0 (#10954)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10954, dll_base + 0x000705E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			D2UnitGUID nOwnerGUID{};
			int nSkillId{};
			int nSkillLevel{};
			int nCharges{};
			BOOL bFreeMemory{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nOwnerGUID, nSkillId, nSkillLevel, nCharges, bFreeMemory);
			original(&original_pUnit, nOwnerGUID, nSkillId, nSkillLevel, nCharges, bFreeMemory);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB08C0 (#10957)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetOwnerGUIDFromSkill, dll_base + 0x000708C0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB08F0 (#10955)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillInfo, dll_base + 0x000708F0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2UnitGUID moo_pOwnerGUID{};
			int moo_pSkillId{};
			int moo_pSkillLevel{};
			int moo_pCharges{};
			D2SkillStrc original_pSkill{};
			D2UnitGUID original_pOwnerGUID{};
			int original_pSkillId{};
			int original_pSkillLevel{};
			int original_pCharges{};

			const auto setup_data = [](
				D2SkillStrc& pSkill,
				D2UnitGUID& pOwnerGUID,
				int& pSkillId,
				int& pSkillLevel,
				int& pCharges
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill, moo_pOwnerGUID, moo_pSkillId, moo_pSkillLevel, moo_pCharges);
			setup_data(original_pSkill, original_pOwnerGUID, original_pSkillId, original_pSkillLevel, original_pCharges);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill, &moo_pOwnerGUID, &moo_pSkillId, &moo_pSkillLevel, &moo_pCharges);
			const auto original_result = original(&original_pSkill, &original_pOwnerGUID, &original_pSkillId, &original_pSkillLevel, &original_pCharges);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
			SKIP_MOO_CHECK_EQ(moo_pOwnerGUID, original_pOwnerGUID, "Comparing pOwnerGUID");
			SKIP_MOO_CHECK_EQ(moo_pSkillId, original_pSkillId, "Comparing pSkillId");
			SKIP_MOO_CHECK_EQ(moo_pSkillLevel, original_pSkillLevel, "Comparing pSkillLevel");
			SKIP_MOO_CHECK_EQ(moo_pCharges, original_pCharges, "Comparing pCharges");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0960 (#10956)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetCharges, dll_base + 0x00070960);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nCharges{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill, nCharges);
			const auto original_result = original(&original_pSkill, nCharges);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB09A0 (#10961)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetLeftActiveSkill, dll_base + 0x000709A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			D2UnitGUID nOwnerGUID{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, nOwnerGUID);
			original(&original_pUnit, nSkillId, nOwnerGUID);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0A30 (#10962)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetRightActiveSkill, dll_base + 0x00070A30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			D2UnitGUID nOwnerGUID{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, nOwnerGUID);
			original(&original_pUnit, nSkillId, nOwnerGUID);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0AC0 (#10963)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillIdFromSkill, dll_base + 0x00070AC0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill, &szFile, nLine);
			const auto original_result = original(&original_pSkill, &szFile, nLine);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0AF0 (#10965)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSeqNumFromSkill, dll_base + 0x00070AF0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0B70 (#10964)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetUseState, dll_base + 0x00070B70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB0F50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_SKILLMANA_CheckStat_6FDB0F50, dll_base + 0x00070F50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1050" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB1050, dll_base + 0x00071050);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2InventoryStrc original_pInventory{};
			int nBodyLoc{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory);
			setup_data(original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, nBodyLoc);
			const auto original_result = original(&original_pInventory, nBodyLoc);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1070" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB1070, dll_base + 0x00071070);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1130" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDB1130, dll_base + 0x00071130);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_pUnit{};
			D2SkillsTxt moo_pSkillsTxtRecord{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_pUnit{};
			D2SkillsTxt original_pSkillsTxtRecord{};
			int nType{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2UnitStrc& pUnit,
				D2SkillsTxt& pSkillsTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pUnit, moo_pSkillsTxtRecord);
			setup_data(original_pItem, original_pUnit, original_pSkillsTxtRecord);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pUnit, &moo_pSkillsTxtRecord, nType);
			const auto original_result = original(&original_pItem, &original_pUnit, &original_pSkillsTxtRecord, nType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkillsTxtRecord, original_pSkillsTxtRecord, "Comparing pSkillsTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1380" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_SKILLS_CheckShapeRestriction_6FDB1380, dll_base + 0x00071380);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1400" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_SKILLMANA_CheckStartStat_6FDB1400, dll_base + 0x00071400);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1450" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CheckSkillDelay, dll_base + 0x00071450);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1540 (#10966)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillsTxtRecordFromSkill, dll_base + 0x00071540);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1550 (#10967)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetShrineSkillLevelBonus, dll_base + 0x00071550);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1580" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetBonusSkillLevel, dll_base + 0x00071580);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1700 (#10968)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillLevel, dll_base + 0x00071700);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};
			BOOL bBonus{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill, bBonus);
			const auto original_result = original(&original_pUnit, &original_pSkill, bBonus);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1750 (#11029)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetBonusSkillLevelFromSkillId, dll_base + 0x00071750);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB17A0 (#11030)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11030, dll_base + 0x000717A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int a3{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, a3);
			original(&original_pUnit, nSkillId, a3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1820 (#11031)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11031, dll_base + 0x00071820);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int a3{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, a3);
			original(&original_pUnit, nSkillId, a3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB18B0 (#10974)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetSkillMode, dll_base + 0x000718B0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nSkillMode{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nSkillMode);
			original(&original_pSkill, nSkillMode);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB18D0 (#10975)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkillMode, dll_base + 0x000718D0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB18F0 (#10969)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10969, dll_base + 0x000718F0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1920 (#10970)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10970, dll_base + 0x00071920);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nUnknown{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nUnknown);
			original(&original_pSkill, nUnknown);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1950 (#10971)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10971, dll_base + 0x00071950);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1980 (#10972)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10972, dll_base + 0x00071980);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nUnknown{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nUnknown);
			original(&original_pSkill, nUnknown);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB19B0 (#10973)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10973, dll_base + 0x000719B0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill);
			original(&original_pSkill);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB19F0 (#10976)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetParam1, dll_base + 0x000719F0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1A20 (#10977)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetParam2, dll_base + 0x00071A20);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1A50 (#10978)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetParam3, dll_base + 0x00071A50);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1A80 (#10979)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetParam4, dll_base + 0x00071A80);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1AB0 (#10980)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetParam1, dll_base + 0x00071AB0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nPar1{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nPar1);
			original(&original_pSkill, nPar1);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1AE0 (#10981)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetParam2, dll_base + 0x00071AE0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nPar2{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nPar2);
			original(&original_pSkill, nPar2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1B10 (#10982)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetParam3, dll_base + 0x00071B10);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nPar3{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nPar3);
			original(&original_pSkill, nPar3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1B40 (#10983)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetParam4, dll_base + 0x00071B40);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nPar4{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nPar4);
			original(&original_pSkill, nPar4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1B70 (#10984)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetFlags, dll_base + 0x00071B70);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nFlags{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nFlags);
			original(&original_pSkill, nFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1BA0 (#10985)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetFlags, dll_base + 0x00071BA0);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1BC0 (#10986)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetRequiredLevel, dll_base + 0x00071BC0);
		
		SUBCASE("")
		{
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nSkillId);
			const auto original_result = original(nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1C00 (#10987)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetRequiredLevelBasedOnCurrent, dll_base + 0x00071C00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1C80 (#10988)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CheckRequiredSkills, dll_base + 0x00071C80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1F80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetSkill, dll_base + 0x00071F80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			D2UnitGUID nOwnerGUID{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nOwnerGUID);
			const auto original_result = original(&original_pUnit, nSkillId, nOwnerGUID);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB1FC0 (#10989)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CheckRequiredAttributes, dll_base + 0x00071FC0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2110 (#10999)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetToHitFactor, dll_base + 0x00072110);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB21E0 (#11000)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetHighestSkillLevelById, dll_base + 0x000721E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2280 (#11001)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetManaCosts, dll_base + 0x00072280);
		
		SUBCASE("")
		{
			int nSkillId{};
			int nSkillLevel{};

			// Call both implementations
			const auto moo_result = sut(nSkillId, nSkillLevel);
			const auto original_result = original(nSkillId, nSkillLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB22E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CalculateDamageBonusByLevel, dll_base + 0x000722E0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pLevelDamage{};
			int original_pLevelDamage{};
			int nLevel{};

			const auto setup_data = [](
				int& pLevelDamage
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pLevelDamage);
			setup_data(original_pLevelDamage);

			// Call both implementations
			const auto moo_result = sut(nLevel, &moo_pLevelDamage);
			const auto original_result = original(nLevel, &original_pLevelDamage);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pLevelDamage, original_pLevelDamage, "Comparing pLevelDamage");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2390 (#11002)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetMinPhysDamage, dll_base + 0x00072390);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			BOOL a4{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel, a4);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel, a4);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB25D0 (#11003)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetMaxPhysDamage, dll_base + 0x000725D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			BOOL a4{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel, a4);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel, a4);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2810 (#11004)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetMinElemDamage, dll_base + 0x00072810);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			BOOL a4{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel, a4);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel, a4);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB29D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CalculateMasteryBonus, dll_base + 0x000729D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nElemType{};
			int nSrcDamage{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nElemType, nSrcDamage);
			const auto original_result = original(&original_pUnit, nElemType, nSrcDamage);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2B00 (#11005)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetMaxElemDamage, dll_base + 0x00072B00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			BOOL a4{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel, a4);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel, a4);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2CA0 (#11006)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetElementalLength, dll_base + 0x00072CA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			BOOL bUnused{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel, bUnused);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel, bUnused);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2E70 (#11239)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_Return0, dll_base + 0x00072E70);
		
		SUBCASE("")
		{
			int arg{};

			// Call both implementations
			const auto moo_result = sut(arg);
			const auto original_result = original(arg);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2E80 (#11008)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetClassIdFromSkillId, dll_base + 0x00072E80);
		
		SUBCASE("")
		{
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nSkillId);
			const auto original_result = original(nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2EC0 (#11010)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_IsPlayerClassSkill, dll_base + 0x00072EC0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pPlayerClass{};
			int original_pPlayerClass{};
			int nSkillId{};

			const auto setup_data = [](
				int& pPlayerClass
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayerClass);
			setup_data(original_pPlayerClass);

			// Call both implementations
			const auto moo_result = sut(nSkillId, &moo_pPlayerClass);
			const auto original_result = original(nSkillId, &original_pPlayerClass);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayerClass, original_pPlayerClass, "Comparing pPlayerClass");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2F40 (#11011)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetQuantity, dll_base + 0x00072F40);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pSkill);
			const auto original_result = original(&original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2F70 (#11012)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_SetQuantity, dll_base + 0x00072F70);
		
		SUBCASE("")
		{
			// Input data
			D2SkillStrc moo_pSkill{};
			D2SkillStrc original_pSkill{};
			int nQuantity{};

			const auto setup_data = [](
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pSkill);
			setup_data(original_pSkill);

			// Call both implementations
			sut(&moo_pSkill, nQuantity);
			original(&original_pSkill, nQuantity);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB2FA0 (#11014)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11014_ConvertShapeShiftedMode, dll_base + 0x00072FA0);
		
		SUBCASE("")
		{
			int nArrayIndex{};
			int nMonsterId{};

			// Call both implementations
			const auto moo_result = sut(nArrayIndex, nMonsterId);
			const auto original_result = original(nArrayIndex, nMonsterId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB30A0 (#11013)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11013_ConvertMode, dll_base + 0x000730A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pType{};
			int moo_pClass{};
			int moo_pMode{};
			D2UnitStrc original_pUnit{};
			int original_pType{};
			int original_pClass{};
			int original_pMode{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pType,
				int& pClass,
				int& pMode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pType, moo_pClass, moo_pMode);
			setup_data(original_pUnit, original_pType, original_pClass, original_pMode);

			// Call both implementations
			sut(&moo_pUnit, &moo_pType, &moo_pClass, &moo_pMode, &szFile, nLine);
			original(&original_pUnit, &original_pType, &original_pClass, &original_pMode, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pType, original_pType, "Comparing pType");
			SKIP_MOO_CHECK_EQ(moo_pClass, original_pClass, "Comparing pClass");
			SKIP_MOO_CHECK_EQ(moo_pMode, original_pMode, "Comparing pMode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3290 (#11015)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11015, dll_base + 0x00073290);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int a2{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, a2, nSkillId);
			original(&original_pUnit, a2, nSkillId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3340 (#11016)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11016, dll_base + 0x00073340);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSkill);
			setup_data(original_pUnit, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pSkill);
			const auto original_result = original(&original_pUnit, &original_pSkill);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB33A0 (#11017)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11017_CheckUnitIfConsumeable, dll_base + 0x000733A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3480 (#11020)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11020, dll_base + 0x00073480);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3520 (#11022)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CanUnitCorpseBeSelected, dll_base + 0x00073520);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB35B0 (#11024)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11024, dll_base + 0x000735B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2SkillStrc moo_pSkill{};
			BOOL moo_pHasThrowBonus{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2SkillStrc original_pSkill{};
			BOOL original_pHasThrowBonus{};
			int nType{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2SkillStrc& pSkill,
				BOOL& pHasThrowBonus
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_pSkill, moo_pHasThrowBonus);
			setup_data(original_pUnit, original_pItem, original_pSkill, original_pHasThrowBonus);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, &moo_pSkill, nType, &moo_pHasThrowBonus);
			const auto original_result = original(&original_pUnit, &original_pItem, &original_pSkill, nType, &original_pHasThrowBonus);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
			SKIP_MOO_CHECK_EQ(moo_pHasThrowBonus, original_pHasThrowBonus, "Comparing pHasThrowBonus");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB36D0 (#11023)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetWeaponMasteryBonus, dll_base + 0x000736D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2SkillStrc moo_pSkill{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			D2SkillStrc original_pSkill{};
			int nType{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem,
				D2SkillStrc& pSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem, moo_pSkill);
			setup_data(original_pUnit, original_pItem, original_pSkill);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, &moo_pSkill, nType);
			const auto original_result = original(&original_pUnit, &original_pItem, &original_pSkill, nType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pSkill, original_pSkill, "Comparing pSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB37B0 (#11032)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11032, dll_base + 0x000737B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nSkillLevel{};
			int nType{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId, nSkillLevel, nType);
			const auto original_result = original(&original_pUnit, nSkillId, nSkillLevel, nType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3910 (#11025)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11025, dll_base + 0x00073910);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};
			int a6{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(nX1, nY1, nX2, nY2, &moo_pRoom, a6);
			const auto original_result = original(nX1, nY1, nX2, nY2, &original_pRoom, a6);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3960 (#11026)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11026, dll_base + 0x00073960);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nX{};
			int nY{};
			uint16_t nColMask{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(nX, nY, &moo_pUnit, nColMask);
			const auto original_result = original(nX, nY, &original_pUnit, nColMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3A10 (#11027)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetShiftedManaCosts, dll_base + 0x00073A10);
		
		SUBCASE("")
		{
			int nSkillId{};
			int nLevel{};

			// Call both implementations
			const auto moo_result = sut(nSkillId, nLevel);
			const auto original_result = original(nSkillId, nLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3A90 (#11028)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11028, dll_base + 0x00073A90);
		
		SUBCASE("")
		{
			int a1{};

			// Call both implementations
			const auto moo_result = sut(a1);
			const auto original_result = original(a1);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3AB0 (#11033)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11033, dll_base + 0x00073AB0);
		
		SUBCASE("")
		{
			int nLevel{};
			int nParam{};
			int nMax{};

			// Call both implementations
			const auto moo_result = sut(nLevel, nParam, nMax);
			const auto original_result = original(nLevel, nParam, nMax);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3B00 (#11034)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11034, dll_base + 0x00073B00);
		
		SUBCASE("")
		{
			int nLevel{};
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nLevel, nSkillId);
			const auto original_result = original(nLevel, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3B90 (#11035)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11035, dll_base + 0x00073B90);
		
		SUBCASE("")
		{
			int nLevel{};
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nLevel, nSkillId);
			const auto original_result = original(nLevel, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3C20 (#11036)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11036_GetMonCurseResistanceSubtraction, dll_base + 0x00073C20);
		
		SUBCASE("")
		{
			int nLevel{};
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nLevel, nSkillId);
			const auto original_result = original(nLevel, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3CB0 (#11037)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CheckIfCanLeapTo, dll_base + 0x00073CB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			int moo_pX{};
			int moo_pY{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2, moo_pX, moo_pY);
			setup_data(original_pUnit1, original_pUnit2, original_pX, original_pY);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2, &moo_pX, &moo_pY);
			const auto original_result = original(&original_pUnit1, &original_pUnit2, &original_pX, &original_pY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB3F60 (#11039)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11039_CheckWeaponIsMissileBased, dll_base + 0x00073F60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pValue{};
			D2UnitStrc original_pUnit{};
			int original_pValue{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pValue
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pValue);
			setup_data(original_pUnit, original_pValue);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pValue);
			const auto original_result = original(&original_pUnit, &original_pValue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pValue, original_pValue, "Comparing pValue");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4020 (#11040)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_IsEnhanceable, dll_base + 0x00074020);
		
		SUBCASE("")
		{
			int nSkillId{};

			// Call both implementations
			const auto moo_result = sut(nSkillId);
			const auto original_result = original(nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4070 (#11230)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_RemoveTransformStatesFromShapeshiftedUnit, dll_base + 0x00074070);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4100 (#11041)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetClassSkillId, dll_base + 0x00074100);
		
		SUBCASE("")
		{
			int nClassId{};
			int nPosition{};

			// Call both implementations
			const auto moo_result = sut(nClassId, nPosition);
			const auto original_result = original(nClassId, nPosition);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4150 (#11042)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetPlayerSkillCount, dll_base + 0x00074150);
		
		SUBCASE("")
		{
			int nClassId{};

			// Call both implementations
			const auto moo_result = sut(nClassId);
			const auto original_result = original(nClassId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4180 (#11043)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11043, dll_base + 0x00074180);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB41D0 (#11047)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_GetConcentrationDamageBonus, dll_base + 0x000741D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSkillId);
			const auto original_result = original(&original_pUnit, nSkillId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB4260 (#11283)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(SKILLS_CalculateKickDamage, dll_base + 0x00074260);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pMinDamage{};
			int moo_pMaxDamage{};
			int moo_pDamagePercent{};
			D2UnitStrc original_pUnit{};
			int original_pMinDamage{};
			int original_pMaxDamage{};
			int original_pDamagePercent{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pMinDamage,
				int& pMaxDamage,
				int& pDamagePercent
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pMinDamage, moo_pMaxDamage, moo_pDamagePercent);
			setup_data(original_pUnit, original_pMinDamage, original_pMaxDamage, original_pDamagePercent);

			// Call both implementations
			sut(&moo_pUnit, &moo_pMinDamage, &moo_pMaxDamage, &moo_pDamagePercent);
			original(&original_pUnit, &original_pMinDamage, &original_pMaxDamage, &original_pDamagePercent);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pMinDamage, original_pMinDamage, "Comparing pMinDamage");
			SKIP_MOO_CHECK_EQ(moo_pMaxDamage, original_pMaxDamage, "Comparing pMaxDamage");
			SKIP_MOO_CHECK_EQ(moo_pDamagePercent, original_pDamagePercent, "Comparing pDamagePercent");
		}
	}
}
#endif
