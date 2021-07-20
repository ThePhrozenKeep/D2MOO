
add_library(D2Lang
  SHARED
    src/DllMain.cpp
    src/D2Unicode/D2UnicodeChar.cpp
    src/D2Unicode/D2UnicodeStr.cpp

    include/D2Lang.h
    include/D2Unicode.h
)
# The definition file that matches functions with their ordinals
target_sources(D2Lang PRIVATE definitions/D2Lang.${D2MOO_ORDINALS_VERSION}.def)
target_include_directories(D2Lang PUBLIC include)

# If not defined, we use declspec(dllimport), so only define it to build the .dll
target_compile_definitions(D2Lang PRIVATE D2LANG_IMPL)

target_link_libraries(D2Lang PUBLIC D2CommonDefinitions)
