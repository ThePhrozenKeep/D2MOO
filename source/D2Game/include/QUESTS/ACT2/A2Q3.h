#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//TaintedSun
struct D2Act2Quest3Strc						//sizeof 0xA0
{
	uint8_t unused0x00;						//0x00
	uint8_t bDarkenTimerCreated;			//0x01
	uint8_t bDarknessApplied;				//0x02
	uint8_t bDarknessPending;				//0x03
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
//1.14d: 0x0059A040
void __fastcall ACT2Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA14B0
//1.14d: 0x0059A1C0
bool __fastcall ACT2Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA1640
//1.14d: 0x00599F50
int32_t __fastcall ACT2Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA16B0
//1.14d: 0x0059A100
int32_t __fastcall ACT2Q3_UnitIterate_UpdateClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA1730
//1.14d: 0x0059EEA0
void __fastcall ACT2Q3_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA17F0
//1.14d: 0x0059EBE0
void __fastcall ACT2Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA19B0
//1.14d: 0x00599FB0
void __fastcall ACT2Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA19E0
//1.14d: 0x0059A270
void __fastcall ACT2Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA1AC0
//1.14d: 0x0059EE00
void __fastcall ACT2Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA1C00
//1.14d: 0x0059ED80
bool __fastcall ACT2Q3_DarkenEnvironment(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA1CA0
//1.14d: 0x0059A480
bool __fastcall ACT2Q3_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA1D10
//1.14d: 0x0059A4F0
int32_t __fastcall ACT2Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA1D90
//1.14d: 0x0059A5D0
void __fastcall ACT2Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA1F00
//1.14d: 0x0059A7E0
int32_t __fastcall OBJECTS_OperateFunction24_TaintedSunAltar(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA2320
//1.14d: 0x00599FE0
int32_t __fastcall ACT2Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2380
//1.14d: 0x0059A0B0
int32_t __fastcall ACT2Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA23D0
//1.14d: 0x0059A170
int32_t __fastcall ACT2Q3_UnitIterate_SendPacket0x53ToClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2420
//1.14d: 0x0059A680
int32_t __fastcall ACT2Q3_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA24A0
//1.14d: 0x0059A700
bool __fastcall ACT2Q3_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA24D0
//1.14d: 0x0059A730
int32_t __fastcall ACT2Q3_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2510
//1.14d: 0x0059A770
int32_t __fastcall ACT2Q3_UnitIterate_DetermineViperAmuletDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2580
void __fastcall ACT2Q3_ApplyPendingDarkness(D2GameStrc* pGame, uint8_t nActNo);
