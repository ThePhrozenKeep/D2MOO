
add_library(D2Sound SHARED src/DllMain.cpp include/D2Sound.h)

# The definition file that matches functions with their ordinals
target_sources(D2Sound PRIVATE definitions/D2Sound.${D2MOO_ORDINALS_VERSION}.def)
target_include_directories(D2Sound PUBLIC include)
# If not defined, we use declspec(dllimport), so only define it to build the .dll

target_compile_definitions(D2Sound PRIVATE D2SOUND_IMPL)

target_link_libraries(D2Sound PUBLIC D2CommonDefinitions)
