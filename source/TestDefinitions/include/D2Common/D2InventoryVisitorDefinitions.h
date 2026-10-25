#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Inventory.h>


BEGIN_VISIT(D2InvRectStrc)
	FIELD(nLeft)
	FIELD(nRight)
	FIELD(nTop)
	FIELD(nBottom)
END_VISIT()

BEGIN_VISIT(D2InventoryGridStrc)
	POINTER(pItem)
	POINTER(pLastItem)
	FIELD(nGridWidth)
	FIELD(nGridHeight)
	FIELD(pad0x0A)
	//POINTER(ppItems)
END_VISIT()

BEGIN_VISIT(D2CorpseStrc)
	FIELD(unk0x00)
	FIELD(dwUnitId)
	FIELD(unk0x08)
	POINTER(pNextCorpse)
END_VISIT()

BEGIN_VISIT(D2InventoryNodeStrc)
	FIELD(nItemId)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2InventoryStrc)
	FIELD(dwSignature)
	OMIT(pMemPool)
	POINTER(pOwner)
	POINTER(pFirstItem)
	POINTER(pLastItem)
	//POINTER(pGrids)
	FIELD(nGridCount)
	FIELD(dwLeftItemGUID)
	POINTER(pCursorItem)
	FIELD(dwOwnerGuid)
	FIELD(dwItemCount)
	POINTER(pFirstNode)
	POINTER(pLastNode)
	POINTER(pFirstCorpse)
	POINTER(pLastCorpse)
	FIELD(nCorpseCount)
END_VISIT()

BEGIN_VISIT(D2ItemExtraDataStrc)
	POINTER(pParentInv)
	POINTER(pPreviousItem)
	POINTER(pNextItem)
	FIELD(nNodePos)
	FIELD(nNodePosOther)
	FIELD(unk0x0E)
	POINTER(pPreviousGridItem)
	POINTER(pNextGridItem)
END_VISIT()
