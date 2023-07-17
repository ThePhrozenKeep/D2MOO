#pragma once

#include <D2BasicTypes.h>


#include <Fog.h>

#pragma pack(1)

enum FOGCalcTypes
{
	CALCTYPE_None = 0,
	CALCTYPE_CallbackTable = 1,
	CALCTYPE_Callback_Param_Int8 = 4,
	CALCTYPE_Callback_Param_Int16 = 5,
	CALCTYPE_Callback_Param_Int32 = 6,
	CALCTYPE_Raw_Int8 = 7,
	CALCTYPE_Raw_Int16 = 8,
	CALCTYPE_Raw_Int32 = 9,
	CALCTYPE_LessThan = 10,
	CALCTYPE_GreaterThan = 11,
	CALCTYPE_LessOrEqualThan = 12,
	CALCTYPE_GreaterOrEqualThan = 13,
	CALCTYPE_Equal = 14,
	CALCTYPE_NotEqual = 15,
	CALCTYPE_Addition = 16,
	CALCTYPE_Substraction = 17,
	CALCTYPE_Multipliction = 18,
	CALCTYPE_Division = 19,
	CALCTYPE_Power = 20,
	CALCTYPE_Negate = 21,
	CALCTYPE_Ternary = 22,
};

typedef int(__fastcall* CalcFogCallBack_t)(int, int, int, void*);
typedef int(__fastcall* CalcFogCallBack2_t)(int, void*);

struct D2CalcCallbackInfoStrc
{
	CalcFogCallBack_t fpCallBack;            //0x00
	int nParameters;                         //0x04
};

typedef int(__fastcall* CalcGetKeyWordToNumber_t)(char* szKey);
typedef int(__fastcall* CalcGetCalcIndex_t)(int nVal);
typedef int(__fastcall* CalcGetLinkerIndex_t)(char* szText, int* a2, int a3, int nKeywordNumber);
typedef int(__fastcall* CalcProcessCache_t)(int nIndex);

struct D2CalcProcessStrc
{
	char pIndex[64];
	int pLinkerIndex[64];
	int nCurrentIndex;
	int nCurrentLinkerIndex;
};

struct Fog64IntStack
{
  static const int32_t nCapacity = 64;
  
  int32_t tData[nCapacity];
  int32_t nSize;
};

#pragma pack()

// 1.10f: 0x6FF524D0
// 1.13c: 0x6FF69CB0
int __fastcall DATATBLS_IntStackPop(Fog64IntStack* pCalcStack);

// 1.10f: 0x6FF524B0
// 1.13c: 0x6FF69CD0
void __fastcall DATATBLS_IntStackPush(Fog64IntStack* pCalcStack, int32_t nValue);

// 1.10f: 0x6FF51E30 (#10253)
// 1.13c: 0x6FF69E90 (#10253)
FOG_DLL_DECL int __stdcall DATATBLS_CalcEvaluateExpression(const char* pExpressionBuffer, int32_t nExpressionBufferSize, CalcFogCallBack2_t fpParamCallBack, D2CalcCallbackInfoStrc* pTableData, int nTableSize, void* pUserData);

// 1.10f: 0x6FF53280
// 1.13c: 0x6FF69680
char* __fastcall DATATBLS_ExpressionBuffer_PushRawConstant(char* pExpressionBufferPos, char* pExpressionBufferStart, int szBufferSize, D2CalcProcessStrc* pCalc, int32_t nValue);

// 1.10f: Inlined
// 1.13c: 0x6FF695E0
char* __fastcall DATATBLS_ExpressionBuffer_PushCallbackConstant(char* pExpressionBufferPos, char* pExpressionBufferStart, int szBufferSize, D2CalcProcessStrc* pCalc, int32_t nValue);

