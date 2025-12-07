
D2MOO_prepare_targets(Storm)

target_link_libraries(${StormImplName} PUBLIC D2CommonDefinitions squall::Storm)
target_compile_definitions(${StormImplName} PUBLIC WITH_SQUALL=1)

target_sources(${StormImplName}
  PRIVATE
    src/Storm.cpp

    include/Storm.h
    include/StormHandles.h
)

D2MOO_target_source_group(Storm)