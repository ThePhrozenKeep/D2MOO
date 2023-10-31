add_library(D2Win 
  # We want to create a detours dll for patching
  SHARED
  # The DLL entrypoint
  src/DllMain.cpp
)

target_link_libraries(D2Win PRIVATE Fog D2CMP D2Gfx D2Lang Storm D2Hell D2Sound stb::stb)

target_include_directories(D2Win PUBLIC include)

# The definition file that matches functions with their ordinals
target_sources(D2Win PRIVATE definitions/D2Win.${D2MOO_ORDINALS_VERSION}.def)


target_sources(D2Win
  PRIVATE
    src/D2Comp.cpp
    src/D2WinAccountList.cpp
    src/D2WinAnimImage.cpp
    src/D2WinArchive.cpp
    src/D2WinButton.cpp
    src/D2WinEditBox.cpp
    src/D2WinFont.cpp
    src/D2WinImage.cpp
    src/D2WinList.cpp
    src/D2WinMain.cpp
    src/D2WinPalette.cpp
    src/D2WinPopUp.cpp
    src/D2WinProgressBar.cpp
    src/D2WinScrollBar.cpp
    src/D2WinSmack.cpp
    src/D2WinTextBox.cpp
    src/D2WinTimer.cpp
    src/D2WRegister.cpp
    src/JpegLibraryWrapper.cpp
    src/JpegLibraryWrapper.h

    include/Font.h
    include/D2WinComp.h
    include/D2WinAccountList.h
    include/D2WinAnimImage.h
    include/D2WinArchive.h
    include/D2WinButton.h
    include/D2WinControlHeader.h
    include/D2WinEditBox.h
    include/D2WinFont.h
    include/D2WinImage.h
    include/D2WinList.h
    include/D2WinMain.h
    include/D2WinPalette.h
    include/D2WinPopUp.h
    include/D2WinProgressBar.h
    include/D2WinScrollBar.h
    include/D2WinSmack.h
    include/D2WinTextBox.h
    include/D2WinTimer.h
    include/D2WRegister.h
)

get_target_property(D2WinSources D2Win SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${D2WinSources}
)
