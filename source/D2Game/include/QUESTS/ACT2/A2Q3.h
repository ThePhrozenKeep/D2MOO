#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//TaintedSun
struct D2Act2Quest3Strc						//sizeof 0xA0
{
	uint8_t unused0x00;						//0x00
	uint8_t bDarkenTimerCreated;			//0x01
	uint8_t bRewarded;						//0x02
	uint8_t bBlackened;						//0x03
	uint8_t bAltarDestroyed;				//0x04
	uint8_t bAltarInitialized;				//0x05
	uint8_t unk0x06;						//0x06 TODO
	uint8_t pad0x07;						//0x07
	int nAltarMode;							//0x08
	int nAltarGUID;							//0x0C
	D2ActiveRoomStrc* pRoom;						//0x10
	D2QuestGUIDStrc tPlayerGUIDs;					//0x14
	int nAltarLevelId;						//0x98
	int nViperAmuletDropCount;				//0x9C
};

#pragma pack()



//D2Game.0x6FCA1440
void __fastcall ACT2Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA14B0
bool __fastcall ACT2Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA1640
int32_t __fastcall ACT2Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA16B0
int32_t __fastcall ACT2Q3_UnitIterate_UpdateClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA1730
void __fastcall ACT2Q3_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA17F0
void __fastcall ACT2Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA19B0
void __fastcall ACT2Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA19E0
void __fastcall ACT2Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA1AC0
void __fastcall ACT2Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA1C00
bool __fastcall ACT2Q3_DarkenEnvironment(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA1CA0
bool __fastcall ACT2Q3_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA1D10
int32_t __fastcall ACT2Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA1D90
void __fastcall ACT2Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA1F00
int32_t __fastcall OBJECTS_OperateFunction24_TaintedSunAltar(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA2320
int32_t __fastcall ACT2Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2380
int32_t __fastcall ACT2Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA23D0
int32_t __fastcall ACT2Q3_UnitIterate_SendPacket0x53ToClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2420
int32_t __fastcall ACT2Q3_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA24A0
bool __fastcall ACT2Q3_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA24D0
int32_t __fastcall ACT2Q3_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2510
int32_t __fastcall ACT2Q3_UnitIterate_DetermineViperAmuletDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2580
void __fastcall ACT2Q3_BrightenEnvironment(D2GameStrc* pGame, uint8_t nActNo);
