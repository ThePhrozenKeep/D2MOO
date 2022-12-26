
add_library(Storm SHARED src/DllMain.cpp include/Storm.h include/StormHandles.h)
# The definition file that matches functions with their ordinals
target_sources(Storm PRIVATE definitions/Storm.${D2MOO_ORDINALS_VERSION}.def)
target_include_directories(Storm PUBLIC include)

# If not defined, we use declspec(dllimport), so only define it to build the .dll
target_compile_definitions(Storm PRIVATE STORM_IMPL)

target_link_libraries(Storm PUBLIC D2CommonDefinitions)
