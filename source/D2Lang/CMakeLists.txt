
add_library(D2LangStatic
  STATIC
    src/D2LangStubs.cpp
    src/D2Crc16.cpp
    src/D2Unicode/D2UnicodeChar.cpp
    src/D2Unicode/D2UnicodeStr.cpp
    src/D2Unicode/D2UnicodeUtf.cpp
    src/D2StrTable.cpp

    include/D2Lang.h
    include/D2Crc16.h
    include/D2StrTable.h
    include/D2Unicode.h
)
target_include_directories(D2LangStatic PUBLIC include)
# If not defined, we use declspec(dllimport), so only define it to build the .dll
target_compile_definitions(D2LangStatic PRIVATE D2LANG_EXPORT)
target_link_libraries(D2LangStatic PUBLIC D2CommonDefinitions D2Hell Fog)


add_library(D2Lang SHARED src/DllMain.cpp)
# The definition file that matches functions with their ordinals
target_sources(D2Lang PRIVATE definitions/D2Lang.${D2MOO_ORDINALS_VERSION}.def)
target_link_libraries(D2Lang PUBLIC D2LangStatic)
target_compile_definitions(D2Lang INTERFACE D2LANG_IMPORT)

if(D2MOO_BUILD_TESTS)
    add_subdirectory(tests)
endif()
