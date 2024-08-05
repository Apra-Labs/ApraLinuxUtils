/*
 * Synchronization.cpp
 *
 *  Created on: Jan 31, 2012
 *      Author: dhirvonen
 */

#include <utils/ScopeLock.h>

namespace apra
{
ScopeLock::ScopeLock(Mutex &mutex) :
		m_lock(mutex.get())
{
	pthread_mutex_lock(&m_lock);
}

ScopeLock::ScopeLock(pthread_mutex_t &mutex) :
		m_lock(mutex)
{
	pthread_mutex_lock(&m_lock);
}

ScopeLock::~ScopeLock()
{
	pthread_mutex_unlock(&m_lock);
}
}
