#pragma once

#include "storm/Memory.hpp"
#include "storm/list/TSGetLink.hpp"
#include "storm/list/TSLink.hpp"
#include <typeinfo>

#define STORM_LIST(T) TSList<T, TSGetLink<T> >

template <class T, class TGetLink>
class TSList
{
public:
	// Member variables
	ptrdiff_t m_linkoffset;
	TSLink<T> m_terminator;

	// Member functions
	TSList() : m_linkoffset(0) { this->InitializeTerminator(); }
	~TSList() { this->UnlinkAll(); }
	void ChangeLinkOffset(ptrdiff_t linkoffset);
	void Clear();
	T* DeleteNode(T* ptr);
	T* Head();
	void InitializeTerminator();
	bool IsLinked(T* ptr);
	TSLink<T>* Link(const T* ptr);
	void LinkNode(T* ptr, uint32_t linktype, T* existingptr);
	void LinkToHead(T* ptr);
	void LinkToTail(T* ptr);
	const char* MemFileName() const;
	int32_t MemLineNo() const;
	T* NewNode(uint32_t location, size_t extrabytes, uint32_t flags);
	T* Next(const T* ptr);
	T* RawNext(const T* ptr);
	void SetLinkOffset(ptrdiff_t linkoffset);
	T* Tail();
	void UnlinkAll();
	void UnlinkNode(T* node);
};

template <class T, class TGetLink>
void TSList<T, TGetLink>::ChangeLinkOffset(ptrdiff_t linkoffset)
{
	if (linkoffset != this->m_linkoffset)
	{
		this->UnlinkAll();
		this->SetLinkOffset(linkoffset);
	}
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::Clear()
{
	T* node;

	while ((node = this->Head()))
	{
		this->DeleteNode(node);
	}
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::DeleteNode(T* ptr)
{
	T* next = this->Next(ptr);

	ptr->~T();
	SMemFree(ptr, __FILE__, __LINE__, 0);

	return next;
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::Head()
{
	return this->m_terminator.Next();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::InitializeTerminator()
{
	this->m_terminator.m_prevlink = &this->m_terminator;

	// Set sentinel node (indicates list end)
	this->m_terminator.m_next = reinterpret_cast<T*>(~reinterpret_cast<uintptr_t>(&this->m_terminator));
}

template <class T, class TGetLink>
bool TSList<T, TGetLink>::IsLinked(T* ptr)
{
	return TGetLink::Link(ptr, this->m_linkoffset)->IsLinked();
}

template <class T, class TGetLink>
TSLink<T>* TSList<T, TGetLink>::Link(const T* ptr)
{
	if (ptr)
	{
		return TGetLink::Link(ptr, this->m_linkoffset);
	}
	else
	{
		return &this->m_terminator;
	}
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::LinkNode(T* ptr, uint32_t linktype, T* existingptr)
{
	TSLink<T>* v5 = this->Link(ptr);

	if (v5->m_prevlink)
	{
		v5->Unlink();
	}

	TSLink<T>* v7;

	if (existingptr)
	{
		v7 = this->Link(existingptr);
	}
	else
	{
		v7 = &this->m_terminator;
	}

	TSLink<T>* v8;

	switch (linktype)
	{
	case 1:
		// After existingptr
		v5->m_prevlink = v7;
		v5->m_next = v7->m_next;
		v7->NextLink(this->m_linkoffset)->m_prevlink = v5;
		v7->m_next = ptr;
		break;

	case 2:
		// Before existingptr
		v8 = v7->m_prevlink;
		v5->m_prevlink = v7->m_prevlink;
		v5->m_next = v8->m_next;
		v8->m_next = ptr;
		v7->m_prevlink = v5;
		break;

	default:
		// TODO error
		break;
	}
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::LinkToHead(T* ptr)
{
	this->LinkNode(ptr, 1, NULL);
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::LinkToTail(T* ptr)
{
	this->LinkNode(ptr, 2, NULL);
}

template <class T, class TGetLink>
const char* TSList<T, TGetLink>::MemFileName() const
{
	return typeid(T).name();
}

template <class T, class TGetLink>
int32_t TSList<T, TGetLink>::MemLineNo() const
{
	return -2;
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::NewNode(uint32_t location, size_t extrabytes, uint32_t flags)
{
	void* m = SMemAlloc(sizeof(T) + extrabytes, this->MemFileName(), this->MemLineNo(), flags | 0x8);

	T* node;

	if (m)
	{
		node = new (m) T();
	}
	else
	{
		node = NULL;
	}

	if (location)
	{
		this->LinkNode(node, location, NULL);
	}

	return node;
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::Next(const T* ptr)
{
	return this->Link(ptr)->Next();
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::RawNext(const T* ptr)
{
	TSLink<T>* link = this->Link(ptr);
	return link->RawNext();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::SetLinkOffset(ptrdiff_t linkoffset)
{
	this->m_linkoffset = linkoffset;
	this->InitializeTerminator();
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::Tail()
{
	return this->m_terminator.Prev();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::UnlinkAll()
{
	T* node;

	while ((node = this->Head()))
	{
		this->UnlinkNode(node);
	}
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::UnlinkNode(T* node)
{
	TSLink<T>* link = this->Link(node);
	link->Unlink();
}