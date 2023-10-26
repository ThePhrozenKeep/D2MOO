
add_library(D2Sound
  SHARED
    src/DllMain.cpp
	
	include/D2Sound.h
)
target_include_directories(D2Sound PUBLIC include)
target_link_libraries(D2Sound PUBLIC D2CommonDefinitions)

# The definition file that matches functions with their ordinals
target_sources(D2Sound PRIVATE definitions/D2Sound.${D2MOO_ORDINALS_VERSION}.def)

# If not defined, we use declspec(dllimport), so only define it to build the .dll
target_compile_definitions(D2Sound PRIVATE D2SOUND_IMPL)

get_target_property(D2SoundSources D2Sound SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2SoundSources}
)
