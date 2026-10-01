#pragma once

#include "storm/Error.hpp"
#include "storm/List.hpp"
#include "storm/hash/TSHashObjectChunk.hpp"
#include "storm/hash/TSHashTable.hpp"

template <class T, class TKey>
class TSHashTableReuse : public TSHashTable<T, TKey>
{
public:
	// Virtual member functions
	virtual ~TSHashTableReuse();
	virtual void Destroy();

protected:
	typedef TSHashObjectChunk<T, TKey> object_chunk_t;

	// Note: see TSHashTable for why the link offsets are applied from the
	// constructor body rather than through STORM_EXPLICIT_LIST at class scope.
	typedef typename TSHashTable<T, TKey>::SlotListT SlotListT;
	typedef TSExplicitList<object_chunk_t, 0> ChunkListT;

	// Member variables
	SlotListT m_reuseList;
	uint32_t m_chunkSize;
	ChunkListT m_chunkList;

	// Virtual member functions
	virtual void InternalDelete(T* ptr);
	virtual T* InternalNew(SlotListT* listptr, size_t extrabytes, uint32_t flags);

	// Member functions
	void Destructor();

public:
	// Member functions
	TSHashTableReuse() : m_chunkSize(16)
	{
		this->m_reuseList.SetLinkOffset(offsetof(T, m_linktoslot));
		this->m_chunkList.SetLinkOffset(offsetof(object_chunk_t, m_link));
	}
};

template <class T, class TKey>
TSHashTableReuse<T, TKey>::~TSHashTableReuse()
{
	this->Destructor();
}

template <class T, class TKey>
void TSHashTableReuse<T, TKey>::Destroy()
{
	this->Clear();
	this->Destructor();
}

template <class T, class TKey>
void TSHashTableReuse<T, TKey>::Destructor()
{
	this->m_chunkList.Clear();
	this->m_reuseList.Clear();
	this->m_chunkSize = 16;
}

template <class T, class TKey>
void TSHashTableReuse<T, TKey>::InternalDelete(T* ptr)
{
	this->m_fulllist.UnlinkNode(ptr);
	this->m_reuseList.LinkNode(ptr, 1, NULL);
}

template <class T, class TKey>
T* TSHashTableReuse<T, TKey>::InternalNew(SlotListT* listptr, size_t extrabytes, uint32_t flags)
{
	STORM_ASSERT(!extrabytes);
	(void)flags;

	T* node = this->m_reuseList.Head();

	if (!node)
	{
		object_chunk_t* chunk;

		while (true)
		{
			chunk = this->m_chunkList.Head();

			if (chunk && chunk->m_array.Reserved())
			{
				break;
			}

			chunk = this->m_chunkList.NewNode(1, 0, 0x0);
			chunk->m_array.Reserve(this->m_chunkSize, 0);
		}

		node = chunk->m_array.New();
	}

	listptr->LinkNode(node, 1, NULL);

	return node;
}