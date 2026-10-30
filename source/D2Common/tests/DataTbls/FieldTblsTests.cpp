#include <D2CommonTestDefines.h>

#ifdef FIELD_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <DataTbls/FieldTbls.h>
#include <Drlg/D2DrlgDrlg.h>


TEST_SUITE("FieldTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD51FC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadExpFieldD2, dll_base + 0x00011FC0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			const auto moo_result = sut(hArchive);
			const auto original_result = original(hArchive);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52010 (#11089)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_InitializeCollisionFieldTable, dll_base + 0x00012010);
		
		SUBCASE("")
		{
			// Input data
			char moo_pExpField{};
			char original_pExpField{};
			int nSize{};

			const auto setup_data = [](
				char& pExpField
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pExpField);
			setup_data(original_pExpField);

			// Call both implementations
			const auto moo_result = sut(&moo_pExpField, nSize);
			const auto original_result = original(&original_pExpField, nSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pExpField, original_pExpField, "Comparing pExpField");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD520F0 (#11090)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_FreeCollisionFieldTable, dll_base + 0x000120F0);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52140 (#11091)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetCollisionFieldCenter, dll_base + 0x00012140);
		
		SUBCASE("")
		{
			// Input data
			int moo_pCenterX{};
			int moo_pCenterY{};
			int original_pCenterX{};
			int original_pCenterY{};

			const auto setup_data = [](
				int& pCenterX,
				int& pCenterY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pCenterX, moo_pCenterY);
			setup_data(original_pCenterX, original_pCenterY);

			// Call both implementations
			sut(&moo_pCenterX, &moo_pCenterY);
			original(&original_pCenterX, &original_pCenterY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pCenterX, original_pCenterX, "Comparing pCenterX");
			SKIP_MOO_CHECK_EQ(moo_pCenterY, original_pCenterY, "Comparing pCenterY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52160 (#11092)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetCollisionFieldWidth, dll_base + 0x00012160);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52170 (#11093)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetCollisionFieldHeight, dll_base + 0x00012170);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52180 (#11094)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_AllocField, dll_base + 0x00012180);
		
		SUBCASE("")
		{
			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD521D0 (#11095)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_FreeField, dll_base + 0x000121D0);
		
		SUBCASE("")
		{
			// Input data
			D2FieldStrc moo_pField{};
			D2FieldStrc original_pField{};

			const auto setup_data = [](
				D2FieldStrc& pField
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pField);
			setup_data(original_pField);

			// Call both implementations
			sut(&moo_pField);
			original(&original_pField);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pField, original_pField, "Comparing pField");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52210 (#11096)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_SetFieldCoordinates, dll_base + 0x00012210);
		
		SUBCASE("")
		{
			// Input data
			D2FieldStrc moo_pField{};
			D2FieldStrc original_pField{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2FieldStrc& pField
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pField);
			setup_data(original_pField);

			// Call both implementations
			sut(&moo_pField, nX, nY);
			original(&original_pField, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pField, original_pField, "Comparing pField");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52250 (#11097)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11097, dll_base + 0x00012250);
		
		SUBCASE("")
		{
			// Input data
			D2FieldStrc moo_pField{};
			D2FieldStrc original_pField{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2FieldStrc& pField
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pField);
			setup_data(original_pField);

			// Call both implementations
			const auto moo_result = sut(&moo_pField, nX, nY);
			const auto original_result = original(&original_pField, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pField, original_pField, "Comparing pField");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD522A0 (#11098)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11098, dll_base + 0x000122A0);
		
		SUBCASE("")
		{
			// Input data
			D2FieldStrc moo_pField{};
			int moo_pX{};
			int moo_pY{};
			D2FieldStrc original_pField{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				D2FieldStrc& pField,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pField, moo_pX, moo_pY);
			setup_data(original_pField, original_pX, original_pY);

			// Call both implementations
			const auto moo_result = sut(&moo_pField, &moo_pX, &moo_pY);
			const auto original_result = original(&original_pField, &original_pX, &original_pY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pField, original_pField, "Comparing pField");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD52360 (#11099)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11099, dll_base + 0x00012360);
		
		SUBCASE("")
		{
			// Input data
			D2FieldStrc moo_pField{};
			D2ActiveRoomStrc moo_pRoom{};
			D2FieldStrc original_pField{};
			D2ActiveRoomStrc original_pRoom{};
			int nX{};
			int nY{};
			uint16_t fMask{};

			const auto setup_data = [](
				D2FieldStrc& pField,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pField, moo_pRoom);
			setup_data(original_pField, original_pRoom);

			// Call both implementations
			const auto moo_result = sut(&moo_pField, &moo_pRoom, nX, nY, fMask);
			const auto original_result = original(&original_pField, &original_pRoom, nX, nY, fMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pField, original_pField, "Comparing pField");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
}
#endif
