#pragma once

#include <Units/Units.h>


//D2Game.0x6FC7AB50
//1.14d: 0x005A57B0
void __fastcall PARTYSCREEN_ToggleLootability(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7AC20
//1.14d: 0x005A5690
void __fastcall PARTYSCREEN_SendEventUpdateToClient(D2ClientStrc* pClient, int32_t nUnitGUID, uint8_t nType);
//D2Game.0x6FC7AD10
//1.14d: 0x005A5840
void __fastcall PARTYSCREEN_ToggleIgnore(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7AD70
//1.14d: 0x005A58A0
void __fastcall PARTYSCREEN_ToggleSquelch(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7ADD0
//1.14d: 0x005A5A30
void __fastcall PARTYSCREEN_AcceptInvitation(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7AED0
//1.14d: 0x005A5970
void __fastcall PARTYSCREEN_PlayerIterate_PartyJoined(D2GameStrc* pGame, D2UnitStrc* pLocalPlayer, void* pArgs);
//D2Game.0x6FC7B030
//1.14d: 0x005A5B30
void __fastcall PARTYSCREEN_InvitationCancelled(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B0E0
//1.14d: 0x005A5BE0
void __fastcall PARTYSCREEN_ReceivedInvitation(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B160
//1.14d: 0x005A5D20
void __fastcall PARTYSCREEN_PlayerLeftParty(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B190
//1.14d: 0x005A5C70
void __fastcall PARTYSCREEN_PartyIterate_PlayerLeftParty(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg);
//D2Game.0x6FC7B1D0
//1.14d: 0x005A5E50
void __fastcall PARTYSCREEN_ToggleHostile(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure);
//D2Game.0x6FC7B3F0
//1.14d: 0x005A5D50
void __fastcall PARTYSCREEN_PartyIterate_OpenHostility(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg);
//D2Game.0x6FC7B450
//1.14d: 0x005A5FF0
void __fastcall PARTYSCREEN_RefreshPartyRelationsAfterLeave(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7B460
//1.14d: 0x005A5CB0
void __fastcall PARTYSCREEN_PlayerIterate_RefreshPartyRelations(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg);
//D2Game.0x6FC7B4C0
//1.14d: 0x005A6000
int32_t __fastcall PARTYSCREEN_CommandHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCallbackId, int32_t nOtherPlayerGUID, int32_t nParam);
