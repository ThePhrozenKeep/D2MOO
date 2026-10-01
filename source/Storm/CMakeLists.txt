
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
	target_link_libraries(${StormImplName} PUBLIC D2CommonDefinitions)
	target_compile_definitions(${StormImplName} PUBLIC WITH_SQUALL=0)
	target_sources(${StormImplName}
	  PRIVATE
	    src/Storm.cpp
	    src/CCritSect.cpp
	    include/storm/hash/Hashkey.cpp

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