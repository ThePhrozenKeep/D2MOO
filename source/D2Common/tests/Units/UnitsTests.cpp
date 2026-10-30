#include <D2CommonTestDefines.h>

#ifdef UNITS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Skills.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <GAME/Event.h>
#include <Units/Units.h>


TEST_SUITE("UnitsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD520 (#10457)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetDirection, dll_base + 0x0007D520);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD570 (#10320)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetStartSkill, dll_base + 0x0007D570);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD5B0 (#10321)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetLeftSkill, dll_base + 0x0007D5B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD5F0 (#10322)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetRightSkill, dll_base + 0x0007D5F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD630 (#10324)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetUsedSkill, dll_base + 0x0007D630);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2SkillStrc moo_pUsedSkill{};
			D2UnitStrc original_pUnit{};
			D2SkillStrc original_pUsedSkill{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2SkillStrc& pUsedSkill
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pUsedSkill);
			setup_data(original_pUnit, original_pUsedSkill);

			// Call both implementations
			sut(&moo_pUnit, &moo_pUsedSkill);
			original(&original_pUnit, &original_pUsedSkill);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pUsedSkill, original_pUsedSkill, "Comparing pUsedSkill");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD670 (#10323)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetUsedSkill, dll_base + 0x0007D670);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD6B0 (#11259)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_AllocUnit, dll_base + 0x0007D6B0);
		
		SUBCASE("")
		{
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nUnitType{};

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, nUnitType);
			const auto original_result = original(original_pMemPool, nUnitType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD720 (#11260)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_FreeUnit, dll_base + 0x0007D720);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD780 (#10327)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetPrecisionX, dll_base + 0x0007D780);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD7D0 (#10330)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetPrecisionY, dll_base + 0x0007D7D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD820 (#10328)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetXForStaticUnit, dll_base + 0x0007D820);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nX{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nX);
			original(&original_pUnit, nX);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD890 (#10331)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetYForStaticUnit, dll_base + 0x0007D890);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nY);
			original(&original_pUnit, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBD900 (#10336)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetUnitSizeX, dll_base + 0x0007D900);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDA00 (#10337)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetUnitSizeY, dll_base + 0x0007DA00);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDB10 (#10333)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetClientCoordX, dll_base + 0x0007DB10);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDB60 (#10334)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetClientCoordY, dll_base + 0x0007DB60);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDBB0 (#10411)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetAbsoluteXDistance, dll_base + 0x0007DBB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2);
			const auto original_result = original(&original_pUnit1, &original_pUnit2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDC20 (#10412)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetAbsoluteYDistance, dll_base + 0x0007DC20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2);
			const auto original_result = original(&original_pUnit1, &original_pUnit2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDC90 (#10340)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetTargetX, dll_base + 0x0007DC90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nTargetX{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nTargetX);
			original(&original_pUnit, nTargetX);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDCD0 (#10341)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetTargetY, dll_base + 0x0007DCD0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nTargetY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nTargetY);
			original(&original_pUnit, nTargetY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDD10 (#10332)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetCoords, dll_base + 0x0007DD10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2CoordStrc moo_pCoord{};
			D2UnitStrc original_pUnit{};
			D2CoordStrc original_pCoord{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2CoordStrc& pCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pCoord);
			setup_data(original_pUnit, original_pCoord);

			// Call both implementations
			sut(&moo_pUnit, &moo_pCoord);
			original(&original_pUnit, &original_pCoord);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pCoord, original_pCoord, "Comparing pCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDDA0 (#10335)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetClientCoords, dll_base + 0x0007DDA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2CoordStrc moo_pClientCoords{};
			D2UnitStrc original_pUnit{};
			D2CoordStrc original_pClientCoords{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2CoordStrc& pClientCoords
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pClientCoords);
			setup_data(original_pUnit, original_pClientCoords);

			// Call both implementations
			sut(&moo_pUnit, &moo_pClientCoords);
			original(&original_pUnit, &original_pClientCoords);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pClientCoords, original_pClientCoords, "Comparing pClientCoords");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDE10 (#10338)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetCollisionMask, dll_base + 0x0007DE10);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBDEC0 (#10352)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_FreeCollisionPath, dll_base + 0x0007DEC0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE060 (#10351)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_BlockCollisionPath, dll_base + 0x0007E060);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc original_pUnit{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pRoom);
			setup_data(original_pUnit, original_pRoom);

			// Call both implementations
			sut(&moo_pUnit, &moo_pRoom, nX, nY);
			original(&original_pUnit, &original_pRoom, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE1A0 (#10350)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_InitializeStaticPath, dll_base + 0x0007E1A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc original_pUnit{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pRoom);
			setup_data(original_pUnit, original_pRoom);

			// Call both implementations
			sut(&moo_pUnit, &moo_pRoom, nX, nY);
			original(&original_pUnit, &original_pRoom, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE210 (#10343)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_ResetRoom, dll_base + 0x0007E210);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE270 (#10342)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetRoom, dll_base + 0x0007E270);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE2D0 (#10344)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetTargetUnitForDynamicUnit, dll_base + 0x0007E2D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pTargetUnit{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pTargetUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pTargetUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pTargetUnit);
			setup_data(original_pUnit, original_pTargetUnit);

			// Call both implementations
			sut(&moo_pUnit, &moo_pTargetUnit);
			original(&original_pUnit, &original_pTargetUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pTargetUnit, original_pTargetUnit, "Comparing pTargetUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE330 (#10345)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetTargetTypeFromDynamicUnit, dll_base + 0x0007E330);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE3A0 (#10346)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetTargetGUIDFromDynamicUnit, dll_base + 0x0007E3A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE410 (#10347)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetTargetUnitForPlayerOrMonster, dll_base + 0x0007E410);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pTargetUnit{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pTargetUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pTargetUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pTargetUnit);
			setup_data(original_pUnit, original_pTargetUnit);

			// Call both implementations
			sut(&moo_pUnit, &moo_pTargetUnit);
			original(&original_pUnit, &original_pTargetUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pTargetUnit, original_pTargetUnit, "Comparing pTargetUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE470 (#10354)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetRunAndWalkSpeedForPlayer, dll_base + 0x0007E470);
		
		SUBCASE("")
		{
			// Input data
			int moo_pWalkSpeed{};
			int moo_pRunSpeed{};
			int original_pWalkSpeed{};
			int original_pRunSpeed{};
			int nUnused{};
			int nCharId{};

			const auto setup_data = [](
				int& pWalkSpeed,
				int& pRunSpeed
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pWalkSpeed, moo_pRunSpeed);
			setup_data(original_pWalkSpeed, original_pRunSpeed);

			// Call both implementations
			sut(nUnused, nCharId, &moo_pWalkSpeed, &moo_pRunSpeed);
			original(nUnused, nCharId, &original_pWalkSpeed, &original_pRunSpeed);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pWalkSpeed, original_pWalkSpeed, "Comparing pWalkSpeed");
			SKIP_MOO_CHECK_EQ(moo_pRunSpeed, original_pRunSpeed, "Comparing pRunSpeed");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE4C0 (#10325)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetAnimData, dll_base + 0x0007E4C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nUnitType{};
			int nClassId{};
			int nMode{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nUnitType, nClassId, nMode);
			original(&original_pUnit, nUnitType, nClassId, nMode);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBE510" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetAnimStartFrame, dll_base + 0x0007E510);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEA60 (#10348)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_ChangeAnimMode, dll_base + 0x0007EA60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nMode{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nMode);
			const auto original_result = original(&original_pUnit, nMode);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEAD0 (#10355)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsCurrentRoomInvalid, dll_base + 0x0007EAD0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEB20 (#10356)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetCurrentRoomInvalid, dll_base + 0x0007EB20);
		
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
			sut(&moo_pUnit, a2);
			original(&original_pUnit, a2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEB80 (#10357)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_RefreshInventory, dll_base + 0x0007EB80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL bSetFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, bSetFlag);
			original(&original_pUnit, bSetFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEBE0 (#10409)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetInventoryRecordId, dll_base + 0x0007EBE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nInvPage{};
			BOOL bLoD{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nInvPage, bLoD);
			const auto original_result = original(&original_pUnit, nInvPage, bLoD);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBECD0 (#10383)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_ResetLightMap, dll_base + 0x0007ECD0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBED10 (#10369)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetAnimOrSeqMode, dll_base + 0x0007ED10);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBED40 (#10370)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetAnimOrSeqMode, dll_base + 0x0007ED40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nAnimMode{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nAnimMode);
			original(&original_pUnit, nAnimMode);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBED90 (#10371)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_InitializeSequence, dll_base + 0x0007ED90);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEE20 (#10372)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetAnimationFrame, dll_base + 0x0007EE20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nFrame{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nFrame);
			original(&original_pUnit, nFrame);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEE60 (#10373)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_StopSequence, dll_base + 0x0007EE60);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBEFF0 (#10374)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_UpdateFrame, dll_base + 0x0007EFF0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBF020 (#10375)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10375_UNITS_SetFrameNonRate, dll_base + 0x0007F020);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nRate{};
			int nFailRate{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nRate, nFailRate);
			original(&original_pUnit, nRate, nFailRate);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBF050 (#10376)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_UpdateAnimRateAndVelocity, dll_base + 0x0007F050);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
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
			sut(&moo_pUnit, &szFile, nLine);
			original(&original_pUnit, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBF8D0 (#10377)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetAnimationSpeed, dll_base + 0x0007F8D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSpeed{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSpeed);
			original(&original_pUnit, nSpeed);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBF910 (#10378)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsAtEndOfFrameCycle, dll_base + 0x0007F910);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBF970 (#10379)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetShiftedFrameMetrics, dll_base + 0x0007F970);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pFrameNo{};
			int moo_pFrameCount{};
			D2UnitStrc original_pUnit{};
			int original_pFrameNo{};
			int original_pFrameCount{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pFrameNo,
				int& pFrameCount
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pFrameNo, moo_pFrameCount);
			setup_data(original_pUnit, original_pFrameNo, original_pFrameCount);

			// Call both implementations
			sut(&moo_pUnit, &moo_pFrameNo, &moo_pFrameCount);
			original(&original_pUnit, &original_pFrameNo, &original_pFrameCount);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pFrameNo, original_pFrameNo, "Comparing pFrameNo");
			SKIP_MOO_CHECK_EQ(moo_pFrameCount, original_pFrameCount, "Comparing pFrameCount");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBF9E0 (#10380)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetFrameMetrics, dll_base + 0x0007F9E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pFrame{};
			int moo_pFrameCount{};
			D2UnitStrc original_pUnit{};
			int original_pFrame{};
			int original_pFrameCount{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pFrame,
				int& pFrameCount
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pFrame, moo_pFrameCount);
			setup_data(original_pUnit, original_pFrame, original_pFrameCount);

			// Call both implementations
			sut(&moo_pUnit, &moo_pFrame, &moo_pFrameCount);
			original(&original_pUnit, &original_pFrame, &original_pFrameCount);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pFrame, original_pFrame, "Comparing pFrame");
			SKIP_MOO_CHECK_EQ(moo_pFrameCount, original_pFrameCount, "Comparing pFrameCount");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFA40 (#10381)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetAnimActionFrame, dll_base + 0x0007FA40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nFrame{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nFrame);
			original(&original_pUnit, nFrame);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFA90 (#10382)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetEventFrameInfo, dll_base + 0x0007FA90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nFrame{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nFrame);
			const auto original_result = original(&original_pUnit, nFrame);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFB40 (#10410)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_HasCollision, dll_base + 0x0007FB40);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFB70 (#10358)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetSkillFromSkillId, dll_base + 0x0007FB70);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFC10 (#10392)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsDoor, dll_base + 0x0007FC10);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFC50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_CheckIfObjectOrientationIs1, dll_base + 0x0007FC50);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFC90 (#10393)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsShrine, dll_base + 0x0007FC90);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFCB0 (#10394)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetObjectTxtRecordFromObject, dll_base + 0x0007FCB0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFD00 (#10395)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetShrineTxtRecordFromObject, dll_base + 0x0007FD00);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFD50 (#10396)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetShrineTxtRecordInObjectData, dll_base + 0x0007FD50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2ShrinesTxt moo_pShrinesTxtRecord{};
			D2UnitStrc original_pUnit{};
			D2ShrinesTxt original_pShrinesTxtRecord{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2ShrinesTxt& pShrinesTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pShrinesTxtRecord);
			setup_data(original_pUnit, original_pShrinesTxtRecord);

			// Call both implementations
			sut(&moo_pUnit, &moo_pShrinesTxtRecord);
			original(&original_pUnit, &original_pShrinesTxtRecord);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pShrinesTxtRecord, original_pShrinesTxtRecord, "Comparing pShrinesTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFDB0 (#10413)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_UpdateDirectionAndSpeed, dll_base + 0x0007FDB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nX, nY);
			original(&original_pUnit, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFDD0 (#10414)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetNewDirection, dll_base + 0x0007FDD0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFF20 (#10416)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_StoreOwnerTypeAndGUID, dll_base + 0x0007FF20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nOwnerType{};
			D2UnitGUID nOwnerId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nOwnerType, nOwnerId);
			original(&original_pUnit, nOwnerType, nOwnerId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFF40" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_StoreOwnerInfo, dll_base + 0x0007FF40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nOwnerType{};
			int nOwnerId{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nOwnerType, nOwnerId);
			original(&original_pUnit, nOwnerType, nOwnerId);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBFFE0 (#10415)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_StoreOwner, dll_base + 0x0007FFE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pOwner{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pOwner{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pOwner
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pOwner);
			setup_data(original_pUnit, original_pOwner);

			// Call both implementations
			sut(&moo_pUnit, &moo_pOwner);
			original(&original_pUnit, &original_pOwner);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pOwner, original_pOwner, "Comparing pOwner");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0060 (#10417)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_StoreLastAttacker, dll_base + 0x00080060);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pKiller{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pKiller{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pKiller
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pKiller);
			setup_data(original_pUnit, original_pKiller);

			// Call both implementations
			sut(&moo_pUnit, &moo_pKiller);
			original(&original_pUnit, &original_pKiller);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pKiller, original_pKiller, "Comparing pKiller");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC00E0 (#10418)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetDirectionToCoords, dll_base + 0x000800E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nNewX{};
			int nNewY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nNewX, nNewY);
			const auto original_result = original(&original_pUnit, nNewX, nNewY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0160 (#10437)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetOverlay, dll_base + 0x00080160);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nOverlay{};
			int nUnused{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nOverlay, nUnused);
			original(&original_pUnit, nOverlay, nUnused);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC01F0 (#10367)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetBeltType, dll_base + 0x000801F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0260 (#10368)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetCurrentLifePercentage, dll_base + 0x00080260);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC02A0 (#10359)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsSoftMonster, dll_base + 0x000802A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0320 (#10420)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_AllocPlayerData, dll_base + 0x00080320);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC03F0 (#10421)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_FreePlayerData, dll_base + 0x000803F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc original_pPlayer{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pPlayer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer);
			setup_data(original_pPlayer);

			// Call both implementations
			sut(moo_pMemPool, &moo_pPlayer);
			original(original_pMemPool, &original_pPlayer);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC04A0 (#10422)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetNameInPlayerData, dll_base + 0x000804A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			char moo_szName{};
			D2UnitStrc original_pUnit{};
			char original_szName{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				char& szName
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_szName);
			setup_data(original_pUnit, original_szName);

			// Call both implementations
			sut(&moo_pUnit, &moo_szName);
			original(&original_pUnit, &original_szName);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_szName, original_szName, "Comparing szName");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0530 (#10423)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetPlayerName, dll_base + 0x00080530);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC05B0 (#10424)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetPlayerData, dll_base + 0x000805B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0600 (#10425)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetPlayerPortalFlags, dll_base + 0x00080600);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nPortalFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nPortalFlags);
			original(&original_pUnit, nPortalFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0660 (#10426)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetPlayerPortalFlags, dll_base + 0x00080660);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC06C0 (#10353)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetNameOffsetFromObject, dll_base + 0x000806C0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0700 (#10427)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetObjectPortalFlags, dll_base + 0x00080700);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0760 (#10428)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetObjectPortalFlags, dll_base + 0x00080760);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			uint8_t nPortalFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nPortalFlag);
			original(&original_pUnit, nPortalFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC07C0 (#10429)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_CheckObjectPortalFlag, dll_base + 0x000807C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			uint8_t nFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nFlag);
			const auto original_result = original(&original_pUnit, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0820 (#10430)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetOverlayHeight, dll_base + 0x00080820);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC08B0 (#10431)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetDefense, dll_base + 0x000808B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0AC0 (#10432)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetAttackRate, dll_base + 0x00080AC0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pAttacker{};
			D2UnitStrc original_pAttacker{};

			const auto setup_data = [](
				D2UnitStrc& pAttacker
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAttacker);
			setup_data(original_pAttacker);

			// Call both implementations
			const auto moo_result = sut(&moo_pAttacker);
			const auto original_result = original(&original_pAttacker);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAttacker, original_pAttacker, "Comparing pAttacker");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0B60 (#10433)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetBlockRate, dll_base + 0x00080B60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL bExpansion{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, bExpansion);
			const auto original_result = original(&original_pUnit, bExpansion);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0DA0 (#10434)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10434, dll_base + 0x00080DA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL a2{};

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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0F70 (#10435)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetEquippedWeaponFromMonster, dll_base + 0x00080F70);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC0FC0 (#10436)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetFrameBonus, dll_base + 0x00080FC0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1120 (#10360)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetMeleeRange, dll_base + 0x00081120);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1230 (#10364)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_TestCollisionByCoordinates, dll_base + 0x00081230);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nX{};
			int nY{};
			int nFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nX, nY, nFlags);
			const auto original_result = original(&original_pUnit, nX, nY, nFlags);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC13D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_TestCollision, dll_base + 0x000813D0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX1{};
			int nY1{};
			int nSize1{};
			int nX2{};
			int nY2{};
			int nSize2{};
			int nCollisionMask{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(nX1, nY1, nSize1, nX2, nY2, nSize2, &moo_pRoom, nCollisionMask);
			const auto original_result = original(nX1, nY1, nSize1, nX2, nY2, nSize2, &original_pRoom, nCollisionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC14C0 (#10362)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_TestCollisionWithUnit, dll_base + 0x000814C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};
			int nCollisionMask{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2, nCollisionMask);
			const auto original_result = original(&original_pUnit1, &original_pUnit2, nCollisionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1760" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_ToggleUnitFlag, dll_base + 0x00081760);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nFlag{};
			BOOL bSet{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nFlag, bSet);
			original(&original_pUnit, nFlag, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1790 (#10363)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_TestCollisionBetweenInteractingUnits, dll_base + 0x00081790);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};
			int nCollisionMask{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2, nCollisionMask);
			const auto original_result = original(&original_pUnit1, &original_pUnit2, nCollisionMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1A70 (#10361)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsInMeleeRange, dll_base + 0x00081A70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};
			int nRangeBonus{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2, nRangeBonus);
			const auto original_result = original(&original_pUnit1, &original_pUnit2, nRangeBonus);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1B40 (#10318)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsInMovingMode, dll_base + 0x00081B40);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1C30 (#10319)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsInMovingModeEx, dll_base + 0x00081C30);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1C50 (#10365)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetHitClass, dll_base + 0x00081C50);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1CE0 (#10366)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetWeaponClass, dll_base + 0x00081CE0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1D00 (#10438)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetHealingCost, dll_base + 0x00081D00);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1D90 (#10439)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetInventoryGoldLimit, dll_base + 0x00081D90);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1DB0 (#10440)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_MergeDualWieldWeaponStatLists, dll_base + 0x00081DB0);
		
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
			sut(&moo_pUnit, a2);
			original(&original_pUnit, a2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1EE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetMonStats2TxtRecord, dll_base + 0x00081EE0);
		
		SUBCASE("")
		{
			int nRecordId{};

			// Call both implementations
			const auto moo_result = sut(nRecordId);
			const auto original_result = original(nRecordId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1F10 (#10442)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetItemComponentId, dll_base + 0x00081F10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem);
			const auto original_result = original(&original_pUnit, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC1FE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetMonStats2TxtRecordFromMonsterId, dll_base + 0x00081FE0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2030 (#10443)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_InitRightSkill, dll_base + 0x00082030);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC20A0 (#10444)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_InitLeftSkill, dll_base + 0x000820A0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2110 (#10445)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_InitSwitchRightSkill, dll_base + 0x00082110);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2180 (#10446)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_InitSwitchLeftSkill, dll_base + 0x00082180);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC21F0 (#10447)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetRightSkillData, dll_base + 0x000821F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pRightSkillId{};
			int moo_pRightSkillFlags{};
			D2UnitStrc original_pUnit{};
			int original_pRightSkillId{};
			int original_pRightSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pRightSkillId,
				int& pRightSkillFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pRightSkillId, moo_pRightSkillFlags);
			setup_data(original_pUnit, original_pRightSkillId, original_pRightSkillFlags);

			// Call both implementations
			sut(&moo_pUnit, &moo_pRightSkillId, &moo_pRightSkillFlags);
			original(&original_pUnit, &original_pRightSkillId, &original_pRightSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pRightSkillId, original_pRightSkillId, "Comparing pRightSkillId");
			SKIP_MOO_CHECK_EQ(moo_pRightSkillFlags, original_pRightSkillFlags, "Comparing pRightSkillFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2250 (#10448)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetLeftSkillData, dll_base + 0x00082250);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pLeftSkillId{};
			int moo_pLeftSkillFlags{};
			D2UnitStrc original_pUnit{};
			int original_pLeftSkillId{};
			int original_pLeftSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pLeftSkillId,
				int& pLeftSkillFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pLeftSkillId, moo_pLeftSkillFlags);
			setup_data(original_pUnit, original_pLeftSkillId, original_pLeftSkillFlags);

			// Call both implementations
			sut(&moo_pUnit, &moo_pLeftSkillId, &moo_pLeftSkillFlags);
			original(&original_pUnit, &original_pLeftSkillId, &original_pLeftSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pLeftSkillId, original_pLeftSkillId, "Comparing pLeftSkillId");
			SKIP_MOO_CHECK_EQ(moo_pLeftSkillFlags, original_pLeftSkillFlags, "Comparing pLeftSkillFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC22B0 (#10449)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetSwitchRightSkillDataResetRightSkill, dll_base + 0x000822B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pSwitchRightSkillId{};
			int moo_pSwitchRightSkillFlags{};
			D2UnitStrc original_pUnit{};
			int original_pSwitchRightSkillId{};
			int original_pSwitchRightSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pSwitchRightSkillId,
				int& pSwitchRightSkillFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSwitchRightSkillId, moo_pSwitchRightSkillFlags);
			setup_data(original_pUnit, original_pSwitchRightSkillId, original_pSwitchRightSkillFlags);

			// Call both implementations
			sut(&moo_pUnit, &moo_pSwitchRightSkillId, &moo_pSwitchRightSkillFlags);
			original(&original_pUnit, &original_pSwitchRightSkillId, &original_pSwitchRightSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSwitchRightSkillId, original_pSwitchRightSkillId, "Comparing pSwitchRightSkillId");
			SKIP_MOO_CHECK_EQ(moo_pSwitchRightSkillFlags, original_pSwitchRightSkillFlags, "Comparing pSwitchRightSkillFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2330 (#10450)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetSwitchLeftSkillDataResetLeftSkill, dll_base + 0x00082330);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pSwitchLeftSkillId{};
			int moo_pSwitchLeftSkillFlags{};
			D2UnitStrc original_pUnit{};
			int original_pSwitchLeftSkillId{};
			int original_pSwitchLeftSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pSwitchLeftSkillId,
				int& pSwitchLeftSkillFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSwitchLeftSkillId, moo_pSwitchLeftSkillFlags);
			setup_data(original_pUnit, original_pSwitchLeftSkillId, original_pSwitchLeftSkillFlags);

			// Call both implementations
			sut(&moo_pUnit, &moo_pSwitchLeftSkillId, &moo_pSwitchLeftSkillFlags);
			original(&original_pUnit, &original_pSwitchLeftSkillId, &original_pSwitchLeftSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSwitchLeftSkillId, original_pSwitchLeftSkillId, "Comparing pSwitchLeftSkillId");
			SKIP_MOO_CHECK_EQ(moo_pSwitchLeftSkillFlags, original_pSwitchLeftSkillFlags, "Comparing pSwitchLeftSkillFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC23B0 (#10451)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetSwitchLeftSkillData, dll_base + 0x000823B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pSwitchLeftSkillId{};
			int moo_pSwitchLeftSkillFlags{};
			D2UnitStrc original_pUnit{};
			int original_pSwitchLeftSkillId{};
			int original_pSwitchLeftSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pSwitchLeftSkillId,
				int& pSwitchLeftSkillFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSwitchLeftSkillId, moo_pSwitchLeftSkillFlags);
			setup_data(original_pUnit, original_pSwitchLeftSkillId, original_pSwitchLeftSkillFlags);

			// Call both implementations
			sut(&moo_pUnit, &moo_pSwitchLeftSkillId, &moo_pSwitchLeftSkillFlags);
			original(&original_pUnit, &original_pSwitchLeftSkillId, &original_pSwitchLeftSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSwitchLeftSkillId, original_pSwitchLeftSkillId, "Comparing pSwitchLeftSkillId");
			SKIP_MOO_CHECK_EQ(moo_pSwitchLeftSkillFlags, original_pSwitchLeftSkillFlags, "Comparing pSwitchLeftSkillFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2420 (#10452)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetSwitchRightSkillData, dll_base + 0x00082420);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pSwitchRightSkillId{};
			int moo_pSwitchRightSkillFlags{};
			D2UnitStrc original_pUnit{};
			int original_pSwitchRightSkillId{};
			int original_pSwitchRightSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pSwitchRightSkillId,
				int& pSwitchRightSkillFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pSwitchRightSkillId, moo_pSwitchRightSkillFlags);
			setup_data(original_pUnit, original_pSwitchRightSkillId, original_pSwitchRightSkillFlags);

			// Call both implementations
			sut(&moo_pUnit, &moo_pSwitchRightSkillId, &moo_pSwitchRightSkillFlags);
			original(&original_pUnit, &original_pSwitchRightSkillId, &original_pSwitchRightSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pSwitchRightSkillId, original_pSwitchRightSkillId, "Comparing pSwitchRightSkillId");
			SKIP_MOO_CHECK_EQ(moo_pSwitchRightSkillFlags, original_pSwitchRightSkillFlags, "Comparing pSwitchRightSkillFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2490 (#10453)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetSwitchLeftSkill, dll_base + 0x00082490);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSwitchLeftSkillId{};
			int nSwitchLeftSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSwitchLeftSkillId, nSwitchLeftSkillFlags);
			original(&original_pUnit, nSwitchLeftSkillId, nSwitchLeftSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC24E0 (#10454)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetSwitchRightSkill, dll_base + 0x000824E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSwitchRightSkillId{};
			int nSwitchRightSkillFlags{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSwitchRightSkillId, nSwitchRightSkillFlags);
			original(&original_pUnit, nSwitchRightSkillId, nSwitchRightSkillFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2530 (#10455)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetWeaponGUID, dll_base + 0x00082530);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pWeapon{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pWeapon{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pWeapon
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pWeapon);
			setup_data(original_pUnit, original_pWeapon);

			// Call both implementations
			sut(&moo_pUnit, &moo_pWeapon);
			original(&original_pUnit, &original_pWeapon);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pWeapon, original_pWeapon, "Comparing pWeapon");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC25B0 (#10456)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetWeaponGUID, dll_base + 0x000825B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2630 (#10339)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetStashGoldLimit, dll_base + 0x00082630);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2680 (#10317)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_CanSwitchAI, dll_base + 0x00082680);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2720 (#10458)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetTimerArg, dll_base + 0x00082720);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2TimerArgStrc moo_pTimerArg{};
			D2UnitStrc original_pUnit{};
			D2TimerArgStrc original_pTimerArg{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2TimerArgStrc& pTimerArg
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pTimerArg);
			setup_data(original_pUnit, original_pTimerArg);

			// Call both implementations
			sut(&moo_pUnit, &moo_pTimerArg);
			original(&original_pUnit, &original_pTimerArg);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pTimerArg, original_pTimerArg, "Comparing pTimerArg");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2750 (#10459)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetTimerArg, dll_base + 0x00082750);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2780 (#10460)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_AllocStaticPath, dll_base + 0x00082780);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC27C0 (#10461)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_FreeStaticPath, dll_base + 0x000827C0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC27F0 (#10462)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_CanDualWield, dll_base + 0x000827F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2860 (#11238)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsCorpseUseable, dll_base + 0x00082860);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2910 (#11307)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsObjectInInteractRange, dll_base + 0x00082910);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pObject{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pObject{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pObject
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pObject);
			setup_data(original_pUnit, original_pObject);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pObject);
			const auto original_result = original(&original_pUnit, &original_pObject);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pObject, original_pObject, "Comparing pObject");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2C80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetCharStatsTxtRecord, dll_base + 0x00082C80);
		
		SUBCASE("")
		{
			int nRecordId{};

			// Call both implementations
			const auto moo_result = sut(nRecordId);
			const auto original_result = original(nRecordId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2CB0 (#10399)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10399, dll_base + 0x00082CB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2);
			const auto original_result = original(&original_pUnit1, &original_pUnit2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2E40 (#10397)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetDistanceToOtherUnit, dll_base + 0x00082E40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc moo_pUnit2{};
			D2UnitStrc original_pUnit1{};
			D2UnitStrc original_pUnit2{};

			const auto setup_data = [](
				D2UnitStrc& pUnit1,
				D2UnitStrc& pUnit2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1, moo_pUnit2);
			setup_data(original_pUnit1, original_pUnit2);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, &moo_pUnit2);
			const auto original_result = original(&original_pUnit1, &original_pUnit2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2F50 (#10398)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_GetDistanceToCoordinates, dll_base + 0x00082F50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nX, nY);
			const auto original_result = original(&original_pUnit, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC2FF0 (#10400)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_IsInRange, dll_base + 0x00082FF0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2CoordStrc moo_pCoord{};
			D2UnitStrc original_pUnit{};
			D2CoordStrc original_pCoord{};
			int nDistance{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2CoordStrc& pCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pCoord);
			setup_data(original_pUnit, original_pCoord);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pCoord, nDistance);
			const auto original_result = original(&original_pUnit, &original_pCoord, nDistance);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pCoord, original_pCoord, "Comparing pCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3090 (#10406)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10406, dll_base + 0x00083090);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			void* moo_a3 = nullptr;
			void* original_a3 = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nullptr, moo_a3);
			const auto original_result = original(&original_pUnit, nullptr, original_a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC33C0 (#10407)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10407, dll_base + 0x000833C0);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			void* moo_a5 = nullptr;
			void* original_a5 = nullptr;
			int a6{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom);
			setup_data(original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pRoom, nX, nY, nullptr, moo_a5, a6);
			const auto original_result = original(&original_pRoom, nX, nY, nullptr, original_a5, a6);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_a5, original_a5, "Comparing a5");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3680 (#10419)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(UNITS_SetInteractData, dll_base + 0x00083680);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSkillId{};
			int nUnitType{};
			D2UnitGUID nUnitGUID{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nSkillId, nUnitType, nUnitGUID);
			original(&original_pUnit, nSkillId, nUnitType, nUnitGUID);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
}
#endif
