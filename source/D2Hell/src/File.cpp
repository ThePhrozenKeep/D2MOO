#include "File.h"

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
