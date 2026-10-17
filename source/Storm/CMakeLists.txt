
D2MOO_prepare_targets(Storm)

# Vendored, VC6-compatible reimplementation of the Storm containers in
# include/storm. These are D2MOO's own headers; the retail Storm entry points
# remain declared in include/Storm.h.
#
# Layout and behaviour are ABI-critical: D2MOO inherits these classes inside
# structs whose sizes and offsets are asserted against the retail game.
add_library(stormvendored STATIC
    src/CCritSect.cpp
    include/storm/hash/Hashkey.cpp
)
target_include_directories(stormvendored PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(stormvendored PUBLIC D2CommonDefinitions)
target_compile_definitions(stormvendored PRIVATE STORM_DLL_DECL= "__declspec( dllimport )")

target_link_libraries(${StormImplName} PUBLIC D2CommonDefinitions stormvendored)
target_sources(${StormImplName}
  PRIVATE
    src/Storm.cpp

    include/Storm.h
    include/StormHandles.h

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

D2MOO_target_source_group(Storm)
