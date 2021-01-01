#pragma once 

#include <D2BasicTypes.h>

struct D2CubeInputItem
{
	uint16_t wInputFlags;				//0x00
	uint16_t wItem;						//0x02
	uint16_t wItemID;					//0x04
	uint8_t nQuality;					//0x06
	uint8_t nQuantity;					//0x07
};

struct D2CubeOutputParamStrc
{
	int32_t nMod;						//0x00
	uint16_t nModParam;					//0x04
	int16_t nModMin;					//0x06
	int16_t nModMax;					//0x08
	uint16_t nModChance;				//0x0A
};

struct D2CubeOutputItem
{
	uint16_t wItemFlags;				//0x00
	uint16_t wBaseItemId;				//0x02
	uint16_t wItemID;					//0x04
	uint8_t nQuality;					//0x06
	uint8_t nQuantity;					//0x07
	uint8_t nType;						//0x08
	uint8_t nLvl;						//0x09
	uint8_t nPLvl;						//0x0A
	uint8_t nILvl;						//0x0B
	uint16_t wPrefixId[3];				//0x0C
	uint16_t wSuffixId[3];				//0x12
	D2CubeOutputParamStrc pParam[5];	//0x18
};

struct D2CubeMainTxt
{
	uint8_t nEnabled;					//0x00
	uint8_t nLadder;					//0x01
	uint8_t nMinDiff;					//0x02
	uint8_t nClass;						//0x03
	uint8_t nOp;						//0x04
	uint8_t pad0x05[3];					//0x05
	int32_t nParam;						//0x08
	int32_t nValue;						//0x0C
	int8_t nNumInputs;					//0x10
	uint8_t pad0x11;					//0x11
	uint16_t wVersion;					//0x12
	D2CubeInputItem pInputItem[7];		//0x14
	D2CubeOutputItem pOutputItem[3];	//0x4C
};


//D2Common.0x6FD523E0
void __fastcall DATATBLS_CubeMainInputLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD52410
BOOL __fastcall DATATBLS_CubeMainInputParser(D2CubeInputItem* pCubeInput, char* szInput, int a3, int a4);
//D2Common.0x6FD528D0
void __fastcall DATATBLS_CubeMainOutputLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD52910
BOOL __fastcall DATATBLS_CubeMainOutputParser(D2CubeOutputItem* pCubeOutputParam, char* szOutput, int nTxtRow, int nItemId);
//D2Common.0x6FD52FC0
void __fastcall DATATBLS_CubeMainParamLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD53030
void __fastcall DATATBLS_LoadCubeMainTxt(void* pMemPool);
//D2Common.0x6FD54250
void __fastcall DATATBLS_UnloadCubeMainTxt();
//D2Common.0x6FD54260 (#11232)
D2CubeMainTxt* __stdcall DATATBLS_GetCubemainTxtRecord(int nIndex);
//D2Common.0x6FD542C0 (#11233)
int __fastcall DATATBLS_GetCubemainTxtRecordCount();

