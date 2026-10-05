#include "File.h"

#if defined(_MSC_VER) && _MSC_VER < 1600
#include <Windows.h>

static CRITICAL_SECTION sgFileLockSection;
static BOOL sgFileLockSectionInited = FALSE;

static int __cdecl _lock_file(FILE* Stream)
{
	(void)Stream;
	if (!sgFileLockSectionInited)
	{
		InitializeCriticalSection(&sgFileLockSection);
		sgFileLockSectionInited = TRUE;
	}
	EnterCriticalSection(&sgFileLockSection);
	return 0;
}

static int __cdecl _unlock_file(FILE* Stream)
{
	(void)Stream;
	if (sgFileLockSectionInited)
	{
		LeaveCriticalSection(&sgFileLockSection);
	}
	return 0;
}
#endif

//D2Common.0x6FDC45EE
//D2Game.0x6FD1D70D
size_t __cdecl FileLockAndWrite(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
	_lock_file(Stream);
	size_t nbItemsWritten = fwrite(Buffer, ElementSize, ElementCount, Stream);
	_unlock_file(Stream);
	return nbItemsWritten;
}


//D2Game.0x6FD1D960
size_t __cdecl FileLockAndRead(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
	_lock_file(Stream);
	size_t nbItemsWritten = fread(Buffer, ElementSize, ElementCount, Stream);
	_unlock_file(Stream);
	return nbItemsWritten;
}
