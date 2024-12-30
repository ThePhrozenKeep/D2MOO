#include <Unit/Item.h> 
#include <Unit/CUnit.h>
#include <D2StatList.h>
#include <D2States.h>
#include <D2Items.h>
#include <Engine/Cursor.h>
#include <UI/UI.h>

#ifdef D2_VERSION_110F
//1.10f: D2CLient.0x6FB90C30
static int gnItemCursorUIStates;
//1.10f: D2CLient.0x6FB90C28
static int gaItemCursors[] = { 0, 2 };

//1.10f: D2Client.0x6FB2E880
void __fastcall UIHandleCursorItemPacket(D2GSPacketSrv3F* pPacket)
{
	if (pPacket->nSkill == 0xFFFF)
	{
		if (pPacket->nCursor == 0xFF)
		{
			if (D2UnitStrc* pUnit = CUNIT_GetClientUnit(pPacket->dwItemId, UNIT_ITEM))
			{
				ITEMS_SetItemFlag(pUnit, 4u, 0);
				if (D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_UNINTERRUPTABLE))
				{
					STATES_ToggleState(pUnit, STATE_UNINTERRUPTABLE, 0);
					D2Common_10474(pUnit, pStatList);
					STATLIST_FreeStatList(pStatList);
					UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
				}
			}
			CLIENT_SetCursorHeldItem(nullptr);
		}
		else
		{
			for (int i = 0; i < gnItemCursorUIStates; ++i)
			{
				if (pPacket->nCursor == gaItemCursors[i] && !D2CLIENT_CheckUIState_6FB23860(UI_INVENTORY))
					D2CLIENT_UI_Handler_6FB23260(UI_INVENTORY, UI_TURNON, 0);
			}
			if (D2UnitStrc* pItem = CUNIT_GetClientUnit(pPacket->dwItemId, UNIT_ITEM))
			{
				ITEMS_SetItemFlag(pItem, 4u, 1);
				CLIENT_SetCursorUsingItem(D2C_CursorBuySellFrame(pPacket->nCursor), pItem);
			}
		}
	}
	else
	{
		for (int i = 0; i < gnItemCursorUIStates; ++i)
		{
			if (pPacket->nCursor == gaItemCursors[i] && !D2CLIENT_CheckUIState_6FB23860(UI_INVENTORY))
				D2CLIENT_UI_Handler_6FB23260(UI_INVENTORY, UI_TURNON, 0);
		}
		if (D2UnitStrc* pItem = CUNIT_GetClientUnit(pPacket->dwItemId, UNIT_ITEM))
		{
			CLIENT_SetCursorUsingItem(D2C_CursorBuySellFrame(pPacket->nCursor), pItem);
		}
	}
}

#endif //D2_VERSION_110F
