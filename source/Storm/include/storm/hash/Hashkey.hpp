#pragma once

#include <stddef.h>
#include <D2StdInt.h>
#include <new>

class HASHKEY_PTR
{
public:
	// Member variables
	void* m_key;

	// Member functions
	bool operator==(const HASHKEY_PTR& key) { return this->m_key == key.m_key; }
};

class HASHKEY_STR
{
public:
	// Member variables
	char* m_str;

	HASHKEY_STR() : m_str(NULL) {}
	~HASHKEY_STR();
	HASHKEY_STR& operator=(const char* str);
	bool operator==(const char* str) const;
};

class HASHKEY_STRI : public HASHKEY_STR
{
public:
	// Member functions
	// NOLINTNEXTLINE(bugprone-derived-method-shadowing-base-method)
	HASHKEY_STRI& operator=(const char* str);
	// NOLINTNEXTLINE(bugprone-derived-method-shadowing-base-method)
	bool operator==(const char* str) const;
};

class HASHKEY_NONE
{
public:
	// Member functions
	bool operator==(const HASHKEY_NONE&) const { return true; }
};