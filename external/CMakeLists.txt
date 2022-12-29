
# Disable clang-tidy for external dependencies.
# This should really be avoided but that's what we get for including deps directly...
set(CMAKE_CXX_CLANG_TIDY "")

if(ENABLE_D2DETOURS_EMBEDDED_PATCHES)
  add_subdirectory(D2.Detours)
  
  # Look for D2SE first, then Diablo II.
  find_program(D2_EXE NAMES D2SE.exe "Game.exe" "Diablo II.exe"
    HINTS $ENV{D2SE}
    PATH_SUFFIXES "Diablo II"
  )
  if(D2_EXE)
    get_filename_component(D2_PATH ${D2_EXE} DIRECTORY CACHE)
  else()
    message(STATUS "D2 executable was not found, please set the `D2_EXE` CMake variable for debugging if your game executable is not at the install location.")
  endif()
endif()




#Doctest for unit tests
add_library(doctest INTERFACE)
target_include_directories(
    doctest
    INTERFACE
    doctest/doctest # note : will expose the parts/ folder...
)
add_library(doctest::doctest ALIAS doctest)
