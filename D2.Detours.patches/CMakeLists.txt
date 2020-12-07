# We embed the patching ordinal information directly into the .DLLs

target_sources(D2Common PRIVATE ${D2MOO_ORDINALS_VERSION}/D2Common.patch.cpp)
target_link_libraries(D2Common PRIVATE D2.Detours)
add_dependencies(D2Common D2.DetoursLauncher)

set_target_properties(D2Common
  PROPERTIES
    VS_DEBUGGER_COMMAND $<TARGET_FILE:D2.DetoursLauncher>
    VS_DEBUGGER_WORKING_DIRECTORY "${D2_PATH}"
    VS_DEBUGGER_ENVIRONMENT "DIABLO2_PATCH=$<TARGET_FILE_DIR:D2Common>"
)

if(D2_EXE)
  set_target_properties(D2Common PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "\"${D2_EXE}\"")
else()
  message(STATUS "D2 executable was not found, please set the `D2_EXE` CMake variable for debugging.")
endif()

set_property(DIRECTORY ${D2MOO_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT D2Common)
