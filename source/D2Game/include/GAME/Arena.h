#pragma once

#include <Units/Units.h>

#pragma pack(1)


enum D2ArenaScoreTypes
{
    ARENASCORE_SUICIDE,
    ARENASCORE_PLAYERKILL,
    ARENASCORE_PLAYERKILLPERCENT,
    ARENASCORE_MONSTERKILL,
    ARENASCORE_PLAYERDEATH,
    ARENASCORE_PLAYERDEATHPERCENT,
    ARENASCORE_MONSTERDEATH,
    NUM_ARENA_SCORES,
};

struct D2ArenaStrc
{
    int32_t nAlternateStartTown;				//0x00
    int32_t nType;								//0x04
    uint32_t fFlags;							//0x08 D2GameFlags
    int32_t nTemplate;							//0x0C - uint8_t with 3 pad
};

struct D2ArenaUnitStrc
{
    int32_t nScore;								//0x00
    BOOL bUpdateScore;						    //0x04 
};

#pragma pack()

//D2Game.0x6FC31010
int32_t __fastcall D2Game_10001_Return0();
//D2Game.0x6FC31040
void __fastcall ARENA_AllocArena(D2GameStrc* pGame, int32_t nUnused, uint32_t nFlags, int32_t nTemplate);
//D2Game.0x6FC31090
void __fastcall ARENA_FreeArena(D2GameStrc* pGame);
//D2Game.0x6FC310C0
void __fastcall ARENA_AllocArenaUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC31110
void __fastcall ARENA_FreeArenaUnit(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC31160
int32_t __fastcall ARENA_GetAlternateStartTown(D2GameStrc* pGame);
//D2Game.0x6FC31190
void __fastcall ARENA_ProcessKillEvent(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender);
//D2Game.0x6FC31280
void __fastcall ARENA_UpdateScore(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2ArenaScoreTypes eScore);
//D2Game.0x6FC31470
void __fastcall ARENA_SynchronizeWithClients(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC315C0
void __fastcall ARENA_SendScoresToClient(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC31690
uint32_t __fastcall ARENA_NeedsClientUpdate(D2GameStrc* pGame);
//D2Game.0x6FC316D0
uint32_t __fastcall ARENA_IsInArenaMode(D2GameStrc* pGame);
//D2Game.0x6FC31710
uint32_t __fastcall ARENA_IsActive(D2GameStrc* pGame);
//D2Game.0x6FC31750
uint32_t __fastcall ARENA_GetFlags(D2GameStrc* pGame);
//D2Game.0x6FC31780
int32_t __fastcall ARENA_Return0();
//D2Game.0x6FC31790
uint32_t __fastcall ARENA_ShouldTreatClassIdAsTemplateId(D2GameStrc* pGame);
//D2Game.0x6FC317C0
int32_t __fastcall ARENA_GetTemplateType(D2GameStrc* pGame);
