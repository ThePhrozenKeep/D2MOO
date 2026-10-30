# Note :
# Tests in static libraries might not get registered, see https://github.com/onqtam/doctest/blob/master/doc/markdown/faq.md#why-are-my-tests-in-a-static-library-not-getting-registered
# For this reason, and because it is interesting to have individual
# test executables for each library, it is suggested not to put tests directly in the libraries (even though doctest advocates this usage)
# Creating multiple executables is of course not mandatory, and one could use the same executable with various command lines to filter what tests to run.

add_executable(D2GfxTests D2GfxTests.cpp D2GfxTestDefines.h)

target_sources(D2GfxTests
  PRIVATE
    CmnSubtileTests.cpp
    GfxTests.cpp
    ScaleTests.cpp
    TextureTests.cpp
    TileTests.cpp
    WindowTests.cpp
)

get_target_property(D2GfxTestsSources D2GfxTests SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${D2GfxTestsSources})

target_include_directories(D2GfxTests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
# The headers of D2Gfx also need D2CommonDefinitions, which is a private dependency of the library
target_link_libraries(D2GfxTests PRIVATE doctest::doctest TestDefinitions TestUtilities VisitorFramework D2CommonDefinitions D2Hell Fog Storm ${D2GfxImplName})
target_compile_features(D2GfxTests PRIVATE cxx_std_17)

set_target_properties(D2GfxTests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../testWorkingDirectory
)

add_test(
    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME D2GfxTests.unittests
    COMMAND D2GfxTests ${TEST_RUNNER_PARAMS}
    WORKING_DIRECTORY $<TARGET_PROPERTY:D2GfxTests,VS_DEBUGGER_WORKING_DIRECTORY>
)
