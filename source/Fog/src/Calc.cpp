#include <cstdint>
#include <limits>

#include <Calc.h>

static_assert(sizeof(FOGASTNodeStrc) == sizeof(uint8_t), "Make sure we use the right size for FOGASTNodeStrc");

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
T ReadFromBuffer(const FOGASTNodeStrc*& pBuffer)
{
	T val = *(T*)pBuffer;
	pBuffer += sizeof(T);
	return val;
}

// Inlined
static void DATATBLS_EvaluateCalcCallback(Fog64IntStack* pCalcStack, const FOGASTNodeStrc*& pBufferCurrentPos, D2CalcCallbackInfoStrc* pTableData, int nTableSize, void* pUserData)
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
static void DATATBLS_EvaluateUsingConstantFromBuffer(Fog64IntStack* pCalcStack, FOGASTType nASTType, const FOGASTNodeStrc*& pBufferCurrentPos, CalcFogCallBack2_t fpParamCallBack, void* pUserData)
{
	int nCallbackResult = 0;

	switch (nASTType)
	{
	case AST_Callback_Param_UInt8:
		nCallbackResult = fpParamCallBack(ReadFromBuffer<uint8_t>(pBufferCurrentPos), pUserData);
		break;
	case AST_Callback_Param_UInt16:
		nCallbackResult = fpParamCallBack(ReadFromBuffer<uint16_t>(pBufferCurrentPos), pUserData);
		break;
	case AST_Callback_Param_UInt32:
		nCallbackResult = fpParamCallBack(ReadFromBuffer<uint32_t>(pBufferCurrentPos), pUserData);
		break;
	case AST_Raw_Int8:
		nCallbackResult = ReadFromBuffer<int8_t>(pBufferCurrentPos);
		break;
	case AST_Raw_Int16:
		nCallbackResult = ReadFromBuffer<int16_t>(pBufferCurrentPos);
		break;
	case AST_Raw_Int32:
		nCallbackResult = ReadFromBuffer<int32_t>(pBufferCurrentPos);
		break;
	default:
		D2_UNREACHABLE;
	}

	DATATBLS_IntStackPush(pCalcStack, nCallbackResult);
}

// Inlined
static void DATATBLS_EvaluateBinaryOperator(Fog64IntStack* pCalcStack, FOGASTType nAST)
{
	const int nRightHandSide = DATATBLS_IntStackPop(pCalcStack);
	const int nLeftHandSide = DATATBLS_IntStackPop(pCalcStack);
	int nResult = 0;
	switch (nAST)
	{
	case AST_LessThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide < nRightHandSide);
		break;
	case AST_GreaterThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide > nRightHandSide);
		break;
	case AST_LessOrEqualThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide <= nRightHandSide);
		break;
	case AST_GreaterOrEqualThan:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide >= nRightHandSide);
		break;
	case AST_Equal:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide == nRightHandSide);
		break;
	case AST_NotEqual:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide != nRightHandSide);
		break;
	case AST_Addition:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide + nRightHandSide);
		break;
	case AST_Substraction:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide - nRightHandSide);
		break;
	case AST_Multipliction:
		DATATBLS_IntStackPush(pCalcStack, nLeftHandSide * nRightHandSide);
		break;
	case AST_Division:
		if (nRightHandSide == 0)
		{
			DATATBLS_IntStackPush(pCalcStack, 0);
		}
		else 
		{
			DATATBLS_IntStackPush(pCalcStack, nLeftHandSide / nRightHandSide);
		}
		break;
	case AST_Power:
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
int __stdcall DATATBLS_CalcEvaluateExpression(const FOGASTNodeStrc* pExpressionBuffer, int32_t nExpressionBufferSize, CalcFogCallBack2_t fpParamCallBack, D2CalcCallbackInfoStrc* pTableData, int nTableSize, void* pUserData)
{
	if (nExpressionBufferSize <= 0)
	{
		return 0;
	}

	const FOGASTNodeStrc* pBufferCurrentPos = pExpressionBuffer;

	// Note: This actually does not prevent reading more than 1 byte, which means that some of the functions below may read out of bounds...
	//       Not fixing it to keep game behaviour
	auto ReachedEndOfBuffer = [&]()
	{
		return pBufferCurrentPos - pExpressionBuffer >= nExpressionBufferSize;
	};

	Fog64IntStack tCalcStack;
	tCalcStack.nSize = 0;

	while (!ReachedEndOfBuffer())
	{
		uint8_t nAST = ReadFromBuffer<uint8_t>(pBufferCurrentPos);
		switch (nAST)
		{
		case AST_CallbackTable:
		{
			DATATBLS_EvaluateCalcCallback(&tCalcStack, pBufferCurrentPos, pTableData, nTableSize, pUserData);
			break;
		}
		case AST_Callback_Param_UInt8:
		case AST_Callback_Param_UInt16:
		case AST_Callback_Param_UInt32:
			if (!fpParamCallBack)
			{
				return 0;
			}
			// FALLTHROUGH
		case AST_Raw_Int8:
		case AST_Raw_Int16:
		case AST_Raw_Int32:
		{
			if (ReachedEndOfBuffer()) // We may read OOB for non 8bit constants...
			{
				return 0;
			}
			DATATBLS_EvaluateUsingConstantFromBuffer(&tCalcStack, (FOGASTType)nAST, pBufferCurrentPos, fpParamCallBack, pUserData);
			break;
		}
		case AST_LessThan:
		case AST_GreaterThan:
		case AST_LessOrEqualThan:
		case AST_GreaterOrEqualThan:
		case AST_Equal:
		case AST_NotEqual:
		case AST_Addition:
		case AST_Substraction:
		case AST_Multipliction:
		case AST_Division:
		case AST_Power:
			DATATBLS_EvaluateBinaryOperator(&tCalcStack, (FOGASTType)nAST);
			break;
		case AST_Negate:
			DATATBLS_IntStackPush(&tCalcStack, -DATATBLS_IntStackPop(&tCalcStack));
			break;
		case AST_Ternary:
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

// Helper function
static FOGASTNodeStrc* __fastcall DATATBLS_ExpressionBuffer_PushConstant(FOGASTNodeStrc* pExpressionBufferPos, FOGASTNodeStrc* pExpressionBufferStart, int szBufferSize, FOGExpressionParserContextStrc* pCalc, int32_t nValue, bool bCallback)
{
	FOGASTNodeStrc* pNewExpressionBufferPos;

	if (std::numeric_limits<int8_t>::lowest() <= nValue && nValue <= std::numeric_limits<int8_t>::max())
	{
		const int nPayloadSize = 1 + sizeof(int8_t);
		if (pExpressionBufferPos - pExpressionBufferStart >= szBufferSize) // Can overflow...
		{
			pExpressionBufferStart->nType = AST_None;
			return nullptr;
		}
		pExpressionBufferPos[0].nType = bCallback ? AST_Callback_Param_UInt8 : AST_Raw_Int8;
		pExpressionBufferPos[1].nRawValue = nValue;
		pNewExpressionBufferPos = pExpressionBufferPos + nPayloadSize;
	}
	else if (std::numeric_limits<int16_t>::lowest() <= nValue && nValue <= std::numeric_limits<int16_t>::max())
	{
		const int nPayloadSize = 1 + sizeof(int16_t);
		if (pExpressionBufferPos - pExpressionBufferStart + nPayloadSize >= szBufferSize)
		{
			pExpressionBufferStart->nType = AST_None;
			return nullptr;
		}
		pExpressionBufferPos[0].nType = bCallback ? AST_Callback_Param_UInt16 : AST_Raw_Int16;
		*(int16_t*)(pExpressionBufferPos + 1) = nValue;
		pNewExpressionBufferPos = pExpressionBufferPos + nPayloadSize;
	}
	else
	{
		const int nPayloadSize = 1 + sizeof(int32_t);
		if (pExpressionBufferPos - pExpressionBufferStart + nPayloadSize >= szBufferSize)
		{
			pExpressionBufferStart->nType = AST_None;
			return nullptr;
		}
		pExpressionBufferPos[0].nType = bCallback ? AST_Callback_Param_UInt32 : AST_Raw_Int32;
		*(int32_t*)(pExpressionBufferPos + 1) = nValue;
		pNewExpressionBufferPos = pExpressionBufferPos + nPayloadSize;
	}
	++pCalc->nPendingParameters;
	return pNewExpressionBufferPos;
}

// 1.10f: 0x6FF53280
// 1.13c: 0x6FF69680
FOGASTNodeStrc* __fastcall DATATBLS_ExpressionBuffer_PushRawConstant(FOGASTNodeStrc* pExpressionBufferPos, FOGASTNodeStrc* pExpressionBufferStart, int szBufferSize, FOGExpressionParserContextStrc* pCalc, int32_t nValue)
{
	return DATATBLS_ExpressionBuffer_PushConstant(pExpressionBufferPos, pExpressionBufferStart, szBufferSize, pCalc, nValue, false);
}

// 1.10f: Inlined
// 1.13c: 0x6FF695E0
FOGASTNodeStrc* __fastcall DATATBLS_ExpressionBuffer_PushCallbackConstant(FOGASTNodeStrc* pExpressionBufferPos, FOGASTNodeStrc* pExpressionBufferStart, int szBufferSize, FOGExpressionParserContextStrc* pCalc, int32_t nValue)
{
	return DATATBLS_ExpressionBuffer_PushConstant(pExpressionBufferPos, pExpressionBufferStart, szBufferSize, pCalc, nValue, true);
}

// Helper function
static int GetASTNodeParameterCount(FOGASTType nASTType, int nTokenAssociatedValue, CalcGetFunctionParameterCount_t pfnGetFunctionParameterCount)
{
	switch (nASTType)
	{
	case AST_CallbackTable:
		if (pfnGetFunctionParameterCount)
		{
			return pfnGetFunctionParameterCount(nTokenAssociatedValue);
		}
		else
		{
			return 2;
		}
	case AST_LessThan:
	case AST_GreaterThan:
	case AST_LessOrEqualThan:
	case AST_GreaterOrEqualThan:
	case AST_Equal:
	case AST_NotEqual:
	case AST_Addition:
	case AST_Substraction:
	case AST_Multipliction:
	case AST_Division:
	case AST_Power:
		return 2;
	case AST_Negate:
		return 1;
	case AST_Ternary:
		return 3;
		break;
	default:
		return 0;
	}
}

static const char nCacheExceptionsList[] = { 
	23,
	1,
	2,
	3,
	9,9,9,9,9,9,15,15,15,15,15,15,17,17,19,19,20,21,22 };

// 1.10f: 0x6FF712F0
static const FOGASTType aASTOperatorPrecedence[24] = {
	AST_Ternary_Colon,				  // AST_None = 0
	AST_CallbackTable,				  // AST_CallbackTable = 1
	AST_Parenthesis_Open,			  // AST_Parenthesis_Open = 2
	AST_Callback_Param_Separator,	  // AST_Callback_Param_Separator = 3
	AST_Raw_Int32,					  // AST_Callback_Param_UInt8 = 4
	AST_Raw_Int32,					  // AST_Callback_Param_UInt16 = 5
	AST_Raw_Int32,					  // AST_Callback_Param_UInt32 = 6
	AST_Raw_Int32,					  // AST_Raw_Int8 = 7
	AST_Raw_Int32,					  // AST_Raw_Int16 = 8
	AST_Raw_Int32,					  // AST_Raw_Int32 = 9
	AST_NotEqual, 					  // AST_LessThan = 10
	AST_NotEqual, 					  // AST_GreaterThan = 11
	AST_NotEqual, 					  // AST_LessOrEqualThan = 12
	AST_NotEqual, 					  // AST_GreaterOrEqualThan = 13
	AST_NotEqual, 					  // AST_Equal = 14
	AST_NotEqual,					  // AST_NotEqual = 15
	AST_Substraction,				  // AST_Addition = 16
	AST_Substraction,				  // AST_Substraction = 17
	AST_Division,					  // AST_Multipliction = 18
	AST_Division,					  // AST_Division = 19
	AST_Power,						  // AST_Power = 20
	AST_Negate,						  // AST_Negate = 21
	AST_Ternary,					  // AST_Ternary = 22
	AST_None						  // AST_Ternary_Colon = 23
};

// 1.10f: 0x6FF53000
// 1.13c: 0x6FF69790
BOOL __fastcall DATATABLS_CheckPrecendence(FOGASTType nPreviousCalcType, FOGASTType nNewCalcType)
{
	if (nPreviousCalcType == AST_CallbackTable || nNewCalcType == AST_CallbackTable || nPreviousCalcType >= AST_Ternary_Colon)
	{
		return FALSE;
	}
	else
	{
		return aASTOperatorPrecedence[nPreviousCalcType] >= nNewCalcType;
	}
}

// 1.10f: 0x6FF530B0
// 1.13c: 0x6FF69CF0
FOGASTNodeStrc* __fastcall DATATBLS_Evaluate_HandleNewOp(FOGASTNodeStrc* pASTBufferPos, FOGASTNodeStrc* pASTBufferStart, int nASTBufferSize, FOGExpressionParserContextStrc* pContext, FOGASTType nNewASTType, CalcGetFunctionParameterCount_t pfnGetFunctionParametersCount, int nTokenAssociatedValue)
{
	while (pContext->nPendingOps > 0)
	{
		const FOGASTType nPreviousASTType = pContext->tPendingOperationsASTs[pContext->nPendingOps - 1];
		if (!DATATABLS_CheckPrecendence(nPreviousASTType, nNewASTType))
		{
			break;
		}
		const int nParams = GetASTNodeParameterCount(nPreviousASTType, nTokenAssociatedValue, pfnGetFunctionParametersCount);
		if (pContext->nPendingParameters < nParams || (pASTBufferPos - pASTBufferStart) >= nASTBufferSize)
		{
			pASTBufferStart->nRawValue = 0;
			return nullptr;
		}
		pASTBufferPos->nType = nPreviousASTType;
		pASTBufferPos++;
		pContext->nPendingParameters = pContext->nPendingParameters + 1 - nParams;
		pContext->nPendingOps--;
	}

	if (nNewASTType == AST_Ternary_Colon || nNewASTType == AST_Callback_Param_Separator)
	{
		return pASTBufferPos;
	}

	if (nNewASTType == AST_None)
	{
		if (pASTBufferPos == pASTBufferStart)
		{
			return nullptr;
		}
		if ((pASTBufferPos - pASTBufferStart) >= nASTBufferSize)
		{
			pASTBufferStart->nRawValue = 0;
			return nullptr;
		}

		pASTBufferPos->nType = AST_None;
		pASTBufferPos++;
		return pASTBufferPos;
	}
	else
	{
		if (pContext->nPendingOps >= 64)
		{
			pASTBufferStart->nRawValue = 0;
			return nullptr;
		}
		pContext->tPendingOperationsASTs[pContext->nPendingOps] = nNewASTType;
		pContext->tLinkerIndex[pContext->nPendingOps] = nTokenAssociatedValue;
		pContext->nPendingOps++;
		return pASTBufferPos;
	}
}

// Helper function
static void DATATBLS_ResolveConstantLink(char* szLinkName, FOGExpressionParserContextStrc* pCalc, FOGCalcExpressionParserTokenType* pOutTokenType, int* pTokenAssociatedValue, CalcGetLinkerIndex_t pfnLinkParse)
{
	BOOL pOutIsFunctionParameter = FALSE;
	const int32_t nLinkerIndex = pfnLinkParse(
		szLinkName,
		&pOutIsFunctionParameter,
		pCalc->tPendingOperationsASTs[pCalc->nPendingOps - 1],
		pCalc->tLinkerIndex[pCalc->nPendingOps - 1]
	);
	if (nLinkerIndex >= 0)
	{
		*pOutTokenType = pOutIsFunctionParameter ? TOKEN_CUSTOM_FUNCTION_PARAMETER_CONSTANT : TOKEN_CUSTOM_RAW_CONSTANT;
		*pTokenAssociatedValue = nLinkerIndex;
	}
	else // Not found
	{
		*pOutTokenType = TOKEN_CUSTOM_RAW_CONSTANT;
		*pTokenAssociatedValue = 0;
	}
}

// Helper function
static const char* DATATBLS_ParseLinkToken(const char* szExpression, FOGExpressionParserContextStrc* pCalc, FOGCalcExpressionParserTokenType* pOutTokenType, int* pTokenAssociatedValue, CalcGetLinkerIndex_t pfnLinkParse)
{
	D2_ASSERT(*szExpression == '\'');
	const char* pLinkNameCurCharacter = szExpression + 1; // Skip '\''
	char tLinkNameBuffer[256];
	int nLinkNameSize = 0;
	while (*pLinkNameCurCharacter != 0 && *pLinkNameCurCharacter != '\'')
	{
		if (nLinkNameSize < 255)
		{
			tLinkNameBuffer[nLinkNameSize++] = *pLinkNameCurCharacter;
		}
		pLinkNameCurCharacter++;
	}
	if (*pLinkNameCurCharacter != 0) // we stopped because we reached an '
		pLinkNameCurCharacter++;

	tLinkNameBuffer[nLinkNameSize] = 0;
	
	DATATBLS_ResolveConstantLink(tLinkNameBuffer, pCalc, pOutTokenType, pTokenAssociatedValue, pfnLinkParse);

	return pLinkNameCurCharacter;
}

// Helper function
static const char* DATATBLS_ParseSublinkToken(const char* szExpression, FOGExpressionParserContextStrc* pCalc, FOGCalcExpressionParserTokenType* pOutTokenType, int* pTokenAssociatedValue, CalcGetLinkerIndex_t pfnLinkParse)
{
	D2_ASSERT(*szExpression == '.');
	const char* pLinkNameCurrentCharacter = szExpression + 1; // Skip '.'
	char tLinkNameBuffer[256];
	int nLinkNameSize = 0;
	while (isalnum(*pLinkNameCurrentCharacter))
	{
		if (nLinkNameSize < 255)
		{
			tLinkNameBuffer[nLinkNameSize++] = *pLinkNameCurrentCharacter;
		}
		pLinkNameCurrentCharacter++;
	}
	tLinkNameBuffer[nLinkNameSize] = 0;

	BOOL pOutIsFunctionParameter = FALSE;
	const int32_t nLinkerIndex = pfnLinkParse(
		tLinkNameBuffer,
		&pOutIsFunctionParameter,
		pCalc->tPendingOperationsASTs[pCalc->nPendingOps - 1],
		pCalc->tLinkerIndex[pCalc->nPendingOps - 1]
	);
	if (nLinkerIndex >= 0)
	{
		*pOutTokenType = TOKEN_CUSTOM_RAW_CONSTANT;
		*pTokenAssociatedValue = nLinkerIndex;
		return pLinkNameCurrentCharacter;
	}
	else
	{
		return nullptr;
	}
}

// Helper function
static const char* DATATABLS_ParseSubExpression(const char* szExpression, FOGExpressionParserContextStrc* pCalc, FOGCalcExpressionParserTokenType* pOutTokenType, int* pTokenAssociatedValue, CalcGetKeyWordToNumber_t pfnFunctionNameToId, CalcGetLinkerIndex_t pfnLinkParse)
{
	// Read token
	int nSubExpressionSize = 0;
	char tSubExpressionBuffer[256];
	while (isalnum(*szExpression))
	{
		if (nSubExpressionSize < 255)
		{
			tSubExpressionBuffer[nSubExpressionSize++] = *szExpression;
		}
		szExpression++;
	}
	tSubExpressionBuffer[nSubExpressionSize] = 0;

	// Skip spaces
	while (isspace(*szExpression))
	{
		szExpression++;
	}

	// Function call ?
	if (*szExpression == '(')
	{
		if (const int nFunctionId = pfnFunctionNameToId(tSubExpressionBuffer))
		{
			*pOutTokenType = TOKEN_CUSTOM_FUNCTION_TABLE;
			*pTokenAssociatedValue = nFunctionId;
			return szExpression + 1;
		}
	}
	
	// If we couldn't resolve a function, try constants
	DATATBLS_ResolveConstantLink(tSubExpressionBuffer, pCalc, pOutTokenType, pTokenAssociatedValue, pfnLinkParse);
	return szExpression;
}

// 1.10f: Inlined
// 1.13c: 0x6FF697B0
// Returns the position after the parsed token
const char* DATATABLS_ParseExpressionToken(const char* szExpression, FOGCalcExpressionParserTokenType* pOutTokenType, FOGExpressionParserContextStrc* pCalc, int* pTokenAssociatedValue, CalcGetKeyWordToNumber_t pfnFunctionNameToId, CalcGetLinkerIndex_t pfnLinkParse)
{
	*pTokenAssociatedValue = 0;
	// Advance until next token
	while (*szExpression != 0 && !isspace(*szExpression) && *szExpression != '"')
	{
		szExpression++;
	}

	if (isdigit(*szExpression))
	{
		*pOutTokenType = TOKEN_CUSTOM_RAW_CONSTANT;
		*pTokenAssociatedValue = *szExpression - '0';
		while (isdigit(*(++szExpression))) {
			*pTokenAssociatedValue = *pTokenAssociatedValue * 10 + (*szExpression - '0');
		}
		return szExpression;
	}
	else if(*szExpression == '\'')
	{
		return DATATBLS_ParseLinkToken(szExpression, pCalc, pOutTokenType, pTokenAssociatedValue, pfnLinkParse);
	}
	else if (isalpha(*szExpression))
	{
		return DATATABLS_ParseSubExpression(szExpression, pCalc, pOutTokenType, pTokenAssociatedValue, pfnFunctionNameToId, pfnLinkParse);
	}
	else
	{
		switch (*szExpression)
		{
		case '!':
			szExpression++;
			if (*szExpression != '=')
			{
				return nullptr;
			}
			*pOutTokenType = TOKEN_LOGIC_DIFFERENT;
			break;
		case '(':
			*pOutTokenType = TOKEN_PAREN_OPEN;
			break;
		case ')':
			*pOutTokenType = TOKEN_PAREN_CLOSE;
			break;
		case '*':
			*pOutTokenType = TOKEN_ASTERISK;
			break;
		case '+':
			*pOutTokenType = TOKEN_PLUS;
			break;
		case ',':
			*pOutTokenType = TOKEN_COMMA;
			break;
		case '-':
			*pOutTokenType = TOKEN_MINUS;
			break;
		case '.':
			return DATATBLS_ParseSublinkToken(szExpression, pCalc, pOutTokenType, pTokenAssociatedValue, pfnLinkParse);
		case '/':
			*pOutTokenType = TOKEN_SLASH;
			break;
		case ':':
			*pOutTokenType = TOKEN_COLON;
			break;
		case '<':
			if (*(szExpression + 1) == '=')
			{
				szExpression++;
				*pOutTokenType = TOKEN_LESS_OR_EQ;
			}
			else
			{
				*pOutTokenType = TOKEN_LESS;
			}
			break;
		case '=':
			szExpression++;
			if (*szExpression != '=')
			{
				return nullptr;
			}
			*pOutTokenType = TOKEN_LOGIC_EQUALS;
			break;
		case '>':
			if (*(szExpression + 1) == '=')
			{
				szExpression++;	
				*pOutTokenType = TOKEN_GREATER_OR_EQ;
			}
			else
			{
				*pOutTokenType = TOKEN_GREATER;
			}
			break;
		case '?':
			*pOutTokenType = TOKEN_QUESTION_MARK;
			break;
		case '^':
			*pOutTokenType = TOKEN_CIRCUM;
			break;
		default:
			return nullptr;
		}

		return szExpression + 1;
	}


}

// 1.10f: 0x6FF524F0 (#10254)
// 1.13c: 0x6FF5BB20 (#10254)
int __stdcall DATATBLS_CompileExpression(const char* szFormulaString, FOGASTNodeStrc* pOutASTBuffer, int nOutASTBufferSize, CalcGetKeyWordToNumber_t pfnFunctionNameToId, CalcGetFunctionParameterCount_t pfnGetFunctionParameterCount, CalcGetLinkerIndex_t pfnLinkParse)
{
	bool bPendingValue = false;
	bool bHasAnyFunctionCall = false;

	const char* szRemainingExpression = szFormulaString;
	FOGASTNodeStrc* pASTBufferPos = pOutASTBuffer;
	FOGCalcExpressionParserTokenType nToken = TOKEN_PAREN_OPEN;
	int32_t nTokenAssociatedValue = 0;
	FOGExpressionParserContextStrc tContext;
	tContext.nPendingOps = 0;
	tContext.nPendingParameters = 0;
	while (
		nToken = TOKEN_PAREN_OPEN,
		szRemainingExpression = DATATABLS_ParseExpressionToken(szRemainingExpression, &nToken, &tContext, &nTokenAssociatedValue, pfnFunctionNameToId, pfnLinkParse)
		)
	{
		switch (nToken)
		{
		case TOKEN_PAREN_OPEN:
			if (tContext.nPendingOps >= 64)
			{
				return 0;
			}
			tContext.tPendingOperationsASTs[tContext.nPendingOps++] = AST_Parenthesis_Open;
			if (pASTBufferPos == nullptr)
			{
				return 0;
			}
			break;
		case TOKEN_PAREN_CLOSE:
			if (tContext.nPendingOps <= 0)
			{
				return 0;
			}
			while (1)
			{
				const FOGASTType nPreviousASTType = tContext.tPendingOperationsASTs[tContext.nPendingOps - 1];
				if (nPreviousASTType == AST_Parenthesis_Open) // Until we reach the previous parenthesis, evaluate params
				{
					break;
				}
				const int32_t nLinkerIndex = tContext.tLinkerIndex[tContext.nPendingOps - 1];
				const int32_t nParams = GetASTNodeParameterCount(nPreviousASTType, nLinkerIndex, pfnGetFunctionParameterCount);

				if (tContext.nPendingParameters < nParams || (pASTBufferPos - pOutASTBuffer) >= nOutASTBufferSize)
				{
					return 0; // error, not enough values or AST buffer full
				}
				pASTBufferPos->nType = nPreviousASTType;
				pASTBufferPos++;

				tContext.nPendingParameters = tContext.nPendingParameters + 1 - nParams;
				if (nPreviousASTType == AST_CallbackTable)
				{
					if ((pASTBufferPos - pOutASTBuffer) >= nOutASTBufferSize)
					{
						return 0; // AST buffer full
					}
					pASTBufferPos->nRawValue = nLinkerIndex;
					pASTBufferPos++;
					break;
				}
				if ((--tContext.nPendingOps) <= 0)
				{
					return 0; // Should never happen
				}
			}
			continue; // Handle next token
		case TOKEN_COMMA:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Callback_Param_Separator, 0, 0
			);
			break;
		case TOKEN_PLUS:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Addition, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_MINUS:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				bPendingValue ? AST_Substraction : AST_Negate, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_ASTERISK:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Multipliction, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_SLASH:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Division, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_QUESTION_MARK:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Ternary, 0, 0
			);
			break;
		case TOKEN_COLON:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Ternary_Colon, 0, 0
			);
			break;
		case TOKEN_CIRCUM:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Power, 0, 0
			);
			break;
		case TOKEN_LESS_OR_EQ:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_LessOrEqualThan, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_LESS:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_LessThan, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_GREATER_OR_EQ:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_GreaterOrEqualThan, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_GREATER:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_GreaterThan, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_LOGIC_EQUALS:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_Equal, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_LOGIC_DIFFERENT:
			pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
				AST_NotEqual, 0, 0
			);
			bPendingValue = false;
			break;
		case TOKEN_CUSTOM_RAW_CONSTANT:
			pASTBufferPos = DATATBLS_ExpressionBuffer_PushRawConstant(
				pASTBufferPos,
				pOutASTBuffer,
				nOutASTBufferSize,
				&tContext,
				nTokenAssociatedValue
			);
			bPendingValue = true;
			break;
		case TOKEN_CUSTOM_FUNCTION_PARAMETER_CONSTANT:
			pASTBufferPos = DATATBLS_ExpressionBuffer_PushCallbackConstant(
				pASTBufferPos,
				pOutASTBuffer,
				nOutASTBufferSize,
				&tContext,
				nTokenAssociatedValue
			);
			bPendingValue = true;
			bHasAnyFunctionCall = true;
			break;
		case TOKEN_CUSTOM_FUNCTION_TABLE:
			if (tContext.nPendingOps < 64)
			{
				tContext.tPendingOperationsASTs[tContext.nPendingOps] = AST_CallbackTable;
				tContext.tLinkerIndex[tContext.nPendingOps] = nTokenAssociatedValue;
				tContext.nPendingOps++;
			}
			else
			{
				pOutASTBuffer->nRawValue = 0;
				pASTBufferPos = 0;
			}
			bPendingValue = TRUE;
			bHasAnyFunctionCall = TRUE;
			break;
		default:
			break;
		}

		if (pASTBufferPos == nullptr)
		{
			return 0;
		}
	}



	pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext,
		AST_None, 0, 0
	);
	if (!pASTBufferPos)
	{
		pOutASTBuffer->nRawValue = 0;
		return 0;
	}
	if (!bHasAnyFunctionCall)
	{
		const int32_t nEvaluatedValue = DATATBLS_CalcEvaluateExpression(pOutASTBuffer, pASTBufferPos - pOutASTBuffer, 0, 0, 0, 0);
		pASTBufferPos = DATATBLS_ExpressionBuffer_PushRawConstant(pOutASTBuffer, pOutASTBuffer, nOutASTBufferSize, &tContext, nEvaluatedValue);
		pASTBufferPos = DATATBLS_Evaluate_HandleNewOp(pASTBufferPos, pOutASTBuffer, nOutASTBufferSize, &tContext, 
			AST_None, 0, 0
		);
	}
	return pASTBufferPos - pOutASTBuffer;
}

