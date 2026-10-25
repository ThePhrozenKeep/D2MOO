#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/AnimTbls.h>


BEGIN_VISIT(D2AnimDataRecordStrc)
	ARRAY(szAnimDataName)
	FIELD(dwFrames)
	FIELD(dwAnimSpeed)
	ARRAY(pFrameFlags)
END_VISIT()

BEGIN_VISIT(D2AnimDataBucketStrc)
	FIELD(nbEntries)
	DYNAMIC_ARRAY(aEntries, nbEntries)
END_VISIT()

BEGIN_VISIT(D2AnimDataTableStrc)
	OMIT(pBinaryData)
	ARRAY(pHashTableBucket)
	OBJECT(tDefaultRecord)
END_VISIT()
