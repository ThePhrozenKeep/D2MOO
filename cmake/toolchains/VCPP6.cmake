get_filename_component(_VCPP6_ROOT_DEFAULT "C:/MSVC600/VC98" ABSOLUTE)
set(VCPP6_ROOT "$ENV{VCPP6_ROOT}" CACHE PATH "Root of the VC6 (VC98) installation")

if(NOT EXISTS "${VCPP6_ROOT}/Bin/cl.exe")
  if(NOT EXISTS "${_VCPP6_ROOT_DEFAULT}/Bin/cl.exe")
    message(FATAL_ERROR
      "Could not find cl.exe in '${VCPP6_ROOT}/Bin'.\n"
      "Set VCPP6_ROOT (cache variable or environment variable) to the root of your "
      "VC98 installation, eg. C:\\MSVC600\\VC98")
  endif()
  set(VCPP6_ROOT "${_VCPP6_ROOT_DEFAULT}" CACHE PATH "Root of the VC6 (VC98) installation" FORCE)
endif()

set(VC98_BIN "${VCPP6_ROOT}/Bin")

if(NOT EXISTS "${VC98_BIN}/mspdb60.dll")
  find_file(_VCPP6_MSPDB60 mspdb60.dll
    HINTS "${VCPP6_ROOT}/../Common/MSDev98/Bin" "${VCPP6_ROOT}/Common/MSDev98/Bin"
  )
  if(_VCPP6_MSPDB60)
    message(STATUS "Copying MSPDB60.DLL into '${VC98_BIN}' (required by cl.exe)")
    configure_file("${_VCPP6_MSPDB60}" "${VC98_BIN}/mspdb60.dll" COPYONLY)
  else()
    message(WARNING "mspdb60.dll not found next to cl.exe. cl.exe will fail to start unless it is on PATH.")
  endif()
endif()

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION "") # VC6 only ever targeted Windows 95/98/NT4

set(CMAKE_C_COMPILER   "${VC98_BIN}/cl.exe")
set(CMAKE_CXX_COMPILER "${VC98_BIN}/cl.exe")

# rc.exe and mt.exe are part of the Visual Studio common tools, not VC98 itself.
find_program(_VCPP6_RC rc.exe
  HINTS "${VCPP6_ROOT}/../Common/MSDev98/Bin" "${VCPP6_ROOT}/Common/MSDev98/Bin"
)
if(_VCPP6_RC)
  set(CMAKE_RC_COMPILER "${_VCPP6_RC}")
endif()
find_program(_VCPP6_MT mt.exe
  HINTS "${VCPP6_ROOT}/../Common/MSDev98/Bin" "${VCPP6_ROOT}/Common/MSDev98/Bin"
)
if(_VCPP6_MT)
  set(CMAKE_MT_COMPILER "${_VCPP6_MT}")
endif()

set(CMAKE_AR      "${VC98_BIN}/lib.exe"  CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB  "${VC98_BIN}/lib.exe"  CACHE FILEPATH "" FORCE)
set(CMAKE_LINKER  "${VC98_BIN}/link.exe" CACHE FILEPATH "" FORCE)

set(CMAKE_MAKE_PROGRAM "${VC98_BIN}/nmake.exe" CACHE FILEPATH "Program used to build the project")

set(VCPP6_INCLUDE_DIRS
  "${VCPP6_ROOT}/Include"
  "${VCPP6_ROOT}/ATL/Include"
  "${VCPP6_ROOT}/MFC/Include"
)
set(VCPP6_LIB_DIRS
  "${VCPP6_ROOT}/Lib"
  "${VCPP6_ROOT}/MFC/Lib"
)

# So that <D2StdInt.h>, which includes <VCPP6.h>, can find it. The /FI below is what
# makes the shims apply to every TU; this only makes the header reachable by name.
list(APPEND VCPP6_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}")
list(TRANSFORM VCPP6_INCLUDE_DIRS PREPEND "/I")
string(REPLACE ";" " " VCPP6_INCLUDE_FLAGS "${VCPP6_INCLUDE_DIRS}")

list(TRANSFORM VCPP6_LIB_DIRS PREPEND "/LIBPATH:")
string(REPLACE ";" " " VCPP6_LIB_FLAGS "${VCPP6_LIB_DIRS}")

set(CMAKE_C_FLAGS_INIT   "${CMAKE_C_FLAGS_INIT} ${VCPP6_INCLUDE_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} ${VCPP6_INCLUDE_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /FI\"${CMAKE_CURRENT_LIST_DIR}/VCPP6.h\"")

foreach(_config "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
  set(CMAKE_EXE_LINKER_FLAGS${_config}_INIT        "${CMAKE_EXE_LINKER_FLAGS${_config}_INIT} ${VCPP6_LIB_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS${_config}_INIT     "${CMAKE_SHARED_LINKER_FLAGS${_config}_INIT} ${VCPP6_LIB_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS${_config}_INIT      "${CMAKE_MODULE_LINKER_FLAGS${_config}_INIT} ${VCPP6_LIB_FLAGS}")
  set(CMAKE_STATIC_LINKER_FLAGS${_config}_INIT      "${CMAKE_STATIC_LINKER_FLAGS${_config}_INIT} ${VCPP6_LIB_FLAGS}")
endforeach()

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)
