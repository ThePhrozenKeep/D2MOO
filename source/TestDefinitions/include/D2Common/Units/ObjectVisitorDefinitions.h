#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Units/Object.h>


BEGIN_VISIT(D2ObjectRoomCoordStrc)
	POINTER(pRoom)
	FIELD(nX)
	FIELD(nY)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2ObjectRegionStrc)
	//FIELD(nAct)
	//ARRAY(pad0x01)
	//FIELD(field_4)
	//FIELD(nPopulatedRooms)
	//FIELD(unk0x0C)
	//FIELD(nHealingShrines)
	//FIELD(nShrines)
	//FIELD(nWells)
	//FIELD(nTrapMonsterId)
	//ARRAY(wellCoordinates)
	//ARRAY(shrineCoordinates)
END_VISIT()

BEGIN_VISIT(D2ShrineDataStrc)
	//ARRAY(nShrineSubTypes)
	//ARRAY(pShrineSubTypeIds)
END_VISIT()

BEGIN_VISIT(D2ObjectControlStrc)
	//OBJECT(pSeed)
	//OBJECT(shrineData)
	//ARRAY(pObjectRegion)
	//ARRAY(field_1048)
	//POINTER(pObjectRoomCoord)
END_VISIT()

BEGIN_VISIT(D2ObjectDataStrc)
	POINTER(pObjectTxt)
	FIELD(InteractType)
	FIELD(nPortalFlags)
	FIELD(unk0x06)
	POINTER(pShrineTxt)
	FIELD(dwOperateGUID)
	FIELD(bPermanent)
	FIELD(unk0x014)
	OBJECT(DestRoomCooords)
	OBJECT(DestPortalCoords)
	ARRAY(szOwner)
END_VISIT()
