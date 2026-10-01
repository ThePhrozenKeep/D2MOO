#include "storm/thread/CCritSect.hpp"

CCritSect::CCritSect()
{
	InitializeCriticalSection(&this->m_critsect);
}

CCritSect::~CCritSect()
{
	DeleteCriticalSection(&this->m_critsect);
}

void CCritSect::Enter()
{
	EnterCriticalSection(&this->m_critsect);
}

void CCritSect::Enter(int32_t forWriting)
{
	// Overload to provide compatibility with CSRWLock
	(void)forWriting;
	this->Enter();
}

void CCritSect::Leave()
{
	LeaveCriticalSection(&this->m_critsect);
}

void CCritSect::Leave(int32_t fromWriting)
{
	// Overload to provide compatibility with CSRWLock
	(void)fromWriting;
	this->Leave();
}
