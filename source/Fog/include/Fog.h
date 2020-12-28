#pragma once

#include <D2Dll.h>
#include <D2DataTables.h>

#ifdef FOG_IMPL
#define FOG_DLL_DECL __declspec( dllexport )
#else
#define FOG_DLL_DECL __declspec( dllimport )
#endif

struct D2UnkFogStrc
{
	void* pCallback;			//0x00
	int unk0x04;				//0x04
};

D2FUNC_DLL(FOG, 10018, const char*, __cdecl, (char* szDest, const char* szFormat, ...), 0xDD90)														//Fog.#10018
D2FUNC_DLL(FOG, Assertion, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED30)												//Fog.#10023
D2FUNC_DLL(FOG, 10024_PacketAssertion, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED60)									//Fog.#10024
D2FUNC_DLL(FOG, 10025, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED90)													//Fog.#10025
D2FUNC_DLL(FOG, WriteToLogFile, void, __cdecl, (char* szFormat, ...), 0x120A0)																		//Fog.#10029
D2FUNC_DLL(FOG, AllocClientMemory, void*, __fastcall, (int nSize, char* szFile, int nLine, int n0), 0x8F50)											//Fog.#10042
D2FUNC_DLL(FOG, FreeClientMemory, void, __fastcall, (void* pFree, char* szFile, int nLine, int n0), 0x8F90)											//Fog.#10043
D2FUNC_DLL(FOG, AllocServerMemory, void*, __fastcall, (void* pMemPool, int nSize, char* szFile, int nLine, int n0), 0x8FF0)							//Fog.#10045
D2FUNC_DLL(FOG, FreeServerMemory, void, __fastcall, (void* pMemPool, void* pFree, char* szFile, int nLine, int n0), 0x9030)							//Fog.#10046
D2FUNC_DLL(FOG, ReallocServerMemory, void*, __fastcall, (void* pMemPool, void* pMemory, int nSize, char* szFile, int nLine, int n0), 0x9060)		//Fog.#10047
D2FUNC_DLL(FOG, 10050_EnterCriticalSection, void, __fastcall, (CRITICAL_SECTION* pCriticalSection, int nLine), 0xDC20)								//Fog.#10050
D2FUNC_DLL(FOG, MPQFileOpen, BOOL, __fastcall, (char* szFile, void** pFileHandle), 0x11600)															//Fog.#10102
D2FUNC_DLL(FOG, MPQFileClose, void, __fastcall, (void* pFile), 0x11610)																				//Fog.#10103
D2FUNC_DLL(FOG, MPQFileRead, BOOL, __fastcall, (void* pFile, void* pBuffer, size_t nSize, int* nBytesRead, DWORD, DWORD, DWORD), 0x11620)			//Fog.#10104
D2FUNC_DLL(FOG, MPQFileGetSize, size_t, __fastcall, (void* pFileHandle, DWORD* lpFileSizeHigh), 0x11650)											//Fog.#10105
D2FUNC_DLL(FOG, 10207, void, __stdcall, (D2BinFileStrc* pBinFile, D2BinFieldStrc* pBinField, void* pTxt, int nRecordCount, int nRecordSize), 0xAA60)//Fog.#10207
D2FUNC_DLL(FOG, CreateBinFile, D2BinFileStrc*, __stdcall, (void* pDataBuffer, int nBufferSize), 0xA8B0)												//Fog.#10208
D2FUNC_DLL(FOG, FreeBinFile, void, __stdcall, (D2BinFileStrc* pBinFile), 0xAA10)																	//Fog.#10209
D2FUNC_DLL(FOG, GetRecordCountFromBinFile, int, __stdcall, (D2BinFileStrc* pBinFile), 0xAA50)														//Fog.#10210
D2FUNC_DLL(FOG, AllocLinker, void*, __stdcall, (char* szFile, int nLine), 0xB720)																	//Fog.#10211
D2FUNC_DLL(FOG, FreeLinker, void, __stdcall, (void* pLinker), 0xB750)																				//Fog.#10212
D2FUNC_DLL(FOG, GetLinkIndex, int, __stdcall, (void* pLink, DWORD dwCode, BOOL bLogError), 0xB810)													//Fog.#10213
D2FUNC_DLL(FOG, GetStringFromLinkIndex, int, __stdcall, (void* pLinker, int nIndex, char* szString), 0xB8F0)										//Fog.#10214
D2FUNC_DLL(FOG, 10215, int, __stdcall, (void* pBin, int a2), 0xB990)																				//Fog.#10215
D2FUNC_DLL(FOG, 10216_AddRecordToLinkingTable, int, __stdcall, (void* pBin, char* a2), 0xBD80)														//Fog.#10216
D2FUNC_DLL(FOG, GetRowFromTxt, int, __stdcall, (void* pBin, char* szText, int nColumn), 0xBC20)														//Fog.#10217
D2FUNC_DLL(FOG, IsExpansion, int, __fastcall, (), 0xD730)																							//Fog.#10227
D2FUNC_DLL(FOG, 10253, int, __stdcall, (void*, int, void*, void*, int, void*), 0x1E30)																//Fog.#10253
D2FUNC_DLL(FOG, 10254, int, __stdcall, (char* pSrc, char* pBuffer, int nBufferSize, void*, void*, void*), 0x24F0)									//Fog.#10254
D2FUNC_DLL(FOG, 10255, char*, __stdcall, (void* pLinker, int nId, int a3), 0xBB20)																	//Fog.#10255


#define D2_ALLOC_CLIENT(size) FOG_AllocClientMemory((size), __FILE__, __LINE__, 0)
#define D2_CALLOC_CLIENT(size) memset(FOG_AllocClientMemory(size, __FILE__, __LINE__, 0), 0x00, size)
#define D2_ALLOC_STRC_CLIENT(type) (type*)FOG_AllocClientMemory(sizeof(type), __FILE__, __LINE__, 0)
#define D2_CALLOC_STRC_CLIENT(type) (type*)memset(FOG_AllocClientMemory(sizeof(type), __FILE__, __LINE__, 0), 0x00, sizeof(type))
#define D2_FREE_CLIENT(ptr) FOG_FreeClientMemory(ptr, __FILE__, __LINE__, 0)


#define D2_ALLOC_SERVER(size, pMemPool) FOG_AllocServerMemory(pMemPool, size, __FILE__, __LINE__, 0)
#define D2_CALLOC_SERVER(size, pMemPool) memset(FOG_AllocServerMemory(pMemPool, size, __FILE__, __LINE__, 0),0x00,size)
#define D2_REALLOC_SERVER(pMem, size, pMemPool) FOG_ReallocServerMemory(pMemPool, pMem, size, __FILE__, __LINE__, 0)
#define D2_ALLOC_STRC_SERVER(type, pMemPool) (type*)FOG_AllocServerMemory(pMemPool, sizeof(type), __FILE__, __LINE__, 0)
#define D2_CALLOC_STRC_SERVER(type, pMemPool) (type*)memset(FOG_AllocServerMemory(pMemPool, sizeof(type), __FILE__, __LINE__, 0), 0x00, sizeof(type))
#define D2_FREE_SERVER(pMemPool, ptr) FOG_FreeServerMemory(pMemPool, ptr, __FILE__, __LINE__, 0)

#define D2_ASSERT(expr) __analysis_assume(!!(expr)); (void)( (!!(expr)) || (FOG_Assertion(#expr, __FILE__, __LINE__), exit(-1) , 0))
#define D2_ASSERTM(expr,msg) __analysis_assume(!!(expr)); (void)( (!!(expr)) || (FOG_Assertion(msg, __FILE__, __LINE__), exit(-1) , 0))
#define D2_UNREACHABLE D2_ASSERT(false)