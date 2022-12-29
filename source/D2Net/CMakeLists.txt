
add_library(D2Net
  SHARED
    src/DllMain.cpp
	
	include/D2Net.h
)

# The definition file that matches functions with their ordinals
target_sources(D2Net PRIVATE definitions/D2Net.${D2MOO_ORDINALS_VERSION}.def)
target_include_directories(D2Net PUBLIC include)

# If not defined, we use declspec(dllimport), so only define it to build the .dll
target_compile_definitions(D2Net PRIVATE D2NET_IMPL)

target_link_libraries(D2Net PUBLIC Fog)
