#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/TokenTbls.h>


BEGIN_VISIT(D2ArmTypeTxt)
	ARRAY(szName)
	ARRAY(szToken)
END_VISIT()

BEGIN_VISIT(D2PlrModeTypeTxt)
	ARRAY(szName)
	ARRAY(szToken)
END_VISIT()

BEGIN_VISIT(D2PlrModeDataTbl)
	FIELD(nPlrModeTypeTxtRecordCount)
	POINTER(pPlrModeTypeTxt)
	POINTER(pPlayerType)
	POINTER(pPlayerMode)
END_VISIT()

BEGIN_VISIT(D2MonModeTxt)
	ARRAY(szName)
	FIELD(dwToken)
	FIELD(nDTDir)
	FIELD(nNUDir)
	FIELD(nWLDir)
	FIELD(nGHDir)
	FIELD(nA1Dir)
	FIELD(nA2Dir)
	FIELD(nBLDir)
	FIELD(nSCDir)
	FIELD(nS1Dir)
	FIELD(nS2Dir)
	FIELD(nS3Dir)
	FIELD(nS4Dir)
	FIELD(nDDDir)
	FIELD(nKBDir)
	FIELD(nSQDir)
	FIELD(nRNDir)
END_VISIT()

BEGIN_VISIT(D2MonModeDataTbl)
	FIELD(nMonModeTxtRecordCount)
	POINTER(pMonModeTxt)
	ARRAY(pMonMode)
END_VISIT()

BEGIN_VISIT(D2ObjModeTypeTxt)
	ARRAY(szName)
	ARRAY(szToken)
END_VISIT()

BEGIN_VISIT(D2ObjModeDataTbl)
	FIELD(nObjModeTypeTxtRecordCount)
	POINTER(pObjModeTypeTxt)
	POINTER(pObjType)
	POINTER(pObjMode)
END_VISIT()

BEGIN_VISIT(D2CompositTxt)
	ARRAY(szName)
	ARRAY(szToken)
END_VISIT()

BEGIN_VISIT(D2CompCodeTxt)
	FIELD(dwCode)
END_VISIT()
