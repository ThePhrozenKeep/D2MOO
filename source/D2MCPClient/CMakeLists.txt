
add_library(D2MCPClient
  SHARED
    src/DllMain.cpp
	
	include/D2MCPClient.h
)
target_include_directories(D2MCPClient PUBLIC include)

# The definition file that matches functions with their ordinals
D2MOO_configure_exports(D2MCPClient)

get_target_property(D2MCPClientSources D2MCPClient SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2MCPClientSources}
)
