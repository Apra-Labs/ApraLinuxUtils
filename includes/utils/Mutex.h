/*
 * Mutex.h
 *
 *  Created on: Jun 17, 2024
 *      Author: developer
 */

#ifndef SRC_UTILS_MUTEX_H_
#define SRC_UTILS_MUTEX_H_
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

#endif /* SRC_UTILS_MUTEX_H_ */
