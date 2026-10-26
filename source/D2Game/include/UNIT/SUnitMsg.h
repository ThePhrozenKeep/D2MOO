#pragma once

#include <Units/Units.h>


//D2Game.0x6FCC5520
void __fastcall SUNITMSG_SendUnitToClient(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCC58E0
void __fastcall D2GAME_STATES_SendUnitStates_6FCC58E0(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCC5BE0
void __fastcall D2GAME_STATES_SendUnitStateUpdates_6FCC5BE0(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCC5F00
void __fastcall D2GAME_STATES_SendStates_6FCC5F00(D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t bPlayer);
//D2Game.0x6FCC5F20
void __fastcall SUNITMSG_SendOverlay(D2UnitStrc* pItem, D2ClientStrc* pClient);
//D2Game.0x6FCC5F80
void __fastcall D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FCC5F80(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCC5FA0
void __fastcall SUNITMSG_SendHoverText(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCC6080
void __fastcall D2GAME_UpdateUnit_6FCC6080(D2UnitStrc* pPlayer, D2ClientStrc* pClient);
//D2Game.0x6FCC60D0
void __fastcall SUNITMSG_QueueItemSkillOnUnit(D2UnitStrc* pUnit, int16_t nSkillId, uint8_t nSkillLevel, uint8_t nUnitType, int32_t nUnitGUID, uint8_t nSkillParam);
//D2Game.0x6FCC6150
void __fastcall SUNITMSG_QueueItemSkillOnPosition(D2UnitStrc* pUnit, int16_t nSkillId, uint8_t nSkillLevel, int16_t nX, int16_t nY, uint8_t nSkillParam);
//D2Game.0x6FCC61D0
void __fastcall D2GAME_MERCS_SendStat_6FCC61D0(D2UnitStrc* pUnit, uint16_t nStatId, int32_t nValue);
//D2Game.0x6FCC6270
void __fastcall SUNITMSG_QueueLifeUpdate(D2UnitStrc* pUnit, uint8_t nLifePercent);
//D2Game.0x6FCC6300
void __fastcall SUNITMSG_QueueSkillEvent(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, int16_t nSkillId, int16_t nSkillLevel, int32_t nX, int32_t nY, uint8_t nSkillParam);
//D2Game.0x6FCC63D0
void __fastcall SUNITMSG_QueueSkillStop(D2UnitStrc* pUnit, int16_t nSkillId);
//D2Game.0x6FCC6470
void __fastcall SUNITMSG_QueueMonsterPrecache(D2UnitStrc* pUnit, int16_t nMonsterClassId);
//D2Game.0x6FCC64D0
void __fastcall SUNITMSG_QueueSkillSelection(D2UnitStrc* pUnit, uint8_t bLeftSkill, int16_t nSkillId, int32_t nOwnerGUID);
//D2Game.0x6FCC6540
void __fastcall SUNITMSG_SendQueuedMessages(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FCC6790
void __fastcall D2GAME_SUNITMSG_FreeUnitMessages_6FCC6790(D2UnitStrc* pUnit);
