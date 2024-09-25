
D2MOO_prepare_targets(Fog)

target_link_libraries(${FogImplName} PUBLIC D2CommonDefinitions Storm)

target_sources(${FogImplName}
  PRIVATE
	src/Fog.cpp
	
	# Official file names
	src/AsyncData.cpp
	src/BitManip/BitManip.cpp
	src/BitManip/StringPack.cpp
	src/D2QSQueue.cpp
	src/D2QSSocket.cpp
	src/DataArrays.cpp
	src/ErrorManager.cpp
	src/ErrorManager.cpp
	src/Excel/Excel.cpp
	src/Exceptions.cpp
	src/Gamma.cpp
	src/IFF.cpp
	src/LogManager.cpp
	src/Mem/mem.cpp
	src/QServer/QServer.cpp
	src/QServer/QServer98.cpp
	src/QServer/QServerNT.cpp
	src/Safesock.cpp
	src/String/FogString.cpp
	src/System/System.cpp
	
	# Made up filename
	src/Calc.cpp
	
	# Headers
	include/Fog.h
	include/D2BitManip.h
	include/Safesock.h
	include/System/System.h
)

if(D2MOO_WITH_STATIC_TESTS)
  target_sources(Fog
    PRIVATE
      statictests/Fog_staticchecks.cpp
  )
endif()

D2MOO_target_source_group(Fog)
