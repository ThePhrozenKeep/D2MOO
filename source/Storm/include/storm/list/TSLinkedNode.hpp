#pragma once

#include "storm/list/TSLink.hpp"

template <class T>
class TSLinkedNode
{
public:
	// Member variables
	TSLink<T> m_link;

	// Member functions
	~TSLinkedNode() { this->Unlink(); }
	T* Next();
	T* Prev();
	void Unlink();
};

template <class T>
T* TSLinkedNode<T>::Next()
{
	return this->m_link.Next();
}

template <class T>
T* TSLinkedNode<T>::Prev()
{
	return this->m_link.Prev();
}

template <class T>
void TSLinkedNode<T>::Unlink()
{
	this->m_link.Unlink();
}