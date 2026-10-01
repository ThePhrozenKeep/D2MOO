#pragma once

#include "storm/list/TSGetExplicitLink.hpp"
#include "storm/list/TSList.hpp"

#define STORM_EXPLICIT_LIST(T, link) TSExplicitList<T, offsetof(T, link)>

template <class T, size_t offset>
class TSExplicitList : public TSList<T, TSGetExplicitLink<T> >
{
public:
	// Member functions
	TSExplicitList() : TSList<T, TSGetExplicitLink<T> >()
	{
		this->SetLinkOffset(offset);
	}
};