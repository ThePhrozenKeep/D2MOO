
add_library(D2MCPClient
  SHARED
    src/DllMain.cpp
	
	include/D2MCPClient.h
)
target_include_directories(D2MCPClient PUBLIC include)

# The definition file that matches functions with their ordinals
target_sources(D2MCPClient PRIVATE definitions/D2MCPClient.${D2MOO_ORDINALS_VERSION}.def)

# If not defined, we use declspec(dllimport), so only define it to build the .dll
target_compile_definitions(D2MCPClient PRIVATE D2MCP_IMPL)

get_target_property(D2MCPClientSources D2MCPClient SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2MCPClientSources}
)
