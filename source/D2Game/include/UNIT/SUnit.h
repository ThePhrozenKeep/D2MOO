#pragma once

#include <GAME/Game.h>
#include <Units/Units.h>
#include <Drlg/D2DrlgPreset.h>

#include <GAME/Event.h>

//1.10f: D2Game.0x6FD4DC40
//1.14d: 0x008846E8
extern uint32_t gnUnitInitSeed;

#pragma pack(push, 1)
struct D2UnitInfoStrc		// sizeof(0x34)
{
	int32_t nUnitType;		// 0x00
	int32_t nClassId;		// 0x04
	int32_t nUnitGUID;		// 0x08
	int32_t nAnimMode;		// 0x0C
	int32_t nX;				// 0x10
	int32_t nY;				// 0x14
	int32_t nOwnerType;		// 0x18
	int32_t nOwnerGUID;		// 0x1C
	int32_t unk0x20;		// 0x20
	char szDescription[16];	// 0x24
};

struct D2UnitDescriptionListStrc
{
	uint32_t nClassId;					//0x00
	uint32_t nCount;					//0x04
	uint32_t nDefaultCount;				//0x08 Always 0 in 1.10f. Used to be added to nCount for display on the server
	char szDescription[128];			//0x0C
	D2UnitDescriptionListStrc* pNext;	//0x8C
}; // Size=0x90
#pragma pack(pop)


//D2Game.0x6FCBAEE0
//1.14d: 0x00555600
void __fastcall SUNIT_RemoveUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBB160
//1.14d: 0x00552DB0
D2UnitStrc* __fastcall SUNIT_GetNextUnitFromList(D2UnitStrc* pUnit);
//D2Game.0x6FCBB190
//1.14d: 0x00554670
void __fastcall SUNIT_SynchronizeRoomClients(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FCBB440
//1.14d: 0x005550B0
void __fastcall SUNIT_WarpPlayer(D2GameStrc* pGame, D2UnitStrc* pTarget, D2UnitStrc* pRoomTile);
//D2Game.0x6FCBB630
//1.14d: 0x00552DF0
void __fastcall SUNIT_InitSeed(D2UnitStrc* pUnit, D2SeedStrc* pSeed);
//D2Game.0x6FCBB6C0
//1.14d: 0x00555230
D2UnitStrc* __fastcall SUNIT_AllocUnitData(int32_t nUnitType, int32_t nClassId, int32_t nX, int32_t nY, D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, char nAllocFlags, int32_t nMode, DWORD nOverrideUnitGUID);
//D2Game.0x6FCBBB00
//1.14d: 0x00552F60
D2UnitStrc* __fastcall SUNIT_GetServerUnit(D2GameStrc* pGame, int32_t nUnitType, int32_t nUnitGUID);
//D2Game.0x6FCBBB70
//1.14d: 0x00552FD0
D2UnitStrc* __fastcall SUNIT_GetOwner(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBBC00
//1.14d: 0x00553010
D2UnitStrc* __fastcall SUNIT_GetLastAttacker(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBBCB0
//1.14d: 0x00554850
void __fastcall SUNIT_Add(D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t bSkipRoomUpdate);
//D2Game.0x6FCBBFE0
//1.14d: 0x00554A30
void __fastcall SUNIT_Restore(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FCBC280
//1.14d: 0x00553180
void __fastcall SUNIT_InitClientInPlayerData(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCBC2E0
//1.14d: 0x005531C0
D2ClientStrc* __fastcall SUNIT_GetClientFromPlayer(D2UnitStrc* pPlayer, const char* szFile, int32_t nLine);
//D2Game.0x6FCBC300
//1.14d: 0x00553220
void __fastcall SUNIT_ClearUpdateData(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBC480
//1.14d: 0x00553380
void __fastcall SUNIT_AttachSound(D2UnitStrc* pUnit, int32_t nUpdateType, D2UnitStrc* pUpdateUnit);
//D2Game.0x6FCBC4D0
//1.14d: 0x005533D0
void __fastcall SUNIT_InitializeAnimation(D2UnitStrc* pUnit);
//D2Game.0x6FCBC590
//1.14d: 0x005557D0
D2UnitStrc* __fastcall SUNIT_CreatePresetUnit(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nUnitType, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode, int32_t nUnitFlags);
//D2Game.0x6FCBC6F0
//1.14d: 0x00555910
void __fastcall SUNIT_SpawnPresetUnit(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2PresetUnitStrc* pPresetUnit);
//D2Game.0x6FCBC780
//1.14d: 0x005559A0
void __fastcall SUNIT_SpawnPresetUnitsInRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FCBC7E0
//1.14d: 0x00553490
void __fastcall SUNIT_ValidateTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBC900
//1.14d: 0x00553540
D2UnitStrc* __stdcall SUNIT_GetTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBC930
//1.14d: 0x00554CA0
int32_t __fastcall SUNIT_UpdateUnitPath(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBC9A0
//1.14d: 0x00553570
void __fastcall SUNIT_SetCombatMode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode);
//D2Game.0x6FCBC9C0
//1.14d: 0x00553590
void __fastcall SUNIT_LinkPortalCoordinates(D2UnitStrc* pFirst, D2UnitStrc* pSecond);
//D2Game.0x6FCBCB30
//1.14d: 0x00553720
D2UnitStrc* __fastcall SUNIT_GetPortalOwner(D2GameStrc* pGame, D2UnitStrc* pPortal);
//D2Game.0x6FCBCC40
//1.14d: 0x005537D0
void __fastcall SUNIT_IterateUnitsOfType(D2GameStrc* pGame, int32_t nType, void* pArg, int32_t(__fastcall* pfIterate)(D2GameStrc*, D2UnitStrc*, void*));
//D2Game.0x6FCBCD70
//1.14d: 0x005538D0
void __fastcall SUNIT_IterateLivingPlayers(D2GameStrc* pGame, void(__fastcall* pfIterate)(D2GameStrc*, D2UnitStrc*, void*), void* pArg);
//D2Game.0x6FCBCE30
//1.14d: 0x00553970
void __fastcall SUNIT_SetTimerOnUnit(D2UnitStrc* pUnit, D2EventTimerStrc* pTimer);
//D2Game.0x6FCBCE40
//1.14d: 0x00553980
D2EventTimerStrc* __fastcall SUNIT_GetTimerFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCBCE50
//1.14d: 0x00553990
void __fastcall D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBCE70
//1.14d: 0x005539B0
void __fastcall SUNIT_ScheduleAnimationEvents(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBCFD0
//1.14d: 0x00553B10
void __fastcall SUNIT_RewindAnimationByPercentage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nRewindPercentage);
//D2Game.0x6FCBD120
//1.14d: 0x00553C70
void __fastcall SUNIT_RewindAnimationByFrames(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nFramesToRewind);
//D2Game.0x6FCBD260
//1.14d: 0x00553DC0
void __fastcall D2GAME_SKILLS_RewindSkillEx_6FCBD260(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nElapsedFrames);
//D2Game.0x6FCBD3A0
//1.14d: 0x00553F00
void __fastcall SUNIT_ResetRecurringModeChangeEvent(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBD3D0
//1.14d: 0x00553F30
void __fastcall SUNIT_FillUnitInfo(D2UnitStrc* pUnit, D2UnitInfoStrc* pInfo);
//D2Game.0x6FCBD4D0
//1.14d: 0x00554D00
D2UnitStrc* __fastcall SUNIT_GetInteractUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBD550
//1.14d: 0x00555AE0
void __fastcall SUNIT_RemoveAllItemsFromInventory(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBD730
//1.14d: 0x00554010
D2GameStrc* __fastcall SUNIT_GetGameFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCBD760
//1.14d: 0x00554040
void __fastcall SUNIT_SetTrackedSummonGUID(D2UnitStrc* pUnit, int32_t nSummonGUID);
//D2Game.0x6FCBD790
//1.14d: 0x00554070
uint32_t __fastcall SUNIT_GetTrackedSummonGUID(D2UnitStrc* pUnit);
//D2Game.0x6FCBD7C0
//1.14d: 0x005540A0
void __fastcall D2GAME_SetSparkChest_6FCBD7C0(D2UnitStrc* pUnit, uint8_t nSparkChest);
//D2Game.0x6FCBD7F0
//1.14d: 0x005540D0
uint8_t __fastcall D2GAME_CheckIfSparklyChest_6FCBD7F0(D2UnitStrc* pUnit);
//D2Game.0x6FCBD820
//1.14d: 0x00554100
int32_t __stdcall SUNIT_GetInteractInfo(D2UnitStrc* pUnit, int32_t* pInteractUnitType, int32_t* pInteractUnitGUID);
//D2Game.0x6FCBD840
//1.14d: 0x00554120
void __stdcall SUNIT_SetInteractInfo(D2UnitStrc* pUnit, int32_t nInteractUnitType, int32_t nInteractUnitGUID);
//D2Game.0x6FCBD890
//1.14d: 0x00554190
void __stdcall SUNIT_ResetInteractInfo(D2UnitStrc* pUnit);
//D2Game.0x6FCBD8B0
//1.14d: 0x005541B0
int32_t __fastcall SUNIT_IsDead(D2UnitStrc* pUnit);
//D2Game.0x6FCBD900
//1.14d: 0x00554200
int32_t __fastcall SUNIT_AreUnitsHostile(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FCBDA10
//1.14d: 0x00552F30
D2UnitStrc** __fastcall SUNIT_GetUnitList(int32_t nUnitType, D2GameStrc* pGame, int32_t nUnitGUID);
//D2Game.0x6FCBDA40
//1.14d: 0x005542C0
int32_t __fastcall SUNIT_CanPetBeTargetedBySkill(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pPet, int32_t nSkillId);
//D2Game.0x6FCBDAD0
//1.14d: 0x00554D20
int32_t __fastcall SUNIT_CanAllyBeTargetedBySkill(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pPet, int32_t nSkillId);
//D2Game.0x6FCBDC60
//1.14d: 0x00554DE0
int32_t __fastcall SUNIT_AreUnitsAligned(D2GameStrc* pGame, D2UnitStrc* pUnit1, D2UnitStrc* pUnit);
//D2Game.0x6FCBDD30
//1.14d: 0x005543B0
void __fastcall SUNIT_SetAlignment(D2UnitStrc* pUnit, uint8_t nAlignNew, int32_t bUpdateMonsterRegion);
//D2Game.0x6FCBDE90
//1.14d: 0x005544B0
void __fastcall SUNIT_SetUninterruptable(D2UnitStrc* pUnit, int32_t bSetUninterruptable);
//D2Game.0x6FCBDF90
//1.14d: 0x00554570
int32_t __fastcall SUNIT_EventClearTeleportCoordinates(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t nUnusedParam1, int32_t nUnusedParam2);
//D2Game.0x6FCBDFE0
//1.14d: 0x00554EA0
int32_t __fastcall SUNIT_TeleportUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pInputRoom, int32_t nX, int32_t nY, int32_t bSkipCollisionSearch, int32_t bSyncOtherPlayersOnly);
//D2Game.0x6FCBE2D0
//1.14d: 0x00554630
int16_t __fastcall SUNIT_GetPartyId(D2UnitStrc* pPlayer);
