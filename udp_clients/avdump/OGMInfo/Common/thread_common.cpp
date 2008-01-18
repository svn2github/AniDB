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

#include "thread.h"
#include "common.h"


MutexLocker::MutexLocker(Mutex& mutex)
: m_Mutex(mutex)
{
	++m_Mutex;
}

MutexLocker::~MutexLocker(void) {
	--m_Mutex;
}

//////////////////////////////////////////////////////////////////////

CriticalSectionLocker::CriticalSectionLocker(CriticalSection& CSect)
: m_CSect(CSect)
{
	++m_CSect;
}

CriticalSectionLocker::~CriticalSectionLocker(void) {
	--m_CSect;
}

//////////////////////////////////////////////////////////////////////

SemaphoreLocker::SemaphoreLocker(Semaphore& sem)
: m_Semaphore(sem)
{
	++m_Semaphore;
}

SemaphoreLocker::~SemaphoreLocker(void) {
	--m_Semaphore;
}

//////////////////////////////////////////////////////////////////////

#if TOOLS_USETHREAD

Thread::~Thread(void) {
	Thread_Wait();
}

eThreadState Thread::Thread_GetState(void) {
	return m_eThreadState;
}

void Thread::Thread_SetState(eThreadState eState) {
	m_eThreadState = eState;
}

THREAD_RESULTTYPE Thread::Thread_GetReturnCode(void) {
	return m_ReturnCode;
}

bool Thread::Thread_IsRun(void) {
	CriticalSectionLocker locker(m_Lock);

	return ((Thread_GetState()==kThread_Running) || (Thread_GetState()==kThread_Suspended));
}

bool Thread::Thread_IsRunning(void) {
	CriticalSectionLocker locker(m_Lock);

	return (Thread_GetState() == kThread_Running);
}

bool Thread::Thread_IsSuspended(void) {
	CriticalSectionLocker locker(m_Lock);

	return ((Thread_GetState()==kThread_Suspended) || m_bSuspend);
}

bool Thread::Thread_IsAborted(void) {
	return m_bAbort;
}

void Thread::Thread_Abort(void) {
	CriticalSectionLocker locker(m_Lock);

	m_bAbort = true;
	if(m_eThreadState == kThread_Ready)
		m_eThreadState = kThread_Finished;

	Thread_OnAbort();
}

bool Thread::Thread_Suspend(void) {
	CriticalSectionLocker locker(m_Lock);

	if(m_eThreadState != kThread_Suspended)
		m_bSuspend = true;

	return true;
}

bool Thread::Thread_Resume(void) {
	CriticalSectionLocker locker(m_Lock);

	if(m_eThreadState == kThread_Suspended) {
		--m_ResumeSemaphore;
		m_eThreadState = kThread_Running;
	}
	m_bSuspend = false;

	return true;
}

bool Thread::Thread_CheckAbort(void) {
	CriticalSectionLocker locker(m_Lock);

	if(m_bAbort)
		return true;

	if(m_bSuspend) {
		m_bSuspend = false;
		m_eThreadState = kThread_Suspended;
		--m_Lock;
		++m_ResumeSemaphore;
		++m_Lock;
		return m_bAbort;
	}

	return false;
}

bool Thread::Thread_Reset(void) {
	CriticalSectionLocker locker(m_Lock);

	if((m_eThreadState==kThread_Running) || (m_eThreadState==kThread_Suspended))
		return false;

	Thread_Free();

	m_eThreadState = kThread_Ready;
	m_bCodeReturned = false;
	m_bAbort = false;
	m_bSuspend = false;
	m_ReturnCode = (THREAD_RESULTTYPE)-1;

	return true;
}

THREAD_RESULTTYPE THREAD_MODIFIER Thread::Thread_Run(void *pParam) {
	Thread *pThread = (Thread *)pParam;

	if(!pThread)
		return (THREAD_RESULTTYPE)-1;

	/* Wait for the thread creator to let us start */
	++(pThread->m_ResumeSemaphore);

	if(pThread->Thread_IsAborted()) {
		pThread->Thread_Exit((void *)-1);
		return (THREAD_RESULTTYPE)-1;
	}

	void *pResult = pThread->_Thread_Run();

	/* Exit properly (releasing resources when necessary) */
	pThread->Thread_Exit(pResult);

	return (THREAD_RESULTTYPE)pResult;
}

void *Thread::Thread_Wait(void) {
	CriticalSectionLocker waitLocker(m_WaitLock);

	if(m_bCodeReturned)
		return (void *)m_ReturnCode;

	if(_Thread_Wait()) {
		m_bCodeReturned = true;
		return (void *)m_ReturnCode;
	}

	return (void *)-1;
}

#endif /* TOOLS_USETHREAD */
