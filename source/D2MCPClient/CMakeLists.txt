
D2MOO_prepare_targets(D2MCPClient)

target_link_libraries(${D2MCPClientImplName} PRIVATE D2CommonDefinitions Fog Storm)

target_sources(${D2MCPClientImplName}
  PRIVATE
	src/McpConnect.cpp
	src/ToMCP.cpp

	include/D2MCPClient.h
	include/McpConnect.h
)
D2MOO_target_source_group(D2MCPClient)
