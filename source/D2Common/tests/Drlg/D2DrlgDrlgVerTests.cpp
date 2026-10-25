#include <D2CommonTestDefines.h>

#ifdef DRLG_VER_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Drlg/D2DrlgDrlgVer.h>


TEST_SUITE("D2DrlgDrlgVerTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD782A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGVER_AllocVertex, dll_base + 0x000382A0);
		
		SUBCASE("")
		{
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			uint8_t nDirection{};

			// Call both implementations
			const auto moo_result = sut(moo_pMemPool, nDirection);
			const auto original_result = original(original_pMemPool, nDirection);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD782D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGVER_CreateVertices, dll_base + 0x000382D0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgVertexStrc* moo_ppVertices{};
			D2DrlgCoordStrc moo_pDrlgCoord{};
			D2DrlgOrthStrc moo_pDrlgRoomData{};
			D2DrlgVertexStrc* original_ppVertices{};
			D2DrlgCoordStrc original_pDrlgCoord{};
			D2DrlgOrthStrc original_pDrlgRoomData{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;
			uint8_t nDirection{};

			const auto setup_data = [](
				D2DrlgVertexStrc*& ppVertices,
				D2DrlgCoordStrc& pDrlgCoord,
				D2DrlgOrthStrc& pDrlgRoomData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ppVertices, moo_pDrlgCoord, moo_pDrlgRoomData);
			setup_data(original_ppVertices, original_pDrlgCoord, original_pDrlgRoomData);

			// Call both implementations
			sut(moo_pMemPool, &moo_ppVertices, &moo_pDrlgCoord, nDirection, &moo_pDrlgRoomData);
			original(original_pMemPool, &original_ppVertices, &original_pDrlgCoord, nDirection, &original_pDrlgRoomData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_ppVertices, original_ppVertices, "Comparing ppVertices");
			SKIP_MOO_CHECK_EQ(moo_pDrlgCoord, original_pDrlgCoord, "Comparing pDrlgCoord");
			SKIP_MOO_CHECK_EQ(moo_pDrlgRoomData, original_pDrlgRoomData, "Comparing pDrlgRoomData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD786C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGVER_FreeVertices, dll_base + 0x000386C0);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgVertexStrc* moo_ppVertices{};
			D2DrlgVertexStrc* original_ppVertices{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2DrlgVertexStrc*& ppVertices
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_ppVertices);
			setup_data(original_ppVertices);

			// Call both implementations
			sut(moo_pMemPool, &moo_ppVertices);
			original(original_pMemPool, &original_ppVertices);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_ppVertices, original_ppVertices, "Comparing ppVertices");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD78730" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DRLGVER_GetCoordDiff, dll_base + 0x00038730);
		
		SUBCASE("")
		{
			// Input data
			D2DrlgVertexStrc moo_pDrlgVertex{};
			int moo_pDiffX{};
			int moo_pDiffY{};
			D2DrlgVertexStrc original_pDrlgVertex{};
			int original_pDiffX{};
			int original_pDiffY{};

			const auto setup_data = [](
				D2DrlgVertexStrc& pDrlgVertex,
				int& pDiffX,
				int& pDiffY
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pDrlgVertex, moo_pDiffX, moo_pDiffY);
			setup_data(original_pDrlgVertex, original_pDiffX, original_pDiffY);

			// Call both implementations
			sut(&moo_pDrlgVertex, &moo_pDiffX, &moo_pDiffY);
			original(&original_pDrlgVertex, &original_pDiffX, &original_pDiffY);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pDrlgVertex, original_pDrlgVertex, "Comparing pDrlgVertex");
			SKIP_MOO_CHECK_EQ(moo_pDiffX, original_pDiffX, "Comparing pDiffX");
			SKIP_MOO_CHECK_EQ(moo_pDiffY, original_pDiffY, "Comparing pDiffY");
		}
	}
}
#endif
