
add_library(Storm SHARED src/DllMain.cpp include/Storm.h include/StormHandles.h)
# The definition file that matches functions with their ordinals
D2MOO_configure_exports(Storm)
target_include_directories(Storm PUBLIC include)

target_link_libraries(Storm PUBLIC D2CommonDefinitions squall::Storm)
target_compile_definitions(Storm PUBLIC WITH_SQUALL=1)