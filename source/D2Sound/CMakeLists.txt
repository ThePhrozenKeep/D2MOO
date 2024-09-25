
D2MOO_prepare_targets(D2Sound)

target_link_libraries(${D2SoundImplName} PUBLIC D2CommonDefinitions)

target_sources(${D2SoundImplName}
  PRIVATE
    src/D2Sound.cpp

	include/D2Sound.h
)

D2MOO_target_source_group(D2Sound)
