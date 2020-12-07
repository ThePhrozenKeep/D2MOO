cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project("D2MOO" LANGUAGES C CXX VERSION 0.1.0)

set(D2MOO_ORDINALS_VERSION "1.10f" CACHE STRING "Name of the version to be used for ordinals and patches")

option(ENABLE_D2DETOURS_EMBEDDED_PATCHES "Build the DLL with builtin patching using D2.Detours" ON)
set(D2DETOURS_EMBEDDED_PATCHES_DIR "D2.Detours.patches" CACHE PATH "The directory to your D2.Detours embedded patches")


if(NOT ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    message(WARNING "Diablo2 is 32bits only, 64bits build are unsupported. Invoke CMake with '-A Win32'")
endif()

# Standard CMake modules
include(CMakeDependentOption)# This is a really useful scripts that creates options that depends on other options. It can even be used with generator expressions !

# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 3rd party dependencies
add_subdirectory(external)

add_subdirectory(source)

if(ENABLE_D2DETOURS_EMBEDDED_PATCHES)
  add_subdirectory(${D2DETOURS_EMBEDDED_PATCHES_DIR})
endif()

###############
## Packaging ##
###############

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  include(GNUInstallDirs)
  
  install(TARGETS D2.DetoursLauncher D2.Detours)
  
  install(TARGETS D2Common
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/patch
  )
  install(FILES README.md TYPE DOC)

  set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)
  include(CPack)
endif()

