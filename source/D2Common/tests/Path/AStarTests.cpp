#include <D2CommonTestDefines.h>

#ifdef ASTAR_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <Path/AStar.h>


TEST_SUITE("AStarTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA69E0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_ComputePath, dll_base + 0x000669E0);
		
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
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6D10" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_PushToVisitedCache, dll_base + 0x00066D10);
		
		SUBCASE("")
		{
			// Input data
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallNodeStrc moo_pNode{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathFoWallNodeStrc original_pNode{};

			const auto setup_data = [](
				D2PathFoWallContextStrc& pContext,
				D2PathFoWallNodeStrc& pNode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext, moo_pNode);
			setup_data(original_pContext, original_pNode);

			// Call both implementations
			const auto moo_result = sut(&moo_pContext, &moo_pNode);
			const auto original_result = original(&original_pContext, &original_pNode);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_pNode, original_pNode, "Comparing pNode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA6D50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_ExploreChildren, dll_base + 0x00066D50);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallNodeStrc moo_a3{};
			D2PathInfoStrc original_pPathInfo{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathFoWallNodeStrc original_a3{};
			D2PathPointStrc tTargetCoord{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo,
				D2PathFoWallContextStrc& pContext,
				D2PathFoWallNodeStrc& a3
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo, moo_pContext, moo_a3);
			setup_data(original_pPathInfo, original_pContext, original_a3);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo, &moo_pContext, &moo_a3, tTargetCoord);
			const auto original_result = original(&original_pPathInfo, &original_pContext, &original_a3, tTargetCoord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_a3, original_a3, "Comparing a3");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7230" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_Heuristic, dll_base + 0x00067230);
		
		SUBCASE("")
		{
			D2PathPointStrc tPoint1{};
			D2PathPointStrc tPoint2{};

			// Call both implementations
			const auto moo_result = sut(tPoint1, tPoint2);
			const auto original_result = original(tPoint1, tPoint2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7280" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_GetNodeFromPendingCache, dll_base + 0x00067280);
		
		SUBCASE("")
		{
			// Input data
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathPointStrc tPathPoint{};

			const auto setup_data = [](
				D2PathFoWallContextStrc& pContext
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext);
			setup_data(original_pContext);

			// Call both implementations
			const auto moo_result = sut(&moo_pContext, tPathPoint);
			const auto original_result = original(&original_pContext, tPathPoint);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA72D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_FindPointInVisitedCache, dll_base + 0x000672D0);
		
		SUBCASE("")
		{
			// Input data
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathPointStrc tPathPoint{};

			const auto setup_data = [](
				D2PathFoWallContextStrc& pContext
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext);
			setup_data(original_pContext);

			// Call both implementations
			const auto moo_result = sut(&moo_pContext, tPathPoint);
			const auto original_result = original(&original_pContext, tPathPoint);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7320" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_MakeCandidate, dll_base + 0x00067320);
		
		SUBCASE("")
		{
			// Input data
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallNodeStrc moo_pNode{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathFoWallNodeStrc original_pNode{};

			const auto setup_data = [](
				D2PathFoWallContextStrc& pContext,
				D2PathFoWallNodeStrc& pNode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext, moo_pNode);
			setup_data(original_pContext, original_pNode);

			// Call both implementations
			sut(&moo_pContext, &moo_pNode);
			original(&original_pContext, &original_pNode);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_pNode, original_pNode, "Comparing pNode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7390" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_PropagateNewFScoreToChildren, dll_base + 0x00067390);
		
		SUBCASE("")
		{
			// Input data
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallNodeStrc moo_pNewNode{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathFoWallNodeStrc original_pNewNode{};
			int nUnused{};

			const auto setup_data = [](
				D2PathFoWallContextStrc& pContext,
				D2PathFoWallNodeStrc& pNewNode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pContext, moo_pNewNode);
			setup_data(original_pContext, original_pNewNode);

			// Call both implementations
			sut(&moo_pContext, nUnused, &moo_pNewNode);
			original(&original_pContext, nUnused, &original_pNewNode);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_pNewNode, original_pNewNode, "Comparing pNewNode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA7490" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_EvaluateNeighbor, dll_base + 0x00067490);
		
		SUBCASE("")
		{
			// Input data
			D2PathInfoStrc moo_pPathInfo{};
			D2PathFoWallContextStrc moo_pContext{};
			D2PathFoWallNodeStrc moo_pCurrentNode{};
			D2PathInfoStrc original_pPathInfo{};
			D2PathFoWallContextStrc original_pContext{};
			D2PathFoWallNodeStrc original_pCurrentNode{};
			D2PathPointStrc tNewPointCoord{};
			D2PathPointStrc tTargetCoord{};

			const auto setup_data = [](
				D2PathInfoStrc& pPathInfo,
				D2PathFoWallContextStrc& pContext,
				D2PathFoWallNodeStrc& pCurrentNode
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPathInfo, moo_pContext, moo_pCurrentNode);
			setup_data(original_pPathInfo, original_pContext, original_pCurrentNode);

			// Call both implementations
			const auto moo_result = sut(&moo_pPathInfo, &moo_pContext, &moo_pCurrentNode, tNewPointCoord, tTargetCoord);
			const auto original_result = original(&original_pPathInfo, &original_pContext, &original_pCurrentNode, tNewPointCoord, tTargetCoord);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
			SKIP_MOO_CHECK_EQ(moo_pContext, original_pContext, "Comparing pContext");
			SKIP_MOO_CHECK_EQ(moo_pCurrentNode, original_pCurrentNode, "Comparing pCurrentNode");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA78A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(PATH_AStar_FlushNodeToDynamicPath, dll_base + 0x000678A0);
		
		SUBCASE("")
		{
			// Input data
			D2PathFoWallNodeStrc moo_pNode{};
			D2PathInfoStrc moo_pPathInfo{};
			D2PathFoWallNodeStrc original_pNode{};
			D2PathInfoStrc original_pPathInfo{};

			const auto setup_data = [](
				D2PathFoWallNodeStrc& pNode,
				D2PathInfoStrc& pPathInfo
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pNode, moo_pPathInfo);
			setup_data(original_pNode, original_pPathInfo);

			// Call both implementations
			const auto moo_result = sut(&moo_pNode, &moo_pPathInfo);
			const auto original_result = original(&original_pNode, &original_pPathInfo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pNode, original_pNode, "Comparing pNode");
			SKIP_MOO_CHECK_EQ(moo_pPathInfo, original_pPathInfo, "Comparing pPathInfo");
		}
	}
}
#endif
