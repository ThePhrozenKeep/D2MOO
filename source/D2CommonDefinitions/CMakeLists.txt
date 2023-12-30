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
    include/D2BuildInformation.h
    include/D2Config.h
)
target_include_directories(D2CommonDefinitions INTERFACE include)
# Unless we get rid of <Windows.h> in D2BasicTypes.h, we might as well define this for all consumers.
target_compile_definitions(D2CommonDefinitions INTERFACE NOMINMAX WIN32_LEAN_AND_MEAN)

include(CMakePrintHelpers)

if(D2MOO_ORDINALS_VERSION)
  set(D2_VERSION_REGEX "^([0-9]+)\\.([0-9][0-9])([A-Za-z])(-(.+))?$")
  if(D2MOO_ORDINALS_VERSION MATCHES ${D2_VERSION_REGEX})
    set(D2MOO_ORDINAL_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(D2MOO_ORDINAL_VERSION_MINOR ${CMAKE_MATCH_2})
    set(D2MOO_ORDINAL_VERSION_PATCH ${CMAKE_MATCH_3})
    set(D2MOO_ORDINAL_VERSION_SPECIAL ${CMAKE_MATCH_5})
    set(D2MOO_ORDINAL_VERSION_DEFINE_NAME "D2_VERSION_${D2MOO_ORDINAL_VERSION_MAJOR}${D2MOO_ORDINAL_VERSION_MINOR}${D2MOO_ORDINAL_VERSION_PATCH}")
    string(TOUPPER "${D2MOO_ORDINAL_VERSION_DEFINE_NAME}" D2MOO_ORDINAL_VERSION_DEFINE_NAME)

    cmake_print_variables(D2MOO_ORDINAL_VERSION_MAJOR D2MOO_ORDINAL_VERSION_MINOR D2MOO_ORDINAL_VERSION_PATCH D2MOO_ORDINAL_VERSION_SPECIAL D2MOO_ORDINAL_VERSION_DEFINE_NAME)

	string(REGEX REPLACE "^0+" "" D2MOO_ORDINAL_VERSION_MINOR_NO_LEADING_ZEROS ${D2MOO_ORDINAL_VERSION_MINOR})
    target_compile_definitions(D2CommonDefinitions
      INTERFACE 
        ${D2MOO_ORDINAL_VERSION_DEFINE_NAME}
        D2_VERSION_MAJOR=${D2MOO_ORDINAL_VERSION_MAJOR}
        D2_VERSION_MINOR=${D2MOO_ORDINAL_VERSION_MINOR_NO_LEADING_ZEROS}
        D2_VERSION_PATCH='${D2MOO_ORDINAL_VERSION_PATCH}' # Allow only single character
        $<$<NOT:$<STREQUAL:D2MOO_ORDINAL_VERSION_SPECIAL,"">>:D2_VERSION_SPECIAL="${D2MOO_ORDINAL_VERSION_SPECIAL}">
    )
  else()
    message(FATAL_ERROR "Ordinal version format is incorrect. Needs to match '${D2_VERSION_REGEX}'")
  endif()
endif()
