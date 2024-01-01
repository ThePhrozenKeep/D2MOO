
D2MOO_prepare_targets(D2CMP)
target_link_libraries(${D2CMPImplName} PUBLIC D2CommonDefinitions Fog)

target_sources(${D2CMPImplName}
  PRIVATE
  
    src/D2CMP.cpp
    include/D2CMP.h

    src/CelCmp.cpp
    src/CelDataHash.cpp
    src/Codec.cpp
    src/Count.cpp
    src/DrwCntxt.cpp
    src/FastCmp.cpp
    src/FindTiles.cpp
    src/GfxHash.cpp
    src/LRUCache.cpp
    src/Palette.cpp
    src/PalShift.cpp
    src/Raw.cpp
    src/SpriteCache.cpp
    src/SubTile.cpp
    src/TileCache.cpp
    src/Tilecmp.cpp
    src/TileLib.cpp
    src/TileProjects.cpp

    include/CelCmp.h
    include/CelDataHash.h
    include/Codec.h
    include/Count.h
    include/DrwCntxt.h
    include/FastCmp.h
    include/FindTiles.h
    include/GfxHash.h
    include/LRUCache.h
    include/Palette.h
    include/PalShift.h
    include/Raw.h
    include/SpriteCache.h
    include/SubTile.h
    include/TileCache.h
    include/Tilecmp.h
    include/TileLib.h
    include/TileProjects.h
)

D2MOO_target_source_group(D2CMP)

