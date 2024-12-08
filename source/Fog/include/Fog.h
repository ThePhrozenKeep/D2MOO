#pragma once

#include <D2Dll.h>
#include <D2BasicTypes.h>
#include <StormHandles.h>
#include <stdlib.h>

//1.10f Image base: 0x6FF50000

#ifdef D2_VERSION_110F
constexpr int FogImageBase = 0x6FF50000;
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
	const char* szFieldName;		//0x00
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


enum D2C_AsyncDataPriority
{
	ASYNC_DATA_PRIORITY_NORMAL = 0,
	ASYNC_DATA_PRIORITY_HIGH = 1,
};

enum D2C_AsyncDataFlags : uint32_t
{
	ASYNC_DATA_FLAGS_LOADED                    = 0x1,
	ASYNC_DATA_FLAGS_OWNS_BUFFER               = 0x2,
	ASYNC_DATA_FLAGS_POSTPONED_PRIORITY_CHANGE = 0x8,

	ASYNC_DATA_FLAGS_DEBUG_TAG      = 0x41737960,
	ASYNC_DATA_FLAGS_DEBUG_TAG_MASK = 0xFFFFFFF0,
};

struct AsyncDataLoadFileCallback																			   // sizeof(0x1C)
{
	DWORD(__stdcall* callback)(void* pFileData, size_t nDataLength, AsyncDataLoadFileCallback* pCallbackData); // 0x00
	DWORD userData[4];																						   // 0x04
	void* pData;																							   // 0x14
	size_t nFileSize;																						   // 0x18
};

struct AsyncData							 // sizeof(0x54)
{
	HSFILE hFile;							 // 0x00
	void* pMemPool;							 // 0x04
	OVERLAPPED tOverlapped;					 // 0x08
	AsyncDataLoadFileCallback pAsyncOpDesc;	 // 0x1C
	struct AsyncDataEventSlot* pEventSlot;	 // 0x38
	void* pBuffer;							 // 0x3C
	uint32_t nFileSize;						 // 0x40
	uint32_t nPriority;						 // 0x44 D2C_AsyncDataPriority
	uint32_t nFlags;						 // 0x48
	uint32_t nPreviousPriority;				 // 0x4C
	AsyncData* pNextPostponedPriorityChange; // 0x50
};

struct AsyncDataEventSlot  // sizeof(0x0C)
{
	HANDLE hEvent;		   // 0x00
	DWORD bSlotAvailable;  // 0x04
	AsyncData* pAsyncOp;   // 0x08
};


struct QServer;

#pragma pack()

typedef int (*D2ExceptionCallback)();

D2FUNC_DLL(FOG, csprintf, const char*, __cdecl, (char* szDest, const char* szFormat, ...), 0xDD90)													//Fog.#10018
D2FUNC_DLL(FOG, InitErrorMgr, void, __fastcall, (const char* szProgramName, D2ExceptionCallback pExceptionCallback, const char* szVersion, BOOL bLogToFile), 0xE1B0)	//Fog.#10019
D2FUNC_DLL(FOG, SetLogPrefix, int, __fastcall, (const char* szLogPrefix), 0xE1A0)																	//Fog.#10021
D2FUNC_DLL(FOG, GetSystemInfo, void*, __cdecl, (), 0xDF20)																							//Fog.#10022
D2FUNC_DLL(FOG, DisplayAssert, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED30)											//Fog.#10023
D2FUNC_DLL(FOG, DisplayHalt, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED60)												//Fog.#10024
D2FUNC_DLL(FOG, DisplayWarning, void, __cdecl, (const char* szMsg, const char* szFile, int nLine), 0xED90)											//Fog.#10025
D2FUNC_DLL(FOG, DisplayError, void, __cdecl, (int nCategory, const char* szMsg, const char* szFile, int nLine), 0xEDF0)								//Fog.#10026
// Log to default logfile with date and `\n` at end of message
D2FUNC_DLL(FOG, Trace, void, __cdecl, (const char* szFormat, ...), 0x120A0)																			//Fog.#10029
// Log to file. Will be prefixed with "D2" and suffixed by date. Log includes date prefix and `\n` at end of message.
D2FUNC_DLL(FOG, TraceF, void, __cdecl, (const char* pFileSubName, const char* szFormat, ...), 0x120E0)												//Fog.#10030
// Append to the default logfile. No date nor '\n'.
D2FUNC_DLL(FOG, TraceAppend, void, __cdecl, (const char* szFormat, ...), 0x12180)																	//Fog.#10031
D2FUNC_DLL(FOG, TraceMemory, void, __cdecl, (const char* pFileSubName, const char* szMessage, void* pMemory, size_t nByteCount, BOOL bLogOffset, BOOL bNoCopy, BOOL bNoASCII), 0x12210) //Fog.#10032
D2FUNC_DLL(FOG, CrashDeadlockDetected, void, __fastcall, (HANDLE hThreadToWalk), 0xFA10)															//Fog.#10033
D2FUNC_DLL(FOG, CrashDumpThread, void, __fastcall, (HANDLE hThreadToWalk), 0x1DE0)																	//Fog.#10034
D2FUNC_DLL(FOG, IsHandlingError, BOOL, __cdecl, (), 0xF2A0)																							//Fog.#10039
D2FUNC_DLL(FOG, Alloc, void*, __fastcall, (int nSize, const char* szFile, int nLine, int n0), 0x8F50)												//Fog.#10042
D2FUNC_DLL(FOG, Free, void, __fastcall, (void* pFree, const char* szFile, int nLine, int n0), 0x8F90)												//Fog.#10043
D2FUNC_DLL(FOG, Realloc, void, __fastcall, (void* pMemory, int nSize, const char* szFile, int nLine, int n0), 0x8F90)								//Fog.#10044
D2FUNC_DLL(FOG, AllocPool, void*, __fastcall, (void* pMemPool, int nSize, const char* szFile, int nLine, int n0), 0x8FF0)							//Fog.#10045
D2FUNC_DLL(FOG, FreePool, void, __fastcall, (void* pMemPool, void* pFree, const char* szFile, int nLine, int n0), 0x9030)							//Fog.#10046
D2FUNC_DLL(FOG, ReallocPool, void*, __fastcall, (void* pMemPool, void* pMemory, int nSize, const char* szFile, int nLine, int n0), 0x9060)			//Fog.#10047
D2FUNC_DLL(FOG, 10050_EnterCriticalSection, void, __fastcall, (_Acquires_lock_(*_Curr_) CRITICAL_SECTION* pCriticalSection, int nLine), 0xDC20)		//Fog.#10050
D2FUNC_DLL(FOG, 10055_GetSyncTime, int32_t, __fastcall, (), 0xA690)																					//Fog.#10055
// Noop, same as 10048, 10049, 10053, 10054, 10146, 10194, 10195, 10196, 10197, 10220, 10221, 10225, 10232, 10240, 10241, 10242
D2FUNC_DLL(FOG, 10082_Noop, void, __fastcall, (), 0x1DE0)																						//Fog.#10082
D2FUNC_DLL(FOG, 10083_Cos_LUT, float, __stdcall, (int16_t index), 0x1DF0)																			//Fog.#10083
D2FUNC_DLL(FOG, 10084_Sin_LUT, float, __stdcall, (int16_t index), 0x1E10)																			//Fog.#10084
D2FUNC_DLL(FOG, Decode14BitsFromString, unsigned, __stdcall, (uint16_t* p2Characters), 0x1B20)														//Fog.#10085
D2FUNC_DLL(FOG, Encode14BitsToString, void, __stdcall, (uint16_t* p2Characters, uint32_t value), 0x1B40)											//Fog.#10086
// Note: works up to value 0xFC05FC00
D2FUNC_DLL(FOG, Decode32BitsFromString, unsigned, __stdcall, (uint32_t* p4Characters), 0x1B60)														//Fog.#10087
D2FUNC_DLL(FOG, Encode32BitsToString, void , __stdcall, (uint32_t* p4Characters, uint32_t value), 0x1BA0)											//Fog.#10088
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
D2FUNC_DLL(FOG, FOpenFile, BOOL, __fastcall, (const char* szFile, HSFILE* pFileHandle), 0x11600)													//Fog.#10102
D2FUNC_DLL(FOG, FCloseFile, void, __fastcall, (HSFILE pFile), 0x11610)																				//Fog.#10103
D2FUNC_DLL(FOG, FReadFile, BOOL, __fastcall, (HSFILE pFile, void* pBuffer, size_t nSize, int* nBytesRead, uint32_t, uint32_t, uint32_t), 0x11620)	//Fog.#10104
D2FUNC_DLL(FOG, FGetFileSize, uint32_t, __fastcall, (HSFILE pFileHandle, uint32_t* lpFileSizeHigh), 0x11650)										//Fog.#10105
D2FUNC_DLL(FOG, FSetFilePointer, size_t, __fastcall, (HSFILE hFile, int32_t lDistanceToMove, int32_t* lpDistanceToMoveHigh, uint32_t dwMoveMethod), 0x11660)//Fog.#10106
D2FUNC_DLL(FOG, CreateFileA, HANDLE, __fastcall, (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile), 0x11680); //Fog.#10107
D2FUNC_DLL(FOG, DeleteFileA, BOOL, __fastcall, (LPCSTR lpFileName), 0x116B0);																		//Fog.#10108
D2FUNC_DLL(FOG, CloseFile, BOOL, __fastcall, (HANDLE hFILE), 0x116C0);																				//Fog.#10109
D2FUNC_DLL(FOG, CreatePathHierarchy, BOOL, __fastcall, (char* szPath), 0x11730)																		//Fog.#10114
D2FUNC_DLL(FOG, GetSavePath, size_t, __fastcall, (char* pPathBuffer, size_t nBufferSize), 0x11900)													//Fog.#10115
D2FUNC_DLL(FOG, GetInstallPath, BOOL, __fastcall, (char* pPathBuffer, size_t nBufferSize), 0x11870)													//Fog.#10116
D2FUNC_DLL(FOG, UseDirect, BOOL, __fastcall, (), 0x11A10)																							//Fog.#10117
D2FUNC_DLL(FOG, ComputeStringCRC16, uint16_t, __stdcall, (const char* szString), 0x3DB0)															//Fog.#10137
D2FUNC_DLL(FOG, CreateNewPoolSystem, void, __cdecl, (void** pMemPoolSystem, const char* szName, uint32_t nPools, uint32_t nUnused), 0xA280)			//Fog.#10142
D2FUNC_DLL(FOG, DestroyMemoryPoolSystem, void, __cdecl, (void* pMemoryPoolSystem), 0xA100)															//Fog.#10143
D2FUNC_DLL(FOG, FlushMemoryPoolSystem, void, __cdecl, (void* pMemoryPoolSystem), 0x9E90)															//Fog.#10144
D2FUNC_DLL(FOG, SetMemoryPoolSystemMode, void, __cdecl, (void* pMemoryPoolSystem, BOOL bFreePoolAlloc), 0xA4A0)										//Fog.#10145
D2FUNC_DLL(FOG, GetMemoryUsage, DWORD, __cdecl, (void*), 0xA4E0)																					//Fog.#10147
D2FUNC_DLL(FOG, InitializeServer, QServer*, __stdcall, (int, int, int, int, void*, void*, void*, void*), 0x4150)									//Fog.#10149
D2FUNC_DLL(FOG, SetMaxClientsPerGame, int, __stdcall, (QServer*, int), 0x4970)																		//Fog.#10151
D2FUNC_DLL(FOG, 10152, int, __stdcall, (void*, const uint8_t*, int), 0x44F0)																		//Fog.#10152
D2FUNC_DLL(FOG, WaitForSingleObject, int, __stdcall, (void*, int), 0x4A60)																			//Fog.#10154
D2FUNC_DLL(FOG, 10156, int, __stdcall, (void*, int, uint8_t*, int), 0x59D0)																			//Fog.#10156
D2FUNC_DLL(FOG, 10157, int, __stdcall, (void*, int, const uint8_t*, int), 0x5EA0)																	//Fog.#10157
D2FUNC_DLL(FOG, 10158, int, __stdcall, (void*, int), 0x6210)																						//Fog.#10158
D2FUNC_DLL(FOG, 10159, int, __stdcall, (void*, int, char*, int), 0x6180)																			//Fog.#10159
D2FUNC_DLL(FOG, 10161, int, __stdcall, (void*, int), 0x6310)																						//Fog.#10161
D2FUNC_DLL(FOG, 10162, int, __stdcall, (void*, int, const char*, int), 0x6380)																		//Fog.#10162
D2FUNC_DLL(FOG, 10163, int, __stdcall, (void*, int, const char*, int), 0x66E0)																		//Fog.#10163
D2FUNC_DLL(FOG, 10164, int, __stdcall, (void*, int, int, int), 0x69D0)																				//Fog.#10164
D2FUNC_DLL(FOG, 10165, int, __stdcall, (void*, int, const char*, int), 0x6A10)																		//Fog.#10165
D2FUNC_DLL(FOG, 10166, int, __stdcall, (void*, int, int, int), 0x6C90)																				//Fog.#10166
D2FUNC_DLL(FOG, 10170, int, __stdcall, (void*, int), 0x6F70)																						//Fog.#10170
D2FUNC_DLL(FOG, 10171, int, __stdcall, (void*, void*), 0x6FB0)																						//Fog.#10171
D2FUNC_DLL(FOG, 10172, int, __stdcall, (void*, int, int), 0x6FD0)																					//Fog.#10172
D2FUNC_DLL(FOG, 10173, int, __stdcall, (void*, int), 0x7040)																						//Fog.#10173
D2FUNC_DLL(FOG, 10175, int, __stdcall, (void*, const uint8_t*, int, int), 0x7420)																	//Fog.#10175
D2FUNC_DLL(FOG, 10177, int, __stdcall, (void*, int), 0x5E60)																						//Fog.#10177
D2FUNC_DLL(FOG, SetHackListEnabled, int, __stdcall, (QServer* pServer, BOOL bHacklistEnabled), 0x75A0)												//Fog.#10178
D2FUNC_DLL(FOG, 10180, int, __stdcall, (void*), 0x4920)																								//Fog.#10180
D2FUNC_DLL(FOG, 10181, int, __stdcall, (void*, const uint8_t*, int, int), 0x75C0)																	//Fog.#10181
D2FUNC_DLL(FOG, 10182_Return, int, __stdcall, (void*), 0x1B10)																						//Fog.#10182
D2FUNC_DLL(FOG, 10183_Return, int, __stdcall, (void*, int), 0x7620)																					//Fog.#10183
D2FUNC_DLL(FOG, 10186, int, __stdcall, (void*, int, int), 0x44C0)																					//Fog.#10186
D2FUNC_DLL(FOG, 10187, int, __fastcall, (void*, int, int), 0x7630)																					//Fog.#10187
D2FUNC_DLL(FOG, 10207, void, __stdcall, (D2BinFileStrc* pBinFile, D2BinFieldStrc* pBinField, void* pTxt, int nRecordCount, int nRecordSize), 0xAA60)//Fog.#10207
D2FUNC_DLL(FOG, CreateBinFile, D2BinFileStrc*, __stdcall, (void* pDataBuffer, int nBufferSize), 0xA8B0)												//Fog.#10208
D2FUNC_DLL(FOG, FreeBinFile, void, __stdcall, (D2BinFileStrc* pBinFile), 0xAA10)																	//Fog.#10209
D2FUNC_DLL(FOG, GetRecordCountFromBinFile, int, __stdcall, (D2BinFileStrc* pBinFile), 0xAA50)														//Fog.#10210
D2FUNC_DLL(FOG, AllocLinker, void*, __stdcall, (const char* szFile, int nLine), 0xB720)																//Fog.#10211
D2FUNC_DLL(FOG, FreeLinker, void, __stdcall, (void* pLinker), 0xB750)																				//Fog.#10212
D2FUNC_DLL(FOG, GetLinkIndex, int, __stdcall, (void* pLink, uint32_t dwCode, BOOL bLogError), 0xB810)												//Fog.#10213
D2FUNC_DLL(FOG, GetStringFromLinkIndex, int, __stdcall, (void* pLinker, int nIndex, char* szString), 0xB8F0)										//Fog.#10214
D2FUNC_DLL(FOG, 10215, int, __stdcall, (void* pBin, int a2), 0xB990)																				//Fog.#10215
D2FUNC_DLL(FOG, 10216_AddRecordToLinkingTable, int, __stdcall, (void* pBin, const char* a2), 0xBD80)														//Fog.#10216
D2FUNC_DLL(FOG, GetRowFromTxt, int, __stdcall, (void* pBin, char* szText, int nColumn), 0xBC20)														//Fog.#10217
D2FUNC_DLL(FOG, 10218, int, __fastcall, (), 0xCC40)																									//Fog.#10218
D2FUNC_DLL(FOG, 10219, int, __fastcall, (uint8_t*), 0xC9E0)																							//Fog.#10219
D2FUNC_DLL(FOG, 10222, int, __fastcall, (const uint8_t*, int), 0xCC90)																				//Fog.#10222
D2FUNC_DLL(FOG, 10223, int, __fastcall, (uint8_t*, int, const uint8_t*, int), 0xCD50)																//Fog.#10223
D2FUNC_DLL(FOG, 10224, int, __fastcall, (char*, int, uint8_t*, int), 0xCE00)																		//Fog.#10224
D2FUNC_DLL(FOG, IsExpansion, int, __fastcall, (), 0xD730)																							//Fog.#10227
D2FUNC_DLL(FOG, ComputeChecksum, uint32_t, __stdcall, (void* pData, size_t dwSize), 0x3940)															//Fog.#10229
D2FUNC_DLL(FOG, PopCount, uint32_t, __stdcall, (void* pData, size_t dwSize), 0x3970)																//Fog.#10230
D2FUNC_DLL(FOG, 10233, void, __fastcall, (const char*, int), 0xF300)																				//Fog.#10233
D2FUNC_DLL(FOG, LeadingZeroesCount, uint32_t, __stdcall, (uint32_t nValue), 0x39A0)																	//Fog.#10252
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
#define D2_DISPLAY_ASSERT_THEN_BREAK(msg) (FOG_DisplayAssert(msg, __FILE__, __LINE__), __debugbreak())
#define D2_ASSERT(expr) (void)( (!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(#expr), exit(-1) , 0)); _Analysis_assume_(expr)
#define D2_ASSERTM(expr,msg) (void)( (!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(msg), exit(-1) , 0)); _Analysis_assume_(expr)
#define D2_CHECK(expr) (void)( (!!(expr)) || (FOG_DisplayWarning(#expr, __FILE__, __LINE__), 0)); _Analysis_assume_(expr)


// Assert that an expression must be true, even though the program may be recoverable.
// Contrary to D2_ASSERT, this is still evaluated in release builds, and can be used anywhere the expression would be valid.
// This is to be used when one can recover from an error
// Example:
// if(D2_VERIFY(ptr != nullptr)) ptr->method(); // The only difference between debug and release build is the logging and breakpoint
#define D2_VERIFY(expr) ((!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(#expr), false))
#define D2_VERIFYM(expr,msg) ((!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(msg), false))
#else
  #if defined(__clang__)
    // the argument to '__builtin_assume' has side effects that will be discarded
    #pragma clang diagnostic ignored "-Wassume"
  #endif // defined(__clang__)
#define D2_ASSERT(expr) (__assume(expr), (void)0)
#define D2_ASSERTM(expr,msg) (__assume(expr), (void)0)
#define D2_VERIFY(expr) (!!(expr))
#define D2_VERIFYM(expr,msg) (!!(expr))
#define D2_CHECK(expr) (__assume(expr), (void)0)
#endif
#define D2_UNREACHABLE D2_ASSERT(false)


#define D2_LOCK(pCriticalSection) D2_ASSERT(pCriticalSection); FOG_10050_EnterCriticalSection(pCriticalSection, __LINE__);
#define D2_UNLOCK(pCriticalSection) D2_ASSERT(pCriticalSection); LeaveCriticalSection(pCriticalSection);
