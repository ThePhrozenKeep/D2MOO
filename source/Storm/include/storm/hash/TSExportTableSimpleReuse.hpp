#pragma once

#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSHashTableReuse.hpp"

// Layout-critical: D2MOO embeds one of these in D2GameDataTableStrc, whose size is
// asserted against the retail game.
template <class T, class THandle>
class TSExportTableSimpleReuse : public TSHashTableReuse<T, HASHKEY_NONE>
{
public:
	// Virtual member functions
	virtual ~TSExportTableSimpleReuse() {}

	// Member functions
	void Delete(T* ptr);
	T* New(THandle* handlePtr);
	T* Ptr(THandle handle);

	HASHKEY_NONE* GetKeyPtr() { return &this->m_key; }

	// Note: public on purpose. Squall has no constructor here (it uses in-class
	// initialisers), but D2MOO embeds TSExportTableSyncReuse as a *member* of
	// D2GameDataTableStrc, so a private constructor would make that struct
	// non-default-constructible.
	TSExportTableSimpleReuse() : m_key(), m_sequence(0), m_wrapped(0)
	{
		// Squall seeds this from an in-class initialiser. Assigning in the body
		// instead keeps the same value without tripping VC6's "this used in base
		// member initializer list" warning (C4355).
		this->m_sequence = ~reinterpret_cast<uintptr_t>(this) & 0xFFFFFFF;
	}

private:
	// Member variables
	HASHKEY_NONE m_key;
	uint32_t m_sequence;
	int32_t m_wrapped;

	// Member functions
	THandle GenerateUniqueHandle();
};

template <class T, class THandle>
void TSExportTableSimpleReuse<T, THandle>::Delete(T* ptr)
{
	TSHashTable<T, HASHKEY_NONE>::Delete(ptr);
}

template <class T, class THandle>
THandle TSExportTableSimpleReuse<T, THandle>::GenerateUniqueHandle()
{
	while (true)
	{
		while (true)
		{
			this->m_sequence += 1;

			if (this->m_sequence)
			{
				break;
			}

			this->m_wrapped = 1;
		}

		if (!this->m_wrapped || !this->Ptr(reinterpret_cast<THandle>(this->m_sequence)))
		{
			break;
		}
	}

	return reinterpret_cast<THandle>(this->m_sequence);
}

template <class T, class THandle>
T* TSExportTableSimpleReuse<T, THandle>::New(THandle* handlePtr)
{
	THandle handle = this->GenerateUniqueHandle();
	*handlePtr = handle;

	uint32_t hashval = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
	return this->TSHashTable<T, HASHKEY_NONE>::New(hashval, this->m_key, 0, 0x0);
}

template <class T, class THandle>
T* TSExportTableSimpleReuse<T, THandle>::Ptr(THandle handle)
{
	uint32_t hashval = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
	return this->TSHashTable<T, HASHKEY_NONE>::Ptr(hashval, this->m_key);
}