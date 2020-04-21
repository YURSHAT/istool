/**
** @file
** @brief Thread and synchronization classes
*/

#pragma once

namespace Henden {

/**
** @brief Encapsulates CRITICAL_SECTION
**
** Mostly used from CSingleLock
** @sa CSingleLock CEvent CMutex
*/
class CCriticalSection : public CRITICAL_SECTION {
public:
	/// Constructor
	CCriticalSection() {
		InitializeCriticalSection(this);
	}
	/// Destructor
	~CCriticalSection() {
		DeleteCriticalSection(this);
	}
	/// Enter the critical section
	void Enter() {
		EnterCriticalSection(this);
	}
	/// Leave the critical section
	void Leave() {
		LeaveCriticalSection(this);
	}
};

/**
** @brief Locks a critical section
** @sa CCriticalSection CEvent CMutex
*/
class CSingleLock {
public:
	/// Constructs the lock object
	CSingleLock(CCriticalSection& cs,bool bInitialLock=false) : m_cs(cs) {
		m_bLocked = false;
		if(bInitialLock)
			Lock();
	}
	/// Destructor
	~CSingleLock() {
		if(m_bLocked)
			Unlock();
	}
	/// Locks the critical section
	void Lock() {
		ATLASSERT(m_bLocked==false);
		m_cs.Enter();
		m_bLocked = true;
	}
	/// Unlocks the critical section
	void Unlock() {
		ATLASSERT(m_bLocked==true);
		m_cs.Leave();
		m_bLocked = false;
	}
	/// Returns true if the critical section is locked
	bool IsLocked() const {
		return m_bLocked;
	}
protected:
	CCriticalSection&	m_cs;		///< References the critical section
	bool				m_bLocked;	///< Is the critical section locked?
};

/**
** @brief Event class
** @sa CCriticalSection CSingleLock CMutex
*/
class CEvent {
public:
	/// Constructor
	CEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE, LPCTSTR pstrName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL) {
		m_hObject = ::CreateEvent(lpsaAttribute, bManualReset, bInitiallyOwn, pstrName);
		ATLASSERT(m_hObject != NULL); 
	}

	/// Destructor
	~CEvent() {
		ATLASSERT(m_hObject != NULL); 
		CloseHandle(m_hObject);
	}

	/// Set the event
	BOOL SetEvent() { 
		ATLASSERT(m_hObject != NULL); 
		return ::SetEvent(m_hObject); 
	}

	/// Pulse the event?!?
	BOOL PulseEvent() { 
		ATLASSERT(m_hObject != NULL); 
		return ::PulseEvent(m_hObject); 
	}

	/// Reset the event
	BOOL ResetEvent() { 
		ATLASSERT(m_hObject != NULL); 
		return ::ResetEvent(m_hObject); 
	}

	/// Waits for the event to be set
    void Wait() {
		ATLASSERT(m_hObject != NULL); 
        WaitForSingleObject(m_hObject, INFINITE);
    }

	/// Returns the windows api handle of the event
	operator HANDLE() {
		ATLASSERT(m_hObject != NULL); 
		return m_hObject;
	}
private:
	HANDLE	m_hObject;
};

/**
** @brief Mutex class
** @sa CCriticalSection CSingleLock CEvent
*/
class CMutex {
public:
	/// Constructor
	CMutex(bool bInitiallyOwn=false,LPCTSTR pszName=NULL,LPSECURITY_ATTRIBUTES pszAttribute=NULL) {
		m_hObject = ::CreateMutex(pszAttribute,bInitiallyOwn,pszName);
	}

	/// Destructor
	~CMutex() {
		::CloseHandle(m_hObject);
	}

	/// Waits for the mutex to be available
    void Wait() {
		ATLASSERT(m_hObject != NULL); 
        WaitForSingleObject(m_hObject, INFINITE);
    }

	/// Returns the mutex handle
	operator HANDLE() {
		ATLASSERT(m_hObject != NULL); 
		return m_hObject;
	}
private:
	HANDLE	m_hObject;
};

} // namespace Henden
