#pragma once

#include "storm/List.hpp"
#include <stddef.h>
#include <D2StdInt.h>
#include <new>

// Layout-critical: D2MOO derives game structures from this and asserts the
// resulting offsets against the retail game (see D2GameStrc::lpCriticalSection at
// 0x18). Do not reorder or resize the members.
template <class T, class TKey>
class TSHashObject
{
public:
	// Member variables
	uint32_t m_hashval;
	TSLink<T> m_linktoslot;
	TSLink<T> m_linktofull;
	TKey m_key;

private:
	// Member functions
	TSHashObject() : m_hashval(0), m_key() {}
	friend T;
};