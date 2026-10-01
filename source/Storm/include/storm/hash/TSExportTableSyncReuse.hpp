#pragma once

#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSExportTableSimpleReuse.hpp"

// Layout-critical: D2MOO embeds one of these in D2GameDataTableStrc, whose size is
// asserted against the retail game.
template <class T, class THandle, class TLockedHandle, class TSync>
class TSExportTableSyncReuse : public TSExportTableSimpleReuse<T, THandle>
{
public:
	// Virtual member functions
	virtual ~TSExportTableSyncReuse() {}

	// Member functions
	void Delete(THandle handle);
	void DeleteUnlock(T* ptr, TLockedHandle lockedHandle);
	T* Lock(THandle handle, TLockedHandle* lockedHandlePtr, int32_t forWriting);
	T* NewLock(THandle* handlePtr, TLockedHandle* lockedHandlePtr);
	void Unlock(TLockedHandle lockedHandle);
	TSync& GetSync() { return this->m_sync; }

	// Public for the same reason as TSExportTableSimpleReuse's constructor: this
	// is embedded by value in D2GameDataTableStrc.
	TSExportTableSyncReuse() : m_sync() {}

private:
	// Member variables
	TSync m_sync;

	// Member functions
	int32_t IsForWriting(TLockedHandle lockedHandle);
	void SyncEnterLock(TLockedHandle* lockedHandlePtr, int32_t forWriting);
	void SyncLeaveLock(TLockedHandle lockedHandle);
};

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::Delete(THandle handle)
{
	TLockedHandle lockedHandle;
	T* ptr = this->Lock(handle, &lockedHandle, 1);
	this->DeleteUnlock(ptr, lockedHandle);
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::DeleteUnlock(T* ptr, TLockedHandle lockedHandle)
{
	this->TSExportTableSimpleReuse<T, THandle>::Delete(ptr);
	this->Unlock(lockedHandle);
}

template <class T, class THandle, class TLockedHandle, class TSync>
int32_t TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::IsForWriting(TLockedHandle lockedHandle)
{
	return reinterpret_cast<intptr_t>(lockedHandle) == 1;
}

template <class T, class THandle, class TLockedHandle, class TSync>
T* TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::Lock(THandle handle, TLockedHandle* lockedHandlePtr, int32_t forWriting)
{
	this->SyncEnterLock(lockedHandlePtr, forWriting);

	T* ptr = this->Ptr(handle);

	if (!ptr)
	{
		this->SyncLeaveLock(*lockedHandlePtr);
		*lockedHandlePtr = 0;
		return NULL;
	}

	return ptr;
}

template <class T, class THandle, class TLockedHandle, class TSync>
T* TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::NewLock(THandle* handlePtr, TLockedHandle* lockedHandlePtr)
{
	this->SyncEnterLock(lockedHandlePtr, 1);
	return this->New(handlePtr);
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::SyncEnterLock(TLockedHandle* lockedHandlePtr, int32_t forWriting)
{
	this->m_sync.Enter(forWriting);
	*lockedHandlePtr = reinterpret_cast<TLockedHandle>(forWriting ? 1 : -1);
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::SyncLeaveLock(TLockedHandle lockedHandle)
{
	if (lockedHandle)
	{
		this->m_sync.Leave(this->IsForWriting(lockedHandle));
	}
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::Unlock(TLockedHandle lockedHandle)
{
	this->SyncLeaveLock(lockedHandle);
}