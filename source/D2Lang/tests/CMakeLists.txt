# Note :
# Tests in static libraries might not get registered, see https://github.com/onqtam/doctest/blob/master/doc/markdown/faq.md#why-are-my-tests-in-a-static-library-not-getting-registered
# For this reason, and because it is interesting to have individual
# test executables for each library, it is suggested not to put tests directly in the libraries (even though doctest advocates this usage)
# Creating multiple executables is of course not mandatory, and one could use the same executable with various command lines to filter what tests to run.

add_executable(D2LangTests D2LangTests.cpp)
target_link_libraries(D2LangTests PRIVATE doctest::doctest ${D2LangImplName})
target_compile_definitions(D2LangTests PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
target_compile_features(D2LangTests PRIVATE cxx_std_17)

set_target_properties(D2LangTests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/workingDirectory
)

add_test(
    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME D2LangTests.unittests
    COMMAND D2LangTests ${TEST_RUNNER_PARAMS}
    WORKING_DIRECTORY $<TARGET_PROPERTY:D2LangTests,VS_DEBUGGER_WORKING_DIRECTORY>
)


