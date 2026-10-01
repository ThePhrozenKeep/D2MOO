#pragma once

#include "storm/Memory.hpp"
#include "storm/array/TSBaseArray.hpp"

template <class T>
class TSFixedArray : public TSBaseArray<T>
{
public:
	TSFixedArray() { this->Constructor(); }
	~TSFixedArray();
	TSFixedArray<T>& operator=(const TSFixedArray<T>& source);
	void Clear();
	void ReallocAndClearData(uint32_t count);
	void ReallocData(uint32_t count);
	void Set(uint32_t count, const T* data);
	void SetCount(uint32_t count);
};

template <class T>
TSFixedArray<T>::~TSFixedArray()
{
	for (uint32_t i = 0; i < this->Count(); i++)
	{
		T* element = &this->operator[](i);
		element->~T();
	}

	if (this->Ptr())
	{
		SMemFree(this->Ptr(), this->MemFileName(), this->MemLineNo(), 0x0);
	}
}

template <class T>
TSFixedArray<T>& TSFixedArray<T>::operator=(const TSFixedArray<T>& source)
{
	if (this != &source)
	{
		this->Set(source.Count(), source.Ptr());
	}

	return *this;
}

template <class T>
void TSFixedArray<T>::Clear()
{
	this->~TSFixedArray<T>();
	this->Constructor();
}

template <class T>
void TSFixedArray<T>::ReallocAndClearData(uint32_t count)
{
	this->m_alloc = count;

	if (this->m_data || count)
	{
		void* m = SMemReAlloc(this->m_data, sizeof(T) * count, this->MemFileName(), this->MemLineNo(), 0x0);
		this->m_data = static_cast<T*>(m);
	}
}

template <class T>
void TSFixedArray<T>::ReallocData(uint32_t count)
{
	T* oldData = this->m_data;

	if (count < this->m_count)
	{
		for (uint32_t i = count; i < this->m_count; i++)
		{
			(&this->m_data[i])->~T();
		}
	}

	this->m_alloc = count;

	void* v6 = SMemReAlloc(oldData, sizeof(T) * count, NULL, 0, 0x10);
	this->m_data = (T*)v6;

	if (!v6)
	{
		this->m_data = (T*)SMemAlloc(sizeof(T) * count, NULL, 0, 0x0);

		if (oldData)
		{
			uint32_t smallestCount = count >= this->m_count ? this->m_count : count;

			for (uint32_t i = 0; i < smallestCount; i++)
			{
				new (&this->m_data[i]) T(oldData[i]);
				(&oldData[i])->~T();
			}

			SMemFree(oldData, NULL, 0, 0x0);
		}
	}
}

template <class T>
void TSFixedArray<T>::Set(uint32_t count, const T* data)
{
	this->ReallocAndClearData(count);

	for (uint32_t i = 0; i < count; i++)
	{
		new (&this->m_data[i]) T(data[i]);
	}

	this->m_count = count;
}

template <class T>
void TSFixedArray<T>::SetCount(uint32_t count)
{
	if (count != this->m_count)
	{
		if (count)
		{
			this->ReallocData(count);

			for (uint32_t i = this->m_count; i < count; i++)
			{
				new (&this->m_data[i]) T();
			}

			this->m_count = count;
		}
		else
		{
			this->Clear();
		}
	}
}