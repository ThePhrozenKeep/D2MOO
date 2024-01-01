
add_executable(Game WIN32 
  src/Main.cpp
  include/Main.h
  
  resources/Game.rc
)

if(EXISTS resources/Game.${D2MOO_ORDINALS_VERSION}.rc)
  target_sources(Game PRIVATE resources/Game.${D2MOO_ORDINALS_VERSION}.rc)
endif()

target_include_directories(Game PRIVATE include)
target_link_libraries(Game 
  PRIVATE
    Fog
	Storm
	D2Win
	D2Gfx
	D2Sound
	D2MCPClient
)
set_target_properties(Game PROPERTIES OUTPUT_NAME Game.D2MOO)
