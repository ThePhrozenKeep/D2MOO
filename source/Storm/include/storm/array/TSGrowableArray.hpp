#pragma once

#include "storm/array/TSFixedArray.hpp"
#include <cstring>

template <class T>
class TSGrowableArray : public TSFixedArray<T>
{
public:
	// Member variables
	uint32_t m_chunk;

	// Member functions
	TSGrowableArray() : m_chunk(0) {}

	uint32_t Add(uint32_t count, const T* data);
	uint32_t Add(uint32_t count, uint32_t incr, const T* data);
	uint32_t CalcChunkSize(uint32_t count);
	void GrowToFit(uint32_t index, int32_t zero);
	T* New(void);
	void Reserve(uint32_t count, int32_t round);
	uint32_t Reserved() const;
	uint32_t RoundToChunk(uint32_t count, uint32_t chunk);
	void SetCount(uint32_t count);
};

template <class T>
uint32_t TSGrowableArray<T>::Add(uint32_t count, const T* data)
{
	this->Reserve(count, 1);

	for (uint32_t i = 0; i < count; i++)
	{
		T* element = &this->m_data[this->m_count + i];
		*element = data[i];
	}

	this->m_count += count;
	return this->m_count - count;
}

template <class T>
uint32_t TSGrowableArray<T>::Add(uint32_t count, uint32_t incr, const T* data)
{
	this->Reserve(count, 1);

	const char* dataptr = reinterpret_cast<const char*>(data);

	for (uint32_t i = 0; i < count; i++)
	{
		T* element = &this->m_data[this->m_count + i];
		*element = *reinterpret_cast<const T*>(dataptr);
		dataptr += incr;
	}

	this->m_count += count;
	return this->m_count - count;
}

template <class T>
uint32_t TSGrowableArray<T>::CalcChunkSize(uint32_t count)
{
	uint32_t maxChunk = static_cast<int32_t>(256 / sizeof(T)) > 8 ? static_cast<uint32_t>(256 / sizeof(T)) : 8;

	if (count >= maxChunk)
	{
		this->m_chunk = maxChunk;
		return maxChunk;
	}

	uint32_t result = count;

	for (uint32_t i = count & (count - 1); i; i &= i - 1)
	{
		result = i;
	}

	if (result < 1)
	{
		result = 1;
	}

	return result;
}

template <class T>
void TSGrowableArray<T>::GrowToFit(uint32_t index, int32_t zero)
{
	uint32_t count = this->m_count;

	if (index >= count)
	{
		this->Reserve(index - count + 1, 1);

		if (zero)
		{
			memset(&this->m_data[count], 0, sizeof(T) * (index - count + 1));
		}

		this->m_count = index + 1;
	}
}

template <class T>
T* TSGrowableArray<T>::New()
{
	this->Reserve(1, 1);
	void* element = &this->m_data[this->m_count++];
	return new (element) T();
}

template <class T>
void TSGrowableArray<T>::Reserve(uint32_t count, int32_t round)
{
	if (count + this->m_count > this->m_alloc)
	{
		if (round)
		{
			uint32_t chunk = this->m_chunk;

			if (!chunk)
			{
				chunk = this->CalcChunkSize(count + this->m_count);
			}

			count = this->RoundToChunk(count, chunk);
		}

		this->ReallocData(count + this->m_count);
	}
}

template <class T>
uint32_t TSGrowableArray<T>::Reserved() const
{
	return this->m_alloc - this->m_count;
}

template <class T>
uint32_t TSGrowableArray<T>::RoundToChunk(uint32_t count, uint32_t chunk)
{
	if (count % chunk)
	{
		return chunk - count % chunk + count;
	}
	else
	{
		return count;
	}
}

template <class T>
void TSGrowableArray<T>::SetCount(uint32_t count)
{
	if (count > this->m_count)
	{
		// Expand size
		this->Reserve(count - this->m_count, 1);

		T* element;

		for (uint32_t i = this->m_count; i < count; ++i)
		{
			element = &this->m_data[i];
			new (element) T();
		}
	}

	this->m_count = count;
}