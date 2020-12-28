#pragma once

#include <D2Dll.h>

#ifdef D2CMP_IMPL
#define D2CMP_DLL_DECL // We use .def files, not dllexport
#else
#define D2CMP_DLL_DECL __declspec( dllimport )
#endif

struct D2TileLibraryEntryStrc;
struct D2TileLibraryHashStrc;
struct D2TileLibraryEntryStrc;

D2FUNC_DLL(D2CMP, 10078_GetTileIndex, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFF30)											//D2Cmp.#10078
D2FUNC_DLL(D2CMP, 10079_GetTileFlags, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFF60)											//D2Cmp.#10079
D2FUNC_DLL(D2CMP, 10081_GetTileFrame, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFC0)											//D2Cmp.#10081
D2FUNC_DLL(D2CMP, 10082_GetTileSubIndex, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFF0)										//D2Cmp.#10082
D2FUNC_DLL(D2CMP, 10085_GetTileFlagArray, BYTE*, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0x10080)									//D2Cmp.#10085
D2FUNC_DLL(D2CMP, 10087_LoadTileLibrarySlot, void, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, const char* szFileName), 0xFDE0)			//D2Cmp.#10087
D2FUNC_DLL(D2CMP, 10088_GetTileSize, int, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, int nOrientation, int nIndex, int nSubIndex, D2TileLibraryEntryStrc** pTileList, int nTileListSize), 0xFE70)//D2Cmp.#10088
D2FUNC_DLL(D2CMP, MixPalette, BYTE*, __stdcall, (BYTE nTrans, int nColor), 0xB760)																	//D2Cmp.#10098
