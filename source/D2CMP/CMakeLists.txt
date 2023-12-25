
add_library(D2CMP
  SHARED
    src/DllMain.cpp
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

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(D2CMP)
target_include_directories(D2CMP PUBLIC include)

target_link_libraries(D2CMP PUBLIC D2CommonDefinitions Fog)
