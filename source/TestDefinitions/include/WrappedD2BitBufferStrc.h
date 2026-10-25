#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2BitManip.h>


struct WrappedD2BitBufferStrc : D2BitBufferStrc {
	int buffer_size;

	WrappedD2BitBufferStrc() {
		this->pBuffer = nullptr;
		this->nBits = 0;
		this->nPos = 0;
		this->nPosBits = 0;
		this->bFull = FALSE;
		this->buffer_size = 0;
	}

	WrappedD2BitBufferStrc(const D2BitBufferStrc& buffer, unsigned int size) {
		this->pBuffer = buffer.pBuffer;
		this->nBits = buffer.nBits;
		this->nPos = buffer.nPos;
		this->nPosBits = buffer.nPosBits;
		this->bFull = buffer.bFull;
		this->buffer_size = size;
	}
};

BEGIN_VISIT(WrappedD2BitBufferStrc)
	DYNAMIC_ARRAY(pBuffer, buffer_size)
	FIELD(nBits)
	FIELD(nPos)
	FIELD(nPosBits)
	FIELD(bFull)
END_VISIT()
