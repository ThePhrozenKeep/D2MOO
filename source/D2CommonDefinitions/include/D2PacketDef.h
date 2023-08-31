#pragma once

#include <cstdint>

#pragma pack(1)

// Originally packets used a single byte for stat id, and prevented any stat with id > 255
// Set PACKETS_USE_16BITS_STATID to 1 if you want to support all stat ids.
#ifndef PACKETS_USE_16BITS_STATID
#define PACKETS_USE_16BITS_STATID 0
#endif

#if PACKETS_USE_16BITS_STATID
using PacketStatId = uint16_t;
#else
using PacketStatId = uint8_t;
#endif // PACKETS_USE_16BITS_STATID


//Sub-Structs
struct D2MessageStrc		//sizeof 0x04
{
	uint8_t nMenu;				//0x00
	uint8_t pad0x01;			//0x01
	uint16_t nStringId;			//0x02
};

struct D2MessageListStrc	//sizeof 0x22
{
	uint8_t nCount;			//0x00
	uint8_t pad0x01;			//0x01
	D2MessageStrc pMessages[8];	//0x02
};



struct D2GSPacketClt01		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint16_t nPosX;				//0x01
	uint16_t nPosY;				//0x03
};

struct D2GSPacketClt02		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt03		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint16_t nPosX;				//0x01
	uint16_t nPosY;				//0x03
};

struct D2GSPacketClt04		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt05		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint16_t nPosX;				//0x01
	uint16_t nPosY;				//0x03
};

struct D2GSPacketClt06		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt07		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt08		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint16_t nPosX;				//0x01
	uint16_t nPosY;				//0x03
};

struct D2GSPacketClt09		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt0A		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt0B		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt0C		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint16_t nPosX;				//0x01
	uint16_t nPosY;				//0x03
};

struct D2GSPacketClt0D		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt0E		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt0F		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint16_t nPosX;				//0x01
	uint16_t nPosY;				//0x03
};

struct D2GSPacketClt10		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt11		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt12		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt13		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketClt14		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nMessageType;		//0x01
	uint8_t nLang;				//0x02
	char szMessage[256];	//0x03
};

struct D2GSPacketClt15		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nMessageType;		//0x01
	uint8_t nLang;				//0x02
	//union
	//{
	//	char szMessage[256];//0x03

	//	//struct
	//	//{
	//	//	uint8_t nPlayerId;	//0x03
	//	//	int32_t nItemId;	//0x04
	//	//	char pExtraData[251];
	//	//};
	//};
	char szMessage[256];		//0x03
	char szName[16];			//0x103
};

struct D2GSPacketClt16		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitType;		//0x01
	uint32_t dwUnitGUID;		//0x05
	uint32_t bCursor;			//0x09
};

struct D2GSPacketClt17		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint32_t dwItemGUID;		//0x01
};

struct D2GSPacketClt18		//size of 0x11
{
	uint8_t nHeader;			//0x00
	uint32_t dwItemGUID;		//0x01
	uint32_t dwPosX;			//0x05
	uint32_t dwPosY;			//0x09
	uint32_t dwInvPage;		//0x0D
};

struct D2GSPacketClt19		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint32_t dwItemGUID;		//0x01
};

//TODO: 0x1A - 0x1F

struct D2GSPacketClt20		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	int32_t nItemGUID;			//0x01
	int32_t nPosX;				//0x05
	int32_t nPosY;				//0x09
};

//TODO: 0x21 - 0x22

struct D2GSPacketClt23		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01;			//0x01
	int32_t nBeltSlotId;		//0x05
};

//TODO: 0x24 - 0x25

struct D2GSPacketClt26		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	int32_t nItemGUID;			//0x01
	uint32_t bUseOnMerc;		//0x05
	int32_t unk0x09;			//0x09
};

struct D2GSPacketClt27		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t nTargetItemGUID;	//0x01
	int32_t nUseItemGUID;		//0x05
};

//TODO: 0x28

struct D2GSPacketClt29		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t nScrollGUID;		//0x01
	int32_t nBookGUID;			//0x05
};

struct D2GSPacketClt2A		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwItemGUID;		//0x01
	uint32_t dwCubeGUID;		//0x05
};

//CUSTOM - Unused in Vanilla
struct D2GSPacketClt2B		//size of 0x23
{
	uint8_t nHeader;			//0x00
	int32_t nUnitGUID;			//0x01
	uint8_t nOptionValues[39];	//0x05
};

//Unused: 0x2C - 0x2D

//struct D2GSPacketClt2E	//size of 0x05
//{
//	uint8_t nHeader;		//0x00
//	uint32_t dwType;		//0x01
//};

//TODO: 0x2F - 0x30

struct D2GSPacketClt31		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01[2];			//0x01
};

enum D2TransactionMode : uint32_t {
    TRANSACTIONMODE_BUY = 0,
    TRANSACTIONMODE_GAMBLE = 2,
    TRANSACTIONMODE_FILL = 0x80000000
};

struct D2GSPacketClt32		//size of 0x11
{
	uint8_t nHeader;						//0x00
	int32_t dwNpcGUID;						//0x01
	int32_t dwItemGUID;						//0x05
	D2TransactionMode dwTransactionMode;	//0x09
	int32_t dwCost;							//0x0D
};
static_assert(sizeof(D2GSPacketClt32) == 0x11, "size mismatch");

struct D2GSPacketClt33		//size of 0x11
{
	uint8_t nHeader;			//0x00
	uint32_t dwMonGUID;		//0x01
	uint32_t dwItemGUID;		//0x05
	uint16_t nTab;				//0x09
	uint16_t unk0x0B;			//0x0B
	uint32_t dwGold;			//0x0D
};

//TODO: 0x34 - 0x37

struct D2GSPacketClt38		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint32_t nAction;			//0x01
	int32_t nNpcGUID;			//0x05
	int32_t nItemGUID;			//0x09
};

//TODO: 0x39

struct D2GSPacketClt3A		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint16_t nStat;				//0x01
	int32_t nPoints;			//0x03 ???
};

struct D2GSPacketClt3B		//size of 0x03
{
	uint8_t nHeader;			//0x00
	uint16_t nSkill;			//0x01
};

struct D2GSPacketClt3C		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint16_t nSkill;			//0x01
	uint16_t nMode;				//0x03
	uint32_t dwFlags;			//0x04
};

struct D2GSPacketClt3D		//size of 0x05
{
	uint8_t nHeader;			//0x00
	int32_t nDoorGUID;			//0x01
};

//TODO: 0x3E - 0x40

struct D2GSPacketClt41		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt42		//size of 0x0B
{
	uint8_t nHeader;			//0x00
	char szVersion[10];		//0x01
};

//TODO: 0x43

struct D2GSPacketClt44		//size of 0x11
{
	uint8_t nHeader;			//0x00
	int32_t nPlayerGUID;		//0x01
	int32_t nAnvilInteractGUID;	//0x05
	int32_t nItemGUID;			//0x09
	short nState;			//0x0D
	short unk0x0F;			//0x0F
};

struct D2GSPacketClt45		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t nPortalGUID;		//0x01
	int32_t nLevelId;			//0x05
};

//TODO: 0x46 - 0x48

struct D2GSPacketClt49		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t nWaypointGUID;			//0x01
	int32_t nLevelId;			//0x05
};

//TODO: 0x4A - 0x4E

struct D2GSPacketClt4F		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint16_t nButtonId;			//0x01
	union
	{
		struct
		{
			uint16_t nParams[2];//0x03
		};

		int32_t nItemGUID;		//0x03
	};
};

//TODO: 0x50

struct D2GSPacketClt51
{
	uint8_t nHeader;			//0x00
	int32_t nSkillId;			//0x01
	int32_t nOwnerGUID;			//0x05
};

// TODO: 0x52 - 0x57

struct D2GSPacketClt58		//size of 0x03
{
	uint8_t nHeader;			//0x00
	uint16_t nQuest;			//0x01
};

struct D2GSPacketClt59		//size of 0x11
{
	uint8_t nHeader;			//0x00
	uint32_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x05
	uint32_t dwTargetX;			//0x09
	uint32_t dwTargetY;			//0x0D
};

//TODO: 0x5A - 0x5C

struct D2GSPacketClt5D		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint8_t nType;				//0x01
	uint8_t nToggle;			//0x02
	uint32_t dwPlayerGUID;		//0x03
};

struct D2GSPacketClt5E		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nType;				//0x01
	int32_t dwPlayerGUID;		//0x02
};

struct D2GSPacketClt5F
{
	uint8_t nHeader;			//0x00
	uint16_t nX;				//0x01
	uint16_t nY;				//0x02
};

//TODO: 0x60 - 0x62

struct D2GSPacketClt63		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint32_t dwItemGUID;		//0x01
};

struct D2GSPacketClt64		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01[2];			//0x01
};

struct D2GSPacketClt65		//size of 0x11
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01[4];			//0x01
};

struct D2GSPacketClt66		//size of 0x2E
{
	uint8_t nHeader;			//0x00
	char szGameName[16];		//0x01
	uint8_t nGameType;			//0x11
	uint8_t nPlayerClass;		//0x12
	int8_t nTemplate;			//0x13
	int8_t nDifficulty ;		//0x14
	char szClientName[16];		//0x15
	int16_t unk0x25;			//0x25
	uint32_t nGameFlags;		//0x27
	int8_t unk0x2B;				//0x2B
	int8_t unk0x2C;				//0x2C
	int8_t nLocale;				//0x2D
};

struct D2GSPacketClt67		//size of 0x1D
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01;			//0x01
	uint16_t nGameId;			//0x05
	uint8_t nPlayerClass;		//0x07
	int32_t unk0x08;			//0x08
	uint8_t nLocale;			//0x0C
	char szClientName[16];		//0x0D
};

struct D2GSPacketClt68		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt69		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt6A		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt6B
{
	uint8_t nHeader;			//0x00
	uint8_t nSize;				//0x01
	uint32_t nTotalSize;		//0x02
	uint8_t saveData[8192];		//0x06
	//...
};

struct D2GSPacketClt6C		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01;			//0x01
	int32_t unk0x05;			//0x05
};

struct D2GSPacketClt6D		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketClt6F		//size of 0x01
{
	uint8_t nHeader;			//0x00
};



struct D2GSPacketSrv00		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv01		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t nDifficulty;		//0x01
	uint32_t dwArenaFlags;		//0x02
	uint8_t nExpansion;		//0x06
	uint8_t nLadder;			//0x07
};

struct D2GSPacketSrv02		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv03		//size of 0x0C
{
	uint8_t nHeader;			//0x00
	uint8_t nAct;				//0x01
	uint32_t dwGameSeed;		//0x02
	uint16_t nTownLevelId;		//0x06
	uint32_t nObjectSeed;		//0x08
};

struct D2GSPacketSrv04		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv05		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv06		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv07		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint16_t nX;				//0x01
	uint16_t nY;				//0x03
	uint8_t nLevelId;			//0x05
};

struct D2GSPacketSrv08		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint16_t nX;				//0x01
	uint16_t nY;				//0x03
	uint8_t nLevelId;			//0x04
};

struct D2GSPacketSrv09		//size of 0x0B
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t unk0x06;			//0x06
	uint16_t nPosX;				//0x07
	uint16_t nPosY;				//0x09
	//uint16_t nWarp;				//0x0A
};

struct D2GSPacketSrv0A		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
};

struct D2GSPacketSrv0B		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
};

struct D2GSPacketSrv0C		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t unk0x06;			//0x06
	uint8_t unk0x07;			//0x07
	uint8_t nLife;				//0x08
};

struct D2GSPacketSrv0D		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t unk0x06;			//0x06
	uint16_t unk0x07;			//0x07
	uint16_t unk0x09;			//0x09
	uint8_t unk0x0B;			//0x0B
	uint8_t unk0x0C;			//0x0C
};

struct D2GSPacketSrv0E		//size of 0x0C
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t unk0x06;			//0x06
	uint8_t unk0x07;			//0x07
	uint32_t unk0x08;			//0x08
};

struct D2GSPacketSrv0F		//size of 0x10
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t nFlags;			//0x06
	uint16_t nDestPosX;			//0x07
	uint16_t nDestPosY;			//0x09
	uint8_t __00B;				//0x0B
	uint16_t nCurrentPosX;		//0x0C
	uint16_t nCurrentPosY;		//0x0E
};

struct D2GSPacketSrv10		//size of 0x10
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t nFlags;			//0x06
	uint8_t nTargetType;		//0x07
	uint32_t dwTargetGUID;		//0x08
	uint16_t nCurrentPosX;		//0x0C
	uint16_t nCurrentPosY;		//0x0E
};

struct D2GSPacketSrv11		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint16_t __006;				//0x06
};

struct D2GSPacketSrv12		//size of 0x1A
{
	uint8_t nHeader;			//0x00
	uint8_t __001[25];			//0x01
};

struct D2GSPacketSrv13		//size of 0x0E
{
	uint8_t nHeader;			//0x00
	uint8_t __001[13];			//0x01
};

struct D2GSPacketSrv14		//size of 0x12
{
	uint8_t nHeader;			//0x00
	uint8_t __001[17];			//0x01
};

struct D2GSPacketSrv15		//size of 0x0B
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint16_t nPosX;				//0x06
	uint16_t nPosY;				//0x08
	uint8_t __00A;				//0x0A
};

struct D2ClientUnitUpdateStrc
{
	uint8_t nUnitType;							//0x00
	int32_t nUnitGUID;							//0x01
	uint16_t nX;								//0x05
	uint16_t nY;								//0x07
};

struct D2GSPacketSrv16		//variable size
{
	uint8_t nHeader;			//0x00
	uint16_t nSize;				//0x01
	uint8_t nNumUpdates;		//0x03
	D2ClientUnitUpdateStrc unitUpdate[55];//0x04
};

struct D2GSPacketSrv17		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv18		//size of 0x0F
{
	uint8_t nHeader;			//0x00
	uint8_t __001[14];			//0x01
};

struct D2GSPacketSrv19		//size of 0x02
{
	uint8_t nHeader;			//0x00
	uint8_t nGold;				//0x01
};

struct D2GSPacketSrv1A		//size of 0x02
{
	uint8_t nHeader;			//0x00
	uint8_t nExperience;		//0x01
};

struct D2GSPacketSrv1B		//size of 0x03
{
	uint8_t nHeader;			//0x00
	uint16_t nExperience;		//0x01
};

struct D2GSPacketSrv1C		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint32_t nExperience;		//0x01
};

//struct D2GSPacketSrv1D		//size of 0x03
//{
//	uint8_t nHeader;			//0x00
//	uint8_t nStat;				//0x01
//	uint8_t nValue;			//0x02
//};
//
//struct D2GSPacketSrv1E		//size of 0x04
//{
//	uint8_t nHeader;			//0x00
//	uint8_t nStat;				//0x01
//	uint16_t nValue;			//0x02
//};
//
//struct D2GSPacketSrv1F		//size of 0x06
//{
//	uint8_t nHeader;			//0x00
//	uint8_t nStat;				//0x01
//	int32_t nValue;				//0x02
//};
//
//struct D2GSPacketSrv20	//size of 0x0A
//{
//	uint8_t nHeader;			//0x00
//	uint32_t dwUnitGUID;		//0x01
//	uint8_t nStat;				//0x05
//	int32_t nValue;				//0x06
//};

struct D2GSPacketSrvStat	//size of 0x04 (originally 0x03)
{
	uint8_t nHeader;		//0x00
	PacketStatId nStat;		//0x01 
	uint8_t nValue;			//0x03 (Originally 0x02)
};
using D2GSPacketSrv1D = D2GSPacketSrvStat;
using D2GSPacketSrv1E = D2GSPacketSrvStat;
using D2GSPacketSrv1F = D2GSPacketSrvStat;


struct D2GSPacketSrv20		//size of 0x0B (originally 0x0A)
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitGUID;		//0x01
	PacketStatId nStat;			//0x05
	int32_t nValue;				//0x07 (Originally 0x06)
};

struct D2GSPacketSrv21		//size of 0x0C
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint8_t unk0x02;			//0x02
	uint32_t nUnitGUID;			//0x03
	uint16_t nSkillId;			//0x07
	uint8_t nSkillLevel;		//0x09
	uint8_t nBonusSkillLevel;	//0x0A
	uint8_t unk0x0B;			//0x0B
};

struct D2GSPacketSrv22		//size of 0x0C
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint8_t unk0x02;			//0x02
	int32_t nUnitGUID;			//0x03
	uint16_t nSkillId;			//0x07
	uint8_t nQuantity;			//0x09
	uint8_t unk0x0A;			//0x0A
	uint8_t bIsCorpse;			//0x0B
};

struct D2GSPacketSrv23		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t nPosition;			//0x06
	uint16_t nSkill;			//0x07
	uint32_t __009;			//0x09
};

struct D2GSPacketSrv24		//size of 0x5A
{
	uint8_t nHeader;			//0x00
	uint8_t __001[89];			//0x01
};

struct D2GSPacketSrv25		//size of 0x5A
{
	uint8_t nHeader;			//0x00
	uint8_t __001[89];			//0x01
};

struct D2GSPacketSrv26		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nMessageType;		//0x01
	uint8_t nLang;				//0x02
	uint8_t nUnitType;			//0x03
	uint32_t dwUnitGUID;		//0x04
	uint8_t nMessageColor;		//0x08
	uint8_t nNameColor;		//0x09
	char szName[16];		//0x0A
	char szMessage[256];	//0x1A
};

struct D2GSPacketSrv26Args
{
	D2GSPacketSrv26* pPacket26;		//0x00
	D2GSPacketClt15* pPacket15;		//0x04
	int32_t nUnitId;				//0x08
};

struct D2GSPacketSrv27		//size of 0x28
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	D2MessageListStrc pMessageList;	//0x06
};

struct D2GSPacketSrv28		//size of 0x67
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint32_t unk0x02;			//0x02
	uint8_t unk0x06;			//0x06
	uint8_t pData[96];			//0x07
};

struct D2GSPacketSrv29		//size of 0x61
{
	uint8_t nHeader;			//0x00
	uint8_t pData[96];			//0x01
};

struct D2GSPacketSrv2A		//size of 0x0F
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint8_t unk0x02;			//0x02
	uint32_t unk0x03;			//0x03
	uint32_t unk0x07;			//0x07
	uint32_t unk0x0B;			//0x0B
};

//Unused: 0x2B
struct D2GSPacketSrv2B		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint32_t dwSkill;			//0x01
	uint32_t dwFrame;			//0x05
};

struct D2GSPacketSrv2C		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint16_t nSoundEvent;		//0x06
};

//Unused: 0x2D - 0x3D
//struct D2GSPacketSrv2D		//size of 0x01
//{
//	uint8_t nHeader;					//0x00
//	uint8_t nUnitType;					//0x01
//	uint32_t dwUnitGUID;				//0x02
//	uint32_t dwRecords[3];				//0x06
//	uint32_t dwData[3];				//0x12
//	BOOL bCompleted[3];				//0x1E
//	uint32_t dwCompletionCount[3];		//0x2A
//};
//
//struct D2GSPacketSrv2E		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv2F		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv30		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv31		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv32		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv33		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv34		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv35		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv36		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv37		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv38		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv39		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
struct D2GSPacketSrv3A		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv3B		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

//struct D2GSPacketSrv3C		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv3D		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv3E		//size of 0x22
{
	uint8_t nHeader;			//0x00
	uint8_t nSize;				//0x01
	uint8_t pBuffer[32];		//0x02
};

struct D2GSPacketSrv3F		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t nCursor;			//0x01
	uint32_t dwItemId;			//0x02
	uint16_t nSkill;			//0x06
};

struct D2GSPacketSrv40		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint8_t __001[12];			//0x01
};

//struct D2GSPacketSrv41		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv42		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
};

//struct D2GSPacketSrv43		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv44		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv45		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint8_t __001[12];			//0x01
};

//struct D2GSPacketSrv46		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv47		//size of 0x0B
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint8_t unk0x02;			//0x02
	uint32_t nUnitGUID;			//0x03
	uint32_t unk0x07;			//0x07
};

struct D2GSPacketSrv48		//size of 0x0B
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint8_t unk0x02;			//0x02
	uint32_t nUnitGUID;			//0x03
	uint32_t unk0x07;			//0x07
};

//struct D2GSPacketSrv49		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv4A		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv4B		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv4C		//size of 0x10
{
	uint8_t nHeader;			//0x00
	uint8_t __001[15];			//0x01
};

struct D2GSPacketSrv4D		//size of 0x11
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint32_t unk0x02;			//0x02
	uint32_t unk0x06;			//0x06
	uint8_t unk0x0A;			//0x0A
	uint16_t unk0x0B;			//0x0B
	uint16_t unk0x0D;			//0x0D
	uint16_t unk0x0F;			//0x0F
};

struct D2GSPacketSrv4E		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint16_t nMercName;			//0x01
	uint32_t nSeed;				//0x03
};

struct D2GSPacketSrv4F		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv50		//size of 0x0F
{
	uint8_t nHeader;			//0x00
	uint16_t nQuestId;			//0x01

	union
	{
		struct
		{
			uint16_t nMonstersToBeKilled;	//0x03
			uint16_t nStaffTombLevelOffset;	//0x05
			uint16_t nBarbsToBeRescued;		//0x07
			uint8_t field_9;				//0x09
			uint8_t field_A;				//0x0A
			uint8_t field_B;				//0x0B
			uint16_t field_C;				//0x0C
			uint8_t field_E;				//0x0E
		} QuestsStatusPayload; // Used with QuestId = QUEST_A1Q1_DENOFEVIL(1)

		struct
		{
			uint16_t nHireling;				//0x03
			uint16_t padding0x05[5];		//0x05
		} HirelingPayload; // Used with QuestId = QUEST_A1Q2_BLOODRAVEN(2)

		struct
		{
			uint16_t nStaffTombLevelOffset; //0x03
			uint16_t padding0x05[5];		//0x05
		} TombOffsetPayload; // Used with QuestId = QUEST_A2Q6_DURIEL(13)

		struct
		{
			uint16_t nStoneOrder[5];	//0x03
			uint16_t padding0x0D;		//0x0D
		} A1Q4_CainPayload; // Used with QuestId = 4 (QUEST_A1Q4_CAIN)

		struct 
		{
			uint16_t nZooMonsterId;		//0x03
			uint16_t padding0x05[5];	//0x05
		} A5Q6_BaalPayload; // Used with QuestId = 36 (QUEST_A5Q6_BAAL)
	};
};

struct D2GSPacketSrv51		//size of 0x0E
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint16_t nObject;			//0x06
	uint16_t nPosX;				//0x08
	uint16_t nPosY;				//0x0A
	uint8_t unk0x0C[2];			//0x0C
};

struct D2GSPacketSrv52		//size of 0x2A
{
	uint8_t nHeader;			//0x00
	uint8_t pQuestList[41];	//0x01
};

struct D2GSPacketSrv53		//size of 0x0A
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01;			//0x01
	int32_t unk0x05;			//0x05
	uint8_t unk0x09;			//0x09
};

struct D2GSPacketSrv54		//size of 0x0A
{
	uint8_t nHeader;			//0x00
	uint8_t __001[9];			//0x01
};

struct D2GSPacketSrv55		//size of 0x03
{
	uint8_t nHeader;			//0x00
	uint8_t __001[2];			//0x01
};

//struct D2GSPacketSrv56		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv57		//size of 0x0E
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint8_t unk0x05;			//0x05
	uint16_t unk0x06;			//0x06
	uint16_t unk0x08;			//0x08
	uint16_t unk0x0A;			//0x0A
	uint16_t unk0x0C;			//0x0C
};

struct D2GSPacketSrv58		//size of 0x0E
{
	uint8_t nHeader;			//0x00
	int32_t nUnitId;			//0x01
	//uint8_t unk0x05[9];		//0x05
	uint8_t unk0x05;			//0x05
	uint8_t unk0x06;			//0x06
};

struct D2GSPacketSrv59		//size of 0x1A
{
	uint8_t nHeader;			//0x00
	uint32_t dwGUID;			//0x01
	uint8_t nClass;			//0x05
	char szName[16];		//0x06
	uint16_t nPosX;				//0x16
	uint16_t nPosY;				//0x18
};

struct D2GSPacketSrv5A		//size of 0x28
{
	uint8_t nHeader;			//0x00
	uint8_t nType;				//0x01
	union
	{
		struct //nType < 0x13
		{
			uint8_t nColor;			//0x02
			uint32_t dwParam;			//0x03
			uint8_t nParam;			//0x07
			union
			{
				char szText[32];	//0x08
				char szTextData[2][16];	//0x08
			};	
		};

		struct	//nType 0x13
		{
			uint32_t dwSkill;			//0x02
			uint32_t dwEndFrame;		//0x06
			uint8_t nExtraData2[30];	//0x0A
		};

		struct	//nType 0x14
		{
			uint8_t nGroup;			//0x02
			uint32_t dwGroupEndFrame;	//0x03
			uint8_t nExtraData3[33];	//0x07
		};

		struct //nType 0x16
		{
			int32_t nItemId;			//0x02
			int32_t nPlayerId;			//0x06
			uint8_t nExtraData4[30];	//0x0A
		};

		struct //nType 0x17
		{
			uint8_t nDamageColor;		//0x02
			uint32_t dwDamage;			//0x03
			int32_t nDefenderGUID;		//0x07
			int32_t nDefenderType;		//0x0B
			uint8_t nExtraData5[25];	//0x0F
		};

		struct //nType 0x18
		{
			uint8_t nUnitType;			//0x02
			int32_t nItemGUID;			//0x03
			uint8_t nAction;			//0x07
			int32_t nPosX;				//0x08
			int32_t nPosY;				//0x0C
			uint8_t nExtraData6[24];	//0x10
		};

		struct //nType 0x19
		{
			uint8_t nDefenderUnitType;	//0x02
			int32_t nDefenderUnitGUID;	//0x03
			uint8_t nAttackerUnitType;	//0x07
			int32_t nAttackerUnitGUID;	//0x08
			uint8_t nAttackMessageColor;//0x0C
			uint8_t nExtraData7[27];	//0x0D
		};

		struct //nType 0x1A
		{
			int32_t nFrames;			//0x02
			uint8_t nExtraData8[34];	//0x06
		};

		struct //nType 0x1B
		{
			int32_t nCount;			//0x02
			uint8_t nExtraData9[34];	//0x06
		};
	};
};

struct D2GSPacketSrv5B		//variable size
{
	uint8_t nHeader;			//0x00
	uint16_t nPacketLen;		//0x01
	uint32_t nUnitGUID;			//0x03
	uint8_t nClass;				//0x07
	char szName[16];			//0x08
	uint16_t nLevel;			//0x18
	uint16_t nPartyId;			//0x1A
	uint16_t unk0x1C;			//0x1C
	uint16_t nPartyFlags;		//0x1E
	uint16_t unk0x20;			//0x20
	char unk0x22[256];			//0x22
};

struct D2GSPacketSrv5C		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint32_t dwPlayerGUID;		//0x01
};

struct D2GSPacketSrv5D		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nQuestId;			//0x01
	uint8_t nFlags;			//0x02
	uint8_t nState;			//0x03
	uint16_t field_4;			//0x04
};

struct D2GSPacketSrv5E		//size of 0x26
{
	uint8_t nHeader;			//0x00
	uint8_t __001[37];			//0x01
};

struct D2GSPacketSrv5F		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint8_t __001[4];			//0x01
};

struct D2GSPacketSrv60		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint8_t nFlags;			//0x01
	uint8_t nLevel;			//0x02
	uint32_t dwUnitGUID;		//0x03
};

struct D2GSPacketSrv61		//size of 0x02
{
	uint8_t nHeader;			//0x00
	uint8_t __001;				//0x01
};

struct D2GSPacketSrv62		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	int32_t nUnitId;			//0x02
	uint8_t unk0x06;			//0x06
};

struct D2GSPacketSrv63		//variable size
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitId;			//0x01
	uint16_t pData[8];			//0x05
};

//struct D2GSPacketSrv64		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

////TODO: Other Structure
//struct D2GSPacketSrv65		//size of 0x09
//{
//	uint8_t nHeader;			//0x00
//	uint8_t nUpdateTeamScore;	//0x01
//	uint32_t dwPlayerGUID;		//0x02
//	uint32_t dwScore;			//0x06
//	uint32_t dwTeamScore;		//0x0A
//};

struct D2GSPacketSrv65			//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitId;			//0x01
	uint16_t unk0x05;			//0x05
};

struct D2GSPacketSrv66		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint8_t __001[6];			//0x01
};

struct D2GSPacketSrv67		//size of 0x10
{
	uint8_t nHeader;			//0x00
	int32_t nUnitGUID;			//0x01
	uint8_t unk0x05;			//0x05
	int16_t nX;					//0x06
	int16_t nY;					//0x08
	int8_t unk0x0A;				//0x0A
	int8_t unk0x0B;				//0x0B
	int8_t nPathType;			//0x0C
	int16_t nVelocity;			//0x0D
	int8_t unk0x0F;				//0x0F
};

struct D2GSPacketSrv68		//size of 0x15
{
	uint8_t nHeader;			//0x00
	int32_t nUnitGUID;			//0x01
	uint8_t unk0x05;			//0x05
	int16_t nX;					//0x06
	int16_t nY;					//0x08
	int8_t unk0x0A;				//0x0A
	int32_t unk0x0B;			//0x0B
	int8_t unk0x0F;				//0x0F
	int8_t unk0x10;				//0x10
	int8_t nPathType;			//0x11
	int16_t nVelocity;			//0x12
	int8_t unk0x14;				//0x14
};

struct D2GSPacketSrv69		//size of 0x0C
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint8_t unk0x05;			//0x05
	uint16_t unk0x06;			//0x06
	uint16_t unk0x08;			//0x08
	uint8_t unk0x0A;			//0x0A
	uint8_t unk0x0B;			//0x0B
};

struct D2GSPacketSrv6A		//size of 0x0C
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint8_t unk0x05;			//0x05
	uint8_t unk0x06;			//0x06
	uint32_t unk0x07;			//0x07
	uint8_t unk0x0B;			//0x0B
};

struct D2GSPacketSrv6B		//size of 0x10
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint8_t unk0x05;			//0x05
	uint16_t unk0x06;			//0x06
	uint16_t unk0x08;			//0x08
	uint8_t unk0x0A;			//0x0A
	uint8_t unk0x0B;			//0x0B
	uint16_t unk0x0C;			//0x0C
	uint16_t unk0x0E;			//0x0E
};

struct D2GSPacketSrv6C		//size of 0x10
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint8_t unk0x05;			//0x05
	uint8_t unk0x06;			//0x06
	uint32_t unk0x07;			//0x07
	uint8_t unk0x0B;			//0x0B
	uint16_t unk0x0C;			//0x0C
	uint16_t unk0x0E;			//0x0E
};

struct D2GSPacketSrv6D		//size of 0x0A
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint16_t unk0x05;			//0x05
	uint16_t unk0x07;			//0x07
	uint8_t unk0x09;			//0x09
};

struct D2GSPacketSrv6E		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv6F		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv70		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv71		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv72		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv73		//size of 0x20
{
	uint8_t nHeader;            //0x00
	int32_t unk0x01;            //0x01
	int16_t nMissileId;         //0x05
	int32_t nX;                 //0x07
	int32_t nY;                 //0x0B
	int32_t nFirstX;            //0x0F
	int32_t nFirstY;            //0x13
	int16_t nCurrentFrame;      //0x17
	uint8_t nUnitType;          //0x19
	int32_t nUnitGUID;          //0x1A
	uint8_t nLevel;             //0x1E
	uint8_t nPierce;            //0x1F
};

struct D2GSPacketSrv74		//size of 0x0A
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	int32_t unk0x02;			//0x02
	int32_t unk0x06;			//0x06
};

struct D2GSPacketSrv75		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint32_t nUnitGUID;			//0x01
	uint16_t nPartyId;			//0x05
	uint16_t nLevel;			//0x07
	uint16_t nFlags;			//0x09
	uint16_t unk0x0B;			//0x0B
};

struct D2GSPacketSrv76		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
};

struct D2GSPacketSrv77		//size of 0x02
{
	uint8_t nHeader;			//0x00
	uint8_t nAction;			//0x01
};

struct D2GSPacketSrv78		//size of 0x15
{
	uint8_t nHeader;			//0x00
	char szName[16];		//0x01
	uint32_t dwPlayerGUID;		//0x11
};

struct D2GSPacketSrv79		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nOwnerGUID;		//0x01
	uint32_t dwGold;			//0x02
};

struct D2GSPacketSrv7A		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint8_t unk0x02;			//0x02
	uint16_t unk0x03;			//0x03
	uint32_t unk0x05;			//0x05
	uint32_t unk0x09;			//0x09
};

struct D2GSPacketSrv7B		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint16_t unk0x02;			//0x02
	uint32_t unk0x04;			//0x04
};

struct D2GSPacketSrv7C		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	int32_t nItemGUID;			//0x02
};

struct D2GSPacketSrv7D		//size of 0x12
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t nUnitGUID1;		//0x02
	uint32_t nUnitGUID2;		//0x06
	uint32_t unk0x0A;			//0x0A
	uint32_t unk0x0E;			//0x0E
};

struct D2GSPacketSrv7E		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint8_t __001[4];			//0x01
};

struct D2GSPacketSrv7F		//size of 0x0A
{
	uint8_t nHeader;			//0x00
	uint8_t bIsPlayer;			//0x01
	uint16_t nLifePercentage;	//0x02
	int32_t nUnitGUID;			//0x04
	uint16_t nLevelId;			//0x08
};

//struct D2GSPacketSrv80		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv81		//size of 0x14
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint16_t unk0x02;			//0x02
	uint32_t unk0x04;			//0x04
	uint32_t unk0x08;			//0x08
	uint32_t unk0x0C;			//0x0C
	uint32_t unk0x10;			//0x10
};

struct D2GSPacketSrv82		//size of 0x1D
{
	uint8_t nHeader;			//0x00
	uint32_t dwOwnerGUID;		//0x01
	char szOwnerName[16];	//0x05
	uint32_t dwPortalGUID[2];	//0x15
};

//struct D2GSPacketSrv83		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv84		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv85		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv86		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv87		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};
//
//struct D2GSPacketSrv88		//size of 0x01
//{
//	uint8_t nHeader;			//0x00
//};

struct D2GSPacketSrv89		//size of 0x02
{
	uint8_t nHeader;			//0x00
	uint8_t nEvent;			//0x01
};

struct D2GSPacketSrv8A		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x05
};

struct D2GSPacketSrv8B		//size of 0x06
{
	uint8_t nHeader;			//0x00
	int32_t nUnitGUID;			//0x01
	uint8_t bParty;				//0x05
};

struct D2GSPacketSrv8C		//size of 0x0B
{
	uint8_t nHeader;			//0x00
	uint32_t dwPlayerGUID[2];	//0x01
	uint16_t nFlags;			//0x09
};

struct D2GSPacketSrv8D		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint32_t dwPlayerGUID;		//0x01
	uint16_t nPartyGUID;		//0x05
};

struct D2GSPacketSrv8E		//size of 0x0A
{
	uint8_t nHeader;			//0x00
	uint8_t nType;				//0x01
	uint32_t dwPlayerGUID;		//0x02
	uint32_t dwCorpseGUID;		//0x06
};

struct D2GSPacketSrv8F		//size of 0x21
{
	uint8_t nHeader;			//0x00
	uint8_t __001[32];			//0x01
};

struct D2GSPacketSrv90		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint32_t dwGUID;			//0x01
	uint32_t dwPosX;			//0x05
	uint32_t dwPosY;			//0x09
};

struct D2GSPacketSrv91		//size of 0x1A
{
	uint8_t nHeader;			//0x00
	uint8_t nAct;				//0x01
	uint16_t pNPCFlags[12];		//0x02
};

struct D2GSPacketSrv92		//size of 0x06
{
	uint8_t nHeader;			//0x00
	uint8_t __001[5];			//0x01
};

struct D2GSPacketSrv93		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t __001[7];			//0x01
};

struct D2GSPacketSrv94		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nSkills;			//0x01
	uint32_t dwUnitGUID;		//0x02
	struct SkillInfo			//0x06
	{
		uint16_t nSkill;
		uint8_t nSkillLevel;
	} Skills[255];
};

struct D2GSPacketSrv95		//size of 0x0D
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01[12];		//0x01
	//uint32_t dwLife;			//0x01
	//uint32_t dwMana;			//0x05
	//uint32_t dwStamina;		//0x09
	//uint16_t nPosX;				//0x0D
	//uint16_t nPosY;				//0x0F
	//uint16_t __011;				//0x11
};

struct D2GSPacketSrv96		//size of 0x09
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01[8];			//0x01
};

struct D2GSPacketSrv97		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrv98		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint32_t unk0x01;			//0x01
	uint16_t unk0x05;			//0x05
};

struct D2GSPacketSrv99		//size of 0x10
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint32_t unk0x02;			//0x02
	uint16_t unk0x06;			//0x06
	uint8_t unk0x08;			//0x08
	uint8_t nUnitType;			//0x09
	int32_t nUnitGUID;			//0x0A
	uint16_t unk0x0E;			//0x0E
};

// Same as D2GSPacketSrv4D
struct D2GSPacketSrv9A		//size of 0x11
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint32_t unk0x02;			//0x02
	uint32_t unk0x06;			//0x06
	uint8_t unk0x0A;			//0x0A
	uint16_t nX;				//0x0B
	uint16_t nY;				//0x0D
	uint16_t unk0x0F;			//0x0F
};

struct D2GSPacketSrv9B		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint16_t unk0x01;			//0x01
	uint32_t unk0x03;			//0x03
};

struct D2GSPacketSrv9C		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nAction;			//0x01
	uint8_t nPacketSize;		//0x02
	uint8_t nComponent;		//0x03
	int32_t nItemId;			//0x04
	uint8_t pBitstream[244];	//0x08

							//variable data
};

struct D2GSPacketSrv9D		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nAction;			//0x01
	uint8_t nPacketSize;		//0x02
	uint8_t nComponent;		//0x03
	int32_t nItemId;			//0x04
	uint8_t nUnitType;			//0x08
	int32_t nUnitId;			//0x09
	uint8_t pBitstream[244];	//0x0D

							//variable data
};

//struct D2GSPacketSrv9E	//size of 0x07
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nStat;			//0x01
//	uint32_t dwGUID;		//0x02
//	uint8_t nValue;		//0x06
//};
//
//struct D2GSPacketSrv9F	//size of 0x08
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nStat;			//0x01
//	uint32_t dwGUID;		//0x02
//	uint16_t nValue;		//0x06
//};
//
//struct D2GSPacketSrvA0	//size of 0x0A
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nStat;			//0x01
//	uint32_t dwGUID;		//0x02
//	uint32_t nValue;		//0x06
//};
//
//struct D2GSPacketSrvA1	//size of 0x07
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nStat;			//0x01
//	uint32_t dwGUID;		//0x02
//	uint8_t nValue;		//0x06
//};
//
//struct D2GSPacketSrvA2	//size of 0x08
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nStat;			//0x01
//	uint32_t dwGUID;		//0x02
//	uint16_t nValue;		//0x06
//};


struct D2GSPacketSrvStatAndGuid	//size of 0x08 (originally 0x07)
{
	uint8_t nHeader;			//0x00
	PacketStatId nStat;			//0x01
	uint32_t dwGUID;			//0x03 (Originally 0x02)
	uint8_t nValue;				//0x07 (Originally 0x06)
};

using D2GSPacketSrv9E = D2GSPacketSrvStatAndGuid;
using D2GSPacketSrv9F = D2GSPacketSrvStatAndGuid;
using D2GSPacketSrvA0 = D2GSPacketSrvStatAndGuid;
using D2GSPacketSrvA1 = D2GSPacketSrvStatAndGuid;
using D2GSPacketSrvA2 = D2GSPacketSrvStatAndGuid;

struct D2GSPacketSrvA3		//size of 0x18
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	uint16_t unk0x02;			//0x02
	uint16_t unk0x04;			//0x04
	uint8_t unk0x06;			//0x06
	uint32_t unk0x07;			//0x07
	uint8_t unk0x0B;			//0x0B
	uint32_t unk0x0C;			//0x0C
	uint32_t unk0x10;			//0x10
	uint32_t unk0x14;			//0x14
};

struct D2GSPacketSrvA4		//size of 0x03
{
	uint8_t nHeader;			//0x00
	uint16_t unk0x01;			//0x01
};

struct D2GSPacketSrvA5		//size of 0x08
{
	uint8_t nHeader;			//0x00
	uint8_t unk0x01;			//0x01
	int32_t unk0x02;			//0x02
	uint16_t unk0x06;			//0x06
};

struct D2GSPacketSrvA6		//variable size
{
	uint8_t nHeader;			//0x00

							//variable data
};

struct D2GSPacketSrvA7		//size of 0x08 (originally 0x07)
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	int32_t nUnitId;			//0x02
	uint16_t nState;			//0x06
};

struct D2GSPacketSrvA8		//Variable Size
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	int32_t nUnitId;			//0x02
	uint8_t nSize;				//0x06
	uint16_t nState;			//0x07 - originally a uint8_t
	uint8_t pStream[512];		//0x09
};

//struct D2GSPacketSrvA7	//size of 0x07
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nUnitType;		//0x01
//	int32_t nUnitId;		//0x02
//	uint8_t nState;		//0x06
//};
//
//struct D2GSPacketSrvA8	//Variable Size
//{
//	uint8_t nHeader;			//0x00
//	uint8_t nUnitType;			//0x01
//	int32_t nUnitId;			//0x02
//	uint8_t nSize;				//0x06
//	uint8_t nState;			//0x07
//	uint8_t pStream[512];		//0x08
//};
//
//struct D2GSPacketSrvA9	//size of 0x07
//{
//	uint8_t nHeader;		//0x00
//	uint8_t nUnitType;		//0x01
//	int32_t nUnitId;		//0x02
//	uint8_t nState;		//0x06
//};

struct D2GSPacketSrvA9		//size of 0x08 (originally 0x07)
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	int32_t nUnitId;			//0x02
	uint16_t nState;			//0x06
};

struct D2GSPacketSrvAA		//variable size
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	int32_t nUnitId;			//0x02
	uint8_t nSize;				//0x06
	uint8_t pStream[512];		//0x07
};

struct D2GSPacketSrvAB		//size of 0x07
{
	uint8_t nHeader;			//0x00
	uint8_t nUnitType;			//0x01
	uint32_t dwUnitGUID;		//0x02
	uint8_t nLife;				//0x06
};

struct D2GSPacketSrvAC		//variable size
{
	uint8_t nHeader;			//0x00
	uint32_t dwUnitGUID;		//0x01
	uint16_t nUnitIndex;		//0x05
	uint16_t nPosX;				//0x07
	uint16_t nPosY;				//0x09
	uint8_t nLifePct;			//0x0B
	uint8_t nPacketLength;		//0x0C
	uint8_t bitstream[244];		//0x0D
							//variable data bitstream
};

struct D2GSPacketSrvAD		//size of 0x09
{
	uint8_t nHeader;			//0x00
	int32_t unk0x01;			//0x01
	int32_t unk0x05;			//0x05
};

struct D2GSPacketSrvAE		//variable size
{
	uint8_t nHeader;			//0x00

							//variable data
};

struct D2GSPacketSrvAF		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrvB0		//size of 0x01
{
	uint8_t nHeader;			//0x00
};

struct D2GSPacketSrvB1		//size of 0x35
{
	uint8_t nHeader;			//0x00
	char unk0x01[48];		//0x01
	uint16_t unk0x31;			//0x31
	uint16_t unk0x33;			//0x33
};

struct D2GSPacketSrvB2		//variable size
{
	uint8_t nHeader;			//0x00

							//variable data
};

struct D2GSPacketSrvB3		//size of 0x05
{
	uint8_t nHeader;			//0x00
	uint32_t nErrorCode;		//0x01
};

struct D2GSPacketSrvB4		//variable size
{
	uint8_t nHeader;			//0x00
	uint32_t dwType;			//0x01
};

struct D2GameServerInfoStrc
{
	int32_t field_0x0;
	int32_t field_0x4;
	int32_t field_0x8;
	int32_t field_0xc;
	int32_t field_0x10;
	int32_t field_0x14;
	int32_t field_0x18;
	int32_t field_0x1c;
	int32_t field_0x20;
	int32_t field_0x24;
	char szCompilationData[128];
	int32_t field_0xa8;
	int32_t field_0xac;
	int32_t field_0xb0;
	int32_t field_0xb4;
	int32_t field_0xb8;
};

struct D2GameServerInfoExStrc : D2GameServerInfoStrc
{
	uint32_t dwords0xBC[6];
	uint16_t word0xD4;
	char szUnk0xD6[20];
	uint16_t word0xEA;
	uint32_t dwords0xD8;
	uint32_t dwords0xF0[8];
	uint32_t dwords0x110;
};

struct D2GSPacketSrvFFFD {
	uint8_t nPacketSubType;				//0x00
	D2GameServerInfoStrc tServerInfo;	//0x01
};



struct D2GSPacketSrvFF01 {
	uint8_t nPacketSubType;				//0x00
	D2GameServerInfoStrc tServerInfo;	//0x01
	int32_t field_0xbd;
	int32_t aSomeInts[8];
	int32_t field_0xe1;
	int32_t field_0xe5;
	int32_t field_0xe9;
	int32_t field_0xed;
	int32_t field_0xf1;
	int16_t field_0xf5;
	char field_0xf6[20];
	int32_t field_0x10b;
	int32_t field_0x10f;
	int32_t field_0x113;
	int32_t field_0x117;
	char nZeroed[74];
};


#pragma pack()
