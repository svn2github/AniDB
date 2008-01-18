/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Thread tools
 *
 * Copyright (C) 2003 Julien Coloos.  All rights reserved.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE.QPL included in the packaging of this file.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * Licensees holding an other license may use this file in accordance with 
 * the Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See http://www.opensource.org/licenses/qtpl.php for QPL licensing information.
 * See http://www.opensource.org/licenses/gpl-license.html for GPL licensing information.
 *
 * Contact cyrius@corecodec.org if any conditions of this licensing
 * are not clear to you.
 *
 ********************************************************************/

#ifndef __TOOLS_THREAD_H__
#define __TOOLS_THREAD_H__

#include "common.h"

#if defined(__POSIX__) /* POSIX */

#include <pthread.h>

typedef pthread_mutex_t	Mutex_Handle;
typedef class Mutex	    CSect_Handle;
#define MUTEX_FOR_CRITICALSECTION	1

#elif defined(WIN32) /* Microsoft */

#include <windows.h>

typedef HANDLE				Mutex_Handle;
typedef CRITICAL_SECTION	CSect_Handle;

#else

#error "Please implement the mutexes for your system."

#endif

//////////////////////////////////////////////////////////////////////
// Mutex

typedef enum {
	kMutex_OK	= 0
	,kMutex_EBusy
	,kMutex_EInvalid
	,kMutex_EPermission
	,kMutex_EOther
} eMutexResult;

/* Use inline call for acquiring a Mutex under Win32 */
#if defined(WIN32)
#define MUTEX_MODIFIER	inline
#else
#define MUTEX_MODIFIER
#endif

class TOOLS_DLL_API Mutex {
protected:
	Mutex_Handle	m_hMutex;

#if defined(WIN32)
	eMutexResult	Lock(DWORD dwWaitMilliseconds);
#endif

public:
	Mutex(void);
	~Mutex(void);

	/* ++/-- seems to make the code cleaner :p */
	MUTEX_MODIFIER eMutexResult	operator++(void);
	               eMutexResult	operator--(void);

	MUTEX_MODIFIER eMutexResult	TryLock(void);
};

#if defined(WIN32)
	MUTEX_MODIFIER eMutexResult	Mutex::operator++(void) { return Lock(INFINITE); }
	MUTEX_MODIFIER eMutexResult	Mutex::TryLock(void) { return Lock(0); }
#endif

/* Automatically lock a Mutex */
class TOOLS_DLL_API MutexLocker {
protected:
	Mutex&	m_Mutex;

public:
	MutexLocker(Mutex& mutex);
	~MutexLocker(void);
};

//////////////////////////////////////////////////////////////////////
// Critical Section (Win32 specific, resolving to Mutex under POSIX)

/* Use inline calls when CriticalSection actually use Mutex */
#if MUTEX_FOR_CRITICALSECTION
#define CSECT_MODIFIER	inline
#else
#define CSECT_MODIFIER
#endif

class TOOLS_DLL_API CriticalSection {
protected:
	CSect_Handle	m_CSect;

public:
	CSECT_MODIFIER CriticalSection(void);
	CSECT_MODIFIER ~CriticalSection(void);

	CSECT_MODIFIER void	operator++(void);
	CSECT_MODIFIER void	operator--(void);
};

#if MUTEX_FOR_CRITICALSECTION
	CSECT_MODIFIER CriticalSection::CriticalSection() { }
	CSECT_MODIFIER CriticalSection::~CriticalSection() { }

	CSECT_MODIFIER void CriticalSection::operator++(void) { ++m_CSect; }
	CSECT_MODIFIER void CriticalSection::operator--(void) { --m_CSect; }
#endif /* MUTEX_FOR_CRITICALSECTION */

/* Automatically lock a CriticalSection */
class TOOLS_DLL_API CriticalSectionLocker {
protected:
	CriticalSection&	m_CSect;

public:
	CriticalSectionLocker(CriticalSection& CSect);
	~CriticalSectionLocker(void);
};

//////////////////////////////////////////////////////////////////////
// Semaphore

typedef enum {
	kSemaphore_OK	= 0
	,kSemaphore_EBusy
	,kSemaphore_EOther
} eSemaphoreResult;

/* Use inline call for acquiring a Semaphore under Win32 */
#if defined(WIN32)
#define SEMAPHORE_MODIFIER	inline
#else
#define SEMAPHORE_MODIFIER
#endif

class TOOLS_DLL_API Semaphore {
protected:
#if defined(WIN32)
	HANDLE		m_hSemaphore;

	eSemaphoreResult	Lock(DWORD dwWaitMilliseconds);
#elif defined(__POSIX__)
	uint32		m_iMaxCount;
	uint32		m_iCount;

	pthread_mutex_t	m_Mutex;
	pthread_cond_t	m_Cond;
#endif

public:
	Semaphore(uint32 iMaxCount=0, uint32 iInitialCount=0);
	~Semaphore(void);

	SEMAPHORE_MODIFIER eSemaphoreResult	Wait(uint32 iMilliseconds);

	SEMAPHORE_MODIFIER eSemaphoreResult	operator++(void);
	                   eSemaphoreResult	operator--(void);

	SEMAPHORE_MODIFIER eSemaphoreResult	TryLock(void);
};

#if defined(WIN32)
	SEMAPHORE_MODIFIER eSemaphoreResult	Semaphore::Wait(uint32 iMilliseconds) { return Lock(iMilliseconds); }
	SEMAPHORE_MODIFIER eSemaphoreResult	Semaphore::operator++(void) { return Lock(INFINITE); }
	SEMAPHORE_MODIFIER eSemaphoreResult	Semaphore::TryLock(void) { return Lock(0); }
#endif

	/* Automatically lock a Mutex */
class TOOLS_DLL_API SemaphoreLocker {
protected:
	Semaphore&	m_Semaphore;

public:
	SemaphoreLocker(Semaphore& sem);
	~SemaphoreLocker(void);
};

//////////////////////////////////////////////////////////////////////
/* Event
 *
 * Principle:
 * An Event can be 'signaled' or 'non-signaled'.
 * Threads can ask to wait for an Event. One (or more, if asked)
 * waiting thread is unblocked when the state of the Event is set to
 * 'signaled'.
 * A thread can signal the Event, thus setting its state to 'signaled'.
 * The nature of the signal state is specified when creating the Event.
 * It can be either
 *  - permanent: i.e. a thread needs to manually reset its state to
 *    'non-signaled'.
 *  - auto-reseting: when a waiting thread is unblocked, the state of
 *    the event is reseted automatically to 'non-signaled', preventing
 *    other waiting threads to be unblocked too.
 *
 * Implementation:
 *  - Win32: that's exactly what Events do :]
 *  - POSIX: the pthread conditions almost do what we need, except the
 *    signal is lost if no thread was waiting (meaning that if the Event
 *    is signaled before a thread ask to wait for it, then the waiting
 *    thread will be blocked.
 *    Solution: wrap the pthread condition system in a class that will
 *    save the state of the Event for us :)
 */

class TOOLS_DLL_API Event {
protected:
#if defined(WIN32)
	HANDLE	m_hEvent;
#elif defined(__POSIX__)
	pthread_cond_t	m_Cond;
	pthread_mutex_t	m_Mutex;

	uint16			m_iWaitingThreads;
	char			m_bPersistent;
	char			m_bSignaled;
#endif

public:
	Event(bool bPersistent=false, bool bSignaled=false);
	~Event(void);

	void	signal(void);
	void	unsignal(void);
	void	broadcast(void);
	bool	check(void);
	bool	wait(void);
};


class TOOLS_DLL_API EventPersistent : public Event {
public:
	EventPersistent(bool bSignaled=false)
		: Event(true, bSignaled) { };
};

//////////////////////////////////////////////////////////////////////
// Thread

#if TOOLS_USETHREAD

typedef enum {
	kThread_Ready		= 0
	,kThread_Running
	,kThread_Suspended
	,kThread_Finished
} eThreadState;

#if defined(WIN32)
#define THREAD_RESULTTYPE	unsigned
#define THREAD_MODIFIER	__stdcall
#elif defined(__POSIX__)
#define THREAD_RESULTTYPE	void *
#define THREAD_MODIFIER
#elif
#error "Please define your threading system"
#endif

class TOOLS_DLL_API Thread {
protected:
#if defined(__POSIX__)
	pthread_t	m_iThreadID;
#elif defined(WIN32)
	HANDLE		m_hThread;
	unsigned	m_iThreadID;
#endif
	CriticalSection	m_Lock;
	CriticalSection	m_WaitLock;
	bool			m_bCodeReturned;
	Semaphore		m_ResumeSemaphore;
	eThreadState	m_eThreadState;
	volatile bool	m_bAbort;
	bool			m_bSuspend;
	THREAD_RESULTTYPE	m_ReturnCode;

	void			Thread_Free(void);
	bool			Thread_Reset(void);				/* Reset values */
	bool			_Thread_Wait(void);				/* Wait for the thread to finish */
	virtual void	*_Thread_Run(void) = 0;			/* The thread main function */
	void			Thread_Exit(void *pResult=0);	/* To end the thread */

	bool			Thread_CheckAbort(void);		/* Check if aborting (or suspension) was asked */

	virtual void	Thread_OnAbort(void) { };		/* Triggered when Thread_Abort is called */
	virtual void	Thread_OnExit(void) { };		/* Triggered just before exiting the thread */

	void			Thread_SetState(eThreadState eState);

public:
	Thread(void);
	virtual ~Thread(void);

	eThreadState	Thread_GetState(void);
	THREAD_RESULTTYPE	Thread_GetReturnCode(void);
	bool			Thread_IsRun(void);
	bool			Thread_IsRunning(void);
	bool			Thread_IsSuspended(void);
	bool			Thread_IsAborted(void);

	virtual bool	Thread_Run(void);		/* Called to launch the thread */
	static THREAD_RESULTTYPE THREAD_MODIFIER	Thread_Run(void *pThread);		/* Launch the thread, calling _RunThread */
	static void		Thread_Sleep(uint64 iMicroSeconds);	/* sleep for a while */
	bool			Thread_Suspend(void);	/* Suspend the thread */
	bool			Thread_Resume(void);	/* Resume the thread */
	void			*Thread_Wait(void);		/* Wait for the thread to finish */
	void			Thread_Abort(void);		/* Kindly ask the thread to finish its job */
	virtual void	Thread_Kill(void);		/* Use as a last resort to kill the thread */
};

#endif /* TOOLS_USETHREAD */

#endif /* __TOOLS_THREAD_H__ */
