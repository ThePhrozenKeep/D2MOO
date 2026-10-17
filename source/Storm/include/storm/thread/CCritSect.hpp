#pragma once

#include <stddef.h>
#include <D2StdInt.h>
#include <new>
#include <Windows.h>

// Layout-critical: embedded by value in D2GameDataTableStrc via
// TSExportTableSyncReuse, so the CRITICAL_SECTION must stay the only member.
class CCritSect
{
public:
	// Member functions
	CCritSect();
	~CCritSect();
	void Enter();
	void Enter(int32_t forWriting);
	void Leave();
	void Leave(int32_t fromWriting);

private:
	// Member variables
	CRITICAL_SECTION m_critsect;
};