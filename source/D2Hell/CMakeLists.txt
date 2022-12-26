# This library is linked statically by each DLL.
# Any modules (exe or DLL) linking it will need to include Fault.h after defining XFAULT_IMPL and XFAULT_MODULE_NAME in EXACTLY one of its translation units (.cpp file).

add_library(D2Hell
  STATIC
    src/Archive.cpp
    src/CRC.cpp
    src/Fault.cpp
    src/Window.cpp

    include/Archive.h
    include/CRC.h
    include/Fault.h
    include/Window.h
)
# The definition file that matches functions with their ordinals
target_include_directories(D2Hell PUBLIC include)

target_link_libraries(D2Hell PUBLIC Fog Storm)
