#include <cstdint>
#include <limits>

#include <Calc.h>

// 1.10f: 0x6FF524D0
// 1.13c: 0x6FF69CB0
int __fastcall DATATBLS_IntStackPop(Fog64IntStack* pCalcStack)
{
	if (pCalcStack->nSize <= 0)
	{
		return 0;
	}
	
	return pCalcStack->tData[--pCalcStack->nSize];
}

// 1.10f: 0x6FF524B0
// 1.13c: 0x6FF69CD0
void __fastcall DATATBLS_IntStackPush(Fog64IntStack* pCalcStack, int32_t nValue)
{
	if (pCalcStack->nSize < pCalcStack->nCapacity)
	{
		pCalcStack->tData[pCalcStack->nSize++] = nValue;
	}
}

// Inlined
template<class T>
T ReadFromBuffer(const char*& pBuffer)
{
	T val = *(T*)pBuffer;
	pBuffer += sizeof(T);
	return val;
}

// Inlined
static void DATATBLS_EvaluateCalcCallback(Fog64IntStack* pCalcStack, const char*& pBufferCurrentPos, D2CalcCallbackInfoStrc* pTableData, int nTableSize, void* pUserData)
{
	int nTableIndex = ReadFromBuffer<uint8_t>(pBufferCurrentPos);
	int nResult = 0;
	if (nTableIndex < nTableSize)
	{
		int nParameters = pTableData[nTableIndex].nParameters;
		int nParam1 = 0;
		int nParam2 = 0;
		int nParam3 = 0;
		switch (nParameters)
		{
		case 3:
			nParam3 = DATATBLS_IntStackPop(pCalcStack);
			// FALLTHROUGH
		case 2:
			nParam2 = DATATBLS_IntStackPop(pCalcStack);
			// FALLTHROUGH
		case 1:
			nParam1 = DATATBLS_IntStackPop(pCalcStack);
			// FALLTHROUGH
		case 0:
			nResult = pTableData[nTableIndex].fpCallBack(nParam1, nParam2, nParam3, pUserData);
		}
	}
	DATATBLS_IntStackPush(pCalcStack, nResult);
}

// Inlined
static void DATATBLS_EvaluateUsingConstantFromBuffer(Fog64IntStack* pCalcStack, FOGCalcTypes nCalcType, const char*& pBufferCurrentPos, CalcFogCallBack2_t fpParamCallBack, void* pUserData)
{
	int nCallbackResult = 0;

	switch (nCalcType)
	{
	case CALCTYPE_Callback_Param_UInt8:
		nCallbackResult = fpParamCallBack(ReadFromBuffer<uint8_t>(pBufferCurrentPos), pUserData);
		break;
	case CALCTYPE_Callback_ParamUInt16:
		nCallbackResult = fpParamCallBack(ReadFromBuffer<uint16_t>(pBufferCurrentPos), pUserData);
		break;
	case CALCTYPE_Callback_Param_UInt32:
		nCallbackResult = fpParamCallBack(ReadFromBuffer<uint32_t>(pBufferCurrentPos), pUserData);
		break;
	case CALCTYPE_Raw_Int8:
		nCallbackResult = ReadFromBuffer<int8_t>(pBufferCurrentPos);
		break;
	case CALCTYPE_Raw_Int16:
		nCallbackResult = ReadFromBuffer<int16_t>(pBufferCurrentPos);
		break;
	case CALCTYPE_Raw_Int32:
		nCallbackResult = ReadFromBuffer<int32_t>(pBufferCurrentPos);
		break;
	default:
		D2_UNREACHABLE;
	}

	DATATBLS_IntStackPush(pCalcStack, nCallbackResult);
}

// Inlined
static void DATATBLS_EvaluateBinaryOperator(Fog64IntStack* pCalcStack, FOGCalcTypes nCalcType)
{
	const int nRightHandSide = DATATBLS_IntStackPop(pCalcStack);
	const int nLeftHandSide = DATATBLS_IntStackPop(pCalcStack);
	int nResult = 0;
	switch (nCalcType)
	{
	case CALCTYPE_LessThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide < nRightHandSide);
		break;
	case CALCTYPE_GreaterThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide > nRightHandSide);
		break;
	case CALCTYPE_LessOrEqualThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide <= nRightHandSide);
		break;
	case CALCTYPE_GreaterOrEqualThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide >= nRightHandSide);
		break;
	case CALCTYPE_Equal:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide == nRightHandSide);
		break;
	case CALCTYPE_NotEqual:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide != nRightHandSide);
		break;
	case CALCTYPE_Addition:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide + nRightHandSide);
		break;
	case CALCTYPE_Substraction:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide - nRightHandSide);
		break;
	case CALCTYPE_Multipliction:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide * nRightHandSide);
		break;
	case CALCTYPE_Division:
		if (nRightHandSide == 0)
		{
			DATATBLS_IntStackPush(pCalcStack, 0);
		}
		else 
		{
			DATATBLS_IntStackPush(pCalcStack, nLeftHandSide / nRightHandSide);
		}
		break;
	case CALCTYPE_Power:
		if (nRightHandSide <= 0)
		{
			DATATBLS_IntStackPush(pCalcStack, 1);
		}
		else
		{
			int nPower = nLeftHandSide;
			int nExponent = nRightHandSide;
			while ((--nExponent) != 0)
			{
				nPower *= nLeftHandSide;
			}
			DATATBLS_IntStackPush(pCalcStack, nPower);
		}
		break;
	default:
		D2_UNREACHABLE;
	}
}


// 1.10f: 0x6FF51E30 (#10253)
// 1.13c: 0x6FF69E90 (#10253)
int __stdcall DATATBLS_CalcEvaluateExpression(const char* pExpressionBuffer, int32_t nExpressionBufferSize, CalcFogCallBack2_t fpParamCallBack, D2CalcCallbackInfoStrc* pTableData, int nTableSize, void* pUserData)
{
	if (nExpressionBufferSize <= 0)
	{
		return 0;
	}

	const char* pBufferCurrentPos = pExpressionBuffer;

	// Note: This actually does not prevent reading more than 1 byte, which means that some of the functions below may read out of bounds...
	//       Not fixing it to keep game behaviour
	auto ReachedEndOfBuffer = [&]()
	{
		return pBufferCurrentPos - pExpressionBuffer >= nExpressionBufferSize;
	};

	Fog64IntStack tCalcStack;
	while (!ReachedEndOfBuffer())
	{
		uint8_t nCalcType = ReadFromBuffer<uint8_t>(pBufferCurrentPos);
		switch (nCalcType)
		{
		case CALCTYPE_CallbackTable:
		{
			DATATBLS_EvaluateCalcCallback(&tCalcStack, pBufferCurrentPos, pTableData, nTableSize, pUserData);
			break;
		}
		case CALCTYPE_Callback_Param_UInt8:
		case CALCTYPE_Callback_ParamUInt16:
		case CALCTYPE_Callback_Param_UInt32:
			if (!fpParamCallBack)
			{
				return 0;
			}
			// FALLTHROUGH
		case CALCTYPE_Raw_Int8:
		case CALCTYPE_Raw_Int16:
		case CALCTYPE_Raw_Int32:
		{
			if (ReachedEndOfBuffer()) // We may read OOB for non 8bit constants...
			{
				return 0;
			}
			DATATBLS_EvaluateUsingConstantFromBuffer(&tCalcStack, (FOGCalcTypes)nCalcType, pBufferCurrentPos, fpParamCallBack, pUserData);
			break;
		}
		case CALCTYPE_LessThan:
		case CALCTYPE_GreaterThan:
		case CALCTYPE_LessOrEqualThan:
		case CALCTYPE_GreaterOrEqualThan:
		case CALCTYPE_Equal:
		case CALCTYPE_NotEqual:
		case CALCTYPE_Addition:
		case CALCTYPE_Substraction:
		case CALCTYPE_Multipliction:
		case CALCTYPE_Division:
		case CALCTYPE_Power:
			DATATBLS_EvaluateBinaryOperator(&tCalcStack, (FOGCalcTypes)nCalcType);
			break;
		case CALCTYPE_Negate:
			DATATBLS_IntStackPush(&tCalcStack, -DATATBLS_IntStackPop(&tCalcStack));
			break;
		case CALCTYPE_Ternary:
		{
			const int nIfFalse = DATATBLS_IntStackPop(&tCalcStack);
			const int nIfTrue = DATATBLS_IntStackPop(&tCalcStack);
			const int nCondition = DATATBLS_IntStackPop(&tCalcStack);
			DATATBLS_IntStackPush(&tCalcStack, nCondition ? nIfTrue : nIfFalse);
		}
		default:
			return DATATBLS_IntStackPop(&tCalcStack);
		}
	}
	
	return 0;
}

// 1.10f: 0x6FF53280
// 1.13c: 0x6FF69680
char* __fastcall DATATBLS_ExpressionBuffer_PushConstant(char* pExpressionBufferPos, char* pExpressionBufferStart, int szBufferSize, D2CalcProcessStrc* pCalc, int32_t nValue)
{
	char* pNewExpressionBufferPos; // eax
		
	if (nValue >= std::numeric_limits<int8_t>::lowest() && nValue <= std::numeric_limits<int8_t>::max())
	{
		const int nPayloadSize = 1 + sizeof(int8_t);
		if (pExpressionBufferPos - pExpressionBufferStart >= szBufferSize) // Can overflow...
		{
			*pExpressionBufferStart = CALCTYPE_None;
			return nullptr;
		}
		pExpressionBufferPos[0] = CALCTYPE_Raw_Int8;
		pExpressionBufferPos[1] = nValue;
		pNewExpressionBufferPos = pExpressionBufferPos + nPayloadSize;
	}
	else if (nValue >= std::numeric_limits<int16_t>::lowest() && nValue <= std::numeric_limits<int16_t>::max())
	{
		const int nPayloadSize = 1 + sizeof(int16_t);
		if (pExpressionBufferPos - pExpressionBufferStart + nPayloadSize >= szBufferSize)
		{
			*pExpressionBufferStart = CALCTYPE_None;
			return nullptr;
		}
		pExpressionBufferPos[0] = CALCTYPE_Raw_Int16;
		*(int16_t*)(pExpressionBufferPos + 1) = nValue;
		pNewExpressionBufferPos = pExpressionBufferPos + nPayloadSize;
	}
	else
	{
		const int nPayloadSize = 1 + sizeof(int32_t);
		if (pExpressionBufferPos - pExpressionBufferStart + nPayloadSize >= szBufferSize)
		{
			*pExpressionBufferStart = CALCTYPE_None;
			return nullptr;
		}
		pExpressionBufferPos[0] = CALCTYPE_Raw_Int32;
		*(int32_t*)(pExpressionBufferPos + 1) = nValue;
		pNewExpressionBufferPos = pExpressionBufferPos + nPayloadSize;
	}
	++pCalc->nCurrentLinkerIndex;
	return pNewExpressionBufferPos;
}




