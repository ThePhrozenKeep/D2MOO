
add_library(Fog SHARED src/DllMain.cpp include/Fog.h)
# The definition file that matches functions with their ordinals
target_sources(Fog PRIVATE definitions/Fog.${D2MOO_ORDINALS_VERSION}.def)
target_include_directories(Fog PUBLIC include)
# If not defined, we use declspec(dllimport), so only define it to build the .dll

target_compile_definitions(Fog PRIVATE FOG_IMPL)

target_link_libraries(Fog PUBLIC D2CommonDefinitions)
