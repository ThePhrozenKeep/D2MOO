#include "Drlg/D2DrlgActivate.h"

#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgRoomTile.h"

//TODO: Find names

int dword_6FDEA6EC;
int dword_6FDEA6F0;
int dword_6FDEA6F4;
int dword_6FDEA6F8;

void (__fastcall* off_6FDE07B0[4])(D2RoomExStrc*) =
{
	sub_6FD733D0,
	sub_6FD73450,
	sub_6FD73550,
	sub_6FD736F0
};

void (__fastcall* off_6FDE07C0[4])(D2RoomExStrc*) =
{
	sub_6FD73790,
	sub_6FD73790,
	sub_6FD73790,
	sub_6FD73880
};


//D2Common.0x6FD733D0
void __fastcall sub_6FD733D0(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pRooms = NULL;

	if (pRoomEx->fRoomStatus)
	{
		if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
		{
			pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
			pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
			pRoomEx->pStatusPrev = NULL;
			pRoomEx->pStatusNext = NULL;
		}

		pRooms = pRoomEx->pLevel->pDrlg->pRooms;

		pRoomEx->pStatusNext = pRooms;
		pRoomEx->pStatusPrev = pRooms->pStatusPrev;

		pRooms->pStatusPrev->pStatusNext = pRoomEx;
		pRooms->pStatusPrev = pRoomEx;

		pRoomEx->fRoomStatus = 0;
	}
}

//D2Common.0x6FD73450
void __fastcall sub_6FD73450(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pRooms = NULL;
	D2DrlgStrc* pDrlg = NULL;

	if (!pRoomEx->pRoom && !(pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM))
	{
		pDrlg = pRoomEx->pLevel->pDrlg;
		if (!pRoomEx->nRoomsNear)
		{
			sub_6FD77BB0(pDrlg->pMempool, pRoomEx);
		}

		DRLGROOMTILE_InitRoomGrids(pRoomEx);
		DRLGROOMTILE_AddRoomMapTiles(pRoomEx);
		DRLG_CreateRoomForRoomEx(pDrlg, pRoomEx);
		++pDrlg->unk0x3DC;
		++pDrlg->unk0x3E0[0];
		pDrlg->unk0x3DD[0] = 2 * (DRLG_IsOnClient(pDrlg) == 0) + 5;
	}

	if (pRoomEx->fRoomStatus > 1)
	{
		if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
		{
			pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
			pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
			pRoomEx->pStatusPrev = NULL;
			pRoomEx->pStatusNext = NULL;
		}

		pRooms = &pRoomEx->pLevel->pDrlg->pRooms[1];

		pRoomEx->pStatusNext = pRooms;
		pRoomEx->pStatusPrev = pRooms->pStatusPrev;

		pRooms->pStatusPrev->pStatusNext = pRoomEx;
		pRooms->pStatusPrev = pRoomEx;

		pRoomEx->fRoomStatus = 1;
	}
}

//D2Common.0x6FD73550
void __fastcall sub_6FD73550(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pRooms = NULL;
	D2DrlgStrc* pDrlg = NULL;

	if (pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED && (pRoomEx->nType != DRLGTYPE_PRESET || pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED) && pRoomEx->fRoomStatus > 2)
	{
		if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
		{
			pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
			pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
			pRoomEx->pStatusPrev = NULL;
			pRoomEx->pStatusNext = NULL;
		}

		pRooms = &pRoomEx->pLevel->pDrlg->pRooms[2];

		pRoomEx->pStatusNext = pRooms;
		pRoomEx->pStatusPrev = pRooms->pStatusPrev;

		pRooms->pStatusPrev->pStatusNext = pRoomEx;
		pRooms->pStatusPrev = pRoomEx;

		pRoomEx->fRoomStatus = 2;

		if (pRoomEx->unk0xAE[1] != 0)
		{
			if (!pRoomEx->pRoom && !(pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM))
			{
				pDrlg = pRoomEx->pLevel->pDrlg;
				if (!pRoomEx->nRoomsNear)
				{
					sub_6FD77BB0(pDrlg->pMempool, pRoomEx);
				}

				DRLGROOMTILE_InitRoomGrids(pRoomEx);
				DRLGROOMTILE_AddRoomMapTiles(pRoomEx);
				DRLG_CreateRoomForRoomEx(pDrlg, pRoomEx);
				++pDrlg->unk0x3DC;
				++pDrlg->unk0x3E0[0];
				pDrlg->unk0x3DD[0] = 2 * (DRLG_IsOnClient(pDrlg) == 0) + 5;
			}

			if (pRoomEx->fRoomStatus > 1)
			{
				if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
				{
					pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
					pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
					pRoomEx->pStatusPrev = NULL;
					pRoomEx->pStatusNext = NULL;
				}

				pRooms = &pRoomEx->pLevel->pDrlg->pRooms[1];

				pRoomEx->pStatusNext = pRooms;
				pRoomEx->pStatusPrev = pRooms->pStatusPrev;

				pRooms->pStatusPrev->pStatusNext = pRoomEx;
				pRooms->pStatusPrev = pRoomEx;

				pRoomEx->fRoomStatus = 1;
			}
		}
	}
}

//D2Common.0x6FD736F0
void __fastcall sub_6FD736F0(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pRooms = NULL;

	if (!(pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED))
	{
		DRLGROOMTILE_LoadDT1FilesForRoom(pRoomEx);
	}

	if (pRoomEx->nType == DRLGTYPE_PRESET && !(pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED))
	{
		DRLGPRESET_SpawnHardcodedPresetUnits(pRoomEx);
	}

	if (pRoomEx->fRoomStatus > 3)
	{
		if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
		{
			pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
			pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
			pRoomEx->pStatusPrev = NULL;
			pRoomEx->pStatusNext = NULL;
		}

		pRooms = &pRoomEx->pLevel->pDrlg->pRooms[3];

		pRoomEx->pStatusNext = pRooms;
		pRoomEx->pStatusPrev = pRooms->pStatusPrev;

		pRooms->pStatusPrev->pStatusNext = pRoomEx;
		pRooms->pStatusPrev = pRoomEx;

		pRoomEx->fRoomStatus = 3;
	}
}

//D2Common.0x6FD73790
void __fastcall sub_6FD73790(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pRooms = NULL;
	uint8_t nCounter = 0;

	if (pRoomEx->fRoomStatus >= 4 || !pRoomEx->unk0xAE[pRoomEx->fRoomStatus])
	{
		nCounter = 0;
		while (!pRoomEx->unk0xAE[nCounter])
		{
			++nCounter;

			if (nCounter >= 4)
			{
				break;
			}
		}

		if (pRoomEx->fRoomStatus != nCounter)
		{
			if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
			{
				pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
				pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
				pRoomEx->pStatusPrev = NULL;
				pRoomEx->pStatusNext = NULL;
			}

			if (nCounter < 4)
			{
				pRooms = &pRoomEx->pLevel->pDrlg->pRooms[nCounter];

				pRoomEx->pStatusNext = pRooms;
				pRoomEx->pStatusPrev = pRooms->pStatusPrev;

				pRooms->pStatusPrev->pStatusNext = pRoomEx;
				pRooms->pStatusPrev = pRoomEx;

				pRoomEx->fRoomStatus = nCounter;
			}
		}

		pRoomEx->fRoomStatus = nCounter;
	}
}

//D2Common.0x6FD73880
void __fastcall sub_6FD73880(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pRooms = NULL;
	uint8_t nCounter = 0;

	if (pRoomEx->fRoomStatus != 4)
	{
		if (pRoomEx->fRoomStatus >= 4 || !pRoomEx->unk0xAE[pRoomEx->fRoomStatus])
		{
			nCounter = 0;
			while (!pRoomEx->unk0xAE[nCounter])
			{
				++nCounter;

				if (nCounter >= 4)
				{
					break;
				}
			}

			if (pRoomEx->fRoomStatus != nCounter)
			{
				if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
				{
					pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
					pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
					pRoomEx->pStatusPrev = NULL;
					pRoomEx->pStatusNext = NULL;
				}

				if (nCounter < 4)
				{
					pRooms = &pRoomEx->pLevel->pDrlg->pRooms[nCounter];
					pRoomEx->pStatusNext = pRooms;
					pRoomEx->pStatusPrev = pRooms->pStatusPrev;
					pRooms->pStatusPrev->pStatusNext = pRoomEx;
					pRooms->pStatusPrev = pRoomEx;
					pRoomEx->fRoomStatus = nCounter;
				}
			}

			pRoomEx->fRoomStatus = nCounter;
		}

		if (pRoomEx->fRoomStatus == 4)
		{
			if (DRLG_IsOnClient(pRoomEx->pLevel->pDrlg))
			{
				sub_6FD8A2E0(pRoomEx, 0);
			}
		}
	}
}

//D2Common.0x6FD739A0
//TODO: Rename v9
void __fastcall sub_6FD739A0(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomEx)
{
	D2DrlgLevelStrc* pLevel = NULL;
	D2RoomExStrc* v9 = NULL;
	int nCounter = 0;

	pLevel = DRLG_GetLevel(pDrlg, nLevelId);

	v9 = DRLG_GetRoomExFromCoordinates(nX, nY, pDrlg, (pRoomEx && pRoomEx->pLevel->nLevelId == nLevelId) ? pRoomEx : NULL, pLevel);

	if (!v9->unk0xAE[1])
	{
		sub_6FD73A30(v9->pLevel->pDrlg->pMempool, v9, 2);
		if (v9->fRoomStatus >= 1)
		{
			nCounter = 0;
			while (!v9->unk0xAE[nCounter])
			{
				++nCounter;
				if (nCounter > 1)
				{
					off_6FDE07B0[1](v9);
					break;
				}
			}
		}

		++v9->unk0xAE[1];
	}
}

//D2Common.0x6FD73A30
void __fastcall sub_6FD73A30(void* pMemPool, D2RoomExStrc* pRoomEx, uint8_t a3)
{
	int nCounter = 0;

	if (!pRoomEx->nRoomsNear)
	{
		sub_6FD77BB0(pMemPool, pRoomEx);
	}

	if (a3 >= 3)
	{
		for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
		{
			if (pRoomEx->ppRoomsNear[i]->fRoomStatus >= a3)
			{
				nCounter = 0;
				while (!pRoomEx->ppRoomsNear[i]->unk0xAE[nCounter])
				{
					++nCounter;
					if (nCounter > a3)
					{
						off_6FDE07B0[a3](pRoomEx->ppRoomsNear[i]);
						break;
					}
				}
			}

			++pRoomEx->ppRoomsNear[i]->unk0xAE[a3];
		}
	}
	else
	{
		for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
		{
			sub_6FD73A30(pMemPool, pRoomEx->ppRoomsNear[i], a3 + 1);

			if (pRoomEx->ppRoomsNear[i]->fRoomStatus >= a3)
			{
				if (a3 < 0)
				{
					off_6FDE07B0[a3](pRoomEx->ppRoomsNear[i]);
				}
				else
				{
					nCounter = 0;
					while (!pRoomEx->ppRoomsNear[i]->unk0xAE[nCounter])
					{
						++nCounter;
						if (nCounter > a3)
						{
							off_6FDE07B0[a3](pRoomEx->ppRoomsNear[i]);
							break;
						}
					}
				}
			}

			++pRoomEx->ppRoomsNear[i]->unk0xAE[a3];
		}
	}
}

//D2Common.0x6FD73B40
//TODO: Rename v9
void __fastcall sub_6FD73B40(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomEx)
{
	D2DrlgLevelStrc* pLevel = NULL;
	D2RoomExStrc* v9 = NULL;

	pLevel = DRLG_GetLevel(pDrlg, nLevelId);

	v9 = DRLG_GetRoomExFromCoordinates(nX, nY, pDrlg, (pRoomEx && pRoomEx->pLevel->nLevelId == nLevelId) ? pRoomEx : NULL, pLevel);
	if (v9 && v9->unk0xAE[1])
	{
		--v9->unk0xAE[1];

		off_6FDE07C0[1](v9);

		for (int i = 0; i < v9->nRoomsNear; ++i)
		{
			if (v9->ppRoomsNear[i])
			{
				--v9->ppRoomsNear[i]->unk0xAE[2];
				off_6FDE07C0[2](v9->ppRoomsNear[i]);
				sub_6FD73BE0(v9->ppRoomsNear[i], 3);
			}
		}
	}
}

//D2Common.0x6FD73BE0
void __fastcall sub_6FD73BE0(D2RoomExStrc* pRoomEx, uint8_t a2)
{
	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		if (pRoomEx->ppRoomsNear[i])
		{
			--pRoomEx->ppRoomsNear[i]->unk0xAE[a2];

			off_6FDE07C0[a2](pRoomEx->ppRoomsNear[i]);

			if (a2 < 3)
			{
				sub_6FD73BE0(pRoomEx->ppRoomsNear[i], a2 + 1);
			}
		}
	}
}

//D2Common.0x6FD73C40
void __fastcall sub_6FD73C40(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2)
{
	int nCounter = 0;

	if (pRoomEx1 != pRoomEx2)
	{
		if (pRoomEx2)
		{
			sub_6FD73A30(pRoomEx2->pLevel->pDrlg->pMempool, pRoomEx2, 1);

			nCounter = 0;
			while (!pRoomEx2->unk0xAE[nCounter])
			{
				++nCounter;

				if (nCounter > 0)
				{
					off_6FDE07B0[0](pRoomEx2);
					break;
				}
			}

			++pRoomEx2->unk0xAE[0];
		}

		if (pRoomEx1)
		{
			if (pRoomEx1->unk0xAE[0])
			{
				--pRoomEx1->unk0xAE[0];

				off_6FDE07C0[0](pRoomEx1);

				for (int i = 0; i < pRoomEx1->nRoomsNear; ++i)
				{
					if (pRoomEx1->ppRoomsNear[i])
					{
						--pRoomEx1->ppRoomsNear[i]->unk0xAE[1];
						off_6FDE07C0[1](pRoomEx1->ppRoomsNear[i]);
						sub_6FD73BE0(pRoomEx1->ppRoomsNear[i], 2);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD73CF0
void __fastcall DRLGACTIVATE_InitializeRoomEx(D2RoomExStrc* pRoomEx)
{
	if (!(pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED))
	{
		DRLGROOMTILE_LoadDT1FilesForRoom(pRoomEx);
	}

	if (!(pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED) && pRoomEx->nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_SpawnHardcodedPresetUnits(pRoomEx);
	}

	if (!(pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM))
	{
		if (!pRoomEx->nRoomsNear)
		{
			sub_6FD77BB0(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx);
		}

		DRLGROOMTILE_InitRoomGrids(pRoomEx);
		DRLGROOMTILE_AddRoomMapTiles(pRoomEx);
		DRLG_CreateRoomForRoomEx(pRoomEx->pLevel->pDrlg, pRoomEx);

		++pRoomEx->pLevel->pDrlg->unk0x3DC;
		++pRoomEx->pLevel->pDrlg->unk0x3E0[0];
	}
}

//D2Common.0x6FD73D80
D2RoomStrc* __fastcall sub_6FD73D80(D2DrlgStrc* pDrlg, int nX, int nY)
{
	D2RoomExStrc* pRoomEx = DRLG_GetRoomExFromCoordinates(nX, nY, pDrlg, 0, 0);

	if (pRoomEx)
	{
		if (!(pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED))
		{
			DRLGROOMTILE_LoadDT1FilesForRoom(pRoomEx);
		}

		if (!(pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED) && pRoomEx->nType == DRLGTYPE_PRESET)
		{
			DRLGPRESET_SpawnHardcodedPresetUnits(pRoomEx);
		}

		if (!(pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM))
		{
			if (!pRoomEx->nRoomsNear)
			{
				sub_6FD77BB0(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx);
			}

			DRLGROOMTILE_InitRoomGrids(pRoomEx);
			DRLGROOMTILE_AddRoomMapTiles(pRoomEx);
			DRLG_CreateRoomForRoomEx(pRoomEx->pLevel->pDrlg, pRoomEx);

			++pRoomEx->pLevel->pDrlg->unk0x3DC;
			++pRoomEx->pLevel->pDrlg->unk0x3E0[0];
		}

		return pRoomEx->pRoom;
	}

	return NULL;
}

//D2Common.0x6FD73E30
void __fastcall sub_6FD73E30(D2DrlgStrc* pDrlg)
{
	for (int i = 0; i < 4; ++i)
	{
		pDrlg->pRooms[i].fRoomStatus = i;

		pDrlg->pRooms[i].pStatusNext = &pDrlg->pRooms[i];
		pDrlg->pRooms[i].pStatusPrev = &pDrlg->pRooms[i];
	}
}

//D2Common.0x6FD73E60
D2RoomStrc* __fastcall sub_6FD73E60(D2DrlgStrc* pDrlg)
{
	D2RoomExStrc* pRoomEx = pDrlg->pRooms[0].pStatusNext;

	if (pRoomEx != pDrlg->pRooms && pRoomEx)
	{
		return pRoomEx->pRoom;
	}

	pRoomEx = pDrlg->pRooms[1].pStatusNext;
	
	if (pRoomEx != &pDrlg->pRooms[1] && pRoomEx)
	{
		return pRoomEx->pRoom;
	}

	return NULL;
}

//D2Common.0x6FD73E90
//TODO: Rename v3
D2RoomStrc* __fastcall sub_6FD73E90(D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* v3 = pRoomEx->pStatusNext;

	if ((pRoomEx->fRoomStatus || v3 != pDrlg->pRooms) && v3)
	{
		return v3->pRoom;
	}
	
	v3 = pDrlg->pRooms[1].pStatusNext;
	
	if (v3 != &pDrlg->pRooms[1] && v3)
	{
		return v3->pRoom;
	}

	return NULL;
}

//D2Common.0x6FD73EF0
void __fastcall D2Common_10015(int* a1, int* a2, int* a3, int* a4)
{
	*a1 = dword_6FDEA6F0;
	*a2 = dword_6FDEA6EC;
	*a3 = dword_6FDEA6F8;
	*a4 = dword_6FDEA6F4;
}

//D2Common.0x6FD73F20
void __stdcall D2Common_10003(D2DrlgStrc* pDrlg)
{
	D2RoomExStrc* pRoomEx = NULL;
	D2RoomExStrc* i = NULL;

	if (DRLG_IsOnClient(pDrlg))
	{
		dword_6FDEA6F0 = pDrlg->unk0x3E0[0];
		dword_6FDEA6EC = pDrlg->unk0x3E0[1];
	}
	else
	{
		dword_6FDEA6F8 = pDrlg->unk0x3E0[0];
		dword_6FDEA6F4 = pDrlg->unk0x3E0[1];
	}

	if (pDrlg->unk0x3DC > 1)
	{
		pDrlg->unk0x3DC = 0;
		return;
	}

	--pDrlg->unk0x3DD[0];

	if (!pDrlg->unk0x3DD[0])
	{
		pDrlg->unk0x3DD[0] = 2 * (DRLG_IsOnClient(pDrlg) == 0) + 5;

		if (!pDrlg->pRoomEx || pDrlg->pRoomEx->fRoomStatus != 2)
		{
			pDrlg->pRoomEx = pDrlg->pRooms[2].pStatusNext;
		}

		pRoomEx = pDrlg->pRoomEx;
		if (pRoomEx)
		{
			i = pDrlg->pRoomEx;
			do
			{
				if (!(i->dwFlags & 0x100000) && i != &pDrlg->pRooms[2] && !i->pRoom)
				{
					if (!i->nRoomsNear)
					{
						sub_6FD77BB0(pDrlg->pMempool, i);
					}

					DRLGROOMTILE_InitRoomGrids(i);
					DRLGROOMTILE_AddRoomMapTiles(i);
					DRLG_CreateRoomForRoomEx(i->pLevel->pDrlg, i);
					++i->pLevel->pDrlg->unk0x3DC;
					++i->pLevel->pDrlg->unk0x3E0[0];
				}

				i = i->pStatusNext;
			}
			while (i != pRoomEx && pDrlg->unk0x3DC < 1);

			pDrlg->pRoomEx = i;
			pDrlg->unk0x3DC = 0;
		}
	}
}

//D2Common.0x6FD74060
BOOL __fastcall DRLGACTIVATE_TestRoomCanUnTile(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->dwFlags & ROOMEXFLAG_HASPORTAL)
	{
		return FALSE;
	}

	D2_ASSERT(!DRLG_IsOnClient(DRLGROOM_GetDrlgFromRoomEx(pRoomEx)));

	if (pRoomEx->fRoomStatus <= 1)
	{
		return FALSE;
	}

	if (DRLG_IsTownLevel(pRoomEx->pLevel->nLevelId) || pRoomEx->pLevel->nLevelId == LEVEL_ROCKYSUMMIT)
	{
		for (D2RoomExStrc* pCurrentRoomEx = pRoomEx->pLevel->pFirstRoomEx; pCurrentRoomEx; pCurrentRoomEx = pCurrentRoomEx->pRoomExNext)
		{
			if (pCurrentRoomEx->fRoomStatus <= 1)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD740F0
void __fastcall DRLGACTIVATE_ToggleHasPortalFlag(D2RoomExStrc* pRoomEx, BOOL bReset)
{
	if (bReset)
	{
		pRoomEx->dwFlags &= ~ROOMEXFLAG_HASPORTAL;
	}
	else
	{
		pRoomEx->dwFlags |= ROOMEXFLAG_HASPORTAL;
	}
}

//D2Common.0x6FD74110
uint8_t __fastcall DRLGACTIVATE_GetRoomStatusFlags(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->fRoomStatus;
}
