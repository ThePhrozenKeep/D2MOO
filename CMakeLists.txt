cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project("D2MOO" LANGUAGES C CXX VERSION 0.1.0)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(D2MOO_IS_ROOT_PROJECT TRUE)
endif()

# Standard CMake modules
include(CMakeDependentOption)           # This is a really useful scripts that creates options that depends on other options. It can even be used with generator expressions !
include(GNUInstallDirs)                 # This will define the default values for installation directories (all platforms even if named GNU)
include(InstallRequiredSystemLibraries) # Tell CMake that the `install` target needs to install required system libraries (eg: Windows SDK)
include(CTest)                          # Must be called before adding tests but after calling project(). This automatically calls enable_testing() and configures ctest targets when using Make/Ninja
include(cmake/D2MOO.ModHelpers.cmake)   # Used to make modders life easier

#####################
##  D2MOO OPTIONS  ##
#####################

set(D2MOO_ORDINALS_VERSION "1.10f" CACHE STRING "Name of the version to be used for ordinals and patches")
option(ENABLE_D2DETOURS_EMBEDDED_PATCHES "Build the DLL with builtin patching using D2.Detours" ON)
if(D2MOO_IS_ROOT_PROJECT) # Let parent project decide wether to use its own patches or not
	set(D2DETOURS_EMBEDDED_PATCHES_DIR "${EMBEDDED_PATCHES_DIR_PREFIX}D2.Detours.patches" CACHE PATH "The directory to your D2.Detours embedded patches. Defaults to the root D2.Detours.patches directory")
endif()	

option(D2MOO_INSTALL "Should we install the D2MOO targets?" ${D2MOO_IS_ROOT_PROJECT})
option(D2MOO_WITH_STATIC_TESTS "Enable static tests for struct layouts. Use for original game only, not mods." ON)
cmake_dependent_option(D2MOO_BUILD_TESTS
    "Enable D2Moo project tests targets" ON # By default we want tests if CTest is enabled
    "BUILD_TESTING" OFF # Stay coherent with CTest variables
)

if(CMAKE_GENERATOR MATCHES "Visual Studio" AND MSVC)
  option(D2MOO_ENABLE_PARALLEL_BUILD "Force /MP parallel compilation" ${D2MOO_IS_ROOT_PROJECT})
  if(D2MOO_ENABLE_PARALLEL_BUILD)
    add_compile_options("/MP")
  endif()
endif()


if(NOT ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
  message(WARNING "Diablo2 is 32bits only, 64bits build are unsupported. Invoke CMake with '-A Win32'")
endif()

# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 3rd party dependencies
add_subdirectory(external)

add_subdirectory(source)

if(ENABLE_D2DETOURS_EMBEDDED_PATCHES AND D2DETOURS_EMBEDDED_PATCHES_DIR)
  add_subdirectory(${D2DETOURS_EMBEDDED_PATCHES_DIR} ${D2DETOURS_EMBEDDED_PATCHES_BINARY_DIR})
endif()

#===========#
#   Tests   #
#===========#

if(D2MOO_BUILD_TESTS)
    # Let the user add options to the test runner if needed
    set(TEST_RUNNER_PARAMS "--force-colors=true" CACHE STRING "Options to add to our test runners commands")
    # In a real project you most likely want to exclude test folders
    # list(APPEND CUSTOM_COVERAGE_EXCLUDE "/test/")
    
    # You can setup some custom variables and add them to the CTestCustom.cmake.in template to have custom ctest settings
    # For example, you can exclude some directories from the coverage reports such as third-parties and tests
    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/CTestCustom.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake
        @ONLY
    )
endif()

###############
## Packaging ##
###############

if(${D2MOO_INSTALL})
  include(GNUInstallDirs)
  
  install(TARGETS D2.DetoursLauncher D2.Detours)
  
  install(TARGETS D2Common
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/patch
  )
  install(FILES README.md TYPE DOC)

  set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)
  include(CPack)
endif()

