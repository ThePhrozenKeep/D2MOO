
D2MOO_prepare_targets(D2Gfx)

target_link_libraries(${D2GfxImplName} PRIVATE Fog Storm D2CommonDefinitions)

target_sources(${D2GfxImplName}
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

D2MOO_target_source_group(D2Gfx)
