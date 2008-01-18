/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Thread tools (Win32)
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

#include "../thread.h"
#include "../common.h"

#if defined(WIN32)

#if TOOLS_USETHREAD
/* _MT must be declared to use _beginthread(ex)/_endthread(ex) */
#if !defined(_MT)
	#define _MT
#endif
#endif

#include <windows.h>
#include <process.h>


Mutex::Mutex(void) {
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}

Mutex::~Mutex(void) {
	if(m_hMutex)
		CloseHandle(&m_hMutex);
}

eMutexResult Mutex::Lock(DWORD dwWaitMilliseconds) {
	switch(WaitForSingleObject(&m_hMutex, dwWaitMilliseconds)) {
	case WAIT_OBJECT_0:
	case WAIT_ABANDONED:
		return kMutex_OK;

	case WAIT_TIMEOUT:
		return kMutex_EBusy;
	}

	return kMutex_EOther;
}

eMutexResult Mutex::operator--(void) {
	if(ReleaseMutex(&m_hMutex))
		return kMutex_OK;

	return kMutex_EPermission;
}

//////////////////////////////////////////////////////////////////////

CriticalSection::CriticalSection(void) {
	InitializeCriticalSection(&m_CSect);
}

CriticalSection::~CriticalSection(void) {
	DeleteCriticalSection(&m_CSect);
}

void CriticalSection::operator++(void) {
	EnterCriticalSection(&m_CSect);
}

void CriticalSection::operator--(void) {
	LeaveCriticalSection(&m_CSect);
}

//////////////////////////////////////////////////////////////////////

Semaphore::Semaphore(uint32 iMaxCount, uint32 iInitialCount) {
	/* A max count of 0 means we don't want to hinder the number
	 * of accesses
	 */
	if(iMaxCount == 0)
		iMaxCount = LONG_MAX;

	if(iInitialCount < iMaxCount)
		iInitialCount = iMaxCount;

	m_hSemaphore = CreateSemaphore(NULL, iInitialCount, iMaxCount, NULL);
}

Semaphore::~Semaphore(void) {
	if(m_hSemaphore)
		CloseHandle(m_hSemaphore);
}

eSemaphoreResult Semaphore::Lock(DWORD dwWaitMilliseconds) {
	switch(WaitForSingleObject(&m_hSemaphore, dwWaitMilliseconds)) {
	case WAIT_OBJECT_0:
		return kSemaphore_OK;

	case WAIT_TIMEOUT:
		return kSemaphore_EBusy;
	}

	return kSemaphore_EOther;
}

eSemaphoreResult Semaphore::operator--(void) {
	if(ReleaseSemaphore(m_hSemaphore, 1, NULL))
		return kSemaphore_OK;

	return kSemaphore_EOther;
}

//////////////////////////////////////////////////////////////////////

Event::Event(bool bPersistent, bool bSignaled) {
	m_hEvent = CreateEvent(NULL, bPersistent, bSignaled, NULL);
}

Event::~Event(void) {
	CloseHandle(m_hEvent);
}

void Event::signal(void) {
	SetEvent(m_hEvent);
}

void Event::unsignal(void) {
	ResetEvent(m_hEvent);
}

void Event::broadcast(void) {
	PulseEvent(m_hEvent);
}

bool Event::wait(void) {
	return (WaitForSingleObject(m_hEvent, INFINITE) == WAIT_OBJECT_0);
}

bool Event::check(void) {
	return (WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0);
}

//////////////////////////////////////////////////////////////////////

#if TOOLS_USETHREAD

Thread::Thread(void)
: m_eThreadState(kThread_Ready)
, m_iThreadID(0)
, m_hThread(NULL)
, m_bAbort(false)
, m_bSuspend(false)
, m_ReturnCode((THREAD_RESULTTYPE)0)
, m_bCodeReturned(false)
{
}

void Thread::Thread_Free(void) {
	if(m_hThread) {
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_iThreadID = 0;
	}
}

bool Thread::Thread_Run(void) {
	if(Thread_IsRun())
		return true;

	if(Thread_GetState() != kThread_Ready)
		return false;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread::Thread_Run, this, 0, &m_iThreadID);

	/* If the thread was created properly, it is waiting for our signal */
	if(m_hThread) {
		--m_ResumeSemaphore;
		return true;
	}

	return false;
}

void Thread::Thread_Sleep(uint64 iMicroSeconds) {
	iMicroSeconds /= 1000;
	if(iMicroSeconds > UINT32_MAX)
		Sleep(INFINITE);
	else
		Sleep(iMicroSeconds);
}

bool Thread::_Thread_Wait(void) {
	HANDLE hThread = NULL;

	{
		CriticalSectionLocker locker(m_Lock);
		hThread = m_hThread;
	}

	if(hThread) {
		if(WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0) {
			m_ReturnCode = (THREAD_RESULTTYPE)-1;
			return false;
		}

		/* The thread should be finished by now
		 * but we never know ...
		 */
		DWORD dwCode = 0;
		do {
			if(!GetExitCodeThread(hThread, &dwCode))
				dwCode = -1;
		} while(dwCode == STILL_ACTIVE);
		{
			CriticalSectionLocker locker(m_Lock);
			Thread_Free();
			m_eThreadState = kThread_Finished;

			assert(m_ReturnCode == (THREAD_RESULTTYPE)dwCode);

			m_ReturnCode = (THREAD_RESULTTYPE)dwCode;
		}
	}
	return (Thread_GetState() == kThread_Finished);
}

void Thread::Thread_Kill(void) {
	HANDLE hThread = NULL;

	if(!Thread_IsRun())
		return;

	{
		CriticalSectionLocker locker(m_Lock);

		hThread = m_hThread;
	}

	if(hThread) {
		if(::TerminateThread(hThread, (DWORD)-1)) {
			CriticalSectionLocker locker(m_Lock);

			Thread_Free();
			m_ReturnCode = (THREAD_RESULTTYPE)(DWORD)-1;
			m_eThreadState = kThread_Finished;
		}
	}
}

void Thread::Thread_Exit(void *pResult) {
	/* Free the thread handle before exiting */
	{
		CriticalSectionLocker locker(m_Lock);
		Thread_Free();
		m_ReturnCode = (THREAD_RESULTTYPE)pResult;
		m_eThreadState = kThread_Finished;
	}
	Thread_OnExit();
	_endthreadex((THREAD_RESULTTYPE)pResult);
}

#endif /* TOOLS_USETHREAD */

#endif /* Win32 */
