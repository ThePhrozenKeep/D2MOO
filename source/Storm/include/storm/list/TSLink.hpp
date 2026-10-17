#pragma once

#include <stddef.h>
#include <D2StdInt.h>
#include <new>

template <class T>
class TSLink
{
public:
	// Member variables
	TSLink<T>* m_prevlink;
	T* m_next;

	// Member functions
	TSLink() : m_prevlink(NULL), m_next(NULL) {}
	~TSLink();
	bool IsLinked() const;
	T* Next();
	TSLink<T>* NextLink(ptrdiff_t linkoffset);
	T* Prev();
	T* RawNext();
	void Unlink();
};

template <class T>
TSLink<T>::~TSLink()
{
	this->Unlink();
}

template <class T>
bool TSLink<T>::IsLinked() const
{
	return this->m_next != NULL;
}

template <class T>
T* TSLink<T>::Next()
{
	// Check for sentinel node (indicates list end)
	return reinterpret_cast<intptr_t>(this->m_next) <= 0 ? NULL : this->m_next;
}

template <class T>
TSLink<T>* TSLink<T>::NextLink(ptrdiff_t linkoffset)
{
	T* next = this->m_next;

	if (reinterpret_cast<intptr_t>(next) <= 0)
	{
		// End of list
		return reinterpret_cast<TSLink<T>*>(~reinterpret_cast<uintptr_t>(next));
	}
	else
	{
		ptrdiff_t offset;

		if (linkoffset < 0)
		{
			offset = reinterpret_cast<uintptr_t>(this) - reinterpret_cast<uintptr_t>(this->m_prevlink->m_next);
		}
		else
		{
			offset = linkoffset;
		}

		return reinterpret_cast<TSLink<T>*>(reinterpret_cast<uintptr_t>(this->m_next) + offset);
	}
}

template <class T>
T* TSLink<T>::Prev()
{
	return this->m_prevlink->m_prevlink->Next();
}

template <class T>
T* TSLink<T>::RawNext()
{
	return this->m_next;
}

template <class T>
void TSLink<T>::Unlink()
{
	if (this->m_prevlink)
	{
		this->NextLink(-1)->m_prevlink = this->m_prevlink;
		this->m_prevlink->m_next = this->m_next;

		this->m_prevlink = NULL;
		this->m_next = NULL;
	}
}