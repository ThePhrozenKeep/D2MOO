#pragma once

#include <Windows.h>

#ifdef D2MCP_IMPL
#define D2MCP_DLL_DECL 
#else
#define D2MCP_DLL_DECL __declspec( dllimport )
#endif


void __cdecl D2MCPClientCloseMCP();