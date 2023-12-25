
add_library(D2Sound
  SHARED
    src/DllMain.cpp
	
	include/D2Sound.h
)
target_include_directories(D2Sound PUBLIC include)
target_link_libraries(D2Sound PUBLIC D2CommonDefinitions)

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(D2Sound)

get_target_property(D2SoundSources D2Sound SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2SoundSources}
)
