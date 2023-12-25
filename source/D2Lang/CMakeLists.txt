
add_library(D2LangStatic
  STATIC
    src/D2LangStubs.cpp
    src/D2Crc16.cpp
    src/D2Unicode/D2UnicodeChar.cpp
    src/D2Unicode/D2UnicodeStr.cpp
    src/D2Unicode/D2UnicodeUtf.cpp
    src/D2Unicode/D2UnicodeWin.cpp
    src/D2StrTable.cpp

    include/D2Lang.h
    include/D2Crc16.h
    include/D2StrTable.h
    include/D2Unicode.h
)
target_include_directories(D2LangStatic PUBLIC include)
target_link_libraries(D2LangStatic PUBLIC D2CommonDefinitions D2Hell Fog)


add_library(D2Lang SHARED src/DllMain.cpp)
target_link_libraries(D2Lang PUBLIC D2LangStatic)

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(D2Lang FORCE_EXPORT)

if(D2MOO_BUILD_TESTS)
    add_subdirectory(tests)
endif()
