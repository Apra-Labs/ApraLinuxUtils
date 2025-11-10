/*
 * Synchronization.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
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
