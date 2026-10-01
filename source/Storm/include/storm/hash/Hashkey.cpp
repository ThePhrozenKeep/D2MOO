#include "storm/hash/Hashkey.hpp"
#include "storm/Memory.hpp"
#include "storm/String.hpp"
#include <string.h>

HASHKEY_STR::~HASHKEY_STR()
{
	if (this->m_str)
	{
		SMemFree(this->m_str, __FILE__, __LINE__, 0x0);
	}
}

HASHKEY_STR& HASHKEY_STR::operator=(const char* str)
{
	if (this->m_str != str)
	{
		if (this->m_str)
		{
			SMemFree(this->m_str, __FILE__, __LINE__, 0x0);
		}

		size_t len = strlen(str) + 1;
		char* dup = (char*)SMemAlloc(len, __FILE__, __LINE__, 0x0);
		memcpy(dup, str, len);
		this->m_str = dup;
	}

	return *this;
}

bool HASHKEY_STR::operator==(const char* str) const
{
	return SStrCmp(this->m_str, str, STORM_MAX_STR) == 0;
}

HASHKEY_STRI& HASHKEY_STRI::operator=(const char* str)
{
	static_cast<HASHKEY_STR&>(*this) = str;
	return *this;
}

bool HASHKEY_STRI::operator==(const char* str) const
{
	return SStrCmpI(this->m_str, str, STORM_MAX_STR) == 0;
}