
D2MOO_prepare_targets(D2Lang FORCE_EXPORT)
target_link_libraries(${D2LangImplName} PUBLIC D2CommonDefinitions D2Hell Fog)

target_sources(${D2LangImplName}
  PRIVATE
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

D2MOO_target_source_group(D2Lang)
