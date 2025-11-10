/*
 * ScopeLock.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_SYNCHRONIZATION_H_
#define INCLUDES_APRA_SYNCHRONIZATION_H_

#include <pthread.h>
#include <vector>
#include <deque>
#include "utils/Mutex.h"

namespace apra
{
class ScopeLock
{
public:
	ScopeLock(pthread_mutex_t &mutex);
	ScopeLock(Mutex &mutex);
	~ScopeLock();
protected:
	pthread_mutex_t &m_lock;
};
}

#endif /* INCLUDES_APRA_SYNCHRONIZATION_H_ */
