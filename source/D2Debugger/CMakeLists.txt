
add_library(D2Debugger SHARED 
  src/D2Debugger.cpp
  src/D2Debugger.Game.cpp
  src/D2Debugger.imgui.d3d9.cpp
  src/D2Debugger.patch.common.cpp
  
  # Font awesome 6
  #src/fa-solid-900.cpp
  src/IconsFontAwesome6.h
  
  include/D2Debugger.h
  include/D2Debugger.patch.common.h
)
target_include_directories(D2Debugger PUBLIC include)
D2MOO_configure_exports(D2Debugger IGNORE_MISSING_DEF)

target_link_libraries(D2Debugger 
  PRIVATE
    D2Common
    D2Game
    D2Client
    imgui::imgui d3d9
)
# These are required because somehow loading a .dll that loads DirectX during LoadLibrary crashes.
# So we just delay its loading until first use.
if(MSVC)
    target_link_options(D2Debugger PRIVATE "/DELAYLOAD:d3d9.dll" "Delayimp.lib")
endif()

set_target_properties(D2Debugger
    PROPERTIES
      VS_DEBUGGER_COMMAND_ARGUMENTS "-- -debug"
)