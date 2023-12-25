
D2MOO_prepare_targets(D2Net)

target_link_libraries(${D2NetImplName} PRIVATE D2CommonDefinitions Fog Storm ws2_32)

target_sources(${D2NetImplName}
  PRIVATE
    src/Client.cpp
    src/D2Net.cpp
    src/Server.cpp

    include/Client.h
    include/D2Net.h
    include/Packet.h
    include/Server.h
)

D2MOO_target_source_group(D2Net)
