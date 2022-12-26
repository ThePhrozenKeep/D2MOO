#pragma once

#include <D2Dll.h>
#include <D2BasicTypes.h>
#include <stdlib.h>

#ifdef FOG_IMPL
#define FOG_DLL_DECL 
#else
#define FOG_DLL_DECL __declspec( dllimport )
#endif

#pragma pack(1)

struct D2TxtLinkNodeStrc
{
	char szText[32];				//0x00
	int nLinkIndex;					//0x20
	D2TxtLinkNodeStrc* pPrevious;	//0x24
	D2TxtLinkNodeStrc* pNext;		//0x28
};

struct D2TxtLinkTblStrc
{
	union
	{
		char szCode[4];				//0x00
		uint32_t dwCode;			//0x00
	};
	int nLinkIndex;					//0x04
};

struct D2TxtLinkStrc
{
	int32_t nRecords;				//0x00
	int32_t nAllocatedCells;		//0x04
	D2TxtLinkTblStrc* pTbl;			//0x08
	D2TxtLinkNodeStrc* pFirstNode;	//0x0C
};


struct D2BinFileStrc
{
	uint8_t* pDataBuffer;			//0x00
	uint8_t* pData;					//0x04
	int32_t nRecordCount;			//0x08
	int32_t nCellCount;				//0x0C
};

enum D2C_TxtFieldTypes : uint32_t
{
	TXTFIELD_NONE,
	TXTFIELD_ASCII,
	TXTFIELD_DWORD,
	TXTFIELD_WORD,
	TXTFIELD_BYTE,
	TXTFIELD_UNKNOWN1,
	TXTFIELD_UNKNOWN2,
	TXTFIELD_BYTE2,
	TXTFIELD_DWORD2,
	TXTFIELD_RAW,
	TXTFIELD_ASCIITOCODE,
	TXTFIELD_UNKNOWN3,
	TXTFIELD_UNKNOWN4,
	TXTFIELD_CODETOBYTE,
	TXTFIELD_UNKNOWN5,
	TXTFIELD_CODETOWORD,
	TXTFIELD_UNKNOWN6,
	TXTFIELD_NAMETOINDEX,
	TXTFIELD_NAMETOINDEX2,
	TXTFIELD_NAMETODWORD,
	TXTFIELD_NAMETOWORD,
	TXTFIELD_NAMETOWORD2,
	TXTFIELD_KEYTOWORD,
	TXTFIELD_CUSTOMLINK,
	TXTFIELD_UNKNOWN7,
	TXTFIELD_CALCTODWORD,
	TXTFIELD_BIT
};

struct D2BinFieldStrc
{
	char* szFieldName;				//0x00
	D2C_TxtFieldTypes nFieldType;	//0x04
	int32_t nFieldLength;			//0x08
	int32_t nFieldOffset;			//0x0C
	void* pLinkField;				//0x10
};

struct D2UnkExcelStrc
{
	D2UnkExcelStrc* pNext;			//0x00
	uint32_t dwHash;				//0x04
	D2BinFieldStrc* pBinField;		//0x08
};

struct D2UnkFogStrc
{
	void* pCallback;				//0x00
	int32_t unk0x04;				//0x04
};

#ifdef DECLARE_STRICT_HANDLE
#error "Should move this define out of fog.h"
#endif
#define  DECLARE_STRICT_HANDLE(name)        typedef struct name##__ { int unused; } *name

// Those are actully Storm.dll types
DECLARE_STRICT_HANDLE(HSFILE);
DECLARE_STRICT_HANDLE(HSARCHIVE);


enum D2C_AsyncDataPriority
{
	ASYNC_DATA_PRIORITY_NORMAL = 0,
	ASYNC_DATA_PRIORITY_HIGH = 1,
};

enum D2C_AsyncDataFlags
{
	ASYNC_DATA_FLAGS_LOADED = 0x01,
	
	ASYNC_DATA_FLAGS_DEBUG_TAG      = 0x41737960,
	ASYNC_DATA_FLAGS_DEBUG_TAG_MASK = 0xFFFFFFF0,
};

struct AsyncDataLoadFileCallback
{
	DWORD(__stdcall* callback)(int a1, int a3, int C);
	DWORD userData[6];
};

struct AsyncDataEventSlot;

struct AsyncData
{
	HSFILE hFile;
	DWORD pMemPool;
	OVERLAPPED tOverlapped;
	AsyncDataLoadFileCallback ayncOpDesc;
	AsyncDataEventSlot* pEventSlot;
	void* pBuffer;
	DWORD nFileSize;
	DWORD nPriority; // D2C_AsyncDataPriority
	DWORD dwFlags;
	DWORD dword4C; // Also storing priority ?
	DWORD dword50;
};

struct AsyncDataEventSlot
{
	HANDLE hEvent;
	DWORD dw04;
	AsyncData* pAsyncOp;
};

#pragma pack()

typedef int (*D2ExceptionCallback)();

D2FUNC_DLL(FOG, csprintf, const char*, __cdecl, (char* szDest, const char* szFormat, ...), 0xDD90)													//Fog.#10018
D2FUNC_DLL(FOG, InitErrorMgr, void, __fastcall, (const char* szProgramName, D2ExceptionCallback pExceptionCallback, const char* szVersion, BOOL bLogToFile), 0xE1B0)	//Fog.#10019
D2FUNC_DLL(FOG, 10021, int, __fastcall, (const char* szLogName), 0xE1A0)																			//Fog.#10021
D2FUNC_DLL(FOG, GetSystemInfo, void*, __cdecl, (), 0xDF20)																							//Fog.#10022
D2FUNC_DLL(FOG, DisplayAssert, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED30)											//Fog.#10023
D2FUNC_DLL(FOG, DisplayHalt, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED60)												//Fog.#10024
D2FUNC_DLL(FOG, DisplayWarning, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED90)											//Fog.#10025
D2FUNC_DLL(FOG, DisplayError, void, __cdecl, (int nCategory, const char* szMsg, const char* szFile, int nLine), 0xEDF0)								//Fog.#10026
D2FUNC_DLL(FOG, Trace, void, __cdecl, (const char* szFormat, ...), 0x120A0)																			//Fog.#10029
D2FUNC_DLL(FOG, Alloc, void*, __fastcall, (int nSize, const char* szFile, int nLine, int n0), 0x8F50)												//Fog.#10042
D2FUNC_DLL(FOG, Free, void, __fastcall, (void* pFree, const char* szFile, int nLine, int n0), 0x8F90)												//Fog.#10043
D2FUNC_DLL(FOG, AllocPool, void*, __fastcall, (void* pMemPool, int nSize, const char* szFile, int nLine, int n0), 0x8FF0)							//Fog.#10045
D2FUNC_DLL(FOG, FreePool, void, __fastcall, (void* pMemPool, void* pFree, const char* szFile, int nLine, int n0), 0x9030)							//Fog.#10046
D2FUNC_DLL(FOG, ReallocPool, void*, __fastcall, (void* pMemPool, void* pMemory, int nSize, const char* szFile, int nLine, int n0), 0x9060)			//Fog.#10047
D2FUNC_DLL(FOG, 10050_EnterCriticalSection, void, __fastcall, (CRITICAL_SECTION* pCriticalSection, int nLine), 0xDC20)								//Fog.#10050
D2FUNC_DLL(FOG, 10083_Cos_LUT, float, __stdcall, (int16_t index), 0x1DF0)																			//Fog.#10083
D2FUNC_DLL(FOG, 10084_Sin_LUT, float, __stdcall, (int16_t index), 0x1E10)																			//Fog.#10084
D2FUNC_DLL(FOG, AsyncDataInitialize, void, __fastcall, (BOOL bEnableAsyncOperations), 0xC010)														//Fog.#10089
D2FUNC_DLL(FOG, AsyncDataDestroy, void, __fastcall, (), 0xC0E0)																						//Fog.#10090
D2FUNC_DLL(FOG, AsyncDataLoadFileEx, AsyncData*, __fastcall, (void* pMemPool, const char* pFileName, BOOL bAllowAsync, LONG nFileOffset, int nFileSize, void* pBuffer, AsyncDataLoadFileCallback* asyncOpDesc, int a8, const char* sourceFile, int sourceLine), 0xC2B0)	//Fog.#10091
D2FUNC_DLL(FOG, AsyncDataTestLoaded, BOOL, __fastcall, (AsyncData* pAsyncData), 0xC480)																//Fog.#10092
D2FUNC_DLL(FOG, AsyncDataWaitAndGetBuffer, void, __fastcall, (AsyncData* pAsyncData), 0xC630)														//Fog.#10093
D2FUNC_DLL(FOG, AsyncDataGetBuffer, void*, __fastcall, (AsyncData* pAsyncData), 0xC700)																//Fog.#10094
D2FUNC_DLL(FOG, AsyncDataGetBufferSize, DWORD, __fastcall, (AsyncData* pAsyncData), 0xC790)															//Fog.#10095
D2FUNC_DLL(FOG, AsyncDataSetResults, void, __fastcall, (AsyncData* pAsyncData, void* pBuffer, int nBufferSize), 0xC7E0)								//Fog.#10096
D2FUNC_DLL(FOG, AsyncDataFree, void, __fastcall, (AsyncData* pAsyncData), 0xC880)																	//Fog.#10097
D2FUNC_DLL(FOG, AsyncDataSetPriority, void, __fastcall, (AsyncData* pAsyncData, int nPriority), 0xC4E0)												//Fog.#10099
D2FUNC_DLL(FOG, AsyncDataHandlePriorityChanges, void, __fastcall, (BOOL bPostpone), 0xC5A0)															//Fog.#10100
D2FUNC_DLL(FOG, MPQSetConfig, BOOL, __fastcall, (int dwDirectAccessFlags, int bEnableSeekOptimization), 0x11590)									//Fog.#10101
D2FUNC_DLL(FOG, MPQFileOpen, BOOL, __fastcall, (const char* szFile, HSFILE* pFileHandle), 0x11600)													//Fog.#10102
D2FUNC_DLL(FOG, MPQFileClose, void, __fastcall, (HSFILE pFile), 0x11610)																			//Fog.#10103
D2FUNC_DLL(FOG, MPQFileRead, BOOL, __fastcall, (HSFILE pFile, void* pBuffer, size_t nSize, int* nBytesRead, uint32_t, uint32_t, uint32_t), 0x11620)	//Fog.#10104
D2FUNC_DLL(FOG, MPQFileGetSize, size_t, __fastcall, (HSFILE pFileHandle, uint32_t* lpFileSizeHigh), 0x11650)										//Fog.#10105
D2FUNC_DLL(FOG, 10207, void, __stdcall, (D2BinFileStrc* pBinFile, D2BinFieldStrc* pBinField, void* pTxt, int nRecordCount, int nRecordSize), 0xAA60)//Fog.#10207
D2FUNC_DLL(FOG, CreateBinFile, D2BinFileStrc*, __stdcall, (void* pDataBuffer, int nBufferSize), 0xA8B0)												//Fog.#10208
D2FUNC_DLL(FOG, FreeBinFile, void, __stdcall, (D2BinFileStrc* pBinFile), 0xAA10)																	//Fog.#10209
D2FUNC_DLL(FOG, GetRecordCountFromBinFile, int, __stdcall, (D2BinFileStrc* pBinFile), 0xAA50)														//Fog.#10210
D2FUNC_DLL(FOG, AllocLinker, void*, __stdcall, (const char* szFile, int nLine), 0xB720)																//Fog.#10211
D2FUNC_DLL(FOG, FreeLinker, void, __stdcall, (void* pLinker), 0xB750)																				//Fog.#10212
D2FUNC_DLL(FOG, GetLinkIndex, int, __stdcall, (void* pLink, uint32_t dwCode, BOOL bLogError), 0xB810)												//Fog.#10213
D2FUNC_DLL(FOG, GetStringFromLinkIndex, int, __stdcall, (void* pLinker, int nIndex, char* szString), 0xB8F0)										//Fog.#10214
D2FUNC_DLL(FOG, 10215, int, __stdcall, (void* pBin, int a2), 0xB990)																				//Fog.#10215
D2FUNC_DLL(FOG, 10216_AddRecordToLinkingTable, int, __stdcall, (void* pBin, char* a2), 0xBD80)														//Fog.#10216
D2FUNC_DLL(FOG, GetRowFromTxt, int, __stdcall, (void* pBin, char* szText, int nColumn), 0xBC20)														//Fog.#10217
D2FUNC_DLL(FOG, IsExpansion, int, __fastcall, (), 0xD730)																							//Fog.#10227
D2FUNC_DLL(FOG, 10253, int, __stdcall, (void*, int, void*, void*, int, void*), 0x1E30)																//Fog.#10253
D2FUNC_DLL(FOG, 10254, int, __stdcall, (char* pSrc, char* pBuffer, int nBufferSize, void*, void*, void*), 0x24F0)									//Fog.#10254
D2FUNC_DLL(FOG, 10255, char*, __stdcall, (void* pLinker, int nId, int a3), 0xBB20)																	//Fog.#10255


#define D2_ALLOC(size) FOG_Alloc((size), __FILE__, __LINE__, 0)
#define D2_CALLOC(size) memset(FOG_Alloc(size, __FILE__, __LINE__, 0), 0x00, size)
#define D2_ALLOC_STRC(type) (type*)FOG_Alloc(sizeof(type), __FILE__, __LINE__, 0)
#define D2_CALLOC_STRC(type) (type*)memset(FOG_Alloc(sizeof(type), __FILE__, __LINE__, 0), 0x00, sizeof(type))
#define D2_FREE(ptr) FOG_Free(ptr, __FILE__, __LINE__, 0)


#define D2_ALLOC_POOL(pMemPool, size) FOG_AllocPool(pMemPool, size, __FILE__, __LINE__, 0)
#define D2_CALLOC_POOL(pMemPool, size) memset(FOG_AllocPool(pMemPool, size, __FILE__, __LINE__, 0),0x00,size)
#define D2_REALLOC_POOL(pMemPool, pMem, size) FOG_ReallocPool(pMemPool, pMem, size, __FILE__, __LINE__, 0)
#define D2_ALLOC_STRC_POOL(pMemPool, type) (type*)FOG_AllocPool(pMemPool, sizeof(type), __FILE__, __LINE__, 0)
#define D2_CALLOC_STRC_POOL(pMemPool, type) (type*)memset(FOG_AllocPool(pMemPool, sizeof(type), __FILE__, __LINE__, 0), 0x00, sizeof(type))
#define D2_FREE_POOL(pMemPool, ptr) FOG_FreePool(pMemPool, ptr, __FILE__, __LINE__, 0)

#ifndef NDEBUG
// Assert that an expression must be true, otherwise assume the program state will not be recoverable.
// In debug builds, this will trigger a log and exit, in release this is used as a hint for performance optimization.
// Do NOT use this if the program can recover when expr if false, as it is used as a hint for performance and can impact generated code.
// For recoverable errors, use D2_VERIFY
#define D2_ASSERT(expr) (void)( (!!(expr)) || (FOG_DisplayAssert(#expr, __FILE__, __LINE__), exit(-1) , 0))
#define D2_ASSERTM(expr,msg) (void)( (!!(expr)) || (FOG_DisplayAssert(msg, __FILE__, __LINE__), exit(-1) , 0))

// Assert that an expression must be true, even though the program may be recoverable.
// Contrary to D2_ASSERT, this is still evaluated in release builds, and can be used anywhere the expression would be valid.
// This is to be used when one can recover from an error
// Example:
// if(D2_VERIFY(ptr != nullptr)) ptr->method(); // The only difference between debug and release build is the logging and breakpoint
#define D2_VERIFY(expr) ((!!(expr)) || (FOG_DisplayAssert(#expr, __FILE__, __LINE__), false))
#define D2_VERIFYM(expr,msg) ((!!(expr)) || (FOG_DisplayAssert(msg, __FILE__, __LINE__), false))
#else
#define D2_ASSERT(expr) (__assume(expr), (void)0)
#define D2_ASSERTM(expr,msg) (__assume(expr), (void)0)
#define D2_VERIFY(expr) (!!(expr))
#define D2_VERIFYM(expr,msg) (!!(expr))
#endif
#define D2_UNREACHABLE D2_ASSERT(false)
