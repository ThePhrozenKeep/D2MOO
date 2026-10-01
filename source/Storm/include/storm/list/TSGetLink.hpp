#pragma once

#include "storm/list/TSLink.hpp"

template <class T>
class TSGetLink
{
public:
	// Static functions
	static TSLink<T>* Link(const void* nodeptr, ptrdiff_t linkoffset);
};

template <class T>
TSLink<T>* TSGetLink<T>::Link(const void* nodeptr, ptrdiff_t linkoffset)
{
	return reinterpret_cast<TSLink<T>*>(reinterpret_cast<uintptr_t>(nodeptr));
}