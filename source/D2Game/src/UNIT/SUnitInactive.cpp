#include "UNIT/SUnitInactive.h"

#include <Fog.h>
#include <D2BitManip.h>

#include <D2Items.h>
#include <D2Inventory.h>
#include <Drlg/D2DrlgPreset.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2DataTbls.h>
#include <D2Monsters.h>
#include <D2StatList.h>
#include <D2States.h>
#include <Units/UnitRoom.h>
#include <D2Collision.h>


#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Targets.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "PLAYER/PlayerPets.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT4/A4Q2.h"
#include "UNIT/SUnit.h"


constexpr uint32_t dword_6FD3F45C = 0xDEADDEAD;
constexpr uint32_t dword_6FD3F460 = 1;
constexpr uint32_t dword_6FD3F464 = 2;




//D2Game.0x6FCC3850
void __fastcall SUNITINACTIVE_RestoreInactiveUnits(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom)
{
	D2DrlgCoordsStrc pDrlgCoords = {};
	DUNGEON_GetRoomCoordinates(pRoom, &pDrlgCoords);
	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	const uint8_t nAct = DRLG_GetActNoFromLevelId(nLevelId);

	D2InactiveUnitListStrc* pInactiveUnitList = SUNITINACTIVE_GetListNodeFromActAndCoordinates(pGame, nAct, pDrlgCoords.nSubtileX, pDrlgCoords.nSubtileY, 0);
	if (pInactiveUnitList)
	{
		D2InactiveMonsterNodeStrc* pInactiveMonsterNode = pInactiveUnitList->pInactiveMonsters;

		int32_t bIsChaosSanctum = 0;
		if (nLevelId == LEVEL_CHAOSSANCTUM && ACT4Q2_IsChaosSanctumCleared(pGame))
		{
			bIsChaosSanctum = 1;
		}

		while (pInactiveMonsterNode)
		{
			D2InactiveMonsterNodeStrc* pNextMonsterNode = pInactiveMonsterNode->pNext;

			int32_t nMode = MONMODE_NEUTRAL;
			const int32_t nClassId = pInactiveMonsterNode->nClassId;

			const int32_t nX = pInactiveMonsterNode->nX;
			const int32_t nY = pInactiveMonsterNode->nY;

			int32_t bContinue = 0;
			if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_DEAD)
			{
				nMode = MONMODE_DEAD;
			}
			else
			{
				if (bIsChaosSanctum && nClassId != MONSTER_DIABLO)
				{
					SUNITINACTIVE_FreeInactiveMonsterNode(pGame, pInactiveMonsterNode);
					bContinue = 1;
				}
			}

			if (!bContinue)
			{
				D2UnitStrc* pUnit = nullptr;
				if (!(pInactiveMonsterNode->nUnitFlagsEx & UNITFLAGEX_HASBEENDELETED))
				{
					int32_t nFlags = 0;
					if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_UNKSUMMONERFLAG)
					{
						nFlags = 0x40 | 0x20 | 0x8 | 0x2;
					}
					else
					{
						nFlags = 0x40 | 0x20 | 0x2;
					}

					if (sgptDataTables->pMonStatsTxt[nClassId].dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_FLYING])
					{
						nFlags |= 0x80;
					}

					int32_t nType = 0;
					if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_TYPE_OTHER)
					{
						nType = 1;
					}

					if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_TYPE_MINION)
					{
						nType = 2;
					}

					if (nType == 0)
					{
						pUnit = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, nMode, pInactiveMonsterNode->nUnitId, -1, nFlags);

						if (!pUnit)
						{
							pUnit = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, nMode, pInactiveMonsterNode->nUnitId, 5, nFlags);

							if (!pUnit)
							{
								D2CoordStrc pCoord = {};
								if (sub_6FC66260(pGame, pRoom, 0, nClassId, &pCoord.nX, &pCoord.nY, 0))
								{
									pUnit = sub_6FC6A0F0(pGame, pRoom, pCoord.nX, pCoord.nY, nClassId, nMode, pInactiveMonsterNode->nUnitId, -1, nFlags);
									if (!pUnit)
									{
										pCoord.nX = nX;
										pCoord.nY = nY;
										D2ActiveRoomStrc* ppRoom = nullptr;
										D2Common_10136(pRoom, &pCoord, 1, 0x3C01, &ppRoom);
										if (ppRoom)
										{
											pUnit = sub_6FC6A0F0(pGame, ppRoom, pCoord.nX, pCoord.nY, nClassId, nMode, pInactiveMonsterNode->nUnitId, -1, nFlags);
										}
									}
								}
								else
								{
									pCoord.nX = nX;
									pCoord.nY = nY;
									D2ActiveRoomStrc* ppRoom = nullptr;
									D2Common_10136(pRoom, &pCoord, 1, 0x3C01, &ppRoom);
									if (ppRoom)
									{
										pUnit = sub_6FC6A0F0(pGame, ppRoom, pCoord.nX, pCoord.nY, nClassId, nMode, pInactiveMonsterNode->nUnitId, -1, nFlags);
									}
								}
							}
						}
					}
					else if (nType == 1)
					{
						pUnit = sub_6FC6FBA0(pGame, pRoom, nX, nY, nClassId, pInactiveMonsterNode->nUnitId, pInactiveMonsterNode->nNameSeed, pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_TYPE_CHAMPION, pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_TYPE_SUPERUNIQUE, pInactiveMonsterNode->nBossHcIdx, pInactiveMonsterNode->nMonUMods);
					}
					else if (nType == 2)
					{
						pUnit = sub_6FC6FDC0(pGame, pRoom, nX, nY, nClassId, pInactiveMonsterNode->nUnitId, pInactiveMonsterNode->nMonUMods);
					}

					int32_t nOldAlign = UNIT_ALIGNMENT_EVIL;
					if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_ALIGNMENT_GOOD)
					{
						nOldAlign = UNIT_ALIGNMENT_GOOD;
					}

					if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_ALIGNMENT_NEUTRAL)
					{
						nOldAlign = UNIT_ALIGNMENT_NEUTRAL;
					}

					if (pUnit)
					{
						AIGENERAL_SetAiControlInfo(pGame, pUnit, pInactiveMonsterNode->unk0x1C, 1, pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_MINIONRELATED1, pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_MINIONRELATED2, pInactiveMonsterNode->pMinionList);

						STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, pInactiveMonsterNode->nExperience, 0);

						if ((uint32_t)(pGame->dwGameFrame - pInactiveMonsterNode->nGameFrame) < 7500)
						{
							STATLIST_SetUnitStat(pUnit, STAT_MAXHP, pInactiveMonsterNode->nMaxHitpoints, 0);
							STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, pInactiveMonsterNode->nHitpoints, 0);
						}
						else
						{
							STATLIST_SetUnitStat(pUnit, STAT_MAXHP, pInactiveMonsterNode->nMaxHitpoints, 0);
							STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, pInactiveMonsterNode->nMaxHitpoints, 0);
						}

						AITHINK_ExecuteAiFn(pGame, pUnit, AIGENERAL_GetAiControlFromUnit(pUnit), pInactiveMonsterNode->nAiSpecialState);

						SUNITINACTIVE_RestoreSpecialMonsterParameters(pGame, pUnit, pInactiveMonsterNode);

						if (pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_NODEINDEXUNEQUAL11)
						{
							if (nOldAlign == 2)
							{
								sub_6FC40280(pGame, pUnit, 0, 8);
							}
							else
							{
								D2_ASSERT(nOldAlign == UNIT_ALIGNMENT_NEUTRAL);

								sub_6FC40280(pGame, pUnit, 0, 9);
							}
						}

						const uint8_t* pUMods = MONSTERUNIQUE_GetUMods(pUnit);
						if (pUMods)
						{
							for (int32_t i = 0; i < 9; ++i)
							{
								if (pUMods[i] != pInactiveMonsterNode->nMonUMods[i])
								{
									D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, pInactiveMonsterNode->nMonUMods[i], 0);
								}
							}
						}

						sub_6FCBDD30(pUnit, nOldAlign, 1);

						if (pInactiveMonsterNode->nUnitFlagsEx & 0x200)
						{
							SUNITINACTIVE_SetUnitFlagEx(pUnit, 0x200, 1);
						}

						pUnit->dwFlags |= pInactiveMonsterNode->nUnitFlags & UNITFLAG_NOTC | UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
					}
					else
					{
						AIGENERAL_FreeAllMinionLists(pGame, pInactiveMonsterNode->pMinionList);
					}

					sub_6FC682C0(pGame->pMonReg, pInactiveMonsterNode->nLevelId, DUNGEON_GetLevelIdFromRoom(pRoom), nOldAlign, nMode == MONMODE_DEAD, pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_UNKSUMMONERFLAG, pUnit);
					SUNITINACTIVE_FreeInactiveMonsterNode(pGame, pInactiveMonsterNode);
				}
				else
				{
					pUnit = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pInactiveMonsterNode->nUnitId);
					if (pUnit)
					{
						SUNIT_Restore(pGame, pUnit, pRoom, nX, nY);
					}

					AIGENERAL_FreeAllMinionLists(pGame, pInactiveMonsterNode->pMinionList);
					SUNITINACTIVE_FreeInactiveMonsterNode(pGame, pInactiveMonsterNode);
				}
			}

			pInactiveMonsterNode = pNextMonsterNode;
		}
	}

	DUNGEON_GetRoomCoordinates(pRoom, &pDrlgCoords);

	D2InactiveUnitListStrc* pPreviousList = nullptr;
	D2InactiveItemNodeStrc* pInactiveItemNode = nullptr;
	D2InactiveUnitNodeStrc* pInactiveUnitNode = nullptr;
	pInactiveUnitList = pGame->pInactiveUnitList[nAct];
	while (pInactiveUnitList)
	{
		if (pDrlgCoords.nSubtileX > pInactiveUnitList->nX)
		{
			break;
		}

		if (pDrlgCoords.nSubtileX == pInactiveUnitList->nX && pDrlgCoords.nSubtileY == pInactiveUnitList->nY)
		{
			if (pPreviousList)
			{
				pPreviousList->pNext = pInactiveUnitList->pNext;
			}
			else
			{
				pGame->pInactiveUnitList[nAct] = pInactiveUnitList->pNext;
			}

			pInactiveItemNode = pInactiveUnitList->pInactiveItems;
			pInactiveUnitNode = pInactiveUnitList->pInactiveUnits;

			D2_FREE_POOL(pGame->pMemoryPool, pInactiveUnitList);
			break;
		}

		pPreviousList = pInactiveUnitList;
		pInactiveUnitList = pInactiveUnitList->pNext;
	}

	while (pInactiveItemNode)
	{
		D2InactiveItemNodeStrc* pNextItemNode = pInactiveItemNode->pNext;
		if (!pInactiveItemNode->nFrame || (int32_t)pGame->dwGameFrame <= pInactiveItemNode->nFrame)
		{
			SUNITINACTIVE_RestoreInactiveItem(pGame, pRoom, pInactiveItemNode);
		}

		D2_FREE_POOL(pGame->pMemoryPool, pInactiveItemNode);
		pInactiveItemNode = pNextItemNode;
	}

	while (pInactiveUnitNode)
	{
		D2UnitStrc* pUnit = nullptr;
		D2InactiveUnitNodeStrc* pNextUnitNode = pInactiveUnitNode->pNext;

		if (!(pInactiveUnitNode->nUnitFlagsEx & UNITFLAGEX_HASBEENDELETED))
		{
			if (pInactiveUnitNode->nUnitType != UNIT_MONSTER)
			{
				pUnit = SUNIT_CreatePresetUnit(pGame, pRoom, pInactiveUnitNode->nUnitType, pInactiveUnitNode->nClassId, pInactiveUnitNode->nX, pInactiveUnitNode->nY, pInactiveUnitNode->nAnimMode, pInactiveUnitNode->nUnitFlags);
			}

			if (pInactiveUnitNode->nUnitType == UNIT_OBJECT)
			{
				D2_ASSERT(pUnit);

				D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);

				if (UNITS_IsShrine(pUnit) && pInactiveUnitNode->nGameFrame)
				{
					int32_t nFrame = ((WORD)pInactiveUnitNode->nFrame) | ((WORD)pInactiveUnitNode->nUnitId << 16);
					if (nFrame > 0)
					{
						if (nFrame <= pGame->dwGameFrame)
						{
							nFrame = pGame->dwGameFrame + 1;
						}
						EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ACTIVESTATE, nFrame, 0, 0);
					}

					pUnit->pObjectData->InteractType = pInactiveUnitNode->nInteractType;
					UNITS_SetShrineTxtRecordInObjectData(pUnit, DATATBLS_GetShrinesTxtRecord(pInactiveUnitNode->nInteractType));
				}
				else if (!(pObjectsTxtRecord->nSubClass & 0x20))
				{
					pUnit->pObjectData->InteractType = pInactiveUnitNode->nInteractType;
					pUnit->bSparkChest = LOBYTE(pInactiveUnitNode->nFrame);

					if (pInactiveUnitNode->nDropItemCode)
					{
						pUnit->dwDropItemCode = pInactiveUnitNode->nDropItemCode;
					}
				}
				else
				{
					int32_t v45 = 2 * pObjectsTxtRecord->dwParm[2];
					int32_t v46 = pInactiveUnitNode->nInteractType;
					if (v46 < v45)
					{
						int32_t nFrameDelta = pObjectsTxtRecord->dwParm[0];
						D2_ASSERT(nFrameDelta);

						if (pInactiveUnitNode->nGameFrame >= (int32_t)(pGame->dwGameFrame + nFrameDelta))
						{
							pUnit->pObjectData->InteractType = v46;
							if (v46 < v45 && v45 - v46 > 0)
							{
								int32_t nOffset = nFrameDelta;
								int32_t nCounter = v45 - v46;
								do
								{
									EVENT_SetEvent(pGame, pUnit, EVENTTYPE_AITHINK, nOffset + pGame->dwGameFrame, 0, 0);

									nOffset += nFrameDelta;
									--nCounter;
								}
								while (nCounter);
							}
						}
						else
						{
							int32_t v50 = (pGame->dwGameFrame - pInactiveUnitNode->nGameFrame) / nFrameDelta;
							if (v50 >= 1)
							{
								v50 = 1;
							}

							int32_t v71 = (BYTE)(v50 + v46);
							if (v71 > v45)
							{
								v71 = v45;
							}

							pUnit->dwAnimMode = (BYTE)2 - v71 / (int32_t)pObjectsTxtRecord->dwParm[2];
							if (pUnit->dwAnimMode > 2)
							{
								pUnit->dwAnimMode = OBJMODE_NEUTRAL;
								pUnit->pObjectData->InteractType = v45;
							}
							else
							{
								v46 = v71;
								pUnit->pObjectData->InteractType = v46;
								if (v46 < v45 && v45 - v46 > 0)
								{
									int32_t nOffset = nFrameDelta;
									int32_t nCounter = v45 - v46;
									do
									{
										EVENT_SetEvent(pGame, pUnit, EVENTTYPE_AITHINK, nOffset + pGame->dwGameFrame, 0, 0);

										nOffset += nFrameDelta;
										--nCounter;
									}
									while (nCounter);
								}
							}
						}
					}
				}
			}

			D2_FREE_POOL(pGame->pMemoryPool, pInactiveUnitNode);
		}
		else
		{
			pUnit = SUNIT_GetServerUnit(pGame, pInactiveUnitNode->nUnitType, pInactiveUnitNode->nUnitId);
			if (pUnit)
			{
				SUNIT_Restore(pGame, pUnit, pRoom, pInactiveUnitNode->nX, pInactiveUnitNode->nY);

				pUnit->dwFlags |= UNITFLAG_INITSEEDSET;

				if (pInactiveUnitNode->nUnitType == UNIT_PLAYER)
				{
					D2_ASSERT(pInactiveUnitNode->nAnimMode == PLRMODE_DEAD);

					UNITS_ChangeAnimMode(pUnit, PLRMODE_DEAD);

				}
				else if (pInactiveUnitNode->nUnitType == UNIT_OBJECT)
				{
					pUnit->pObjectData->InteractType = pInactiveUnitNode->nInteractType;
				}
			}

			D2_FREE_POOL(pGame->pMemoryPool, pInactiveUnitNode);
		}

		pInactiveUnitNode = pNextUnitNode;
	}
}

//D2Game.0x6FCC40D0
D2AiControlStrc* __fastcall AIGENERAL_GetAiControlFromUnit(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		return pUnit->pMonsterData->pAiControl;
	}

	return nullptr;
}

//D2Game.0x6FCC40F0
void __fastcall SUNITINACTIVE_FreeInactiveMonsterNode(D2GameStrc* pGame, D2InactiveMonsterNodeStrc* pInactiveMonsterNode)
{
	if (pInactiveMonsterNode->pMapAI)
	{
		DRLGPRESET_FreeMapAI(pGame->pMemoryPool, pInactiveMonsterNode->pMapAI);
	}

	D2_FREE_POOL(pGame->pMemoryPool, pInactiveMonsterNode);
}

//D2Game.0x6FCC4120
D2UnitStrc* __fastcall SUNITINACTIVE_RestoreInactiveItem(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2InactiveItemNodeStrc* pInactiveItemNode)
{
	D2ItemSaveStrc pItemSave = {};
	uint8_t* pBitstream = &pInactiveItemNode->pBitstream;
	ITEMS_GetCompactItemDataFromBitstream(pBitstream, pInactiveItemNode->nBitstreamSize, 1, &pItemSave);

	uint32_t nSize = 0;
	D2UnitStrc* pItem = sub_6FC4EC10(pGame, pRoom, pBitstream, pInactiveItemNode->nBitstreamSize, 1, &pItemSave, &nSize, 96);
	if (!pItem)
	{
		return nullptr;
	}

	pBitstream = &pBitstream[nSize];
	int32_t nAvailableSize = pInactiveItemNode->nBitstreamSize - nSize;

	if (pInactiveItemNode->nFrame && pInactiveItemNode->nFrame < pGame->dwGameFrame + 15000)
	{
		pItem->pItemData->dwActionStamp = pGame->dwGameFrame + 15000;
	}
	else
	{
		pItem->pItemData->dwActionStamp = pInactiveItemNode->nFrame;
	}

	if (pItemSave.nItemFileIndex)
	{
		D2InventoryStrc* pInventory = pItem->pInventory;
		if (!pInventory)
		{
			pInventory = INVENTORY_AllocInventory(nullptr, pItem);
		}

		for (int32_t i = 0; i < pItemSave.nItemFileIndex; ++i)
		{
			ITEMS_GetCompactItemDataFromBitstream(pBitstream, nAvailableSize, 1, &pItemSave);
			D2UnitStrc* pSocketable = sub_6FC4EC10(pGame, nullptr, pBitstream, nAvailableSize, 1, &pItemSave, &nSize, 96);
			if (!pSocketable)
			{
				FOG_DisplayWarning("Unable to create item", __FILE__, __LINE__);
				return nullptr;
			}

			pBitstream = &pBitstream[nSize];
			nAvailableSize -= nSize;
			INVENTORY_PlaceItemInSocket(pInventory, pSocketable, 1);
			D2GAME_ITEMS_UpdateStatList_6FC4BC00(pGame, pSocketable, pItem);
		}
	}

	return pItem;
}

//D2Game.0x6FCC4270
void __fastcall SUNITINACTIVE_FreeInactiveUnitLists(D2GameStrc* pGame)
{
	for (int32_t i = 0; i < 5; ++i)
	{
		D2InactiveUnitListStrc* pCurrentList = pGame->pInactiveUnitList[i];
		while (pCurrentList)
		{
			D2InactiveUnitListStrc* pNextList = pCurrentList->pNext;

			D2InactiveUnitNodeStrc* pCurrentInactiveUnit = pCurrentList->pInactiveUnits;
			while (pCurrentInactiveUnit)
			{
				D2InactiveUnitNodeStrc* pNextInactiveUnit = pCurrentInactiveUnit->pNext;
				D2_FREE_POOL(pGame->pMemoryPool, pCurrentInactiveUnit);
				pCurrentInactiveUnit = pNextInactiveUnit;
			}

			D2InactiveItemNodeStrc* pCurrentInactiveItem = pCurrentList->pInactiveItems;
			while (pCurrentInactiveItem)
			{
				D2InactiveItemNodeStrc* pNextInactiveItem = pCurrentInactiveItem->pNext;
				D2_FREE_POOL(pGame->pMemoryPool, pCurrentInactiveItem);
				pCurrentInactiveItem = pNextInactiveItem;
			}

			D2InactiveMonsterNodeStrc* pCurrentInactiveMonster = pCurrentList->pInactiveMonsters;
			while (pCurrentInactiveMonster)
			{
				D2InactiveMonsterNodeStrc* pNextInactiveMonster = pCurrentInactiveMonster->pNext;
				AIGENERAL_FreeAllMinionLists(pGame, pCurrentInactiveMonster->pMinionList);
				SUNITINACTIVE_FreeInactiveMonsterNode(pGame, pCurrentInactiveMonster);
				pCurrentInactiveMonster = pNextInactiveMonster;
			}

			D2_FREE_POOL(pGame->pMemoryPool, pCurrentList);

			pCurrentList = pNextList;
		}
	}
}

//D2Game.0x6FCC4370
void __fastcall SUNITINACTIVE_CompressUnitIfNeeded(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	int32_t bCompress = 0;
	int32_t bIsDead = 0;

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		if (SUNIT_IsDead(pUnit))
		{
			bIsDead = 1;
		}

		if (DRLG_GetSaveMonstersFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)) || pUnit->dwFlags & UNITFLAG_ISRESURRECT)
		{
			bCompress = 1;
		}

		if (pUnit->dwAnimMode == MONMODE_DEAD)
		{
			D2SeedStrc* pSeed = pRoom ? &pRoom->pSeed : nullptr;
			if (ITEMS_RollRandomNumber(pSeed) % 3)
			{
				bCompress = 0;
			}
		}

		if (bIsDead)
		{
			if (STATES_CheckStateMaskUdeadOnUnit(pUnit))
			{
				bCompress = 0;
			}

			if (STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_GOOD)
			{
				bCompress = 0;
			}
		}

		if (pUnit->dwNodeIndex < 8)
		{
			if (!bIsDead)
			{
				bCompress = 1;
				sub_6FCBDD30(pUnit, 0, 1);
				D2GAME_TARGETS_Last_6FC40380(pGame, pUnit);
			}
			else
			{
				bCompress = 0;
			}
		}
		else
		{
			if (bIsDead && (pUnit->dwClassId == MONSTER_NECROSKELETON || pUnit->dwClassId == MONSTER_NECROMAGE))
			{
				bCompress = 0;
			}
		}

		if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_MINION | MONTYPEFLAG_UNIQUE) && bIsDead)
		{
			bCompress = 0;
		}

		int32_t bUnk = 0;
		if (pUnit->dwFlags & UNITFLAG_ISREVIVE)
		{
			D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
			if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
			{
				bUnk = sub_6FC7D5F0(pGame, pOwner, pUnit);
			}
			bCompress = 0;
		}

		if (pUnit->dwFlags & UNITFLAG_ISMERC)
		{
			bCompress = 0;
		}

		const D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->nRestore)
		{
			if (pMonStats2TxtRecord->nRestore == 2)
			{
				bCompress = 1;
			}
		}
		else
		{
			bCompress = 0;
		}

		if (bUnk)
		{
			SUNITINACTIVE_SetUnitFlagEx(pUnit, UNITFLAGEX_HASBEENDELETED, 1);

			if (bCompress)
			{
				SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
			}

			UNITROOM_UpdatePath(pUnit);
		}
		else
		{
			if (bCompress)
			{
				SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
			}

			SUNIT_RemoveUnit(pGame, pUnit);
		}
	}
	else
	{
		if (DRLG_GetSaveMonstersFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)) || pUnit->dwFlags & UNITFLAG_ISRESURRECT)
		{
			bCompress = 1;
		}

		switch (pUnit->dwUnitType)
		{
		case UNIT_PLAYER:
		{
			if (STATES_CheckState(pUnit, STATE_PLAYERBODY))
			{
				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);
				pUnit->dwFlagEx |= UNITFLAGEX_HASBEENDELETED;
				SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
				UNITROOM_UpdatePath(pUnit);
				return;
			}
			else
			{
				if (bCompress)
				{
					SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
				}
			}
			break;
		}
		case UNIT_OBJECT:
		{
			if (pUnit->dwClassId == OBJECT_TOWN_PORTAL || pUnit->dwClassId == OBJECT_PERMANENT_TOWN_PORTAL)
			{
				D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);
				pUnit->dwFlagEx |= UNITFLAGEX_HASBEENDELETED;
				SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
				UNITROOM_UpdatePath(pUnit);
				return;
			}

			const D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
			if (!pObjectsTxtRecord->nRestore || D2GAME_CheckIfSparklyChest_6FCBD7F0(pUnit) & 2)
			{
				bCompress = 0;
			}

			if (pObjectsTxtRecord->nRestoreVirgins && pUnit->dwAnimMode != OBJMODE_NEUTRAL)
			{
				bCompress = 0;
			}

			D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);

			if (bCompress)
			{
				SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
			}
			break;
		}
		case UNIT_ITEM:
		case UNIT_TILE:
		{
			SUNITINACTIVE_CompressInactiveUnit(pGame, pUnit);
			break;
		}
		default:
			break;
		}

		if (pUnit->dwUnitType != UNIT_ITEM)
		{
			SUNIT_RemoveUnit(pGame, pUnit);
		}
	}
}

//D2Game.0x6FCC4650
void __fastcall SUNITINACTIVE_CompressInactiveUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		for (int32_t i = 0; i < sgptDataTables->nCurseStates; ++i)
		{
			if (STATES_CheckState(pUnit, sgptDataTables->pCurseStates[i]))
			{
				D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, sgptDataTables->pCurseStates[i]);
				if (pStatList)
				{
					D2Common_10474(pUnit, pStatList);
					STATLIST_FreeStatList(pStatList);
				}
			}
		}

		D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_AITHINK, 0);

		D2InactiveMonsterNodeStrc* pInactiveMonsterNode = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2InactiveMonsterNodeStrc);

		pInactiveMonsterNode->unk0x1C = -1;
		pInactiveMonsterNode->nX = CLIENTS_GetUnitX(pUnit);
		pInactiveMonsterNode->nY = CLIENTS_GetUnitY(pUnit);
		pInactiveMonsterNode->nClassId = pUnit->dwClassId;
		pInactiveMonsterNode->nGameFrame = pGame->dwGameFrame;
		pInactiveMonsterNode->nAiSpecialState = AITHINK_GetSpecialStateFromAiControl(AIGENERAL_GetAiControlFromUnit(pUnit));
		pInactiveMonsterNode->nNameSeed = MONSTERUNIQUE_GetNameSeed(pUnit);
		SUNITINACTIVE_SaveSpecialMonsterParameters(pGame, pUnit, pInactiveMonsterNode);

		if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_CHAMPION))
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_TYPE_CHAMPION;
		}

		if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_SUPERUNIQUE))
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_TYPE_SUPERUNIQUE;
			pInactiveMonsterNode->nBossHcIdx = MONSTERUNIQUE_GetBossHcIdx(pUnit);
		}

		if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_OTHER))
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_TYPE_OTHER;
			QUESTS_SetGoldenBirdSpawn(pGame, pUnit);
		}

		if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_MINION))
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_TYPE_MINION;
		}

		if (MONSTER_CheckSummonerFlag(pUnit, 2))
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_UNKSUMMONERFLAG;
		}

		const int32_t nAlignment = STATLIST_GetUnitAlignment(pUnit);
		if (nAlignment == UNIT_ALIGNMENT_NEUTRAL)
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_ALIGNMENT_NEUTRAL;
		}
		else if (nAlignment == UNIT_ALIGNMENT_GOOD)
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_ALIGNMENT_GOOD;
		}
		else if (nAlignment == UNIT_ALIGNMENT_EVIL)
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_ALIGNMENT_EVIL;
		}

		if (pUnit->dwNodeIndex < 8)
		{
			FOG_DisplayAssert("UnitGetTargetListNo (hUnit) >= TARGET_LISTNO_MAX_REGULAR", __FILE__, __LINE__);
			exit(-1);
		}

		if (pUnit->dwNodeIndex != 11)
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_NODEINDEXUNEQUAL11;
		}

		pInactiveMonsterNode->nExperience = STATLIST_UnitGetStatValue(pUnit, STAT_EXPERIENCE, 0);
		pInactiveMonsterNode->nMaxHitpoints = STATLIST_GetMaxLifeFromUnit(pUnit);
		pInactiveMonsterNode->nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
		pInactiveMonsterNode->nLevelId = MONSTER_GetLevelId(pUnit);

		const uint8_t* pMonUMods = MONSTERUNIQUE_GetUMods(pUnit);
		if (pMonUMods)
		{
			for (int32_t i = 0; i < 9; ++i)
			{
				pInactiveMonsterNode->nMonUMods[i] = pMonUMods[i];
			}
		}

		if (pUnit->dwAnimMode == MONMODE_DEAD || pUnit->dwAnimMode == MONMODE_DEATH)
		{
			pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_DEAD;
		}

		D2MinionListStrc* pMinionList = nullptr;
		int32_t a2 = 0;
		int32_t a3 = 0;
		int32_t a4 = 0;
		int32_t a5 = 0;
		AIGENERAL_GetAiControlInfo(pUnit, &a2, &a3, &a4, &a5, &pMinionList);

		if (a2 != -1 && a3 == 1)
		{
			pInactiveMonsterNode->unk0x1C = a2;
			pInactiveMonsterNode->pMinionList = pMinionList;

			if (a4)
			{
				pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_MINIONRELATED1;
			}

			if (a5)
			{
				pInactiveMonsterNode->nTypeFlags |= INACTIVEMONSTERFLAG_MINIONRELATED2;
			}
		}

		pInactiveMonsterNode->nUnitId = pUnit->dwUnitId;
		pInactiveMonsterNode->nUnitFlags = pUnit->dwFlags;
		pInactiveMonsterNode->nUnitFlagsEx = pUnit->dwFlagEx;

		D2InactiveUnitListStrc* pInactiveUnitList = SUNITINACTIVE_GetListNodeFromRoom(pGame, pRoom, 1);
		pInactiveMonsterNode->pNext = pInactiveUnitList->pInactiveMonsters;
		pInactiveUnitList->pInactiveMonsters = pInactiveMonsterNode;
	}
	else
	{
		D2InactiveUnitListStrc* pInactiveUnitList = SUNITINACTIVE_GetListNodeFromRoom(pGame, pRoom, 1);

		if (pUnit->dwUnitType == UNIT_ITEM)
		{
			uint8_t pBitstream[1024] = {};

			const size_t nBitstreamSize = ITEMS_SerializeItemToBitstream(pUnit, pBitstream, ARRAY_SIZE(pBitstream), 1, 1, 0);

			D2InactiveItemNodeStrc* pInactiveItemNode = (D2InactiveItemNodeStrc*)D2_ALLOC_POOL(pGame->pMemoryPool, nBitstreamSize + 15);
			pInactiveItemNode->pNext = nullptr;
			pInactiveItemNode->nFrame = ITEMS_GetGroundRemovalTime(pGame, pUnit);
			pInactiveItemNode->nOwnerId = ITEMS_GetOwnerId(pUnit);
			pInactiveItemNode->nBitstreamSize = nBitstreamSize;
			memcpy(&pInactiveItemNode->pBitstream, pBitstream, nBitstreamSize + 1);

			if (ITEMS_CheckIfSocketable(pUnit) && pUnit->pInventory)
			{
				D2UnitStrc* pSocketable = INVENTORY_GetFirstItem(pUnit->pInventory);
				while (pSocketable)
				{
					D2UnitStrc* pNextSocketable = INVENTORY_GetNextItem(pSocketable);
					if (INVENTORY_UnitIsItem(pSocketable))
					{
						INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pSocketable);
						SUNIT_RemoveUnit(pGame, pSocketable);
					}
					pSocketable = pNextSocketable;
				}
			}

			SUNIT_RemoveUnit(pGame, pUnit);

			pInactiveItemNode->pNext = pInactiveUnitList->pInactiveItems;
			pInactiveUnitList->pInactiveItems = pInactiveItemNode;
		}
		else
		{
			D2InactiveUnitNodeStrc* pInactiveUnitNode = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2InactiveUnitNodeStrc);

			pInactiveUnitNode->nX = CLIENTS_GetUnitX(pUnit);
			pInactiveUnitNode->nY = CLIENTS_GetUnitY(pUnit);
			pInactiveUnitNode->nUnitType = pUnit->dwUnitType;
			pInactiveUnitNode->nClassId = pUnit->dwClassId;

			if (pUnit->dwUnitType == UNIT_PLAYER)
			{
				pInactiveUnitNode->nUnitId = pUnit->dwUnitId;
				pInactiveUnitNode->nAnimMode = pUnit->dwAnimMode;
			}
			else if (pUnit->dwUnitType == UNIT_OBJECT)
			{
				D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
				if (pUnit->dwAnimMode == OBJMODE_OPERATING && !pObjectsTxtRecord->nCycleAnim[1] && pObjectsTxtRecord->nMode[2])
				{
					pUnit->dwAnimMode = OBJMODE_OPENED;
				}

				pInactiveUnitNode->nAnimMode = pUnit->dwAnimMode;

				if (UNITS_IsShrine(pUnit))
				{
					const uint32_t nData = EVENT_GetEventFrame(pGame, pUnit, EVENTTYPE_ACTIVESTATE);
					pInactiveUnitNode->nUnitId = (uint32_t)nData >> 16;
					pInactiveUnitNode->nFrame = (WORD)nData;
				}
				else
				{
					if (pUnit->dwClassId == OBJECT_TOWN_PORTAL || pUnit->dwClassId == OBJECT_PERMANENT_TOWN_PORTAL || pUnit->dwClassId == OBJECT_PORTAL_TO_DURIELS_LAIR)
					{
						pInactiveUnitNode->nUnitId = pUnit->dwUnitId;
					}
					else
					{
						pInactiveUnitNode->nFrame = (BYTE)pUnit->bSparkChest;
					}
				}

				pInactiveUnitNode->nDropItemCode = pUnit->dwDropItemCode;
				pInactiveUnitNode->nInteractType = pUnit->pObjectData->InteractType;
			}
			else
			{
				pInactiveUnitNode->nAnimMode = pUnit->dwAnimMode;
			}

			pInactiveUnitNode->nGameFrame = pGame->dwGameFrame;
			pInactiveUnitNode->nUnitFlags = pUnit->dwFlags;
			pInactiveUnitNode->nUnitFlagsEx = pUnit->dwFlagEx;

			pInactiveUnitNode->pNext = pInactiveUnitList->pInactiveUnits;
			pInactiveUnitList->pInactiveUnits = pInactiveUnitNode;
		}
	}
}

//D2Game.0x6FCC4C90
void __fastcall SUNITINACTIVE_DeleteSingleListNode(D2GameStrc* pGame, uint16_t nUnitType, uint16_t nClassId, uint8_t nAct)
{
	if (nUnitType == UNIT_MONSTER)
	{
		D2InactiveUnitListStrc* pInactiveUnitList = pGame->pInactiveUnitList[nAct];
		while (pInactiveUnitList)
		{
			D2InactiveMonsterNodeStrc* pInactiveMonsterNode = pInactiveUnitList->pInactiveMonsters;
			D2InactiveMonsterNodeStrc* pPreviousMonsterNode = nullptr;

			while (pInactiveMonsterNode)
			{
				if (pInactiveMonsterNode->nClassId == nClassId)
				{
					if (pInactiveMonsterNode == pInactiveUnitList->pInactiveMonsters)
					{
						pInactiveUnitList->pInactiveMonsters = pInactiveMonsterNode->pNext;
					}
					else
					{
						pPreviousMonsterNode->pNext = pInactiveMonsterNode->pNext;
					}

					AIGENERAL_FreeAllMinionLists(pGame, pInactiveMonsterNode->pMinionList);
					SUNITINACTIVE_FreeInactiveMonsterNode(pGame, pInactiveMonsterNode);
					return;
				}

				pPreviousMonsterNode = pInactiveMonsterNode;
				pInactiveMonsterNode = pInactiveMonsterNode->pNext;
			}

			pInactiveUnitList = pInactiveUnitList->pNext;
		}
	}
	else
	{
		D2InactiveUnitListStrc* pInactiveUnitList = pGame->pInactiveUnitList[nAct];
		while (pInactiveUnitList)
		{
			D2InactiveUnitNodeStrc* pInactiveUnitNode = pInactiveUnitList->pInactiveUnits;
			D2InactiveUnitNodeStrc* pPreviousUnitNode = nullptr;

			while (pInactiveUnitNode)
			{
				if (pInactiveUnitNode->nUnitType == nUnitType && pInactiveUnitNode->nClassId == nClassId)
				{
					if (pInactiveUnitNode == pInactiveUnitList->pInactiveUnits)
					{
						pInactiveUnitList->pInactiveUnits = pInactiveUnitNode->pNext;
					}
					else
					{
						pPreviousUnitNode->pNext = pInactiveUnitNode->pNext;
					}

					D2_FREE_POOL(pGame->pMemoryPool, pInactiveUnitNode);
					return;
				}

				pPreviousUnitNode = pInactiveUnitNode;
				pInactiveUnitNode = pInactiveUnitNode->pNext;
			}

			pInactiveUnitList = pInactiveUnitList->pNext;
		}
	}
}

//D2Game.0x6FCC4DC0
D2InactiveUnitListStrc* __fastcall SUNITINACTIVE_GetListNodeFromActAndCoordinates(D2GameStrc* pGame, int32_t nAct, int32_t nX, int32_t nY, int32_t bAllocNewNode)
{
	int32_t bFound = 0;

	D2InactiveUnitListStrc* pCurrent = pGame->pInactiveUnitList[nAct];
	D2InactiveUnitListStrc* pPrevious = nullptr;
	while (pCurrent)
	{
		if (nX == pCurrent->nX && nY == pCurrent->nY)
		{
			bFound = 1;
			break;
		}

		if (nX > pCurrent->nX)
		{
			break;
		}

		pPrevious = pCurrent;
		pCurrent = pCurrent->pNext;
	}

	if (bFound)
	{
		return pCurrent;
	}

	if (bAllocNewNode)
	{
		D2InactiveUnitListStrc* pNew = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2InactiveUnitListStrc);

		pNew->nX = nX;
		pNew->nY = nY;
		pNew->pNext = pCurrent;

		if (pPrevious)
		{
			pPrevious->pNext = pNew;
		}
		else
		{
			pGame->pInactiveUnitList[nAct] = pNew;
		}

		return pNew;
	}

	return nullptr;
}

//D2Game.0x6FCC4E80
D2InactiveUnitListStrc* __fastcall SUNITINACTIVE_GetListNodeFromRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t bAllocNewNode)
{
	D2DrlgCoordsStrc pCoord = {};
	DUNGEON_GetRoomCoordinates(pRoom, &pCoord);

	return SUNITINACTIVE_GetListNodeFromActAndCoordinates(pGame, DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)), pCoord.nSubtileX, pCoord.nSubtileY, bAllocNewNode);
}

//D2Game.0x6FCC4ED0
void __fastcall SUNITINACTIVE_RestoreSpecialMonsterParameters(D2GameStrc* pGame, D2UnitStrc* pUnit, D2InactiveMonsterNodeStrc* pInactiveMonsterNode)
{
	const int32_t nClassId = pUnit->dwClassId;

	const D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);

	if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT] || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC] || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTOWN])
	{
		D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
		if (pAiCmd)
		{
			pAiCmd->nCmdParam[1] = pInactiveMonsterNode->nCmdParam1;
			pAiCmd->nCmdParam[2] = pInactiveMonsterNode->nCmdParam2;
			pAiCmd->nCmdParam[3] = 0;
		}

		D2MapAIStrc** ppMapAi = AIGENERAL_GetMapAiFromUnit(pUnit);
		if (pInactiveMonsterNode->pMapAI)
		{
			DRLGPRESET_ChangeMapAI(&pInactiveMonsterNode->pMapAI, ppMapAi);
		}
	}

	if (nClassId == MONSTER_BLOODRAVEN || nClassId == MONSTER_DIABLO || pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_TYPE_SUPERUNIQUE && pInactiveMonsterNode->nBossHcIdx == 6)
	{
		if (pInactiveMonsterNode->nCmdParam1 || pInactiveMonsterNode->nCmdParam2)
		{
			D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
			if (pAiCmd)
			{
				pAiCmd->nCmdParam[1] = pInactiveMonsterNode->nCmdParam1;
				pAiCmd->nCmdParam[2] = pInactiveMonsterNode->nCmdParam2;
			}
		}
	}

	if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		if (pMonStatsTxtRecord->nBaseId == MONSTER_SANDMAGGOT1)
		{
			AIGENERAL_SetAiControlParam(pUnit, 3, pInactiveMonsterNode->nCmdParam1);
		}
		else if (pMonStatsTxtRecord->nBaseId == MONSTER_BAALTHRONE)
		{
			AIGENERAL_SetAiControlParam(pUnit, 1, pInactiveMonsterNode->nCmdParam1);
		}
	}

	if (nClassId == MONSTER_DOOMKNIGHT3)
	{
		if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pUnit->pMonsterData->nComponent[10] = (BYTE)pInactiveMonsterNode->nCmdParam1;
			pUnit->pMonsterData->nComponent[11] = (BYTE)pInactiveMonsterNode->nCmdParam1;
		}
	}
	else if (nClassId == MONSTER_DOOMKNIGHT2)
	{
		if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pUnit->pMonsterData->nComponent[10] = (BYTE)pInactiveMonsterNode->nCmdParam1;
		}
	}

	if (pInactiveMonsterNode->nCmdParam1 == dword_6FD3F45C)
	{
		if (pInactiveMonsterNode->nCmdParam2 & dword_6FD3F460)
		{
			pUnit->dwFlags |= UNITFLAG_NOXP;
		}

		if (pInactiveMonsterNode->nCmdParam2 & dword_6FD3F464)
		{
			pUnit->dwFlags |= UNITFLAG_NOTC;
		}
	}
}

//D2Game.0x6FCC50B0
void __fastcall SUNITINACTIVE_SaveSpecialMonsterParameters(D2GameStrc* pGame, D2UnitStrc* pUnit, D2InactiveMonsterNodeStrc* pInactiveMonsterNode)
{
	const int32_t nClassId = pUnit->dwClassId;

	const D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);

	if (pUnit->dwFlags & UNITFLAG_NOXP)
	{
		pInactiveMonsterNode->nCmdParam1 = dword_6FD3F45C;
		pInactiveMonsterNode->nCmdParam2 |= dword_6FD3F460;
	}

	if (pUnit->dwFlags & UNITFLAG_NOTC)
	{
		pInactiveMonsterNode->nCmdParam1 = dword_6FD3F45C;
		pInactiveMonsterNode->nCmdParam2 |= dword_6FD3F464;
	}

	if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]
		|| pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC]
		|| pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTOWN])
	{
		const D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
		if (pAiCmd)
		{
			if (pAiCmd->nCmdParam[1])
			{
				pInactiveMonsterNode->nCmdParam1 = pAiCmd->nCmdParam[1];
				pInactiveMonsterNode->nCmdParam2 = pAiCmd->nCmdParam[2];
			}
		}

		D2MapAIStrc** ppMapAi = AIGENERAL_GetMapAiFromUnit(pUnit);
		if (*ppMapAi)
		{
			DRLGPRESET_ChangeMapAI(ppMapAi, &pInactiveMonsterNode->pMapAI);
		}
	}

	if (nClassId == MONSTER_BLOODRAVEN || nClassId == MONSTER_DIABLO || pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_TYPE_SUPERUNIQUE && pInactiveMonsterNode->nBossHcIdx == 6)
	{
		const D2AiCmdStrc* pAiCmd = AIGENERAL_SetCurrentAiCommand(pGame, pUnit, 10, 0);
		if (pAiCmd)
		{
			pInactiveMonsterNode->nCmdParam1 = pAiCmd->nCmdParam[1];
			pInactiveMonsterNode->nCmdParam2 = pAiCmd->nCmdParam[2];
		}
	}

	if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		if (pMonStatsTxtRecord->nBaseId == MONSTER_SANDMAGGOT1)
		{
			pInactiveMonsterNode->nCmdParam1 = AIGENERAL_GetAiControlParam(pUnit, 3);
		}
		else if (pMonStatsTxtRecord->nBaseId == MONSTER_BAALTHRONE)
		{
			pInactiveMonsterNode->nCmdParam1 = AIGENERAL_GetAiControlParam(pUnit, 1);
		}
	}

	if (nClassId == MONSTER_DOOMKNIGHT2 || nClassId == MONSTER_DOOMKNIGHT3)
	{
		if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
		{
			pInactiveMonsterNode->nCmdParam1 = pUnit->pMonsterData->nComponent[10];
		}
		else
		{
			pInactiveMonsterNode->nCmdParam1 = 0;
		}
	}
}

//D2Game.0x6FCC52C0
int32_t __fastcall SUNITINACTIVE_IsUnitInsideRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoomNear, int32_t nGameX, int32_t nGameY, int32_t nClassId)
{
	D2_ASSERT(pGame);
	D2_ASSERT(pRoomNear);
	D2_ASSERT(nGameX && nGameY);

	const D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(pRoomNear, nGameX, nGameY);
	const uint8_t nAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoomNear));
	if (!pRoom)
	{
		pRoom = DUNGEON_FindRoomBySubtileCoordinates(pGame->pAct[nAct], nGameX, nGameY);
	}

	if (pRoom)
	{
		for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pRoomNext)
		{
			if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->dwClassId == nClassId && SUNIT_IsDead(pUnit))
			{
				return 1;
			}
		}
	}

	const D2InactiveUnitListStrc* pInactiveUnitList = SUNITINACTIVE_GetListNodeFromActAndCoordinates(pGame, nAct, nGameX, nGameY, 0);
	if (!pInactiveUnitList)
	{
		return 0;
	}

	for (const D2InactiveMonsterNodeStrc* pInactiveMonsterNode = pInactiveUnitList->pInactiveMonsters; pInactiveMonsterNode; pInactiveMonsterNode = pInactiveMonsterNode->pNext)
	{
		if (pInactiveMonsterNode->nClassId == nClassId && pInactiveMonsterNode->nTypeFlags & INACTIVEMONSTERFLAG_DEAD)
		{
			return 1;
		}
	}

	return 0;
}

//D2Game.0x6FCC5490
void __fastcall SUNITINACTIVE_DeleteExpiredItemNodes(D2GameStrc* pGame, int32_t nAct)
{
	for (D2InactiveUnitListStrc* i = pGame->pInactiveUnitList[nAct]; i; i = i->pNext)
	{
		D2InactiveItemNodeStrc* pPrevious = nullptr;
		D2InactiveItemNodeStrc* pCurrent = i->pInactiveItems;

		while (pCurrent)
		{
			D2InactiveItemNodeStrc* pNext = pCurrent->pNext;

			if (pCurrent->nFrame && (int32_t)pGame->dwGameFrame > pCurrent->nFrame)
			{
				if (pPrevious)
				{
					pPrevious->pNext = pNext;
				}
				else
				{
					i->pInactiveItems = pNext;
				}

				D2_FREE_POOL(pGame->pMemoryPool, pCurrent);
			}
			else
			{
				pPrevious = pCurrent;
			}

			pCurrent = pNext;
		}
	}
}

//D2Game.0x6FCC54F0
void __fastcall SUNITINACTIVE_SetUnitFlagEx(D2UnitStrc* pUnit, uint32_t nFlag, int32_t bSet)
{
	if (!pUnit)
	{
		return;
	}

	if (bSet)
	{
		pUnit->dwFlagEx |= nFlag;
	}
	else
	{
		pUnit->dwFlagEx &= ~nFlag;
	}
}
