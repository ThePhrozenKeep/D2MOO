add_library(TestDefinitions STATIC src/TestDefinitions.cpp)

target_sources(TestDefinitions
  PRIVATE
)
target_include_directories(TestDefinitions PUBLIC include)
target_link_libraries(TestDefinitions PRIVATE D2CommonDefinitions D2Common VisitorFramework)
target_compile_features(TestDefinitions PRIVATE cxx_std_17)

get_target_property(TestDefinitionsSources TestDefinitions SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TestDefinitionsSources})
