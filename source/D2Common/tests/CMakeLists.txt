# Note :
# Tests in static libraries might not get registered, see https://github.com/onqtam/doctest/blob/master/doc/markdown/faq.md#why-are-my-tests-in-a-static-library-not-getting-registered
# For this reason, and because it is interesting to have individual
# test executables for each library, it is suggested not to put tests directly in the libraries (even though doctest advocates this usage)
# Creating multiple executables is of course not mandatory, and one could use the same executable with various command lines to filter what tests to run.

add_executable(D2CommonTests D2CommonTests.cpp D2CommonTestDefines.h)

# DataTbls
target_sources(D2CommonTests
  PRIVATE
    DataTbls\\AnimTblsTests.cpp
    DataTbls\\ArenaTblsTests.cpp
    DataTbls\\BeltsTblsTests.cpp
    DataTbls\\FieldTblsTests.cpp
    DataTbls\\HoradricCubeTests.cpp
    DataTbls\\InvTblsTests.cpp
    DataTbls\\ItemsTblsTests.cpp
    DataTbls\\LevelsTblsTests.cpp
    DataTbls\\MissilesTblsTests.cpp
    DataTbls\\MonsterTblsTests.cpp
    DataTbls\\ObjectsTblsTests.cpp
    DataTbls\\OverlayTblsTests.cpp
    DataTbls\\SequenceTblsTests.cpp
    DataTbls\\SkillsTblsTests.cpp
    DataTbls\\TokenTblsTests.cpp
    DataTbls\\TransformTblsTests.cpp
)

# Drlg
target_sources(D2CommonTests
  PRIVATE
    Drlg\\D2DrlgActivateTests.cpp
    Drlg\\D2DrlgDrlgTests.cpp
    Drlg\\D2DrlgDrlgAnimTests.cpp
    Drlg\\D2DrlgDrlgGridTests.cpp
    Drlg\\D2DrlgDrlgLogicTests.cpp
    Drlg\\D2DrlgDrlgRoomTests.cpp
    Drlg\\D2DrlgDrlgVerTests.cpp
    Drlg\\D2DrlgDrlgWarpTests.cpp
    Drlg\\D2DrlgMazeTests.cpp
    Drlg\\D2DrlgOutDesrTests.cpp
    Drlg\\D2DrlgOutdoorsTests.cpp
    Drlg\\D2DrlgOutJungTests.cpp
    Drlg\\D2DrlgOutPlaceTests.cpp
    Drlg\\D2DrlgOutRoomTests.cpp
    Drlg\\D2DrlgOutSiegeTests.cpp
    Drlg\\D2DrlgOutWildTests.cpp
    Drlg\\D2DrlgPresetTests.cpp
    Drlg\\D2DrlgRoomTileTests.cpp
    Drlg\\D2DrlgTileSubTests.cpp
)

# Path
target_sources(D2CommonTests
  PRIVATE
    Path\\AStarTests.cpp
    Path\\IDAStarTests.cpp
    Path\\PathTests.cpp
    Path\\PathMiscTests.cpp
    Path\\PathUtilTests.cpp
    Path\\PathWFTests.cpp
    Path\\StepTests.cpp
)

# Units
target_sources(D2CommonTests
  PRIVATE
    Units\\MissileTests.cpp
    Units\\MissileStreamTests.cpp
    Units\\UnitFindsTests.cpp
    Units\\UnitRoomTests.cpp
    Units\\UnitsTests.cpp
)

# Top-level
target_sources(D2CommonTests
  PRIVATE
    D2ChatTests.cpp
    D2CollisionTests.cpp
    D2CompositTests.cpp
    D2DataTblsTests.cpp
    D2DungeonTests.cpp
    D2EnvironmentTests.cpp
    D2InventoryTests.cpp
    D2ItemModsTests.cpp
    D2ItemsTests.cpp
    D2MonstersTests.cpp
    D2QuestRecordTests.cpp
    D2SeedTests.cpp
    D2SkillsTests.cpp
    D2StatesTests.cpp
    D2StatListTests.cpp
    D2TextTests.cpp
    D2WaypointsTests.cpp
)

get_target_property(D2CommonTestsSources D2CommonTests SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${D2CommonTestsSources})

if(MSVC)
    target_compile_options(D2CommonTests PRIVATE /bigobj)
endif()

target_include_directories(D2CommonTests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(D2CommonTests PRIVATE doctest::doctest TestDefinitions TestUtilities VisitorFramework D2Game D2Lang ${D2CommonImplName})
target_compile_features(D2CommonTests PRIVATE cxx_std_17)

set_target_properties(D2CommonTests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../testWorkingDirectory
)

add_test(
    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME D2CommonTests.unittests
    COMMAND D2CommonTests ${TEST_RUNNER_PARAMS}
    WORKING_DIRECTORY $<TARGET_PROPERTY:D2CommonTests,VS_DEBUGGER_WORKING_DIRECTORY>
)
