#include <D2CommonTestDefines.h>

#ifdef DRLG_GRID_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlgGrid.h>


TEST_SUITE("D2DrlgDrlgGridTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75BA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRGLGRID_OverwriteFlag, dll_base + 0x00035BA0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pFlag{};
			int original_pFlag{};
			int nFlag{};

			const auto setup_data = [](
				int& pFlag
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pFlag);
			setup_data(original_pFlag);

			// Call both implementations
			sut(&moo_pFlag, nFlag);
			original(&original_pFlag, nFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pFlag, original_pFlag, "Comparing pFlag");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75BB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRGLGRID_OrFlag, dll_base + 0x00035BB0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pFlag{};
			int original_pFlag{};
			int nFlag{};

			const auto setup_data = [](
				int& pFlag
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pFlag);
			setup_data(original_pFlag);

			// Call both implementations
			sut(&moo_pFlag, nFlag);
			original(&original_pFlag, nFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pFlag, original_pFlag, "Comparing pFlag");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75BC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRGLGRID_AndFlag, dll_base + 0x00035BC0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pFlag{};
			int original_pFlag{};
			int nFlag{};

			const auto setup_data = [](
				int& pFlag
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pFlag);
			setup_data(original_pFlag);

			// Call both implementations
			sut(&moo_pFlag, nFlag);
			original(&original_pFlag, nFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pFlag, original_pFlag, "Comparing pFlag");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75BD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRGLGRID_XorFlag, dll_base + 0x00035BD0);
		
		SUBCASE("")
		{
			// Input data
			int moo_pFlag{};
			int original_pFlag{};
			int nFlag{};

			const auto setup_data = [](
				int& pFlag
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pFlag);
			setup_data(original_pFlag);

			// Call both implementations
			sut(&moo_pFlag, nFlag);
			original(&original_pFlag, nFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pFlag, original_pFlag, "Comparing pFlag");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75C00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_IsGridValid, dll_base + 0x00035C00);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgGrid);
			const auto original_result = original(&original_pDrlgGrid);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75C20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_IsPointInsideGridArea, dll_base + 0x00035C20);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgGrid, nX, nY);
			const auto original_result = original(&original_pDrlgGrid, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75C50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_AlterGridFlag, dll_base + 0x00035C50);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nX{};
			int nY{};
			int nFlag{};
			FlagOperation eOperation{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			sut(&moo_pDrlgGrid, nX, nY, nFlag, eOperation);
			original(&original_pDrlgGrid, nX, nY, nFlag, eOperation);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75C80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_GetGridFlagsPointer, dll_base + 0x00035C80);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgGrid, nX, nY);
			const auto original_result = original(&original_pDrlgGrid, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75CA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_GetGridEntry, dll_base + 0x00035CA0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nX{};
			int nY{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			const auto moo_result = sut(&moo_pDrlgGrid, nX, nY);
			const auto original_result = original(&original_pDrlgGrid, nX, nY);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75CC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_AlterAllGridFlags, dll_base + 0x00035CC0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nFlag{};
			FlagOperation eOperation{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			sut(&moo_pDrlgGrid, nFlag, eOperation);
			original(&original_pDrlgGrid, nFlag, eOperation);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75D20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_AlterEdgeGridFlags, dll_base + 0x00035D20);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int nFlag{};
			FlagOperation eOperation{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			sut(&moo_pDrlgGrid, nFlag, eOperation);
			original(&original_pDrlgGrid, nFlag, eOperation);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75F10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_SetVertexGridFlags, dll_base + 0x00035F10);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgVertexStrc moo_pDrlgVertex{};
			D2DrlgGridStrc original_pDrlgGrid{};
			D2DrlgVertexStrc original_pDrlgVertex{};
			int nFlag{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid,
				D2DrlgVertexStrc& pDrlgVertex
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid, moo_pDrlgVertex);
			setup_data(original_pDrlgGrid, original_pDrlgVertex);

			// Call both implementations
			sut(&moo_pDrlgGrid, &moo_pDrlgVertex, nFlag);
			original(&original_pDrlgGrid, &original_pDrlgVertex, nFlag);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
			SKIP_MOO_CHECK_EQ(moo_pDrlgVertex, original_pDrlgVertex, "Comparing pDrlgVertex");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD75F60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FD75F60, dll_base + 0x00035F60);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgVertexStrc moo_pDrlgVertex{};
			D2DrlgCoordStrc moo_pDrlgCoord{};
			D2DrlgGridStrc original_pDrlgGrid{};
			D2DrlgVertexStrc original_pDrlgVertex{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			int nFlag{};
			FlagOperation eOperation{};
			int nSize{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid,
				D2DrlgVertexStrc& pDrlgVertex,
				D2DrlgCoordStrc& pDrlgCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid, moo_pDrlgVertex, moo_pDrlgCoord);
			setup_data(original_pDrlgGrid, original_pDrlgVertex, original_pDrlgCoord);

			// Call both implementations
			sut(&moo_pDrlgGrid, &moo_pDrlgVertex, &moo_pDrlgCoord, nFlag, eOperation, nSize);
			original(&original_pDrlgGrid, &original_pDrlgVertex, &original_pDrlgCoord, nFlag, eOperation, nSize);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
			SKIP_MOO_CHECK_EQ(moo_pDrlgVertex, original_pDrlgVertex, "Comparing pDrlgVertex");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD76230" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_InitializeGridCells, dll_base + 0x00036230);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nWidth{};
			int nHeight{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgGrid, nWidth, nHeight);
			original(original_pMemPool, &original_pDrlgGrid, nWidth, nHeight);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD762B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_FillGrid, dll_base + 0x000362B0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			int moo_pCellPos{};
			int moo_pCellRowOffsets{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int original_pCellPos{};
			int original_pCellRowOffsets{};
			int nWidth{};
			int nHeight{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid,
				int& pCellPos,
				int& pCellRowOffsets
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid, moo_pCellPos, moo_pCellRowOffsets);
			setup_data(original_pDrlgGrid, original_pCellPos, original_pCellRowOffsets);

			// Call both implementations
			sut(&moo_pDrlgGrid, nWidth, nHeight, &moo_pCellPos, &moo_pCellRowOffsets);
			original(&original_pDrlgGrid, nWidth, nHeight, &original_pCellPos, &original_pCellRowOffsets);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
			SKIP_MOO_CHECK_EQ(moo_pCellPos, original_pCellPos, "Comparing pCellPos");
			SKIP_MOO_CHECK_EQ(moo_pCellRowOffsets, original_pCellRowOffsets, "Comparing pCellRowOffsets");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD76310" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_FillNewCellFlags, dll_base + 0x00036310);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			int moo_pCellPos{};
			D2DrlgCoordStrc moo_pDrlgCoord{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int original_pCellPos{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			int nWidth{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid,
				int& pCellPos,
				D2DrlgCoordStrc& pDrlgCoord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid, moo_pCellPos, moo_pDrlgCoord);
			setup_data(original_pDrlgGrid, original_pCellPos, original_pDrlgCoord);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgGrid, &moo_pCellPos, &moo_pDrlgCoord, nWidth);
			original(original_pMemPool, &original_pDrlgGrid, &original_pCellPos, &original_pDrlgCoord, nWidth);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
			SKIP_MOO_CHECK_EQ(moo_pCellPos, original_pCellPos, "Comparing pCellPos");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD76380" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_AssignCellsOffsetsAndFlags, dll_base + 0x00036380);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			int moo_pCellPos{};
			D2DrlgCoordStrc moo_pDrlgCoord{};
			int moo_pCellFlags{};
			D2DrlgGridStrc original_pDrlgGrid{};
			int original_pCellPos{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			int original_pCellFlags{};
			int nWidth{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid,
				int& pCellPos,
				D2DrlgCoordStrc& pDrlgCoord,
				int& pCellFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid, moo_pCellPos, moo_pDrlgCoord, moo_pCellFlags);
			setup_data(original_pDrlgGrid, original_pCellPos, original_pDrlgCoord, original_pCellFlags);

			// Call both implementations
			sut(&moo_pDrlgGrid, &moo_pCellPos, &moo_pDrlgCoord, nWidth, &moo_pCellFlags);
			original(&original_pDrlgGrid, &original_pCellPos, &original_pDrlgCoord, nWidth, &original_pCellFlags);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
			SKIP_MOO_CHECK_EQ(moo_pCellPos, original_pCellPos, "Comparing pCellPos");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
			SKIP_MOO_CHECK_EQ(moo_pCellFlags, original_pCellFlags, "Comparing pCellFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD763E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_FreeGrid, dll_base + 0x000363E0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			sut(moo_pMemPool, &moo_pDrlgGrid);
			original(original_pMemPool, &original_pDrlgGrid);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD76410" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGGRID_ResetGrid, dll_base + 0x00036410);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgGridStrc moo_pDrlgGrid{};
			D2DrlgGridStrc original_pDrlgGrid{};

			const auto setup_data = [](
				D2DrlgGridStrc& pDrlgGrid
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgGrid);
			setup_data(original_pDrlgGrid);

			// Call both implementations
			sut(&moo_pDrlgGrid);
			original(&original_pDrlgGrid);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgGrid, original_pDrlgGrid, "Comparing pDrlgGrid");
		}
	}
}
#endif
