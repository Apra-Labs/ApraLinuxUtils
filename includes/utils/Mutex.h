/*
 * Mutex.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef SRC_APRA_UTILS_MUTEX_H_
#define SRC_APRA_UTILS_MUTEX_H_
#include <pthread.h>

namespace apra
{
class Mutex
{
public:
	friend class ScopeLock;
	Mutex();
	~Mutex();
	void lock();
	void unlock();
protected:
	pthread_mutex_t& get();
	pthread_mutex_t m_mutex;
};
} /* namespace apra */

#endif /* SRC_APRA_UTILS_MUTEX_H_ */
