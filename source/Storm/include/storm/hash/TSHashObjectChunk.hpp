#pragma once

#include "storm/Array.hpp"
#include "storm/List.hpp"

template <class T, class TKey>
class TSHashObjectChunk
{
public:
	// Member variables
	TSGrowableArray<T> m_array;
	TSLink<TSHashObjectChunk<T, TKey> > m_link;

	// Member functions
	TSHashObjectChunk() {}
};