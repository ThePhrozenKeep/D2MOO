
add_library(D2CMP SHARED src/DllMain.cpp include/D2CMP.h)

# The definition file that matches functions with their ordinals
target_sources(D2CMP PRIVATE definitions/D2CMP.${D2MOO_ORDINALS_VERSION}.def)
target_include_directories(D2CMP PUBLIC include)
# If not defined, we use declspec(dllimport), so only define it to build the .dll

target_compile_definitions(D2CMP PRIVATE D2CMP_IMPL)

target_link_libraries(D2CMP PUBLIC D2CommonDefinitions)
