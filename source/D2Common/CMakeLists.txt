
add_library(D2CommonStatic 
  # We want to create a static library for tests or easier integration in user mods
  STATIC
  # Ordinals that correspond to empty functions due to removed (debug) code
  src/UselessOrdinals.cpp
  include/UselessOrdinals.h
  # Misc
  include/CommonDefinitions.h
  include/D2Common.h
  include/Imports/DllBases.h
)

target_link_libraries(D2CommonStatic PUBLIC D2CommonDefinitions D2CMP D2Lang Fog)

target_include_directories(D2CommonStatic PUBLIC include)

target_compile_definitions(D2CommonStatic PRIVATE D2COMMON_IMPL NOMINMAX WIN32_LEAN_AND_MEAN)

# Data tables
target_sources(D2CommonStatic
  PRIVATE
    src/DataTbls/AnimTbls.cpp
    src/DataTbls/ArenaTbls.cpp
    src/DataTbls/BeltsTbls.cpp
    src/DataTbls/DataTbls.cpp
    src/DataTbls/FieldTbls.cpp
    src/DataTbls/HoradricCube.cpp
    src/DataTbls/InvTbls.cpp
    src/DataTbls/ItemsTbls.cpp
    src/DataTbls/LevelsTbls.cpp
    src/DataTbls/MissilesTbls.cpp
    src/DataTbls/MonsterTbls.cpp
    src/DataTbls/ObjectsTbls.cpp
    src/DataTbls/OverlayTbls.cpp
    src/DataTbls/SequenceTbls.cpp
    src/DataTbls/SkillsTbls.cpp
    src/DataTbls/TokenTbls.cpp
    src/DataTbls/TransformTbls.cpp

    include/DataTbls/AnimTbls.h
    include/DataTbls/ArenaTbls.h
    include/DataTbls/BeltsTbls.h
    include/DataTbls/DataTbls.h
    include/DataTbls/FieldTbls.h
    include/DataTbls/HoradricCube.h
    include/DataTbls/InvTbls.h
    include/DataTbls/ItemsTbls.h
    include/DataTbls/LevelsTbls.h
    include/DataTbls/MissilesTbls.h
    include/DataTbls/MonsterTbls.h
    include/DataTbls/ObjectsTbls.h
    include/DataTbls/OverlayTbls.h
    include/DataTbls/SequenceTbls.h
    include/DataTbls/SkillsTbls.h
    include/DataTbls/TokenTbls.h
    include/DataTbls/TransformTbls.h

    # Enums for asset indices, should be generated from .txt files
    include/DataTbls/LevelsIds.h
    include/DataTbls/MissilesIds.h
    include/DataTbls/MonsterIds.h
    include/DataTbls/ObjectsIds.h
    include/DataTbls/StringIds.h
    include/DataTbls/SkillsIds.h
)

# Dynamic Random Level Generation
target_sources(D2CommonStatic
  PRIVATE
    src/Drlg/DrlgActivate.cpp
    src/Drlg/DrlgDrlg.cpp
    src/Drlg/DrlgDrlgAnim.cpp
    src/Drlg/DrlgDrlgGrid.cpp
    src/Drlg/DrlgDrlgLogic.cpp
    src/Drlg/DrlgDrlgRoom.cpp
    src/Drlg/DrlgDrlgVer.cpp
    src/Drlg/DrlgDrlgWarp.cpp
    src/Drlg/DrlgMaze.cpp
    src/Drlg/DrlgOutDesr.cpp
    src/Drlg/DrlgOutdoors.cpp
    src/Drlg/DrlgOutJung.cpp
    src/Drlg/DrlgOutPlace.cpp
    src/Drlg/DrlgOutRoom.cpp
    src/Drlg/DrlgOutSiege.cpp
    src/Drlg/DrlgOutWild.cpp
    src/Drlg/DrlgPreset.cpp
    src/Drlg/DrlgRoomTile.cpp
    src/Drlg/DrlgTileSub.cpp

    include/Drlg/D2DrlgActivate.h
    include/Drlg/D2DrlgDrlg.h
    include/Drlg/D2DrlgDrlgAnim.h
    include/Drlg/D2DrlgDrlgGrid.h
    include/Drlg/D2DrlgDrlgLogic.h
    include/Drlg/D2DrlgDrlgRoom.h
    include/Drlg/D2DrlgDrlgVer.h
    include/Drlg/D2DrlgDrlgWarp.h
    include/Drlg/D2DrlgMaze.h
    include/Drlg/D2DrlgOutDesr.h
    include/Drlg/D2DrlgOutdoors.h
    include/Drlg/D2DrlgOutJung.h
    include/Drlg/D2DrlgOutPlace.h
    include/Drlg/D2DrlgOutRoom.h
    include/Drlg/D2DrlgOutSiege.h
    include/Drlg/D2DrlgOutWild.h
    include/Drlg/D2DrlgPreset.h
    include/Drlg/D2DrlgRoomTile.h
    include/Drlg/D2DrlgTileSub.h
)

# Items
target_sources(D2CommonStatic
  PRIVATE
    src/Items/Items.cpp
    src/Items/ItemMods.cpp
)

# Monsters
target_sources(D2CommonStatic
  PRIVATE
    src/Monsters/Monsters.cpp
    src/Monsters/MonsterRegion.cpp
    src/Monsters/MonsterRand.cpp
)

# Path
target_sources(D2CommonStatic
  PRIVATE
    src/Path/Path.cpp
    src/Path/PathMisc.cpp
    src/Path/IDAStar.cpp

    include/Path/Path.h
    include/Path/PathMisc.h
    include/Path/Path_IDAStar.h
)

# Units
target_sources(D2CommonStatic
  PRIVATE
    src/Units/Missile.cpp
    src/Units/MissileStream.cpp
    src/Units/UnitFinds.cpp
    src/Units/UnitRoom.cpp
    src/Units/Units.cpp

    include/Units/Item.h
    include/Units/Missile.h
    include/Units/MissileStream.h
    include/Units/Monster.h
    include/Units/Object.h
    include/Units/Player.h
    include/Units/UnitFinds.h
    include/Units/UnitRoom.h
    include/Units/Units.h
)

# Remaining files that were not placed into subfolders
target_sources(D2CommonStatic
  PRIVATE
    src/D2Chat.cpp
    src/D2Collision.cpp
    src/D2Composit.cpp
    src/D2Dungeon.cpp
    src/D2Environment.cpp
    src/D2Inventory.cpp
    src/D2Log.cpp
    src/D2Math.cpp
    src/D2QuestRecord.cpp
    src/D2Seed.cpp
    src/D2Skills.cpp
    src/D2States.cpp
    src/D2StatList.cpp
    src/D2Text.cpp
    src/D2Waypoints.cpp

    include/D2Chat.h
    include/D2Collision.h
    include/D2Combat.h
    include/D2Composit.h
    include/D2DataTbls.h
    include/D2Dungeon.h
    include/D2Environment.h
    include/D2Inventory.h
    include/D2ItemMods.h
    include/D2Items.h
    include/D2Log.h
    include/D2Math.h
    include/D2Monsters.h
    include/D2QuestRecord.h
    include/D2Roster.h
    include/D2Seed.h
    include/D2Skills.h
    include/D2States.h
    include/D2StatList.h
    include/D2Text.h
    include/D2Timer.h
    include/D2Waypoints.h
)

if(D2MOO_WITH_STATIC_TESTS)
  target_sources(D2CommonStatic
    PRIVATE
      statictests/D2StatList_staticchecks.cpp
  )
endif()

if(D2MOO_BUILD_TESTS)
    add_subdirectory(tests)
endif()

get_target_property(D2CommonSources D2CommonStatic SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2CommonSources}
)

# The D2Common.dll
add_library(D2Common
  # We want to create a detours dll for patching
  SHARED
  # The DLL entrypoint
  src/DllMain.cpp
  # The definition file that matches functions with their ordinals  
  definitions/D2Common.${D2MOO_ORDINALS_VERSION}.def
)

target_link_libraries(D2Common
# D2CommonStatic itself should not be linked by targets linking D2Common, hence why it is PRIVATE
  PRIVATE D2CommonStatic
# But we still its dependencies to be propagated so that users can consume them
  PUBLIC D2CommonDefinitions D2CMP D2Lang Fog
)
target_include_directories(D2Common INTERFACE include)
