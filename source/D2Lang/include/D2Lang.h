#pragma once

#include <D2Dll.h>

#ifdef D2LANG_IMPL
#define D2LANG_DLL_DECL // We use .def files, not dllexport
#else
#define D2LANG_DLL_DECL __declspec( dllimport )
#endif

D2FUNC_DLL(D2LANG, GetStringByReferenceString, wchar_t*, __fastcall, (char* string), 0x3BC0)			//D2LANG.#10003
D2FUNC_DLL(D2LANG, GetStringFromTblIndex, wchar_t*, __fastcall, (short index), 0x3740)					//D2LANG.#10004
D2FUNC_DLL(D2LANG, GetLocaleId, int, __fastcall, (), 0x3DF0)											//D2LANG.#10007
D2FUNC_DLL(D2LANG, GetTblIndex, short, __stdcall, (char* szReference, wchar_t** pUnicode), 0x3960)		//D2LANG.#10013

