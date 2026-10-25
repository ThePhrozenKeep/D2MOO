#include <D2CommonTestDefines.h>

#ifdef PATH_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlg.h>
#include <Path/Path.h>
#include <Units/Units.h>


TEST_SUITE("PathTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA8220" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDA8220, dll_base + 0x00068220);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA82A0 (#10141)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetClientCoordsVelocity, dll_base + 0x000682A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			int moo_pX{};
			int moo_pY{};
			D2UnitStrc original_pUnit{};
			int original_pX{};
			int original_pY{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				int& pX,
				int& pY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pX, moo_pY);
			setup_data(original_pUnit, original_pX, original_pY);

			// Call both implementations
			sut(&moo_pUnit, &moo_pX, &moo_pY);
			original(&original_pUnit, &original_pX, &original_pY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pX, original_pX, "Comparing pX");
			SKIP_MOO_CHECK_EQ(moo_pY, original_pY, "Comparing pY");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA8320 (#10222)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AddCollisionFootprintForUnit, dll_base + 0x00068320);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA8450 (#10223)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_RemoveCollisionFootprintForUnit, dll_base + 0x00068450);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL bForce{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, bForce);
			const auto original_result = original(&original_pUnit, bForce);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA8600 (#10142)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10142, dll_base + 0x00068600);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pPath{};
			D2UnitStrc moo_pUnit{};
			D2DynamicPathStrc original_pPath{};
			D2UnitStrc original_pUnit{};
			int bAllowInTown{};

			const auto setup_data = [](
				D2DynamicPathStrc& pPath,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPath, moo_pUnit);
			setup_data(original_pPath, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pPath, &moo_pUnit, bAllowInTown);
			const auto original_result = original(&original_pPath, &original_pUnit, bAllowInTown);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPath, original_pPath, "Comparing pPath");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA8E30" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputePathClassicMissile, dll_base + 0x00068E30);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2UnitStrc moo_pUnit{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pUnit);
			setup_data(original_pDynamicPath, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_pUnit);
			const auto original_result = original(&original_pDynamicPath, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA8FE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_FindValidTargetCoordsByMovingOrthogonally, dll_base + 0x00068FE0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			sut(&moo_pPathInfo);
			original(&original_pPathInfo);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA90C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AdvanceToDoor, dll_base + 0x000690C0);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo);
			setup_data(original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo);
			const auto original_result = original(&original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9190 (#10156)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_FreeDynamicPath, dll_base + 0x00069190);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDynamicPath);
			original(original_pMemPool, &original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA91B0 (#11282)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetCollisionPatternFromMonStats2Txt, dll_base + 0x000691B0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9250 (#11281)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_11281_CollisionPatternFromSize, dll_base + 0x00069250);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nSize{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nSize);
			const auto original_result = original(&original_pUnit, nSize);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA92F0 (#10214)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10214, dll_base + 0x000692F0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9480 (#10152)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AllocDynamicPath, dll_base + 0x00069480);
		
		SUBCASE("")
		{
			// Input data
			D2ActiveRoomStrc moo_pRoom{};
			D2UnitStrc moo_pUnit{};
			D2ActiveRoomStrc original_pRoom{};
			D2UnitStrc original_pUnit{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nX{};
			int nY{};
			BOOL bSetFlag{};

			const auto setup_data = [](
				D2ActiveRoomStrc& pRoom,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pRoom, moo_pUnit);
			setup_data(original_pRoom, original_pUnit);

			// Call both implementations
			sut(moo_pMemPool, &moo_pRoom, nX, nY, &moo_pUnit, bSetFlag);
			original(original_pMemPool, &original_pRoom, nX, nY, &original_pUnit, bSetFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9720" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDA9720, dll_base + 0x00069720);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			uint8_t nDirection{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nDirection);
			original(&original_pDynamicPath, nDirection);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9770 (#10193)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10193_PATH_AdjustDirection, dll_base + 0x00069770);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA97C0 (#10216)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10216, dll_base + 0x000697C0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nX{};
			int nY{};
			int a4{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nX, nY, a4);
			original(&original_pDynamicPath, nX, nY, a4);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9850 (#10228)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10228, dll_base + 0x00069850);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9870 (#10143)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetUnitDeadCollision, dll_base + 0x00069870);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL bForGameLogic{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, bForGameLogic);
			original(&original_pUnit, bForGameLogic);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA98F0 (#10144)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetUnitAliveCollision, dll_base + 0x000698F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL bForGameLogic{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, bForGameLogic);
			original(&original_pUnit, bForGameLogic);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9A70 (#10146)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetVelocity, dll_base + 0x00069A70);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nVelocity{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nVelocity, &szFile, nLine);
			original(&original_pDynamicPath, nVelocity, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9AB0 (#10147)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetVelocity, dll_base + 0x00069AB0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9AC0 (#10148)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetMaxVelocity, dll_base + 0x00069AC0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nMaxVelocity{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nMaxVelocity);
			original(&original_pDynamicPath, nMaxVelocity);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9AE0 (#10149)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetMaxVelocity, dll_base + 0x00069AE0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9AF0 (#10150)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetAcceleration, dll_base + 0x00069AF0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nAcceleration{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nAcceleration);
			original(&original_pDynamicPath, nAcceleration);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9B10 (#10151)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetAcceleration, dll_base + 0x00069B10);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9B20 (#10153)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10153, dll_base + 0x00069B20);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9B40 (#10208)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10208_PathSetPathingFlag, dll_base + 0x00069B40);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			BOOL bSet{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, bSet);
			original(&original_pDynamicPath, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9B70 (#10209)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10209_PathCheckPathingFlag, dll_base + 0x00069B70);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9B80 (#10154)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetNumberOfPathPoints, dll_base + 0x00069B80);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9B90 (#11291)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetNumberOfPathPoints, dll_base + 0x00069B90);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int a2{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, a2);
			original(&original_pDynamicPath, a2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9BC0 (#10155)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10155, dll_base + 0x00069BC0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9BD0 (#10157)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetPathPoints, dll_base + 0x00069BD0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2PathPointStrc* moo_ppPathPoints{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2PathPointStrc* original_ppPathPoints{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2PathPointStrc*& ppPathPoints
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_ppPathPoints);
			setup_data(original_pDynamicPath, original_ppPathPoints);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_ppPathPoints);
			const auto original_result = original(&original_pDynamicPath, &original_ppPathPoints);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_ppPathPoints, original_ppPathPoints, "Comparing ppPathPoints");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9BF0 (#10158)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetDirection, dll_base + 0x00069BF0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9C10 (#10159)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetNewDirection, dll_base + 0x00069C10);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9C20 (#10160)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10160_PathUpdateDirection, dll_base + 0x00069C20);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			uint8_t nDirection{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nDirection);
			original(&original_pDynamicPath, nDirection);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9C90 (#10161)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetDirection, dll_base + 0x00069C90);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			uint8_t nDirection{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nDirection);
			original(&original_pDynamicPath, nDirection);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9CB0 (#10162)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetXPosition, dll_base + 0x00069CB0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9CF0 (#10163)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetYPosition, dll_base + 0x00069CF0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9D30 (#10194)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetPrecisionX, dll_base + 0x00069D30);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9D60 (#10195)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetPrecisionY, dll_base + 0x00069D60);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9D90 (#10196)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetPrecisionX, dll_base + 0x00069D90);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nPrecisionX{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nPrecisionX);
			original(&original_pDynamicPath, nPrecisionX);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9DA0 (#10197)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetPrecisionY, dll_base + 0x00069DA0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nPrecisionY{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nPrecisionY);
			original(&original_pDynamicPath, nPrecisionY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9DB0 (#10164)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetClientCoordX, dll_base + 0x00069DB0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDC3CE0 (#10165)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetClientCoordY, dll_base + 0x00083CE0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9DC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetClientCoordX, dll_base + 0x00069DC0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nTargetX{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nTargetX);
			original(&original_pDynamicPath, nTargetX);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9DD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetClientCoordY, dll_base + 0x00069DD0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nTargetY{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nTargetY);
			original(&original_pDynamicPath, nTargetY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9DE0 (#10175)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10175_PathGetFirstPointX, dll_base + 0x00069DE0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9DF0 (#10176)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10176_PathGetFirstPointY, dll_base + 0x00069DF0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E00 (#10224)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10224, dll_base + 0x00069E00);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E10 (#10225)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10225, dll_base + 0x00069E10);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E20 (#10177)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10177_PATH_GetLastPointX, dll_base + 0x00069E20);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E40 (#10178)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10178_PATH_GetLastPointY, dll_base + 0x00069E40);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDB9C10 (#10166)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetRoom, dll_base + 0x00079C10);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E60 (#10167)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetRoom, dll_base + 0x00069E60);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2ActiveRoomStrc moo_pRoom{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2ActiveRoomStrc original_pRoom{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2ActiveRoomStrc& pRoom
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pRoom);
			setup_data(original_pDynamicPath, original_pRoom);

			// Call both implementations
			sut(&moo_pDynamicPath, &moo_pRoom);
			original(&original_pDynamicPath, &original_pRoom);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pRoom, original_pRoom, "Comparing pRoom");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E70 (#10168)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetNextRoom, dll_base + 0x00069E70);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E80 (#10169)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ClearNextRoom, dll_base + 0x00069E80);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9E90 (#10170)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10170_PathSetTargetPos, dll_base + 0x00069E90);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nX, nY);
			original(&original_pDynamicPath, nX, nY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9EC0 (#10172)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_IsCurrentRoomInvalid, dll_base + 0x00069EC0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9ED0 (#10173)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetCurrentRoomInvalid, dll_base + 0x00069ED0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			BOOL bSet{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, bSet);
			original(&original_pDynamicPath, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9F00 (#10145)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetUnusedFlag_0x00004, dll_base + 0x00069F00);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			BOOL bSet{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, bSet);
			original(&original_pDynamicPath, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9F30 (#10174)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetUnusedFlag_0x00004, dll_base + 0x00069F30);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9F40 (#10179)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetTargetUnit, dll_base + 0x00069F40);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2UnitStrc moo_pUnit{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pUnit);
			setup_data(original_pDynamicPath, original_pUnit);

			// Call both implementations
			sut(&moo_pDynamicPath, &moo_pUnit);
			original(&original_pDynamicPath, &original_pUnit);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9F60 (#10171)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetTargetTypeAndGUID, dll_base + 0x00069F60);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			int moo_pTargetType{};
			D2UnitGUID moo_pTargetGUID{};
			D2DynamicPathStrc original_pDynamicPath{};
			int original_pTargetType{};
			D2UnitGUID original_pTargetGUID{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				int& pTargetType,
				D2UnitGUID& pTargetGUID
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_pTargetType, moo_pTargetGUID);
			setup_data(original_pDynamicPath, original_pTargetType, original_pTargetGUID);

			// Call both implementations
			sut(&moo_pDynamicPath, &moo_pTargetType, &moo_pTargetGUID);
			original(&original_pDynamicPath, &original_pTargetType, &original_pTargetGUID);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_pTargetType, original_pTargetType, "Comparing pTargetType");
			SKIP_MOO_CHECK_EQ(moo_pTargetGUID, original_pTargetGUID, "Comparing pTargetGUID");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9FA0 (#10180)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetTargetUnit, dll_base + 0x00069FA0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9FC0 (#10181)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetFootprintCollisionMask, dll_base + 0x00069FC0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA9FE0 (#10182)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetFootprintCollisionMask, dll_base + 0x00069FE0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nCollisionMask{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nCollisionMask);
			original(&original_pDynamicPath, nCollisionMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA0C0 (#10183)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetMoveTestCollisionMask, dll_base + 0x0006A0C0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA0D0 (#10184)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetMoveTestCollisionMask, dll_base + 0x0006A0D0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nCollisionMask{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nCollisionMask);
			original(&original_pDynamicPath, nCollisionMask);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA0E0 (#10185)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetType, dll_base + 0x0006A0E0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nPathType{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nPathType);
			original(&original_pDynamicPath, nPathType);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA1E0 (#10186)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ResetToPreviousType, dll_base + 0x0006A1E0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA240 (#10187)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetType, dll_base + 0x0006A240);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA250 (#10190)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10190_PATH_SetDistance, dll_base + 0x0006A250);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			uint8_t nDistance{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nDistance);
			original(&original_pDynamicPath, nDistance);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA270 (#10191)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10191_PATH_GetDistance, dll_base + 0x0006A270);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA280 (#10188)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetNewDistance, dll_base + 0x0006A280);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			uint8_t nNewDistance{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nNewDistance);
			original(&original_pDynamicPath, nNewDistance);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA2B0 (#10189)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetMaxDistance, dll_base + 0x0006A2B0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA2C0 (#10201)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10201, dll_base + 0x0006A2C0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA300 (#10202)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10202, dll_base + 0x0006A300);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA310 (#10192)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetIDAStarInitFScore, dll_base + 0x0006A310);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nIDAStarInitFScore{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nIDAStarInitFScore);
			original(&original_pDynamicPath, nIDAStarInitFScore);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA350 (#10198)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10198_PathGetSaveStep, dll_base + 0x0006A350);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2PathPointStrc* moo_ppPathPoints{};
			D2DynamicPathStrc original_pDynamicPath{};
			D2PathPointStrc* original_ppPathPoints{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath,
				D2PathPointStrc*& ppPathPoints
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath, moo_ppPathPoints);
			setup_data(original_pDynamicPath, original_ppPathPoints);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath, &moo_ppPathPoints);
			const auto original_result = original(&original_pDynamicPath, &original_ppPathPoints);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
			SKIP_MOO_CHECK_EQ(moo_ppPathPoints, original_ppPathPoints, "Comparing ppPathPoints");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA390 (#10199)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10199_PathGetSaveX, dll_base + 0x0006A390);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA3E0 (#10200)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10200_PathGetSaveY, dll_base + 0x0006A3E0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA430 (#10203)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10203_PATH_SetRotateFlag, dll_base + 0x0006A430);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			BOOL bReset{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, bReset);
			original(&original_pDynamicPath, bReset);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA460 (#10204)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10204_PATH_ClearPoint2, dll_base + 0x0006A460);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath);
			original(&original_pDynamicPath);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA480 (#10205)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetStepNum, dll_base + 0x0006A480);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			uint8_t nSteps{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nSteps);
			original(&original_pDynamicPath, nSteps);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA4B0 (#10206)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetStepNum, dll_base + 0x0006A4B0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA4C0 (#10207)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10207, dll_base + 0x0006A4C0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			char a2{};
			char a3{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, a2, a3);
			original(&original_pDynamicPath, a2, a3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA4E0 (#10217)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetDistance, dll_base + 0x0006A4E0);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nDist{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nDist);
			original(&original_pDynamicPath, nDist);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA520 (#10218)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetDistance, dll_base + 0x0006A520);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			const auto moo_result = sut(&moo_pDynamicPath);
			const auto original_result = original(&original_pDynamicPath);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA530 (#10219)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AddToDistance, dll_base + 0x0006A530);
		
		SUBCASE("")
		{
			// Input data
			D2DynamicPathStrc moo_pDynamicPath{};
			D2DynamicPathStrc original_pDynamicPath{};
			int nAddition{};

			const auto setup_data = [](
				D2DynamicPathStrc& pDynamicPath
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDynamicPath);
			setup_data(original_pDynamicPath);

			// Call both implementations
			sut(&moo_pDynamicPath, nAddition);
			original(&original_pDynamicPath, nAddition);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDynamicPath, original_pDynamicPath, "Comparing pDynamicPath");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA570 (#10210)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_GetUnitCollisionPattern, dll_base + 0x0006A570);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA580 (#10211)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_SetUnitCollisionPattern, dll_base + 0x0006A580);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nCollisionPattern{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, nCollisionPattern);
			original(&original_pUnit, nCollisionPattern);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA5A0 (#10212)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_10212_PATH_SetMoveFlags, dll_base + 0x0006A5A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			BOOL bSet{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			sut(&moo_pUnit, bSet);
			original(&original_pUnit, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA600 (#10213)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10213, dll_base + 0x0006A600);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA6A0 (#10220)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_ComputeSquaredDistance, dll_base + 0x0006A6A0);
		
		SUBCASE("")
		{
			int nX1{};
			int nY1{};
			int nX2{};
			int nY2{};

			// Call both implementations
			const auto moo_result = sut(nX1, nY1, nX2, nY2);
			const auto original_result = original(nX1, nY1, nX2, nY2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA6D0 (#10221)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AddCollisionFootprintForOptionalUnit, dll_base + 0x0006A6D0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDAA6F0 (#10237)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2Common_10237, dll_base + 0x0006A6F0);
		
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
}
#endif
