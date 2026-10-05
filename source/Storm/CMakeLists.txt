
D2MOO_prepare_targets(Storm)

if(D2MOO_USE_SQUALL)
	target_link_libraries(${StormImplName} PUBLIC D2CommonDefinitions squall::Storm)
	target_compile_definitions(${StormImplName} PUBLIC WITH_SQUALL=1)
	target_sources(${StormImplName}
	  PRIVATE
	    src/Storm.cpp

	    include/Storm.h
	    include/StormHandles.h
	)
else()
	# Vendored, VC6-compatible reimplementation of the Squall Storm containers.
	# The headers live in include/storm, which this target already exports, so
	# `#include <storm/...>` resolves to them without touching any consumer.
	# WITH_SQUALL=0 also makes Storm.h declare the retail Storm imports itself
	# instead of pulling them in from Squall's headers.
	#
	# Two of the vendored classes have out-of-line members (CCritSect wraps a
	# CRITICAL_SECTION, HASHKEY_STR owns a Storm allocation), so they cannot be
	# header-only. They go into a static library that every consumer of
	# <storm/...> links, which is exactly how the squallstorm target supplies
	# the real squall's CCritSect.cpp; without it the consumers -- D2Game
	# embeds a CCritSect by value in its game-data table -- would have nothing
	# to link the member functions against.
	add_library(stormvendored STATIC
	    src/CCritSect.cpp
	    include/storm/hash/Hashkey.cpp
	)
	target_include_directories(stormvendored PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_link_libraries(stormvendored PUBLIC D2CommonDefinitions)
	target_compile_definitions(stormvendored PUBLIC WITH_SQUALL=0)
	# Hashkey.cpp goes through Storm.h, which annotates the retail imports with
	# this. Consumers get the same value from Storm's own interface, so it only
	# has to be set while compiling these two files.
	target_compile_definitions(stormvendored PRIVATE STORM_DLL_DECL= "__declspec( dllimport )")

	target_link_libraries(${StormImplName} PUBLIC D2CommonDefinitions stormvendored)
	target_compile_definitions(${StormImplName} PUBLIC WITH_SQUALL=0)
	target_sources(${StormImplName}
	  PRIVATE
	    src/Storm.cpp

	    include/Storm.h
	    include/StormHandles.h

	    include/storm/StormCompat.hpp
	    include/storm/Array.hpp
	    include/storm/Error.hpp
	    include/storm/List.hpp
	    include/storm/Memory.hpp
	    include/storm/String.hpp
	    include/storm/array/TSBaseArray.hpp
	    include/storm/array/TSFixedArray.hpp
	    include/storm/array/TSGrowableArray.hpp
	    include/storm/hash/Hashkey.hpp
	    include/storm/hash/TSExportTableSimpleReuse.hpp
	    include/storm/hash/TSExportTableSyncReuse.hpp
	    include/storm/hash/TSHashObject.hpp
	    include/storm/hash/TSHashObjectChunk.hpp
	    include/storm/hash/TSHashTable.hpp
	    include/storm/hash/TSHashTableReuse.hpp
	    include/storm/list/TSExplicitList.hpp
	    include/storm/list/TSGetExplicitLink.hpp
	    include/storm/list/TSGetLink.hpp
	    include/storm/list/TSLink.hpp
	    include/storm/list/TSLinkedNode.hpp
	    include/storm/list/TSList.hpp
	    include/storm/thread/CCritSect.hpp
	)
endif() # D2MOO_USE_SQUALL

D2MOO_target_source_group(Storm)