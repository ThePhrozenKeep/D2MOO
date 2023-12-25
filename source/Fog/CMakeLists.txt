
add_library(Fog
  SHARED
    src/DllMain.cpp
	src/D2BitManip.cpp
	src/Safesock.cpp
	src/Calc.cpp
	
	include/Fog.h
	include/D2BitManip.h
	include/Safesock.h
)

if(D2MOO_WITH_STATIC_TESTS)
  target_sources(Fog
    PRIVATE
      statictests/Fog_staticchecks.cpp
  )
endif()

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(Fog)
target_include_directories(Fog PUBLIC include)

target_link_libraries(Fog PUBLIC D2CommonDefinitions Storm)
