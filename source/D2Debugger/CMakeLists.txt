
add_library(D2Debugger SHARED 
  src/D2Debugger.cpp
  src/D2Debugger.Game.cpp
  src/D2Debugger.imgui.d3d9.cpp
  
  # Font awesome 6
  #src/fa-solid-900.cpp
  src/IconsFontAwesome6.h
  
  include/D2Debugger.h
)
target_include_directories(D2Debugger PUBLIC include)
target_compile_definitions(D2Debugger PRIVATE D2DEBUGGER_IMPL)

target_link_libraries(D2Debugger 
  PRIVATE
    D2Common
    D2Game
    imgui::imgui d3d9
)
# These are required because somehow loading a .dll that loads DirectX during LoadLibrary crashes.
# So we just delay its loading until first use.
if(MSVC)
    target_link_options(D2Debugger PRIVATE "/DELAYLOAD:d3d9.dll" "Delayimp.lib")
endif()
