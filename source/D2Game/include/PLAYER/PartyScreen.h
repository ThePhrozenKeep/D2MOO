#pragma once

#include "D2Structs.h"
#include <Units/Units.h>


//D2Game.0x6FC7AB50
void __fastcall PARTYSCREEN_ToggleLootability(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7AC20
void __fastcall PARTYSCREEN_SendEventUpdateToClient(D2ClientStrc* pClient, int32_t nUnitGUID, uint8_t nType);
//D2Game.0x6FC7AD10
void __fastcall D2GAME_PARTYSCREEN_Command2_6FC7AD10(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7AD70
void __fastcall D2GAME_PARTYSCREEN_Command3_6FC7AD70(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7ADD0
void __fastcall D2GAME_PARTYSCREEN_Command8_6FC7ADD0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7AED0
void __fastcall sub_6FC7AED0(D2GameStrc* pGame, D2UnitStrc* pLocalPlayer, void* a3);
//D2Game.0x6FC7B030
void __fastcall PARTYSCREEN_InvitationCancelled(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B0E0
void __fastcall PARTYSCREEN_ReceivedInvitation(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B160
void __fastcall PARTYSCREEN_PlayerLeftParty(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B190
void __fastcall PARTYSCREEN_PartyIterate_PlayerLeftParty(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg);
//D2Game.0x6FC7B1D0
void __fastcall D2GAME_PARTYSCREEN_Command4_6FC7B1D0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B3F0
void __fastcall sub_6FC7B3F0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg);
//D2Game.0x6FC7B450
void __fastcall sub_6FC7B450(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7B460
void __fastcall sub_6FC7B460(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg);
//D2Game.0x6FC7B4C0
int32_t __fastcall PARTYSCREEN_CommandHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCallbackId, int32_t nOtherPlayerGUID, int32_t nParam);
