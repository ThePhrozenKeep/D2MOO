#pragma once

#include "storm/Error.hpp"
#include <typeinfo>

template <class T>
class TSBaseArray
{
public:
	// Member variables
	uint32_t m_alloc;
	uint32_t m_count;
	T* m_data;

	// Member functions
	T& operator[](uint32_t index);
	void CheckArrayBounds(uint32_t index) const;
	void Constructor();
	uint32_t Count() const;
	T* Ptr();
	const T* Ptr() const;
	T* Top();

	// Non-virtual here: these were never virtual in the retail Storm either.
	const char* MemFileName() const;
	int32_t MemLineNo() const;
};

template <class T>
T& TSBaseArray<T>::operator[](uint32_t index)
{
	this->CheckArrayBounds(index);
	return this->m_data[index];
}

template <class T>
void TSBaseArray<T>::CheckArrayBounds(uint32_t index) const
{
	if (index < this->Count())
	{
		return;
	}

	SErrDisplayErrorFmt(
		0x85100080,
		this->MemFileName(),
		this->MemLineNo(),
		1,
		1,
		"index (0x%08X), array size (0x%08X)",
		index,
		this->Count());
}

template <class T>
void TSBaseArray<T>::Constructor()
{
	this->m_alloc = 0;
	this->m_count = 0;
	this->m_data = NULL;
}

template <class T>
uint32_t TSBaseArray<T>::Count() const
{
	return this->m_count;
}

template <class T>
const char* TSBaseArray<T>::MemFileName() const
{
	return typeid(T).name();
}

template <class T>
int32_t TSBaseArray<T>::MemLineNo() const
{
	return -2;
}

template <class T>
T* TSBaseArray<T>::Ptr()
{
	return this->m_data;
}

template <class T>
const T* TSBaseArray<T>::Ptr() const
{
	return this->m_data;
}

template <class T>
T* TSBaseArray<T>::Top()
{
	if (this->m_count == 0)
	{
		return NULL;
	}

	return &this->m_data[this->m_count - 1];
}