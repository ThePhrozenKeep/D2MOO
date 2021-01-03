#pragma once 

#include <D2BasicTypes.h>

#pragma pack(1)

enum D2C_OverlayTypes
{
	OVERLAYTYPE_MODECHNG,		//overlay is deleted when unit mode changes
	OVERLAYTYPE_COUNTER,		//param 1 = counter. deletes overlay when counter ends
	OVERLAYTYPE_ONCE,			//overlay is deleted once animation reaches it's end
	OVERLAYTYPE_LOOP,			//loops, durrrrr
	OVERLAYTYPE_LOOPEX,			//dual overlay (param1 + param2), loops
	OVERLAYTYPE_STOP,			//freezes ate last frame. bugged
	OVERLAYTYPE_DELAYED,		//delayed loop, random frame delay
	OVERLAYTYPE_BONEARMOR,		//bone armor hardcode
	OVERLAYTYPE_AURA,			//doesn't replace overlays of this type. param1 = aura state. overlay is deleted when state is unset
	OVERLAYTYPE_AURAEX			//param1 = front overlay, param2 = back overlay, param3 = aura state, set overlay is state is set
};

struct D2OverlayTxt
{
	uint16_t wOverlay;					//0x00
	char szFilename[64];				//0x02
	uint16_t wVersion;					//0x42
	uint32_t dwFrames;					//0x44
	uint8_t nPreDraw;					//0x48
	uint8_t pad0x49[3];					//0x49
	uint32_t dw1ofN;					//0x4C
	uint8_t nDir;						//0x50
	uint8_t nOpen;						//0x51
	uint8_t nBeta;						//0x52
	uint8_t pad0x53;					//0x53
	uint32_t dwOffsetX;					//0x54
	uint32_t dwOffsetY;					//0x58
	uint32_t dwHeight[4];				//0x5C
	uint32_t dwAnimRate;				//0x6C
	uint32_t dwInitRadius;				//0x70
	uint32_t dwRadius;					//0x74
	uint32_t dwLoopWaitTime;			//0x78
	uint8_t nTrans;						//0x7C
	uint8_t nRed;						//0x7D
	uint8_t nGreen;						//0x7E
	uint8_t nBlue;						//0x7F
	uint8_t nNumDirections;				//0x80
	uint8_t nLocalBlood;				//0x81
	uint16_t pad0x82;					//0x82
};

#pragma pack()

//D2Common.0x6FD720A0
void __fastcall DATATBLS_LoadOverlayTxt(void* pMemPool);
//D2Common.0x6FD72500
void __fastcall DATATBLS_UnloadOverlayTxt();
//D2Common.0x6FD72530 (#10674)
uint32_t __stdcall DATATBLS_GetFramesFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72570 (#10675)
int __stdcall DATABLS_GetPreDrawFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD725B0 (#10676)
uint32_t __stdcall DATATBLS_GetOffsetXFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD725F0 (#10677)
uint32_t __stdcall DATATBLS_GetOffsetYFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72630 (#10678)
int __stdcall DATATBLS_GetTransFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72670 (#10679)
int __stdcall DATATBLS_GetRadiusAndColorFromOverlayTxt(int nOverlayId, int* pInitRadius, int* pRadius, uint8_t* pRed, uint8_t* pGreen, uint8_t* pBlue);
//D2Common.0x6FD72720 (#10680)
uint32_t __stdcall DATATBLS_Get1OfNFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72760 (#10681)
int __stdcall DATATBLS_GetDirFromOverlayTxt(int nOverlayId);
//Inlined at various places
D2OverlayTxt* __fastcall DATATBLS_GetOverlayTxtRecord(int nOverlay);
