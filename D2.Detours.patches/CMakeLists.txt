# We embed the patching ordinal information directly into the .DLLs

D2MOO_register_D2_DLL_patches()
D2MOO_add_detours_patch_to_dll(D2Debugger)

set_property(DIRECTORY ${D2MOO_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT D2Debugger)
