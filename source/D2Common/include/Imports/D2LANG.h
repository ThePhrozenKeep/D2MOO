#pragma once


D2FUNC(D2LANG, GetStringByReferenceString, wchar_t*, __fastcall, (char* string), 0x3BC0)														//D2Lang.#10003
D2FUNC(D2LANG, GetStringFromTblIndex, wchar_t*, __fastcall, (short index), 0x3740)																//D2Lang.#10004
D2FUNC(D2LANG, GetLocaleId, int, __fastcall, (), 0x3DF0)																						//D2Lang.#10007
D2FUNC(D2LANG, GetTblIndex, short, __stdcall, (char* szReference, wchar_t** pUnicode), 0x3960)													//D2Lang.#10013

