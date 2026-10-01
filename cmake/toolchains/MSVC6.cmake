get_filename_component(_MSVC6_ROOT_DEFAULT "C:/MSVC600/VC98" ABSOLUTE)
set(MSVC6_ROOT "$ENV{MSVC6_ROOT}" CACHE PATH "Root of the VC6 (VC98) installation")

if(NOT EXISTS "${MSVC6_ROOT}/Bin/cl.exe")
  if(NOT EXISTS "${_MSVC6_ROOT_DEFAULT}/Bin/cl.exe")
    message(FATAL_ERROR
      "Could not find cl.exe in '${MSVC6_ROOT}/Bin'.\n"
      "Set MSVC6_ROOT (cache variable or environment variable) to the root of your "
      "VC98 installation, eg. C:\\MSVC600\\VC98")
  endif()
  set(MSVC6_ROOT "${_MSVC6_ROOT_DEFAULT}" CACHE PATH "Root of the VC6 (VC98) installation" FORCE)
endif()

set(VC98_BIN "${MSVC6_ROOT}/Bin")

if(NOT EXISTS "${VC98_BIN}/mspdb60.dll")
  find_file(_MSVC6_MSPDB60 mspdb60.dll
    HINTS "${MSVC6_ROOT}/../Common/MSDev98/Bin" "${MSVC6_ROOT}/Common/MSDev98/Bin"
  )
  if(_MSVC6_MSPDB60)
    message(STATUS "Copying MSPDB60.DLL into '${VC98_BIN}' (required by cl.exe)")
    configure_file("${_MSVC6_MSPDB60}" "${VC98_BIN}/mspdb60.dll" COPYONLY)
  else()
    message(WARNING "mspdb60.dll not found next to cl.exe. cl.exe will fail to start unless it is on PATH.")
  endif()
endif()

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION "") # VC6 only ever targeted Windows 95/98/NT4

set(CMAKE_C_COMPILER   "${VC98_BIN}/cl.exe")
set(CMAKE_CXX_COMPILER "${VC98_BIN}/cl.exe")

find_program(_MSVC6_RC rc.exe
  HINTS "${MSVC6_ROOT}/../Common/MSDev98/Bin" "${MSVC6_ROOT}/Common/MSDev98/Bin"
)
if(_MSVC6_RC)
  set(CMAKE_RC_COMPILER "${_MSVC6_RC}")
endif()
find_program(_MSVC6_MT mt.exe
  HINTS "${MSVC6_ROOT}/../Common/MSDev98/Bin" "${MSVC6_ROOT}/Common/MSDev98/Bin"
)
if(_MSVC6_MT)
  set(CMAKE_MT_COMPILER "${_MSVC6_MT}")
endif()

set(CMAKE_AR      "${VC98_BIN}/lib.exe"  CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB  "${VC98_BIN}/lib.exe"  CACHE FILEPATH "" FORCE)
set(CMAKE_LINKER  "${VC98_BIN}/link.exe" CACHE FILEPATH "" FORCE)

set(CMAKE_MAKE_PROGRAM "${VC98_BIN}/nmake.exe" CACHE FILEPATH "Program used to build the project")

set(MSVC6_INCLUDE_DIRS
  "${MSVC6_ROOT}/Include"
  "${MSVC6_ROOT}/ATL/Include"
  "${MSVC6_ROOT}/MFC/Include"
)
set(MSVC6_LIB_DIRS
  "${MSVC6_ROOT}/Lib"
  "${MSVC6_ROOT}/MFC/Lib"
)

list(TRANSFORM MSVC6_INCLUDE_DIRS PREPEND "/I")
string(REPLACE ";" " " MSVC6_INCLUDE_FLAGS "${MSVC6_INCLUDE_DIRS}")

list(TRANSFORM MSVC6_LIB_DIRS PREPEND "/LIBPATH:")
string(REPLACE ";" " " MSVC6_LIB_FLAGS "${MSVC6_LIB_DIRS}")

set(CMAKE_C_FLAGS_INIT   "${CMAKE_C_FLAGS_INIT} ${MSVC6_INCLUDE_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} ${MSVC6_INCLUDE_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE")

foreach(_config "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
  set(CMAKE_EXE_LINKER_FLAGS${_config}_INIT        "${CMAKE_EXE_LINKER_FLAGS${_config}_INIT} ${MSVC6_LIB_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS${_config}_INIT     "${CMAKE_SHARED_LINKER_FLAGS${_config}_INIT} ${MSVC6_LIB_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS${_config}_INIT      "${CMAKE_MODULE_LINKER_FLAGS${_config}_INIT} ${MSVC6_LIB_FLAGS}")
  set(CMAKE_STATIC_LINKER_FLAGS${_config}_INIT      "${CMAKE_STATIC_LINKER_FLAGS${_config}_INIT} ${MSVC6_LIB_FLAGS}")
endforeach()

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)