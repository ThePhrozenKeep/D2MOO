
add_library(D2Gfx 
  # We want to create a detours dll for patching
  SHARED
  # The DLL entrypoint
  src/DllMain.cpp
)

target_link_libraries(D2Gfx PRIVATE Fog Storm D2CommonDefinitions)

target_include_directories(D2Gfx PUBLIC include)

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(D2Gfx)

target_sources(D2Gfx
  PRIVATE
    src/CmnSubtile.cpp
    src/D2Gfx.cpp
    src/Scale.cpp
    src/Texture.cpp
    src/Tile.cpp
    src/Window.cpp

    include/CmnSubtile.h
    include/DisplayType.h
    include/DrawMode.h
    include/D2Gfx.h
    include/Scale.h
    include/Texture.h
    include/Tile.h
    include/Window.h
)

get_target_property(D2GfxSources D2Gfx SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2GfxSources}
)
