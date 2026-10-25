#include <D2CommonTestDefines.h>

#ifdef MISSILESTREAM_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Units/MissileStream.h>
#include <Units/Units.h>


TEST_SUITE("MissileStreamTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC230 (#11213)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MISSTREAM_AllocMissileStream, dll_base + 0x0007C230);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMissile{};
			D2UnitStrc original_pMissile{};

			const auto setup_data = [](
				D2UnitStrc& pMissile
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMissile);
			setup_data(original_pMissile);

			// Call both implementations
			sut(&moo_pMissile);
			original(&original_pMissile);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMissile, original_pMissile, "Comparing pMissile");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC280 (#11214)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MISSTREAM_FreeMissileStream, dll_base + 0x0007C280);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pMissile{};
			D2UnitStrc original_pMissile{};

			const auto setup_data = [](
				D2UnitStrc& pMissile
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pMissile);
			setup_data(original_pMissile);

			// Call both implementations
			sut(&moo_pMissile);
			original(&original_pMissile);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMissile, original_pMissile, "Comparing pMissile");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC2E0 (#11215)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MISSTREAM_ExecuteHit, dll_base + 0x0007C2E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nCollisionMask{};
			int nCollisionPattern{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nCollisionMask, nCollisionPattern, nullptr);
			original(&original_pUnit, nCollisionMask, nCollisionPattern, nullptr);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC3B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MISSTREAM_Return1, dll_base + 0x0007C3B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit1{};
			D2UnitStrc original_pUnit1{};
			void* moo_pUnit2 = nullptr;
			void* original_pUnit2 = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pUnit1
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit1);
			setup_data(original_pUnit1);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit1, moo_pUnit2);
			const auto original_result = original(&original_pUnit1, original_pUnit2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit1, original_pUnit1, "Comparing pUnit1");
			SKIP_MOO_CHECK_EQ(moo_pUnit2, original_pUnit2, "Comparing pUnit2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDBC3C0 (#11216)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(MISSTREAM_Update, dll_base + 0x0007C3C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_a1{};
			D2UnitStrc moo_pMissile{};
			D2UnitStrc original_a1{};
			D2UnitStrc original_pMissile{};
			int a3{};
			int a4{};

			const auto setup_data = [](
				D2UnitStrc& a1,
				D2UnitStrc& pMissile
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_a1, moo_pMissile);
			setup_data(original_a1, original_pMissile);

			// Call both implementations
			sut(&moo_a1, &moo_pMissile, a3, a4, nullptr, nullptr);
			original(&original_a1, &original_pMissile, a3, a4, nullptr, nullptr);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_a1, original_a1, "Comparing a1");
			SKIP_MOO_CHECK_EQ(moo_pMissile, original_pMissile, "Comparing pMissile");
		}
	}
}
#endif
