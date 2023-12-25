
add_library(D2Net
  SHARED
    src/DllMain.cpp

    include/Client.h
    include/D2Net.h
    include/Packet.h
    include/Server.h
)

target_link_libraries(D2Net PRIVATE D2CommonDefinitions Fog Storm ws2_32)

target_include_directories(D2Net PUBLIC include)

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(D2Net)

target_sources(D2Net
  PRIVATE

    src/Client.cpp
    src/D2Net.cpp
    src/Server.cpp
)

get_target_property(D2NetSources D2Net SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2NetSources}
)
