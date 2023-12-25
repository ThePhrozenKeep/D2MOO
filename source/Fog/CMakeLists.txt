
D2MOO_prepare_targets(Fog)

target_link_libraries(${FogImplName} PUBLIC D2CommonDefinitions Storm)

target_sources(${FogImplName}
  PRIVATE
	src/Fog.cpp
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

D2MOO_target_source_group(Fog)
