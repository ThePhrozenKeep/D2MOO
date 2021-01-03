#pragma once


#pragma pack(1)

struct D2ObjectRoomCoordStrc
{
	D2RoomStrc* pRoom;						//0x00
	int32_t nX;									//0x04
	int32_t nY;									//0x08
	D2ObjectRoomCoordStrc* pNext;			//0x0C
};

struct D2ObjectRegionStrc
{
	uint32_t dwAct;							//0x00
	uint32_t dwSpawnCount;						//0x04
	uint8_t unk0x08[8];						//0x08
	D2ObjectRoomCoordStrc* pObjectRoomCoord;//0x10
	uint8_t unk0x14[16];						//0x14
};

struct D2ObjectControlStrc
{
	D2SeedStrc pSeed;						//0x00
	int32_t nShrineTypes[5];					//0x08
	void* pShrineList[5];					//0x20
	uint8_t unk0x034[20];						//0x34
	D2ObjectRegionStrc pReg[1024];			//0x48
};

struct D2ObjectDataStrc
{
	D2ObjectsTxt* pObjectTxt;				//0x00
	uint8_t InteractType;					//0x04
	uint8_t nPortalFlags;					//0x05
	uint16_t unk0x06;						//0x06
	D2ShrinesTxt* pShrineTxt;				//0x08
	uint32_t dwOperateGUID;					//0x0C
	BOOL bPermanent;						//0x10
	uint32_t __014;							//0x14
	D2CoordStrc DestRoomCooords;			//0x18
	D2CoordStrc DestPortalCoords;			//0x20
	char szOwner[16];						//0x28
};

#pragma pack()
