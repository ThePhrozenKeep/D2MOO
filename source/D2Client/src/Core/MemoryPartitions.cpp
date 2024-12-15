#include <Core/MemoryPartitions.h> 
#include <D2Client.h>

#ifdef D2_VERSION_110F
D2CLIENTSTUB(CLIENT_InitMemoryPartitions, 6FAA13A0, void, __fastcall, (void* pMemPoolSystem, int bInitTileCache));


//1.00 : D2Client.0x100063B0
//1.10f: D2Client.0x6FAA13A0
void __fastcall CLIENT_InitMemoryPartitions(void* pMemPoolSystem, int bInitTileCache)
{
	CLIENT_InitMemoryPartitions_6FAA13A0(pMemPoolSystem, bInitTileCache);
}

#endif