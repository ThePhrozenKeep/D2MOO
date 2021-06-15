

function(D2MOO_add_detours_patch_to_dll DLLTargetName)
  if(ARGV1)
    set(PatchCppFile ${ARGV1})
  else()
    set(PatchCppFile "${D2MOO_ORDINALS_VERSION}/${DLLTargetName}.patch.cpp")
  endif()
  message(STATUS "[${DLLTargetName}]PatchCppFile: '${PatchCppFile}'")
  
  target_sources(${DLLTargetName} PRIVATE ${PatchCppFile})
  target_link_libraries(${DLLTargetName} PRIVATE D2.Detours)
  add_dependencies(${DLLTargetName} D2.DetoursLauncher)

  set_target_properties(${DLLTargetName}
    PROPERTIES
      VS_DEBUGGER_COMMAND $<TARGET_FILE:D2.DetoursLauncher>
      VS_DEBUGGER_ENVIRONMENT "DIABLO2_PATCH=$<TARGET_FILE_DIR:${DLLTargetName}>"
  )

  if(D2_EXE)
    set_target_properties(${DLLTargetName} 
      PROPERTIES 
        VS_DEBUGGER_COMMAND_ARGUMENTS "\"${D2_EXE}\""
        VS_DEBUGGER_WORKING_DIRECTORY "${D2_PATH}"
    )
  endif()

endfunction()

function(D2MOO_install_dll DLLName)
  install(TARGETS ${DLLName}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/patch
  )
endfunction()

function(D2MOO_install_mod_dll ModName DLLName)
  set(target_name "${DLLName}.${ModName}")
  install(TARGETS ${target_name}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/patch
  )
endfunction()

function(D2MOO_add_mod_dll ModName DLLName)
  set(target_name "${DLLName}.${ModName}")
  message(STATUS "Creating '${target_name}' target for '${DLLName}.dll'")
  # We name the target so that it doesnt conflict with D2MOO
  add_library(${target_name} SHARED)
  # However, we still want to output it with the correct dll name.
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${DLLName})
  
  if(ENABLE_D2DETOURS_EMBEDDED_PATCHES AND IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${DLLName}/patch")
    message(STATUS "Adding patch for ${DLLName} ${D2MOO_ORDINALS_VERSION}")
    D2MOO_add_detours_patch_to_dll(${target_name} "${CMAKE_CURRENT_LIST_DIR}/${DLLName}/patch/${DLLName}.${D2MOO_ORDINALS_VERSION}.patch.cpp")
  else()
    message(STATUS "Expected to find ${CMAKE_CURRENT_LIST_DIR}/${DLLName}/patch")
  endif()
  
  D2MOO_install_mod_dll(${ModName} ${DLLName})
endfunction()

macro(D2MOO_add_mod_dll_if_exists dir)
    if (IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${dir}")
		D2MOO_add_mod_dll(${PROJECT_NAME} ${dir})
	endif ()
endmacro()

macro(D2MOO_add_subdirectory_if_exists dir)
	if (IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${dir}")
		add_subdirectory(${dir})
	endif ()
endmacro()


macro(D2MOO_add_subdirectories_for_d2_known_dlls ModName)
  set(D2_KNOWN_DLLS    binkw32     Bnclient     D2Client     D2CMP     D2Common     D2DDraw     D2Direct3D     D2Game     D2Gdi     D2gfx     D2Glide     D2Lang     D2Launch     D2MCPClient     D2Multi     D2Net     D2sound     D2Win     Fog     ijl11     SmackW32     Storm 
  )
  # Start by creating all targets so that we can later link to them without caring about order.
  foreach(dllname ${D2_KNOWN_DLLS})
    D2MOO_add_mod_dll_if_exists(${dllname})
  endforeach()
  # Now that all targets are correctly setup, we can let the user do what they need to.
  foreach(dllname ${D2_KNOWN_DLLS})
    D2MOO_add_subdirectory_if_exists(${dllname})
  endforeach()
endmacro()
