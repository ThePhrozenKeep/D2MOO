#include "File.h"

#if defined(_MSC_VER) && _MSC_VER < 1600
#include <Windows.h>

// MSVC600's CRT declares _lock_file/_unlock_file, but its headers are not C++,
// so the D2MOO sources cannot call them: passing a volatile LONG* to their
// non-volatile LPLONG parameter is C2664. These are file-static, so they add no
// symbol, and they take a real critical section exactly as retail's do.
//
// Retail's own pair, at D2Common.0x6FDC599E and 0x6FDC59F0, picks a lock per
// stream: it range-checks the FILE* against the __iob table (__iob_func's
// backing array), and if it lands there uses (p - __iob) / sizeof(FILE) as a slot
// index into the lock table -- `sub ecx / sar eax,5 / add eax,0x1c / push / call
// _lock` -- otherwise it falls through to EnterCriticalSection(&p->_lock) at
// +0x20. One section for every stream is coarser than that, so it serializes
// more but cannot deadlock or lose mutual exclusion.
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
