add_library(TestDefinitions STATIC src/TestDefinitions.cpp)

target_sources(TestDefinitions
  PRIVATE
    include/Fixtures/DataTbls/Fixtures.h
    include/Fixtures/DataTbls/AnimDataFixture.h
    include/Fixtures/DataTbls/ArenaTxtFixture.h
    include/Fixtures/DataTbls/ArmTypeTxtFixture.h
    include/Fixtures/DataTbls/BeltsTxtFixture.h
    include/Fixtures/DataTbls/BooksTxtFixture.h
    include/Fixtures/DataTbls/CharStatsTxtFixture.h
    include/Fixtures/DataTbls/CharTemplateTxtFixture.h
    include/Fixtures/DataTbls/CompCodeTxtFixture.h
    include/Fixtures/DataTbls/CompositTxtFixture.h
    include/Fixtures/DataTbls/CubeMainTxtFixture.h
    include/Fixtures/DataTbls/DifficultyLevelsTxtFixture.h
    include/Fixtures/DataTbls/ElemTypesTxtFixture.h
    include/Fixtures/DataTbls/EventsTxtFixture.h
    include/Fixtures/DataTbls/ExperienceTxtFixture.h
    include/Fixtures/DataTbls/HirelingTxtFixture.h
    include/Fixtures/DataTbls/InventoryTxtFixture.h
    include/Fixtures/DataTbls/ItemStatCostTxtFixture.h
    include/Fixtures/DataTbls/ItemsTxtFixture.h
    include/Fixtures/DataTbls/ItemTypesTxtFixture.h
    include/Fixtures/DataTbls/LevelDefsTxtFixture.h
    include/Fixtures/DataTbls/LevelsTxtFixture.h
    include/Fixtures/DataTbls/LvlMazeTxtFixture.h
    include/Fixtures/DataTbls/LvlPrestTxtFixture.h
    include/Fixtures/DataTbls/LvlSubTxtFixture.h
    include/Fixtures/DataTbls/LvlTypesTxtFixture.h
    include/Fixtures/DataTbls/LvlWarpTxtFixture.h
    include/Fixtures/DataTbls/MissCalcTxtFixture.h
    include/Fixtures/DataTbls/MissilesTxtFixture.h
    include/Fixtures/DataTbls/MonAiTxtFixture.h
    include/Fixtures/DataTbls/MonEquipTxtFixture.h
    include/Fixtures/DataTbls/MonLvlTxtFixture.h
    include/Fixtures/DataTbls/MonModeTxtFixture.h
    include/Fixtures/DataTbls/MonPlaceTxtFixture.h
    include/Fixtures/DataTbls/MonPresetTxtFixture.h
    include/Fixtures/DataTbls/MonPropTxtFixture.h
    include/Fixtures/DataTbls/MonSeqTxtFixture.h
    include/Fixtures/DataTbls/MonSoundsTxtFixture.h
    include/Fixtures/DataTbls/MonStats2TxtFixture.h
    include/Fixtures/DataTbls/MonStatsTxtFixture.h
    include/Fixtures/DataTbls/MonTypeTxtFixture.h
    include/Fixtures/DataTbls/MonUModTxtFixture.h
    include/Fixtures/DataTbls/NpcTxtFixture.h
    include/Fixtures/DataTbls/ObjectsTxtFixture.h
    include/Fixtures/DataTbls/ObjGroupTxtFixture.h
    include/Fixtures/DataTbls/OverlayTxtFixture.h
    include/Fixtures/DataTbls/PetTypeTxtFixture.h
    include/Fixtures/DataTbls/PortalLevelsFixture.h
    include/Fixtures/DataTbls/PropertiesTxtFixture.h
    include/Fixtures/DataTbls/SetItemsTxtFixture.h
    include/Fixtures/DataTbls/SetsTxtFixture.h
    include/Fixtures/DataTbls/ShrinesTxtFixture.h
    include/Fixtures/DataTbls/SkillCalcTxtFixture.h
    include/Fixtures/DataTbls/SkillDescTxtFixture.h
    include/Fixtures/DataTbls/SkillsTxtFixture.h
    include/Fixtures/DataTbls/StatesTxtFixture.h
    include/Fixtures/DataTbls/SuperUniquesTxtFixture.h
    include/Fixtures/DataTbls/UniqueAppellationTxtFixture.h
    include/Fixtures/DataTbls/UniqueItemsTxtFixture.h
    include/Fixtures/DataTbls/UniquePrefixTxtFixture.h
    include/Fixtures/DataTbls/UniqueSuffixTxtFixture.h
    include/Fixtures/DataTbls/UniqueTitleTxtFixture.h

    include/Fixtures/D2Lang/D2LangStringTblFixture.h

    include/Fixtures/UnitWithStatFlagsFixture.h
)
target_include_directories(TestDefinitions PUBLIC include)
target_link_libraries(TestDefinitions PRIVATE D2CommonDefinitions D2Common VisitorFramework)
target_compile_features(TestDefinitions PRIVATE cxx_std_17)

get_target_property(TestDefinitionsSources TestDefinitions SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TestDefinitionsSources})
