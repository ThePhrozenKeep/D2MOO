# Common definitions for all DLLs.

# So that the project appear in IDEs with cmake >= 3.19
if(${CMAKE_VERSION} VERSION_GREATER 3.19.0)
  set(SOURCES_TRANSITIVE_USAGE PRIVATE)
else()
  set(SOURCES_TRANSITIVE_USAGE INTERFACE)
endif()

add_library(D2CommonDefinitions INTERFACE)

target_sources(D2CommonDefinitions
  ${SOURCES_TRANSITIVE_USAGE}
    include/D2BasicTypes.h
    include/D2Constants.h
    include/D2Math.h
    include/D2Structs.OtherDLLs.h
    include/D2PacketDef.h
    include/D2Dll.h
    include/D2Config.h
)
target_include_directories(D2CommonDefinitions INTERFACE include)
# Unless we get rid of <Windows.h> in D2BasicTypes.h, we might as well define this for all consumers.
target_compile_definitions(D2CommonDefinitions INTERFACE NOMINMAX WIN32_LEAN_AND_MEAN)

if(D2MOO_ORDINALS_VERSION)
  string(REPLACE "." "" VERSION_DEFINE_NAME "D2_VERSION_${D2MOO_ORDINALS_VERSION}")
  string(TOUPPER "${VERSION_DEFINE_NAME}" VERSION_DEFINE_NAME)
  target_compile_definitions(D2CommonDefinitions INTERFACE ${VERSION_DEFINE_NAME})
endif()
